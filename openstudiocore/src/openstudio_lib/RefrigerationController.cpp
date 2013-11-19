/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "RefrigerationController.hpp"
#include "RefrigerationGraphicsItems.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "../model/Model.hpp"
#include "../model/RefrigerationSystem.hpp"
#include "../model/RefrigerationSystem_Impl.hpp"
#include "../model/RefrigerationCondenserAirCooled.hpp"
#include "../model/RefrigerationCondenserAirCooled_Impl.hpp"
#include "../model/RefrigerationCompressor.hpp"
#include "../model/RefrigerationCompressor_Impl.hpp"
#include "../model/RefrigerationCase.hpp"
#include "../model/RefrigerationCase_Impl.hpp"
#include "../utilities/core/Compare.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

namespace openstudio {

RefrigerationController::RefrigerationController()
  : QObject()
{
  m_refrigerationGridScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());

  m_refrigerationGraphicsView = new QGraphicsView();

  // These get deleted with when the scene is deleted
  m_refrigerationSystemGridView = new RefrigerationSystemGridView();

  m_refrigerationSystemListController = QSharedPointer<RefrigerationSystemListController>(new RefrigerationSystemListController(this));

  m_refrigerationSystemGridView->setListController(m_refrigerationSystemListController);

  m_refrigerationSystemGridView->setDelegate(QSharedPointer<RefrigerationSystemItemDelegate>(new RefrigerationSystemItemDelegate()));

  m_refrigerationGridScene->addItem(m_refrigerationSystemGridView);

  m_refrigerationGraphicsView->setScene(m_refrigerationGridScene.data());

  refresh();
}

RefrigerationController::~RefrigerationController()
{
  if( m_refrigerationGraphicsView )
  {
    delete m_refrigerationGraphicsView;
  }

  // This is for completeness, but will be taken care of when the scene is deleted.
  if( m_refrigerationSystemGridView )
  {
    delete m_refrigerationSystemGridView;
  }

  // This is for completeness, but will be taken care of when the scene is deleted.
  if( m_detailView )
  {
    delete m_detailView;
  }
}

void RefrigerationController::zoomInOnSystem(model::RefrigerationSystem & refrigerationSystem)
{
  m_currentSystem = refrigerationSystem;

  m_refrigerationScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());

  // These get deleted with when the scene is deleted
  m_detailView = new RefrigerationSystemDetailView();

  refresh();

  m_refrigerationScene->addItem(m_detailView);

  m_refrigerationGraphicsView->setScene(m_refrigerationScene.data());

  bool bingo;
  bingo = connect(m_detailView->zoomOutButton,SIGNAL(mouseClicked()),this,SLOT(zoomOutToSystemGridView()));
  OS_ASSERT(bingo);

  bingo = connect(m_detailView->refrigerationSystemView->refrigerationCondenserView,SIGNAL(componentDropped(const OSItemId &)),
                  this,SLOT(onCondenserViewDrop(const OSItemId &)));
  OS_ASSERT(bingo);

  bingo = connect(m_detailView->refrigerationSystemView->refrigerationCompressorView->refrigerationCompressorDropZoneView,SIGNAL(componentDropped(const OSItemId &)),
                  this,SLOT(onCompressorViewDrop(const OSItemId &)));
  OS_ASSERT(bingo);

  bingo = connect(m_detailView->refrigerationSystemView->refrigerationCasesView->refrigerationCasesDropZoneView,SIGNAL(componentDropped(const OSItemId &)),
                  this,SLOT(onCasesViewDrop(const OSItemId &)));
  OS_ASSERT(bingo);
}

void RefrigerationController::zoomOutToSystemGridView()
{
  m_currentSystem = boost::none;

  refresh();

  m_refrigerationGraphicsView->setScene(m_refrigerationGridScene.data());
}

void RefrigerationController::onCondenserViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationCondenserAirCooled> condenser 
          = mo->optionalCast<model::RefrigerationCondenserAirCooled>() )
    {
      model::RefrigerationCondenserAirCooled condenserClone = 
        condenser->clone(m_currentSystem->model()).cast<model::RefrigerationCondenserAirCooled>();

      m_currentSystem->setRefrigerationCondenser(condenserClone);

      refresh();
    }
  }
}

void RefrigerationController::onCompressorViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationCompressor> compressor 
          = mo->optionalCast<model::RefrigerationCompressor>() )
    {
      model::RefrigerationCompressor compressorClone = 
        compressor->clone(m_currentSystem->model()).cast<model::RefrigerationCompressor>();

      m_currentSystem->addCompressor(compressorClone);

      refresh();
    }
  }
}

void RefrigerationController::onCasesViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationCase> _case 
          = mo->optionalCast<model::RefrigerationCase>() )
    {
      model::RefrigerationCase caseClone = 
        _case->clone(m_currentSystem->model()).cast<model::RefrigerationCase>();

      m_currentSystem->addCase(caseClone);

      refresh();
    }
  }
}

void RefrigerationController::refresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refreshNow()));
}

void RefrigerationController::refreshNow()
{
  if( ! m_dirty ) return;

  if( m_detailView )
  {
    m_detailView->refrigerationSystemView->refrigerationCondenserView->setEmpty(true);

    if( m_currentSystem )
    {
      if( m_currentSystem->refrigerationCondenser() )
      {
        m_detailView->refrigerationSystemView->refrigerationCondenserView->setEmpty(false);
      }

      m_detailView->refrigerationSystemView->refrigerationCompressorView->setNumberOfCompressors(m_currentSystem->compressors().size());

      m_detailView->refrigerationSystemView->refrigerationCasesView->setNumberOfDisplayCases(m_currentSystem->cases().size());

      m_detailView->refrigerationSystemView->adjustLayout();
    }
  }
  
  m_dirty = false;
}

QGraphicsView * RefrigerationController::refrigerationGraphicsView() const
{
  return m_refrigerationGraphicsView;
}

QSharedPointer<RefrigerationSystemListController> RefrigerationController::refrigerationSystemListController() const
{
  return m_refrigerationSystemListController;
}

RefrigerationSystemListController::RefrigerationSystemListController(RefrigerationController * refrigerationController)
  : m_refrigerationController(refrigerationController)
{
}

RefrigerationController * RefrigerationSystemListController::refrigerationController() const
{
  return m_refrigerationController;
}

QSharedPointer<OSListItem> RefrigerationSystemListController::itemAt(int i)
{
  QSharedPointer<OSListItem> item;

  if( i == 0 )
  {
    item = QSharedPointer<RefrigerationSystemListDropZoneItem>(new RefrigerationSystemListDropZoneItem(this));
  }
  else if( i > 0 && i < count() )
  {
    item = QSharedPointer<RefrigerationSystemListItem>(new RefrigerationSystemListItem(systems()[i - 1],this));
  }

  return item;
}

int RefrigerationSystemListController::count()
{
  return systems().size() + 1;
}

int RefrigerationSystemListController::systemIndex(const model::RefrigerationSystem & system) const
{
  std::vector<model::RefrigerationSystem> _systems = systems();

  int i = 1;

  for( std::vector<model::RefrigerationSystem>::const_iterator it = _systems.begin();
       it != _systems.end();
       it++ )
  {
    if( *it == system )
    {
      break;
    }

    i++;
  }

  return i;
}

void RefrigerationSystemListController::createNewSystem()
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    model::RefrigerationSystem system(model.get());

    emit itemInserted(systemIndex(system));
  }
}

void RefrigerationSystemListController::removeSystem(model::RefrigerationSystem & refrigerationSystem)
{
  int i = systemIndex(refrigerationSystem);

  refrigerationSystem.remove();

  emit itemRemoved(i);
}

std::vector<model::RefrigerationSystem> RefrigerationSystemListController::systems() const
{
  std::vector<model::RefrigerationSystem> result;

  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    result = model->getModelObjects<model::RefrigerationSystem>();
  }

  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());

  return result;
}

RefrigerationSystemListItem::RefrigerationSystemListItem(const model::RefrigerationSystem & refrigerationSystem,OSListController * listController)
  : OSListItem(listController),
    m_refrigerationSystem(refrigerationSystem)
{
}

QString RefrigerationSystemListItem::systemName() const
{
  return QString::fromStdString(m_refrigerationSystem.name().get());
}

void RefrigerationSystemListItem::remove()
{
  qobject_cast<RefrigerationSystemListController *>(controller())->removeSystem(m_refrigerationSystem);
}

void RefrigerationSystemListItem::zoomInOnSystem()
{
  qobject_cast<RefrigerationSystemListController *>(controller())->refrigerationController()->zoomInOnSystem(m_refrigerationSystem);
}

RefrigerationSystemListDropZoneItem::RefrigerationSystemListDropZoneItem(OSListController * listController)
  : OSListItem(listController)
{
}

QGraphicsObject * RefrigerationSystemItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QGraphicsObject * itemView = NULL;

  if( QSharedPointer<RefrigerationSystemListItem> listItem = dataSource.dynamicCast<RefrigerationSystemListItem>() )
  {
    RefrigerationSystemMiniView * refrigerationSystemMiniView = new RefrigerationSystemMiniView();

    bool bingo;
    bingo = connect(refrigerationSystemMiniView->removeButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(remove()));
    OS_ASSERT(bingo);

    bingo = connect(refrigerationSystemMiniView->zoomInButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(zoomInOnSystem()));
    OS_ASSERT(bingo);

    refrigerationSystemMiniView->setName(listItem->systemName());

    itemView = refrigerationSystemMiniView;
  }
  else if( dataSource.dynamicCast<RefrigerationSystemListDropZoneItem>() )
  {
    RefrigerationSystemDropZoneView * refrigerationSystemDropZoneView = new RefrigerationSystemDropZoneView();

    bool bingo;
    bingo = connect(refrigerationSystemDropZoneView,SIGNAL(mouseClicked()),
                    qobject_cast<RefrigerationSystemListController *>(dataSource->controller()),SLOT(createNewSystem()));
    OS_ASSERT(bingo);

    itemView = refrigerationSystemDropZoneView;
  }

  return itemView;
}

} // openstudio

