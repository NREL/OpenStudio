/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEMIXING_HPP
#define MODEL_ZONEMIXING_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Space;
  class ThermalZone;

  namespace detail {

    class ZoneMixing_Impl;

  }  // namespace detail

  /** ZoneMixing is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneMixing'. */
  class MODEL_API ZoneMixing : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneMixing(const ThermalZone& thermalZone);
    explicit ZoneMixing(const Space& space);

    virtual ~ZoneMixing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneMixing(const ZoneMixing& other) = default;
    ZoneMixing(ZoneMixing&& other) = default;
    ZoneMixing& operator=(const ZoneMixing&) = default;
    ZoneMixing& operator=(ZoneMixing&&) = default;

    //@}

    static IddObjectType iddObjectType();

    bool hardSize();

    /** @name Getters */
    //@{

    /// Returns the ThermalZone which receives air from this mixing object.
    boost::optional<ThermalZone> zone() const;
    boost::optional<Space> space() const;
    ModelObject zoneOrSpace() const;

    /// Returns fractional schedule which moderates amount of air flow from the design level.
    Schedule schedule() const;

    std::string designFlowRateCalculationMethod() const;

    boost::optional<double> designFlowRate() const;

    OS_DEPRECATED(3, 5, 0) boost::optional<double> flowRateperZoneFloorArea() const;
    boost::optional<double> flowRateperFloorArea() const;

    boost::optional<double> flowRateperPerson() const;

    boost::optional<double> airChangesperHour() const;

    /// Returns the ThermalZone which supplies air to this mixing object.
    boost::optional<ThermalZone> sourceZone() const;
    boost::optional<Space> sourceSpace() const;
    boost::optional<ModelObject> sourceZoneOrSpace() const;

    /// Returns the constant temperature differential between source and receiving zones below which mixing is shutoff.
    // DLM: is this a signed difference or absolute?
    boost::optional<double> deltaTemperature() const;

    /// Returns the temperature schedule containing the differential between source and receiving zones versus time below which mixing is shutoff.
    boost::optional<Schedule> deltaTemperatureSchedule() const;

    /// Returns the temperature schedule containing the receiving zone or space dry-bulb temperature below which mixing is shutoff.
    boost::optional<Schedule> minimumReceivingTemperatureSchedule() const;
    OS_DEPRECATED(3, 5, 0) boost::optional<Schedule> minimumZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the receiving zone or space dry-bulb temperature above which mixing is shutoff.
    boost::optional<Schedule> maximumReceivingTemperatureSchedule() const;
    OS_DEPRECATED(3, 5, 0) boost::optional<Schedule> maximumZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the source zone or space dry-bulb temperature below which mixing is shutoff.
    boost::optional<Schedule> minimumSourceTemperatureSchedule() const;
    OS_DEPRECATED(3, 5, 0) boost::optional<Schedule> minimumSourceZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the source zone or space dry-bulb temperature above which mixing is shutoff.
    boost::optional<Schedule> maximumSourceTemperatureSchedule() const;
    OS_DEPRECATED(3, 5, 0) boost::optional<Schedule> maximumSourceZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the outdoor temperature below which mixing is shutoff.
    boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

    /// Returns the temperature schedule containing the outdoor temperature above which mixing is shutoff.
    boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the fractional schedule which moderates amount of air flow from the design level.
    bool setSchedule(Schedule& schedule);

    bool setDesignFlowRate(double designFlowRate);

    bool setFlowRateperFloorArea(double flowRateperFloorArea);
    OS_DEPRECATED(3, 5, 0) bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

    bool setFlowRateperPerson(double flowRateperPerson);

    bool setAirChangesperHour(double airChangesperHour);

    /// Sets the ThermalZone which supplies air to this mixing object.
    bool setSourceZone(const ThermalZone& zone);
    OS_DEPRECATED(3, 5, 0) void resetSourceZone();

    bool setSourceSpace(const Space& space);
    void resetSourceZoneOrSpace();

    /// Sets the constant temperature differential between source and receiving zones below which mixing is shutoff.
    bool setDeltaTemperature(double deltaTemperature);

    void resetDeltaTemperature();

    /// Sets the temperature schedule containing the differential between source and receiving zones versus time below which mixing is shutoff.
    bool setDeltaTemperatureSchedule(Schedule& schedule);

    void resetDeltaTemperatureSchedule();

    /// Sets the temperature schedule containing the receiving zone dry-bulb temperature below which mixing is shutoff.
    bool setMinimumReceivingTemperatureSchedule(Schedule& schedule);
    void resetMinimumReceivingTemperatureSchedule();

    OS_DEPRECATED(3, 5, 0) bool setMinimumZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED(3, 5, 0) void resetMinimumZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the receiving zone dry-bulb temperature above which mixing is shutoff.
    bool setMaximumReceivingTemperatureSchedule(Schedule& schedule);
    void resetMaximumReceivingTemperatureSchedule();

    OS_DEPRECATED(3, 5, 0) bool setMaximumZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED(3, 5, 0) void resetMaximumZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the source zone dry-bulb temperature below which mixing is shutoff.
    bool setMinimumSourceTemperatureSchedule(Schedule& schedule);
    void resetMinimumSourceTemperatureSchedule();

    OS_DEPRECATED(3, 5, 0) bool setMinimumSourceZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED(3, 5, 0) void resetMinimumSourceZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the source zone dry-bulb temperature above which mixing is shutoff.
    bool setMaximumSourceTemperatureSchedule(Schedule& schedule);
    void resetMaximumSourceTemperatureSchedule();

    OS_DEPRECATED(3, 5, 0) bool setMaximumSourceZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED(3, 5, 0) void resetMaximumSourceZoneTemperatureSchedule();

    /// Returns the temperature schedule containing the outdoor temperature below which mixing is shutoff.
    bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

    void resetMinimumOutdoorTemperatureSchedule();

    /// Returns the temperature schedule containing the outdoor temperature above which mixing is shutoff.
    bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

    void resetMaximumOutdoorTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}

   protected:
    /// @cond
    using ImplType = detail::ZoneMixing_Impl;

    explicit ZoneMixing(std::shared_ptr<detail::ZoneMixing_Impl> impl);

    friend class detail::ZoneMixing_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneMixing");
  };

  /** \relates ZoneMixing*/
  using OptionalZoneMixing = boost::optional<ZoneMixing>;

  /** \relates ZoneMixing*/
  using ZoneMixingVector = std::vector<ZoneMixing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEMIXING_HPP
