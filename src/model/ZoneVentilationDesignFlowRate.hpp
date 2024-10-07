/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP
#define MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class ZoneVentilationDesignFlowRate_Impl;

  }  // namespace detail

  /** ZoneVentilationDesignFlowRate is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneVentilation:DesignFlowRate'. */
  class MODEL_API ZoneVentilationDesignFlowRate : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneVentilationDesignFlowRate(const Model& model);

    virtual ~ZoneVentilationDesignFlowRate() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneVentilationDesignFlowRate(const ZoneVentilationDesignFlowRate& other) = default;
    ZoneVentilationDesignFlowRate(ZoneVentilationDesignFlowRate&& other) = default;
    ZoneVentilationDesignFlowRate& operator=(const ZoneVentilationDesignFlowRate&) = default;
    ZoneVentilationDesignFlowRate& operator=(ZoneVentilationDesignFlowRate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> designFlowRateCalculationMethodValues();

    static std::vector<std::string> ventilationTypeValues();

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

    //@}
    /** @name Setters */
    //@{

    bool setSchedule(Schedule& schedule);

    bool setDesignFlowRate(double designFlowRate);

    bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

    bool setFlowRateperPerson(double flowRateperPerson);

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

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneVentilationDesignFlowRate_Impl;

    explicit ZoneVentilationDesignFlowRate(std::shared_ptr<detail::ZoneVentilationDesignFlowRate_Impl> impl);

    friend class detail::ZoneVentilationDesignFlowRate_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneVentilationDesignFlowRate");
  };

  /** \relates ZoneVentilationDesignFlowRate*/
  using OptionalZoneVentilationDesignFlowRate = boost::optional<ZoneVentilationDesignFlowRate>;

  /** \relates ZoneVentilationDesignFlowRate*/
  using ZoneVentilationDesignFlowRateVector = std::vector<ZoneVentilationDesignFlowRate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP
