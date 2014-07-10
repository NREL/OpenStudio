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

#ifndef MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP
#define MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {
namespace model {

class Schedule;
class Node;

namespace detail {

  class SetpointManagerScheduledDualSetpoint_Impl;

} // detail

/** SetpointManagerScheduledDualSetpoint is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:Scheduled:DualSetpoint'. */
class MODEL_API SetpointManagerScheduledDualSetpoint : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerScheduledDualSetpoint(const Model& model);

  virtual ~SetpointManagerScheduledDualSetpoint() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  boost::optional<Schedule> highSetpointSchedule() const;

  boost::optional<Schedule> lowSetpointSchedule() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  bool setHighSetpointSchedule(Schedule& schedule);

  void resetHighSetpointSchedule();

  bool setLowSetpointSchedule(Schedule& schedule);

  void resetLowSetpointSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerScheduledDualSetpoint_Impl ImplType;

  explicit SetpointManagerScheduledDualSetpoint(std::shared_ptr<detail::SetpointManagerScheduledDualSetpoint_Impl> impl);

  friend class detail::SetpointManagerScheduledDualSetpoint_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerScheduledDualSetpoint");
};

/** \relates SetpointManagerScheduledDualSetpoint*/
typedef boost::optional<SetpointManagerScheduledDualSetpoint> OptionalSetpointManagerScheduledDualSetpoint;

/** \relates SetpointManagerScheduledDualSetpoint*/
typedef std::vector<SetpointManagerScheduledDualSetpoint> SetpointManagerScheduledDualSetpointVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_HPP

