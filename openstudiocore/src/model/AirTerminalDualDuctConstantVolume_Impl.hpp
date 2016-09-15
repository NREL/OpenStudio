/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Connection;

namespace detail {

  /** AirTerminalDualDuctConstantVolume_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctConstantVolume.*/
  class MODEL_API AirTerminalDualDuctConstantVolume_Impl : public Mixer_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalDualDuctConstantVolume_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    AirTerminalDualDuctConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    AirTerminalDualDuctConstantVolume_Impl(const AirTerminalDualDuctConstantVolume_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~AirTerminalDualDuctConstantVolume_Impl() {}

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

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> airOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> hotAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> coldAirInletNode() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const Connection& connection);

    void resetAirOutletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHotAirInletNode(const Connection& connection);

    void resetHotAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setColdAirInletNode(const Connection& connection);

    void resetColdAirInletNode();

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctConstantVolume");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_IMPL_HPP

