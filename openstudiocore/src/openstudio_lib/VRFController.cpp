/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "VRFController.hpp"
#include "VRFGraphicsItems.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "ModelObjectItem.hpp"
#include "OSDropZone.hpp"
#include "MainWindow.hpp"
#include "MainRightColumnController.hpp"
#include "../model/Model.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"
#include "../utilities/core/Compare.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>

namespace openstudio {

VRFController::VRFController()
  : QObject(),
    m_detailView(nullptr),
    m_dirty(false)
{
  m_currentSystem = boost::none;

  m_vrfView = new VRFView();
  connect(m_vrfView->zoomOutButton, &QPushButton::clicked, this, &VRFController::zoomOutToSystemGridView);

  m_vrfSystemGridView = new GridLayoutItem();
  m_vrfSystemGridView->setCellSize(VRFSystemMiniView::cellSize());
  m_vrfGridScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());
  m_vrfGridScene->addItem(m_vrfSystemGridView);

  m_vrfSystemListController = QSharedPointer<VRFSystemListController>(new VRFSystemListController(this));
  m_vrfSystemGridView->setListController(m_vrfSystemListController);
  m_vrfSystemGridView->setDelegate(QSharedPointer<VRFSystemItemDelegate>(new VRFSystemItemDelegate()));

  zoomOutToSystemGridView();
}

VRFController::~VRFController()
{
  delete m_vrfView;
}

VRFView * VRFController::vrfView() const
{
  return m_vrfView;
}

QSharedPointer<VRFSystemListController> VRFController::vrfSystemListController() const
{
  return m_vrfSystemListController;
}

void VRFController::refresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refreshNow()));
}

void VRFController::refreshNow()
{
  if( ! m_dirty ) return;

  if( m_detailView )
  {
    m_detailView->setId(OSItemId());
    m_detailView->removeAllVRFTerminalViews();

    if( m_currentSystem )
    {
      m_detailView->setId(OSItemId(m_currentSystem->handle().toString(),modelToSourceId(m_currentSystem->model()),false));

      connect(m_detailView.data(), &VRFSystemView::inspectClicked, this, &VRFController::inspectOSItem);

      std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = m_currentSystem->terminals();
      for(auto it = terminals.begin();
          it != terminals.end();
          ++it)
      {
        auto vrfTerminalView = new VRFTerminalView();
        vrfTerminalView->setId(OSItemId(it->handle().toString(),modelToSourceId(it->model()),false));
        m_detailView->addVRFTerminalView(vrfTerminalView);
        connect(vrfTerminalView, &VRFTerminalView::componentDroppedOnZone, this, &VRFController::onVRFTerminalViewDrop);
        connect(vrfTerminalView, &VRFTerminalView::removeZoneClicked, this, &VRFController::onRemoveZoneClicked);
        connect(vrfTerminalView, &VRFTerminalView::removeTerminalClicked, this, &VRFController::onRemoveTerminalClicked);
        connect(vrfTerminalView, &VRFTerminalView::terminalIconClicked, this, &VRFController::inspectOSItem);
        if( boost::optional<model::ThermalZone> zone = it->thermalZone() )
        {
          vrfTerminalView->zoneDropZone->setHasZone(true);
          vrfTerminalView->removeZoneButtonItem->setVisible(true);
          QString zoneName = QString::fromStdString(zone->name().get());
          vrfTerminalView->zoneDropZone->setText(zoneName);
          vrfTerminalView->zoneDropZone->setToolTip(zoneName);
        }
      }
    }
  }

  m_dirty = false;
}

void VRFController::onVRFSystemViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);
    OS_ASSERT(mo); 

    if( boost::optional<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminal 
          = mo->optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>() )
    {
      model::ZoneHVACTerminalUnitVariableRefrigerantFlow terminalClone = 
        terminal->clone(m_currentSystem->model()).cast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
      m_currentSystem->addTerminal(terminalClone);

      refresh();
    }
  } else {
    if( auto component = doc->getComponent(itemid) ) {
      if( auto terminal = component->primaryObject().optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>() ) {
        // Ugly hack to avoid the component being treated as a resource.
        component->componentData().setString(OS_ComponentDataFields::UUID,createUUID().toString().toStdString());
        std::cout << component->componentData().getString(OS_ComponentDataFields::UUID) << std::endl;;
        if( auto componentData = m_currentSystem->model().insertComponent(component.get()) ) {
          terminal = componentData->primaryComponentObject().optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
          OS_ASSERT(terminal);
          m_currentSystem->addTerminal(terminal.get());

          refresh();
        }
      }
    }
  }
}

void VRFController::onVRFSystemViewZoneDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromModel(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);
    OS_ASSERT(mo); 
    if( boost::optional<model::ThermalZone> thermalZone = mo->optionalCast<model::ThermalZone>() )
    {
      std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = m_currentSystem->terminals();
      if(terminals.empty())
      {
        QMessageBox message(m_vrfView);
        message.setText("You must add a VRF terminal before assigning zones.");
        message.exec();
      }
      else
      {
        model::ZoneHVACTerminalUnitVariableRefrigerantFlow terminal = terminals.back();
        model::Model t_model = m_currentSystem->model();
        model::ZoneHVACTerminalUnitVariableRefrigerantFlow terminalClone = terminal.clone(t_model).cast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
        m_currentSystem->addTerminal(terminalClone);
        terminalClone.addToThermalZone(thermalZone.get());

        refresh();
      }
    }
  }
}

void VRFController::onVRFTerminalViewDrop(const OSItemId & terminalId, const OSItemId & thermalZoneId)
{
  OS_ASSERT(m_currentSystem);
  auto doc = OSAppBase::instance()->currentDocument();

  if( doc->fromModel(thermalZoneId) )
  {
    auto mo = doc->getModelObject(thermalZoneId);
    OS_ASSERT(mo); 
    if( auto thermalZone = mo->optionalCast<model::ThermalZone>() )
    {
      if( (mo = doc->getModelObject(terminalId)) ) {
        if( auto terminal = mo->optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>() ) {
          terminal->addToThermalZone(thermalZone.get());

          refresh();
        }
      } else {
        if( auto component = doc->getComponent(terminalId) ) {
          if( auto terminal = component->primaryObject().optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>() ) {
            // Ugly hack to avoid the component being treated as a resource.
            component->componentData().setString(OS_ComponentDataFields::UUID,createUUID().toString().toStdString());
            if( auto componentData = m_currentSystem->model().insertComponent(component.get()) ) {
              terminal = componentData->primaryComponentObject().optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
              OS_ASSERT(terminal);
              terminal->addToThermalZone(thermalZone.get());

              refresh();
            }
          }
        }
      }
    }
  }
}

void VRFController::onRemoveZoneClicked(const OSItemId & terminalId)
{
  OS_ASSERT(m_currentSystem);
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  boost::optional<model::ModelObject> mo = doc->getModelObject(terminalId);
  OS_ASSERT(mo);
  boost::optional<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminal = mo->optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
  OS_ASSERT(terminal);
  terminal->removeFromThermalZone();

  refresh();
}

void VRFController::onRemoveTerminalClicked(const OSItemId & terminalId)
{
  OS_ASSERT(m_currentSystem);
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  boost::optional<model::ModelObject> mo = doc->getModelObject(terminalId);
  OS_ASSERT(mo);
  boost::optional<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminal = mo->optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
  OS_ASSERT(terminal);
  terminal->remove();

  refresh();
}

void VRFController::zoomInOnSystem(model::AirConditionerVariableRefrigerantFlow & system)
{
  m_currentSystem = system;

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  model::OptionalModelObject mo;
  doc->mainRightColumnController()->inspectModelObject(mo,false);

  m_detailScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());
  m_detailView = new VRFSystemView();
  connect(m_detailView->terminalDropZone, &OSDropZoneItem::componentDropped, this, &VRFController::onVRFSystemViewDrop);
  connect(m_detailView->zoneDropZone, &OSDropZoneItem::componentDropped, this, &VRFController::onVRFSystemViewZoneDrop);
  m_detailScene->addItem(m_detailView);
  m_vrfView->header->show();
  m_vrfView->graphicsView->setScene(m_detailScene.data());
  m_vrfView->graphicsView->setAlignment(Qt::AlignCenter);

  refresh();
}

void VRFController::zoomOutToSystemGridView()
{
  m_currentSystem = boost::none;

  model::OptionalModelObject mo;
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  doc->mainRightColumnController()->inspectModelObject(mo,false);

  m_vrfSystemListController->reset();

  m_vrfView->graphicsView->setScene(m_vrfGridScene.data());
  m_vrfView->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_vrfView->header->hide();

  refresh();
}

void VRFController::inspectOSItem(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);
  boost::optional<model::ModelObject> mo = m_currentSystem->model().getModelObject<model::ModelObject>(Handle(itemid.itemId()));

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  OS_ASSERT(doc);
  doc->mainRightColumnController()->inspectModelObject(mo,false);
}

VRFSystemListController::VRFSystemListController(VRFController * vrfController)
  : m_vrfController(vrfController)
{
}

void VRFSystemListController::reset()
{
  emit modelReset();
}

VRFController * VRFSystemListController::vrfController() const
{
  return m_vrfController;
}

QSharedPointer<OSListItem> VRFSystemListController::itemAt(int i)
{
  QSharedPointer<OSListItem> item;

  if( i == 0 )
  {
    item = QSharedPointer<VRFSystemListDropZoneItem>(new VRFSystemListDropZoneItem(this));
  }
  else if( i > 0 && i < count() )
  {
    item = QSharedPointer<VRFSystemListItem>(new VRFSystemListItem(systems()[i - 1],this));
  }

  return item;
}

int VRFSystemListController::count()
{
  return (int)(systems().size() + 1);
}

int VRFSystemListController::systemIndex(const model::AirConditionerVariableRefrigerantFlow & system) const
{
  std::vector<model::AirConditionerVariableRefrigerantFlow> _systems = systems();

  int i = 1;

  for( std::vector<model::AirConditionerVariableRefrigerantFlow>::const_iterator it = _systems.begin();
       it != _systems.end();
       ++it )
  {
    if( *it == system )
    {
      break;
    }

    i++;
  }

  return i;
}

void VRFSystemListController::createNewSystem()
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    model::AirConditionerVariableRefrigerantFlow system(model.get());

    emit itemInserted(systemIndex(system));
  }
}

void VRFSystemListController::addSystem(const OSItemId & itemid)
{
  auto doc = OSAppBase::instance()->currentDocument();
  auto model = OSAppBase::instance()->currentModel();

  if( ! model ) return;

  if( doc->fromComponentLibrary(itemid) ) {
    auto mo = doc->getModelObject(itemid);

    if( mo ) {
      if( auto system = mo->optionalCast<model::AirConditionerVariableRefrigerantFlow>() ) {
        auto systemClone = system->clone(model.get()).cast<model::AirConditionerVariableRefrigerantFlow>();
        emit itemInserted(systemIndex(systemClone));
      }
    }
  } else {
    if( auto component = doc->getComponent(itemid) ) {
      if( auto system = component->primaryObject().optionalCast<model::AirConditionerVariableRefrigerantFlow>() ) {
        // Ugly hack to avoid the component being treated as a resource.
        component->componentData().setString(OS_ComponentDataFields::UUID,createUUID().toString().toStdString());
        if( auto componentData = model->insertComponent(component.get()) ) {
          system = componentData->primaryComponentObject().optionalCast<model::AirConditionerVariableRefrigerantFlow>();
          OS_ASSERT(system);
          emit itemInserted(systemIndex(system.get()));
        }
      }
    }
  }
}

void VRFSystemListController::removeSystem(model::AirConditionerVariableRefrigerantFlow & vrfSystem)
{
  std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = vrfSystem.terminals();
  if( ! terminals.empty() )
  {
    QMessageBox message(m_vrfController->vrfView());
    
    QString text;
    int size = (int)terminals.size();
    if( size == 1 )
    {
      text.append("There is ");
      text.append(QString::number(size));
      text.append( " terminal ");
    }
    else
    {
      text.append("There are ");
      text.append(QString::number(size));
      text.append( " terminals ");
    }
    text.append("attached to this system which will be removed if you continue.");
    message.setText(text);
    message.addButton(QMessageBox::Cancel);
    message.addButton(QMessageBox::Ok);
    int response = message.exec();

    if( response != QMessageBox::Ok )
    {
      return;
    }
  }

  int i = systemIndex(vrfSystem);

  for(auto it = terminals.begin();
      it != terminals.end();
      ++it)
  {
    it->remove();
  }
  vrfSystem.remove();

  emit itemRemoved(i);
}

void VRFSystemListItem::zoomInOnSystem()
{
  qobject_cast<VRFSystemListController *>(controller())->vrfController()->zoomInOnSystem(m_vrfSystem);
}

std::vector<model::AirConditionerVariableRefrigerantFlow> VRFSystemListController::systems() const
{
  std::vector<model::AirConditionerVariableRefrigerantFlow> result;

  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    result = model->getModelObjects<model::AirConditionerVariableRefrigerantFlow>();
  }

  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());

  return result;
}

VRFSystemListItem::VRFSystemListItem(const model::AirConditionerVariableRefrigerantFlow & vrfSystem,OSListController * listController)
  : OSListItem(listController),
    m_vrfSystem(vrfSystem)
{
}

QString VRFSystemListItem::systemName() const
{
  return QString::fromStdString(m_vrfSystem.name().get());
}

int VRFSystemListItem::numberOfConnectedZones() const
{
  int result = 0;
  std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals;
  terminals = m_vrfSystem.terminals();
  for(auto it = terminals.begin();
      it != terminals.end();
      ++it)
  {
    if( it->thermalZone() )
    {
      result++;
    }
  }
  return result;
}

int VRFSystemListItem::numberOfConnectedTerminals() const
{
  return (int)m_vrfSystem.terminals().size();
}

void VRFSystemListItem::remove()
{
  qobject_cast<VRFSystemListController *>(controller())->removeSystem(m_vrfSystem);
}

VRFSystemListDropZoneItem::VRFSystemListDropZoneItem(OSListController * listController)
  : OSListItem(listController)
{
}

QGraphicsObject * VRFSystemItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QGraphicsObject * itemView = nullptr;

  if( QSharedPointer<VRFSystemListItem> listItem = dataSource.dynamicCast<VRFSystemListItem>() )
  {
    auto vrfSystemMiniView = new VRFSystemMiniView();

    connect(vrfSystemMiniView->removeButtonItem, &RemoveButtonItem::mouseClicked, listItem.data(), &VRFSystemListItem::remove);

    connect(vrfSystemMiniView->zoomInButtonItem, &ZoomInButtonItem::mouseClicked, listItem.data(), &VRFSystemListItem::zoomInOnSystem);
    
    vrfSystemMiniView->setName(listItem->systemName());
    vrfSystemMiniView->setNumberOfZones(listItem->numberOfConnectedZones());
    vrfSystemMiniView->setNumberOfTerminals(listItem->numberOfConnectedTerminals());

    itemView = vrfSystemMiniView;
  }
  else if( dataSource.dynamicCast<VRFSystemListDropZoneItem>() )
  {
    auto vrfSystemDropZoneView = new VRFSystemDropZoneView();

    connect(vrfSystemDropZoneView, &VRFSystemDropZoneView::componentDropped,
      qobject_cast<VRFSystemListController *>(dataSource->controller()), &VRFSystemListController::addSystem);

    itemView = vrfSystemDropZoneView;
  }

  return itemView;
}

} // openstudio

