/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
class UnitarySystemPerformanceMultispeed;

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

    virtual ModelObject clone(Model model) const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IdfObject> remove() override;

    virtual unsigned airInletPort() override;
   
    virtual unsigned airOutletPort() override;

    virtual unsigned waterInletPort() override;

    virtual unsigned waterOutletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
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

    boost::optional<UnitarySystemPerformanceMultispeed> designSpecificationMultispeedObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlType(std::string controlType);

    void resetControlType();

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

    bool setDesignSpecificationMultispeedObject(const boost::optional<UnitarySystemPerformanceMultispeed>& unitarySystemPerformace);

    void resetDesignSpecificationMultispeedObject();

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

