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

#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include <utilities/idd/OS_Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWater_Impl::CoilCoolingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : WaterToAirComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWater::iddObjectType());
  }

  CoilCoolingWater_Impl::CoilCoolingWater_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingWater::iddObjectType());
  }

  CoilCoolingWater_Impl::CoilCoolingWater_Impl(const CoilCoolingWater_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
                                                 : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  CoilCoolingWater_Impl::~CoilCoolingWater_Impl(){}

  IddObjectType CoilCoolingWater_Impl::iddObjectType() const {
    return CoilCoolingWater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_WaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingWater","Availability"));
    }
    return result;
  }

  Schedule CoilCoolingWater_Impl::availabilitySchedule() const {
    OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_WaterFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilCoolingWater_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_WaterFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_WaterFields::AvailabilityScheduleName,
                              "CoilCoolingWater",
                              "Availability",
                              schedule);
    return result;
  }

  boost::optional<double> CoilCoolingWater_Impl::designWaterFlowRate()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignWaterFlowRate );
  }

  void CoilCoolingWater_Impl::setDesignWaterFlowRate( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignWaterFlowRate, value );
  }

  bool CoilCoolingWater_Impl::isDesignWaterFlowRateAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignWaterFlowRate()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignWaterFlowRate, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designAirFlowRate()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignAirFlowRate );
  }

  void CoilCoolingWater_Impl::setDesignAirFlowRate( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignAirFlowRate, value );
  }

  bool CoilCoolingWater_Impl::isDesignAirFlowRateAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignAirFlowRate()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignAirFlowRate, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designInletWaterTemperature()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletWaterTemperature );
  }

  void CoilCoolingWater_Impl::setDesignInletWaterTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletWaterTemperature, value );
  }

  bool CoilCoolingWater_Impl::isDesignInletWaterTemperatureAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignInletWaterTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignInletWaterTemperature()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignInletWaterTemperature, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designInletAirTemperature()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletAirTemperature );
  }

  void CoilCoolingWater_Impl::setDesignInletAirTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletAirTemperature, value );
  }

  bool CoilCoolingWater_Impl::isDesignInletAirTemperatureAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignInletAirTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignInletAirTemperature()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignInletAirTemperature, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designOutletAirTemperature()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignOutletAirTemperature );
  }

  void CoilCoolingWater_Impl::setDesignOutletAirTemperature( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignOutletAirTemperature, value );
  }

  bool CoilCoolingWater_Impl::isDesignOutletAirTemperatureAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignOutletAirTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignOutletAirTemperature()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignOutletAirTemperature, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designInletAirHumidityRatio()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletAirHumidityRatio );
  }

  void CoilCoolingWater_Impl::setDesignInletAirHumidityRatio( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignInletAirHumidityRatio, value );
  }

  bool CoilCoolingWater_Impl::isDesignInletAirHumidityRatioAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignInletAirHumidityRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignInletAirHumidityRatio()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignInletAirHumidityRatio, "Autosize");
  }

  boost::optional<double> CoilCoolingWater_Impl::designOutletAirHumidityRatio()
  {
    return getDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio );
  }

  void CoilCoolingWater_Impl::setDesignOutletAirHumidityRatio( double value )
  {
    setDouble( openstudio::OS_Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio, value );
  }

  bool CoilCoolingWater_Impl::isDesignOutletAirHumidityRatioAutosized()
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  void CoilCoolingWater_Impl::autosizeDesignOutletAirHumidityRatio()
  {
    setString(OS_Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio, "Autosize");
  }

  std::string CoilCoolingWater_Impl::typeOfAnalysis()
  {
    return getString( openstudio::OS_Coil_Cooling_WaterFields::TypeofAnalysis,true ).get();
  }

  void CoilCoolingWater_Impl::setTypeOfAnalysis( std::string value )
  {
    setString( openstudio::OS_Coil_Cooling_WaterFields::TypeofAnalysis, value );
  }

  std::string CoilCoolingWater_Impl::heatExchangerConfiguration()
  {
    return getString( openstudio::OS_Coil_Cooling_WaterFields::HeatExchangerConfiguration,true ).get();
  }

  void CoilCoolingWater_Impl::setHeatExchangerConfiguration( std::string value )
  {
    setString( openstudio::OS_Coil_Cooling_WaterFields::HeatExchangerConfiguration, value );
  }

  bool CoilCoolingWater_Impl::addToNode(Node & node)
  {
    bool success;

    success =  WaterToAirComponent_Impl::addToNode( node );
    auto t_containingZoneHVACComponent = containingZoneHVACComponent();

    if( success && (! t_containingZoneHVACComponent) ) {
      if( auto t_waterInletModelObject = waterInletModelObject() ) {
        if( auto oldController = controllerWaterCoil() ) {
          oldController->remove();
        }

        Model t_model = model();
        ControllerWaterCoil controller(t_model);
        controller.getImpl<detail::ControllerWaterCoil_Impl>()->setWaterCoil(getObject<HVACComponent>());
        controller.setAction("Reverse");
      }
    }

    return success;
  }

  bool CoilCoolingWater_Impl::removeFromPlantLoop()
  {
    if( boost::optional<ControllerWaterCoil> controller = this->controllerWaterCoil() )
    {
      controller->remove();
    }

    return WaterToAirComponent_Impl::removeFromPlantLoop();
  }

  std::vector<openstudio::IdfObject> CoilCoolingWater_Impl::remove()
  {
    if( isRemovable() )
    {
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  ModelObject CoilCoolingWater_Impl::clone(Model model) const
  {
    CoilCoolingWater newCoil = WaterToAirComponent_Impl::clone( model ).optionalCast<CoilCoolingWater>().get();

    return newCoil;
  }

  unsigned CoilCoolingWater_Impl::airInletPort()
  {
    return OS_Coil_Cooling_WaterFields::AirInletNodeName;
  }

  unsigned CoilCoolingWater_Impl::airOutletPort()
  {
    return OS_Coil_Cooling_WaterFields::AirOutletNodeName;
  }

  unsigned CoilCoolingWater_Impl::waterInletPort()
  {
    return OS_Coil_Cooling_WaterFields::WaterInletNodeName;
  }

  unsigned CoilCoolingWater_Impl::waterOutletPort()
  {
    return OS_Coil_Cooling_WaterFields::WaterOutletNodeName;
  }

  boost::optional<ModelObject> CoilCoolingWater_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilCoolingWater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  boost::optional<HVACComponent> CoilCoolingWater_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // CoilSystemCoolingWaterHeatExchangerAssisted
    {
      auto coilSystems = model().getConcreteModelObjects<CoilSystemCoolingWaterHeatExchangerAssisted>();
      for( const auto & coilSystem : coilSystems ) {
        if( coilSystem.coolingCoil().handle() == handle() ) {
          return coilSystem;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingWater_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACFourPipeFanCoil

    std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

    zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

    for( const auto & zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACFourPipeFanCoil;
        }
      }
    }
    return boost::none;
  }

} // detail

CoilCoolingWater::CoilCoolingWater(const Model& model, Schedule & availableSchedule)
  : WaterToAirComponent(CoilCoolingWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWater_Impl>());

  setAvailableSchedule( availableSchedule );
}

CoilCoolingWater::CoilCoolingWater(const Model& model)
  : WaterToAirComponent(CoilCoolingWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWater_Impl>());

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailableSchedule(schedule);
}

CoilCoolingWater::CoilCoolingWater(std::shared_ptr<detail::CoilCoolingWater_Impl> p)
  : WaterToAirComponent(std::move(p))
{}

Schedule CoilCoolingWater::availabilitySchedule() const
{
  return getImpl<detail::CoilCoolingWater_Impl>()->availabilitySchedule();
}

Schedule CoilCoolingWater::availableSchedule() const
{
  return getImpl<detail::CoilCoolingWater_Impl>()->availabilitySchedule();
}

bool CoilCoolingWater::setAvailabilitySchedule(Schedule& schedule )
{
  return getImpl<detail::CoilCoolingWater_Impl>()->setAvailabilitySchedule( schedule );
}

bool CoilCoolingWater::setAvailableSchedule(Schedule& schedule )
{
  return getImpl<detail::CoilCoolingWater_Impl>()->setAvailabilitySchedule( schedule );
}

boost::optional<double> CoilCoolingWater::designWaterFlowRate()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designWaterFlowRate();
}

void CoilCoolingWater::setDesignWaterFlowRate( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignWaterFlowRate( value );
}

bool CoilCoolingWater::isDesignWaterFlowRateAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignWaterFlowRateAutosized();
}

void CoilCoolingWater::autosizeDesignWaterFlowRate()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignWaterFlowRate();
}

boost::optional<double> CoilCoolingWater::designAirFlowRate()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designAirFlowRate();
}

void CoilCoolingWater::setDesignAirFlowRate( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignAirFlowRate( value );
}

bool CoilCoolingWater::isDesignAirFlowRateAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignAirFlowRateAutosized();
}

void CoilCoolingWater::autosizeDesignAirFlowRate()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignAirFlowRate();
}

boost::optional<double> CoilCoolingWater::designInletWaterTemperature()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designInletWaterTemperature();
}

void CoilCoolingWater::setDesignInletWaterTemperature( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignInletWaterTemperature( value );
}

bool CoilCoolingWater::isDesignInletWaterTemperatureAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignInletWaterTemperatureAutosized();
}

void CoilCoolingWater::autosizeDesignInletWaterTemperature()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignInletWaterTemperature();
}

boost::optional<double> CoilCoolingWater::designInletAirTemperature()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designInletAirTemperature();
}

void CoilCoolingWater::setDesignInletAirTemperature( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignInletAirTemperature( value );
}

bool CoilCoolingWater::isDesignInletAirTemperatureAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignInletAirTemperatureAutosized();
}

void CoilCoolingWater::autosizeDesignInletAirTemperature()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignInletAirTemperature();
}

boost::optional<double> CoilCoolingWater::designOutletAirTemperature()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designOutletAirTemperature();
}

void CoilCoolingWater::setDesignOutletAirTemperature( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignOutletAirTemperature( value );
}

bool CoilCoolingWater::isDesignOutletAirTemperatureAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignOutletAirTemperatureAutosized();
}

void CoilCoolingWater::autosizeDesignOutletAirTemperature()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignOutletAirTemperature();
}

boost::optional<double> CoilCoolingWater::designInletAirHumidityRatio()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designInletAirHumidityRatio();
}

void CoilCoolingWater::setDesignInletAirHumidityRatio( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignInletAirHumidityRatio( value );
}

bool CoilCoolingWater::isDesignInletAirHumidityRatioAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignInletAirHumidityRatioAutosized();
}

void CoilCoolingWater::autosizeDesignInletAirHumidityRatio()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignInletAirHumidityRatio();
}

boost::optional<double> CoilCoolingWater::designOutletAirHumidityRatio()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->designOutletAirHumidityRatio();
}

void CoilCoolingWater::setDesignOutletAirHumidityRatio( double value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setDesignOutletAirHumidityRatio( value );
}

bool CoilCoolingWater::isDesignOutletAirHumidityRatioAutosized()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->isDesignOutletAirHumidityRatioAutosized();
}

void CoilCoolingWater::autosizeDesignOutletAirHumidityRatio()
{
  getImpl<detail::CoilCoolingWater_Impl>()->autosizeDesignOutletAirHumidityRatio();
}

std::string CoilCoolingWater::typeOfAnalysis()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->typeOfAnalysis();
}

void CoilCoolingWater::setTypeOfAnalysis( std::string value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setTypeOfAnalysis( value );
}

std::string CoilCoolingWater::heatExchangerConfiguration()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->heatExchangerConfiguration();
}

void CoilCoolingWater::setHeatExchangerConfiguration( std::string value )
{
  getImpl<detail::CoilCoolingWater_Impl>()->setHeatExchangerConfiguration( value );
}

IddObjectType CoilCoolingWater::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Cooling_Water);
  return result;
}

boost::optional<ControllerWaterCoil> CoilCoolingWater::controllerWaterCoil()
{
  return getImpl<detail::CoilCoolingWater_Impl>()->controllerWaterCoil();
}

} // model
} // openstudio

