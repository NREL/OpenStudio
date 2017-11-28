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

#include "CoilCoolingLowTempRadiantVarFlow.hpp"
#include "CoilCoolingLowTempRadiantVarFlow_Impl.hpp"

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

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"


namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
  }

  CoilCoolingLowTempRadiantVarFlow_Impl::CoilCoolingLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingLowTempRadiantVarFlow::iddObjectType());
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

    zoneHVACLowTempRadiantVarFlows = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantVarFlow>();

    for( const auto & zoneHVACLowTempRadiantVarFlow : zoneHVACLowTempRadiantVarFlows )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACLowTempRadiantVarFlow.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACLowTempRadiantVarFlow;
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
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isCoolingControlThrottlingRangeDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange);
  }

  std::string CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlType() const
  {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::isCondensationControlTypeDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType);
  }

  double CoilCoolingLowTempRadiantVarFlow_Impl::condensationControlDewpointOffset() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset,true);
    OS_ASSERT(value);
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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::autosizeMaximumColdWaterFlow()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCoolingControlThrottlingRange(double coolingControlThrottlingRange)
  {
    bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, coolingControlThrottlingRange);
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCoolingControlThrottlingRange()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlType(std::string condensationControlType)
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, condensationControlType);
    return result;
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlType()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlType, "");
    OS_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::setCondensationControlDewpointOffset(double condensationControlDewpointOffset)
  {
    bool result = setDouble(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, condensationControlDewpointOffset);
    OS_ASSERT(result);
  }

  void CoilCoolingLowTempRadiantVarFlow_Impl::resetCondensationControlDewpointOffset()
  {
    bool result = setString(OS_Coil_Cooling_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset, "");
    OS_ASSERT(result);
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

  bool CoilCoolingLowTempRadiantVarFlow_Impl::addToNode(Node & node)
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

 CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(const Model& model,
                                                                       Schedule& coolingControlTemperatureSchedule)
  : StraightComponent(CoilCoolingLowTempRadiantVarFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingLowTempRadiantVarFlow_Impl>());

  bool ok = setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule);
  OS_ASSERT(ok);
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
CoilCoolingLowTempRadiantVarFlow::CoilCoolingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantVarFlow_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

