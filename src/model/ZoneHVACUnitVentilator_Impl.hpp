/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP
#define MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACUnitVentilator_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACUnitVentilator.*/
    class MODEL_API ZoneHVACUnitVentilator_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACUnitVentilator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitVentilator_Impl(const ZoneHVACUnitVentilator_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACUnitVentilator_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

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

      boost::optional<double> maximumSupplyAirFlowRate() const;

      bool isMaximumSupplyAirFlowRateAutosized() const;

      std::string outdoorAirControlType() const;

      boost::optional<double> minimumOutdoorAirFlowRate() const;

      bool isMinimumOutdoorAirFlowRateAutosized() const;

      Schedule minimumOutdoorAirSchedule() const;

      boost::optional<double> maximumOutdoorAirFlowRate() const;

      bool isMaximumOutdoorAirFlowRateAutosized() const;

      Schedule maximumOutdoorAirFractionorTemperatureSchedule() const;

      HVACComponent supplyAirFan() const;

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      boost::optional<HVACComponent> heatingCoil() const;

      double heatingConvergenceTolerance() const;

      boost::optional<HVACComponent> coolingCoil() const;

      double coolingConvergenceTolerance() const;

      // boost::optional<ModelObject> availabilityManagerList() const;

      // boost::optional<ModelObject> designSpecificationZoneHVACSizingObject() const;

      boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

      boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;

      boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

      void autosizeMaximumSupplyAirFlowRate();

      bool setOutdoorAirControlType(const std::string& outdoorAirControlType);

      bool setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate);

      void autosizeMinimumOutdoorAirFlowRate();

      bool setMinimumOutdoorAirSchedule(Schedule& schedule);

      bool setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

      void autosizeMaximumOutdoorAirFlowRate();

      bool setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule);

      bool setSupplyAirFan(const HVACComponent& supplyAirFan);

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      void resetSupplyAirFanOperatingModeSchedule();

      bool setHeatingCoil(const boost::optional<HVACComponent>& heatingCoil);

      void resetHeatingCoil();

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      bool setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil);

      void resetCoolingCoil();

      bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

      // bool setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists);

      // void resetAvailabilityManagerList();

      // bool setDesignSpecificationZoneHVACSizingObject(const boost::optional<ModelObject>& designSpecificationZoneHVACSizingName);

      // void resetDesignSpecificationZoneHVACSizingObject();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACUnitVentilator");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Schedule> optionalMinimumOutdoorAirSchedule() const;
      boost::optional<Schedule> optionalMaximumOutdoorAirFractionorTemperatureSchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP
