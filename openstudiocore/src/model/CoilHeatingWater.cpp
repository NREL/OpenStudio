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

#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail{

  CoilHeatingWater_Impl::CoilHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : WaterToAirComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWater::iddObjectType());
  }

  CoilHeatingWater_Impl::CoilHeatingWater_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWater::iddObjectType());
  }

  CoilHeatingWater_Impl::CoilHeatingWater_Impl(const CoilHeatingWater_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
                                                 : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  CoilHeatingWater_Impl::~CoilHeatingWater_Impl(){}

  bool CoilHeatingWater_Impl::addToNode(Node & node)
  {
    bool success =  WaterToAirComponent_Impl::addToNode( node );

    if( success && (! containingHVACComponent()) && (! containingZoneHVACComponent()) ) {
      if( boost::optional<ModelObject> _waterInletModelObject = waterInletModelObject() ) {
        if( auto oldController = controllerWaterCoil() ) {
          oldController->remove();
        }

        Model _model = model();
        ControllerWaterCoil controller(_model);
        controller.getImpl<detail::ControllerWaterCoil_Impl>()->setWaterCoil(getObject<HVACComponent>());
        controller.setAction("Normal");
      }
    }

    return success;
  }

  bool CoilHeatingWater_Impl::removeFromPlantLoop()
  {
    if( boost::optional<ControllerWaterCoil> controller = this->controllerWaterCoil() )
    {
      controller->remove();
    }

    return WaterToAirComponent_Impl::removeFromPlantLoop();
  }

  std::vector<openstudio::IdfObject> CoilHeatingWater_Impl::remove()
  {
    if( isRemovable() )
    {
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  ModelObject CoilHeatingWater_Impl::clone(Model model) const
  {
    CoilHeatingWater newCoil = WaterToAirComponent_Impl::clone(model).optionalCast<CoilHeatingWater>().get();

    return newCoil;
  }

  IddObjectType CoilHeatingWater_Impl::iddObjectType() const {
    return CoilHeatingWater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_WaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingWater","Availability"));
    }
    return result;
  }

  Schedule CoilHeatingWater_Impl::availabilitySchedule() const {
    OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_WaterFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilHeatingWater_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_WaterFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_WaterFields::AvailabilityScheduleName,
                              "CoilHeatingWater",
                              "Availability",
                              schedule);
    return result;
  }

  boost::optional<double> CoilHeatingWater_Impl::uFactorTimesAreaValue()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::UFactorTimesAreaValue );
  }

  void CoilHeatingWater_Impl::setUFactorTimesAreaValue( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, value );
  }

  bool CoilHeatingWater_Impl::isUFactorTimesAreaValueAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilHeatingWater_Impl::autosizeUFactorTimesAreaValue()
  {
    setString(OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, "Autosize");
  }

  boost::optional<double> CoilHeatingWater_Impl::maximumWaterFlowRate()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::MaximumWaterFlowRate );
  }

  void CoilHeatingWater_Impl::setMaximumWaterFlowRate( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, value );
  }

  bool CoilHeatingWater_Impl::isMaximumWaterFlowRateAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilHeatingWater_Impl::autosizeMaximumWaterFlowRate()
  {
    setString(OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, "Autosize");
  }

  std::string CoilHeatingWater_Impl::performanceInputMethod()
  {
    return getString( openstudio::OS_Coil_Heating_WaterFields::PerformanceInputMethod,true ).get();
  }

  void CoilHeatingWater_Impl::setPerformanceInputMethod( std::string value )
  {
    setString( openstudio::OS_Coil_Heating_WaterFields::PerformanceInputMethod, value );
  }

  boost::optional<double> CoilHeatingWater_Impl::ratedCapacity()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedCapacity );
  }

  void CoilHeatingWater_Impl::setRatedCapacity( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedCapacity, value );
  }

  bool CoilHeatingWater_Impl::isRatedCapacityAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::RatedCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilHeatingWater_Impl::autosizeRatedCapacity()
  {
    setString(OS_Coil_Heating_WaterFields::RatedCapacity, "Autosize");
  }

  double CoilHeatingWater_Impl::ratedInletWaterTemperature()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedInletWaterTemperature,true ).get();
  }

  void CoilHeatingWater_Impl::setRatedInletWaterTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedInletWaterTemperature, value );
  }

  double CoilHeatingWater_Impl::ratedInletAirTemperature()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedInletAirTemperature,true ).get();
  }

  void CoilHeatingWater_Impl::setRatedInletAirTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedInletAirTemperature, value );
  }

  double CoilHeatingWater_Impl::ratedOutletWaterTemperature()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedOutletWaterTemperature,true ).get();
  }

  void CoilHeatingWater_Impl::setRatedOutletWaterTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedOutletWaterTemperature, value );
  }

  double CoilHeatingWater_Impl::ratedOutletAirTemperature()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedOutletAirTemperature,true ).get();
  }

  void CoilHeatingWater_Impl::setRatedOutletAirTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedOutletAirTemperature, value );
  }

  double CoilHeatingWater_Impl::ratedRatioForAirAndWaterConvection()
  {
    return getDouble( openstudio::OS_Coil_Heating_WaterFields::RatedRatioforAirandWaterConvection,true ).get();
  }

  void CoilHeatingWater_Impl::setRatedRatioForAirAndWaterConvection( double value )
  {
    setDouble( openstudio::OS_Coil_Heating_WaterFields::RatedRatioforAirandWaterConvection, value );
  }

  unsigned CoilHeatingWater_Impl::airInletPort()
  {
    return OS_Coil_Heating_WaterFields::AirInletNodeName;
  }

  unsigned CoilHeatingWater_Impl::airOutletPort()
  {
    return OS_Coil_Heating_WaterFields::AirOutletNodeName;
  }

  unsigned CoilHeatingWater_Impl::waterInletPort()
  {
    return OS_Coil_Heating_WaterFields::WaterInletNodeName;
  }

  unsigned CoilHeatingWater_Impl::waterOutletPort()
  {
    return OS_Coil_Heating_WaterFields::WaterOutletNodeName;
  }

  boost::optional<HVACComponent> CoilHeatingWater_Impl::containingHVACComponent() const
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

    std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctVAVReheat>();

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

    // AirTerminalSingleDuctConstantVolumeReheat

    std::vector<AirTerminalSingleDuctConstantVolumeReheat> airTerminalSingleDuctConstantVolumeReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeReheat>();

    for( const auto & airTerminalSingleDuctConstantVolumeReheatObject : airTerminalSingleDuctConstantVolumeReheatObjects )
    {
      if( boost::optional<HVACComponent> coil = airTerminalSingleDuctConstantVolumeReheatObject.reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airTerminalSingleDuctConstantVolumeReheatObject;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

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

  boost::optional<ZoneHVACComponent> CoilHeatingWater_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACFourPipeFanCoil

    std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

    zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

    for( const auto & zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
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
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalAirConditioner.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
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

    // ZoneHVACUnitHeater

    std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

    zoneHVACUnitHeater = this->model().getConcreteModelObjects<ZoneHVACUnitHeater>();

    for( const auto & elem : zoneHVACUnitHeater )
    {
      if( boost::optional<HVACComponent> coil = elem.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
     }

    return boost::none;
  }

  boost::optional<ModelObject> CoilHeatingWater_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilHeatingWater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

CoilHeatingWater::CoilHeatingWater(const Model& model, Schedule & availableSchedule)
  : WaterToAirComponent(CoilHeatingWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWater_Impl>());

  setAvailableSchedule( availableSchedule );
}

CoilHeatingWater::CoilHeatingWater(const Model& model)
  : WaterToAirComponent(CoilHeatingWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWater_Impl>());

  auto availableSchedule = model.alwaysOnDiscreteSchedule();
  setAvailableSchedule(availableSchedule);
}

CoilHeatingWater::CoilHeatingWater(std::shared_ptr<detail::CoilHeatingWater_Impl> p)
  : WaterToAirComponent(std::move(p))
{}

Schedule CoilHeatingWater::availabilitySchedule() const
{
  return getImpl<detail::CoilHeatingWater_Impl>()->availabilitySchedule();
}

Schedule CoilHeatingWater::availableSchedule() const
{
  return getImpl<detail::CoilHeatingWater_Impl>()->availabilitySchedule();
}

bool CoilHeatingWater::setAvailabilitySchedule(Schedule & schedule )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setAvailabilitySchedule( schedule );
}

bool CoilHeatingWater::setAvailableSchedule(Schedule & schedule )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setAvailabilitySchedule( schedule );
}

boost::optional<double> CoilHeatingWater::uFactorTimesAreaValue()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->uFactorTimesAreaValue();
}

void CoilHeatingWater::setUFactorTimesAreaValue( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setUFactorTimesAreaValue( value );
}

bool CoilHeatingWater::isUFactorTimesAreaValueAutosized()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->isUFactorTimesAreaValueAutosized();
}

void CoilHeatingWater::autosizeUFactorTimesAreaValue()
{
  getImpl<detail::CoilHeatingWater_Impl>()->autosizeUFactorTimesAreaValue();
}

boost::optional<double> CoilHeatingWater::maximumWaterFlowRate()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->maximumWaterFlowRate();
}

void CoilHeatingWater::setMaximumWaterFlowRate( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setMaximumWaterFlowRate( value );
}

bool CoilHeatingWater::isMaximumWaterFlowRateAutosized()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->isMaximumWaterFlowRateAutosized();
}

void CoilHeatingWater::autosizeMaximumWaterFlowRate()
{
  getImpl<detail::CoilHeatingWater_Impl>()->autosizeMaximumWaterFlowRate();
}

std::string CoilHeatingWater::performanceInputMethod()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->performanceInputMethod();
}

void CoilHeatingWater::setPerformanceInputMethod( std::string value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setPerformanceInputMethod( value );
}

boost::optional<double> CoilHeatingWater::ratedCapacity()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedCapacity();
}

void CoilHeatingWater::setRatedCapacity( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedCapacity( value );
}

bool CoilHeatingWater::isRatedCapacityAutosized()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->isRatedCapacityAutosized();
}

void CoilHeatingWater::autosizeRatedCapacity()
{
  getImpl<detail::CoilHeatingWater_Impl>()->autosizeRatedCapacity();
}

double CoilHeatingWater::ratedInletWaterTemperature()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedInletWaterTemperature();
}

void CoilHeatingWater::setRatedInletWaterTemperature( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedInletWaterTemperature( value );
}

double CoilHeatingWater::ratedInletAirTemperature()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedInletAirTemperature();
}

void CoilHeatingWater::setRatedInletAirTemperature( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedInletAirTemperature( value );
}

double CoilHeatingWater::ratedOutletWaterTemperature()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedOutletWaterTemperature();
}

void CoilHeatingWater::setRatedOutletWaterTemperature( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedOutletWaterTemperature( value );
}

double CoilHeatingWater::ratedOutletAirTemperature()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedOutletAirTemperature();
}

void CoilHeatingWater::setRatedOutletAirTemperature( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedOutletAirTemperature( value );
}

double CoilHeatingWater::ratedRatioForAirAndWaterConvection()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->ratedRatioForAirAndWaterConvection();
}

void CoilHeatingWater::setRatedRatioForAirAndWaterConvection( double value )
{
  return getImpl<detail::CoilHeatingWater_Impl>()->setRatedRatioForAirAndWaterConvection( value );
}

IddObjectType CoilHeatingWater::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Heating_Water);
  return result;
}

boost::optional<ControllerWaterCoil> CoilHeatingWater::controllerWaterCoil()
{
  return getImpl<detail::CoilHeatingWater_Impl>()->controllerWaterCoil();
}

} // model
} // openstudio

