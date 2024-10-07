/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CoilUserDefined() override = default;
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
