/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_FANZONEEXHAUST_IMPL_HPP
#define MODEL_FANZONEEXHAUST_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Schedule;
class Schedule;
class Schedule;

namespace detail {

  /** FanZoneExhaust_Impl is a ZoneHVACComponent_Impl that is the implementation class for FanZoneExhaust.*/
  class MODEL_API FanZoneExhaust_Impl : public ZoneHVACComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    FanZoneExhaust_Impl(const IdfObject& idfObject,
                        Model_Impl* model,
                        bool keepHandle);

    FanZoneExhaust_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    FanZoneExhaust_Impl(const FanZoneExhaust_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~FanZoneExhaust_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    double fanEfficiency() const;

    double pressureRise() const;

    boost::optional<double> maximumFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> airInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> airOutletNode() const;

    std::string endUseSubcategory() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> flowFractionSchedule() const;

    std::string systemAvailabilityManagerCouplingMode() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> balancedExhaustFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setFanEfficiency(double fanEfficiency);

    void setPressureRise(double pressureRise);

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    void resetMaximumFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirInletNode(const boost::optional<Connection>& connection);

    void resetAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const boost::optional<Connection>& connection);

    void resetAirOutletNode();

    void setEndUseSubcategory(std::string endUseSubcategory);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setFlowFractionSchedule(Schedule& schedule);

    void resetFlowFractionSchedule();

    bool setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

    void resetMinimumZoneTemperatureLimitSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBalancedExhaustFractionSchedule(Schedule& schedule);

    void resetBalancedExhaustFractionSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FANZONEEXHAUST_IMPL_HPP

