/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerScheduled_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddKey.hpp>
#include <utilities/idd/OS_SetpointManager_Scheduled_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerScheduled::iddObjectType());
  }

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerScheduled::iddObjectType());
  }

  SetpointManagerScheduled_Impl::SetpointManagerScheduled_Impl(
      const SetpointManagerScheduled_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : HVACComponent_Impl(other,model,keepHandles)
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

  boost::optional<ParentObject> SetpointManagerScheduled_Impl::parent() const {
    NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
    if (nodes.size() == 1u) {
      return nodes[0];
    }
    return boost::none;
  }

  std::vector<ModelObject> SetpointManagerScheduled_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  bool SetpointManagerScheduled_Impl::addToNode(Node & node)
  {
    if( node.model() != this->model() )
    {
      return false;
    } 

    node.removeSetpointManagerMixedAir();

    node.removeSetpointManagerSingleZoneReheat();

    node.removeSetpointManagerScheduled();

    node.removeSetpointManagerFollowOutdoorAirTemperature();

    node.removeSetpointManagerOutdoorAirReset();

    node.removeSetpointManagerWarmest();

    if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);

        return true;
      }
      if(OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop->airLoopHVACOutdoorAirSystem())
      {
        if(node == oaSystem->outboardOANode().get())
        {
          return false;
        }

        if(oaSystem->oaComponent(node.handle()))
        {
          this->setSetpointNode(node);
        
          return true;
        }
      }
    }
    else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() )
    {
      if( plantLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);

        return true;
      }
    }

    return false;
  }

  std::vector<openstudio::IdfObject> SetpointManagerScheduled_Impl::remove()
  {
    return HVACComponent_Impl::remove();
  }

  ModelObject SetpointManagerScheduled_Impl::clone(Model model)
  {
    return HVACComponent_Impl::clone( model );
  }

  boost::optional<Node> SetpointManagerScheduled_Impl::setpointNode() const
  {
    SetpointManagerScheduled thisModelObject = this->getObject<SetpointManagerScheduled>();

    return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName);
  }

  void SetpointManagerScheduled_Impl::setSetpointNode( Node & node )
  {
    SetpointManagerScheduled thisModelObject = this->getObject<SetpointManagerScheduled>();

    thisModelObject.setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,node.handle());
  }

  std::string SetpointManagerScheduled_Impl::controlVariable() const
  {
    boost::optional<std::string> value = getString(OS_SetpointManager_ScheduledFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerScheduled_Impl::setControlVariable(std::string value) {
    if (boost::optional<IddKey> key = iddObject().getField(OS_SetpointManager_ScheduledFields::ControlVariable).get().getKey(value)) {
      std::string currentScheduleDisplayName = scheduleDisplayName();
      if (currentScheduleDisplayName.empty() || (scheduleDisplayName() == scheduleDisplayName(key->name()))) {
        return setString(OS_SetpointManager_ScheduledFields::ControlVariable,key->name());
      }
    }
    return false;
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
    if (boost::regex_search(candidateControlVariable,boost::regex("Temperature"))) {
      result = std::string("(Exact, Min, Max) Temperature");
    }
    else if (boost::regex_search(candidateControlVariable,boost::regex("HumidityRatio"))) {
      result = std::string("(Exact, Min, Max) Humidity Ratio");
    }
    else if (boost::regex_search(candidateControlVariable,boost::regex("MassFlowRate"))) {
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
  : HVACComponent(SetpointManagerScheduled::iddObjectType(),model) 
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
  : HVACComponent(SetpointManagerScheduled::iddObjectType(),model)
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

SetpointManagerScheduled::SetpointManagerScheduled(boost::shared_ptr<detail::SetpointManagerScheduled_Impl> p)
  : HVACComponent(p)
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

bool SetpointManagerScheduled::setControlVariable(std::string value)
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

