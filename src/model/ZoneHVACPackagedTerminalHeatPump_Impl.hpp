/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class HVACComponent;

  namespace detail {

    /** ZoneHVACPackagedTerminalHeatPump_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACPackagedTerminalHeatPump.*/
    class MODEL_API ZoneHVACPackagedTerminalHeatPump_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACPackagedTerminalHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACPackagedTerminalHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACPackagedTerminalHeatPump_Impl(const ZoneHVACPackagedTerminalHeatPump_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACPackagedTerminalHeatPump_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      std::string outdoorAirMixerObjectType() const;

      std::string outdoorAirMixerName() const;

      boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

      bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

      bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      bool noLoadSupplyAirFlowRateControlSetToLowSpeed() const;

      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

      bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

      bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      HVACComponent supplyAirFan() const;

      HVACComponent heatingCoil() const;

      double heatingConvergenceTolerance() const;

      bool isHeatingConvergenceToleranceDefaulted() const;

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

      boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType);

      bool setOutdoorAirMixerName(const std::string& outdoorAirMixerName);

      bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

      void autosizeSupplyAirFlowRateDuringCoolingOperation();

      bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

      void autosizeSupplyAirFlowRateDuringHeatingOperation();

      bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setNoLoadSupplyAirFlowRateControlSetToLowSpeed(bool noLoadSupplyAirFlowRateControlSetToLowSpeed);

      bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

      void autosizeOutdoorAirFlowRateDuringCoolingOperation();

      bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

      void autosizeOutdoorAirFlowRateDuringHeatingOperation();

      bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

      void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setSupplyAirFan(HVACComponent& hvacComponent);

      bool setHeatingCoil(HVACComponent& hvacComponent);

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      void resetHeatingConvergenceTolerance();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

      bool setCoolingCoil(HVACComponent& hvacComponent);

      bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

      void resetCoolingConvergenceTolerance();

      bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

      bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

      void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

      bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

      void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

      bool setFanPlacement(const std::string& fanPlacement);

      void resetFanPlacement();

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalHeatPump");

      // optional getters for children() in case need to remove() because constructor fails
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;
      boost::optional<HVACComponent> optionalSupplementalHeatingCoil() const;
      boost::optional<Schedule> optionalSupplyAirFanOperatingModeSchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanAsModelObject() const;
      boost::optional<ModelObject> heatingCoilAsModelObject() const;
      boost::optional<ModelObject> coolingCoilAsModelObject() const;
      boost::optional<ModelObject> supplementalHeatingCoilAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanOperatingModeScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP
