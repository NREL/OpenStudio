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

#include "PlantComponentUserDefined.hpp"
#include "PlantComponentUserDefined_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
//#include "WaterStorageTank.hpp"
//#include "WaterStorageTank_Impl.hpp"
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
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantComponentUserDefined::iddObjectType());
  }

  PlantComponentUserDefined_Impl::PlantComponentUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantComponentUserDefined::iddObjectType());
  }

  PlantComponentUserDefined_Impl::PlantComponentUserDefined_Impl(const PlantComponentUserDefined_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
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

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::mainModelProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName);
  }

  int PlantComponentUserDefined_Impl::numberofPlantLoopConnections() const {
    boost::optional<int> value = getInt(OS_PlantComponent_UserDefinedFields::NumberofPlantLoopConnections,true);
    OS_ASSERT(value);
    return value.get();
  }

  Connection PlantComponentUserDefined_Impl::plantConnection1InletNode() const {
    boost::optional<Connection> value = optionalPlantConnection1InletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Plant Connection1Inlet Node attached.");
    }
    return value.get();
  }

  Connection PlantComponentUserDefined_Impl::plantConnection1OutletNode() const {
    boost::optional<Connection> value = optionalPlantConnection1OutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Plant Connection1Outlet Node attached.");
    }
    return value.get();
  }

  std::string PlantComponentUserDefined_Impl::plantConnection1LoadingMode() const {
    boost::optional<std::string> value = getString(OS_PlantComponent_UserDefinedFields::PlantConnection1LoadingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string PlantComponentUserDefined_Impl::plantConnection1LoopFlowRequestMode() const {
    boost::optional<std::string> value = getString(OS_PlantComponent_UserDefinedFields::PlantConnection1LoopFlowRequestMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection1InitializationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection1SimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection2InletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection2InletNodeName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection2OutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection2OutletNodeName);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection2LoadingMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoadingMode,true);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection2LoopFlowRequestMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoopFlowRequestMode,true);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection2InitializationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection2InitializationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection2SimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection2SimulationProgramCallingManagerName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection3InletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection3InletNodeName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection3OutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection3OutletNodeName);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection3LoadingMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoadingMode,true);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection3LoopFlowRequestMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoopFlowRequestMode,true);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection3InitializationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection3InitializationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection3SimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection3SimulationProgramCallingManagerName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection4InletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection4InletNodeName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::plantConnection4OutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection4OutletNodeName);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection4LoadingMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoadingMode,true);
  }

  boost::optional<std::string> PlantComponentUserDefined_Impl::plantConnection4LoopFlowRequestMode() const {
    return getString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoopFlowRequestMode,true);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection4InitializationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection4InitializationProgramCallingManagerName);
  }

  boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined_Impl::plantConnection4SimulationProgramCallingManager() const {
    return getObject<ModelObject>().getModelObjectTarget<EnergyManagementSystemProgramCallingManager>(OS_PlantComponent_UserDefinedFields::PlantConnection4SimulationProgramCallingManagerName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::airConnectionInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::AirConnectionInletNodeName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::airConnectionOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::AirConnectionOutletNodeName);
  }

  //boost::optional<WaterStorageTank> PlantComponentUserDefined_Impl::supplyInletWaterStorageTank() const {
  //  return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_PlantComponent_UserDefinedFields::SupplyInletWaterStorageTankName);
  //}

  //boost::optional<WaterStorageTank> PlantComponentUserDefined_Impl::collectionOutletWaterStorageTank() const {
  //  return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_PlantComponent_UserDefinedFields::CollectionOutletWaterStorageTankName);
  // }

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

  bool PlantComponentUserDefined_Impl::setNumberofPlantLoopConnections(int numberofPlantLoopConnections) {
    bool result = setInt(OS_PlantComponent_UserDefinedFields::NumberofPlantLoopConnections, numberofPlantLoopConnections);
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1InletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, connection.handle());
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1OutletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, connection.handle());
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1LoadingMode(const std::string& plantConnection1LoadingMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection1LoadingMode, plantConnection1LoadingMode);
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1LoopFlowRequestMode(const std::string& plantConnection1LoopFlowRequestMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection1LoopFlowRequestMode, plantConnection1LoopFlowRequestMode);
    return result;
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection1InitializationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection1SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection1SimulationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2InletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection2InletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2InletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2InletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2OutletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection2OutletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2OutletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2OutletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2LoadingMode(const std::string& plantConnection2LoadingMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoadingMode, plantConnection2LoadingMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2LoadingMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoadingMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2LoopFlowRequestMode(const std::string& plantConnection2LoopFlowRequestMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoopFlowRequestMode, plantConnection2LoopFlowRequestMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2LoopFlowRequestMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2LoopFlowRequestMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection2InitializationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2InitializationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2InitializationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection2SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection2SimulationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection2SimulationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection2SimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3InletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection3InletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3InletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3InletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3OutletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection3OutletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3OutletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3OutletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3LoadingMode(const std::string& plantConnection3LoadingMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoadingMode, plantConnection3LoadingMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3LoadingMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoadingMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3LoopFlowRequestMode(const std::string& plantConnection3LoopFlowRequestMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoopFlowRequestMode, plantConnection3LoopFlowRequestMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3LoopFlowRequestMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3LoopFlowRequestMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection3InitializationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3InitializationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3InitializationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection3SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection3SimulationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection3SimulationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection3SimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4InletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection4InletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4InletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4InletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4OutletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection4OutletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4OutletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4OutletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4LoadingMode(const std::string& plantConnection4LoadingMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoadingMode, plantConnection4LoadingMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4LoadingMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoadingMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4LoopFlowRequestMode(const std::string& plantConnection4LoopFlowRequestMode) {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoopFlowRequestMode, plantConnection4LoopFlowRequestMode);
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4LoopFlowRequestMode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4LoopFlowRequestMode, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection4InitializationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4InitializationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4InitializationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setPlantConnection4SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::PlantConnection4SimulationProgramCallingManagerName, energyManagementSystemProgramCallingManager.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetPlantConnection4SimulationProgramCallingManager() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::PlantConnection4SimulationProgramCallingManagerName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setAirConnectionInletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::AirConnectionInletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetAirConnectionInletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::AirConnectionInletNodeName, "");
    OS_ASSERT(result);
  }

  bool PlantComponentUserDefined_Impl::setAirConnectionOutletNode(const Connection& connection) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::AirConnectionOutletNodeName, connection.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetAirConnectionOutletNode() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::AirConnectionOutletNodeName, "");
    OS_ASSERT(result);
  }

  //bool PlantComponentUserDefined_Impl::setSupplyInletWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  // bool result = setPointer(OS_PlantComponent_UserDefinedFields::SupplyInletWaterStorageTankName, waterStorageTank.handle());
  //  return result;
  //}

  //void PlantComponentUserDefined_Impl::resetSupplyInletWaterStorageTank() {
  //  bool result = setString(OS_PlantComponent_UserDefinedFields::SupplyInletWaterStorageTankName, "");
  //  OS_ASSERT(result);
  // }

  //bool PlantComponentUserDefined_Impl::setCollectionOutletWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  //  bool result = setPointer(OS_PlantComponent_UserDefinedFields::CollectionOutletWaterStorageTankName, waterStorageTank.handle());
  //  return result;
  //}

  //void PlantComponentUserDefined_Impl::resetCollectionOutletWaterStorageTank() {
  //  bool result = setString(OS_PlantComponent_UserDefinedFields::CollectionOutletWaterStorageTankName, "");
  //  OS_ASSERT(result);
  //}

  bool PlantComponentUserDefined_Impl::setAmbientZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_PlantComponent_UserDefinedFields::AmbientZoneName, thermalZone.handle());
    return result;
  }

  void PlantComponentUserDefined_Impl::resetAmbientZone() {
    bool result = setString(OS_PlantComponent_UserDefinedFields::AmbientZoneName, "");
    OS_ASSERT(result);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::optionalPlantConnection1InletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection1InletNodeName);
  }

  boost::optional<Connection> PlantComponentUserDefined_Impl::optionalPlantConnection1OutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName);
  }

} // detail

PlantComponentUserDefined::PlantComponentUserDefined(const Model& model)
  : ModelObject(PlantComponentUserDefined::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantComponentUserDefined_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_PlantComponent_UserDefinedFields::PlantConnection1InletNodeName
  //     OS_PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName
  bool ok = true;
  // ok = setNumberofPlantLoopConnections();
  OS_ASSERT(ok);
  // ok = setPlantConnection1InletNode();
  OS_ASSERT(ok);
  // ok = setPlantConnection1OutletNode();
  OS_ASSERT(ok);
  // ok = setPlantConnection1LoadingMode();
  OS_ASSERT(ok);
  // ok = setPlantConnection1LoopFlowRequestMode();
  OS_ASSERT(ok);
}

IddObjectType PlantComponentUserDefined::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantComponent_UserDefined);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection1LoadingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection1LoadingMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection1LoopFlowRequestModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection1LoopFlowRequestMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection2LoadingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection2LoadingMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection2LoopFlowRequestModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection2LoopFlowRequestMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection3LoadingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection3LoadingMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection3LoopFlowRequestModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection3LoopFlowRequestMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection4LoadingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection4LoadingMode);
}

std::vector<std::string> PlantComponentUserDefined::plantConnection4LoopFlowRequestModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PlantComponent_UserDefinedFields::PlantConnection4LoopFlowRequestMode);
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::mainModelProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->mainModelProgramCallingManager();
}

int PlantComponentUserDefined::numberofPlantLoopConnections() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->numberofPlantLoopConnections();
}

Connection PlantComponentUserDefined::plantConnection1InletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1InletNode();
}

Connection PlantComponentUserDefined::plantConnection1OutletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1OutletNode();
}

std::string PlantComponentUserDefined::plantConnection1LoadingMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1LoadingMode();
}

std::string PlantComponentUserDefined::plantConnection1LoopFlowRequestMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1LoopFlowRequestMode();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection1InitializationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1InitializationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection1SimulationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection1SimulationProgramCallingManager();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection2InletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2InletNode();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection2OutletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2OutletNode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection2LoadingMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2LoadingMode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection2LoopFlowRequestMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2LoopFlowRequestMode();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection2InitializationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2InitializationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection2SimulationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection2SimulationProgramCallingManager();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection3InletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3InletNode();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection3OutletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3OutletNode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection3LoadingMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3LoadingMode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection3LoopFlowRequestMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3LoopFlowRequestMode();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection3InitializationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3InitializationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection3SimulationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection3SimulationProgramCallingManager();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection4InletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4InletNode();
}

boost::optional<Connection> PlantComponentUserDefined::plantConnection4OutletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4OutletNode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection4LoadingMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4LoadingMode();
}

boost::optional<std::string> PlantComponentUserDefined::plantConnection4LoopFlowRequestMode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4LoopFlowRequestMode();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection4InitializationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4InitializationProgramCallingManager();
}

boost::optional<EnergyManagementSystemProgramCallingManager> PlantComponentUserDefined::plantConnection4SimulationProgramCallingManager() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->plantConnection4SimulationProgramCallingManager();
}

boost::optional<Connection> PlantComponentUserDefined::airConnectionInletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->airConnectionInletNode();
}

boost::optional<Connection> PlantComponentUserDefined::airConnectionOutletNode() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->airConnectionOutletNode();
}

//boost::optional<WaterStorageTank> PlantComponentUserDefined::supplyInletWaterStorageTank() const {
//  return getImpl<detail::PlantComponentUserDefined_Impl>()->supplyInletWaterStorageTank();
//}

//boost::optional<WaterStorageTank> PlantComponentUserDefined::collectionOutletWaterStorageTank() const {
//  return getImpl<detail::PlantComponentUserDefined_Impl>()->collectionOutletWaterStorageTank();
//}

boost::optional<ThermalZone> PlantComponentUserDefined::ambientZone() const {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->ambientZone();
}

bool PlantComponentUserDefined::setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setMainModelProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetMainModelProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetMainModelProgramCallingManager();
}

bool PlantComponentUserDefined::setNumberofPlantLoopConnections(int numberofPlantLoopConnections) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setNumberofPlantLoopConnections(numberofPlantLoopConnections);
}

bool PlantComponentUserDefined::setPlantConnection1InletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1InletNode(connection);
}

bool PlantComponentUserDefined::setPlantConnection1OutletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1OutletNode(connection);
}

bool PlantComponentUserDefined::setPlantConnection1LoadingMode(const std::string& plantConnection1LoadingMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1LoadingMode(plantConnection1LoadingMode);
}

bool PlantComponentUserDefined::setPlantConnection1LoopFlowRequestMode(const std::string& plantConnection1LoopFlowRequestMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1LoopFlowRequestMode(plantConnection1LoopFlowRequestMode);
}

bool PlantComponentUserDefined::setPlantConnection1InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1InitializationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection1InitializationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection1InitializationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection1SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection1SimulationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection1SimulationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection1SimulationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection2InletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2InletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection2InletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2InletNode();
}

bool PlantComponentUserDefined::setPlantConnection2OutletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2OutletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection2OutletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2OutletNode();
}

bool PlantComponentUserDefined::setPlantConnection2LoadingMode(const std::string& plantConnection2LoadingMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2LoadingMode(plantConnection2LoadingMode);
}

void PlantComponentUserDefined::resetPlantConnection2LoadingMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2LoadingMode();
}

bool PlantComponentUserDefined::setPlantConnection2LoopFlowRequestMode(const std::string& plantConnection2LoopFlowRequestMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2LoopFlowRequestMode(plantConnection2LoopFlowRequestMode);
}

void PlantComponentUserDefined::resetPlantConnection2LoopFlowRequestMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2LoopFlowRequestMode();
}

bool PlantComponentUserDefined::setPlantConnection2InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2InitializationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection2InitializationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2InitializationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection2SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection2SimulationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection2SimulationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection2SimulationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection3InletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3InletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection3InletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3InletNode();
}

bool PlantComponentUserDefined::setPlantConnection3OutletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3OutletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection3OutletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3OutletNode();
}

bool PlantComponentUserDefined::setPlantConnection3LoadingMode(const std::string& plantConnection3LoadingMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3LoadingMode(plantConnection3LoadingMode);
}

void PlantComponentUserDefined::resetPlantConnection3LoadingMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3LoadingMode();
}

bool PlantComponentUserDefined::setPlantConnection3LoopFlowRequestMode(const std::string& plantConnection3LoopFlowRequestMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3LoopFlowRequestMode(plantConnection3LoopFlowRequestMode);
}

void PlantComponentUserDefined::resetPlantConnection3LoopFlowRequestMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3LoopFlowRequestMode();
}

bool PlantComponentUserDefined::setPlantConnection3InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3InitializationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection3InitializationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3InitializationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection3SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection3SimulationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection3SimulationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection3SimulationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection4InletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4InletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection4InletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4InletNode();
}

bool PlantComponentUserDefined::setPlantConnection4OutletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4OutletNode(connection);
}

void PlantComponentUserDefined::resetPlantConnection4OutletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4OutletNode();
}

bool PlantComponentUserDefined::setPlantConnection4LoadingMode(const std::string& plantConnection4LoadingMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4LoadingMode(plantConnection4LoadingMode);
}

void PlantComponentUserDefined::resetPlantConnection4LoadingMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4LoadingMode();
}

bool PlantComponentUserDefined::setPlantConnection4LoopFlowRequestMode(const std::string& plantConnection4LoopFlowRequestMode) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4LoopFlowRequestMode(plantConnection4LoopFlowRequestMode);
}

void PlantComponentUserDefined::resetPlantConnection4LoopFlowRequestMode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4LoopFlowRequestMode();
}

bool PlantComponentUserDefined::setPlantConnection4InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4InitializationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection4InitializationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4InitializationProgramCallingManager();
}

bool PlantComponentUserDefined::setPlantConnection4SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setPlantConnection4SimulationProgramCallingManager(energyManagementSystemProgramCallingManager);
}

void PlantComponentUserDefined::resetPlantConnection4SimulationProgramCallingManager() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetPlantConnection4SimulationProgramCallingManager();
}

bool PlantComponentUserDefined::setAirConnectionInletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setAirConnectionInletNode(connection);
}

void PlantComponentUserDefined::resetAirConnectionInletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetAirConnectionInletNode();
}

bool PlantComponentUserDefined::setAirConnectionOutletNode(const Connection& connection) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setAirConnectionOutletNode(connection);
}

void PlantComponentUserDefined::resetAirConnectionOutletNode() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetAirConnectionOutletNode();
}

//bool PlantComponentUserDefined::setSupplyInletWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//  return getImpl<detail::PlantComponentUserDefined_Impl>()->setSupplyInletWaterStorageTank(waterStorageTank);
//}

//void PlantComponentUserDefined::resetSupplyInletWaterStorageTank() {
//  getImpl<detail::PlantComponentUserDefined_Impl>()->resetSupplyInletWaterStorageTank();
//}

//bool PlantComponentUserDefined::setCollectionOutletWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//  return getImpl<detail::PlantComponentUserDefined_Impl>()->setCollectionOutletWaterStorageTank(waterStorageTank);
//}

//void PlantComponentUserDefined::resetCollectionOutletWaterStorageTank() {
//  getImpl<detail::PlantComponentUserDefined_Impl>()->resetCollectionOutletWaterStorageTank();
//}

bool PlantComponentUserDefined::setAmbientZone(const ThermalZone& thermalZone) {
  return getImpl<detail::PlantComponentUserDefined_Impl>()->setAmbientZone(thermalZone);
}

void PlantComponentUserDefined::resetAmbientZone() {
  getImpl<detail::PlantComponentUserDefined_Impl>()->resetAmbientZone();
}

/// @cond
PlantComponentUserDefined::PlantComponentUserDefined(std::shared_ptr<detail::PlantComponentUserDefined_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

