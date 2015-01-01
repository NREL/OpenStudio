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

#include "RefrigerationController.hpp"
#include "RefrigerationGraphicsItems.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "MainWindow.hpp"
#include "MainRightColumnController.hpp"
#include "IconLibrary.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RefrigerationSystem.hpp"
#include "../model/RefrigerationSystem_Impl.hpp"
#include "../model/RefrigerationCondenserAirCooled.hpp"
#include "../model/RefrigerationCondenserAirCooled_Impl.hpp"
#include "../model/RefrigerationCondenserCascade.hpp"
#include "../model/RefrigerationCondenserCascade_Impl.hpp"
#include "../model/RefrigerationCondenserEvaporativeCooled.hpp"
#include "../model/RefrigerationCondenserEvaporativeCooled_Impl.hpp"
#include "../model/RefrigerationCondenserWaterCooled.hpp"
#include "../model/RefrigerationCondenserWaterCooled_Impl.hpp"
#include "../model/RefrigerationCompressor.hpp"
#include "../model/RefrigerationCompressor_Impl.hpp"
#include "../model/RefrigerationCase.hpp"
#include "../model/RefrigerationCase_Impl.hpp"
#include "../model/RefrigerationWalkIn.hpp"
#include "../model/RefrigerationWalkIn_Impl.hpp"
#include "../model/RefrigerationSubcoolerMechanical.hpp"
#include "../model/RefrigerationSubcoolerMechanical_Impl.hpp"
#include "../model/RefrigerationSubcoolerLiquidSuction.hpp"
#include "../model/RefrigerationSubcoolerLiquidSuction_Impl.hpp"
#include "../utilities/core/Compare.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>

namespace openstudio {

void RefrigerationController::refreshRefrigerationSystemView(RefrigerationSystemView * systemView, boost::optional<model::RefrigerationSystem> & system)
{
  OS_ASSERT(systemView);

  systemView->refrigerationCondenserView->setCondenserId(OSItemId());
  systemView->refrigerationSubCoolerView->setId(OSItemId());
  systemView->refrigerationSHXView->setId(OSItemId());
  systemView->refrigerationCasesView->removeAllCaseDetailViews();
  systemView->refrigerationCompressorView->removeAllCompressorDetailViews();
  systemView->refrigerationSecondaryView->removeAllSecondaryDetailViews();

  if( system )
  {
    systemView->setId(OSItemId(system->handle().toString(),QString(),false));

    if( boost::optional<model::RefrigerationSubcoolerLiquidSuction> subcooler = system->liquidSuctionHeatExchangerSubcooler() )
    {
      systemView->refrigerationSHXView->setId(OSItemId(subcooler->handle().toString(),QString(),false));

      systemView->refrigerationSHXView->setName(QString::fromStdString(subcooler->name().get()));
    }

    if( boost::optional<model::RefrigerationSubcoolerMechanical> subcooler = system->mechanicalSubcooler() )
    {
      systemView->refrigerationSubCoolerView->setId(OSItemId(subcooler->handle().toString(),QString(),false));

      systemView->refrigerationSubCoolerView->setName(QString::fromStdString(subcooler->name().get()));
    }

    if( boost::optional<model::ModelObject> condenser = system->refrigerationCondenser() )
    {
      systemView->refrigerationCondenserView->setCondenserId(OSItemId(condenser->handle().toString(),QString(),false));

      systemView->refrigerationCondenserView->setCondenserName(QString::fromStdString(condenser->name().get()));

      const QPixmap * pixmap = IconLibrary::Instance().findIcon(condenser->iddObjectType().value());
      systemView->refrigerationCondenserView->setIcon(*pixmap);
    }

    // insert secondary systems

    std::vector<model::RefrigerationCondenserCascade> cascadeCondensers = system->cascadeCondenserLoads();
    for( std::vector<model::RefrigerationCondenserCascade>::iterator it = cascadeCondensers.begin();
         it != cascadeCondensers.end();
         ++it )
    {
      SecondaryDetailView * detailView = new SecondaryDetailView();
      connect(detailView, &SecondaryDetailView::zoomInOnSystemClicked, this, static_cast<void (RefrigerationController::*)(const Handle &)>(&RefrigerationController::zoomInOnSystem));
      connect(detailView, &SecondaryDetailView::removeClicked, this, &RefrigerationController::removeLoad);
      if( boost::optional<model::RefrigerationSystem> t_cascadeSystem = cascadeSystem(*it) )
      {
        detailView->setName(QString::fromStdString(t_cascadeSystem->name().get()));
        detailView->setHandle(t_cascadeSystem->handle());
      }
      else
      {
        detailView->setName(QString::fromStdString(it->name().get()));
        detailView->setHandle(it->handle());
      }
      systemView->refrigerationSecondaryView->insertSecondaryDetailView(0,detailView);
    }

    // insert compressors

    std::vector<model::RefrigerationCompressor> compressors = system->compressors();

    int compressorIndex = 1;

    for( std::vector<model::RefrigerationCompressor>::iterator it = compressors.begin();
         it != compressors.end();
         ++it )
    {
      RefrigerationCompressorDetailView * detailView = new RefrigerationCompressorDetailView(); 

      detailView->setId(OSItemId(it->handle().toString(),QString(),false));

      detailView->setLabel(QString::number(compressorIndex));

      connect(detailView, &RefrigerationCompressorDetailView::removeClicked, this, &RefrigerationController::removeCompressor);

      connect(detailView, &RefrigerationCompressorDetailView::inspectClicked, this, &RefrigerationController::inspectOSItem);

      systemView->refrigerationCompressorView->insertCompressorDetailView(0,detailView);

      compressorIndex++;
    }

    // insert cases

    std::vector<model::RefrigerationCase> cases = system->cases();

    systemView->refrigerationCasesView->setNumberOfDisplayCases(cases.size());

    for( std::vector<model::RefrigerationCase>::iterator it = cases.begin();
         it != cases.end();
         ++it )
    {
      RefrigerationCaseDetailView * detailView = new RefrigerationCaseDetailView();

      detailView->setId(OSItemId(it->handle().toString(),QString(),false));

      detailView->setName(QString::fromStdString(it->name().get()));

      connect(detailView, &RefrigerationCaseDetailView::removeClicked, this, &RefrigerationController::removeCase);

      connect(detailView, &RefrigerationCaseDetailView::inspectClicked, this, &RefrigerationController::inspectOSItem);

      systemView->refrigerationCasesView->insertCaseDetailView(0,detailView);
    }

    // insert walkins

    std::vector<model::RefrigerationWalkIn> walkins = system->walkins();

    systemView->refrigerationCasesView->setNumberOfWalkinCases(walkins.size());

    for( std::vector<model::RefrigerationWalkIn>::iterator it = walkins.begin();
         it != walkins.end();
         ++it )
    {
      RefrigerationCaseDetailView * detailView = new RefrigerationCaseDetailView();

      detailView->setId(OSItemId(it->handle().toString(),QString(),false));

      detailView->setName(QString::fromStdString(it->name().get()));

      connect(detailView, &RefrigerationCaseDetailView::removeClicked, this, &RefrigerationController::removeCase);

      connect(detailView, &RefrigerationCaseDetailView::inspectClicked, this, &RefrigerationController::inspectOSItem);

      systemView->refrigerationCasesView->insertCaseDetailView(0,detailView);
    }

    systemView->adjustLayout();
  }
}

RefrigerationController::RefrigerationController()
  : QObject()
{
  m_refrigerationGridScene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());

  m_refrigerationView = new RefrigerationView();

  connect(m_refrigerationView->zoomOutButton, &QPushButton::clicked, this, &RefrigerationController::zoomOutToSystemGridView);

  // These get deleted with when the scene is deleted
  m_refrigerationSystemGridView = new GridLayoutItem();
  m_refrigerationSystemGridView->setCellSize(RefrigerationSystemMiniView::cellSize());
  m_refrigerationSystemGridView->setMargin(RefrigerationSystemView::margin);

  m_refrigerationSystemListController = QSharedPointer<RefrigerationSystemListController>(new RefrigerationSystemListController(this));
  m_refrigerationSystemGridView->setListController(m_refrigerationSystemListController);

  m_refrigerationSystemGridView->setDelegate(QSharedPointer<RefrigerationSystemItemDelegate>(new RefrigerationSystemItemDelegate()));

  m_refrigerationGridScene->addItem(m_refrigerationSystemGridView);

  zoomOutToSystemGridView();
}

RefrigerationController::~RefrigerationController()
{
  delete m_refrigerationView;

  delete m_refrigerationScene;
}

boost::optional<model::RefrigerationSystem> RefrigerationController::supplySystem(const model::RefrigerationCondenserCascade & condenser)
{
  boost::optional<model::RefrigerationSystem> result;

  model::Model t_model = condenser.model();
  std::vector<model::RefrigerationSystem> systems = t_model.getConcreteModelObjects<model::RefrigerationSystem>();
  for(std::vector<model::RefrigerationSystem>::iterator it = systems.begin();
      it != systems.end();
      ++it)
  {
    std::vector<model::RefrigerationCondenserCascade> condenserLoads = it->cascadeCondenserLoads();
    if( std::find(condenserLoads.begin(),condenserLoads.end(),condenser) != condenserLoads.end() )
    {
      result = *it;
      break;
    }
  }

  return result;
}

boost::optional<model::RefrigerationSystem> RefrigerationController::cascadeSystem(const model::RefrigerationCondenserCascade & condenser)
{
  boost::optional<model::RefrigerationSystem> result;

  model::Model t_model = condenser.model();
  std::vector<model::RefrigerationSystem> systems = t_model.getConcreteModelObjects<model::RefrigerationSystem>();
  for(std::vector<model::RefrigerationSystem>::iterator it = systems.begin();
      it != systems.end();
      ++it)
  {
    if( boost::optional<model::ModelObject> mo = it->refrigerationCondenser() )
    {
      if( mo->handle() == condenser.handle() )
      {
        result = *it;
        break;
      }
    }
  }

  return result;
}

void RefrigerationController::zoomInOnSystem(const Handle & handle)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  model::Model t_model = doc->model();
  
  if( boost::optional<model::RefrigerationSystem> system = t_model.getModelObject<model::RefrigerationSystem>(handle) )
  {
    zoomInOnSystem(system.get());
  }
}

void RefrigerationController::zoomInOnSystem(model::RefrigerationSystem & refrigerationSystem)
{
  model::OptionalModelObject mo;

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  doc->mainRightColumnController()->inspectModelObject(mo,false);

  m_currentSystem = refrigerationSystem;

  if( m_refrigerationScene )
  {
    m_refrigerationScene->deleteLater();
  }

  m_refrigerationScene = new QGraphicsScene();

  m_refrigerationView->header->show();

  m_detailView = new RefrigerationSystemView();

  refresh();

  m_refrigerationScene->addItem(m_detailView);

  m_refrigerationView->graphicsView->setScene(m_refrigerationScene);

  connect(m_detailView->refrigerationCondenserView, &RefrigerationCondenserView::componentDropped, this, &RefrigerationController::onCondenserViewDrop);

  connect(m_detailView->refrigerationCondenserView, &RefrigerationCondenserView::removeClicked, this, &RefrigerationController::removeCondenser);

  connect(m_detailView->refrigerationCondenserView, &RefrigerationCondenserView::inspectClicked, this, &RefrigerationController::inspectOSItem);

  connect(m_detailView->refrigerationCompressorView->refrigerationCompressorDropZoneView, &RefrigerationCompressorDropZoneView::componentDropped, this, &RefrigerationController::onCompressorViewDrop);

  connect(m_detailView->refrigerationCasesView->refrigerationCasesDropZoneView, &RefrigerationCasesDropZoneView::componentDropped, this, &RefrigerationController::onCasesViewDrop);

  connect(m_detailView->refrigerationSubCoolerView, &RefrigerationSubCoolerView::componentDropped, this, &RefrigerationController::onSubCoolerViewDrop);

  connect(m_detailView->refrigerationSubCoolerView, &RefrigerationSubCoolerView::removeClicked, this, &RefrigerationController::removeSubCooler);

  connect(m_detailView->refrigerationSubCoolerView, &RefrigerationSubCoolerView::inspectClicked, this, &RefrigerationController::inspectOSItem);

  connect(m_detailView->refrigerationSHXView, &RefrigerationSHXView::componentDropped, this, &RefrigerationController::onSHXViewDrop);

  connect(m_detailView->refrigerationSHXView, &RefrigerationSHXView::removeClicked, this, &RefrigerationController::removeSubCoolerLiquidSuction);

  connect(m_detailView->refrigerationSHXView, &RefrigerationSHXView::inspectClicked, this, &RefrigerationController::inspectOSItem);

  connect(m_detailView.data(), &RefrigerationSystemView::inspectClicked, this, &RefrigerationController::inspectOSItem);

  connect(m_detailView->refrigerationSecondaryView->secondaryDropZoneView, &SecondaryDropZoneView::componentDropped, this, &RefrigerationController::onSecondaryViewDrop);

  m_refrigerationView->graphicsView->setAlignment(Qt::AlignCenter);
}

void RefrigerationController::zoomOutToSystemGridView()
{
  model::OptionalModelObject mo;

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  doc->mainRightColumnController()->inspectModelObject(mo,false);

  m_currentSystem = boost::none;

  m_refrigerationView->header->hide();

  m_refrigerationSystemListController->reset();

  refresh();

  m_refrigerationView->graphicsView->setScene(m_refrigerationGridScene.data());

  m_refrigerationView->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void RefrigerationController::onCondenserViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationCondenserAirCooled> condenser 
          = mo->optionalCast<model::RefrigerationCondenserAirCooled>() )
    {
      if( boost::optional<model::ModelObject> currentCondenser = m_currentSystem->refrigerationCondenser() )
      {
        currentCondenser->remove();
      }

      model::RefrigerationCondenserAirCooled condenserClone = 
        condenser->clone(m_currentSystem->model()).cast<model::RefrigerationCondenserAirCooled>();

      m_currentSystem->setRefrigerationCondenser(condenserClone);

      refresh();
    }
    else if( boost::optional<model::RefrigerationCondenserCascade> condenser 
          = mo->optionalCast<model::RefrigerationCondenserCascade>() )
    {
      if( boost::optional<model::ModelObject> currentCondenser = m_currentSystem->refrigerationCondenser() )
      {
        currentCondenser->remove();
      }

      model::RefrigerationCondenserCascade condenserClone = 
        condenser->clone(m_currentSystem->model()).cast<model::RefrigerationCondenserCascade>();

      m_currentSystem->setRefrigerationCondenser(condenserClone);

      refresh();
    }
    else if( boost::optional<model::RefrigerationCondenserEvaporativeCooled> condenser 
          = mo->optionalCast<model::RefrigerationCondenserEvaporativeCooled>() )
    {
      if( boost::optional<model::ModelObject> currentCondenser = m_currentSystem->refrigerationCondenser() )
      {
        currentCondenser->remove();
      }

      model::RefrigerationCondenserEvaporativeCooled condenserClone = 
        condenser->clone(m_currentSystem->model()).cast<model::RefrigerationCondenserEvaporativeCooled>();

      m_currentSystem->setRefrigerationCondenser(condenserClone);

      refresh();
    }
    else if( boost::optional<model::RefrigerationCondenserWaterCooled> condenser 
          = mo->optionalCast<model::RefrigerationCondenserWaterCooled>() )
    {
      if( boost::optional<model::ModelObject> currentCondenser = m_currentSystem->refrigerationCondenser() )
      {
        currentCondenser->remove();
      }

      model::RefrigerationCondenserWaterCooled condenserClone = 
        condenser->clone(m_currentSystem->model()).cast<model::RefrigerationCondenserWaterCooled>();

      m_currentSystem->setRefrigerationCondenser(condenserClone);

      refresh();
    }
  }
}

void RefrigerationController::onCompressorViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

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

void RefrigerationController::onSecondaryViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);
  model::Model t_model = m_currentSystem->model();

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);
    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationSystem> system 
          = mo->optionalCast<model::RefrigerationSystem>() )
    {
      if( boost::optional<model::ModelObject> condenserModelObject = system->refrigerationCondenser() )
      {
        if( boost::optional<model::RefrigerationCondenserCascade> cascadeCondenser = condenserModelObject->optionalCast<model::RefrigerationCondenserCascade>() )
        {
          // If condenser is not already a load on another system
          if( ! supplySystem(cascadeCondenser.get()) )
          {
            model::RefrigerationSystem systemClone = 
              system->clone(t_model).cast<model::RefrigerationSystem>();
            boost::optional<model::RefrigerationCondenserCascade> cascadeCondenserClone =
              systemClone.refrigerationCondenser()->optionalCast<model::RefrigerationCondenserCascade>();
            OS_ASSERT(cascadeCondenserClone);
            m_currentSystem->addCascadeCondenserLoad(cascadeCondenserClone.get());

            refresh();
          }
        }
      }
      else
      {
        model::RefrigerationSystem systemClone = 
          system->clone(t_model).cast<model::RefrigerationSystem>();

        model::RefrigerationCondenserCascade newCascadeCondenser(t_model);
        systemClone.setRefrigerationCondenser(newCascadeCondenser);
        m_currentSystem->addCascadeCondenserLoad(newCascadeCondenser);

        refresh();
      }
    }
  }
  else if( doc->fromModel(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);
    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationSystem> system 
          = mo->optionalCast<model::RefrigerationSystem>() )
    {
      if( boost::optional<model::ModelObject> condenserModelObject = system->refrigerationCondenser() )
      {
        if( boost::optional<model::RefrigerationCondenserCascade> cascadeCondenser = condenserModelObject->optionalCast<model::RefrigerationCondenserCascade>() )
        {
          // If condenser is not already a load on another system
          if( ! supplySystem(cascadeCondenser.get()) )
          {
            m_currentSystem->addCascadeCondenserLoad(cascadeCondenser.get());
            refresh();
          }
        }
      }
      else
      {
        model::RefrigerationCondenserCascade newCascadeCondenser(t_model);
        system->setRefrigerationCondenser(newCascadeCondenser);
        m_currentSystem->addCascadeCondenserLoad(newCascadeCondenser);
        refresh();
      }
    }
  }
}

void RefrigerationController::onCasesViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

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
    else if( boost::optional<model::RefrigerationWalkIn> _walkin 
              = mo->optionalCast<model::RefrigerationWalkIn>())
    {
      model::RefrigerationWalkIn walkinClone = 
        _walkin->clone(m_currentSystem->model()).cast<model::RefrigerationWalkIn>();
      m_currentSystem->addWalkin(walkinClone);
      refresh();
      //QMessageBox box(doc->mainWindow());
      //box.setText("Walkins are not yet supported by OpenStudio.");
      //box.exec();
    }
  }
}

void RefrigerationController::onSubCoolerViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationSubcoolerMechanical> _subcooler 
          = mo->optionalCast<model::RefrigerationSubcoolerMechanical>() )
    {
      model::RefrigerationSubcoolerMechanical subcoolerClone = 
        _subcooler->clone(m_currentSystem->model()).cast<model::RefrigerationSubcoolerMechanical>();

      m_currentSystem->setMechanicalSubcooler(subcoolerClone);

      refresh();
    }
  }
}

void RefrigerationController::removeSubCooler(const OSItemId & itemid)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::ModelObject> mo = model->getModelObject<model::ModelObject>(Handle(itemid.itemId())))
    {
      mo->remove();

      refresh();
    }
  }
}

void RefrigerationController::removeLoad(const Handle & handle)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::RefrigerationSystem> cascadeSystem = model->getModelObject<model::RefrigerationSystem>(handle))
    {
      OS_ASSERT(m_currentSystem);
      if( boost::optional<model::ModelObject> condenserModelObject = cascadeSystem->refrigerationCondenser() )
      {
        if( boost::optional<model::RefrigerationCondenserCascade> condenserCascade = condenserModelObject->optionalCast<model::RefrigerationCondenserCascade>() )
        {
          m_currentSystem->removeCascadeCondenserLoad(condenserCascade.get());
          refresh();
        }
      }
    }
    else if(boost::optional<model::RefrigerationCondenserCascade> condenserCascade = model->getModelObject<model::RefrigerationCondenserCascade>(handle))
    {
      condenserCascade->remove();
      refresh();
    }
  }
}

void RefrigerationController::onSHXViewDrop(const OSItemId & itemid)
{
  OS_ASSERT(m_currentSystem);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    OS_ASSERT(mo); 

    if( boost::optional<model::RefrigerationSubcoolerLiquidSuction> _subcooler 
          = mo->optionalCast<model::RefrigerationSubcoolerLiquidSuction>() )
    {
      model::RefrigerationSubcoolerLiquidSuction subcoolerClone = 
        _subcooler->clone(m_currentSystem->model()).cast<model::RefrigerationSubcoolerLiquidSuction>();

      m_currentSystem->setLiquidSuctionHeatExchangerSubcooler(subcoolerClone);

      refresh();
    }
  }
}

void RefrigerationController::removeSubCoolerLiquidSuction(const OSItemId & itemid)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::ModelObject> mo = model->getModelObject<model::ModelObject>(Handle(itemid.itemId())))
    {
      mo->remove();

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
    refreshRefrigerationSystemView(m_detailView,m_currentSystem);
  }
  
  m_dirty = false;
}

RefrigerationView * RefrigerationController::refrigerationView() const
{
  return m_refrigerationView;
}

void RefrigerationController::inspectOSItem(const OSItemId & itemid)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  OS_ASSERT(doc);

  OS_ASSERT(m_currentSystem);

  boost::optional<model::ModelObject> mo = m_currentSystem->model().getModelObject<model::ModelObject>(Handle(itemid.itemId()));

  doc->mainRightColumnController()->inspectModelObject(mo,false);
}

void RefrigerationController::removeCompressor(const OSItemId & itemid)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::ModelObject> mo = model->getModelObject<model::ModelObject>(Handle(itemid.itemId())))
    {
      mo->remove();

      refresh();
    }
  }
}

void RefrigerationController::removeCondenser(const OSItemId & itemid)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::ModelObject> mo = model->getModelObject<model::ModelObject>(Handle(itemid.itemId())))
    {
      mo->remove();

      OS_ASSERT(m_detailView);

      m_detailView->refrigerationCondenserView->setCondenserId(OSItemId());
    }
  }
}

void RefrigerationController::removeCase(const OSItemId & itemid)
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    if(boost::optional<model::ModelObject> mo = model->getModelObject<model::ModelObject>(Handle(itemid.itemId())))
    {
      mo->remove();

      refresh();
    }
  }
}

QSharedPointer<RefrigerationSystemListController> RefrigerationController::refrigerationSystemListController() const
{
  return m_refrigerationSystemListController;
}

RefrigerationSystemListController::RefrigerationSystemListController(RefrigerationController * refrigerationController)
  : m_refrigerationController(refrigerationController)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  model::Model t_model = doc->model();
  connect(t_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject),
    this,
    &RefrigerationSystemListController::onModelObjectAdd);

  connect(this, &RefrigerationSystemListController::itemInsertedPrivate, this, &RefrigerationSystemListController::itemInserted, Qt::QueuedConnection);
}

void RefrigerationSystemListController::reset()
{
  emit modelReset();
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

void RefrigerationSystemListController::createNewSystem()
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    model::RefrigerationSystem system(model.get());
  }
}

void RefrigerationSystemListController::onModelObjectAdd(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if( iddObjectType == model::RefrigerationSystem::iddObjectType() )
  {
    emit itemInsertedPrivate(systemIndex(object.cast<model::RefrigerationSystem>()));
  }
}

void RefrigerationSystemListController::addSystem(const OSItemId & itemid)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  if( doc->fromComponentLibrary(itemid) )
  {
    boost::optional<model::ModelObject> mo = doc->getModelObject(itemid);

    boost::optional<model::Model> model = OSAppBase::instance()->currentModel();

    if( mo && model )
    {
      if( boost::optional<model::RefrigerationSystem> system = mo->optionalCast<model::RefrigerationSystem>() )
      {
        model::RefrigerationSystem systemClone = 
          system->clone(model.get()).cast<model::RefrigerationSystem>();
      }
    }
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
    result = model->getConcreteModelObjects<model::RefrigerationSystem>();
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

model::RefrigerationSystem RefrigerationSystemListItem::system() const
{
  return m_refrigerationSystem;
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
  QGraphicsObject * itemView = nullptr;

  if( QSharedPointer<RefrigerationSystemListItem> listItem = dataSource.dynamicCast<RefrigerationSystemListItem>() )
  {
    RefrigerationSystemMiniView * refrigerationSystemMiniView = new RefrigerationSystemMiniView();

    connect(refrigerationSystemMiniView->removeButtonItem, &RemoveButtonItem::mouseClicked,
      static_cast<RefrigerationSystemListItem *>(dataSource.data()), &RefrigerationSystemListItem::remove);

    connect(refrigerationSystemMiniView->zoomInButtonItem, &ZoomInButtonItem::mouseClicked,
      static_cast<RefrigerationSystemListItem *>(dataSource.data()), &RefrigerationSystemListItem::zoomInOnSystem);

    refrigerationSystemMiniView->setName(listItem->systemName());

    boost::optional<model::RefrigerationSystem> system = listItem->system();
    RefrigerationController * refrigerationController = qobject_cast<RefrigerationSystemListController *>(dataSource->controller())->refrigerationController();
    refrigerationController->refreshRefrigerationSystemView(refrigerationSystemMiniView->refrigerationSystemView,system);
    refrigerationSystemMiniView->adjustLayout();

    itemView = refrigerationSystemMiniView;
  }
  else if( dataSource.dynamicCast<RefrigerationSystemListDropZoneItem>() )
  {
    RefrigerationSystemDropZoneView * refrigerationSystemDropZoneView = new RefrigerationSystemDropZoneView();

    connect(refrigerationSystemDropZoneView, &RefrigerationSystemDropZoneView::componentDropped,
      qobject_cast<RefrigerationSystemListController *>(dataSource->controller()), &RefrigerationSystemListController::addSystem);

    itemView = refrigerationSystemDropZoneView;
  }

  return itemView;
}

} // openstudio
