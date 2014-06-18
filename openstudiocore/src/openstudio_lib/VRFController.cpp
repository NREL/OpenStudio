/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#include "../utilities/core/Compare.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>

namespace openstudio {

VRFController::VRFController()
  : QObject(),
    m_detailView(0),
    m_dirty(false)
{
  m_currentSystem = boost::none;

  m_vrfView = new VRFView();
  bool bingo;
  bingo = connect(m_vrfView->zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOutToSystemGridView()));
  OS_ASSERT(bingo);

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

      bool bingo;
      bingo = connect(m_detailView,SIGNAL(inspectClicked(const OSItemId &)),
                      this,SLOT(inspectOSItem(const OSItemId &)));
      OS_ASSERT(bingo);

      std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = m_currentSystem->terminals();
      for(std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>::iterator it = terminals.begin();
          it != terminals.end();
          ++it)
      {
        VRFTerminalView * vrfTerminalView = new VRFTerminalView();
        vrfTerminalView->setId(OSItemId(it->handle().toString(),modelToSourceId(it->model()),false));
        m_detailView->addVRFTerminalView(vrfTerminalView);
        bingo = connect(vrfTerminalView,SIGNAL(componentDroppedOnZone(const OSItemId &, const OSItemId &)),
                        this,SLOT(onVRFTerminalViewDrop(const OSItemId &, const OSItemId &)));
        OS_ASSERT(bingo);
        bingo = connect(vrfTerminalView,SIGNAL(removeZoneClicked(const OSItemId &)),this,SLOT(onRemoveZoneClicked(const OSItemId &)));
        OS_ASSERT(bingo);
        bingo = connect(vrfTerminalView,SIGNAL(removeTerminalClicked(const OSItemId &)),this,SLOT(onRemoveTerminalClicked(const OSItemId &)));
        OS_ASSERT(bingo);
        bingo = connect(vrfTerminalView,SIGNAL(terminalIconClicked(const OSItemId &)),this,SLOT(inspectOSItem(const OSItemId &)));
        OS_ASSERT(bingo);
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
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromModel(thermalZoneId) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(thermalZoneId);
    OS_ASSERT(mo); 
    if( boost::optional<model::ThermalZone> thermalZone = mo->optionalCast<model::ThermalZone>() )
    {
      mo = doc->getModelObject(terminalId);
      OS_ASSERT(mo);
      boost::optional<model::ZoneHVACTerminalUnitVariableRefrigerantFlow> terminal = mo->optionalCast<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>();
      OS_ASSERT(terminal);
      terminal->addToThermalZone(thermalZone.get());

      refresh();
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
  bool bingo = connect(m_detailView->terminalDropZone,SIGNAL(componentDropped(const OSItemId &)),this,SLOT(onVRFSystemViewDrop(const OSItemId &)));
  OS_ASSERT(bingo);
  bingo = connect(m_detailView->zoneDropZone,SIGNAL(componentDropped(const OSItemId &)),this,SLOT(onVRFSystemViewZoneDrop(const OSItemId &)));
  OS_ASSERT(bingo);
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
  return systems().size() + 1;
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
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    boost::optional<model::Model> model = OSAppBase::instance()->currentModel();

    if( mo && model )
    {
      if( boost::optional<model::AirConditionerVariableRefrigerantFlow> system = mo->optionalCast<model::AirConditionerVariableRefrigerantFlow>() )
      {
        model::AirConditionerVariableRefrigerantFlow systemClone = 
          system->clone(model.get()).cast<model::AirConditionerVariableRefrigerantFlow>();

        emit itemInserted(systemIndex(systemClone));
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
    int size = terminals.size();
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

  for(std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>::iterator it = terminals.begin();
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
  for(std::vector<model::ZoneHVACTerminalUnitVariableRefrigerantFlow>::iterator it = terminals.begin();
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
  QGraphicsObject * itemView = NULL;

  if( QSharedPointer<VRFSystemListItem> listItem = dataSource.dynamicCast<VRFSystemListItem>() )
  {
    VRFSystemMiniView * vrfSystemMiniView = new VRFSystemMiniView();

    bool bingo;
    bingo = connect(vrfSystemMiniView->removeButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(remove()));
    OS_ASSERT(bingo);

    bingo = connect(vrfSystemMiniView->zoomInButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(zoomInOnSystem()));
    OS_ASSERT(bingo);

    vrfSystemMiniView->setName(listItem->systemName());
    vrfSystemMiniView->setNumberOfZones(listItem->numberOfConnectedZones());
    vrfSystemMiniView->setNumberOfTerminals(listItem->numberOfConnectedTerminals());

    itemView = vrfSystemMiniView;
  }
  else if( dataSource.dynamicCast<VRFSystemListDropZoneItem>() )
  {
    VRFSystemDropZoneView * vrfSystemDropZoneView = new VRFSystemDropZoneView();

    bool bingo;
    bingo = connect(vrfSystemDropZoneView,SIGNAL(componentDropped(const OSItemId &)),
                    qobject_cast<VRFSystemListController *>(dataSource->controller()),SLOT(addSystem(const OSItemId &)));
    OS_ASSERT(bingo);

    itemView = vrfSystemDropZoneView;
  }

  return itemView;
}

} // openstudio

