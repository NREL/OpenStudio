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

#include <model/Model.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingElectric_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <utilities/idd/OS_Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

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
    if( StraightComponent_Impl::addToNode(node) )
    {
      if( boost::optional<Node> node = this->outletModelObject()->optionalCast<Node>() )
      {
        setTemperatureSetpointNode(node.get());
      }

      return true;
    }
    else
    {
      return false;
    }
  }

  boost::optional<HVACComponent> CoilHeatingElectric_Impl::containingHVACComponent() const
  {
    // Process all types that might contain a CoilHeatingWater object.

    // AirTerminalSingleDuctVAVReheat

    std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects;

    airTerminalSingleDuctVAVReheatObjects = this->model().getModelObjects<AirTerminalSingleDuctVAVReheat>();

    for( std::vector<AirTerminalSingleDuctVAVReheat>::iterator it = airTerminalSingleDuctVAVReheatObjects.begin();
    it < airTerminalSingleDuctVAVReheatObjects.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( std::vector<AirTerminalSingleDuctParallelPIUReheat>::iterator it = airTerminalSingleDuctParallelPIUReheatObjects.begin();
    it < airTerminalSingleDuctParallelPIUReheatObjects.end();
    ++it  )
    {
      if( boost::optional<HVACComponent> coil = it->reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( std::vector<AirLoopHVACUnitaryHeatPumpAirToAir>::iterator it = airLoopHVACUnitaryHeatPumpAirToAirs.begin();
    it < airLoopHVACUnitaryHeatPumpAirToAirs.end();
    ++it )
    {
      if( boost::optional<HVACComponent> supplementalHeatingCoil = it->supplementalHeatingCoil() )
      {
        if( supplementalHeatingCoil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingElectric_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( std::vector<ZoneHVACPackagedTerminalHeatPump>::iterator it = zoneHVACPackagedTerminalHeatPumps.begin();
    it < zoneHVACPackagedTerminalHeatPumps.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

   // ZoneHVACWaterToAirHeatPump
    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( std::vector<ZoneHVACWaterToAirHeatPump>::iterator it = zoneHVACWaterToAirHeatPumps.begin();
    it < zoneHVACWaterToAirHeatPumps.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
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
CoilHeatingElectric::CoilHeatingElectric(boost::shared_ptr<detail::CoilHeatingElectric_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model

} // openstudio

