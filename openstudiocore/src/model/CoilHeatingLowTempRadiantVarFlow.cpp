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

#include "CoilHeatingLowTempRadiantVarFlow.hpp"
#include "CoilHeatingLowTempRadiantVarFlow_Impl.hpp"

#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilHeatingLowTempRadiantVarFlow_Impl::CoilHeatingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingLowTempRadiantVarFlow::iddObjectType());
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

    zoneHVACLowTempRadiantVarFlows = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();

    for( const auto & zoneHVACLowTempRadiantVarFlow : zoneHVACLowTempRadiantVarFlows )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACLowTempRadiantVarFlow.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACLowTempRadiantVarFlow;
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
    OS_ASSERT(value);
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
    OS_ASSERT(result);
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::autosizeMaximumHotWaterFlow()
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingLowTempRadiantVarFlow_Impl::setHeatingControlThrottlingRange(double heatingControlThrottlingRange)
  {
    bool result = setDouble(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, heatingControlThrottlingRange);
    return result;
  }

  void CoilHeatingLowTempRadiantVarFlow_Impl::resetHeatingControlThrottlingRange()
  {
    bool result = setString(OS_Coil_Heating_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange, "");
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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

  bool CoilHeatingLowTempRadiantVarFlow_Impl::addToNode(Node & node)
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

CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(const Model& model,Schedule& heatingControlTemperature)

  : StraightComponent(CoilHeatingLowTempRadiantVarFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingLowTempRadiantVarFlow_Impl>());

  bool ok = setHeatingControlTemperatureSchedule(heatingControlTemperature);
  OS_ASSERT(ok);
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
CoilHeatingLowTempRadiantVarFlow::CoilHeatingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

