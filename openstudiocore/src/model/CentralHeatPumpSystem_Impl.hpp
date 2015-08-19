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

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Connection;
class Connection;
class Connection;
class Connection;
class Schedule;

namespace detail {

  /** CentralHeatPumpSystem_Impl is a WaterToWaterComponent_Impl that is the implementation class for CentralHeatPumpSystem.*/
  class MODEL_API CentralHeatPumpSystem_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CentralHeatPumpSystem_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~CentralHeatPumpSystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    std::string controlMethod() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection coolingLoopInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection coolingLoopOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection sourceLoopInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection sourceLoopOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection heatingLoopInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection heatingLoopOutletNode() const;

    double ancillaryPower() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> ancillaryOperationSchedule() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    bool setControlMethod(std::string controlMethod);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCoolingLoopInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCoolingLoopOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setSourceLoopInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setSourceLoopOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatingLoopInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatingLoopOutletNode(const Connection& connection);

    bool setAncillaryPower(double ancillaryPower);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAncillaryOperationSchedule(Schedule& schedule);

    void resetAncillaryOperationSchedule();

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalCoolingLoopInletNode() const;
    boost::optional<Connection> optionalCoolingLoopOutletNode() const;
    boost::optional<Connection> optionalSourceLoopInletNode() const;
    boost::optional<Connection> optionalSourceLoopOutletNode() const;
    boost::optional<Connection> optionalHeatingLoopInletNode() const;
    boost::optional<Connection> optionalHeatingLoopOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CENTRALHEATPUMPSYSTEM_IMPL_HPP

