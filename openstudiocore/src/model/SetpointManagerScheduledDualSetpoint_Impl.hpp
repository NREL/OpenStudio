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

#ifndef MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP
#define MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager_Impl.hpp>

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

  /** SetpointManagerScheduledDualSetpoint_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerScheduledDualSetpoint.*/
  class MODEL_API SetpointManagerScheduledDualSetpoint_Impl : public SetpointManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SetpointManagerScheduledDualSetpoint_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    SetpointManagerScheduledDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    SetpointManagerScheduledDualSetpoint_Impl(const SetpointManagerScheduledDualSetpoint_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~SetpointManagerScheduledDualSetpoint_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters */
    //@{

    virtual std::string controlVariable() const;

    boost::optional<Schedule> highSetpointSchedule() const;

    boost::optional<Schedule> lowSetpointSchedule() const;

    virtual boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setControlVariable(const std::string& controlVariable);

    bool setHighSetpointSchedule(Schedule& schedule);

    void resetHighSetpointSchedule();

    bool setLowSetpointSchedule(Schedule& schedule);

    void resetLowSetpointSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    virtual bool setSetpointNode( const Node & node );

    virtual void resetSetpointNode();

    REGISTER_LOGGER("openstudio.model.SetpointManagerScheduledDualSetpoint");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP

