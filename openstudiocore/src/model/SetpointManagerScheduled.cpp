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

#include "SetpointManagerScheduled.hpp"
#include "SetpointManagerScheduled_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "PlantLoop.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include "../utilities/idd/IddKey.hpp"
#include <utilities/idd/OS_SetpointManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerScheduled::iddObjectType());
  }

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerScheduled::iddObjectType());
  }

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const SetpointManagerScheduled_Impl& other,
      Model_Impl* model,
      bool keepHandles)
    : SetpointManager_Impl(other,model,keepHandles)
  {
  }

  SetpointManagerScheduled_Impl::~SetpointManagerScheduled_Impl(){}

  const std::vector<std::string>& SetpointManagerScheduled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerScheduled_Impl::iddObjectType() const {
    return SetpointManagerScheduled::iddObjectType();
  }

  bool SetpointManagerScheduled_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      return this->setSetpointNode(node);
    }
    return added;
  }

  std::vector<ScheduleTypeKey> SetpointManagerScheduled_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SetpointManager_ScheduledFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SetpointManagerScheduled",scheduleDisplayName()));
    }
    return result;
  }

  boost::optional<Node> SetpointManagerScheduled_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerScheduled_Impl::setSetpointNode( const Node & node )
  {
    return setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,node.handle());
  }

  void SetpointManagerScheduled_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  std::string SetpointManagerScheduled_Impl::controlVariable() const
  {
    boost::optional<std::string> value = getString(OS_SetpointManager_ScheduledFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerScheduled_Impl::setControlVariable(const std::string& value) {
    std::string result;
    if( istringEqual(value,"Temperature") ) { result = "Temperature"; }
    else if( istringEqual(value,"MaximumTemperature") ) { result = "MaximumTemperature"; }
    else if( istringEqual(value,"MinimumTemperature") ) { result = "MinimumTemperature"; }
    else if( istringEqual(value,"HumidityRatio") ) { result = "HumidityRatio"; }
    else if( istringEqual(value,"MaximumHumidityRatio") ) { result = "MaximumHumidityRatio"; }
    else if( istringEqual(value,"MinimumHumidityRatio") ) { result = "MinimumHumidityRatio"; }
    else if( istringEqual(value,"MassFlowRate") ) { result = "MassFlowRate"; }
    else if( istringEqual(value,"MaximumMassFlowRate") ) { result = "MaximumMassFlowRate"; }
    else if( istringEqual(value,"MinimumMassFlowRate") ) { result = "MinimumMassFlowRate"; }

    if( !result.empty() ) {
      return setString(OS_SetpointManager_ScheduledFields::ControlVariable,result);
    } else {
      return false;
    }
  }

  Schedule SetpointManagerScheduled_Impl::schedule() const
  {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_ScheduledFields::ScheduleName);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerScheduled_Impl::hasSchedule() const
  {
    bool empty = isEmpty(OS_SetpointManager_ScheduledFields::ScheduleName);
    return (!empty);
  }

  bool SetpointManagerScheduled_Impl::setSchedule(Schedule & schedule)
  {
    bool result = ModelObject_Impl::setSchedule(OS_SetpointManager_ScheduledFields::ScheduleName,
                                                "SetpointManagerScheduled",
                                                scheduleDisplayName(),
                                                schedule);
    return result;
  }

  bool SetpointManagerScheduled_Impl::setControlVariableAndSchedule(
         const std::string& controlVariable,
         Schedule& schedule)
  {
    bool result(false);
    if (OptionalIddKey key = iddObject().getField(OS_SetpointManager_ScheduledFields::ControlVariable).get().getKey(controlVariable)) {
      result = ModelObject_Impl::setSchedule(OS_SetpointManager_ScheduledFields::ScheduleName,
                                             "SetpointManagerScheduled",
                                             scheduleDisplayName(key->name()),
                                             schedule);
      if (result) {
        result = setString(OS_SetpointManager_ScheduledFields::ControlVariable,key->name());
        OS_ASSERT(result);
      }
    }
    return result;
  }

  std::string SetpointManagerScheduled_Impl::scheduleDisplayName() const {
    return scheduleDisplayName(controlVariable());
  }

  std::string SetpointManagerScheduled_Impl::scheduleDisplayName(const std::string& candidateControlVariable) const
  {
    std::string result;
    const static boost::regex temperatureRegex("Temperature");
    const static boost::regex humidityRatioRegex("HumidityRatio");
    const static boost::regex massFlowRateRegex("MassFlowRate");
    if (boost::regex_search(candidateControlVariable,temperatureRegex)) {
      result = std::string("(Exact, Min, Max) Temperature");
    }
    else if (boost::regex_search(candidateControlVariable,humidityRatioRegex)) {
      result = std::string("(Exact, Min, Max) Humidity Ratio");
    }
    else if (boost::regex_search(candidateControlVariable,massFlowRateRegex)) {
      result = std::string("(Exact, Min, Max) Mass Flow Rate");
    }
    return result;
  }

  std::vector<std::string> SetpointManagerScheduled_Impl::controlVariableValues() const {
    return SetpointManagerScheduled::controlVariableValues();
  }

  boost::optional<ModelObject> SetpointManagerScheduled_Impl::scheduleAsModelObject() const {
    OptionalModelObject result = schedule();
    return result;
  }

  bool SetpointManagerScheduled_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
    }
    return false;
  }

} // detail

SetpointManagerScheduled::SetpointManagerScheduled(const Model& model,
                                                   Schedule& schedule)
  : SetpointManager(SetpointManagerScheduled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerScheduled_Impl>());
  bool ok = setControlVariable("Temperature");
  OS_ASSERT(ok);
  ok = setSchedule(schedule);
  if (!ok) {
    LOG_AND_THROW("Unable to set " << schedule.briefDescription() << " as "
                  << briefDescription() << "'s temperature setpoint schedule.");
  }
}

SetpointManagerScheduled::SetpointManagerScheduled(const Model& model,
                                                   const std::string& controlVariable,
                                                   Schedule& setpointSchedule)
  : SetpointManager(SetpointManagerScheduled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerScheduled_Impl>());
  bool ok = setControlVariable(controlVariable);
  if (!ok) {
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s control variable to "
                  << controlVariable << ".");
  }
  ok = setSchedule(setpointSchedule);
  if (!ok) {
    LOG_AND_THROW("Unable to set " << setpointSchedule.briefDescription() << " as "
                  << briefDescription() << "'s " << this->controlVariable()
                  << " setpoint schedule.");
  }
}

SetpointManagerScheduled::SetpointManagerScheduled(std::shared_ptr<detail::SetpointManagerScheduled_Impl> p)
  : SetpointManager(std::move(p))
{
}

IddObjectType SetpointManagerScheduled::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SetpointManager_Scheduled);
  return result;
}

std::vector<std::string> SetpointManagerScheduled::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_ScheduledFields::ControlVariable);
}


boost::optional<Node> SetpointManagerScheduled::setpointNode() const
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->setpointNode();
}

std::string SetpointManagerScheduled::controlVariable() const
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->controlVariable();
}

bool SetpointManagerScheduled::setControlVariable(const std::string& value)
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->setControlVariable(value);
}

Schedule SetpointManagerScheduled::schedule() const
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->schedule();
}

bool SetpointManagerScheduled::hasSchedule() const
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->hasSchedule();
}

bool SetpointManagerScheduled::setSchedule(Schedule & schedule)
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->setSchedule(schedule);
}

bool SetpointManagerScheduled::setControlVariableAndSchedule(const std::string &controlVariable,
                                                             Schedule &schedule)
{
  return getImpl<detail::SetpointManagerScheduled_Impl>()->setControlVariableAndSchedule(controlVariable,schedule);
}

} // model
} // openstudio

