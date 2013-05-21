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

#include <model/CoilCoolingLowTempRadiantConstFlow.hpp>
#include <model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingLowTempRadiantConstFlow_Impl::CoilCoolingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == CoilCoolingLowTempRadiantConstFlow::iddObjectType());
  }

  CoilCoolingLowTempRadiantConstFlow_Impl::CoilCoolingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == CoilCoolingLowTempRadiantConstFlow::iddObjectType());
  }

  CoilCoolingLowTempRadiantConstFlow_Impl::CoilCoolingLowTempRadiantConstFlow_Impl(const CoilCoolingLowTempRadiantConstFlow_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingLowTempRadiantConstFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingLowTempRadiantConstFlow_Impl::iddObjectType() const {
    return CoilCoolingLowTempRadiantConstFlow::iddObjectType();
  }

  unsigned CoilCoolingLowTempRadiantConstFlow_Impl::inletPort()
  {
    return OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingWaterInletNodeName;
  }

  unsigned CoilCoolingLowTempRadiantConstFlow_Impl::outletPort()
  {
    return OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingWaterOutletNodeName;
  }

  std::vector<ScheduleTypeKey> CoilCoolingLowTempRadiantConstFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighWaterTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantConstFlow","Cooling High Water Temperature"));
    }
    if (std::find(b,e,OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowWaterTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantConstFlow","Cooling Low Water Temperature"));
    }
    if (std::find(b,e,OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantConstFlow","Cooling High Control Temperature"));
    }
    if (std::find(b,e,OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantConstFlow","Cooling Low Control Temperature"));
    }
    return result;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingLowTempRadiantConstFlow_Impl::containingZoneHVACComponent() const
  {
  
    std::vector<ZoneHVACLowTempRadiantConstFlow> zoneHVACLowTempRadiantConstFlows;

    zoneHVACLowTempRadiantConstFlows = this->model().getModelObjects<ZoneHVACLowTempRadiantConstFlow>();

    for( std::vector<ZoneHVACLowTempRadiantConstFlow>::iterator it = zoneHVACLowTempRadiantConstFlows.begin();
    it < zoneHVACLowTempRadiantConstFlows.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->coolingCoil() )
      {
        if( coil->handle() == this->handle() )  
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow_Impl::coolingHighWaterTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighWaterTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow_Impl::coolingLowWaterTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowWaterTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow_Impl::coolingHighControlTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighControlTemperatureScheduleName);
  }

  boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow_Impl::coolingLowControlTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowControlTemperatureScheduleName);
  }

  std::string CoilCoolingLowTempRadiantConstFlow_Impl::condensationControlType() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::isCondensationControlTypeDefaulted() const {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlType);
  }

  double CoilCoolingLowTempRadiantConstFlow_Impl::condensationControlDewpointOffset() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlDewpointOffset,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::isCondensationControlDewpointOffsetDefaulted() const {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlDewpointOffset);
  }

   bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingHighWaterTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighWaterTemperatureScheduleName,
                              "CoilCoolingLowTempRadiantConstFlow",
                              "Cooling High Water Temperature",
                              schedule);
    return result;
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCoolingHighWaterTemperatureSchedule() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighWaterTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingLowWaterTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowWaterTemperatureScheduleName,
                              "CoilCoolingLowTempRadiantConstFlow",
                              "Cooling Low Water Temperature",
                              schedule);
    return result;
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCoolingLowWaterTemperatureSchedule() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowWaterTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingHighControlTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighControlTemperatureScheduleName,
                              "CoilCoolingLowTempRadiantConstFlow",
                              "Cooling High Control Temperature",
                              schedule);
    return result;
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCoolingHighControlTemperatureSchedule() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingHighControlTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingLowControlTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowControlTemperatureScheduleName,
                              "CoilCoolingLowTempRadiantConstFlow",
                              "Cooling Low Control Temperature",
                              schedule);
    return result;
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCoolingLowControlTemperatureSchedule() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CoolingLowControlTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCondensationControlType(std::string condensationControlType) {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlType, condensationControlType);
    return result;
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCondensationControlType() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlType, "");
    BOOST_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
    bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlDewpointOffset, condensationControlDewpointOffset);
    BOOST_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantConstFlow_Impl::resetCondensationControlDewpointOffset() {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlDewpointOffset, "");
    BOOST_ASSERT(result);
  }

  std::vector<std::string> CoilCoolingLowTempRadiantConstFlow_Impl::condensationControlTypeValues() const {
    return CoilCoolingLowTempRadiantConstFlow::condensationControlTypeValues();
  }

  boost::optional<ModelObject> CoilCoolingLowTempRadiantConstFlow_Impl::coolingHighWaterTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingHighWaterTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilCoolingLowTempRadiantConstFlow_Impl::coolingLowWaterTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingLowWaterTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilCoolingLowTempRadiantConstFlow_Impl::coolingHighControlTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingHighControlTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoilCoolingLowTempRadiantConstFlow_Impl::coolingLowControlTemperatureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingLowControlTemperatureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setCoolingHighWaterTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetCoolingHighWaterTemperatureSchedule();
    }
    return true;
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setCoolingLowWaterTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetCoolingLowWaterTemperatureSchedule();
    }
    return true;
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setCoolingHighControlTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetCoolingHighControlTemperatureSchedule();
    }
    return true;
  }

  bool CoilCoolingLowTempRadiantConstFlow_Impl::setCoolingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setCoolingLowControlTemperatureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetCoolingLowControlTemperatureSchedule();
    }
    return true;
  }

} // detail

 CoilCoolingLowTempRadiantConstFlow::CoilCoolingLowTempRadiantConstFlow(const Model& model,
                                                                       Schedule& coolingHighWaterTemperatureSchedule,
                                                                       Schedule& coolingLowWaterTemperatureSchedule,
                                                                       Schedule& coolingHighControlTemperatureSchedule,
                                                                       Schedule& coolingLowControlTemperatureSchedule)
  : StraightComponent(CoilCoolingLowTempRadiantConstFlow::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>());

  bool ok = setCoolingHighWaterTemperatureSchedule(coolingHighWaterTemperatureSchedule);
  BOOST_ASSERT(ok);

  ok = setCoolingLowWaterTemperatureSchedule(coolingLowWaterTemperatureSchedule);
  BOOST_ASSERT(ok);

  ok = setCoolingHighControlTemperatureSchedule(coolingHighControlTemperatureSchedule);
  BOOST_ASSERT(ok);

  ok = setCoolingLowControlTemperatureSchedule(coolingLowControlTemperatureSchedule);
  BOOST_ASSERT(ok);

}

 IddObjectType CoilCoolingLowTempRadiantConstFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow);
}

std::vector<std::string> CoilCoolingLowTempRadiantConstFlow::condensationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlowFields::CondensationControlType);
}


boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow::coolingHighWaterTemperatureSchedule() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->coolingHighWaterTemperatureSchedule();
}

boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow::coolingLowWaterTemperatureSchedule() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->coolingLowWaterTemperatureSchedule();
}

boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow::coolingHighControlTemperatureSchedule() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->coolingHighControlTemperatureSchedule();
}

boost::optional<Schedule> CoilCoolingLowTempRadiantConstFlow::coolingLowControlTemperatureSchedule() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->coolingLowControlTemperatureSchedule();
}

std::string CoilCoolingLowTempRadiantConstFlow::condensationControlType() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->condensationControlType();
}

bool CoilCoolingLowTempRadiantConstFlow::isCondensationControlTypeDefaulted() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->isCondensationControlTypeDefaulted();
}

double CoilCoolingLowTempRadiantConstFlow::condensationControlDewpointOffset() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->condensationControlDewpointOffset();
}


bool CoilCoolingLowTempRadiantConstFlow::isCondensationControlDewpointOffsetDefaulted() const {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->isCondensationControlDewpointOffsetDefaulted();
}

bool CoilCoolingLowTempRadiantConstFlow::setCoolingHighWaterTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCoolingHighWaterTemperatureSchedule(schedule);
}

void CoilCoolingLowTempRadiantConstFlow::resetCoolingHighWaterTemperatureSchedule() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCoolingHighWaterTemperatureSchedule();
}

bool CoilCoolingLowTempRadiantConstFlow::setCoolingLowWaterTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCoolingLowWaterTemperatureSchedule(schedule);
}

void CoilCoolingLowTempRadiantConstFlow::resetCoolingLowWaterTemperatureSchedule() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCoolingLowWaterTemperatureSchedule();
}

bool CoilCoolingLowTempRadiantConstFlow::setCoolingHighControlTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCoolingHighControlTemperatureSchedule(schedule);
}

void CoilCoolingLowTempRadiantConstFlow::resetCoolingHighControlTemperatureSchedule() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCoolingHighControlTemperatureSchedule();
}

bool CoilCoolingLowTempRadiantConstFlow::setCoolingLowControlTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCoolingLowControlTemperatureSchedule(schedule);
}

void CoilCoolingLowTempRadiantConstFlow::resetCoolingLowControlTemperatureSchedule() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCoolingLowControlTemperatureSchedule();
}

bool CoilCoolingLowTempRadiantConstFlow::setCondensationControlType(std::string condensationControlType) {
  return getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCondensationControlType(condensationControlType);
}

void CoilCoolingLowTempRadiantConstFlow::resetCondensationControlType() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCondensationControlType();
}

void CoilCoolingLowTempRadiantConstFlow::setCondensationControlDewpointOffset(double condensationControlDewpointOffset) {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->setCondensationControlDewpointOffset(condensationControlDewpointOffset);
}

void CoilCoolingLowTempRadiantConstFlow::resetCondensationControlDewpointOffset() {
  getImpl<detail::CoilCoolingLowTempRadiantConstFlow_Impl>()->resetCondensationControlDewpointOffset();
}

/// @cond
CoilCoolingLowTempRadiantConstFlow::CoilCoolingLowTempRadiantConstFlow(boost::shared_ptr<detail::CoilCoolingLowTempRadiantConstFlow_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

