/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirflowNetworkFan.hpp"
#include "AirflowNetworkFan_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

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
    static std::vector<std::string> results{"Fan Electric Power", "Fan Rise in Air Temperature", "Fan Heat Gain to Air", "Fan Electric Energy", "Fan Air Mass Flow Rate"};
    return results;
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

  unsigned FanConstantVolume_Impl::inletPort() const
  {
    return OS_Fan_ConstantVolumeFields::AirInletNodeName;
  }

  unsigned FanConstantVolume_Impl::outletPort() const
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


  double FanConstantVolume_Impl::fanTotalEfficiency() const
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::FanTotalEfficiency,true).get();
  }

  bool FanConstantVolume_Impl::setFanTotalEfficiency(double val)
  {
    return this->setDouble(OS_Fan_ConstantVolumeFields::FanTotalEfficiency,val);
  }

  double FanConstantVolume_Impl::pressureRise() const
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::PressureRise,true).get();
  }

  bool FanConstantVolume_Impl::setPressureRise(double val)
  {
    return this->setDouble(OS_Fan_ConstantVolumeFields::PressureRise,val);
  }

  double FanConstantVolume_Impl::motorEfficiency() const
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::MotorEfficiency,true).get();
  }

  bool FanConstantVolume_Impl::setMotorEfficiency(double val)
  {
    return this->setDouble(OS_Fan_ConstantVolumeFields::MotorEfficiency,val);
  }

  double FanConstantVolume_Impl::motorInAirstreamFraction() const
  {
    return this->getDouble(OS_Fan_ConstantVolumeFields::MotorInAirstreamFraction,true).get();
  }

  bool FanConstantVolume_Impl::setMotorInAirstreamFraction(double val)
  {
    return this->setDouble(OS_Fan_ConstantVolumeFields::MotorInAirstreamFraction,val);
  }

  std::string FanConstantVolume_Impl::endUseSubcategory() const
  {
    return this->getString(OS_Fan_ConstantVolumeFields::EndUseSubcategory).get();
  }

  bool FanConstantVolume_Impl::setEndUseSubcategory(std::string val)
  {
    return this->setString(OS_Fan_ConstantVolumeFields::EndUseSubcategory,val);
  }

  bool FanConstantVolume_Impl::addToNode(Node & node)
  {
    auto oaSystem = node.airLoopHVACOutdoorAirSystem();
    auto airLoop = node.airLoopHVAC();

    if( (airLoop && airLoop->supplyComponent(node.handle())) || (oaSystem && oaSystem->component(node.handle())) ) {
      if( StraightComponent_Impl::addToNode(node) ) {
        if ( airLoop ) {
          SetpointManagerMixedAir::updateFanInletOutletNodes(airLoop.get());
        }
        return true;
      }
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

    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> fan = bypassSystem.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( const auto & airTerminalSingleDuctParallelPIUReheatObject : airTerminalSingleDuctParallelPIUReheatObjects )
    {
      if( boost::optional<HVACComponent> fan = airTerminalSingleDuctParallelPIUReheatObject.fan() )
      {
        if( fan->handle() == this->handle() )
        {
          return airTerminalSingleDuctParallelPIUReheatObject;
        }
      }
    }

    // AirTerminalSingleDuctSeriesPIUReheat

    std::vector<AirTerminalSingleDuctSeriesPIUReheat> airTerminalSingleDuctSeriesPIUReheatObjects;

    airTerminalSingleDuctSeriesPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctSeriesPIUReheat>();

    for( const auto & airTerminalSingleDuctSeriesPIUReheatObject : airTerminalSingleDuctSeriesPIUReheatObjects )
    {
      if( boost::optional<HVACComponent> fan = airTerminalSingleDuctSeriesPIUReheatObject.fan() )
      {
        if( fan->handle() == this->handle() )
        {
          return airTerminalSingleDuctSeriesPIUReheatObject;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> fan = airLoopHVACUnitaryHeatPumpAirToAir.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> FanConstantVolume_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACFourPipeFanCoil

    std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

    zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

    for( const auto & zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils )
    {
      if( boost::optional<HVACComponent> fan = zoneHVACFourPipeFanCoil.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return zoneHVACFourPipeFanCoil;
        }
      }
    }

    // ZoneHVACPackagedTerminalAirConditioner

    std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners;

    zoneHVACPackagedTerminalAirConditioners = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

    for( const auto & zoneHVACPackagedTerminalAirConditioner : zoneHVACPackagedTerminalAirConditioners )
    {
      if( boost::optional<HVACComponent> fan = zoneHVACPackagedTerminalAirConditioner.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalAirConditioner;
        }
      }
    }

    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( const auto & zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps )
    {
      if( boost::optional<HVACComponent> fan = zoneHVACPackagedTerminalHeatPump.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalHeatPump;
        }
      }
    }

    // ZoneHVACUnitHeater

    std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

    zoneHVACUnitHeater = this->model().getConcreteModelObjects<ZoneHVACUnitHeater>();

    for( const auto & elem : zoneHVACUnitHeater )
    {
      if( boost::optional<HVACComponent> fan = elem.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return elem;
        }
      }
    }

    // ZoneHVACUnitVentilator

    std::vector<ZoneHVACUnitVentilator> zoneHVACUnitVentilator;

    zoneHVACUnitVentilator = this->model().getConcreteModelObjects<ZoneHVACUnitVentilator>();

    for( const auto & elem : zoneHVACUnitVentilator )
    {
      if( boost::optional<HVACComponent> fan = elem.supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return elem;
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

  AirflowNetworkFan FanConstantVolume_Impl::getAirflowNetworkFan()
  {
    auto opt = airflowNetworkFan();
    if (opt) {
      return opt.get();
    }
    return AirflowNetworkFan(model(), handle());
  }

  boost::optional<AirflowNetworkFan> FanConstantVolume_Impl::airflowNetworkFan() const
  {
    std::vector<AirflowNetworkFan> myAFNitems = getObject<ModelObject>().getModelObjectSources<AirflowNetworkFan>(AirflowNetworkFan::iddObjectType());
    auto count = myAFNitems.size();
    if (count == 1) {
      return myAFNitems[0];
    } else if (count > 1) {
      LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
      return myAFNitems[0];
    }
    return boost::none;
  }

  boost::optional<double> FanConstantVolume_Impl::autosizedMaximumFlowRate() const {
    return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
  }

  void FanConstantVolume_Impl::autosize() {
    autosizeMaximumFlowRate();
  }

  void FanConstantVolume_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedMaximumFlowRate();
    if (val) {
      setMaximumFlowRate(val.get());
    }

  }

  std::vector<EMSActuatorNames> FanConstantVolume_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{{"Fan","Fan Air Mass Flow Rate"},
                                            {"Fan","Fan Pressure Rise"},
                                            {"Fan","Fan Total Efficiency"},
                                            {"Fan","Fan Autosized Air Flow Rate"}};
    return actuators;
  }

  std::vector<std::string> FanConstantVolume_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types{"Fan Maximum Mass Flow Rate",
                                   "Fan Nominal Pressure Rise",
                                   "Fan Nominal Total Efficiency"};
    return types;
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

FanConstantVolume::FanConstantVolume(const Model& model)
  : StraightComponent(FanConstantVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanConstantVolume_Impl>());
  setString(openstudio::OS_Fan_ConstantVolumeFields::MaximumFlowRate,"AutoSize");
  auto s = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(s);
  setEndUseSubcategory("");
}

FanConstantVolume::FanConstantVolume(std::shared_ptr<detail::FanConstantVolume_Impl> p)
  : StraightComponent(std::move(p))
{}

Schedule FanConstantVolume::availabilitySchedule() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->availabilitySchedule();
}
bool FanConstantVolume::setAvailabilitySchedule(Schedule& s)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setAvailabilitySchedule(s);
}

double FanConstantVolume::fanTotalEfficiency() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->fanTotalEfficiency();
}

bool FanConstantVolume::setFanTotalEfficiency(double val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setFanTotalEfficiency(val);
}

double FanConstantVolume::fanEfficiency() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->fanTotalEfficiency();
}

bool FanConstantVolume::setFanEfficiency(double val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setFanTotalEfficiency(val);
}

double FanConstantVolume::pressureRise() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->pressureRise();
}

bool FanConstantVolume::setPressureRise(double val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setPressureRise(val);
}

double FanConstantVolume::motorEfficiency() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->motorEfficiency();
}

bool FanConstantVolume::setMotorEfficiency(double val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setMotorEfficiency(val);
}

double FanConstantVolume::motorInAirstreamFraction() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->motorInAirstreamFraction();
}

bool FanConstantVolume::setMotorInAirstreamFraction(double val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setMotorInAirstreamFraction(val);
}

std::string FanConstantVolume::endUseSubcategory() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->endUseSubcategory();
}

bool FanConstantVolume::setEndUseSubcategory(std::string val)
{
  return getImpl<detail::FanConstantVolume_Impl>()->setEndUseSubcategory(val);
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

AirflowNetworkFan FanConstantVolume::getAirflowNetworkFan()
{
  return getImpl<detail::FanConstantVolume_Impl>()->getAirflowNetworkFan();
}

boost::optional<AirflowNetworkFan> FanConstantVolume::airflowNetworkFan() const
{
  return getImpl<detail::FanConstantVolume_Impl>()->airflowNetworkFan();
}

boost::optional<double> FanConstantVolume::autosizedMaximumFlowRate() const {
  return getImpl<detail::FanConstantVolume_Impl>()->autosizedMaximumFlowRate();
}
} // model
} // openstudio
