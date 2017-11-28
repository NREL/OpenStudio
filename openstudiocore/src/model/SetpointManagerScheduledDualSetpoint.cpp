/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SetpointManagerScheduledDualSetpoint.hpp"
#include "SetpointManagerScheduledDualSetpoint_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_Scheduled_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

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

  bool SetpointManagerScheduledDualSetpoint_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      return this->setSetpointNode(node);
    }
    return added;
  }

  std::string SetpointManagerScheduledDualSetpoint_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::highSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName);
  }

  boost::optional<Schedule> SetpointManagerScheduledDualSetpoint_Impl::lowSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName);
  }

  boost::optional<Node> SetpointManagerScheduledDualSetpoint_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerScheduledDualSetpoint_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerScheduledDualSetpoint_Impl::setHighSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,
                              "SetpointManagerScheduledDualSetpoint",
                              "High Setpoint",
                              schedule);
    return result;
  }

  void SetpointManagerScheduledDualSetpoint_Impl::resetHighSetpointSchedule()
  {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerScheduledDualSetpoint_Impl::setLowSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,
                              "SetpointManagerScheduledDualSetpoint",
                              "Low Setpoint",
                              schedule);
    return result;
  }

  void SetpointManagerScheduledDualSetpoint_Impl::resetLowSetpointSchedule()
  {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerScheduledDualSetpoint_Impl::setSetpointNode( const Node & node )
  {
    return setPointer(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,node.handle());
  }

  void SetpointManagerScheduledDualSetpoint_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

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

boost::optional<Schedule> SetpointManagerScheduledDualSetpoint::highSetpointSchedule() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->highSetpointSchedule();
}

boost::optional<Schedule> SetpointManagerScheduledDualSetpoint::lowSetpointSchedule() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->lowSetpointSchedule();
}

boost::optional<Node> SetpointManagerScheduledDualSetpoint::setpointNode() const {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setpointNode();
}

bool SetpointManagerScheduledDualSetpoint::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerScheduledDualSetpoint::setHighSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setHighSetpointSchedule(schedule);
}

void SetpointManagerScheduledDualSetpoint::resetHighSetpointSchedule() {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->resetHighSetpointSchedule();
}

bool SetpointManagerScheduledDualSetpoint::setLowSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->setLowSetpointSchedule(schedule);
}

void SetpointManagerScheduledDualSetpoint::resetLowSetpointSchedule() {
  return getImpl<detail::SetpointManagerScheduledDualSetpoint_Impl>()->resetLowSetpointSchedule();
}

/// @cond
SetpointManagerScheduledDualSetpoint::SetpointManagerScheduledDualSetpoint(std::shared_ptr<detail::SetpointManagerScheduledDualSetpoint_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

