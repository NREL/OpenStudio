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

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Connection;
class DesignSpecificationOutdoorAir;

namespace detail {

  /** AirTerminalDualDuctVAVOutdoorAir_Impl is a Mixer_Impl that is the implementation class for AirTerminalDualDuctVAVOutdoorAir.*/
  class MODEL_API AirTerminalDualDuctVAVOutdoorAir_Impl : public Mixer_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalDualDuctVAVOutdoorAir_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    AirTerminalDualDuctVAVOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    AirTerminalDualDuctVAVOutdoorAir_Impl(const AirTerminalDualDuctVAVOutdoorAir_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~AirTerminalDualDuctVAVOutdoorAir_Impl() {}

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
    boost::optional<Connection> outdoorAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> recirculatedAirInletNode() const;

    boost::optional<double> maximumTerminalAirFlowRate() const;

    bool isMaximumTerminalAirFlowRateAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
    DesignSpecificationOutdoorAir designSpecificationOutdoorAirObject() const;

    std::string perPersonVentilationRateMode() const;

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
    bool setOutdoorAirInletNode(const Connection& connection);

    void resetOutdoorAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setRecirculatedAirInletNode(const Connection& connection);

    void resetRecirculatedAirInletNode();

    bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

    void autosizeMaximumTerminalAirFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
    bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<DesignSpecificationOutdoorAir> optionalDesignSpecificationOutdoorAirObject() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_IMPL_HPP

