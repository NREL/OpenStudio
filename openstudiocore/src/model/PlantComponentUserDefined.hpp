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

#ifndef MODEL_PLANTCOMPONENTUSERDEFINED_HPP
#define MODEL_PLANTCOMPONENTUSERDEFINED_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "Connection.hpp"
//#include "WaterStorageTank.hpp"
#include "ThermalZone.hpp"


namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class EnergyManagementSystemProgramCallingManager;
class Connection;
//class WaterStorageTank;
class ThermalZone;

namespace detail {

  class PlantComponentUserDefined_Impl;

} // detail

/** PlantComponentUserDefined is a ModelObject that wraps the OpenStudio IDD object 'OS:PlantComponent:UserDefined'. */
class MODEL_API PlantComponentUserDefined : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantComponentUserDefined(const Model& model);

  virtual ~PlantComponentUserDefined() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> plantConnection1LoadingModeValues();

  static std::vector<std::string> plantConnection1LoopFlowRequestModeValues();

  static std::vector<std::string> plantConnection2LoadingModeValues();

  static std::vector<std::string> plantConnection2LoopFlowRequestModeValues();

  static std::vector<std::string> plantConnection3LoadingModeValues();

  static std::vector<std::string> plantConnection3LoopFlowRequestModeValues();

  static std::vector<std::string> plantConnection4LoadingModeValues();

  static std::vector<std::string> plantConnection4LoopFlowRequestModeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> mainModelProgramCallingManager() const;

  int numberofPlantLoopConnections() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection plantConnection1InletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection plantConnection1OutletNode() const;

  std::string plantConnection1LoadingMode() const;

  std::string plantConnection1LoopFlowRequestMode() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection1InitializationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection1SimulationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection2InletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection2OutletNode() const;

  boost::optional<std::string> plantConnection2LoadingMode() const;

  boost::optional<std::string> plantConnection2LoopFlowRequestMode() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection2InitializationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection2SimulationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection3InletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection3OutletNode() const;

  boost::optional<std::string> plantConnection3LoadingMode() const;

  boost::optional<std::string> plantConnection3LoopFlowRequestMode() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection3InitializationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection3SimulationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection4InletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> plantConnection4OutletNode() const;

  boost::optional<std::string> plantConnection4LoadingMode() const;

  boost::optional<std::string> plantConnection4LoopFlowRequestMode() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection4InitializationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantConnection4SimulationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airConnectionInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airConnectionOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
  //boost::optional<WaterStorageTank> supplyInletWaterStorageTank() const;

  // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
  //boost::optional<WaterStorageTank> collectionOutletWaterStorageTank() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> ambientZone() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetMainModelProgramCallingManager();

  bool setNumberofPlantLoopConnections(int numberofPlantLoopConnections);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection1InletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection1OutletNode(const Connection& connection);

  bool setPlantConnection1LoadingMode(const std::string& plantConnection1LoadingMode);

  bool setPlantConnection1LoopFlowRequestMode(const std::string& plantConnection1LoopFlowRequestMode);

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection1InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection1InitializationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection1SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection1SimulationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection2InletNode(const Connection& connection);

  void resetPlantConnection2InletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection2OutletNode(const Connection& connection);

  void resetPlantConnection2OutletNode();

  bool setPlantConnection2LoadingMode(const std::string& plantConnection2LoadingMode);

  void resetPlantConnection2LoadingMode();

  bool setPlantConnection2LoopFlowRequestMode(const std::string& plantConnection2LoopFlowRequestMode);

  void resetPlantConnection2LoopFlowRequestMode();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection2InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection2InitializationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection2SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection2SimulationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection3InletNode(const Connection& connection);

  void resetPlantConnection3InletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection3OutletNode(const Connection& connection);

  void resetPlantConnection3OutletNode();

  bool setPlantConnection3LoadingMode(const std::string& plantConnection3LoadingMode);

  void resetPlantConnection3LoadingMode();

  bool setPlantConnection3LoopFlowRequestMode(const std::string& plantConnection3LoopFlowRequestMode);

  void resetPlantConnection3LoopFlowRequestMode();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection3InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection3InitializationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection3SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection3SimulationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection4InletNode(const Connection& connection);

  void resetPlantConnection4InletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setPlantConnection4OutletNode(const Connection& connection);

  void resetPlantConnection4OutletNode();

  bool setPlantConnection4LoadingMode(const std::string& plantConnection4LoadingMode);

  void resetPlantConnection4LoadingMode();

  bool setPlantConnection4LoopFlowRequestMode(const std::string& plantConnection4LoopFlowRequestMode);

  void resetPlantConnection4LoopFlowRequestMode();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection4InitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection4InitializationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantConnection4SimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantConnection4SimulationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirConnectionInletNode(const Connection& connection);

  void resetAirConnectionInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirConnectionOutletNode(const Connection& connection);

  void resetAirConnectionOutletNode();

  // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
  //bool setSupplyInletWaterStorageTank(const WaterStorageTank& waterStorageTank);

  //void resetSupplyInletWaterStorageTank();

  // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
  //bool setCollectionOutletWaterStorageTank(const WaterStorageTank& waterStorageTank);

  //void resetCollectionOutletWaterStorageTank();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setAmbientZone(const ThermalZone& thermalZone);

  void resetAmbientZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PlantComponentUserDefined_Impl ImplType;

  explicit PlantComponentUserDefined(std::shared_ptr<detail::PlantComponentUserDefined_Impl> impl);

  friend class detail::PlantComponentUserDefined_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantComponentUserDefined");
};

/** \relates PlantComponentUserDefined*/
typedef boost::optional<PlantComponentUserDefined> OptionalPlantComponentUserDefined;

/** \relates PlantComponentUserDefined*/
typedef std::vector<PlantComponentUserDefined> PlantComponentUserDefinedVector;

} // model
} // openstudio

#endif // MODEL_PLANTCOMPONENTUSERDEFINED_HPP

