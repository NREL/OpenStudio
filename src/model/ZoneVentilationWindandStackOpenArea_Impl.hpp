/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_IMPL_HPP
#define MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneVentilationWindandStackOpenArea_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneVentilationWindandStackOpenArea.*/
    class MODEL_API ZoneVentilationWindandStackOpenArea_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneVentilationWindandStackOpenArea_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneVentilationWindandStackOpenArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneVentilationWindandStackOpenArea_Impl(const ZoneVentilationWindandStackOpenArea_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneVentilationWindandStackOpenArea_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual bool addToThermalZone(ThermalZone& thermalZone) override;

      virtual void removeFromThermalZone() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double openingArea() const;

      Schedule openingAreaFractionSchedule() const;

      boost::optional<double> openingEffectiveness() const;
      bool isOpeningEffectivenessAutocalculated() const;

      double effectiveAngle() const;

      double heightDifference() const;

      boost::optional<double> dischargeCoefficientforOpening() const;
      bool isDischargeCoefficientforOpeningAutocalculated() const;

      double minimumIndoorTemperature() const;

      boost::optional<Schedule> minimumIndoorTemperatureSchedule() const;

      double maximumIndoorTemperature() const;

      boost::optional<Schedule> maximumIndoorTemperatureSchedule() const;

      double deltaTemperature() const;

      boost::optional<Schedule> deltaTemperatureSchedule() const;

      double minimumOutdoorTemperature() const;

      boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

      double maximumOutdoorTemperature() const;

      boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

      double maximumWindSpeed() const;

      //@}
      /** @name Setters */
      //@{

      bool setOpeningArea(double openingArea);

      bool setOpeningAreaFractionSchedule(Schedule& schedule);

      bool setOpeningEffectiveness(double openingEffectiveness);
      void autocalculateOpeningEffectiveness();

      bool setEffectiveAngle(double effectiveAngle);

      bool setHeightDifference(double heightDifference);

      bool setDischargeCoefficientforOpening(double dischargeCoefficientforOpening);
      void autocalculateDischargeCoefficientforOpening();

      bool setMinimumIndoorTemperature(double minimumIndoorTemperature);

      bool setMinimumIndoorTemperatureSchedule(Schedule& schedule);
      void resetMinimumIndoorTemperatureSchedule();

      bool setMaximumIndoorTemperature(double maximumIndoorTemperature);

      bool setMaximumIndoorTemperatureSchedule(Schedule& schedule);
      void resetMaximumIndoorTemperatureSchedule();

      bool setDeltaTemperature(double deltaTemperature);

      bool setDeltaTemperatureSchedule(Schedule& schedule);
      void resetDeltaTemperatureSchedule();

      bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

      bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);
      void resetMinimumOutdoorTemperatureSchedule();

      bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

      bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);
      void resetMaximumOutdoorTemperatureSchedule();

      bool setMaximumWindSpeed(double maximumWindSpeed);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneVentilationWindandStackOpenArea");

      boost::optional<Schedule> optionalOpeningAreaFractionSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEVENTILATIONWINDANDSTACKOPENAREA_IMPL_HPP
