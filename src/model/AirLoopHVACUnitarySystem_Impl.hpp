/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;
  class HVACComponent;
  class Node;
  class UnitarySystemPerformanceMultispeed;

  namespace detail {

    /** AirLoopHVACUnitarySystem_Impl is a ZoneHVACComponent_Impl that is the implementation class for AirLoopHVACUnitarySystem.*/
    class MODEL_API AirLoopHVACUnitarySystem_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACUnitarySystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitarySystem_Impl(const AirLoopHVACUnitarySystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACUnitarySystem_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

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

      bool hasHeatingCoil() const;  // For speed
      boost::optional<HVACComponent> heatingCoil() const;

      double dXHeatingCoilSizingRatio() const;

      bool isDXHeatingCoilSizingRatioDefaulted() const;

      bool hasCoolingCoil() const;  // For speed
      boost::optional<HVACComponent> coolingCoil() const;

      bool useDOASDXCoolingCoil() const;

      bool isUseDOASDXCoolingCoilDefaulted() const;

      double dOASDXCoolingCoilLeavingMinimumAirTemperature() const;

      bool isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const;

      bool isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized() const;

      std::string latentLoadControl() const;

      bool isLatentLoadControlDefaulted() const;

      boost::optional<HVACComponent> supplementalHeatingCoil() const;

      std::string supplyAirFlowRateMethodDuringCoolingOperation() const;

      boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

      bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const;

      boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRate() const;

      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const;

      std::string supplyAirFlowRateMethodDuringHeatingOperation() const;

      boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

      bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRatePerFloorAreaduringHeatingOperation() const;

      boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRate() const;

      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const;

      std::string supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

      bool isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const;

      boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const;

      bool noLoadSupplyAirFlowRateControlSetToLowSpeed() const;

      boost::optional<double> maximumSupplyAirTemperature() const;

      bool isMaximumSupplyAirTemperatureDefaulted() const;

      bool isMaximumSupplyAirTemperatureAutosized() const;

      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

      boost::optional<std::string> outdoorDryBulbTemperatureSensorNodeName() const;

      double ancilliaryOnCycleElectricPower() const;

      bool isAncilliaryOnCycleElectricPowerDefaulted() const;

      double ancilliaryOffCycleElectricPower() const;

      bool isAncilliaryOffCycleElectricPowerDefaulted() const;

      // double designHeatRecoveryWaterFlowRate() const;

      // bool isDesignHeatRecoveryWaterFlowRateDefaulted() const;

      // double maximumTemperatureforHeatRecovery() const;

      // bool isMaximumTemperatureforHeatRecoveryDefaulted() const;

      boost::optional<UnitarySystemPerformanceMultispeed> designSpecificationMultispeedObject() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperature() const;

      boost::optional<double> autosizedDOASDXCoolingCoilLeavingMinimumAirTemperature() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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

      bool setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature);

      void resetDOASDXCoolingCoilLeavingMinimumAirTemperature();

      void autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature();

      bool setLatentLoadControl(const std::string& latentLoadControl);

      void resetLatentLoadControl();

      bool setSupplementalHeatingCoil(const HVACComponent& supplementalHeatingCoil);

      void resetSupplementalHeatingCoil();

      bool setSupplyAirFlowRateMethodDuringCoolingOperation(const std::string& supplyAirFlowRateMethodDuringCoolingOperation);

      bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

      void resetSupplyAirFlowRateDuringCoolingOperation();

      void autosizeSupplyAirFlowRateDuringCoolingOperation();

      bool setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(double supplyAirFlowRatePerFloorAreaDuringCoolingOperation);

      void resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();

      bool setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate);

      void resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();

      bool setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);

      void resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();

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

      bool setNoLoadSupplyAirFlowRateControlSetToLowSpeed(bool noLoadSupplyAirFlowRateControlSetToLowSpeed);

      bool setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

      void resetMaximumSupplyAirTemperature();

      void autosizeMaximumSupplyAirTemperature();

      bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

      void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

      bool setOutdoorDryBulbTemperatureSensorNodeName(const std::string& outdoorDryBulbTemperatureSensorNodeName);

      void resetOutdoorDryBulbTemperatureSensorNodeName();

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

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitarySystem");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYSYSTEM_IMPL_HPP
