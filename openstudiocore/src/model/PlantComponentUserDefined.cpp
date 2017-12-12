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

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "PlantComponentUserDefined.hpp"
#include "PlantComponentUserDefined_Impl.hpp"

#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PlantComponent_UserDefined_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantComponentUserDefined_Impl::PlantComponentUserDefined_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantComponentUserDefined::iddObjectType());
  }

  PlantComponentUserDefined_Impl::PlantComponentUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantComponentUserDefined::iddObjectType());
  }

  PlantComponentUserDefined_Impl::PlantComponentUserDefined_Impl(const PlantComponentUserDefined_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantComponentUserDefined_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantComponentUserDefined_Impl::iddObjectType() const {
    return PlantComponentUserDefined::iddObjectType();
  }

  ModelObject PlantComponentUserDefined_Impl::clone(Model model) const {

    PlantComponentUserDefined newPlantComponentUserDefined = StraightComponent_Impl::clone(model).cast<PlantComponentUserDefined>();

    return newPlantComponentUserDefined;
  }

  std::vector<IddObjectType> PlantComponentUserDefined_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> PlantComponentUserDefined_Impl::children() const {
    std::vector<ModelObject> result;

    return result;
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::mainModelProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName);
  }

  std::string PlantComponentUserDefined_Impl::plantLoadingMode() const {
    boost::optional<std::string> value = getString(OS_PlantComponent_UserDefinedFields::PlantLoadingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string PlantComponentUserDefined_Impl::plantLoopFlowRequestMode() const {
    boost::optional<std::string> value = getString(OS_PlantComponent_UserDefinedFields::PlantLoopFlowRequestMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantInitializationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantInitializationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantSimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantSimulationProgramCallingManagerName);
  }

  boost::optional<ThermalZone> PlantComponentUserDefined_Impl::ambientZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_PlantComponent_UserDefinedFields::AmbientZoneName);
  }

  bool PlantComponentUserDefined_Impl::setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetMainModelProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantLoadingMode(const std::string& plantLoadingMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantLoadingMode, plantLoadingMode);
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantLoopFlowRequestMode(const std::string& plantLoopFlowRequestMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantLoopFlowRequestMode, plantLoopFlowRequestMode);
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantInitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantInitializationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantInitializationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantInitializationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantSimulationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantSimulationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantSimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setAmbientZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::AmbientZoneName, thermalZone.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetAmbientZone() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::AmbientZoneName, "");
    OS_ASSERT(result);
  }

  unsigned PlantComponentUserDefined_Impl::inletPort() {
    return OS_PlantComponent_UserDefinedFields::PlantInletNodeName;
  }

  unsigned PlantComponentUserDefined_Impl::outletPort() {
    return OS_PlantComponent_UserDefinedFields::PlantOutletNodeName;
  }

  bool PlantComponentUserDefined_Impl::addToNode(Node & node) {
    if (boost::optional<PlantLoop> plant = node.plantLoop()) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

PlantComponentUserDefined::PlantComponentUserDefined(const Model& model)
  : StraightComponent(PlantComponentUserDefined::iddObjectType(),model){
  OS_ASSERT(getImpl<detail::PlantComponentUserDefined_Impl>());

  bool ok = true;
  // ok = setPlantLoadingMode();
  OS_ASSERT(ok);
  // ok = setPlantLoopFlowRequestMode();
  OS_ASSERT(ok);
}

IddObjectType PlantComponentUserDefined::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantComponent_UserDefined);
}

std::vector<std::string> PlantComponentUserDefined::plantLoadingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantLoadingMode);
}

std::vector<std::string> PlantComponentUserDefined::plantLoopFlowRequestModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantLoopFlowRequestMode);
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::mainModelProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->mainModelProgramCallingManager();
}

std::string PlantComponentUserDefined::plantLoadingMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantLoadingMode();
}

std::string PlantComponentUserDefined::plantLoopFlowRequestMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantLoopFlowRequestMode();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantInitializationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantInitializationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantSimulationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantSimulationProgramCallingManager();
}

boost::optional<ThermalZone> PlantComponentUserDefined::ambientZone() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->ambientZone();
}

bool PlantComponentUserDefined::setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setMainModelProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetMainModelProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetMainModelProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantLoadingMode(const std::string& plantConnection1LoadingMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantLoadingMode(plantConnection1LoadingMode);
}

bool PlantComponentUserDefined::setPlantLoopFlowRequestMode(const std::string& plantConnection1LoopFlowRequestMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantLoopFlowRequestMode(plantConnection1LoopFlowRequestMode);
}

bool PlantComponentUserDefined::setPlantInitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantInitializationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantInitializationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantInitializationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantSimulationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantSimulationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantSimulationProgramCallingManager();
}

bool PlantComponentUserDefined::setAmbientZone(const ThermalZone& thermalZone) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setAmbientZone(thermalZone);
}

void PlantComponentUserDefined::resetAmbientZone() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetAmbientZone();
}

/// @cond
PlantComponentUserDefined::PlantComponentUserDefined(std::shared_ptr<detail::PlantComponentUserDefined_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

