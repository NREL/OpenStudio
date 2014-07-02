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

#include <model/SetpointManagerScheduledDualSetpoint.hpp>
#include <model/SetpointManagerScheduledDualSetpoint_Impl.hpp>

#include <model/Schedule.hpp>
// #include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
// #include <model/Node_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_Scheduled_DualSetpoint_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerScheduledDualSetpoint_Impl::SetpointManagerScheduledDualSetpoint_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerScheduledDualSetpoint::iddObjectType());
  }

  SetpointManagerScheduledDualSetpoint_Impl::SetpointManagerScheduledDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerScheduledDualSetpoint::iddObjectType());
  }

  SetpointManagerScheduledDualSetpoint_Impl::SetpointManagerScheduledDualSetpoint_Impl(const SetpointManagerScheduledDualSetpoint_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerScheduledDualSetpoint_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerScheduledDualSetpoint_Impl::iddObjectType() const {
    return SetpointManagerScheduledDualSetpoint::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SetpointManagerScheduledDualSetpoint_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SetpointManagerScheduledDualSetpoint","High Setpoint"));
    }
    if (std::find(b,e,OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SetpointManagerScheduledDualSetpoint","Low Setpoint"));
    }
    return result;
  }

  std::string SetpointManagerScheduledDualSetpoint_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  // bool SetpointManagerScheduledDualSetpoint_Impl::isControlVariableDefaulted() const {
  //   return isEmpty(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable);
  // }

  boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::highSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName);
  }

  boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::lowSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName);
  }

  // Schedule SetpointManagerScheduledDualSetpoint_Impl::highSetpointSchedule() const {
  //   boost::optional<Schedule> value = optionalHighSetpointSchedule();
  //   if (!value) {
  //     LOG_AND_THROW(briefDescription() << " does not have an High Setpoint Schedule attached.");
  //   }
  //   return value.get();
  // }

  // Schedule SetpointManagerScheduledDualSetpoint_Impl::lowSetpointSchedule() const {
  //   boost::optional<Schedule> value = optionalLowSetpointSchedule();
  //   if (!value) {
  //     LOG_AND_THROW(briefDescription() << " does not have an Low Setpoint Schedule attached.");
  //   }
  //   return value.get();
  // }

  boost::optional<Node> SetpointManagerScheduledDualSetpoint_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName);
  }

  // Node SetpointManagerScheduledDualSetpoint_Impl::setpointNodeorNodeList() const {
  //   boost::optional<Node> value = optionalSetpointNodeorNodeList();
  //   if (!value) {
  //     LOG_AND_THROW(briefDescription() << " does not have an Setpoint Nodeor Node List attached.");
  //   }
  //   return value.get();
  // }

  bool SetpointManagerScheduledDualSetpoint_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable, controlVariable);
    return result;
  }

  // void SetpointManagerScheduledDualSetpoint_Impl::resetControlVariable() {
  //   bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable, "");
  //   OS_ASSERT(result);
  // }

  bool SetpointManagerScheduledDualSetpoint_Impl::setHighSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,
                              "SetpointManagerScheduledDualSetpoint",
                              "High Setpoint",
                              schedule);
    return result;
  }

  // void SetpointManagerScheduledDualSetpoint_Impl::resetHighSetpointSchedule()
  // {
  //   bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,"");
  //   OS_ASSERT(result);
  // }

  bool SetpointManagerScheduledDualSetpoint_Impl::setLowSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,
                              "SetpointManagerScheduledDualSetpoint",
                              "Low Setpoint",
                              schedule);
    return result;
  }

  // void SetpointManagerScheduledDualSetpoint_Impl::resetLowSetpointSchedule()
  // {
  //   bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,"");
  //   OS_ASSERT(result);
  // }

  bool SetpointManagerScheduledDualSetpoint_Impl::setSetpointNode( const Node & node )
  {
    return setPointer(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,node.handle());
  }

  void SetpointManagerScheduledDualSetpoint_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  // bool SetpointManagerScheduledDualSetpoint_Impl::setSetpointNodeorNodeList(const Node& node) {
  //   bool result = setPointer(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName, node.handle());
  //   return result;
  // }

  // boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::optionalHighSetpointSchedule() const {
  //   return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName);
  // }

  // boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::optionalLowSetpointSchedule() const {
  //   return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName);
  // }

  // boost::optional<Node> SetpointManagerScheduledDualSetpoint_Impl::optionalSetpointNodeorNodeList() const {
  //   return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName);
  // }

} // detail

SetpointManagerScheduledDualSetpoint::SetpointManagerScheduledDualSetpoint(const Model& model)
  : SetpointManager(SetpointManagerScheduledDualSetpoint::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>());
}

IddObjectType SetpointManagerScheduledDualSetpoint::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_Scheduled_DualSetpoint);
}

std::vector<std::string> SetpointManagerScheduledDualSetpoint::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable);
}

std::string SetpointManagerScheduledDualSetpoint::controlVariable() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->controlVariable();
}

// bool SetpointManagerScheduledDualSetpoint::isControlVariableDefaulted() const {
//   return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->isControlVariableDefaulted();
// }

boost::optional<Schedule> SetpointManagerScheduledDualSetpoint::highSetpointSchedule() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->highSetpointSchedule();
}

boost::optional<Schedule> SetpointManagerScheduledDualSetpoint::lowSetpointSchedule() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->lowSetpointSchedule();
}

boost::optional<Node> SetpointManagerScheduledDualSetpoint::setpointNode() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setpointNode();
}

// Node SetpointManagerScheduledDualSetpoint::setpointNodeorNodeList() const {
//   return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setpointNodeorNodeList();
// }

bool SetpointManagerScheduledDualSetpoint::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setControlVariable(controlVariable);
}

// void SetpointManagerScheduledDualSetpoint::resetControlVariable() {
//   getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->resetControlVariable();
// }

bool SetpointManagerScheduledDualSetpoint::setHighSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setHighSetpointSchedule(schedule);
}

// void SetpointManagerScheduledDualSetpoint::resetHighSetpointSchedule() {
//   return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->resetHighSetpointSchedule();
// }

bool SetpointManagerScheduledDualSetpoint::setLowSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setLowSetpointSchedule(schedule);
}

// void SetpointManagerScheduledDualSetpoint::resetLowSetpointSchedule() {
//   return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->resetLowSetpointSchedule();
// }

// bool SetpointManagerScheduledDualSetpoint::setSetpointNodeorNodeList(const Node& node) {
//   return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setSetpointNodeorNodeList(node);
// }

/// @cond
SetpointManagerScheduledDualSetpoint::SetpointManagerScheduledDualSetpoint(boost::shared_ptr<detail::SetpointManagerScheduledDualSetpoint_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

