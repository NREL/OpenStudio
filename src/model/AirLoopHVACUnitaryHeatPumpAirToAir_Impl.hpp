/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class HVACComponent;

  class ThermalZone;

  class Node;

  namespace detail {

    /** AirLoopHVACUnitaryHeatPumpAirToAir_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatPumpAirToAir.*/
    class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAir_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const AirLoopHVACUnitaryHeatPumpAirToAir_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACUnitaryHeatPumpAirToAir_Impl() override = default;

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

      virtual bool addToNode(Node& node) override;

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

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      std::string dehumidificationControlType() const;

      bool isDehumidificationControlTypeDefaulted() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

      void autosizeSupplyAirFlowRateDuringCoolingOperation();

      bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> n2);

      void autosizeSupplyAirFlowRateDuringHeatingOperation();

      bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setControllingZone(ThermalZone& zone);

      void resetControllingZone();

      bool setSupplyAirFan(HVACComponent& hvacComponent);

      bool setHeatingCoil(HVACComponent& hvacComponent);

      bool setCoolingCoil(HVACComponent& hvacComponent);

      bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

      bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

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

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAir");

      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;
      boost::optional<HVACComponent> optionalSupplementalHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP
