/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACWATERTOAIRHEATPUMP_IMPL_HPP
#define MODEL_ZONEHVACWATERTOAIRHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  class HVACComponent;

  namespace detail {

    /** ZoneHVACWaterToAirHeatPump_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACWaterToAirHeatPump.*/
    class MODEL_API ZoneHVACWaterToAirHeatPump_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACWaterToAirHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACWaterToAirHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACWaterToAirHeatPump_Impl(const ZoneHVACWaterToAirHeatPump_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACWaterToAirHeatPump_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

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

      HVACComponent coolingCoil() const;

      HVACComponent supplementalHeatingCoil() const;

      boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

      bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

      std::string outdoorDryBulbTemperatureSensorNodeName() const;

      std::string fanPlacement() const;

      bool isFanPlacementDefaulted() const;

      std::string heatPumpCoilWaterFlowMode() const;
      bool isHeatPumpCoilWaterFlowModeDefaulted() const;

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

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

      bool setHeatingCoil(HVACComponent& heatingCoilsWaterToAirHP);

      bool setCoolingCoil(HVACComponent& coolingCoilsWaterToAirHP);

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

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACWaterToAirHeatPump");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;
      boost::optional<HVACComponent> optionalSupplementalHeatingCoil() const;

      std::vector<std::string> fanPlacementValues() const;
      std::vector<std::string> heatPumpCoilWaterFlowModeValues() const;

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

#endif  // MODEL_ZONEHVACWATERTOAIRHEATPUMP_IMPL_HPP
