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

#include "Model.hpp"
#include "CoilHeatingElectric.hpp"
#include "CoilHeatingElectric_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingElectric_Impl::CoilHeatingElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingElectric::iddObjectType());
  }

  CoilHeatingElectric_Impl::CoilHeatingElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
                                                       : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingElectric::iddObjectType());
  }

  CoilHeatingElectric_Impl::CoilHeatingElectric_Impl(const CoilHeatingElectric_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
                                                       : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingElectric_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingElectric_Impl::iddObjectType() const {
    return CoilHeatingElectric::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_ElectricFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingElectric","Availability"));
    }
    return result;
  }

  double CoilHeatingElectric_Impl::efficiency() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_ElectricFields::Efficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingElectric_Impl::isEfficiencyDefaulted() const {
    return isEmpty(OS_Coil_Heating_ElectricFields::Efficiency);
  }

  boost::optional<double> CoilHeatingElectric_Impl::nominalCapacity() const {
    return getDouble(OS_Coil_Heating_ElectricFields::NominalCapacity,true);
  }

  bool CoilHeatingElectric_Impl::isNominalCapacityDefaulted() const {
    return isEmpty(OS_Coil_Heating_ElectricFields::NominalCapacity);
  }

  bool CoilHeatingElectric_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_ElectricFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<Node> CoilHeatingElectric_Impl::temperatureSetpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Coil_Heating_ElectricFields::TemperatureSetpointNodeName);
  }

  bool CoilHeatingElectric_Impl::setEfficiency(double efficiency) {
    bool result = setDouble(OS_Coil_Heating_ElectricFields::Efficiency, efficiency);
    return result;
  }

  void CoilHeatingElectric_Impl::resetEfficiency() {
    bool result = setString(OS_Coil_Heating_ElectricFields::Efficiency, "");
    OS_ASSERT(result);
  }

  void CoilHeatingElectric_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result = false;
    if (nominalCapacity) {
      result = setDouble(OS_Coil_Heating_ElectricFields::NominalCapacity, nominalCapacity.get());
    } else {
      result = setString(OS_Coil_Heating_ElectricFields::NominalCapacity, "");
    }
    OS_ASSERT(result);
  }

  void CoilHeatingElectric_Impl::resetNominalCapacity() {
    bool result = setString(OS_Coil_Heating_ElectricFields::NominalCapacity, "");
    OS_ASSERT(result);
  }

  void CoilHeatingElectric_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Coil_Heating_ElectricFields::NominalCapacity, "Autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingElectric_Impl::setTemperatureSetpointNode(Node & temperatureSetpointNode) {
    setPointer(OS_Coil_Heating_ElectricFields::TemperatureSetpointNodeName, temperatureSetpointNode.handle());
  }

  void CoilHeatingElectric_Impl::resetTemperatureSetpointNode() {
    bool result = setString(OS_Coil_Heating_ElectricFields::TemperatureSetpointNodeName, "");
    OS_ASSERT(result);
  }

  unsigned CoilHeatingElectric_Impl::inletPort()
  {
    return OS_Coil_Heating_ElectricFields::AirInletNodeName;
  }

  unsigned CoilHeatingElectric_Impl::outletPort()
  {
    return OS_Coil_Heating_ElectricFields::AirOutletNodeName;
  }

  Schedule CoilHeatingElectric_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_ElectricFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilHeatingElectric_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_ElectricFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_ElectricFields::AvailabilityScheduleName,
                              "CoilHeatingElectric",
                              "Availability",
                              schedule);
    return result;
  }

  bool CoilHeatingElectric_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode( node ) )
        {
          if( boost::optional<Node> node = outletModelObject()->optionalCast<Node>() )
          {
            setTemperatureSetpointNode(node.get());
          }
          return true;
        }
      }
    }

    return false;
  }

  boost::optional<HVACComponent> CoilHeatingElectric_Impl::containingHVACComponent() const
  {
    // Process all types that might contain a CoilHeatingWater object.

    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
      if( boost::optional<HVACComponent> suppHeatingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil() )
      {
        if( suppHeatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = bypassSystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirTerminalSingleDuctVAVReheat

    std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects;

    airTerminalSingleDuctVAVReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctVAVReheat>();

    for( const auto & airTerminalSingleDuctVAVReheatObject : airTerminalSingleDuctVAVReheatObjects )
    {
      if( boost::optional<HVACComponent> coil = airTerminalSingleDuctVAVReheatObject.reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airTerminalSingleDuctVAVReheatObject;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( const auto & airTerminalSingleDuctParallelPIUReheatObject : airTerminalSingleDuctParallelPIUReheatObjects )
    {
      if( boost::optional<HVACComponent> coil = airTerminalSingleDuctParallelPIUReheatObject.reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airTerminalSingleDuctParallelPIUReheatObject;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> supplementalHeatingCoil = airLoopHVACUnitaryHeatPumpAirToAir.supplementalHeatingCoil() )
      {
        if( supplementalHeatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for( const auto & system : systems ) {
        auto heatingCoil = system.heatingCoil();
        if( heatingCoil.handle() == this->handle() ) {
          return system;
        }
        auto supHeatingCoil = system.supplementalHeatingCoil();
        if( supHeatingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingElectric_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( const auto & zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalHeatPump;
        }
      }
    }

   // ZoneHVACWaterToAirHeatPump
    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( const auto & zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACWaterToAirHeatPump;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ModelObject> CoilHeatingElectric_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilHeatingElectric_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

} // detail

CoilHeatingElectric::CoilHeatingElectric(const Model& model, Schedule & schedule )
  : StraightComponent(CoilHeatingElectric::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingElectric_Impl>());

  setAvailabilitySchedule(schedule);
}

CoilHeatingElectric::CoilHeatingElectric(const Model& model)
  : StraightComponent(CoilHeatingElectric::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingElectric_Impl>());

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);
}

IddObjectType CoilHeatingElectric::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Heating_Electric);
  return result;
}

double CoilHeatingElectric::efficiency() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->efficiency();
}

bool CoilHeatingElectric::isEfficiencyDefaulted() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->isEfficiencyDefaulted();
}

boost::optional<double> CoilHeatingElectric::nominalCapacity() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->nominalCapacity();
}

bool CoilHeatingElectric::isNominalCapacityDefaulted() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->isNominalCapacityDefaulted();
}

bool CoilHeatingElectric::isNominalCapacityAutosized() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->isNominalCapacityAutosized();
}

boost::optional<Node> CoilHeatingElectric::temperatureSetpointNode() const {
  return getImpl<detail::CoilHeatingElectric_Impl>()->temperatureSetpointNode();
}

bool CoilHeatingElectric::setEfficiency(double efficiency) {
  return getImpl<detail::CoilHeatingElectric_Impl>()->setEfficiency(efficiency);
}

void CoilHeatingElectric::resetEfficiency() {
  getImpl<detail::CoilHeatingElectric_Impl>()->resetEfficiency();
}

void CoilHeatingElectric::setNominalCapacity(double nominalCapacity) {
  getImpl<detail::CoilHeatingElectric_Impl>()->setNominalCapacity(nominalCapacity);
}

void CoilHeatingElectric::resetNominalCapacity() {
  getImpl<detail::CoilHeatingElectric_Impl>()->resetNominalCapacity();
}

void CoilHeatingElectric::autosizeNominalCapacity() {
  getImpl<detail::CoilHeatingElectric_Impl>()->autosizeNominalCapacity();
}

void CoilHeatingElectric::setTemperatureSetpointNode(Node & temperatureSetpointNode) {
  getImpl<detail::CoilHeatingElectric_Impl>()->setTemperatureSetpointNode(temperatureSetpointNode);
}

void CoilHeatingElectric::resetTemperatureSetpointNode() {
  getImpl<detail::CoilHeatingElectric_Impl>()->resetTemperatureSetpointNode();
}

Schedule CoilHeatingElectric::availabilitySchedule() const
{
  return getImpl<detail::CoilHeatingElectric_Impl>()->availabilitySchedule();
}

bool CoilHeatingElectric::setAvailabilitySchedule( Schedule & schedule )
{
  return getImpl<detail::CoilHeatingElectric_Impl>()->setAvailabilitySchedule(schedule);
}

/// @cond
CoilHeatingElectric::CoilHeatingElectric(std::shared_ptr<detail::CoilHeatingElectric_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model

} // openstudio

