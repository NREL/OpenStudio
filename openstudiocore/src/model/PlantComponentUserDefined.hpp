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
#include "StraightComponent.hpp"
#include "ThermalZone.hpp"


namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class EnergyManagementSystemProgramCallingManager;
class Connection;
class Node;
class ThermalZone;

namespace detail {

  class PlantComponentUserDefined_Impl;

} // detail

/** PlantComponentUserDefined is a StraightComponent that wraps the OpenStudio IDD object 'OS:PlantComponent:UserDefined'. */
class MODEL_API PlantComponentUserDefined : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantComponentUserDefined(const Model& model);

  virtual ~PlantComponentUserDefined() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> plantLoadingModeValues();

  static std::vector<std::string> plantLoopFlowRequestModeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> mainModelProgramCallingManager() const;

  std::string plantLoadingMode() const;

  std::string plantLoopFlowRequestMode() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantInitializationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  boost::optional<EnergyManagementSystemProgramCallingManager> plantSimulationProgramCallingManager() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> ambientZone() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetMainModelProgramCallingManager();

  bool setPlantLoadingMode(const std::string& plantLoadingMode);

  bool setPlantLoopFlowRequestMode(const std::string& plantLoopFlowRequestMode);

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantInitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantInitializationProgramCallingManager();

  // TODO: Check argument type. From object lists, some candidates are: EnergyManagementSystemProgramCallingManager.
  bool setPlantSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

  void resetPlantSimulationProgramCallingManager();

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

