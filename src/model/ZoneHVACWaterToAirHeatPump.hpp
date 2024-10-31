/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP
#define MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACWaterToAirHeatPump_Impl;

  }  // namespace detail

  /** ZoneHVACWaterToAirHeatPump is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:WaterToAirHeatPump'. */
  class MODEL_API ZoneHVACWaterToAirHeatPump : public ZoneHVACComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACWaterToAirHeatPump(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan, HVACComponent& heatingCoil,
                               HVACComponent& coolingCoil, HVACComponent& supplementalHeatingCoil);

    virtual ~ZoneHVACWaterToAirHeatPump() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACWaterToAirHeatPump(const ZoneHVACWaterToAirHeatPump& other) = default;
    ZoneHVACWaterToAirHeatPump(ZoneHVACWaterToAirHeatPump&& other) = default;
    ZoneHVACWaterToAirHeatPump& operator=(const ZoneHVACWaterToAirHeatPump&) = default;
    ZoneHVACWaterToAirHeatPump& operator=(ZoneHVACWaterToAirHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanPlacementValues();

    static std::vector<std::string> heatPumpCoilWaterFlowModeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

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

    HVACComponent coolingCoil() const;

    OS_DEPRECATED(3, 7, 0) double maximumCyclingRate() const;

    OS_DEPRECATED(3, 7, 0) bool isMaximumCyclingRateDefaulted() const;

    OS_DEPRECATED(3, 7, 0) double heatPumpTimeConstant() const;

    OS_DEPRECATED(3, 7, 0) bool isHeatPumpTimeConstantDefaulted() const;

    OS_DEPRECATED(3, 7, 0) double fractionofOnCyclePowerUse() const;

    OS_DEPRECATED(3, 7, 0) bool isFractionofOnCyclePowerUseDefaulted() const;

    OS_DEPRECATED(3, 7, 0) double heatPumpFanDelayTime() const;

    OS_DEPRECATED(3, 7, 0) bool isHeatPumpFanDelayTimeDefaulted() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    std::string heatPumpCoilWaterFlowMode() const;

    bool isHeatPumpCoilWaterFlowModeDefaulted() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    // TODO: field 'Availability Manager List Name' isn't implemented

    // TODO: field 'Design Specification ZoneHVAC Sizing' isn't implemented since the object isn't wrapped in SDK

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void resetSupplyAirFlowRateDuringCoolingOperation();

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void resetSupplyAirFlowRateDuringHeatingOperation();

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setNoLoadSupplyAirFlowRateControlSetToLowSpeed(bool noLoadSupplyAirFlowRateControlSetToLowSpeed);

    bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void resetOutdoorAirFlowRateDuringCoolingOperation();

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void resetOutdoorAirFlowRateDuringHeatingOperation();

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setSupplyAirFan(HVACComponent& fansOnOff);

    bool setHeatingCoil(HVACComponent& heatingCoils);

    bool setCoolingCoil(HVACComponent& coolingCoils);

    OS_DEPRECATED(3, 7, 0) bool setMaximumCyclingRate(boost::optional<double> maximumCyclingRate);

    OS_DEPRECATED(3, 7, 0) bool setMaximumCyclingRate(double maximumCyclingRate);

    OS_DEPRECATED(3, 7, 0) void resetMaximumCyclingRate();

    OS_DEPRECATED(3, 7, 0) bool setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant);

    OS_DEPRECATED(3, 7, 0) bool setHeatPumpTimeConstant(double heatPumpTimeConstant);

    OS_DEPRECATED(3, 7, 0) void resetHeatPumpTimeConstant();

    OS_DEPRECATED(3, 7, 0) bool setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse);

    OS_DEPRECATED(3, 7, 0) bool setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse);

    OS_DEPRECATED(3, 7, 0) void resetFractionofOnCyclePowerUse();

    OS_DEPRECATED(3, 7, 0) bool setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime);

    OS_DEPRECATED(3, 7, 0) bool setHeatPumpFanDelayTime(double heatPumpFanDelayTime);

    OS_DEPRECATED(3, 7, 0) void resetHeatPumpFanDelayTime();

    bool setSupplementalHeatingCoil(HVACComponent& heatingCoilName);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void resetMaximumSupplyAirTemperaturefromSupplementalHeater();

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      boost::optional<double> maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    // DLM: this appears to not be implemented
    //bool setOutdoorDryBulbTemperatureSensorNodeName(const std::string& outdoorDryBulbTemperatureSensorNodeName);

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    bool setHeatPumpCoilWaterFlowMode(const std::string& heatPumpCoilWaterFlowMode);

    void resetHeatPumpCoilWaterFlowMode();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

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
    using ImplType = detail::ZoneHVACWaterToAirHeatPump_Impl;

    explicit ZoneHVACWaterToAirHeatPump(std::shared_ptr<detail::ZoneHVACWaterToAirHeatPump_Impl> impl);

    friend class detail::ZoneHVACWaterToAirHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACWaterToAirHeatPump");
  };

  /** \relates ZoneHVACWaterToAirHeatPump*/
  using OptionalZoneHVACWaterToAirHeatPump = boost::optional<ZoneHVACWaterToAirHeatPump>;

  /** \relates ZoneHVACWaterToAirHeatPump*/
  using ZoneHVACWaterToAirHeatPumpVector = std::vector<ZoneHVACWaterToAirHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP
