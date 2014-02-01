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
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "MainWindow.hpp"
#include "MainRightColumnController.hpp"
#include "../model/Model.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../utilities/core/Compare.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>

namespace openstudio {

VRFController::VRFController()
  : QObject()
{
  m_vrfGridScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());

  //bool bingo;
  //bingo = connect(m_refrigerationView->zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOutToSystemGridView()));
  //OS_ASSERT(bingo);

  // These get deleted with when the scene is deleted
  m_vrfView = new QGraphicsView();

  m_vrfSystemGridView = new GridLayoutItem();

  m_vrfSystemListController = QSharedPointer<VRFSystemListController>(new VRFSystemListController(this));

  m_vrfSystemGridView->setListController(m_vrfSystemListController);

  m_vrfSystemGridView->setDelegate(QSharedPointer<VRFSystemItemDelegate>(new VRFSystemItemDelegate()));

  m_vrfGridScene->addItem(m_vrfSystemGridView);
}

VRFController::~VRFController()
{
  delete m_vrfView;

  delete m_vrfSystemGridView;

  //if( m_refrigerationView )
  //{
  //  delete m_refrigerationView;
  //}

  //// This is for completeness, but will be taken care of when the scene is deleted.
  //if( m_refrigerationSystemGridView )
  //{
  //  delete m_refrigerationSystemGridView;
  //}

  //// This is for completeness, but will be taken care of when the scene is deleted.
  //if( m_detailView )
  //{
  //  delete m_detailView;
  //}
}

QWidget * VRFController::vrfView() const
{
  return m_vrfView;
}

QSharedPointer<VRFSystemListController> VRFController::vrfSystemListController() const
{
  return m_vrfSystemListController;
}

VRFSystemListController::VRFSystemListController(VRFController * vrfController)
  : m_vrfController(vrfController)
{
}

VRFController * VRFSystemListController::vrfController() const
{
  return m_vrfController;
}

QSharedPointer<OSListItem> VRFSystemListController::itemAt(int i)
{
  QSharedPointer<OSListItem> item;

  //if( i == 0 )
  //{
  //  item = QSharedPointer<VRFSystemListDropZoneItem>(new RefrigerationSystemListDropZoneItem(this));
  //}
  //else if( i > 0 && i < count() )
  //{
  //  item = QSharedPointer<RefrigerationSystemListItem>(new RefrigerationSystemListItem(systems()[i - 1],this));
  //}

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
  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

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
  int i = systemIndex(vrfSystem);

  vrfSystem.remove();

  emit itemRemoved(i);
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
  //  RefrigerationSystemMiniView * refrigerationSystemMiniView = new RefrigerationSystemMiniView();

  //  bool bingo;
  //  bingo = connect(refrigerationSystemMiniView->removeButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(remove()));
  //  OS_ASSERT(bingo);

  //  bingo = connect(refrigerationSystemMiniView->zoomInButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(zoomInOnSystem()));
  //  OS_ASSERT(bingo);

  //  refrigerationSystemMiniView->setName(listItem->systemName());

  //  itemView = refrigerationSystemMiniView;
  }
  else if( dataSource.dynamicCast<VRFSystemListDropZoneItem>() )
  {
  //  RefrigerationSystemDropZoneView * refrigerationSystemDropZoneView = new RefrigerationSystemDropZoneView();

  //  bool bingo;
  //  bingo = connect(refrigerationSystemDropZoneView,SIGNAL(componentDropped(const OSItemId &)),
  //                  qobject_cast<RefrigerationSystemListController *>(dataSource->controller()),SLOT(addSystem(const OSItemId &)));
  //  OS_ASSERT(bingo);

  //  itemView = refrigerationSystemDropZoneView;
  }

  return itemView;
}

} // openstudio

