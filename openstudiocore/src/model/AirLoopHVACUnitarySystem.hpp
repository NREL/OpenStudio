/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP
#define MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class Schedule;
class HVACComponent;

namespace detail {

  class AirLoopHVACUnitarySystem_Impl;

} // detail

/** AirLoopHVACUnitarySystem is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitarySystem'. */
class MODEL_API AirLoopHVACUnitarySystem : public WaterToAirComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirLoopHVACUnitarySystem(const Model& model);

  virtual ~AirLoopHVACUnitarySystem() {}

  //@}

  static IddObjectType iddObjectType();

  // static std::vector<std::string> controlTypeValues();

  static std::vector<std::string> dehumidificationControlTypeValues();

  static std::vector<std::string> fanPlacementValues();

  static std::vector<std::string> latentLoadControlValues();

  static std::vector<std::string> supplyAirFlowRateMethodDuringCoolingOperationValues();

  static std::vector<std::string> supplyAirFlowRateMethodDuringHeatingOperationValues();

  static std::vector<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequiredValues();

  /** @name Getters */
  //@{

  // std::string controlType() const;

  // bool isControlTypeDefaulted() const;

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

  double dOASDXCoolingCoilLeavingMinimumAirTemperature() const;

  bool isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const;

  std::string latentLoadControl() const;

  bool isLatentLoadControlDefaulted() const;

  boost::optional<HVACComponent> supplementalHeatingCoil() const;

  boost::optional<std::string> supplyAirFlowRateMethodDuringCoolingOperation() const;

  boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

  bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const;

  boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRate() const;

  boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const;

  boost::optional<std::string> supplyAirFlowRateMethodDuringHeatingOperation() const;

  boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

  bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRatePerFloorAreaduringHeatingOperation() const;

  boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRate() const;

  boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const;

  boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const;

  boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

  bool isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const;

  boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const;

  boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

  boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

  boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const;

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

  double ancilliaryOnCycleElectricPower() const;

  bool isAncilliaryOnCycleElectricPowerDefaulted() const;

  double ancilliaryOffCycleElectricPower() const;

  bool isAncilliaryOffCycleElectricPowerDefaulted() const;

  // double designHeatRecoveryWaterFlowRate() const;

  // bool isDesignHeatRecoveryWaterFlowRateDefaulted() const;

  // double maximumTemperatureforHeatRecovery() const;

  // bool isMaximumTemperatureforHeatRecoveryDefaulted() const;

  // boost::optional<UnitarySystemPerformace> designSpecificationMultispeedHeatPumpObject() const;

  //@}
  /** @name Setters */
  //@{

  // bool setControlType(std::string controlType);

  // void resetControlType();

  bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);

  void resetControllingZoneorThermostatLocation();

  bool setDehumidificationControlType(std::string dehumidificationControlType);

  void resetDehumidificationControlType();

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setSupplyFan(const HVACComponent& supplyFan);

  void resetSupplyFan();

  bool setFanPlacement(std::string fanPlacement);

  void resetFanPlacement();

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  bool setHeatingCoil(const HVACComponent& heatingCoil);

  void resetHeatingCoil();

  bool setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio);

  void resetDXHeatingCoilSizingRatio();

  bool setCoolingCoil(const HVACComponent& coolingCoil);

  void resetCoolingCoil();

  void setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil);

  void resetUseDOASDXCoolingCoil();

  bool setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature);

  void resetDOASDXCoolingCoilLeavingMinimumAirTemperature();

  bool setLatentLoadControl(std::string latentLoadControl);

  void resetLatentLoadControl();

  bool setSupplementalHeatingCoil(const HVACComponent& supplementalHeatingCoil);

  void resetSupplementalHeatingCoil();

  bool setSupplyAirFlowRateMethodDuringCoolingOperation(std::string supplyAirFlowRateMethodDuringCoolingOperation);

  void resetSupplyAirFlowRateMethodDuringCoolingOperation();

  bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

  void resetSupplyAirFlowRateDuringCoolingOperation();

  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  bool setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(double supplyAirFlowRatePerFloorAreaDuringCoolingOperation);

  void resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();

  bool setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate);

  void resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();

  bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);

  void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();

  bool setSupplyAirFlowRateMethodDuringHeatingOperation(std::string supplyAirFlowRateMethodDuringHeatingOperation);

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

  bool setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(std::string supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);

  void resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();

  bool setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double supplyAirFlowRateWhenNoCoolingorHeatingisRequired);

  void resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

  void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

  bool setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(double supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired);

  void resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();

  bool setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

  void resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

  bool setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

  void resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

  bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired);

  void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();

  bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired);

  void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();

  void setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

  void resetMaximumSupplyAirTemperature();

  void autosizeMaximumSupplyAirTemperature();

  void setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

  void setOutdoorDryBulbTemperatureSensorNodeName(std::string outdoorDryBulbTemperatureSensorNodeName);

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

  // bool setDesignSpecificationMultispeedHeatPumpObject(const UnitarySystemPerformace& unitarySystemPerformace);

  // void resetDesignSpecificationMultispeedHeatPumpObject();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirLoopHVACUnitarySystem_Impl ImplType;

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
typedef boost::optional<AirLoopHVACUnitarySystem> OptionalAirLoopHVACUnitarySystem;

/** \relates AirLoopHVACUnitarySystem*/
typedef std::vector<AirLoopHVACUnitarySystem> AirLoopHVACUnitarySystemVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYSYSTEM_HPP

