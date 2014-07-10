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

#include "HVACSystemsController.hpp"
#include "RefrigerationController.hpp"
#include "RefrigerationGraphicsItems.hpp"
#include "RefrigerationGridController.hpp"
#include "RefrigerationGridView.hpp"
#include "VRFController.hpp"
#include "VRFGraphicsItems.hpp"
#include "LoopLibraryDialog.hpp"
#include "HVACSystemsView.hpp"
#include "LoopScene.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "RefrigerationScene.hpp"
#include "../shared_gui_components/OSSwitch.hpp"
#include "ServiceWaterScene.hpp"
#include "HorizontalTabWidget.hpp"
#include "MainRightColumnController.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../model/ModelObject.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/WaterToWaterComponent.hpp"
#include "../model/WaterToWaterComponent_Impl.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../model/ControllerMechanicalVentilation.hpp"
#include "../model/ControllerMechanicalVentilation_Impl.hpp"
#include "../model/ControllerOutdoorAir.hpp"
#include "../model/ControllerOutdoorAir_Impl.hpp"
#include "../model/SetpointManager.hpp"
#include "../model/SetpointManagerSingleZoneReheat.hpp"
#include "../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../model/SetpointManagerScheduled.hpp"
#include "../model/SetpointManagerScheduled_Impl.hpp"
#include "../model/SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"
#include "../model/SetpointManagerOutdoorAirReset.hpp"
#include "../model/SetpointManagerOutdoorAirReset_Impl.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../model/CoilCoolingDXTwoSpeed.hpp"
#include "../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilHeatingGas_Impl.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingElectric_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include "../model/Loop.hpp"
#include "../model/Loop_Impl.hpp"
#include "../model/Splitter.hpp"
#include "../model/Splitter_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/Mixer.hpp"
#include "../model/Mixer_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/HVACTemplates.hpp"

#include "../utilities/core/Assert.hpp"

#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QLayout>

namespace openstudio {

const QString SHW = "SHW";
const QString REFRIGERATION = "REFRIGERATION";
const QString VRF = "VRF";

HVACSystemsController::HVACSystemsController(bool isIP, const model::Model & model)
  : QObject(),
    m_model(model),
    m_isIP(isIP)
{
  m_hvacSystemsView = new HVACSystemsView();

  m_hvacLayoutController = std::shared_ptr<HVACLayoutController>(new HVACLayoutController(this));

  m_hvacControlsController = std::shared_ptr<HVACControlsController>(new HVACControlsController(this));

  bool bingo;

  bingo = connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
                   SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this,
                   SLOT(onObjectAdded(const WorkspaceObject&)) );
  OS_ASSERT(bingo);

  bingo = connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
                   SIGNAL(removeWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this,
                   SLOT(onObjectRemoved(const WorkspaceObject&)) );
  OS_ASSERT(bingo);


  bingo = connect( m_hvacSystemsView->hvacToolbarView->addButton,SIGNAL(clicked()),
                   this,SLOT(onAddSystemClicked()));
  OS_ASSERT(bingo);

  bingo = connect( m_hvacSystemsView->hvacToolbarView->deleteButton,SIGNAL(clicked()),
                   this,SLOT(onRemoveLoopClicked()));
  OS_ASSERT(bingo);

  bingo = connect( m_hvacSystemsView->hvacToolbarView->systemComboBox, SIGNAL(currentIndexChanged( int )),
                   this, SLOT(onSystemComboBoxIndexChanged( int )));
  OS_ASSERT(bingo);

  bingo = connect( m_hvacSystemsView->hvacToolbarView->topologyViewButton,SIGNAL(clicked()),
                   this,SLOT(onShowTopologyClicked()));
  OS_ASSERT(bingo);

  bingo = connect( m_hvacSystemsView->hvacToolbarView->controlsViewButton,SIGNAL(clicked()),
                   this,SLOT(onShowControlsClicked()));
  OS_ASSERT(bingo);

  bingo = connect( m_hvacSystemsView->hvacToolbarView->gridViewButton,SIGNAL(clicked()),
                   this,SLOT(onShowGridClicked()));
  OS_ASSERT(bingo);

  updateLater();
}

HVACSystemsController::~HVACSystemsController()
{
  if( m_hvacSystemsView ) { delete m_hvacSystemsView; }
}

void HVACSystemsController::clearSceneSelection()
{
  if( m_hvacLayoutController )
  {
    m_hvacLayoutController->clearSceneSelection();
  }
}

QString HVACSystemsController::currentHandle() const
{
  return m_currentHandle;
}

void HVACSystemsController::setCurrentHandle(const QString & handle)
{
  m_currentHandle = handle; 

  updateLater();
}

model::Model HVACSystemsController::model() const
{
  return m_model;
}

void HVACSystemsController::update()
{
  if( m_dirty )
  {
    m_hvacSystemsView->setUpdatesEnabled(false); 

    model::OptionalModelObject mo;
    OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectModelObject(mo,false);
    
    // Remove old stuff

    QComboBox * systemComboBox = m_hvacSystemsView->hvacToolbarView->systemComboBox;

    systemComboBox->blockSignals(true);

    systemComboBox->clear();

    // Populate system combo box
    std::vector<model::Loop> loops = m_model.getModelObjects<model::Loop>();

    for( std::vector<model::Loop>::iterator it = loops.begin();
         it != loops.end();
         ++it )
    {
      systemComboBox->addItem(QString::fromStdString(it->name().get()),it->handle().toString());
    }

    systemComboBox->addItem("Service Hot Water",SHW);
    systemComboBox->addItem("Refrigeration",REFRIGERATION);
    systemComboBox->addItem("VRF",VRF);

    // Set system combo box current index
    QString handle = currentHandle();
    if( handle == SHW  || 
        m_model.getModelObject<model::WaterUseConnections>(handle)
      )
    {
      int index = systemComboBox->findData(SHW);

      OS_ASSERT(index >= 0);
      
      systemComboBox->setCurrentIndex(index);
    }
    else if( handle == REFRIGERATION ) 
    {
      int index = systemComboBox->findData(REFRIGERATION);

      OS_ASSERT(index >= 0);
      
      systemComboBox->setCurrentIndex(index);
    }
    else if( handle == VRF ) 
    {
      int index = systemComboBox->findData(VRF);

      OS_ASSERT(index >= 0);
      
      systemComboBox->setCurrentIndex(index);
    }
    else
    {
      int index = systemComboBox->findData(handle);

      if(index >= 0)
      {
        systemComboBox->setCurrentIndex(index);
      }
      else
      {
        systemComboBox->setCurrentIndex(systemComboBox->findData(SHW));
      }
    }

    systemComboBox->blockSignals(false);

    // Show layout 

    m_hvacSystemsView->hvacToolbarView->zoomInButton->show();
    m_hvacSystemsView->hvacToolbarView->zoomOutButton->show();

    m_hvacSystemsView->hvacToolbarView->addButton->show();
    m_hvacSystemsView->hvacToolbarView->deleteButton->show();

    m_hvacLayoutController.reset();
    m_hvacControlsController.reset();
    m_refrigerationController.reset();
    m_vrfController.reset();

    if( handle == REFRIGERATION )
    {
      m_hvacSystemsView->hvacToolbarView->zoomInButton->setEnabled(false);
      m_hvacSystemsView->hvacToolbarView->zoomOutButton->setEnabled(false);

      if( m_hvacSystemsView->hvacToolbarView->topologyViewButton->isChecked() )
      {
        m_refrigerationController = std::shared_ptr<RefrigerationController>(new RefrigerationController());

        m_hvacSystemsView->mainViewSwitcher->setView(m_refrigerationController->refrigerationView());
      }  
      else if( m_hvacSystemsView->hvacToolbarView->gridViewButton->isChecked() )
      {
        // TODO

        m_hvacSystemsView->hvacToolbarView->zoomInButton->hide();
        m_hvacSystemsView->hvacToolbarView->zoomOutButton->hide(); 
        
        m_hvacSystemsView->hvacToolbarView->addButton->hide();
        m_hvacSystemsView->hvacToolbarView->deleteButton->hide();

        m_refrigerationGridController = std::shared_ptr<RefrigerationGridController>(new RefrigerationGridController(m_isIP, m_model));

        bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                                   m_refrigerationGridController.get()->refrigerationGridView(), SIGNAL(toggleUnitsClicked(bool)));
        OS_ASSERT(isConnected);

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                              this, SLOT(toggleUnits(bool)));
        OS_ASSERT(isConnected);

        m_hvacSystemsView->mainViewSwitcher->setView(m_refrigerationGridController->refrigerationGridView());
      }
      else
      {
          m_hvacControlsController = std::shared_ptr<HVACControlsController>(new HVACControlsController(this));

          m_hvacSystemsView->mainViewSwitcher->setView(m_hvacControlsController->noControlsView());
      }
    }
    else if( handle == VRF )
    {
      m_hvacSystemsView->hvacToolbarView->zoomInButton->setEnabled(false);
      m_hvacSystemsView->hvacToolbarView->zoomOutButton->setEnabled(false);

      if( m_hvacSystemsView->hvacToolbarView->topologyViewButton->isChecked() )
      {
        m_vrfController = std::shared_ptr<VRFController>(new VRFController());

        m_hvacSystemsView->mainViewSwitcher->setView(m_vrfController->vrfView());
      }
      else
      {
          m_hvacControlsController = std::shared_ptr<HVACControlsController>(new HVACControlsController(this));

          m_hvacSystemsView->mainViewSwitcher->setView(m_hvacControlsController->noControlsView());
      }
    }
    else
    {
      if( m_hvacSystemsView->hvacToolbarView->topologyViewButton->isChecked() )
      {
        m_hvacLayoutController = std::shared_ptr<HVACLayoutController>(new HVACLayoutController(this));
        m_hvacSystemsView->mainViewSwitcher->setView(m_hvacLayoutController->hvacGraphicsView());

        m_hvacSystemsView->hvacToolbarView->zoomInButton->setEnabled(true);
        m_hvacSystemsView->hvacToolbarView->zoomOutButton->setEnabled(true);

        OSAppBase::instance()->currentDocument()->mainRightColumnController()->mainRightColumnView()->setCurrentId(MainRightColumnController::LIBRARY);
      }
      else if( m_hvacSystemsView->hvacToolbarView->gridViewButton->isChecked() )
      {
        // TODO
        m_refrigerationGridController = std::shared_ptr<RefrigerationGridController>(new RefrigerationGridController(m_isIP, m_model));

        bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                                   m_refrigerationGridController.get()->refrigerationGridView(), SIGNAL(toggleUnitsClicked(bool)));
        OS_ASSERT(isConnected);

        isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                              this, SLOT(toggleUnits(bool)));
        OS_ASSERT(isConnected);

        m_hvacSystemsView->mainViewSwitcher->setView(m_refrigerationGridController->refrigerationGridView());
      }
      else
      {
        m_hvacControlsController = std::shared_ptr<HVACControlsController>(new HVACControlsController(this));

        if( currentLoop() && currentLoop()->optionalCast<model::AirLoopHVAC>() )
        {
          m_hvacSystemsView->mainViewSwitcher->setView(m_hvacControlsController->hvacControlsView());

          m_hvacSystemsView->hvacToolbarView->zoomInButton->setEnabled(false);
          m_hvacSystemsView->hvacToolbarView->zoomOutButton->setEnabled(false);

          OSAppBase::instance()->currentDocument()->mainRightColumnController()->mainRightColumnView()->setCurrentId(MainRightColumnController::MY_MODEL);
        }
        else
        {
          m_hvacSystemsView->mainViewSwitcher->setView(m_hvacControlsController->noControlsView());
        }
      }
    }

    m_hvacSystemsView->setUpdatesEnabled(true); 

    m_dirty = false;
  }
}

void HVACSystemsController::updateLater()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(update()));
}

HVACSystemsView * HVACSystemsController::hvacSystemsView() const
{
  return m_hvacSystemsView;
}

std::shared_ptr<HVACLayoutController> HVACSystemsController::hvacLayoutController() const
{
  return m_hvacLayoutController;
}

std::shared_ptr<HVACControlsController> HVACSystemsController::hvacControlsController() const
{
  return m_hvacControlsController;
}

std::vector<IddObjectType> HVACSystemsController::systemComboBoxTypes() const
{
  std::vector<IddObjectType> types;

  types.push_back(model::AirLoopHVAC::iddObjectType());
  types.push_back(model::PlantLoop::iddObjectType());

  return types;
}

void HVACSystemsController::onObjectAdded(const WorkspaceObject & workspaceObject)
{
  std::vector<IddObjectType> types = systemComboBoxTypes();

  if(std::find(types.begin(),types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != types.end())
  {
    updateLater();
  }
}

void HVACSystemsController::onObjectRemoved(const WorkspaceObject & workspaceObject)
{
  std::vector<IddObjectType> types = systemComboBoxTypes();

  if(std::find(types.begin(),types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != types.end())
  {
    updateLater();
  }
}

void HVACSystemsController::onObjectChanged()
{
}

void HVACSystemsController::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

void HVACLayoutController::addLibraryObjectToTopLevel(OSItemId itemid)
{
  model::OptionalModelObject object = OSAppBase::instance()->currentDocument()->getModelObject(itemid);

  if( object )
  {
    if( OSAppBase::instance()->currentDocument()->fromComponentLibrary(itemid) ) 
    {
      if( boost::optional<model::WaterUseConnections> waterUseConnections = object->optionalCast<model::WaterUseConnections>() )
      {
        waterUseConnections->clone(m_hvacSystemsController->model());

        return;
      }
    }
  }

  QMessageBox message(m_hvacSystemsController->hvacSystemsView());
  
  message.setText("The selected component is not allowed at this location.");

  message.exec();
}


void HVACLayoutController::addLibraryObjectToModelNode(OSItemId itemid, model::HVACComponent & comp)
{
  model::OptionalModelObject object = OSAppBase::instance()->currentDocument()->getModelObject(itemid);

  if( object )
  {
    bool remove = false;

    if( ! OSAppBase::instance()->currentDocument()->fromModel(itemid) ) 
    {
      object = object->clone(comp.model());
      remove = true;
    }

    OS_ASSERT(object);

    bool added = false;

    if( boost::optional<model::HVACComponent> hvacComponent = object->optionalCast<model::HVACComponent>() )
    {
      if( boost::optional<model::Node> node = comp.optionalCast<model::Node>() )
      {
        added = hvacComponent->addToNode(node.get());
      }
      else if( boost::optional<model::Splitter> splitter = comp.optionalCast<model::Splitter>() )
      {
        if( boost::optional<model::PlantLoop> plant = splitter->plantLoop() )
        {
          if( plant->supplyComponent(splitter->handle()) )
          {
            added = plant->addSupplyBranchForComponent(hvacComponent.get());
          }
          else if( plant->demandComponent(splitter->handle()) )
          {
            added = plant->addDemandBranchForComponent(hvacComponent.get());
          } 
        }
        else if( boost::optional<model::AirLoopHVAC> airLoop = splitter->airLoopHVAC() )
        {
          if( boost::optional<model::ThermalZone> zone = object->optionalCast<model::ThermalZone>() )
          {
            added = airLoop->addBranchForZone(zone.get());
          }
        }
      }
    }
    else if( boost::optional<model::WaterUseEquipment> waterUseEquipment = object->optionalCast<model::WaterUseEquipment>() )
    {
      if( boost::optional<model::WaterUseConnections> waterUseConnections = comp.optionalCast<model::WaterUseConnections>() )
      {
        added = waterUseConnections->addWaterUseEquipment(waterUseEquipment.get());
      }
    }
    else if( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = 
             object->optionalCast<model::WaterUseEquipmentDefinition>() )
    {
      if( boost::optional<model::WaterUseConnections> waterUseConnections = comp.optionalCast<model::WaterUseConnections>() )
      {
        model::WaterUseEquipment waterUseEquipment(waterUseEquipmentDefinition.get());
        waterUseEquipment.resetFlowRateFractionSchedule();

        added = waterUseConnections->addWaterUseEquipment(waterUseEquipment);
      }
    }

    if( ! added )
    {
      if( remove )
      {
        object->remove();
      }

      QMessageBox message(m_hvacSystemsController->hvacSystemsView());
      
      message.setText("The selected component is not allowed at this location.");

      message.exec();
    }
  }
}

boost::optional<model::Loop> HVACSystemsController::currentLoop() const
{
  return m_model.getModelObject<model::Loop>(m_currentHandle);
}

void HVACLayoutController::removeModelObject(model::ModelObject & modelObject)
{
  if( modelObject.handle() == NULL ) return;

  model::OptionalModelObject mo;

  OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectModelObject(mo,false);

  boost::optional<model::Loop> loop = m_hvacSystemsController->currentLoop();

  if( loop )
  {
    if( boost::optional<model::WaterToWaterComponent> comp = 
          modelObject.optionalCast<model::WaterToWaterComponent>() )
    {
      boost::optional<model::PlantLoop> plant = comp->plantLoop();
      boost::optional<model::PlantLoop> secondaryPlant = comp->secondaryPlantLoop();

      if( plant && secondaryPlant )
      {
        if( plant.get() == loop.get() )
        {
          comp->removeFromPlantLoop();

          return;
        }
        if( secondaryPlant.get() == loop.get() )
        {
          comp->removeFromSecondaryPlantLoop();

          return;
        }
      }
    }
    else if( boost::optional<model::WaterToAirComponent> comp = 
          modelObject.optionalCast<model::WaterToAirComponent>() )
    {
      boost::optional<model::PlantLoop> plant = comp->plantLoop();
      boost::optional<model::AirLoopHVAC> airLoop = comp->airLoopHVAC();

      if( plant && airLoop )
      {
        if( plant.get() == loop.get() )
        {
          comp->removeFromPlantLoop();

          return;
        }
        if( airLoop.get() == loop.get() )
        {
          comp->removeFromAirLoopHVAC();

          return;
        }
      }
    }
    else if( boost::optional<model::ThermalZone> comp = 
          modelObject.optionalCast<model::ThermalZone>() )
    {
      boost::optional<model::AirLoopHVAC> airLoop = comp->airLoopHVAC();

      if( airLoop )
      {
        airLoop->removeBranchForZone(comp.get());

        return;
      }
    }
  }

  modelObject.remove();

  return;
}

void HVACLayoutController::goToOtherLoop( model::ModelObject & modelObject )
{
  if( boost::optional<model::WaterUseConnections> waterUseConnections = modelObject.optionalCast<model::WaterUseConnections>() )
  {
    if( boost::optional<model::Loop> loop = waterUseConnections->loop() )
    {
      goToOtherLoop( loop.get() );
    }

    return;
  }

  if( boost::optional<model::Loop> loop = modelObject.optionalCast<model::Loop>() )
  {
    m_hvacSystemsController->setCurrentHandle(loop->handle().toString());

    return;
  }

  boost::optional<model::Loop> t_currentLoop = m_hvacSystemsController->currentLoop();

  if( t_currentLoop )
  {
    if( boost::optional<model::WaterToAirComponent> comp = modelObject.optionalCast<model::WaterToAirComponent>() )
    {
      if( boost::optional<model::AirLoopHVAC> airLoopHVAC = t_currentLoop->optionalCast<model::AirLoopHVAC>() )
      {
        if( boost::optional<model::PlantLoop> plantLoop = comp->plantLoop() )
        {
          m_hvacSystemsController->setCurrentHandle(plantLoop->handle().toString());
        }

        return;
      }
      else if( boost::optional<model::PlantLoop> plantLoop = t_currentLoop->optionalCast<model::PlantLoop>() )
      {
        if( boost::optional<model::AirLoopHVAC> airLoopHVAC = comp->airLoopHVAC() )
        {
          m_hvacSystemsController->setCurrentHandle(airLoopHVAC->handle().toString());
        }

        return;
      }
    }

    if( boost::optional<model::WaterToWaterComponent> comp = modelObject.optionalCast<model::WaterToWaterComponent>() )
    {
      if( boost::optional<model::PlantLoop> loop = comp->plantLoop() )
      {
        if( t_currentLoop.get() == loop.get() )
        {
          if( boost::optional<model::PlantLoop> secondaryPlantLoop = comp->secondaryPlantLoop() )
          {
            m_hvacSystemsController->setCurrentHandle(secondaryPlantLoop->handle().toString());

            return;
          }
        }
      }

      if( boost::optional<model::PlantLoop> loop = comp->secondaryPlantLoop() )
      {
        if( t_currentLoop.get() == loop.get() )
        {
          if( boost::optional<model::PlantLoop> plantLoop = comp->plantLoop() )
          {
            m_hvacSystemsController->setCurrentHandle(plantLoop->handle().toString());

            return;
          }
        }
      }
    }
  }
}

void HVACSystemsController::addToModel(AddToModelEnum addToModelEnum)
{
  boost::optional<model::Loop> loop;

  switch (addToModelEnum)
  {
    case ADDTOMODEL_AIRLOOPHVAC:
    {
      model::AirLoopHVAC airLoopHVAC(m_model);

      loop = airLoopHVAC;

      break;
    }
    case ADDTOMODEL_PLANTLOOP:
    {
      model::PlantLoop plantLoop(m_model);

      loop = plantLoop;

      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_3:
    {
      loop = model::addSystemType3(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_4:
    {
      loop = model::addSystemType4(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_5:
    {
      loop = model::addSystemType5(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_6:
    {
      loop = model::addSystemType6(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_7:
    {
      loop = model::addSystemType7(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_8:
    {
      loop = model::addSystemType8(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_9:
    {
      loop = model::addSystemType9(m_model);
      break;
    }
    case ADDTOMODEL_SYSTEM_TYPE_10:
    {
      loop = model::addSystemType10(m_model);
      break;
    }
    default:
      break;
  }

  if( loop )
  {
    m_currentHandle = loop->handle().toString();
  }
}

void HVACSystemsController::onShowTopologyClicked()
{
  updateLater();
}

void HVACSystemsController::onShowControlsClicked()
{
  updateLater();
}

void HVACSystemsController::onShowGridClicked()
{
  updateLater();
}

void HVACSystemsController::onAddSystemClicked()
{
  LoopLibraryDialog loopLibraryDialog(m_hvacSystemsView);

  loopLibraryDialog.exec();

  if( loopLibraryDialog.result() == QDialog::Accepted )
  {
    addToModel(loopLibraryDialog.addToModelEnum().get());    
  }

  // Temporary hack to test refrigeration grid view
  //m_hvacLayoutController->refrigerationController()->refrigerationSystemListController()->createNewSystem();
}

void HVACSystemsController::onSystemComboBoxIndexChanged(int i)
{
  QString handle = m_hvacSystemsView->hvacToolbarView->systemComboBox->itemData(i).toString();

  setCurrentHandle(handle);
}

void HVACSystemsController::onRemoveLoopClicked()
{
  if( boost::optional<model::Loop> loop = currentLoop() )
  {
    if( loop->handle().isNull() ) { return; }

    QComboBox * chooser = m_hvacSystemsView->hvacToolbarView->systemComboBox;

    int i = chooser->findData(loop->handle().toString());

    if( i == 0 )
    {
      setCurrentHandle(QUuid(chooser->itemData(i + 1).toString()).toString());
    }
    else
    {
      setCurrentHandle(QUuid(chooser->itemData(i - 1).toString()).toString());
    }

    loop->remove();
  }
}

void HVACLayoutController::onModelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly)
{
  if( modelObject )
  {
    if( boost::optional<model::WaterUseConnections> waterUseConnections = modelObject->optionalCast<model::WaterUseConnections>() )
    {
      m_hvacSystemsController->setCurrentHandle(waterUseConnections->handle().toString());
    }
    else
    {
      OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectModelObject(modelObject,readOnly);
    }
  }
  else
  {
    OSAppBase::instance()->currentDocument()->mainRightColumnController()->inspectModelObject(modelObject,readOnly);
  }
}

HVACControlsController::HVACControlsController(HVACSystemsController * hvacSystemsController)
  : QObject(),
    m_hvacSystemsController(hvacSystemsController)
{
  m_hvacControlsView = new HVACControlsView();

  m_noControlsView = new NoControlsView();

  updateLater();
}

HVACControlsController::~HVACControlsController()
{
  if( m_hvacControlsView ) { delete m_hvacControlsView; }

  if( m_noControlsView ) { delete m_noControlsView; }

  if( m_mechanicalVentilationView ) { delete m_mechanicalVentilationView; }

  if( m_singleZoneReheatSPMView ) { delete m_singleZoneReheatSPMView; }

  if( m_noSupplyAirTempControlView ) { delete m_noSupplyAirTempControlView; }

  if( m_noMechanicalVentilationView) { delete m_noMechanicalVentilationView; }

  if( m_systemAvailabilityDropZone ) { delete m_systemAvailabilityDropZone; }

  if( m_supplyAirTempScheduleDropZone ) { delete m_supplyAirTempScheduleDropZone; }

  if( m_followOATempSPMView ) { delete m_followOATempSPMView; }

  if( m_oaResetSPMView ) { delete m_oaResetSPMView; }

  if( m_airLoopHVACUnitaryHeatPumpAirToAirControlView ) { delete m_airLoopHVACUnitaryHeatPumpAirToAirControlView; }

  if( m_scheduledSPMView ) { delete m_scheduledSPMView; }
}

boost::optional<model::AirLoopHVAC> HVACControlsController::airLoopHVAC() const
{
  if( boost::optional<model::Loop> loop = m_hvacSystemsController->currentLoop() )
  {
    return loop->optionalCast<model::AirLoopHVAC>();
  }
  return boost::none;
}

HVACControlsView * HVACControlsController::hvacControlsView() const
{
  return m_hvacControlsView;
}

NoControlsView * HVACControlsController::noControlsView() const
{
  return m_noControlsView;
}

void HVACControlsController::update()
{
  if( m_dirty )
  {
    m_hvacControlsView->setUpdatesEnabled(false); 

    if( m_mechanicalVentilationView ) { delete m_mechanicalVentilationView; }
    if( m_singleZoneReheatSPMView ) { delete m_singleZoneReheatSPMView; }
    if( m_noSupplyAirTempControlView ) { delete m_noSupplyAirTempControlView; }
    if( m_noMechanicalVentilationView) { delete m_noMechanicalVentilationView; }
    if( m_systemAvailabilityDropZone ) { delete m_systemAvailabilityDropZone; }
    if( m_supplyAirTempScheduleDropZone ) { delete m_supplyAirTempScheduleDropZone; }
    if( m_followOATempSPMView ) { delete m_followOATempSPMView; }
    if( m_oaResetSPMView ) { delete m_oaResetSPMView; }
    if( m_scheduledSPMView ) { delete m_scheduledSPMView; }
    if( m_airLoopHVACUnitaryHeatPumpAirToAirControlView ) { delete m_airLoopHVACUnitaryHeatPumpAirToAirControlView; }

    m_hvacControlsView->nightCycleComboBox->setEnabled(false);

    if( boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC() )
    {
      QString title;
      title.append(QString::fromStdString(t_airLoopHVAC->name().get()));
      m_hvacControlsView->systemNameLabel->setText(title);
      
      // Cooling Type
      
      m_hvacControlsView->coolingTypeLabel->setText("Unclassified Cooling Type");
      
      std::vector<model::ModelObject> modelObjects = t_airLoopHVAC->supplyComponents(model::CoilCoolingDXSingleSpeed::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->coolingTypeLabel->setText("DX Cooling");
      }
      
      modelObjects = t_airLoopHVAC->supplyComponents(model::CoilCoolingDXTwoSpeed::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->coolingTypeLabel->setText("DX Cooling");
      }
      
      modelObjects = t_airLoopHVAC->supplyComponents(model::CoilCoolingWater::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->coolingTypeLabel->setText("Chilled Water");
      }

      modelObjects = t_airLoopHVAC->supplyComponents(model::AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->coolingTypeLabel->setText("DX Cooling");
      }
      
      // Heating Type
      
      m_hvacControlsView->heatingTypeLabel->setText("Unclassified Heating Type");
      
      modelObjects = t_airLoopHVAC->supplyComponents(model::CoilHeatingGas::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->heatingTypeLabel->setText("Gas Heating"); 
      }
      
      modelObjects = t_airLoopHVAC->supplyComponents(model::CoilHeatingElectric::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->heatingTypeLabel->setText("Electric Heating"); 
      }
      
      modelObjects = t_airLoopHVAC->supplyComponents(model::CoilHeatingWater::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->heatingTypeLabel->setText("Hot Water");
      }

      modelObjects = t_airLoopHVAC->supplyComponents(model::AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
      if( modelObjects.size() > 0 )
      {
        m_hvacControlsView->heatingTypeLabel->setText("Air Source Heat Pump");
      }

      // HVAC Operation Schedule

      SystemAvailabilityVectorController * systemAvailabilityVectorController = new SystemAvailabilityVectorController();
      systemAvailabilityVectorController->attach(t_airLoopHVAC.get());
      m_systemAvailabilityDropZone = new OSDropZone(systemAvailabilityVectorController);
      m_systemAvailabilityDropZone->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
      m_systemAvailabilityDropZone->setMinItems(1);
      m_systemAvailabilityDropZone->setMaxItems(1);
      m_systemAvailabilityDropZone->setItemsRemoveable(false);
      m_systemAvailabilityDropZone->setAcceptDrops(true);
      m_systemAvailabilityDropZone->setItemsAcceptDrops(true);
      m_systemAvailabilityDropZone->setEnabled(true);
      m_hvacControlsView->hvacOperationViewSwitcher->setView(m_systemAvailabilityDropZone);

      // Night Cycle Control
      
      m_hvacControlsView->nightCycleComboBox->setEnabled(true);
      
      std::string nightCycleControlType = t_airLoopHVAC->nightCycleControlType();
      
      int nightCycleSelectorIndex = m_hvacControlsView->nightCycleComboBox->findData(QString::fromStdString(nightCycleControlType));
      
      m_hvacControlsView->nightCycleComboBox->setCurrentIndex(nightCycleSelectorIndex);

      bool bingo;

      bingo = connect(m_hvacControlsView->nightCycleComboBox,SIGNAL(currentIndexChanged(int)),
                      this,SLOT(onNightCycleComboBoxIndexChanged(int))); 
      OS_ASSERT(bingo);

      // Mechanical Ventilation

      if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = t_airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir();

        model::ControllerMechanicalVentilation controllerMechanicalVentilation = controllerOutdoorAir.controllerMechanicalVentilation();

        m_mechanicalVentilationView = new MechanicalVentilationView();

        m_hvacControlsView->ventilationViewSwitcher->setView(m_mechanicalVentilationView);

        // Economizer Control Type
        
        bingo = connect(m_mechanicalVentilationView->economizerComboBox,SIGNAL(currentIndexChanged(int)),
                        this,SLOT(onEconomizerComboBoxIndexChanged(int))); 
        OS_ASSERT(bingo);
        
        std::string economizerControlType = controllerOutdoorAir.getEconomizerControlType();
        
        int economizerIndex = m_mechanicalVentilationView->economizerComboBox->findData(QString::fromStdString(economizerControlType));
        
        m_mechanicalVentilationView->economizerComboBox->setCurrentIndex(economizerIndex);

        // Ventilation Calculation Method
        
        bingo = connect(m_mechanicalVentilationView->ventilationCalcMethodComboBox,SIGNAL(currentIndexChanged(int)),
                        this,SLOT(onVentilationCalcMethodComboBoxIndexChanged(int))); 
        OS_ASSERT(bingo);
        
        std::string ventilationMethod = controllerMechanicalVentilation.systemOutdoorAirMethod();
        
        int ventilationMethodIndex = m_mechanicalVentilationView->ventilationCalcMethodComboBox->findData(QString::fromStdString(ventilationMethod));
        
        m_mechanicalVentilationView->ventilationCalcMethodComboBox->setCurrentIndex(ventilationMethodIndex);
        
        // Demand Controlled Ventilation
        
        m_mechanicalVentilationView->dcvButton->bind(controllerMechanicalVentilation,"demandControlledVentilation");
      }
      else
      {
        m_noMechanicalVentilationView = new NoMechanicalVentilationView();

        m_hvacControlsView->ventilationViewSwitcher->setView(m_noMechanicalVentilationView);
      }

      // Supply Air Temperature
      boost::optional<model::SetpointManager> _spm;
      std::vector<model::SetpointManager> _setpointManagers = t_airLoopHVAC->supplyOutletNode().setpointManagers();
      for(std::vector<model::SetpointManager>::iterator it = _setpointManagers.begin();
          it != _setpointManagers.end();
          ++it)
      {
        if( istringEqual("Temperature", it->controlVariable()) ) 
        {
          _spm = *it;
          break;
        }
      }

      if( boost::optional<model::SetpointManagerSingleZoneReheat> spm = _spm->optionalCast<model::SetpointManagerSingleZoneReheat>() )
      {
        m_singleZoneReheatSPMView = new SingleZoneReheatSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_singleZoneReheatSPMView);

        std::vector<model::ThermalZone> thermalZones = t_airLoopHVAC->thermalZones();

        for( std::vector<model::ThermalZone>::const_iterator it = thermalZones.begin();
             it != thermalZones.end();
             ++it )
        {
          m_singleZoneReheatSPMView->controlZoneComboBox->addItem(QString::fromStdString(it->name().get()),it->handle().toString());
        }

        m_singleZoneReheatSPMView->controlZoneComboBox->addItem("",QUuid().toString());

        if( boost::optional<model::ThermalZone> tz = spm->controlZone() )
        {
          int index = m_singleZoneReheatSPMView->controlZoneComboBox->findData(tz->handle().toString());

          if( index > -1 )
          {
            m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(index);
          }
          else
          {
            m_singleZoneReheatSPMView->controlZoneComboBox->addItem(QString::fromStdString(tz->name().get()),tz->handle().toString());

            int i = m_singleZoneReheatSPMView->controlZoneComboBox->count() - 1;

            m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(i);
          }
        }
        else
        {
          int index = m_singleZoneReheatSPMView->controlZoneComboBox->findData(QUuid().toString());

          OS_ASSERT( index > -1 );

          m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(index);
        }
        
        bingo = connect(m_singleZoneReheatSPMView->controlZoneComboBox,SIGNAL(currentIndexChanged(int)),
                        this,SLOT(onControlZoneComboBoxChanged(int))); 
        OS_ASSERT(bingo);
      }
      else if( boost::optional<model::SetpointManagerScheduled> spm = _spm->optionalCast<model::SetpointManagerScheduled>() )
      {
        m_scheduledSPMView = new ScheduledSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_scheduledSPMView);

        SupplyAirTempScheduleVectorController * supplyAirTempScheduleVectorController = new SupplyAirTempScheduleVectorController();
        supplyAirTempScheduleVectorController->attach(spm.get());
        m_supplyAirTempScheduleDropZone = new OSDropZone(supplyAirTempScheduleVectorController);
        m_supplyAirTempScheduleDropZone->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        m_supplyAirTempScheduleDropZone->setMinItems(1);
        m_supplyAirTempScheduleDropZone->setMaxItems(1);
        m_supplyAirTempScheduleDropZone->setItemsRemoveable(false);
        m_supplyAirTempScheduleDropZone->setAcceptDrops(true);
        m_supplyAirTempScheduleDropZone->setItemsAcceptDrops(true);
        m_supplyAirTempScheduleDropZone->setEnabled(true);
        m_scheduledSPMView->supplyAirTemperatureViewSwitcher->setView(m_supplyAirTempScheduleDropZone);
      }
      else if( boost::optional<model::SetpointManagerFollowOutdoorAirTemperature> spm = _spm->optionalCast<model::SetpointManagerFollowOutdoorAirTemperature>() )
      {
        m_followOATempSPMView = new FollowOATempSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_followOATempSPMView);
      }
      else if( boost::optional<model::SetpointManagerOutdoorAirReset> spm = _spm->optionalCast<model::SetpointManagerOutdoorAirReset>() )
      {
        m_oaResetSPMView = new OAResetSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_oaResetSPMView);
      }
      else if( t_airLoopHVAC->supplyComponents(model::AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType()).size() > 0 )
      {
        model::AirLoopHVACUnitaryHeatPumpAirToAir hp = t_airLoopHVAC->supplyComponents(
            model::AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType()
          ).back().cast<model::AirLoopHVACUnitaryHeatPumpAirToAir>();

        m_airLoopHVACUnitaryHeatPumpAirToAirControlView = new AirLoopHVACUnitaryHeatPumpAirToAirControlView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_airLoopHVACUnitaryHeatPumpAirToAirControlView);

        std::vector<model::ThermalZone> thermalZones = t_airLoopHVAC->thermalZones();

        for( std::vector<model::ThermalZone>::const_iterator it = thermalZones.begin();
             it != thermalZones.end();
             ++it )
        {
          m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem(QString::fromStdString(it->name().get()),it->handle().toString());
        }

        m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem("",QUuid().toString());

        if( boost::optional<model::ThermalZone> tz = hp.controllingZone() )
        {
          int index = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->findData(tz->handle().toString());

          if( index > -1 )
          {
            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(index);
          }
          else
          {
            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem(QString::fromStdString(tz->name().get()),tz->handle().toString());

            int i = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->count() - 1;

            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(i);
          }
        }
        else
        {
          int index = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->findData(QUuid().toString());

          OS_ASSERT( index > -1 );

          m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(index);
        }
        
        bingo = connect(m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox,SIGNAL(currentIndexChanged(int)),
                        this,SLOT(onUnitaryHeatPumpControlZoneChanged(int))); 
        OS_ASSERT(bingo);
      }
      else
      {
        m_noSupplyAirTempControlView = new NoSupplyAirTempControlView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_noSupplyAirTempControlView);
      }
    }

    m_hvacControlsView->setUpdatesEnabled(true); 

    m_dirty = false;
  }
}

void HVACControlsController::onEconomizerComboBoxIndexChanged(int index)
{
  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

  OS_ASSERT(t_airLoopHVAC);

  boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = t_airLoopHVAC->airLoopHVACOutdoorAirSystem();

  OS_ASSERT(oaSystem);
  
  model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir();

  OS_ASSERT(m_mechanicalVentilationView);

  QString data = m_mechanicalVentilationView->economizerComboBox->itemData(index).toString();
  
  controllerOutdoorAir.setEconomizerControlType(data.toStdString());
}

void HVACControlsController::onVentilationCalcMethodComboBoxIndexChanged(int index)
{
  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

  OS_ASSERT(t_airLoopHVAC);

  boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = t_airLoopHVAC->airLoopHVACOutdoorAirSystem();

  OS_ASSERT(oaSystem);
  
  model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir();

  model::ControllerMechanicalVentilation controllerMechanicalVentilation = controllerOutdoorAir.controllerMechanicalVentilation();

  OS_ASSERT(m_mechanicalVentilationView);

  QString data = m_mechanicalVentilationView->ventilationCalcMethodComboBox->itemData(index).toString();

  controllerMechanicalVentilation.setSystemOutdoorAirMethod(data.toStdString());
}

void HVACControlsController::onNightCycleComboBoxIndexChanged(int index)
{
  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

  OS_ASSERT(t_airLoopHVAC);

  QString data = m_hvacControlsView->nightCycleComboBox->itemData(index).toString();

  t_airLoopHVAC->setNightCycleControlType(data.toStdString()); 
}

void HVACControlsController::onControlZoneComboBoxChanged(int index)
{
  QString stringHandle = m_singleZoneReheatSPMView->controlZoneComboBox->itemData(index).toString();

  QUuid handle(stringHandle);

  model::Model t_model = m_hvacSystemsController->model();

  boost::optional<model::ThermalZone> tz = t_model.getModelObject<model::ThermalZone>(handle);

  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

  OS_ASSERT(t_airLoopHVAC);

  std::vector<model::SetpointManagerSingleZoneReheat> setpointManagers = subsetCastVector<model::SetpointManagerSingleZoneReheat>(t_airLoopHVAC->supplyOutletNode().setpointManagers());
  if( ! setpointManagers.empty() ) {
    model::SetpointManagerSingleZoneReheat spm = setpointManagers.front();
    if( tz ) {
      spm.setControlZone(*tz);
    }
    else {
      spm.resetControlZone();
    }
  }
}
void HVACControlsController::onUnitaryHeatPumpControlZoneChanged(int index)
{
  QString stringHandle = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->itemData(index).toString();

  QUuid handle(stringHandle);

  model::Model t_model = m_hvacSystemsController->model();

  boost::optional<model::ThermalZone> tz = t_model.getModelObject<model::ThermalZone>(handle);

  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

  OS_ASSERT(t_airLoopHVAC);

  std::vector<model::ModelObject> modelObjects = t_airLoopHVAC->supplyComponents(model::AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());

  OS_ASSERT(modelObjects.size() > 0);

  model::AirLoopHVACUnitaryHeatPumpAirToAir hp = modelObjects.back().cast<model::AirLoopHVACUnitaryHeatPumpAirToAir>();

  if( tz )
  {
    hp.setControllingZone(tz.get());
  }
  else
  {
    hp.resetControllingZone();
  }
}

void HVACControlsController::updateLater()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(update()));
}

HVACLayoutController::HVACLayoutController(HVACSystemsController * hvacSystemsController)
  : QObject(),
    m_hvacSystemsController(hvacSystemsController)
{
  m_hvacGraphicsView = new HVACGraphicsView();

  //m_refrigerationController = std::shared_ptr<RefrigerationController>(new RefrigerationController());

  bool bingo;

  bingo = connect(m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->zoomOutButton,SIGNAL(clicked()),
                  m_hvacGraphicsView,SLOT(zoomOut()));
  OS_ASSERT(bingo);

  bingo = connect(m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->zoomInButton,SIGNAL(clicked()),
                  m_hvacGraphicsView,SLOT(zoomIn()));
  OS_ASSERT(bingo);

  updateLater();
}

HVACLayoutController::~HVACLayoutController()
{
  if( m_hvacGraphicsView )
  {
    delete m_hvacGraphicsView;
  }
}

//std::shared_ptr<RefrigerationController> HVACLayoutController::refrigerationController() const
//{
//  return m_refrigerationController;
//}

HVACGraphicsView * HVACLayoutController::hvacGraphicsView() const
{
  return m_hvacGraphicsView;
}

void HVACLayoutController::goToServiceWaterScene()
{
  // A null QUuid signals the service water scene

  m_hvacSystemsController->setCurrentHandle(QUuid().toString());
}

void HVACLayoutController::clearSceneSelection()
{
  if(QGraphicsScene * scene = m_hvacGraphicsView->scene())
  {
    scene->clearSelection();
  }
}

void HVACLayoutController::update()
{
  if( m_dirty )
  {
    model::Model t_model = m_hvacSystemsController->model();

    // Remove old stuff
    if( QGraphicsScene * oldScene = m_hvacGraphicsView->scene() )
    {
      oldScene->deleteLater();
    }

    QString handle = m_hvacSystemsController->currentHandle();

    boost::optional<model::ModelObject> mo = t_model.getModelObject<model::ModelObject>(handle);

    if( mo )
    {
      if( boost::optional<model::Loop> loop = mo->optionalCast<model::Loop>() )
      {
        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->showControls(true);

        LoopScene * loopScene = new LoopScene(loop.get());
        
        m_hvacGraphicsView->setScene(loopScene);

        bool bingo;
        
        bingo = connect( loopScene,
                          SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
                          this,
                          SLOT(onModelObjectSelected(model::OptionalModelObject &, bool )) );
        OS_ASSERT(bingo);
        
        bingo = connect( loopScene,
                        SIGNAL(removeModelObjectClicked(model::ModelObject & )),
                        this,
                        SLOT(removeModelObject(model::ModelObject & )) );
        OS_ASSERT(bingo);
        
        bingo = connect( loopScene,
                         SIGNAL(innerNodeClicked(model::ModelObject & )),
                         this,
                         SLOT(goToOtherLoop(model::ModelObject & )) );
        OS_ASSERT(bingo);
        
        bingo = connect( loopScene,SIGNAL(hvacComponentDropped(OSItemId, model::HVACComponent &)),
                         this,SLOT(addLibraryObjectToModelNode(OSItemId ,model::HVACComponent &)));
        OS_ASSERT(bingo);
      }
      else if( boost::optional<model::WaterUseConnections> waterUseConnections = mo->optionalCast<model::WaterUseConnections>() )
      {
        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->showControls(false);

        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->label->setText(QString::fromStdString(waterUseConnections->name().get()));

        WaterUseConnectionsDetailScene * waterUseConnectionsScene = new WaterUseConnectionsDetailScene(waterUseConnections.get());

        m_hvacGraphicsView->setScene(waterUseConnectionsScene);

        bool bingo = connect( waterUseConnectionsScene,SIGNAL(hvacComponentDropped(OSItemId, model::HVACComponent &)),
                              this,SLOT(addLibraryObjectToModelNode(OSItemId ,model::HVACComponent &)));
        OS_ASSERT(bingo);

        bingo = connect( waterUseConnectionsScene,
                         SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
                         this,
                         SLOT(onModelObjectSelected(model::OptionalModelObject &, bool )) );
        OS_ASSERT(bingo);

        connect( waterUseConnectionsScene,
                 SIGNAL(goToServiceWaterSceneClicked()),
                 this,
                 SLOT(goToServiceWaterScene()) );

        bingo = connect( waterUseConnectionsScene,
                         SIGNAL(innerNodeClicked(model::ModelObject & )),
                         this,
                         SLOT(goToOtherLoop(model::ModelObject & )) );
        OS_ASSERT(bingo);

        bingo = connect( waterUseConnectionsScene,
                         SIGNAL(removeModelObjectClicked(model::ModelObject & )),
                         this,
                         SLOT(removeModelObject(model::ModelObject & )) );
        OS_ASSERT(bingo);
      }
    }
    //else if(handle == REFRIGERATION)
    //{
    //  //RefrigerationScene * refrigerationScene = new RefrigerationScene();

    //  //m_hvacGraphicsView->setScene(m_refrigerationController->refrigerationScene().data());
    //}
    else
    { 
      m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->showControls(true);

      ServiceWaterScene * serviceWaterScene = new ServiceWaterScene(t_model);

      m_hvacGraphicsView->setScene(serviceWaterScene);

      bool bingo;

      bingo = connect(serviceWaterScene,SIGNAL(hvacComponentDropped(OSItemId)),
                      this,SLOT(addLibraryObjectToTopLevel(OSItemId)));
      OS_ASSERT(bingo);

      bingo = connect(serviceWaterScene,
                      SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
                      this,
                      SLOT(onModelObjectSelected(model::OptionalModelObject &, bool )) );
      OS_ASSERT(bingo);

      bingo = connect(serviceWaterScene,
                      SIGNAL(removeModelObjectClicked(model::ModelObject & )),
                      this,
                      SLOT(removeModelObject(model::ModelObject & )) );
      OS_ASSERT(bingo);
    }
  }

  m_dirty = false;
}

void HVACLayoutController::updateLater()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(update()));
}

void SystemAvailabilityVectorController::attach(const model::ModelObject& modelObject)
{
  detach();

  m_modelObject = modelObject;

  if( m_modelObject )
  {
    m_model = m_modelObject->model();

    connect(m_model->getImpl<model::detail::Model_Impl>().get(),
            SIGNAL(onChange()),
            this,
            SLOT(reportItemsLater()));
  }

  reportItemsLater();
}

void SystemAvailabilityVectorController::detach()
{
  if( m_modelObject )
  {
    m_modelObject.reset();
  }

  if( m_model )
  {
    disconnect(m_model->getImpl<model::detail::Model_Impl>().get(),
               SIGNAL(onChange()),
               this,
               SLOT(reportItemsLater()));

    m_model.reset();
  }

  reportItemsLater();
}

void SystemAvailabilityVectorController::reportItemsLater()
{
  m_reportScheduled = true;

  QTimer::singleShot(0,this,SLOT(reportItems()));
}

void SystemAvailabilityVectorController::reportItems()
{
  if( m_reportScheduled )
  {
    m_reportScheduled = false;

    ModelObjectVectorController::reportItems();
  }
}

boost::optional<model::AirLoopHVAC> 
  SystemAvailabilityVectorController::airLoopHVAC()
{
  if( m_modelObject && ! m_modelObject->handle().isNull() )
  {
    return m_modelObject->optionalCast<model::AirLoopHVAC>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> SystemAvailabilityVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::AirLoopHVAC> _airLoopHVAC = airLoopHVAC() )
  {
    result.push_back(modelObjectToItemId(_airLoopHVAC->availabilitySchedule(),false));
  }

  return result;
}

void SystemAvailabilityVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::AirLoopHVAC> _airLoopHVAC = airLoopHVAC() )
  {
    boost::optional<model::ModelObject> mo = this->getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(_airLoopHVAC->model());
        }

        model::Schedule schedule = mo->cast<model::Schedule>();
        _airLoopHVAC->setAvailabilitySchedule(schedule);
      }
    }
  }
}

void SystemAvailabilityVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SupplyAirTempScheduleVectorController::attach(const model::ModelObject& modelObject)
{
  detach();

  m_modelObject = modelObject;

  if( m_modelObject )
  {
    m_model = m_modelObject->model();

    connect(m_model->getImpl<model::detail::Model_Impl>().get(),
            SIGNAL(onChange()),
            this,
            SLOT(reportItemsLater()));
  }

  reportItemsLater();
}

void SupplyAirTempScheduleVectorController::detach()
{
  if( m_modelObject )
  {
    m_modelObject.reset();
  }

  if( m_model )
  {
    disconnect(m_model->getImpl<model::detail::Model_Impl>().get(),
               SIGNAL(onChange()),
               this,
               SLOT(reportItemsLater()));

    m_model.reset();
  }

  reportItemsLater();
}

void SupplyAirTempScheduleVectorController::reportItemsLater()
{
  m_reportScheduled = true;

  QTimer::singleShot(0,this,SLOT(reportItems()));
}

void SupplyAirTempScheduleVectorController::reportItems()
{
  if( m_reportScheduled )
  {
    m_reportScheduled = false;

    ModelObjectVectorController::reportItems();
  }
}

boost::optional<model::SetpointManagerScheduled> 
  SupplyAirTempScheduleVectorController::setpointManagerScheduled()
{
  if( m_modelObject && ! m_modelObject->handle().isNull() )
  {
    return m_modelObject->optionalCast<model::SetpointManagerScheduled>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> SupplyAirTempScheduleVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::SetpointManagerScheduled> t_setpointManagerScheduled = setpointManagerScheduled() )
  {
    if( boost::optional<model::Schedule> schedule = t_setpointManagerScheduled->schedule() )
    {
      result.push_back(modelObjectToItemId(schedule.get(),false));
    }
  }

  return result;
}

void SupplyAirTempScheduleVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::SetpointManagerScheduled> t_setpointManagerScheduled = setpointManagerScheduled() )
  {
    boost::optional<model::ModelObject> mo = getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(t_setpointManagerScheduled->model());
        }

        model::Schedule schedule = mo->cast<model::Schedule>();
        t_setpointManagerScheduled->setSchedule(schedule);
      }
    }
  }
}

void SupplyAirTempScheduleVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

} // openstudio

