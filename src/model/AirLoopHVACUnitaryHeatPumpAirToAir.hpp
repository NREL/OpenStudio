/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class ThermalZone;

  class HVACComponent;

  namespace detail {

    class AirLoopHVACUnitaryHeatPumpAirToAir_Impl;

  }  // namespace detail

  /** AirLoopHVACUnitaryHeatPumpAirToAir is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatPump:AirToAir'. */
  class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAir : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirLoopHVACUnitaryHeatPumpAirToAir(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyFan, HVACComponent& heatingCoil,
                                       HVACComponent& coolingCoil, HVACComponent& supplementalHeatingCoil);

    virtual ~AirLoopHVACUnitaryHeatPumpAirToAir() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACUnitaryHeatPumpAirToAir(const AirLoopHVACUnitaryHeatPumpAirToAir& other) = default;
    AirLoopHVACUnitaryHeatPumpAirToAir(AirLoopHVACUnitaryHeatPumpAirToAir&& other) = default;
    AirLoopHVACUnitaryHeatPumpAirToAir& operator=(const AirLoopHVACUnitaryHeatPumpAirToAir&) = default;
    AirLoopHVACUnitaryHeatPumpAirToAir& operator=(AirLoopHVACUnitaryHeatPumpAirToAir&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validFanPlacementValues();

    static std::vector<std::string> validDehumidificationControlTypeValues();

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

    boost::optional<ThermalZone> controllingZone() const;

    HVACComponent supplyAirFan() const;

    HVACComponent heatingCoil() const;

    HVACComponent coolingCoil() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    /** Returns the supply air fan operating mode schedule, if present. In this schedule, values
   *  of zero correspond to AUTO fan operation (fan cycles on and off with coils to meet load).
   *  Other values indicate that the supply fan is to run continuously. */
    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    std::string dehumidificationControlType() const;

    bool isDehumidificationControlTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(double n2);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setControllingZone(ThermalZone& zone);

    void resetControllingZone();

    bool setSupplyAirFan(HVACComponent& hvacComponent);

    bool setHeatingCoil(HVACComponent& hvacComponent);

    bool setCoolingCoil(HVACComponent& hvacComponent);

    bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setDehumidificationControlType(const std::string& dehumidificationControlType);

    void resetDehumidificationControlType();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

    //@}
   protected:
    /// @cond

    using ImplType = detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl;

    explicit AirLoopHVACUnitaryHeatPumpAirToAir(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl> impl);

    friend class detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAir");
  };

  /** \relates AirLoopHVACUnitaryHeatPumpAirToAir*/
  using OptionalAirLoopHVACUnitaryHeatPumpAirToAir = boost::optional<AirLoopHVACUnitaryHeatPumpAirToAir>;

  /** \relates AirLoopHVACUnitaryHeatPumpAirToAir*/
  using AirLoopHVACUnitaryHeatPumpAirToAirVector = std::vector<AirLoopHVACUnitaryHeatPumpAirToAir>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP
