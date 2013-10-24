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

#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>

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

#include <utilities/idd/OS_Coil_Heating_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const CoilHeatingLowTempRadiantVarFlow_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}
  
  //ModelObject CoilHeatingLowTempRadiantVarFlow_Impl::clone(Model model) const
  //{
  //  CoilHeatingLowTempRadiantVarFlow newCoil = WaterToAirComponent_Impl::clone(model).optionalCast<CoilHeatingLowTempRadiantVarFlow>().get();

  //  return newCoil;
  //}
  
  const std::vector<std::string>& CoilHeatingLowTempRadiantVarFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType CoilHeatingLowTempRadiantVarFlow_Impl::iddObjectType() const 
  { 
    return CoilHeatingLowTempRadiantVarFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingLowTempRadiantVarFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    
    if (std::find(b,e,OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingLowTempRadiantVarFlow","Heating Control Temperature"));
    }
    return result;
  }
  
  unsigned CoilHeatingLowTempRadiantVarFlow_Impl::inletPort()
  {
    return OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName;
  }

  unsigned CoilHeatingLowTempRadiantVarFlow_Impl::outletPort()
  {
    return OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingLowTempRadiantVarFlow_Impl::containingZoneHVACComponent() const
  {
    std::vector<ZoneHVACLowTempRadiantVarFlow> zoneHVACLowTempRadiantVarFlows;

    zoneHVACLowTempRadiantVarFlows = this->model().getModelObjects<ZoneHVACLowTempRadiantVarFlow>();

    for( std::vector<ZoneHVACLowTempRadiantVarFlow>::iterator it = zoneHVACLowTempRadiantVarFlows.begin();
    it < zoneHVACLowTempRadiantVarFlows.end();
    it++ )
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

  boost::optional<double> CoilHeatingLowTempRadiantVarFlow_Impl::maximumHotWaterFlow() const 
  {
    return getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow,true);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::isMaximumHotWaterFlowDefaulted() const 
  {
    return isEmpty(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::isMaximumHotWaterFlowAutosized() const 
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, true);
    if (value) 
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlThrottlingRange() const 
  {
    boost::optional<double> value = getDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::isHeatingControlThrottlingRangeDefaulted() const 
  {
    return isEmpty(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange);
  }

  boost::optional<Schedule> CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlTemperatureSchedule() const 
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::setMaximumHotWaterFlow(boost::optional<double> maximumHotWaterFlow) 
  {
    bool result(false);
    if (maximumHotWaterFlow) 
    {
      result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, maximumHotWaterFlow.get());
    }
    else 
    {
      resetMaximumHotWaterFlow();
      result = true;
    }
    return result;
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::resetMaximumHotWaterFlow() 
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, "");
    BOOST_ASSERT(result);
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::autosizeMaximumHotWaterFlow() 
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, "autosize");
    BOOST_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlThrottlingRange(double heatingControlThrottlingRange) 
  {
    bool result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, heatingControlThrottlingRange);
    return result;
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::resetHeatingControlThrottlingRange() 
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, "");
    BOOST_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlTemperatureSchedule(Schedule& schedule) 
  {
    bool result = setSchedule(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName,
                              "CoilHeatingLowTempRadiantVarFlow",
                              "Heating Control Temperature",
                              schedule);
    return result;
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::resetHeatingControlTemperatureSchedule() 
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName, "");
    BOOST_ASSERT(result);
  }

  boost::optional<ModelObject> CoilHeatingLowTempRadiantVarFlow_Impl::heatingControlTemperatureScheduleAsModelObject() const 
  {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingControlTemperatureSchedule();
    if (intermediate) 
    {
      result = *intermediate;
    }
    return result;
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) 
  {
    if (modelObject) 
    {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) 
      {
        Schedule schedule(*intermediate);
        return setHeatingControlTemperatureSchedule(schedule);
      }
      else 
      {
        return false;
      }
    }
    else 
    {
      resetHeatingControlTemperatureSchedule();
    }
    return true;
  }

} // detail

CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(const Model& model,Schedule& heatingControlTemperature)

  : StraightComponent(CoilHeatingLowTempRadiantVarFlow::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>());

  bool ok = true;
  
  ok = setHeatingControlTemperatureSchedule(heatingControlTemperature);
  BOOST_ASSERT(ok);
}

IddObjectType CoilHeatingLowTempRadiantVarFlow::iddObjectType() 
{
  return IddObjectType(IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow);
}

boost::optional<double> CoilHeatingLowTempRadiantVarFlow::maximumHotWaterFlow() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->maximumHotWaterFlow();
}

bool CoilHeatingLowTempRadiantVarFlow::isMaximumHotWaterFlowDefaulted() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isMaximumHotWaterFlowDefaulted();
}

bool CoilHeatingLowTempRadiantVarFlow::isMaximumHotWaterFlowAutosized() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isMaximumHotWaterFlowAutosized();
}

double CoilHeatingLowTempRadiantVarFlow::heatingControlThrottlingRange() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingControlThrottlingRange();
}

bool CoilHeatingLowTempRadiantVarFlow::isHeatingControlThrottlingRangeDefaulted() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->isHeatingControlThrottlingRangeDefaulted();
}

boost::optional<Schedule> CoilHeatingLowTempRadiantVarFlow::heatingControlTemperatureSchedule() const 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->heatingControlTemperatureSchedule();
}

bool CoilHeatingLowTempRadiantVarFlow::setMaximumHotWaterFlow(double maximumHotWaterFlow) 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setMaximumHotWaterFlow(maximumHotWaterFlow);
}

void CoilHeatingLowTempRadiantVarFlow::resetMaximumHotWaterFlow() 
{
  getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetMaximumHotWaterFlow();
}

void CoilHeatingLowTempRadiantVarFlow::autosizeMaximumHotWaterFlow() 
{
  getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->autosizeMaximumHotWaterFlow();
}

bool CoilHeatingLowTempRadiantVarFlow::setHeatingControlThrottlingRange(double heatingControlThrottlingRange) 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingControlThrottlingRange(heatingControlThrottlingRange);
}

void CoilHeatingLowTempRadiantVarFlow::resetHeatingControlThrottlingRange() 
{
  getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetHeatingControlThrottlingRange();
}

bool CoilHeatingLowTempRadiantVarFlow::setHeatingControlTemperatureSchedule(Schedule& schedule) 
{
  return getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->setHeatingControlTemperatureSchedule(schedule);
}

void CoilHeatingLowTempRadiantVarFlow::resetHeatingControlTemperatureSchedule() 
{
  getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>()->resetHeatingControlTemperatureSchedule();
}

/// @cond
CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(boost::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

