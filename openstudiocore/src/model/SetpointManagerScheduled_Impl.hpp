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

#ifndef MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP
#define MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

class MODEL_API SetpointManagerScheduled_Impl : public SetpointManager_Impl {
  Q_OBJECT;

  Q_PROPERTY(std::string controlVariable READ controlVariable WRITE setControlVariable);
  Q_PROPERTY(std::vector<std::string> controlVariableValues READ controlVariableValues);

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject);
 public:
  /** @name Constructors and Destructors */
  //@{

  SetpointManagerScheduled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  SetpointManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                       Model_Impl* model, 
                                       bool keepHandle);

  SetpointManagerScheduled_Impl(const SetpointManagerScheduled_Impl& other, 
                                       Model_Impl* model,
                                       bool keepHandles);

  virtual ~SetpointManagerScheduled_Impl();

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

  virtual boost::optional<Node> setpointNode() const;

  virtual std::string controlVariable() const;

  Schedule schedule() const;

  bool hasSchedule() const;

  //@}
  /** @name Setters */
  //@{

  virtual bool setControlVariable(const std::string& controlVariable);

  bool setSchedule(Schedule& schedule);

  //@}
  /** @name Other */
  //@{

  /** Sets control variable and schedule simultaneously. Use in the case that the
   *  control variable change implies a change in schedule units. */
  bool setControlVariableAndSchedule(const std::string& controlVariable,
                                     Schedule& schedule);

  //@}
 private:

  virtual bool setSetpointNode( const Node & node );

  virtual void resetSetpointNode();

  // converts control variable to the schedule's display name
  std::string scheduleDisplayName() const;

  // as above, but used to check candidate control variables
  std::string scheduleDisplayName(const std::string& candidateControlVariable) const;

  std::vector<std::string> controlVariableValues() const;

  boost::optional<ModelObject> scheduleAsModelObject() const;

  bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

};

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP

