/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP
#define MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Schedule;
  class HVACComponent;
  class UnitarySystemPerformanceMultispeed;

  namespace detail {

    class AirLoopHVACUnitarySystem_Impl;

  }  // namespace detail

  /** AirLoopHVACUnitarySystem is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitarySystem'. */
  class MODEL_API AirLoopHVACUnitarySystem : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirLoopHVACUnitarySystem(const Model& model);

    virtual ~AirLoopHVACUnitarySystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACUnitarySystem(const AirLoopHVACUnitarySystem& other) = default;
    AirLoopHVACUnitarySystem(AirLoopHVACUnitarySystem&& other) = default;
    AirLoopHVACUnitarySystem& operator=(const AirLoopHVACUnitarySystem&) = default;
    AirLoopHVACUnitarySystem& operator=(AirLoopHVACUnitarySystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlTypeValues();

    static std::vector<std::string> dehumidificationControlTypeValues();

    static std::vector<std::string> fanPlacementValues();

    static std::vector<std::string> latentLoadControlValues();

    static std::vector<std::string> supplyAirFlowRateMethodDuringCoolingOperationValues();

    static std::vector<std::string> supplyAirFlowRateMethodDuringHeatingOperationValues();

    static std::vector<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequiredValues();

    /** @name Getters */
    //@{

    std::string controlType() const;

    bool isControlTypeDefaulted() const;

    boost::optional<ThermalZone> controllingZoneorThermostatLocation() const;

    std::string dehumidificationControlType() const;

    bool isDehumidificationControlTypeDefaulted() const;

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<HVACComponent> supplyFan() const;

    boost::optional<std::string> fanPlacement() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    boost::optional<HVACComponent> heatingCoil() const;

    double dXHeatingCoilSizingRatio() const;

    bool isDXHeatingCoilSizingRatioDefaulted() const;

    boost::optional<HVACComponent> coolingCoil() const;

    bool useDOASDXCoolingCoil() const;

    bool isUseDOASDXCoolingCoilDefaulted() const;

    /** As of EnergyPlus version 8.7.0 this field maps to MinimumSupplyAirTemperature **/
    double dOASDXCoolingCoilLeavingMinimumAirTemperature() const;

    // Note JM 2019-09-27: I would probably make this field \required instead and remove this.
    bool isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const;

    bool isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized() const;

    std::string latentLoadControl() const;

    bool isLatentLoadControlDefaulted() const;

    boost::optional<HVACComponent> supplementalHeatingCoil() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate Method" **/
    std::string supplyAirFlowRateMethodDuringCoolingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate Per Floor Area" **/
    boost::optional<double> supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Fraction of Autosized Cooling Supply Air Flow Rate" **/
    boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRate() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate Per Unit of Capacity" **/
    boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate Method" **/
    boost::optional<std::string> supplyAirFlowRateMethodDuringHeatingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate Per Floor Area" **/
    boost::optional<double> supplyAirFlowRatePerFloorAreaduringHeatingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Fraction of Autosized Heating Supply Air Flow Rate" **/
    boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRate() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate Per Unit of Capacity" **/
    boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate Method" **/
    boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate Per Floor Area" **/
    boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Fraction of Autosized Cooling Supply Air Flow Rate" **/
    boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Fraction of Autosized Heating Supply Air Flow Rate" **/
    boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate Per Unit of Capacity During Cooling Operation" **/
    boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate Per Unit of Capacity During Heating Operation" **/
    boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const;

    boost::optional<double> maximumSupplyAirTemperature() const;

    bool isMaximumSupplyAirTemperatureDefaulted() const;

    bool isMaximumSupplyAirTemperatureAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    boost::optional<std::string> outdoorDryBulbTemperatureSensorNodeName() const;

    double maximumCyclingRate() const;

    bool isMaximumCyclingRateDefaulted() const;

    double heatPumpTimeConstant() const;

    bool isHeatPumpTimeConstantDefaulted() const;

    double fractionofOnCyclePowerUse() const;

    bool isFractionofOnCyclePowerUseDefaulted() const;

    double heatPumpFanDelayTime() const;

    bool isHeatPumpFanDelayTimeDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Ancillary On-Cycle Electric Power" **/
    double ancilliaryOnCycleElectricPower() const;

    bool isAncilliaryOnCycleElectricPowerDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Ancillary Off-Cycle Electric Power" **/
    double ancilliaryOffCycleElectricPower() const;

    bool isAncilliaryOffCycleElectricPowerDefaulted() const;

    // double designHeatRecoveryWaterFlowRate() const;

    // bool isDesignHeatRecoveryWaterFlowRateDefaulted() const;

    // double maximumTemperatureforHeatRecovery() const;

    // bool isMaximumTemperatureforHeatRecoveryDefaulted() const;

    boost::optional<UnitarySystemPerformanceMultispeed> designSpecificationMultispeedObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlType(const std::string& controlType);

    void resetControlType();

    bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);

    void resetControllingZoneorThermostatLocation();

    bool setDehumidificationControlType(const std::string& dehumidificationControlType);

    void resetDehumidificationControlType();

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setSupplyFan(const HVACComponent& supplyFan);

    void resetSupplyFan();

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    void resetHeatingCoil();

    bool setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio);

    void resetDXHeatingCoilSizingRatio();

    bool setCoolingCoil(const HVACComponent& coolingCoil);

    void resetCoolingCoil();

    bool setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil);

    void resetUseDOASDXCoolingCoil();

    /** As of EnergyPlus version 8.7.0 this field maps to MinimumSupplyAirTemperature **/
    bool setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature);

    void autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature();

    // Would remove
    void resetDOASDXCoolingCoilLeavingMinimumAirTemperature();

    bool setLatentLoadControl(const std::string& latentLoadControl);

    void resetLatentLoadControl();

    bool setSupplementalHeatingCoil(const HVACComponent& supplementalHeatingCoil);

    void resetSupplementalHeatingCoil();

    OS_DEPRECATED(3, 7, 0) bool setSupplyAirFlowRateMethodDuringCoolingOperation(const std::string& supplyAirFlowRateMethodDuringCoolingOperation);
    OS_DEPRECATED(3, 7, 0) void resetSupplyAirFlowRateMethodDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);
    OS_DEPRECATED(3, 7, 0) void resetSupplyAirFlowRateDuringCoolingOperation();
    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(double supplyAirFlowRatePerFloorAreaDuringCoolingOperation);
    OS_DEPRECATED(3, 7, 0) void resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();

    bool setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate);
    OS_DEPRECATED(3, 7, 0) void resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);
    OS_DEPRECATED(3, 7, 0) void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();

    bool setSupplyAirFlowRateMethodDuringHeatingOperation(const std::string& supplyAirFlowRateMethodDuringHeatingOperation);

    void resetSupplyAirFlowRateMethodDuringHeatingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void resetSupplyAirFlowRateDuringHeatingOperation();

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(double supplyAirFlowRatePerFloorAreaduringHeatingOperation);

    void resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();

    bool setFractionofAutosizedDesignHeatingSupplyAirFlowRate(double fractionofAutosizedDesignHeatingSupplyAirFlowRate);

    void resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();

    bool setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(const std::string& supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double supplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(double supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();

    bool setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      double fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      double fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(
      double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(
      double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();

    bool setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

    void resetMaximumSupplyAirTemperature();

    void autosizeMaximumSupplyAirTemperature();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    bool setOutdoorDryBulbTemperatureSensorNodeName(const std::string& outdoorDryBulbTemperatureSensorNodeName);

    void resetOutdoorDryBulbTemperatureSensorNodeName();

    bool setMaximumCyclingRate(double maximumCyclingRate);

    void resetMaximumCyclingRate();

    bool setHeatPumpTimeConstant(double heatPumpTimeConstant);

    void resetHeatPumpTimeConstant();

    bool setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse);

    void resetFractionofOnCyclePowerUse();

    bool setHeatPumpFanDelayTime(double heatPumpFanDelayTime);

    void resetHeatPumpFanDelayTime();

    bool setAncilliaryOnCycleElectricPower(double ancilliaryOnCycleElectricPower);

    void resetAncilliaryOnCycleElectricPower();

    bool setAncilliaryOffCycleElectricPower(double ancilliaryOffCycleElectricPower);

    void resetAncilliaryOffCycleElectricPower();

    // bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

    // void resetDesignHeatRecoveryWaterFlowRate();

    // bool setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery);

    // void resetMaximumTemperatureforHeatRecovery();

    bool setDesignSpecificationMultispeedObject(const UnitarySystemPerformanceMultispeed& unitarySystemPerformace);

    void resetDesignSpecificationMultispeedObject();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

    boost::optional<double> autosizedMaximumSupplyAirTemperature() const;

    boost::optional<double> autosizedDOASDXCoolingCoilLeavingMinimumAirTemperature() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACUnitarySystem_Impl;

    explicit AirLoopHVACUnitarySystem(std::shared_ptr<detail::AirLoopHVACUnitarySystem_Impl> impl);

    friend class detail::AirLoopHVACUnitarySystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitarySystem");
  };

  /** \relates AirLoopHVACUnitarySystem*/
  using OptionalAirLoopHVACUnitarySystem = boost::optional<AirLoopHVACUnitarySystem>;

  /** \relates AirLoopHVACUnitarySystem*/
  using AirLoopHVACUnitarySystemVector = std::vector<AirLoopHVACUnitarySystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP
