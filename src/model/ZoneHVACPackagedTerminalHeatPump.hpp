/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACPackagedTerminalHeatPump_Impl;

  }  // namespace detail

  /** ZoneHVACPackagedTerminalHeatPump is a ZoneHVACComponent that wraps the
 *  OpenStudio IDD object 'OS:ZoneHVAC:PackagedTerminalHeatPump'. */
  class MODEL_API ZoneHVACPackagedTerminalHeatPump : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACPackagedTerminalHeatPump(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan, HVACComponent& heatingCoil,
                                     HVACComponent& coolingCoil, HVACComponent& supplementalHeatingCoil);

    virtual ~ZoneHVACPackagedTerminalHeatPump() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACPackagedTerminalHeatPump(const ZoneHVACPackagedTerminalHeatPump& other) = default;
    ZoneHVACPackagedTerminalHeatPump(ZoneHVACPackagedTerminalHeatPump&& other) = default;
    ZoneHVACPackagedTerminalHeatPump& operator=(const ZoneHVACPackagedTerminalHeatPump&) = default;
    ZoneHVACPackagedTerminalHeatPump& operator=(ZoneHVACPackagedTerminalHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> outdoorAirMixerObjectTypeValues();

    /** \deprecated */
    static std::vector<std::string> validOutdoorAirMixerObjectTypeValues();

    static std::vector<std::string> fanPlacementValues();

    /** \deprecated */
    static std::vector<std::string> validFanPlacementValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string outdoorAirMixerObjectType() const;

    std::string outdoorAirMixerName() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    bool noLoadSupplyAirFlowRateControlSetToLowSpeed() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

    bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

    bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent heatingCoil() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    /** Not used as of EnergyPlus 8.8.0 and above **/
    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

    HVACComponent coolingCoil() const;

    double coolingConvergenceTolerance() const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    Schedule supplyAirFanOperatingModeSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType);

    bool setOutdoorAirMixerName(const std::string& outdoorAirMixerName);

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setNoLoadSupplyAirFlowRateControlSetToLowSpeed(bool noLoadSupplyAirFlowRateControlSetToLowSpeed);

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setSupplyAirFan(HVACComponent& hvacComponent);

    bool setHeatingCoil(HVACComponent& hvacComponent);

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    /** Not used as of EnergyPlus 8.8.0 and above **/
    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

    bool setCoolingCoil(HVACComponent& hvacComponent);

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    /** Sets the supply air fan operating mode schedule. Values of zero indicate that the fan
   *  cycles on and off with the active (heating or cooling) coil. Other values operate the
   *  fan continuously. */
    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACPackagedTerminalHeatPump_Impl;

    explicit ZoneHVACPackagedTerminalHeatPump(std::shared_ptr<detail::ZoneHVACPackagedTerminalHeatPump_Impl> impl);

    friend class detail::ZoneHVACPackagedTerminalHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalHeatPump");
  };

  /** \relates ZoneHVACPackagedTerminalHeatPump*/
  using OptionalZoneHVACPackagedTerminalHeatPump = boost::optional<ZoneHVACPackagedTerminalHeatPump>;

  /** \relates ZoneHVACPackagedTerminalHeatPump*/
  using ZoneHVACPackagedTerminalHeatPumpVector = std::vector<ZoneHVACPackagedTerminalHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP
