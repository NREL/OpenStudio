/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP
#define MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class HVACComponent;
  class Curve;

  namespace detail {

    class CoilSystemIntegratedHeatPumpAirSource_Impl;

  }  // namespace detail

  /** CoilSystemIntegratedHeatPumpAirSource is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:IntegratedHeatPump:AirSource'. */
  class MODEL_API CoilSystemIntegratedHeatPumpAirSource : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilSystemIntegratedHeatPumpAirSource(const Model& model);

    explicit CoilSystemIntegratedHeatPumpAirSource(const Model& model, const StraightComponent& spaceCoolingCoil,
                                                   const StraightComponent& spaceHeatingCoil, const HVACComponent& dedicatedWaterHeatingCoil,
                                                   const HVACComponent& scwhCoil, const StraightComponent& scdwhCoolingCoil,
                                                   const HVACComponent& scdwhWaterHeatingCoil, const StraightComponent& shdwhHeatingCoil,
                                                   const HVACComponent& shdwhWaterHeatingCoil);

    virtual ~CoilSystemIntegratedHeatPumpAirSource() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilSystemIntegratedHeatPumpAirSource(const CoilSystemIntegratedHeatPumpAirSource& other) = default;
    CoilSystemIntegratedHeatPumpAirSource(CoilSystemIntegratedHeatPumpAirSource&& other) = default;
    CoilSystemIntegratedHeatPumpAirSource& operator=(const CoilSystemIntegratedHeatPumpAirSource&) = default;
    CoilSystemIntegratedHeatPumpAirSource& operator=(CoilSystemIntegratedHeatPumpAirSource&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    StraightComponent spaceCoolingCoil() const;

    StraightComponent spaceHeatingCoil() const;

    HVACComponent dedicatedWaterHeatingCoil() const;

    HVACComponent scwhCoil() const;

    StraightComponent scdwhCoolingCoil() const;

    HVACComponent scdwhWaterHeatingCoil() const;

    StraightComponent shdwhHeatingCoil() const;

    HVACComponent shdwhWaterHeatingCoil() const;

    double indoorTemperatureLimitForSCWHMode() const;

    double ambientTemperatureLimitForSCWHMode() const;

    double indoorTemperatureAboveWhichWHHasHigherPriority() const;

    double ambientTemperatureAboveWhichWHHasHigherPriority() const;

    int flagtoIndicateLoadControlInSCWHMode() const;

    int minimumSpeedLevelForSCWHMode() const;

    double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const;

    int minimumSpeedLevelForSCDWHMode() const;

    double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const;

    int minimumSpeedLevelForSHDWHMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil);

    bool setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil);

    bool setDedicatedWaterHeatingCoil(const HVACComponent& dedicatedWaterHeatingCoil);

    bool setSCWHCoil(const HVACComponent& scwhCoil);

    bool setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil);

    bool setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil);

    bool setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil);

    bool setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil);

    bool setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode);

    bool setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode);

    bool setIndoorTemperatureAboveWhichWHHasHigherPriority(double indoorTemperatureAboveWhichWHHasHigherPriority);

    bool setAmbientTemperatureAboveWhichWHHasHigherPriority(double ambientTemperatureAboveWhichWHHasHigherPriority);

    bool setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode);

    bool setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode);

    bool setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);

    bool setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode);

    bool setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
      double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);

    bool setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilSystemIntegratedHeatPumpAirSource_Impl;

    explicit CoilSystemIntegratedHeatPumpAirSource(std::shared_ptr<detail::CoilSystemIntegratedHeatPumpAirSource_Impl> impl);

    friend class detail::CoilSystemIntegratedHeatPumpAirSource_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilSystemIntegratedHeatPumpAirSource");
  };

  /** \relates CoilSystemIntegratedHeatPumpAirSource*/
  using OptionalCoilSystemIntegratedHeatPumpAirSource = boost::optional<CoilSystemIntegratedHeatPumpAirSource>;

  /** \relates CoilSystemIntegratedHeatPumpAirSource*/
  using CoilSystemIntegratedHeatPumpAirSourceVector = std::vector<CoilSystemIntegratedHeatPumpAirSource>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_HPP
