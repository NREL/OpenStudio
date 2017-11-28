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

#include "CoilHeatingLowTempRadiantConstFlow.hpp"
#include "CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "ZoneHVACLowTempRadiantConstFlow.hpp"
#include "ZoneHVACLowTempRadiantConstFlow_Impl.hpp"
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

#include <utilities/idd/OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

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

    zoneHVACLowTempRadiantConstFlows = this->model().getConcreteModelObjects<ZoneHVACLowTempRadiantConstFlow>();

    for( const auto & zoneHVACLowTempRadiantConstFlow : zoneHVACLowTempRadiantConstFlows )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACLowTempRadiantConstFlow.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACLowTempRadiantConstFlow;
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
CoilHeatingLowTempRadiantConstFlow::CoilHeatingLowTempRadiantConstFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantConstFlow_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

