/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_PLANTCOMPONENTUSERDEFINED_HPP
#define MODEL_PLANTCOMPONENTUSERDEFINED_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemActuator.hpp"
#include "StraightComponent.hpp"
#include "ThermalZone.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class EnergyManagementSystemProgramCallingManager;
  class EnergyManagementSystemProgram;
  class EnergyManagementSystemActuator;
  class Connection;
  class Node;
  class ThermalZone;

  namespace detail {

    class PlantComponentUserDefined_Impl;

  }  // namespace detail

  /** PlantComponentUserDefined is a StraightComponent that wraps the OpenStudio IDD object 'OS:PlantComponent:UserDefined'. */
  class MODEL_API PlantComponentUserDefined : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantComponentUserDefined(const Model& model);

    virtual ~PlantComponentUserDefined() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantComponentUserDefined(const PlantComponentUserDefined& other) = default;
    PlantComponentUserDefined(PlantComponentUserDefined&& other) = default;
    PlantComponentUserDefined& operator=(const PlantComponentUserDefined&) = default;
    PlantComponentUserDefined& operator=(PlantComponentUserDefined&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> plantLoadingModeValues();

    static std::vector<std::string> plantLoopFlowRequestModeValues();

    /** @name Getters */
    //@{

    std::string plantLoadingMode() const;

    std::string plantLoopFlowRequestMode() const;

    boost::optional<EnergyManagementSystemProgramCallingManager> mainModelProgramCallingManager() const;

    boost::optional<EnergyManagementSystemProgramCallingManager> plantInitializationProgramCallingManager() const;

    boost::optional<EnergyManagementSystemProgramCallingManager> plantSimulationProgramCallingManager() const;

    boost::optional<EnergyManagementSystemProgram> mainModelProgram() const;

    boost::optional<EnergyManagementSystemProgram> plantInitializationProgram() const;

    boost::optional<EnergyManagementSystemProgram> plantSimulationProgram() const;

    boost::optional<ThermalZone> ambientZone() const;

    boost::optional<EnergyManagementSystemActuator> designVolumeFlowRateActuator() const;

    boost::optional<EnergyManagementSystemActuator> minimumMassFlowRateActuator() const;

    boost::optional<EnergyManagementSystemActuator> maximumMassFlowRateActuator() const;

    boost::optional<EnergyManagementSystemActuator> minimumLoadingCapacityActuator() const;

    boost::optional<EnergyManagementSystemActuator> maximumLoadingCapacityActuator() const;

    boost::optional<EnergyManagementSystemActuator> optimalLoadingCapacityActuator() const;

    boost::optional<EnergyManagementSystemActuator> outletTemperatureActuator() const;

    boost::optional<EnergyManagementSystemActuator> massFlowRateActuator() const;

    //@}
    /** @name Setters */
    //@{

    bool setPlantLoadingMode(const std::string& plantLoadingMode);

    bool setPlantLoopFlowRequestMode(const std::string& plantLoopFlowRequestMode);

    bool setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

    void resetMainModelProgramCallingManager();

    bool setPlantInitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

    void resetPlantInitializationProgramCallingManager();

    bool setPlantSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

    void resetPlantSimulationProgramCallingManager();

    bool setMainModelProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

    void resetMainModelProgram();

    bool setPlantInitializationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

    void resetPlantInitializationProgram();

    bool setPlantSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

    void resetPlantSimulationProgram();

    bool setAmbientZone(const ThermalZone& thermalZone);

    void resetAmbientZone();

    bool setDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setMinimumLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setMaximumLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setOptimalLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    bool setMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

    EnergyManagementSystemProgram
      createInitProgram(const EnergyManagementSystemActuator& dvfrActuator, const EnergyManagementSystemActuator& mmfrActuator,
                        const EnergyManagementSystemActuator& mxfrActuator, const EnergyManagementSystemActuator& mlcActuator,
                        const EnergyManagementSystemActuator& mxlcActuator, const EnergyManagementSystemActuator& olcActuator, const Model& model);

    EnergyManagementSystemProgram createSimProgram(const EnergyManagementSystemActuator& otActuator,
                                                   const EnergyManagementSystemActuator& mfrActuator, const Model& model);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PlantComponentUserDefined_Impl;

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
  using OptionalPlantComponentUserDefined = boost::optional<PlantComponentUserDefined>;

  /** \relates PlantComponentUserDefined*/
  using PlantComponentUserDefinedVector = std::vector<PlantComponentUserDefined>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTCOMPONENTUSERDEFINED_HPP
