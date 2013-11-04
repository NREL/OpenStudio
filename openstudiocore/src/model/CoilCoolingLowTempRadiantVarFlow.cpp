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

#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>

#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>


namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const CoilCoolingLowTempRadiantVarFlow_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingLowTempRadiantVarFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType CoilCoolingLowTempRadiantVarFlow_Impl::iddObjectType() const
  {
    return CoilCoolingLowTempRadiantVarFlow::iddObjectType();
  }

  unsigned CoilCoolingLowTempRadiantVarFlow_Impl::inletPort()
  {
    return OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName;
  }

  unsigned CoilCoolingLowTempRadiantVarFlow_Impl::outletPort()
  {
    return OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName;
  }

  std::vector<ScheduleTypeKey> CoilCoolingLowTempRadiantVarFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingLowTempRadiantVarFlow","Cooling Control Temperature Schedule"));
    }
    return result;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingLowTempRadiantVarFlow_Impl::containingZoneHVACComponent() const
  {

    std::vector<ZoneHVACLowTempRadiantVarFlow> zoneHVACLowTempRadiantVarFlows;

    zoneHVACLowTempRadiantVarFlows = this->model().getModelObjects<ZoneHVACLowTempRadiantVarFlow>();

    for( std::vector<ZoneHVACLowTempRadiantVarFlow>::iterator it = zoneHVACLowTempRadiantVarFlows.begin();
    it < zoneHVACLowTempRadiantVarFlows.end();
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

  boost::optional<Schedule> CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlTemperatureSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName);
  }

  boost::optional<double> CoilCoolingLowTempRadiantVarFlow_Impl::maximumColdWaterFlow() const
  {
    return getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow,true);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isMaximumColdWaterFlowDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isMaximumColdWaterFlowAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlThrottlingRange() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isCoolingControlThrottlingRangeDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange);
  }

  std::string CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlType() const
  {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isCondensationControlTypeDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType);
  }

  double CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlDewpointOffset() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isCondensationControlDewpointOffsetDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlTemperatureSchedule(Schedule& schedule)
  {
    bool result = setSchedule(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName,
                              "CoilCoolingLowTempRadiantVarFlow",
                              "Cooling Control Temperature Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCoolingControlTemperatureSchedule()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setMaximumColdWaterFlow(boost::optional<double> maximumColdWaterFlow)
  {
    bool result(false);
    if (maximumColdWaterFlow)
    {
      result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, maximumColdWaterFlow.get());
    }
    else
    {
      resetMaximumColdWaterFlow();
      result = true;
    }
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetMaximumColdWaterFlow()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, "");
    BOOST_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::autosizeMaximumColdWaterFlow()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, "autosize");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlThrottlingRange(double coolingControlThrottlingRange)
  {
    bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, coolingControlThrottlingRange);
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCoolingControlThrottlingRange()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, "");
    BOOST_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlType(std::string condensationControlType)
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, condensationControlType);
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlType()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, "");
    BOOST_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlDewpointOffset(double condensationControlDewpointOffset)
  {
    bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, condensationControlDewpointOffset);
    BOOST_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlDewpointOffset()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, "");
    BOOST_ASSERT(result);
  }

  std::vector<std::string> CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlTypeValues() const
  {
    return CoilCoolingLowTempRadiantVarFlow::condensationControlTypeValues();
  }

  boost::optional<ModelObject> CoilCoolingLowTempRadiantVarFlow_Impl::coolingControlTemperatureScheduleAsModelObject() const
  {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingControlTemperatureSchedule();
    if (intermediate)
    {
      result = *intermediate;
    }
    return result;
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject)
  {
    if (modelObject)
    {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate)
      {
        Schedule schedule(*intermediate);
        return setCoolingControlTemperatureSchedule(schedule);
      }
      else
      {
        return false;
      }
    }
    else
    {
      resetCoolingControlTemperatureSchedule();
    }
    return true;
  }

} // detail

 CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(const Model& model,
                                                                       Schedule& coolingControlTemperatureSchedule)
  : StraightComponent(CoilCoolingLowTempRadiantVarFlow::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>());

  bool ok = setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule);
  BOOST_ASSERT(ok);
}

IddObjectType CoilCoolingLowTempRadiantVarFlow::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow);
}

std::vector<std::string> CoilCoolingLowTempRadiantVarFlow::condensationControlTypeValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType);
}

boost::optional<Schedule> CoilCoolingLowTempRadiantVarFlow::coolingControlTemperatureSchedule() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingControlTemperatureSchedule();
}

boost::optional<double> CoilCoolingLowTempRadiantVarFlow::maximumColdWaterFlow() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->maximumColdWaterFlow();
}

bool CoilCoolingLowTempRadiantVarFlow::isMaximumColdWaterFlowDefaulted() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isMaximumColdWaterFlowDefaulted();
}

bool CoilCoolingLowTempRadiantVarFlow::isMaximumColdWaterFlowAutosized() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isMaximumColdWaterFlowAutosized();
}

double CoilCoolingLowTempRadiantVarFlow::coolingControlThrottlingRange() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->coolingControlThrottlingRange();
}

bool CoilCoolingLowTempRadiantVarFlow::isCoolingControlThrottlingRangeDefaulted() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCoolingControlThrottlingRangeDefaulted();
}

std::string CoilCoolingLowTempRadiantVarFlow::condensationControlType() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->condensationControlType();
}

bool CoilCoolingLowTempRadiantVarFlow::isCondensationControlTypeDefaulted() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCondensationControlTypeDefaulted();
}

double CoilCoolingLowTempRadiantVarFlow::condensationControlDewpointOffset() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->condensationControlDewpointOffset();
}

bool CoilCoolingLowTempRadiantVarFlow::isCondensationControlDewpointOffsetDefaulted() const
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->isCondensationControlDewpointOffsetDefaulted();
}

bool CoilCoolingLowTempRadiantVarFlow::setMaximumColdWaterFlow(double maximumColdWaterFlow)
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setMaximumColdWaterFlow(maximumColdWaterFlow);
}

void CoilCoolingLowTempRadiantVarFlow::resetMaximumColdWaterFlow()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetMaximumColdWaterFlow();
}

void CoilCoolingLowTempRadiantVarFlow::autosizeMaximumColdWaterFlow()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->autosizeMaximumColdWaterFlow();
}

bool CoilCoolingLowTempRadiantVarFlow::setCoolingControlThrottlingRange(double coolingControlThrottlingRange)
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingControlThrottlingRange(coolingControlThrottlingRange);
}

void CoilCoolingLowTempRadiantVarFlow::resetCoolingControlThrottlingRange()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCoolingControlThrottlingRange();
}

bool CoilCoolingLowTempRadiantVarFlow::setCoolingControlTemperatureSchedule(Schedule& schedule)
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCoolingControlTemperatureSchedule(schedule);
}

void CoilCoolingLowTempRadiantVarFlow::resetCoolingControlTemperatureSchedule()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCoolingControlTemperatureSchedule();
}

bool CoilCoolingLowTempRadiantVarFlow::setCondensationControlType(std::string condensationControlType)
{
  return getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCondensationControlType(condensationControlType);
}

void CoilCoolingLowTempRadiantVarFlow::resetCondensationControlType()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCondensationControlType();
}

void CoilCoolingLowTempRadiantVarFlow::setCondensationControlDewpointOffset(double condensationControlDewpointOffset)
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->setCondensationControlDewpointOffset(condensationControlDewpointOffset);
}

void CoilCoolingLowTempRadiantVarFlow::resetCondensationControlDewpointOffset()
{
  getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>()->resetCondensationControlDewpointOffset();
}

/// @cond
CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(boost::shared_ptr<detail::CoilCoolingLowTempRadiantVarFlow_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

