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
      return WaterToAirComponent_Impl::clone(model);
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

  boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::overallModelSimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName);
  }

  EnergyManagementSystemProgramCallingManager CoilUserDefined_Impl::modelSetupandSizingProgramCallingManager() const {
    boost::optional<EnergyManagementSystemProgramCallingManager> value = optionalModelSetupandSizingProgramCallingManager();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Model Setupand Sizing Program Calling Manager attached.");
    }
    return value.get();
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

  bool CoilUserDefined_Impl::setPlantConnectionisUsed(bool plantConnectionisUsed) {
    bool result = false;
    if (plantConnectionisUsed) {
        result = setString(OS_Coil_UserDefinedFields::PlantConnectionisUsed, "True");
    } else {
        result = setString(OS_Coil_UserDefinedFields::PlantConnectionisUsed, "False");
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

  boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined_Impl::optionalModelSetupandSizingProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName);
  }

} // detail

CoilUserDefined::CoilUserDefined(const Model& model)
  : WaterToAirComponent(CoilUserDefined::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilUserDefined_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName
  //     OS_Coil_UserDefinedFields::AirConnection1InletNodeName
  //     OS_Coil_UserDefinedFields::AirConnection1OutletNodeName
  bool ok = true;
  // ok = setModelSetupandSizingProgramCallingManager();
  OS_ASSERT(ok);
  // ok = setNumberofAirConnections();
  OS_ASSERT(ok);
  // ok = setAirConnection1InletNode();
  OS_ASSERT(ok);
  // ok = setAirConnection1OutletNode();
  OS_ASSERT(ok);
}

IddObjectType CoilUserDefined::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_UserDefined);
}

boost::optional<EnergyManagementSystemProgramCallingManager> CoilUserDefined::overallModelSimulationProgramCallingManager() const {
  return getImpl<detail::CoilUserDefined_Impl>()->overallModelSimulationProgramCallingManager();
}

EnergyManagementSystemProgramCallingManager CoilUserDefined::modelSetupandSizingProgramCallingManager() const {
  return getImpl<detail::CoilUserDefined_Impl>()->modelSetupandSizingProgramCallingManager();
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

bool CoilUserDefined::setPlantConnectionisUsed(bool plantConnectionisUsed) {
  return getImpl<detail::CoilUserDefined_Impl>()->setPlantConnectionisUsed(plantConnectionisUsed);
}

bool CoilUserDefined::setAmbientZone(const ThermalZone& thermalZone) {
  return getImpl<detail::CoilUserDefined_Impl>()->setAmbientZone(thermalZone);
}

void CoilUserDefined::resetAmbientZone() {
  getImpl<detail::CoilUserDefined_Impl>()->resetAmbientZone();
}

/// @cond
CoilUserDefined::CoilUserDefined(std::shared_ptr<detail::CoilUserDefined_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

