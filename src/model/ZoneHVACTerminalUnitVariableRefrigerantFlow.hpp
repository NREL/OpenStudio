/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class HVACComponent;
  class Schedule;
  class CoilHeatingDXVariableRefrigerantFlow;
  class CoilCoolingDXVariableRefrigerantFlow;
  class CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl;
  class CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl;
  class ThermalZone;

  namespace detail {

    class ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;

  }  // namespace detail

  /** ZoneHVACTerminalUnitVariableRefrigerantFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow'. */
  class MODEL_API ZoneHVACTerminalUnitVariableRefrigerantFlow : public ZoneHVACComponent
  {

   public:
    explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model, bool isFluidTemperatureControl = false);

    explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model, const CoilCoolingDXVariableRefrigerantFlow& coolingCoil,
                                                         const CoilHeatingDXVariableRefrigerantFlow& heatingCoil, const HVACComponent& fan);

    explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model,
                                                         const CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl& coolingCoil,
                                                         const CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl& heatingCoil,
                                                         const HVACComponent& fan);

    virtual ~ZoneHVACTerminalUnitVariableRefrigerantFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACTerminalUnitVariableRefrigerantFlow(const ZoneHVACTerminalUnitVariableRefrigerantFlow& other) = default;
    ZoneHVACTerminalUnitVariableRefrigerantFlow(ZoneHVACTerminalUnitVariableRefrigerantFlow&& other) = default;
    ZoneHVACTerminalUnitVariableRefrigerantFlow& operator=(const ZoneHVACTerminalUnitVariableRefrigerantFlow&) = default;
    ZoneHVACTerminalUnitVariableRefrigerantFlow& operator=(ZoneHVACTerminalUnitVariableRefrigerantFlow&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> supplyAirFanPlacementValues();

    Schedule terminalUnitAvailabilityschedule() const;

    bool setTerminalUnitAvailabilityschedule(Schedule& schedule);

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Cooling Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const;

    bool setSupplyAirFlowRateWhenNoCoolingisNeeded(double supplyAirFlowRateWhenNoCoolingisNeeded);

    void autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Heating Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const;

    bool setSupplyAirFlowRateWhenNoHeatingisNeeded(double supplyAirFlowRateWhenNoHeatingisNeeded);

    void autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

    bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

    bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    Schedule supplyAirFanOperatingModeSchedule() const;

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    // Required for zone equipment. Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
    // Also leave blank if terminal unit is used on main AirloopHVAC branch and terminal unit has no fan.
    // TODO: not breaking API (yet), so leaving it as non optional
    HVACComponent supplyAirFan() const;  // TODO: OS_DEPRECATED
    // bool setSupplyAirFan(const HVACComponent& fan);
    // void resetSupplyAirFan();

    boost::optional<HVACComponent> coolingCoil() const;

    // Using a single type of coils is enforced: both must be FluidTemperatureControl or Non-FluidCtrl
    bool setCoolingCoil(const HVACComponent& coil);

    boost::optional<HVACComponent> heatingCoil() const;

    // Using a single type of coils is enforced: both must be FluidTemperatureControl or Non-FluidCtrl
    bool setHeatingCoil(const HVACComponent& coil);

    double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

    bool setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse);

    double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

    bool setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse);

    double ratedTotalHeatingCapacitySizingRatio() const;

    bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

    // \field Availability Manager List Name
    // \field Design Specification ZoneHVAC Sizing Object Name

    // Supplemental Heating Coil Name
    boost::optional<HVACComponent> supplementalHeatingCoil() const;
    bool setSupplementalHeatingCoil(const HVACComponent& coil);
    void resetSupplementalHeatingCoil();

    // Maximum Supply Air Temperature from Supplemental Heater (autosized)
    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;
    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;
    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);
    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    // Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation (default 21C)
    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;
    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    // Supply Air Fan Placement
    std::string supplyAirFanPlacement() const;
    bool setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement);

    // Controlling Zone or Thermostat Location
    boost::optional<ThermalZone> controllingZoneorThermostatLocation() const;
    bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);
    void resetControllingZoneorThermostatLocation();

    boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const;

    boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

    //@}
    /** @name Other */
    //@{

    // Returns true if the Cooling and Heating Coils are of the FluidTemperatureControl type
    bool isFluidTemperatureControl() const;

    /** Convenience function to return parent VRF System:
     * - AirConditionerVariableRefrigerantFlow
     * - AirConditionerVariableRefrigerantFlowFluidTemperatureControl
     * - AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR */
    boost::optional<HVACComponent> vrfSystem() const;

   protected:
    /// @cond
    using ImplType = detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;

    explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(std::shared_ptr<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl> impl);

    friend class detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACTerminalUnitVariableRefrigerantFlow");
  };

  /** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
  using OptionalZoneHVACTerminalUnitVariableRefrigerantFlow = boost::optional<ZoneHVACTerminalUnitVariableRefrigerantFlow>;

  /** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
  using ZoneHVACTerminalUnitVariableRefrigerantFlowVector = std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP
