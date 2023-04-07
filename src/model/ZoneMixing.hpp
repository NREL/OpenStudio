/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~ZoneMixing() = default;
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

    OS_DEPRECATED boost::optional<double> flowRateperZoneFloorArea() const;
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
    OS_DEPRECATED boost::optional<Schedule> minimumZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the receiving zone or space dry-bulb temperature above which mixing is shutoff.
    boost::optional<Schedule> maximumReceivingTemperatureSchedule() const;
    OS_DEPRECATED boost::optional<Schedule> maximumZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the source zone or space dry-bulb temperature below which mixing is shutoff.
    boost::optional<Schedule> minimumSourceTemperatureSchedule() const;
    OS_DEPRECATED boost::optional<Schedule> minimumSourceZoneTemperatureSchedule() const;

    /// Returns the temperature schedule containing the source zone or space dry-bulb temperature above which mixing is shutoff.
    boost::optional<Schedule> maximumSourceTemperatureSchedule() const;
    OS_DEPRECATED boost::optional<Schedule> maximumSourceZoneTemperatureSchedule() const;

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
    OS_DEPRECATED bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

    bool setFlowRateperPerson(double flowRateperPerson);

    bool setAirChangesperHour(double airChangesperHour);

    /// Sets the ThermalZone which supplies air to this mixing object.
    bool setSourceZone(const ThermalZone& zone);
    OS_DEPRECATED void resetSourceZone();

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

    OS_DEPRECATED bool setMinimumZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED void resetMinimumZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the receiving zone dry-bulb temperature above which mixing is shutoff.
    bool setMaximumReceivingTemperatureSchedule(Schedule& schedule);
    void resetMaximumReceivingTemperatureSchedule();

    OS_DEPRECATED bool setMaximumZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED void resetMaximumZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the source zone dry-bulb temperature below which mixing is shutoff.
    bool setMinimumSourceTemperatureSchedule(Schedule& schedule);
    void resetMinimumSourceTemperatureSchedule();

    OS_DEPRECATED bool setMinimumSourceZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED void resetMinimumSourceZoneTemperatureSchedule();

    /// Sets the temperature schedule containing the source zone dry-bulb temperature above which mixing is shutoff.
    bool setMaximumSourceTemperatureSchedule(Schedule& schedule);
    void resetMaximumSourceTemperatureSchedule();

    OS_DEPRECATED bool setMaximumSourceZoneTemperatureSchedule(Schedule& schedule);
    OS_DEPRECATED void resetMaximumSourceZoneTemperatureSchedule();

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
