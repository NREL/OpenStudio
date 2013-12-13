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

#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ZoneHVACUnitHeater.hpp>
#include <model/ZoneHVACUnitHeater_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/OS_Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {

namespace model {

namespace detail {

  FanConstantVolume_Impl::FanConstantVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanConstantVolume::iddObjectType());
  }

  FanConstantVolume_Impl::FanConstantVolume_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanConstantVolume::iddObjectType());
  }

  FanConstantVolume_Impl::FanConstantVolume_Impl(const FanConstantVolume_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
                                                   : StraightComponent_Impl(other,model,keepHandle)
  {}

  FanConstantVolume_Impl::~FanConstantVolume_Impl(){}

  const std::vector<std::string>& FanConstantVolume_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanConstantVolume_Impl::iddObjectType() const {
    return FanConstantVolume::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanConstantVolume_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_ConstantVolumeFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanConstantVolume","Availability"));
    }
    return result;
  }

  unsigned FanConstantVolume_Impl::inletPort()
  {
    return OS_Fan_ConstantVolumeFields::AirInletNodeName;
  }

  unsigned FanConstantVolume_Impl::outletPort()
  {
    return OS_Fan_ConstantVolumeFields::AirOutletNodeName;
  }

  Schedule FanConstantVolume_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<FanConstantVolume_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool FanConstantVolume_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ConstantVolumeFields::AvailabilityScheduleName,
                              "FanConstantVolume",
                              "Availability",
                              schedule);
    return result;
  }


  double FanConstantVolume_Impl::fanEfficiency()
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::FanEfficiency,true).get();
  }

  void FanConstantVolume_Impl::setFanEfficiency(double val)
  {
    this->setDouble(OS_Fan_ConstantVolumeFields::FanEfficiency,val);
  }

  double FanConstantVolume_Impl::pressureRise()
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::PressureRise,true).get();
  }

  void FanConstantVolume_Impl::setPressureRise(double val)
  {
    this->setDouble(OS_Fan_ConstantVolumeFields::PressureRise,val);
  }

  double FanConstantVolume_Impl::motorEfficiency()
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::MotorEfficiency,true).get();
  }

  void FanConstantVolume_Impl::setMotorEfficiency(double val)
  {
    this->setDouble(OS_Fan_ConstantVolumeFields::MotorEfficiency,val);
  }

  double FanConstantVolume_Impl::motorInAirstreamFraction()
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::MotorInAirstreamFraction,true).get();
  }

  void FanConstantVolume_Impl::setMotorInAirstreamFraction(double val)
  {
    this->setDouble(OS_Fan_ConstantVolumeFields::MotorInAirstreamFraction,val);
  }

  std::string FanConstantVolume_Impl::endUseSubcategory()
  {
    return this->getString(OS_Fan_ConstantVolumeFields::EndUseSubcategory).get();
  }

  void FanConstantVolume_Impl::setEndUseSubcategory(std::string val)
  {
    this->setString(OS_Fan_ConstantVolumeFields::EndUseSubcategory,val);
  }

  bool FanConstantVolume_Impl::addToNode(Node & node)
  {
    if( OptionalAirLoopHVAC airLoopHVAC = node.airLoopHVAC() )
    {
      if( OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        if( oaSystem->component(node.handle()) )
        {
          return false;
        }
      }

      if( ! airLoopHVAC->supplyComponent(node.handle()) )
      {
        return false;
      }

      std::vector<ModelObject> allFans;

      std::vector<ModelObject> constantFans = airLoopHVAC->supplyComponents(IddObjectType::OS_Fan_ConstantVolume);

      std::vector<ModelObject> variableFans = airLoopHVAC->supplyComponents(IddObjectType::OS_Fan_VariableVolume);

      allFans = constantFans;
      allFans.insert(allFans.begin(),variableFans.begin(),variableFans.end());

      if( allFans.size() > 0 )
      {
        return false;
      }

      bool result = StraightComponent_Impl::addToNode( node );

      if( result )
      {
        SetpointManagerMixedAir::updateFanInletOutletNodes(airLoopHVAC.get());
      }

      return result;
    }

    return false;
  }

  std::vector<openstudio::IdfObject> FanConstantVolume_Impl::remove()
  {
    return StraightComponent_Impl::remove();
  }

  ModelObject FanConstantVolume_Impl::clone(Model model) const
  {
    return StraightComponent_Impl::clone( model );
  }

  boost::optional<Schedule> FanConstantVolume_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ConstantVolumeFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> FanConstantVolume_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool FanConstantVolume_Impl::setAvailibiltyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  boost::optional<HVACComponent> FanConstantVolume_Impl::containingHVACComponent() const
  {
    // Process all types that might contain a CoilHeatingWater object.

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( std::vector<AirTerminalSingleDuctParallelPIUReheat>::iterator it = airTerminalSingleDuctParallelPIUReheatObjects.begin();
    it < airTerminalSingleDuctParallelPIUReheatObjects.end();
    it++ )
    {
      if( boost::optional<HVACComponent> fan = it->fan() )
      {
        if( fan->handle() == this->handle() )
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
    it++ )
    {
      if( boost::optional<HVACComponent> fan = it->supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> FanConstantVolume_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACFourPipeFanCoil

    std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

    zoneHVACFourPipeFanCoils = this->model().getModelObjects<ZoneHVACFourPipeFanCoil>();

    for( std::vector<ZoneHVACFourPipeFanCoil>::iterator it = zoneHVACFourPipeFanCoils.begin();
    it < zoneHVACFourPipeFanCoils.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // ZoneHVACPackagedTerminalAirConditioner

    std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners;

    zoneHVACPackagedTerminalAirConditioners = this->model().getModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

    for( std::vector<ZoneHVACPackagedTerminalAirConditioner>::iterator it = zoneHVACPackagedTerminalAirConditioners.begin();
    it < zoneHVACPackagedTerminalAirConditioners.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( std::vector<ZoneHVACPackagedTerminalHeatPump>::iterator it = zoneHVACPackagedTerminalHeatPumps.begin();
    it < zoneHVACPackagedTerminalHeatPumps.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // ZoneHVACUnitHeater

    std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

    zoneHVACUnitHeater = this->model().getModelObjects<ZoneHVACUnitHeater>();

    for( std::vector<ZoneHVACUnitHeater>::iterator it = zoneHVACUnitHeater.begin();
    it < zoneHVACUnitHeater.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }

  boost::optional<double> FanConstantVolume_Impl::maximumFlowRate() const {
    return getDouble(OS_Fan_ConstantVolumeFields::MaximumFlowRate,true);
  }

  OSOptionalQuantity FanConstantVolume_Impl::getMaximumFlowRate(bool returnIP) const {
    OptionalDouble value = maximumFlowRate();
    return getQuantityFromDouble(OS_Fan_ConstantVolumeFields::MaximumFlowRate, value, returnIP);
  }

  bool FanConstantVolume_Impl::isMaximumFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ConstantVolumeFields::MaximumFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool FanConstantVolume_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result(false);
    if (maximumFlowRate) {
      result = setDouble(OS_Fan_ConstantVolumeFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

  bool FanConstantVolume_Impl::setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_ConstantVolumeFields::MaximumFlowRate,maximumFlowRate.get());
      if (value) {
        result = setMaximumFlowRate(value);
      }
    }
    else {
      result = setMaximumFlowRate(value);
    }
    return result;
  }

  void FanConstantVolume_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_Fan_ConstantVolumeFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  void FanConstantVolume_Impl::autosizeMaximumFlowRate() {
    bool result = setString(OS_Fan_ConstantVolumeFields::MaximumFlowRate, "autosize");
    OS_ASSERT(result);
  }

} // detail

// create a new FanConstantVolume object in the model's workspace
FanConstantVolume::FanConstantVolume(const Model& model,
                                     Schedule& s)
  : StraightComponent(FanConstantVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanConstantVolume_Impl>());
  setString(openstudio::OS_Fan_ConstantVolumeFields::MaximumFlowRate,"AutoSize");
  setAvailabilitySchedule(s);
  setEndUseSubcategory("");
}

FanConstantVolume::FanConstantVolume(boost::shared_ptr<detail::FanConstantVolume_Impl> p)
  : StraightComponent(p)
{}

Schedule FanConstantVolume::availabilitySchedule() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->availabilitySchedule();
}
bool FanConstantVolume::setAvailabilitySchedule(Schedule& s)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setAvailabilitySchedule(s);
}

double FanConstantVolume::fanEfficiency()
{
  return getImpl<detail::FanConstantVolume_Impl>()->fanEfficiency();
}

void FanConstantVolume::setFanEfficiency(double val)
{
  getImpl<detail::FanConstantVolume_Impl>()->setFanEfficiency(val);
}

double FanConstantVolume::pressureRise()
{
  return getImpl<detail::FanConstantVolume_Impl>()->pressureRise();
}

void FanConstantVolume::setPressureRise(double val)
{
  getImpl<detail::FanConstantVolume_Impl>()->setPressureRise(val);
}

double FanConstantVolume::motorEfficiency()
{
  return getImpl<detail::FanConstantVolume_Impl>()->motorEfficiency();
}

void FanConstantVolume::setMotorEfficiency(double val)
{
  getImpl<detail::FanConstantVolume_Impl>()->setMotorEfficiency(val);
}

double FanConstantVolume::motorInAirstreamFraction()
{
  return getImpl<detail::FanConstantVolume_Impl>()->motorInAirstreamFraction();
}

void FanConstantVolume::setMotorInAirstreamFraction(double val)
{
  getImpl<detail::FanConstantVolume_Impl>()->setMotorInAirstreamFraction(val);
}

std::string FanConstantVolume::endUseSubcategory()
{
  return getImpl<detail::FanConstantVolume_Impl>()->endUseSubcategory();
}

void FanConstantVolume::setEndUseSubcategory(std::string val)
{
  getImpl<detail::FanConstantVolume_Impl>()->setEndUseSubcategory(val);
}

IddObjectType FanConstantVolume::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Fan_ConstantVolume);
  return result;
}

boost::optional<double> FanConstantVolume::maximumFlowRate() const {
  return getImpl<detail::FanConstantVolume_Impl>()->maximumFlowRate();
}

OSOptionalQuantity FanConstantVolume::getMaximumFlowRate(bool returnIP) const {
  return getImpl<detail::FanConstantVolume_Impl>()->getMaximumFlowRate(returnIP);
}

bool FanConstantVolume::isMaximumFlowRateAutosized() const {
  return getImpl<detail::FanConstantVolume_Impl>()->isMaximumFlowRateAutosized();
}

bool FanConstantVolume::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::FanConstantVolume_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

bool FanConstantVolume::setMaximumFlowRate(const Quantity& maximumFlowRate) {
  return getImpl<detail::FanConstantVolume_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanConstantVolume::resetMaximumFlowRate() {
  getImpl<detail::FanConstantVolume_Impl>()->resetMaximumFlowRate();
}

void FanConstantVolume::autosizeMaximumFlowRate() {
  getImpl<detail::FanConstantVolume_Impl>()->autosizeMaximumFlowRate();
}

} // model
} // openstudio

