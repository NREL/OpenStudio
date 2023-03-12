/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "CoilUserDefined.hpp"
#include "CoilUserDefined_Impl.hpp"
#include "Model.hpp"
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_UserDefined_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilUserDefined_Impl::CoilUserDefined_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilUserDefined::iddObjectType());
  }

  CoilUserDefined_Impl::CoilUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilUserDefined::iddObjectType());
  }

  CoilUserDefined_Impl::CoilUserDefined_Impl(const CoilUserDefined_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilUserDefined_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::vector<openstudio::IdfObject> CoilUserDefined_Impl::remove() {
      if (isRemovable()) {
          return WaterToAirComponent_Impl::remove();
      }
      return {};
  }

  IddObjectType CoilUserDefined_Impl::iddObjectType() const {
    return CoilUserDefined::iddObjectType();
  }

  ModelObject CoilUserDefined_Impl::clone(Model model) const {
      auto newCoilUserDefined = ModelObject_Impl::clone(model).cast<CoilUserDefined>();
      //airOutletTemperatureActuator
      if (boost::optional<EnergyManagementSystemActuator> object = airOutletTemperatureActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          //DO THIS HERE OR IN THE CoilUserDefined_Impl::airOutletTemperatureActuator() method??
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setAirOutletTemperatureActuator(objectClone);
      }
      //airOutletHumidityRatioActuator
      if (boost::optional<EnergyManagementSystemActuator> object = airOutletHumidityRatioActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setAirOutletHumidityRatioActuator(objectClone);
      }
      //airMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = airMassFlowRateActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setAirMassFlowRateActuator(objectClone);
      }
      //plantMinimumMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = plantMinimumMassFlowRateActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setPlantMinimumMassFlowRateActuator(objectClone);
      }
      //plantMaximumMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = plantMaximumMassFlowRateActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setPlantMaximumMassFlowRateActuator(objectClone);
      }
      //plantDesignVolumeFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = plantDesignVolumeFlowRateActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setPlantDesignVolumeFlowRateActuator(objectClone);
      }
      //plantMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = plantMassFlowRateActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setPlantMassFlowRateActuator(objectClone);
      }
      //plantOutletTemperatureActuator
      if (boost::optional<EnergyManagementSystemActuator> object = plantOutletTemperatureActuator()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
          objectClone.setActuatedComponent(newCoilUserDefined);
          newCoilUserDefined.setPlantOutletTemperatureActuator(objectClone);
      }
      //overallModelSimulationProgramCallingManager
      if (boost::optional<EnergyManagementSystemProgramCallingManager> object = overallModelSimulationProgramCallingManager()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemProgramCallingManager>();
          //clone the program referenced in the callingManager
          if (boost::optional<EnergyManagementSystemProgram> program = overallSimulationProgram()) {
              auto cloneProgram = program.get().clone(model).cast<EnergyManagementSystemProgram>();
              //add cloned program to cloned PCUD
              newCoilUserDefined.setOverallSimulationProgram(cloneProgram);
              //add cloned program to cloned programCallingManager
              objectClone.erasePrograms();
              objectClone.addProgram(cloneProgram);
          }
          newCoilUserDefined.setOverallModelSimulationProgramCallingManager(objectClone);
      }
      //modelSetupandSizingProgramCallingManager
      if (boost::optional<EnergyManagementSystemProgramCallingManager> object = modelSetupandSizingProgramCallingManager()) {
          auto objectClone = object.get().clone(model).cast<EnergyManagementSystemProgramCallingManager>();
          //clone the program referenced in the callingManager
          if (boost::optional<EnergyManagementSystemProgram> program = initializationSimulationProgram()) {
              auto cloneProgram = program.get().clone(model).cast<EnergyManagementSystemProgram>();
              //add cloned program to cloned PCUD
              newCoilUserDefined.setInitializationSimulationProgram(cloneProgram);
              //add cloned program to cloned programCallingManager
              objectClone.erasePrograms();
              objectClone.addProgram(cloneProgram);
          }
          newCoilUserDefined.setOverallModelSimulationProgramCallingManager(objectClone);
      }
      //ambientZone
      if (boost::optional<ThermalZone> object = ambientZone()) {
          //ThermalZone objectClone = object.get().clone(model).cast<ThermalZone>();
          newCoilUserDefined.setAmbientZone(object.get());
      }
      return std::move(newCoilUserDefined);
  }

  unsigned CoilUserDefined_Impl::airInletPort() const {
      return OS_Coil_UserDefinedFields::AirConnection1InletNodeName;
  }

  unsigned CoilUserDefined_Impl::airOutletPort() const {
      return OS_Coil_UserDefinedFields::AirConnection1OutletNodeName;
  }

  unsigned CoilUserDefined_Impl::waterInletPort() const {
      return OS_Coil_UserDefinedFields::PlantConnectionInletNodeName;
  }

  unsigned CoilUserDefined_Impl::waterOutletPort() const {
      return OS_Coil_UserDefinedFields::PlantConnectionOutletNodeName;
  }

  std::vector<IddObjectType> CoilUserDefined_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager);
      result.push_back(IddObjectType::OS_EnergyManagementSystem_Program);
      result.push_back(IddObjectType::OS_EnergyManagementSystem_Actuator);
      result.push_back(IddObjectType::OS_Coil_UserDefined);
      return result;
  }

  // Returns the children object
  std::vector<ModelObject> CoilUserDefined_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<EnergyManagementSystemProgramCallingManager> omsmpcm = overallModelSimulationProgramCallingManager()) {
          result.push_back(omsmpcm.get());
      }
      if (boost::optional<EnergyManagementSystemProgramCallingManager> msspcm = modelSetupandSizingProgramCallingManager()) {
          result.push_back(msspcm.get());
      }
      if (boost::optional<EnergyManagementSystemProgram> osp = overallSimulationProgram()) {
          result.push_back(osp.get());
      }
      if (boost::optional<EnergyManagementSystemProgram> isp = initializationSimulationProgram()) {
          result.push_back(isp.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> aota = airOutletTemperatureActuator()) {
          result.push_back(aota.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> aohra = airOutletHumidityRatioActuator()) {
          result.push_back(aohra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> amfra = airMassFlowRateActuator()) {
          result.push_back(amfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pminmfra = plantMinimumMassFlowRateActuator()) {
          result.push_back(pminmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pmaxmfra = plantMaximumMassFlowRateActuator()) {
          result.push_back(pmaxmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pdvfra = plantDesignVolumeFlowRateActuator()) {
          result.push_back(pdvfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pmmfra = plantMassFlowRateActuator()) {
          result.push_back(pmmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pota = plantOutletTemperatureActuator()) {
          result.push_back(pota.get());
      }
      return result;
  }
  boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::overallModelSimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::modelSetupandSizingProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgram> CoilUserDefined_Impl::overallSimulationProgram() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgram>(OS_Coil_UserDefinedFields::OverallSimulationProgramName);
  }

  boost::optional<EnergyManagementSystemProgram> CoilUserDefined_Impl::initializationSimulationProgram() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgram>(
          OS_Coil_UserDefinedFields::InitializationSimulationProgramName);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::airOutletTemperatureActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::AirOutletTemperatureActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::airOutletHumidityRatioActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::AirOutletHumidityRatioActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::airMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::AirMassFlowRateActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::plantMinimumMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::PlantMinimumMassFlowRateActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::plantMaximumMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::PlantMaximumMassFlowRateActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::plantDesignVolumeFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::PlantDesignVolumeFlowRateActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::plantMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::PlantMassFlowRateActuator);
  }

  boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::plantOutletTemperatureActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
          OS_Coil_UserDefinedFields::PlantOutletTemperatureActuator);
  }

  bool CoilUserDefined_Impl::plantConnectionisUsed() const {
    boost::optional<std::string> value = getString(OS_Coil_UserDefinedFields::PlantConnectionisUsed, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True"); 
  }

  boost::optional<ThermalZone> CoilUserDefined_Impl::ambientZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Coil_UserDefinedFields::AmbientZoneName);
  }

  bool CoilUserDefined_Impl::setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram) {
    bool result = setPointer(OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, erlProgram.handle());
    return result;
  }

  void CoilUserDefined_Impl::resetOverallModelSimulationProgramCallingManager() {
    bool result = setString(OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool CoilUserDefined_Impl::setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram) {
    bool result = setPointer(OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, erlProgram.handle());
    return result;
  }

  void CoilUserDefined_Impl::resetModelSetupandSizingProgramCallingManager() {
      bool result = setString(OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, "");
      OS_ASSERT(result);
  }

  bool CoilUserDefined_Impl::setOverallSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram) {
      bool result = setPointer(OS_Coil_UserDefinedFields::OverallSimulationProgramName, energyManagementSystemProgram.handle());
      return result;
  }

  void CoilUserDefined_Impl::resetOverallSimulationProgram() {
      bool result = setString(OS_Coil_UserDefinedFields::OverallSimulationProgramName, "");
      OS_ASSERT(result);
  }

  bool CoilUserDefined_Impl::setInitializationSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram) {
      bool result = setPointer(OS_Coil_UserDefinedFields::InitializationSimulationProgramName, energyManagementSystemProgram.handle());
      return result;
  }

  void CoilUserDefined_Impl::resetInitializationSimulationProgram() {
      bool result = setString(OS_Coil_UserDefinedFields::InitializationSimulationProgramName, "");
      OS_ASSERT(result);
  }

  //This really shouldnt be avail to the user since its done in the FT -BLB
  bool CoilUserDefined_Impl::setPlantConnectionisUsed(bool plantConnectionisUsed) {
    bool result = false;
    if (plantConnectionisUsed) {
        result = setString(OS_Coil_UserDefinedFields::PlantConnectionisUsed, "Yes");
    } else {
        result = setString(OS_Coil_UserDefinedFields::PlantConnectionisUsed, "No");
    }
    OS_ASSERT(result);
    return result;
  }

  bool CoilUserDefined_Impl::setAmbientZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Coil_UserDefinedFields::AmbientZoneName, thermalZone.handle());
    return result;
  }

  void CoilUserDefined_Impl::resetAmbientZone() {
    bool result = setString(OS_Coil_UserDefinedFields::AmbientZoneName, "");
    OS_ASSERT(result);
  }

  bool CoilUserDefined_Impl::setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::AirOutletTemperatureActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::AirOutletHumidityRatioActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setAirMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::AirMassFlowRateActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::PlantMinimumMassFlowRateActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::PlantMaximumMassFlowRateActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::PlantDesignVolumeFlowRateActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::PlantOutletTemperatureActuator, energyManagementSystemActuator.handle());
      return result;
  }

  bool CoilUserDefined_Impl::setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
      bool result = setPointer(OS_Coil_UserDefinedFields::PlantMassFlowRateActuator, energyManagementSystemActuator.handle());
      return result;
  }

} // detail

CoilUserDefined::CoilUserDefined(const Model& model)
  : WaterToAirComponent(CoilUserDefined::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilUserDefined_Impl>());

  bool ok = true;
  // setup required Actuators for Plant Connection 1
  EnergyManagementSystemActuator aota(this->cast<ModelObject>(), "Air Connection", "Air Outlet Temperature");
  aota.setName("airOutletTemperature");
  ok = setAirOutletTemperatureActuator(aota);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator aohra(this->cast<ModelObject>(), "Air Connection", "Air Outlet Humidity Ratio");
  aohra.setName("airOutletHumidityRatio");
  ok = setAirOutletHumidityRatioActuator(aohra);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator amfra(this->cast<ModelObject>(), "Air Connection", "Air Mass Flow Rate");
  amfra.setName("airMassFlowRate");
  ok = setAirMassFlowRateActuator(amfra);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator pminmfra(this->cast<ModelObject>(), "Plant Connection", "Plant Minimum Mass FlowRate");
  pminmfra.setName("plantMinimumMassFlowRate");
  ok = setPlantMinimumMassFlowRateActuator(pminmfra);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator pmaxmfra(this->cast<ModelObject>(), "Plant Connection", "Plant Maximum Mass FlowRate");
  pmaxmfra.setName("plantMaximumMassFlowRate");
  ok = setPlantMaximumMassFlowRateActuator(pmaxmfra);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator pdvfra(this->cast<ModelObject>(), "Plant Connection", "Plant Design Volume FlowRate");
  pdvfra.setName("plantDesignVolumeFlowRate");
  ok = setPlantDesignVolumeFlowRateActuator(pdvfra);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator pota(this->cast<ModelObject>(), "Plant Connection", "Plant Outlet Temperature");
  pota.setName("plantMassFlowRate");
  ok = setPlantMassFlowRateActuator(pota);
  OS_ASSERT(ok);
  EnergyManagementSystemActuator pmmfra(this->cast<ModelObject>(), "Plant Connection", "Plant Mass Flow Rate");
  pmmfra.setName("plantOutletTemperature");
  ok = setPlantOutletTemperatureActuator(pmmfra);
  OS_ASSERT(ok);

  // setup Overall Simulation Program and Manager
  EnergyManagementSystemProgram mainProgram(model);
  mainProgram.setName("overallSimulationProgram");

  EnergyManagementSystemProgramCallingManager mainProgramCallingManager(model);
  mainProgramCallingManager.setName("overallModelSimulationProgramCallingManager");
  mainProgramCallingManager.setCallingPoint("UserDefinedComponentModel");
  mainProgramCallingManager.addProgram(mainProgram);

  // add as children so they delete if object gets removed
  ok = setOverallModelSimulationProgramCallingManager(mainProgramCallingManager);
  OS_ASSERT(ok);
  ok = setOverallSimulationProgram(mainProgram);
  OS_ASSERT(ok);

  // setup Overall Simulation Program and Manager
  EnergyManagementSystemProgram initProgram(model);
  initProgram.setName("initializationSimulationProgram");

  EnergyManagementSystemProgramCallingManager modelSetupandSizingPCM(model);
  modelSetupandSizingPCM.setName("modelSetupandSizingProgramCallingManager");
  modelSetupandSizingPCM.setCallingPoint("UserDefinedComponentModel");
  modelSetupandSizingPCM.addProgram(initProgram);

  // add as children so they delete if object gets removed
  ok = setModelSetupandSizingProgramCallingManager(modelSetupandSizingPCM);
  OS_ASSERT(ok);
  ok = setInitializationSimulationProgram(initProgram);
  OS_ASSERT(ok);

  // set NumberofAirConnections to 1
  OS_ASSERT(setString(OS_Coil_UserDefinedFields::NumberofAirConnections, "1"));
}

IddObjectType CoilUserDefined::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_UserDefined);
}

boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined::overallModelSimulationProgramCallingManager() const {
  return getImpl<detail::CoilUserDefined_Impl>()->overallModelSimulationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined::modelSetupandSizingProgramCallingManager() const {
  return getImpl<detail::CoilUserDefined_Impl>()->modelSetupandSizingProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgram> CoilUserDefined::overallSimulationProgram() const {
    return getImpl<detail::CoilUserDefined_Impl>()->overallSimulationProgram();
}

boost::optional<EnergyManagementSystemProgram> CoilUserDefined::initializationSimulationProgram() const {
    return getImpl<detail::CoilUserDefined_Impl>()->initializationSimulationProgram();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::airOutletTemperatureActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airOutletTemperatureActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::airOutletHumidityRatioActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airOutletHumidityRatioActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::airMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airMassFlowRateActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::plantMinimumMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMinimumMassFlowRateActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::plantMaximumMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMaximumMassFlowRateActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::plantDesignVolumeFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantDesignVolumeFlowRateActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::plantMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMassFlowRateActuator();
}

boost::optional<EnergyManagementSystemActuator> CoilUserDefined::plantOutletTemperatureActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantOutletTemperatureActuator();
}

bool CoilUserDefined::plantConnectionisUsed() const {
  return getImpl<detail::CoilUserDefined_Impl>()->plantConnectionisUsed();
}

boost::optional<ThermalZone> CoilUserDefined::ambientZone() const {
  return getImpl<detail::CoilUserDefined_Impl>()->ambientZone();
}

bool CoilUserDefined::setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgramCallingManager) {
  return getImpl<detail::CoilUserDefined_Impl>()->setOverallModelSimulationProgramCallingManager(erlProgramCallingManager);
}

void CoilUserDefined::resetOverallModelSimulationProgramCallingManager() {
  getImpl<detail::CoilUserDefined_Impl>()->resetOverallModelSimulationProgramCallingManager();
}

bool CoilUserDefined::setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgramCallingManager) {
  return getImpl<detail::CoilUserDefined_Impl>()->setModelSetupandSizingProgramCallingManager(erlProgramCallingManager);
}

void CoilUserDefined::resetModelSetupandSizingProgramCallingManager() {
    getImpl<detail::CoilUserDefined_Impl>()->resetModelSetupandSizingProgramCallingManager();
}

bool CoilUserDefined::setOverallSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram) {
    return getImpl<detail::CoilUserDefined_Impl>()->setOverallSimulationProgram(energyManagementSystemProgram);
}

void CoilUserDefined::resetOverallSimulationProgram() {
    getImpl<detail::CoilUserDefined_Impl>()->resetOverallSimulationProgram();
}

bool CoilUserDefined::setInitializationSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram) {
    return getImpl<detail::CoilUserDefined_Impl>()->setInitializationSimulationProgram(energyManagementSystemProgram);
}

void CoilUserDefined::resetInitializationSimulationProgram() {
    getImpl<detail::CoilUserDefined_Impl>()->resetInitializationSimulationProgram();
}

bool CoilUserDefined::setPlantConnectionisUsed(bool plantConnectionisUsed) {
  return getImpl<detail::CoilUserDefined_Impl>()->setPlantConnectionisUsed(plantConnectionisUsed);
}

bool CoilUserDefined::setAmbientZone(const ThermalZone& thermalZone) {
  return getImpl<detail::CoilUserDefined_Impl>()->setAmbientZone(thermalZone);
}

void CoilUserDefined::resetAmbientZone() {
  getImpl<detail::CoilUserDefined_Impl>()->resetAmbientZone();
}

bool CoilUserDefined::setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirOutletTemperatureActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirOutletHumidityRatioActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setAirMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirMassFlowRateActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMinimumMassFlowRateActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMaximumMassFlowRateActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantDesignVolumeFlowRateActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMassFlowRateActuator(energyManagementSystemActuator);
}

bool CoilUserDefined::setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantOutletTemperatureActuator(energyManagementSystemActuator);
}

/// @cond
CoilUserDefined::CoilUserDefined(std::shared_ptr<detail::CoilUserDefined_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

