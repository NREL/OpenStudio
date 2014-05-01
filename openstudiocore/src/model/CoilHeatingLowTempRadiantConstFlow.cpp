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

#include <model/CoilHeatingLowTempRadiantConstFlow.hpp>
#include <model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingLowTempRadiantConstFlow_Impl::CoilHeatingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingLowTempRadiantConstFlow::iddObjectType());
  }

  CoilHeatingLowTempRadiantConstFlow_Impl::CoilHeatingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingLowTempRadiantConstFlow::iddObjectType());
  }

  CoilHeatingLowTempRadiantConstFlow_Impl::CoilHeatingLowTempRadiantConstFlow_Impl(const CoilHeatingLowTempRadiantConstFlow_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingLowTempRadiantConstFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingLowTempRadiantConstFlow_Impl::iddObjectType() const {
    return CoilHeatingLowTempRadiantConstFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingLowTempRadiantConstFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighWaterTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantConstFlow","Heating High Water Temperature"));
    }
    if (std::find(b,e,OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowWaterTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantConstFlow","Heating Low Water Temperature"));
    }
    if (std::find(b,e,OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantConstFlow","Heating High Control Temperature"));
    }
    if (std::find(b,e,OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantConstFlow","Heating Low Control Temperature"));
    }
    return result;
  }


  unsigned CoilHeatingLowTempRadiantConstFlow_Impl::inletPort()
{
  return OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingWaterInletNodeName;
}

  unsigned CoilHeatingLowTempRadiantConstFlow_Impl::outletPort()
{
  return OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingWaterOutletNodeName;
}

  boost::optional<ZoneHVACComponent> CoilHeatingLowTempRadiantConstFlow_Impl::containingZoneHVACComponent() const
  {
    std::vector<ZoneHVACLowTempRadiantConstFlow> zoneHVACLowTempRadiantConstFlows;

    zoneHVACLowTempRadiantConstFlows = this->model().getModelObjects<ZoneHVACLowTempRadiantConstFlow>();

    for( std::vector<ZoneHVACLowTempRadiantConstFlow>::iterator it = zoneHVACLowTempRadiantConstFlows.begin();
    it < zoneHVACLowTempRadiantConstFlows.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->heatingCoil() )
      {
        if( coil->handle() == this->handle() )  
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow_Impl::heatingHighWaterTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighWaterTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow_Impl::heatingLowWaterTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowWaterTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow_Impl::heatingHighControlTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighControlTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow_Impl::heatingLowControlTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowControlTemperatureScheduleName);
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingHighWaterTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighWaterTemperatureScheduleName,
                              "CoilHeatingLowTempRadiantConstFlow",
                              "Heating High Water Temperature",
                              schedule);
    return result;
  }

  void CoilHeatingLowTempRadiantConstFlow_Impl::resetHeatingHighWaterTemperatureSchedule() {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighWaterTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingLowWaterTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowWaterTemperatureScheduleName,
                              "CoilHeatingLowTempRadiantConstFlow",
                              "Heating Low Water Temperature",
                              schedule);
    return result;
  }

  void CoilHeatingLowTempRadiantConstFlow_Impl::resetHeatingLowWaterTemperatureSchedule() {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowWaterTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingHighControlTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighControlTemperatureScheduleName,
                              "CoilHeatingLowTempRadiantConstFlow",
                              "Heating High Control Temperature",
                              schedule);
    return result;
  }

  void CoilHeatingLowTempRadiantConstFlow_Impl::resetHeatingHighControlTemperatureSchedule() {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingHighControlTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingLowControlTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowControlTemperatureScheduleName,
                              "CoilHeatingLowTempRadiantConstFlow",
                              "Heating Low Control Temperature",
                              schedule);
    return result;
  }

  void CoilHeatingLowTempRadiantConstFlow_Impl::resetHeatingLowControlTemperatureSchedule() {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_ConstantFlowFields::HeatingLowControlTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> CoilHeatingLowTempRadiantConstFlow_Impl::heatingHighWaterTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingHighWaterTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilHeatingLowTempRadiantConstFlow_Impl::heatingLowWaterTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingLowWaterTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilHeatingLowTempRadiantConstFlow_Impl::heatingHighControlTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingHighControlTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilHeatingLowTempRadiantConstFlow_Impl::heatingLowControlTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingLowControlTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHeatingHighWaterTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHeatingHighWaterTemperatureSchedule();
    }
    return true;
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHeatingLowWaterTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHeatingLowWaterTemperatureSchedule();
    }
    return true;
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHeatingHighControlTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHeatingHighControlTemperatureSchedule();
    }
    return true;
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::setHeatingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHeatingLowControlTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHeatingLowControlTemperatureSchedule();
    }
    return true;
  }

  bool CoilHeatingLowTempRadiantConstFlow_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

CoilHeatingLowTempRadiantConstFlow::CoilHeatingLowTempRadiantConstFlow(const Model& model,
                                     Schedule& heatingHighWaterTemperatureSchedule,
                                     Schedule& heatingLowWaterTemperatureSchedule,
                                     Schedule& heatingHighControlTemperatureSchedule,
                                     Schedule& heatingLowControlTemperatureSchedule)
  : StraightComponent(CoilHeatingLowTempRadiantConstFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>());
  bool ok = setHeatingHighWaterTemperatureSchedule(heatingHighWaterTemperatureSchedule);
  OS_ASSERT(ok);

  ok = setHeatingLowWaterTemperatureSchedule(heatingLowWaterTemperatureSchedule);
  OS_ASSERT(ok);

  ok = setHeatingHighControlTemperatureSchedule(heatingHighControlTemperatureSchedule);
  OS_ASSERT(ok);

  ok = setHeatingLowControlTemperatureSchedule(heatingLowControlTemperatureSchedule);
  OS_ASSERT(ok);

}

IddObjectType CoilHeatingLowTempRadiantConstFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow);
}

boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow::heatingHighWaterTemperatureSchedule() const {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->heatingHighWaterTemperatureSchedule();
}

boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow::heatingLowWaterTemperatureSchedule() const {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->heatingLowWaterTemperatureSchedule();
}

boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow::heatingHighControlTemperatureSchedule() const {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->heatingHighControlTemperatureSchedule();
}

boost::optional<Schedule> CoilHeatingLowTempRadiantConstFlow::heatingLowControlTemperatureSchedule() const {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->heatingLowControlTemperatureSchedule();
}

bool CoilHeatingLowTempRadiantConstFlow::setHeatingHighWaterTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->setHeatingHighWaterTemperatureSchedule(schedule);
}

void CoilHeatingLowTempRadiantConstFlow::resetHeatingHighWaterTemperatureSchedule() {
  getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->resetHeatingHighWaterTemperatureSchedule();
}

bool CoilHeatingLowTempRadiantConstFlow::setHeatingLowWaterTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->setHeatingLowWaterTemperatureSchedule(schedule);
}

void CoilHeatingLowTempRadiantConstFlow::resetHeatingLowWaterTemperatureSchedule() {
  getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->resetHeatingLowWaterTemperatureSchedule();
}

bool CoilHeatingLowTempRadiantConstFlow::setHeatingHighControlTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->setHeatingHighControlTemperatureSchedule(schedule);
}

void CoilHeatingLowTempRadiantConstFlow::resetHeatingHighControlTemperatureSchedule() {
  getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->resetHeatingHighControlTemperatureSchedule();
}

bool CoilHeatingLowTempRadiantConstFlow::setHeatingLowControlTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->setHeatingLowControlTemperatureSchedule(schedule);
}

void CoilHeatingLowTempRadiantConstFlow::resetHeatingLowControlTemperatureSchedule() {
  getImpl<detail::CoilHeatingLowTempRadiantConstFlow_Impl>()->resetHeatingLowControlTemperatureSchedule();
}

/// @cond
CoilHeatingLowTempRadiantConstFlow::CoilHeatingLowTempRadiantConstFlow(boost::shared_ptr<detail::CoilHeatingLowTempRadiantConstFlow_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

