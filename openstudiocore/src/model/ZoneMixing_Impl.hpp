/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_ZONEMIXING_IMPL_HPP
#define MODEL_ZONEMIXING_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Zone;
class Schedule;
class Zone;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;
class Schedule;

namespace detail {

  /** ZoneMixing_Impl is a ModelObject_Impl that is the implementation class for ZoneMixing.*/
  class MODEL_API ZoneMixing_Impl : public ModelObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneMixing_Impl(const IdfObject& idfObject,
                    Model_Impl* model,
                    bool keepHandle);

    ZoneMixing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    ZoneMixing_Impl(const ZoneMixing_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    virtual ~ZoneMixing_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Zone.
    Zone zone() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule schedule() const;

    std::string designFlowRateCalculationMethod() const;

    bool isDesignFlowRateCalculationMethodDefaulted() const;

    boost::optional<double> designFlowRate() const;

    boost::optional<double> flowRateperZoneFloorArea() const;

    boost::optional<double> flowRateperPerson() const;

    boost::optional<double> airChangesperHour() const;

    // TODO: Check return type. From object lists, some candidates are: Zone.
    boost::optional<Zone> sourceZone() const;

    double deltaTemperature() const;

    bool isDeltaTemperatureDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> deltaTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> minimumZoneTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> maximumZoneTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> minimumSourceZoneTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> maximumSourceZoneTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Zone.
    bool setZone(const Zone& zone);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setSchedule(Schedule& schedule);

    bool setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod);

    void resetDesignFlowRateCalculationMethod();

    bool setDesignFlowRate(boost::optional<double> designFlowRate);

    void resetDesignFlowRate();

    bool setFlowRateperZoneFloorArea(boost::optional<double> flowRateperZoneFloorArea);

    void resetFlowRateperZoneFloorArea();

    bool setFlowRateperPerson(boost::optional<double> flowRateperPerson);

    void resetFlowRateperPerson();

    bool setAirChangesperHour(boost::optional<double> airChangesperHour);

    void resetAirChangesperHour();

    // TODO: Check argument type. From object lists, some candidates are: Zone.
    bool setSourceZone(const boost::optional<Zone>& zone);

    void resetSourceZone();

    void setDeltaTemperature(double deltaTemperature);

    void resetDeltaTemperature();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setDeltaTemperatureSchedule(Schedule& schedule);

    void resetDeltaTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMinimumZoneTemperatureSchedule(Schedule& schedule);

    void resetMinimumZoneTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMaximumZoneTemperatureSchedule(Schedule& schedule);

    void resetMaximumZoneTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMinimumSourceZoneTemperatureSchedule(Schedule& schedule);

    void resetMinimumSourceZoneTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMaximumSourceZoneTemperatureSchedule(Schedule& schedule);

    void resetMaximumSourceZoneTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

    void resetMinimumOutdoorTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

    void resetMaximumOutdoorTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneMixing");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Zone> optionalZone() const;
    boost::optional<Schedule> optionalSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEMIXING_IMPL_HPP

