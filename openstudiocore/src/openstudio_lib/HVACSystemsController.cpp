/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>

#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QMutex>

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

  m_model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObject.connect<HVACSystemsController, &HVACSystemsController::onObjectAdded>(this);
  //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAdded, this, &HVACSystemsController::onObjectAdded, Qt::QueuedConnection);

  m_model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObject.connect<HVACSystemsController, &HVACSystemsController::onObjectRemoved>(this);
  //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemoved, this, &HVACSystemsController::onObjectRemoved, Qt::QueuedConnection);

  connect(m_hvacSystemsView->hvacToolbarView->addButton, &QPushButton::clicked, this, &HVACSystemsController::onAddSystemClicked);

  connect(m_hvacSystemsView->hvacToolbarView->deleteButton, &QPushButton::clicked, this, &HVACSystemsController::onRemoveLoopClicked);

  connect(m_hvacSystemsView->hvacToolbarView->systemComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &HVACSystemsController::onSystemComboBoxIndexChanged);

  connect(m_hvacSystemsView->hvacToolbarView->topologyViewButton, &QPushButton::clicked, this, &HVACSystemsController::onShowTopologyClicked);

  connect(m_hvacSystemsView->hvacToolbarView->controlsViewButton, &QPushButton::clicked, this, &HVACSystemsController::onShowControlsClicked);

  connect(m_hvacSystemsView->hvacToolbarView->gridViewButton, &QPushButton::clicked, this, &HVACSystemsController::onShowGridClicked);

  m_updateMutex = new QMutex();

  updateLater();
}

HVACSystemsController::~HVACSystemsController()
{
  if( m_hvacSystemsView ) { delete m_hvacSystemsView; }

  delete m_updateMutex;
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
  if( ! m_updateMutex->tryLock() ) {
    return;
  }

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
    auto airloops = m_model.getModelObjects<model::AirLoopHVAC>();
    std::sort(airloops.begin(),airloops.end(),WorkspaceObjectNameLess());
    for( auto it = airloops.begin(); it != airloops.end(); ++it ) {
      systemComboBox->addItem(QString::fromStdString(it->name().get()), toQString(it->handle()));
    }

    auto plantloops = m_model.getModelObjects<model::PlantLoop>();
    std::sort(plantloops.begin(),plantloops.end(),WorkspaceObjectNameLess());
    for( auto it = plantloops.begin(); it != plantloops.end(); ++it ) {
      systemComboBox->addItem(QString::fromStdString(it->name().get()), toQString(it->handle()));
    }

    systemComboBox->addItem("Service Hot Water",SHW);
    systemComboBox->addItem("Refrigeration",REFRIGERATION);
    systemComboBox->addItem("VRF",VRF);

    // Set system combo box current index
    QString handle = currentHandle();
    if( handle == SHW  ||
        m_model.getModelObject<model::WaterUseConnections>(toUUID(handle))
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

        connect(this, &HVACSystemsController::toggleUnitsClicked, m_refrigerationGridController.get()->refrigerationGridView(), &RefrigerationGridView::toggleUnitsClicked);

        connect(this, &HVACSystemsController::toggleUnitsClicked, this, &HVACSystemsController::toggleUnits);

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

        connect(this, &HVACSystemsController::toggleUnitsClicked, m_refrigerationGridController.get()->refrigerationGridView(), &RefrigerationGridView::toggleUnitsClicked);

        connect(this, &HVACSystemsController::toggleUnitsClicked, this, &HVACSystemsController::toggleUnits);

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

  m_updateMutex->unlock();
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

void HVACSystemsController::onObjectAdded(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  std::vector<IddObjectType> types = systemComboBoxTypes();

  if(std::find(types.begin(),types.end(),workspaceObject.cast<model::ModelObject>().iddObjectType()) != types.end())
  {
    updateLater();
  }
}

void HVACSystemsController::onObjectRemoved(const WorkspaceObject& workspaceObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
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
  model::OptionalModelObject object;
  bool remove = false;
  bool added = false;
  auto doc = OSAppBase::instance()->currentDocument();

  object = doc->getModelObject(itemid);
  if( object ) {
    if( ! doc->fromModel(itemid) ) {
      object = object->clone(comp.model());
      remove = true;
    }
  }

  if( auto component = doc->getComponent(itemid) ) {
    // Ugly hack to avoid the component being treated as a resource.
    component->componentData().setString(OS_ComponentDataFields::UUID, toString(createUUID()));
    if( auto componentData = comp.model().insertComponent(component.get()) ) {
      object = componentData->primaryComponentObject();
      remove = true;
    }
  }

  if( object ) {
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
  return m_model.getModelObject<model::Loop>(toUUID(m_currentHandle));
}

void HVACLayoutController::removeModelObject(model::ModelObject & modelObject)
{
  if( modelObject.handle().isNull() ) return;

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
    m_hvacSystemsController->setCurrentHandle(toQString(loop->handle()));

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
          m_hvacSystemsController->setCurrentHandle(toQString(plantLoop->handle()));
        }

        return;
      }
      else if( boost::optional<model::PlantLoop> plantLoop = t_currentLoop->optionalCast<model::PlantLoop>() )
      {
        if( boost::optional<model::AirLoopHVAC> airLoopHVAC = comp->airLoopHVAC() )
        {
          m_hvacSystemsController->setCurrentHandle(toQString(airLoopHVAC->handle()));
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
            m_hvacSystemsController->setCurrentHandle(toQString(secondaryPlantLoop->handle()));

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
            m_hvacSystemsController->setCurrentHandle(toQString(plantLoop->handle()));

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
	  case ADDTOMODEL_SPLIT_TYPE_0_THAI:
	  {
		  loop = model::addSplitTypeHVACTHAIType0(m_model);
		  break;
	  }
	  case ADDTOMODEL_SPLIT_TYPE_1_THAI:
	  {
		  loop = model::addSplitTypeHVACTHAIType1(m_model);
		  break;
	  }
	  case ADDTOMODEL_SPLIT_TYPE_2_THAI:
	  {
		  loop = model::addSplitTypeHVACTHAIType2(m_model);
		  break;
	  }
	  case ADDTOMODEL_CENTRAL_SYSTEM_TYPE_0_THAI:
	  {
		  loop = model::addCentralSystemWithAirCooled(m_model);
		  break;
	  }
	  case ADDTOMODEL_CENTRAL_SYSTEM_TYPE_1_THAI:
	  {
		  loop = model::addCentralSystemWithWaterCooled(m_model);
		  break;
	  }
	  case ADDTOMODEL_AIRLOOPHVAC_1_THAI:
	  {
		  loop = model::addThaiAirLoop(m_model);
		  break;
	  }
	  case ADDTOMODEL_AIRLOOPHVAC_2_THAI:
	  {
		  loop = model::addThaizAirLoopWithERV(m_model);
		  break;
	  }
	case ADDTOMODEL_AIRLOOPHVAC_THAI:
	  {
		  loop = model::airLoopHVACTHAI(m_model);
		  break;
	  }
	case ADDTOMODEL_AIRLOOPHVAC:
      {
          model::AirLoopHVAC airLoopHVAC(m_model);
          loop = airLoopHVAC;
          break;
      }
    case ADDTOMODEL_DUAL_AIRLOOPHVAC:
    {
      model::AirLoopHVAC airLoopHVAC(m_model,true);

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
    m_currentHandle = toQString(loop->handle());
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

    int i = chooser->findData(toQString(loop->handle()));

    if( i == 0 )
    {
      setCurrentHandle(chooser->itemData(i + 1).toString());
    }
    else
    {
      setCurrentHandle(chooser->itemData(i - 1).toString());
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
      m_hvacSystemsController->setCurrentHandle(toQString(waterUseConnections->handle()));
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

      auto systemAvailabilityVectorController = new SystemAvailabilityVectorController();
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

      connect(m_hvacControlsView->nightCycleComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &HVACControlsController::onNightCycleComboBoxIndexChanged);

      // Mechanical Ventilation

      if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = t_airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir();

        model::ControllerMechanicalVentilation controllerMechanicalVentilation = controllerOutdoorAir.controllerMechanicalVentilation();

        m_mechanicalVentilationView = new MechanicalVentilationView();

        m_hvacControlsView->ventilationViewSwitcher->setView(m_mechanicalVentilationView);

        // Economizer Control Type

        connect(m_mechanicalVentilationView->economizerComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &HVACControlsController::onEconomizerComboBoxIndexChanged);

        std::string economizerControlType = controllerOutdoorAir.getEconomizerControlType();

        int economizerIndex = m_mechanicalVentilationView->economizerComboBox->findData(QString::fromStdString(economizerControlType));

        m_mechanicalVentilationView->economizerComboBox->setCurrentIndex(economizerIndex);

        // Ventilation Calculation Method

        connect(m_mechanicalVentilationView->ventilationCalcMethodComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &HVACControlsController::onVentilationCalcMethodComboBoxIndexChanged);

        std::string ventilationMethod = controllerMechanicalVentilation.systemOutdoorAirMethod();

        int ventilationMethodIndex = m_mechanicalVentilationView->ventilationCalcMethodComboBox->findData(QString::fromStdString(ventilationMethod));

        m_mechanicalVentilationView->ventilationCalcMethodComboBox->setCurrentIndex(ventilationMethodIndex);

        // Demand Controlled Ventilation

        // m_mechanicalVentilationView->dcvButton->bind(controllerMechanicalVentilation,"demandControlledVentilation");
        m_mechanicalVentilationView->dcvButton->bind(
          controllerMechanicalVentilation,
          std::bind(&model::ControllerMechanicalVentilation::demandControlledVentilation, controllerMechanicalVentilation),
          boost::optional<BoolSetter>(std::bind(&model::ControllerMechanicalVentilation::setDemandControlledVentilation, controllerMechanicalVentilation, std::placeholders::_1)),
          boost::optional<NoFailAction>(std::bind(&model::ControllerMechanicalVentilation::resetDemandControlledVentilation, controllerMechanicalVentilation)),
          boost::optional<BasicQuery>(std::bind(&model::ControllerMechanicalVentilation::isDemandControlledVentilationDefaulted, controllerMechanicalVentilation))
        );
      }
      else
      {
        m_noMechanicalVentilationView = new NoMechanicalVentilationView();

        m_hvacControlsView->ventilationViewSwitcher->setView(m_noMechanicalVentilationView);
      }

      // Supply Air Temperature
      boost::optional<model::SetpointManager> _spm;
      std::vector<model::SetpointManager> _setpointManagers = t_airLoopHVAC->supplyOutletNode().setpointManagers();
      for(auto it = _setpointManagers.begin();
          it != _setpointManagers.end();
          ++it)
      {
        if( istringEqual("Temperature", it->controlVariable()) )
        {
          _spm = *it;
          break;
        }
      }

      boost::optional<model::SetpointManagerSingleZoneReheat> spmSZR;
      boost::optional<model::SetpointManagerScheduled> spmS;
      boost::optional<model::SetpointManagerFollowOutdoorAirTemperature> spmFOAT;
      boost::optional<model::SetpointManagerOutdoorAirReset> spmOAR;

      if( _spm && ( spmSZR = _spm->optionalCast<model::SetpointManagerSingleZoneReheat>() ) )
      {
        m_singleZoneReheatSPMView = new SingleZoneReheatSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_singleZoneReheatSPMView);

        std::vector<model::ThermalZone> thermalZones = t_airLoopHVAC->thermalZones();

        for( std::vector<model::ThermalZone>::const_iterator it = thermalZones.begin();
             it != thermalZones.end();
             ++it )
        {
          m_singleZoneReheatSPMView->controlZoneComboBox->addItem(QString::fromStdString(it->name().get()), toQString(it->handle()));
        }

        m_singleZoneReheatSPMView->controlZoneComboBox->addItem("",toQString(UUID()));

        if( boost::optional<model::ThermalZone> tz = spmSZR->controlZone() )
        {
          int index = m_singleZoneReheatSPMView->controlZoneComboBox->findData(toQString(tz->handle()));

          if( index > -1 )
          {
            m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(index);
          }
          else
          {
            m_singleZoneReheatSPMView->controlZoneComboBox->addItem(QString::fromStdString(tz->name().get()), toQString(tz->handle()));

            int i = m_singleZoneReheatSPMView->controlZoneComboBox->count() - 1;

            m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(i);
          }
        }
        else
        {
          int index = m_singleZoneReheatSPMView->controlZoneComboBox->findData(toQString(UUID()));

          OS_ASSERT( index > -1 );

          m_singleZoneReheatSPMView->controlZoneComboBox->setCurrentIndex(index);
        }

        connect(m_singleZoneReheatSPMView->controlZoneComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &HVACControlsController::onControlZoneComboBoxChanged);
      }
      else if( _spm && ( spmS = _spm->optionalCast<model::SetpointManagerScheduled>() ) )
      {
        m_scheduledSPMView = new ScheduledSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_scheduledSPMView);

        auto supplyAirTempScheduleVectorController = new SupplyAirTempScheduleVectorController();
        supplyAirTempScheduleVectorController->attach(spmS.get());
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
      else if( _spm && ( spmFOAT = _spm->optionalCast<model::SetpointManagerFollowOutdoorAirTemperature>() ) )
      {
        m_followOATempSPMView = new FollowOATempSPMView();

        m_hvacControlsView->supplyAirTemperatureViewSwitcher->setView(m_followOATempSPMView);
      }
      else if( _spm && ( spmOAR = _spm->optionalCast<model::SetpointManagerOutdoorAirReset>() ) )
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
          m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem(QString::fromStdString(it->name().get()), toQString(it->handle()));
        }

        m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem("",toQString(UUID()));

        if( boost::optional<model::ThermalZone> tz = hp.controllingZone() )
        {
          int index = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->findData(toQString(tz->handle()));

          if( index > -1 )
          {
            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(index);
          }
          else
          {
            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->addItem(QString::fromStdString(tz->name().get()),toQString(tz->handle()));

            int i = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->count() - 1;

            m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(i);
          }
        }
        else
        {
          int index = m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->findData(toQString(UUID()));

          OS_ASSERT( index > -1 );

          m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox->setCurrentIndex(index);
        }

        connect(m_airLoopHVACUnitaryHeatPumpAirToAirControlView->controlZoneComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &HVACControlsController::onUnitaryHeatPumpControlZoneChanged);
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

  UUID handle(toUUID(stringHandle));

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

  UUID handle(toUUID(stringHandle));

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

  connect(m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->zoomOutButton, &QPushButton::clicked, m_hvacGraphicsView.data(), &HVACGraphicsView::zoomOut);

  connect(m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->zoomInButton, &QPushButton::clicked, m_hvacGraphicsView.data(), &HVACGraphicsView::zoomIn);

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
  // A null UUID signals the service water scene

  m_hvacSystemsController->setCurrentHandle(toQString(UUID()));
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

    boost::optional<model::ModelObject> mo = t_model.getModelObject<model::ModelObject>(toUUID(handle));

    if( mo )
    {
      if( boost::optional<model::Loop> loop = mo->optionalCast<model::Loop>() )
      {
        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->showControls(true);

        LoopScene * loopScene = new LoopScene(loop.get());

        m_hvacGraphicsView->setScene(loopScene);


        connect(loopScene, &LoopScene::modelObjectSelected, this, &HVACLayoutController::onModelObjectSelected);

        connect(loopScene, &LoopScene::removeModelObjectClicked, this, &HVACLayoutController::removeModelObject);

        connect(loopScene, &LoopScene::innerNodeClicked, this, &HVACLayoutController::goToOtherLoop);

        connect(loopScene, static_cast<void (LoopScene::*)(OSItemId, model::HVACComponent &)>(&LoopScene::hvacComponentDropped), this, &HVACLayoutController::addLibraryObjectToModelNode);
      }
      else if( boost::optional<model::WaterUseConnections> waterUseConnections = mo->optionalCast<model::WaterUseConnections>() )
      {
        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->showControls(false);

        m_hvacSystemsController->hvacSystemsView()->hvacToolbarView->label->setText(QString::fromStdString(waterUseConnections->name().get()));

        auto waterUseConnectionsScene = new WaterUseConnectionsDetailScene(waterUseConnections.get());

        m_hvacGraphicsView->setScene(waterUseConnectionsScene);

        connect(waterUseConnectionsScene, static_cast<void (WaterUseConnectionsDetailScene::*)(OSItemId, model::HVACComponent &)>(&WaterUseConnectionsDetailScene::hvacComponentDropped), this, &HVACLayoutController::addLibraryObjectToModelNode);

        connect(waterUseConnectionsScene, &WaterUseConnectionsDetailScene::modelObjectSelected, this, &HVACLayoutController::onModelObjectSelected);

        connect(waterUseConnectionsScene, &WaterUseConnectionsDetailScene::goToServiceWaterSceneClicked, this, &HVACLayoutController::goToServiceWaterScene);

        connect(waterUseConnectionsScene, &WaterUseConnectionsDetailScene::innerNodeClicked, this, &HVACLayoutController::goToOtherLoop);

        connect(waterUseConnectionsScene, &WaterUseConnectionsDetailScene::removeModelObjectClicked, this, &HVACLayoutController::removeModelObject);
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

      auto serviceWaterScene = new ServiceWaterScene(t_model);

      m_hvacGraphicsView->setScene(serviceWaterScene);

      connect(serviceWaterScene, static_cast<void (ServiceWaterScene::*)(OSItemId)>(&ServiceWaterScene::hvacComponentDropped), this, &HVACLayoutController::addLibraryObjectToTopLevel);

      connect(serviceWaterScene, &ServiceWaterScene::modelObjectSelected, this, &HVACLayoutController::onModelObjectSelected);

      connect(serviceWaterScene, &ServiceWaterScene::removeModelObjectClicked, this, &HVACLayoutController::removeModelObject);
    }
  }

  m_dirty = false;
}

void HVACLayoutController::updateLater()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(update()));
}

SystemAvailabilityVectorController::SystemAvailabilityVectorController() 
  : ModelObjectVectorController()
{
  m_reportItemsMutex = new QMutex();
}

void SystemAvailabilityVectorController::attach(const model::ModelObject& modelObject)
{
  detach();

  m_modelObject = modelObject;

  if( m_modelObject )
  {
    m_model = m_modelObject->model();

    m_model->getImpl<model::detail::Model_Impl>().get()->onChange.connect<SystemAvailabilityVectorController, &SystemAvailabilityVectorController::reportItemsLater>(this);
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
    m_model->getImpl<model::detail::Model_Impl>().get()->onChange.disconnect<SystemAvailabilityVectorController, &SystemAvailabilityVectorController::reportItemsLater>(this);

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
  if( ! m_reportItemsMutex->tryLock() ) {
    return;
  }

  if( m_reportScheduled )
  {
    m_reportScheduled = false;

    ModelObjectVectorController::reportItems();
  }

  m_reportItemsMutex->unlock();
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

SupplyAirTempScheduleVectorController::SupplyAirTempScheduleVectorController()
  : ModelObjectVectorController()
{
  m_reportItemsMutex = new QMutex();
}

void SupplyAirTempScheduleVectorController::attach(const model::ModelObject& modelObject)
{
  detach();

  m_modelObject = modelObject;

  if( m_modelObject )
  {
    m_model = m_modelObject->model();

    m_model->getImpl<model::detail::Model_Impl>().get()->onChange.connect<SupplyAirTempScheduleVectorController, &SupplyAirTempScheduleVectorController::reportItemsLater>(this);
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
    m_model->getImpl<model::detail::Model_Impl>().get()->onChange.disconnect<SupplyAirTempScheduleVectorController, &SupplyAirTempScheduleVectorController::reportItemsLater>(this);

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
  if( ! m_reportItemsMutex->tryLock() ) {
    return;
  }

  if( m_reportScheduled )
  {
    m_reportScheduled = false;

    ModelObjectVectorController::reportItems();
  }

  m_reportItemsMutex->unlock();
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
