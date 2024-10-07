/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACLowTemperatureRadiantElectric_Impl is a ZoneHVAC_Impl that is the implementation class for ZoneHVACLowTemperatureRadiantElectric.*/
    class MODEL_API ZoneHVACLowTemperatureRadiantElectric_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */

      ZoneHVACLowTemperatureRadiantElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTemperatureRadiantElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTemperatureRadiantElectric_Impl(const ZoneHVACLowTemperatureRadiantElectric_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACLowTemperatureRadiantElectric_Impl() override = default;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      /** @name Virtual Methods */

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      /** @name Getters */

      Schedule availabilitySchedule() const;

      Schedule heatingSetpointTemperatureSchedule() const;

      boost::optional<std::string> radiantSurfaceType() const;

      std::vector<Surface> surfaces() const;

      boost::optional<double> maximumElectricalPowertoPanel() const;

      bool isMaximumElectricalPowertoPanelDefaulted() const;

      bool isMaximumElectricalPowertoPanelAutosized() const;

      std::string temperatureControlType() const;

      bool isTemperatureControlTypeDefaulted() const;

      std::string setpointControlType() const;

      bool isSetpointControlTypeDefaulted() const;

      double heatingThrottlingRange() const;

      bool isHeatingThrottlingRangeDefaulted() const;

      boost::optional<double> autosizedMaximumElectricalPowertoPanel() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      /** @name Setters */

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

      bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

      void resetRadiantSurfaceType();

      bool setMaximumElectricalPowertoPanel(boost::optional<double> maximumElectricalPowertoPanel);

      void resetMaximumElectricalPowertoPanel();

      void autosizeMaximumElectricalPowertoPanel();

      bool setTemperatureControlType(const std::string& temperatureControlType);

      void resetTemperatureControlType();

      bool setSetpointControlType(const std::string& setpointControlType);

      void resetSetpointControlType();

      bool setHeatingThrottlingRange(double heatingThrottlingRange);

      void resetHeatingThrottlingRange();

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACLowTemperatureRadiantElectric");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Schedule> optionalHeatingSetpointTemperatureSchedule() const;

      std::vector<std::string> radiantSurfaceTypeValues() const;
      std::vector<std::string> temperatureControlTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP
