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

#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <algorithm>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_UserDefined_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    CoilUserDefined_Impl::CoilUserDefined_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilUserDefined::iddObjectType());
    }

    CoilUserDefined_Impl::CoilUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilUserDefined::iddObjectType());
    }

    CoilUserDefined_Impl::CoilUserDefined_Impl(const CoilUserDefined_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilUserDefined_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    boost::optional<HVACComponent> CoilUserDefined_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem is the only accepted type, but the coil can be used as any of the three HC, CC or suppHC
      for (const auto& airLoopHVACUnitarySystem : this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>()) {
        if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        } else if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.heatingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        } else if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }
      return boost::none;
    }

    IddObjectType CoilUserDefined_Impl::iddObjectType() const {
      return CoilUserDefined::iddObjectType();
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
      std::vector<IddObjectType> result{
        IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager,
        IddObjectType::OS_EnergyManagementSystem_Program,
        IddObjectType::OS_EnergyManagementSystem_Actuator,
        IddObjectType::OS_Coil_UserDefined,
      };
      return result;
    }

    // Returns the children object
    std::vector<ModelObject> CoilUserDefined_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<EnergyManagementSystemProgramCallingManager> omsmpcm = optionalOverallModelSimulationProgramCallingManager()) {
        result.push_back(omsmpcm.get());
      }
      if (boost::optional<EnergyManagementSystemProgramCallingManager> msspcm = optionalModelSetupandSizingProgramCallingManager()) {
        result.push_back(msspcm.get());
      }
      if (boost::optional<EnergyManagementSystemProgram> osp = optionalOverallSimulationProgram()) {
        result.push_back(osp.get());
      }
      if (boost::optional<EnergyManagementSystemProgram> isp = optionalInitializationSimulationProgram()) {
        result.push_back(isp.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> aota = optionalAirOutletTemperatureActuator()) {
        result.push_back(aota.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> aohra = optionalAirOutletHumidityRatioActuator()) {
        result.push_back(aohra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> amfra = optionalAirMassFlowRateActuator()) {
        result.push_back(amfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pminmfra = optionalPlantMinimumMassFlowRateActuator()) {
        result.push_back(pminmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pmaxmfra = optionalPlantMaximumMassFlowRateActuator()) {
        result.push_back(pmaxmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pdvfra = optionalPlantDesignVolumeFlowRateActuator()) {
        result.push_back(pdvfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pmmfra = optionalPlantMassFlowRateActuator()) {
        result.push_back(pmmfra.get());
      }
      if (boost::optional<EnergyManagementSystemActuator> pota = optionalPlantOutletTemperatureActuator()) {
        result.push_back(pota.get());
      }
      return result;
    }

    ModelObject CoilUserDefined_Impl::clone(Model model) const {
      auto newCoilUserDefined = ModelObject_Impl::clone(model).cast<CoilUserDefined>();  // NOLINT(bugprone-parent-virtual-call
      //airOutletTemperatureActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalAirOutletTemperatureActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setAirOutletTemperatureActuator(objectClone);
      }
      //airOutletHumidityRatioActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalAirOutletHumidityRatioActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setAirOutletHumidityRatioActuator(objectClone);
      }
      //airMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalAirMassFlowRateActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setAirMassFlowRateActuator(objectClone);
      }
      //plantMinimumMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalPlantMinimumMassFlowRateActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setPlantMinimumMassFlowRateActuator(objectClone);
      }
      //plantMaximumMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalPlantMaximumMassFlowRateActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setPlantMaximumMassFlowRateActuator(objectClone);
      }
      //plantDesignVolumeFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalPlantDesignVolumeFlowRateActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setPlantDesignVolumeFlowRateActuator(objectClone);
      }
      //plantMassFlowRateActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalPlantMassFlowRateActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setPlantMassFlowRateActuator(objectClone);
      }
      //plantOutletTemperatureActuator
      if (boost::optional<EnergyManagementSystemActuator> object = optionalPlantOutletTemperatureActuator()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemActuator>();
        objectClone.setActuatedComponent(newCoilUserDefined);
        newCoilUserDefined.setPlantOutletTemperatureActuator(objectClone);
      }
      //overallModelSimulationProgramCallingManager
      if (boost::optional<EnergyManagementSystemProgramCallingManager> object = optionalOverallModelSimulationProgramCallingManager()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemProgramCallingManager>();
        //clone the program referenced in the callingManager
        if (boost::optional<EnergyManagementSystemProgram> program = optionalOverallSimulationProgram()) {
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
      if (boost::optional<EnergyManagementSystemProgramCallingManager> object = optionalModelSetupandSizingProgramCallingManager()) {
        auto objectClone = object.get().clone(model).cast<EnergyManagementSystemProgramCallingManager>();
        //clone the program referenced in the callingManager
        if (boost::optional<EnergyManagementSystemProgram> program = optionalInitializationSimulationProgram()) {
          auto cloneProgram = program.get().clone(model).cast<EnergyManagementSystemProgram>();
          //add cloned program to cloned PCUD
          newCoilUserDefined.setInitializationSimulationProgram(cloneProgram);
          //add cloned program to cloned programCallingManager
          objectClone.erasePrograms();
          objectClone.addProgram(cloneProgram);
        }
        newCoilUserDefined.setModelSetupandSizingProgramCallingManager(objectClone);
      }
      // ambientZone
      newCoilUserDefined.resetAmbientZone();

      return std::move(newCoilUserDefined);
    }

    EnergyManagementSystemProgramCallingManager CoilUserDefined_Impl::overallModelSimulationProgramCallingManager() const {
      boost::optional<EnergyManagementSystemProgramCallingManager> value = optionalOverallModelSimulationProgramCallingManager();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Overall Model Simulation Program Calling Manager attached.");
      }
      return value.get();
    }

    EnergyManagementSystemProgramCallingManager CoilUserDefined_Impl::modelSetupandSizingProgramCallingManager() const {
      boost::optional<EnergyManagementSystemProgramCallingManager> value = optionalModelSetupandSizingProgramCallingManager();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Model Setupand Sizing Program Calling Manager attached.");
      }
      return value.get();
    }

    int CoilUserDefined_Impl::numberofAirConnections() const {
      if (airLoopHVAC() || containingHVACComponent()) {
        return 1;
      }
      return 0;
    }

    boost::optional<ThermalZone> CoilUserDefined_Impl::ambientZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Coil_UserDefinedFields::AmbientZoneName);
    }

    EnergyManagementSystemProgram CoilUserDefined_Impl::overallSimulationProgram() const {
      boost::optional<EnergyManagementSystemProgram> value = optionalOverallSimulationProgram();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Overall Simulation Program attached.");
      }
      return value.get();
    }

    EnergyManagementSystemProgram CoilUserDefined_Impl::initializationSimulationProgram() const {
      boost::optional<EnergyManagementSystemProgram> value = optionalInitializationSimulationProgram();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Initialization Simulation Program attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::airOutletTemperatureActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalAirOutletTemperatureActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Temperature Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::airOutletHumidityRatioActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalAirOutletHumidityRatioActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Humidity Ratio Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::airMassFlowRateActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalAirMassFlowRateActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Air Mass Flow Rate Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::plantMinimumMassFlowRateActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalPlantMinimumMassFlowRateActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Plant Minimum Mass Flow Rate Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::plantMaximumMassFlowRateActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalPlantMaximumMassFlowRateActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Plant Maximum Mass Flow Rate Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::plantDesignVolumeFlowRateActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalPlantDesignVolumeFlowRateActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Plant Design Volume Flow Rate Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::plantOutletTemperatureActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalPlantOutletTemperatureActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Plant Outlet Temperature Actuator attached.");
      }
      return value.get();
    }

    EnergyManagementSystemActuator CoilUserDefined_Impl::plantMassFlowRateActuator() const {
      boost::optional<EnergyManagementSystemActuator> value = optionalPlantMassFlowRateActuator();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Plant Mass Flow Rate Actuator attached.");
      }
      return value.get();
    }

    bool CoilUserDefined_Impl::setOverallModelSimulationProgramCallingManager(
      const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, emsProgramCallingManager.handle());
      return result;
    }

    bool
      CoilUserDefined_Impl::setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, emsProgramCallingManager.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setAmbientZone(const ThermalZone& thermalZone) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::AmbientZoneName, thermalZone.handle());
      return result;
    }

    void CoilUserDefined_Impl::resetAmbientZone() {
      const bool result = setString(OS_Coil_UserDefinedFields::AmbientZoneName, "");
      OS_ASSERT(result);
    }

    bool CoilUserDefined_Impl::setOverallSimulationProgram(const EnergyManagementSystemProgram& emsProgram) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::OverallSimulationProgramName, emsProgram.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setInitializationSimulationProgram(const EnergyManagementSystemProgram& emsProgram) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::InitializationSimulationProgramName, emsProgram.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::AirOutletTemperatureActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::AirOutletHumidityRatioActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setAirMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::AirMassFlowRateActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::PlantMinimumMassFlowRateActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::PlantMaximumMassFlowRateActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::PlantDesignVolumeFlowRateActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::PlantOutletTemperatureActuator, emsActuator.handle());
      return result;
    }

    bool CoilUserDefined_Impl::setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
      const bool result = setPointer(OS_Coil_UserDefinedFields::PlantMassFlowRateActuator, emsActuator.handle());
      return result;
    }

    boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::optionalOverallModelSimulationProgramCallingManager() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(
        OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName);
    }

    boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::optionalModelSetupandSizingProgramCallingManager() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(
        OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName);
    }

    boost::optional<EnergyManagementSystemProgram> CoilUserDefined_Impl::optionalOverallSimulationProgram() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgram>(OS_Coil_UserDefinedFields::OverallSimulationProgramName);
    }

    boost::optional<EnergyManagementSystemProgram> CoilUserDefined_Impl::optionalInitializationSimulationProgram() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgram>(
        OS_Coil_UserDefinedFields::InitializationSimulationProgramName);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalAirOutletTemperatureActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(OS_Coil_UserDefinedFields::AirOutletTemperatureActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalAirOutletHumidityRatioActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(OS_Coil_UserDefinedFields::AirOutletHumidityRatioActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalAirMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(OS_Coil_UserDefinedFields::AirMassFlowRateActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalPlantMinimumMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
        OS_Coil_UserDefinedFields::PlantMinimumMassFlowRateActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalPlantMaximumMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
        OS_Coil_UserDefinedFields::PlantMaximumMassFlowRateActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalPlantDesignVolumeFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(
        OS_Coil_UserDefinedFields::PlantDesignVolumeFlowRateActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalPlantOutletTemperatureActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(OS_Coil_UserDefinedFields::PlantOutletTemperatureActuator);
    }

    boost::optional<EnergyManagementSystemActuator> CoilUserDefined_Impl::optionalPlantMassFlowRateActuator() const {
      return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemActuator>(OS_Coil_UserDefinedFields::PlantMassFlowRateActuator);
    }

    void CoilUserDefined_Impl::renameEMSSubComponents() {
      auto newName = nameString();
      std::replace(newName.begin(), newName.end(), ' ', '_');
      newName += "_";
      overallModelSimulationProgramCallingManager().setName(newName + "overallModelSimulationProgramCallingManager");
      modelSetupandSizingProgramCallingManager().setName(newName + "modelSetupandSizingProgramCallingManager");
      overallSimulationProgram().setName(newName + "overallSimulationProgram");
      initializationSimulationProgram().setName(newName + "initializationSimulationProgram");
      airOutletTemperatureActuator().setName(newName + "airOutletTemperatureActuator");
      airOutletHumidityRatioActuator().setName(newName + "airOutletHumidityRatioActuator");
      airMassFlowRateActuator().setName(newName + "airMassFlowRateActuator");
      plantMinimumMassFlowRateActuator().setName(newName + "plantMinimumMassFlowRateActuator");
      plantMaximumMassFlowRateActuator().setName(newName + "plantMaximumMassFlowRateActuator");
      plantDesignVolumeFlowRateActuator().setName(newName + "plantDesignVolumeFlowRateActuator");
      plantMassFlowRateActuator().setName(newName + "plantMassFlowRateActuator");
      plantOutletTemperatureActuator().setName(newName + "plantOutletTemperatureActuator");
    }

  }  // namespace detail

  CoilUserDefined::CoilUserDefined(const Model& model) : WaterToAirComponent(CoilUserDefined::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilUserDefined_Impl>());

    bool ok = true;
    // setup required Actuators
    EnergyManagementSystemActuator aota(this->cast<ModelObject>(), "Air Connection 1", "Outlet Temperature");
    aota.setName("airOutletTemperature");
    ok = setAirOutletTemperatureActuator(aota);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator aohra(this->cast<ModelObject>(), "Air Connection 1", "Outlet Humidity Ratio");
    aohra.setName("airOutletHumidityRatio");
    ok = setAirOutletHumidityRatioActuator(aohra);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator amfra(this->cast<ModelObject>(), "Air Connection 1", "Mass Flow Rate");
    amfra.setName("airMassFlowRate");
    ok = setAirMassFlowRateActuator(amfra);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator pminmfra(this->cast<ModelObject>(), "Plant Connection", "Minimum Mass Flow Rate");
    pminmfra.setName("plantMinimumMassFlowRate");
    ok = setPlantMinimumMassFlowRateActuator(pminmfra);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator pmaxmfra(this->cast<ModelObject>(), "Plant Connection", "Maximum Mass Flow Rate");
    pmaxmfra.setName("plantMaximumMassFlowRate");
    ok = setPlantMaximumMassFlowRateActuator(pmaxmfra);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator pdvfra(this->cast<ModelObject>(), "Plant Connection", "Design Volume Flow Rate");
    pdvfra.setName("plantDesignVolumeFlowRate");
    ok = setPlantDesignVolumeFlowRateActuator(pdvfra);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator pota(this->cast<ModelObject>(), "Plant Connection", "Mass Flow Rate");
    pota.setName("plantMassFlowRate");
    ok = setPlantMassFlowRateActuator(pota);
    OS_ASSERT(ok);
    EnergyManagementSystemActuator pmmfra(this->cast<ModelObject>(), "Plant Connection", "Outlet Temperature");
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

    // getImpl<detail::CoilUserDefined_Impl>()->renameEMSSubComponents();
  }

  IddObjectType CoilUserDefined::iddObjectType() {
    return {IddObjectType::OS_Coil_UserDefined};
  }

  EnergyManagementSystemProgramCallingManager CoilUserDefined::overallModelSimulationProgramCallingManager() const {
    return getImpl<detail::CoilUserDefined_Impl>()->overallModelSimulationProgramCallingManager();
  }

  EnergyManagementSystemProgramCallingManager CoilUserDefined::modelSetupandSizingProgramCallingManager() const {
    return getImpl<detail::CoilUserDefined_Impl>()->modelSetupandSizingProgramCallingManager();
  }

  int CoilUserDefined::numberofAirConnections() const {
    return getImpl<detail::CoilUserDefined_Impl>()->numberofAirConnections();
  }

  boost::optional<ThermalZone> CoilUserDefined::ambientZone() const {
    return getImpl<detail::CoilUserDefined_Impl>()->ambientZone();
  }

  EnergyManagementSystemProgram CoilUserDefined::overallSimulationProgram() const {
    return getImpl<detail::CoilUserDefined_Impl>()->overallSimulationProgram();
  }

  EnergyManagementSystemProgram CoilUserDefined::initializationSimulationProgram() const {
    return getImpl<detail::CoilUserDefined_Impl>()->initializationSimulationProgram();
  }

  EnergyManagementSystemActuator CoilUserDefined::airOutletTemperatureActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airOutletTemperatureActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::airOutletHumidityRatioActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airOutletHumidityRatioActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::airMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->airMassFlowRateActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::plantMinimumMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMinimumMassFlowRateActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::plantMaximumMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMaximumMassFlowRateActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::plantDesignVolumeFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantDesignVolumeFlowRateActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::plantOutletTemperatureActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantOutletTemperatureActuator();
  }

  EnergyManagementSystemActuator CoilUserDefined::plantMassFlowRateActuator() const {
    return getImpl<detail::CoilUserDefined_Impl>()->plantMassFlowRateActuator();
  }

  bool CoilUserDefined::setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager) {
    return getImpl<detail::CoilUserDefined_Impl>()->setOverallModelSimulationProgramCallingManager(emsProgramCallingManager);
  }

  bool CoilUserDefined::setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager) {
    return getImpl<detail::CoilUserDefined_Impl>()->setModelSetupandSizingProgramCallingManager(emsProgramCallingManager);
  }

  bool CoilUserDefined::setAmbientZone(const ThermalZone& thermalZone) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAmbientZone(thermalZone);
  }

  void CoilUserDefined::resetAmbientZone() {
    getImpl<detail::CoilUserDefined_Impl>()->resetAmbientZone();
  }

  bool CoilUserDefined::setOverallSimulationProgram(const EnergyManagementSystemProgram& emsProgram) {
    return getImpl<detail::CoilUserDefined_Impl>()->setOverallSimulationProgram(emsProgram);
  }

  bool CoilUserDefined::setInitializationSimulationProgram(const EnergyManagementSystemProgram& emsProgram) {
    return getImpl<detail::CoilUserDefined_Impl>()->setInitializationSimulationProgram(emsProgram);
  }

  bool CoilUserDefined::setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirOutletTemperatureActuator(emsActuator);
  }

  bool CoilUserDefined::setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirOutletHumidityRatioActuator(emsActuator);
  }

  bool CoilUserDefined::setAirMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setAirMassFlowRateActuator(emsActuator);
  }

  bool CoilUserDefined::setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMinimumMassFlowRateActuator(emsActuator);
  }

  bool CoilUserDefined::setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMaximumMassFlowRateActuator(emsActuator);
  }

  bool CoilUserDefined::setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantDesignVolumeFlowRateActuator(emsActuator);
  }

  bool CoilUserDefined::setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantOutletTemperatureActuator(emsActuator);
  }

  bool CoilUserDefined::setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator) {
    return getImpl<detail::CoilUserDefined_Impl>()->setPlantMassFlowRateActuator(emsActuator);
  }

  void CoilUserDefined::renameEMSSubComponents() {
    return getImpl<detail::CoilUserDefined_Impl>()->renameEMSSubComponents();
  }

  /// @cond
  CoilUserDefined::CoilUserDefined(std::shared_ptr<detail::CoilUserDefined_Impl> impl) : WaterToAirComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
