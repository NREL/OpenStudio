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

#ifndef MODEL_COILUSERDEFINED_HPP
#define MODEL_COILUSERDEFINED_HPP

#include <model/ModelAPI.hpp>
#include "WaterToAirComponent.hpp"

namespace openstudio {
namespace model {

  class EnergyManagementSystemActuator;
  class EnergyManagementSystemProgram;
  class EnergyManagementSystemProgramCallingManager;
  class ThermalZone;

  namespace detail {

    class CoilUserDefined_Impl;

  }  // namespace detail

  /** CoilUserDefined is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:UserDefined'. */
  class MODEL_API CoilUserDefined : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilUserDefined(const Model& model);

    virtual ~CoilUserDefined() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilUserDefined(const CoilUserDefined& other) = default;
    CoilUserDefined(CoilUserDefined&& other) = default;
    CoilUserDefined& operator=(const CoilUserDefined&) = default;
    CoilUserDefined& operator=(CoilUserDefined&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    EnergyManagementSystemProgramCallingManager overallModelSimulationProgramCallingManager() const;

    EnergyManagementSystemProgramCallingManager modelSetupandSizingProgramCallingManager() const;

    boost::optional<ThermalZone> ambientZone() const;

    EnergyManagementSystemProgram overallSimulationProgram() const;

    EnergyManagementSystemProgram initializationSimulationProgram() const;

    EnergyManagementSystemActuator airOutletTemperatureActuator() const;

    EnergyManagementSystemActuator airOutletHumidityRatioActuator() const;

    EnergyManagementSystemActuator airMassFlowRateActuator() const;

    EnergyManagementSystemActuator plantMinimumMassFlowRateActuator() const;

    EnergyManagementSystemActuator plantMaximumMassFlowRateActuator() const;

    EnergyManagementSystemActuator plantDesignVolumeFlowRateActuator() const;

    EnergyManagementSystemActuator plantOutletTemperatureActuator() const;

    EnergyManagementSystemActuator plantMassFlowRateActuator() const;

    //@}
    /** @name Setters */
    //@{

    bool setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager);

    bool setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager);

    bool setAmbientZone(const ThermalZone& thermalZone);

    void resetAmbientZone();

    bool setOverallSimulationProgram(const EnergyManagementSystemProgram& emsProgram);

    bool setInitializationSimulationProgram(const EnergyManagementSystemProgram& emsProgram);

    bool setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setAirMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator);

    bool setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

    //@}
    /** @name Other */
    //@{

    int numberofAirConnections() const;

    // Will rename the EMS Program, Program Calling Manager, and Actuators to be nameString() + " xxx"
    void renameEMSSubComponents();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilUserDefined_Impl;

    explicit CoilUserDefined(std::shared_ptr<detail::CoilUserDefined_Impl> impl);

    friend class detail::CoilUserDefined_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilUserDefined");
  };

  /** \relates CoilUserDefined*/
  using OptionalCoilUserDefined = boost::optional<CoilUserDefined>;

  /** \relates CoilUserDefined*/
  using CoilUserDefinedVector = std::vector<CoilUserDefined>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILUSERDEFINED_HPP
