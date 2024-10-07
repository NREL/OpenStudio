/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP
#define MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACFourPipeFanCoil_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACFourPipeFanCoil.*/
    class MODEL_API ZoneHVACFourPipeFanCoil_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACFourPipeFanCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACFourPipeFanCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACFourPipeFanCoil_Impl(const ZoneHVACFourPipeFanCoil_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACFourPipeFanCoil_Impl() override = default;

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

      std::string capacityControlMethod() const;

      boost::optional<double> maximumSupplyAirFlowRate() const;

      bool isMaximumSupplyAirFlowRateAutosized() const;

      double lowSpeedSupplyAirFlowRatio() const;

      bool isLowSpeedSupplyAirFlowRatioDefaulted() const;

      double mediumSpeedSupplyAirFlowRatio() const;

      bool isMediumSpeedSupplyAirFlowRatioDefaulted() const;

      boost::optional<double> maximumOutdoorAirFlowRate() const;

      bool isMaximumOutdoorAirFlowRateAutosized() const;

      boost::optional<Schedule> outdoorAirSchedule() const;

      std::string outdoorAirMixerObjectType() const;

      std::string outdoorAirMixerName() const;

      HVACComponent supplyAirFan() const;

      HVACComponent coolingCoil() const;

      boost::optional<double> maximumColdWaterFlowRate() const;

      bool isMaximumColdWaterFlowRateAutosized() const;

      double minimumColdWaterFlowRate() const;

      bool isMinimumColdWaterFlowRateDefaulted() const;

      double coolingConvergenceTolerance() const;

      bool isCoolingConvergenceToleranceDefaulted() const;

      HVACComponent heatingCoil() const;

      boost::optional<double> maximumHotWaterFlowRate() const;

      bool isMaximumHotWaterFlowRateAutosized() const;

      double minimumHotWaterFlowRate() const;

      bool isMinimumHotWaterFlowRateDefaulted() const;

      double heatingConvergenceTolerance() const;

      bool isHeatingConvergenceToleranceDefaulted() const;

      boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

      boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

      boost::optional<double> autosizedMaximumColdWaterFlowRate() const;

      boost::optional<double> autosizedMaximumHotWaterFlowRate() const;

      boost::optional<double> autosizedMinimumSupplyAirTemperatureinCoolingMode() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperatureinHeatingMode() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setCapacityControlMethod(const std::string& capacityControlMethod);

      bool setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

      void autosizeMaximumSupplyAirFlowRate();

      bool setLowSpeedSupplyAirFlowRatio(double lowSpeedSupplyAirFlowRatio);

      void resetLowSpeedSupplyAirFlowRatio();

      bool setMediumSpeedSupplyAirFlowRatio(double mediumSpeedSupplyAirFlowRatio);

      void resetMediumSpeedSupplyAirFlowRatio();

      bool setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

      void autosizeMaximumOutdoorAirFlowRate();

      bool setOutdoorAirSchedule(Schedule& schedule);

      void resetOutdoorAirSchedule();

      bool setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType);

      bool setOutdoorAirMixerName(const std::string& outdoorAirMixerName);

      bool setSupplyAirFan(HVACComponent& fan);

      bool setCoolingCoil(HVACComponent& coolingCoil);

      bool setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate);

      void autosizeMaximumColdWaterFlowRate();

      bool setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

      void resetMinimumColdWaterFlowRate();

      bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

      void resetCoolingConvergenceTolerance();

      bool setHeatingCoil(HVACComponent& heatingCoil);

      bool setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

      void autosizeMaximumHotWaterFlowRate();

      bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

      void resetMinimumHotWaterFlowRate();

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      void resetHeatingConvergenceTolerance();

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      void resetSupplyAirFanOperatingModeSchedule();

      boost::optional<double> minimumSupplyAirTemperatureInCoolingMode() const;

      bool isMinimumSupplyAirTemperatureInCoolingModeAutosized() const;

      void autosizeMinimumSupplyAirTemperatureInCoolingMode();

      bool setMinimumSupplyAirTemperatureInCoolingMode(double minimumSupplyAirTemperatureInCoolingMode);

      boost::optional<double> maximumSupplyAirTemperatureInHeatingMode() const;

      bool isMaximumSupplyAirTemperatureInHeatingModeAutosized() const;

      void autosizeMaximumSupplyAirTemperatureInHeatingMode();

      bool setMaximumSupplyAirTemperatureInHeatingMode(double maximumSupplyAirTemperatureInHeatingMode);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACFourPipeFanCoil");

      std::vector<std::string> capacityControlMethodValues() const;
      std::vector<std::string> outdoorAirMixerObjectTypeValues() const;
      // optional getters so can remove() if constructor fails
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> outdoorAirScheduleAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanAsModelObject() const;
      boost::optional<ModelObject> coolingCoilAsModelObject() const;
      boost::optional<ModelObject> heatingCoilAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setOutdoorAirScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP
