/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEVENTILATIONDESIGNFLOWRATE_IMPL_HPP
#define MODEL_ZONEVENTILATIONDESIGNFLOWRATE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneVentilationDesignFlowRate_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneVentilationDesignFlowRate.*/
    class MODEL_API ZoneVentilationDesignFlowRate_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneVentilationDesignFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneVentilationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneVentilationDesignFlowRate_Impl(const ZoneVentilationDesignFlowRate_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneVentilationDesignFlowRate_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule schedule() const;

      std::string designFlowRateCalculationMethod() const;

      double designFlowRate() const;

      double flowRateperZoneFloorArea() const;

      double flowRateperPerson() const;

      double airChangesperHour() const;

      std::string ventilationType() const;

      double fanPressureRise() const;

      double fanTotalEfficiency() const;

      double constantTermCoefficient() const;

      double temperatureTermCoefficient() const;

      double velocityTermCoefficient() const;

      double velocitySquaredTermCoefficient() const;

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

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setSchedule(Schedule& schedule);

      bool setDesignFlowRateCalculationMethod(const std::string& designFlowRateCalculationMethod);

      bool setDesignFlowRate(double designFlowRate);

      bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

      bool setFlowRateperPerson(double flowRateperPerson);

      void resetFlowRateperPerson();

      bool setAirChangesperHour(double airChangesperHour);

      bool setVentilationType(const std::string& ventilationType);

      bool setFanPressureRise(double fanPressureRise);

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      bool setConstantTermCoefficient(double constantTermCoefficient);

      bool setTemperatureTermCoefficient(double temperatureTermCoefficient);

      bool setVelocityTermCoefficient(double velocityTermCoefficient);

      bool setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

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

      unsigned inletPort() const override;

      unsigned outletPort() const override;

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneVentilationDesignFlowRate");

      boost::optional<Schedule> optionalSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEVENTILATIONDESIGNFLOWRATE_IMPL_HPP
