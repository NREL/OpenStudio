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

#ifndef MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class Schedule;
class HVACComponent;
class Node;

namespace detail {

  /** AirLoopHVACUnitarySystem_Impl is a WaterToAirComponent_Impl that is the implementation class for AirLoopHVACUnitarySystem.*/
  class MODEL_API AirLoopHVACUnitarySystem_Impl : public WaterToAirComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirLoopHVACUnitarySystem_Impl(const IdfObject& idfObject,
                                  Model_Impl* model,
                                  bool keepHandle);

    AirLoopHVACUnitarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    AirLoopHVACUnitarySystem_Impl(const AirLoopHVACUnitarySystem_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~AirLoopHVACUnitarySystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual std::vector<IdfObject> remove();

    virtual unsigned airInletPort();
   
    virtual unsigned airOutletPort();

    virtual unsigned waterInletPort();

    virtual unsigned waterOutletPort();

    virtual bool addToNode(Node & node);

    //@}
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

    bool setControllingZoneorThermostatLocation(const boost::optional<ThermalZone>& thermalZone);

    void resetControllingZoneorThermostatLocation();

    bool setDehumidificationControlType(std::string dehumidificationControlType);

    void resetDehumidificationControlType();

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setSupplyFan(const boost::optional<HVACComponent>& supplyFan);

    void resetSupplyFan();

    bool setFanPlacement(boost::optional<std::string> fanPlacement);

    void resetFanPlacement();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setHeatingCoil(const boost::optional<HVACComponent>& heatingCoil);

    void resetHeatingCoil();

    bool setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio);

    void resetDXHeatingCoilSizingRatio();

    bool setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil);

    void resetCoolingCoil();

    void setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil);

    void resetUseDOASDXCoolingCoil();

    bool setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature);

    void resetDOASDXCoolingCoilLeavingMinimumAirTemperature();

    bool setLatentLoadControl(std::string latentLoadControl);

    void resetLatentLoadControl();

    bool setSupplementalHeatingCoil(const boost::optional<HVACComponent>& supplementalHeatingCoil);

    void resetSupplementalHeatingCoil();

    bool setSupplyAirFlowRateMethodDuringCoolingOperation(boost::optional<std::string> supplyAirFlowRateMethodDuringCoolingOperation);

    void resetSupplyAirFlowRateMethodDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

    void resetSupplyAirFlowRateDuringCoolingOperation();

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(boost::optional<double> supplyAirFlowRatePerFloorAreaDuringCoolingOperation);

    void resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();

    bool setFractionofAutosizedDesignCoolingSupplyAirFlowRate(boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRate);

    void resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();

    bool setSupplyAirFlowRateMethodDuringHeatingOperation(boost::optional<std::string> supplyAirFlowRateMethodDuringHeatingOperation);

    void resetSupplyAirFlowRateMethodDuringHeatingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

    void resetSupplyAirFlowRateDuringHeatingOperation();

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(boost::optional<double> supplyAirFlowRatePerFloorAreaduringHeatingOperation);

    void resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();

    bool setFractionofAutosizedDesignHeatingSupplyAirFlowRate(boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRate);

    void resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();

    bool setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired);

    void resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();

    bool setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);

    void resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();

    bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired);

    void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();

    void setMaximumSupplyAirTemperature(boost::optional<double> maximumSupplyAirTemperature);

    void resetMaximumSupplyAirTemperature();

    void autosizeMaximumSupplyAirTemperature();

    void setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    void setOutdoorDryBulbTemperatureSensorNodeName(boost::optional<std::string> outdoorDryBulbTemperatureSensorNodeName);

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

    // bool setDesignSpecificationMultispeedHeatPumpObject(const boost::optional<UnitarySystemPerformace>& unitarySystemPerformace);

    // void resetDesignSpecificationMultispeedHeatPumpObject();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitarySystem");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP

