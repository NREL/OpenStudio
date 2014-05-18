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

#include <model/CoilCoolingWater.hpp>
#include <model/CoilCoolingWater_Impl.hpp>
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/AirLoopHVACUnitarySystem.hpp>
#include <model/AirLoopHVACUnitarySystem_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <utilities/idd/OS_Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

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
    
    if( success && (! containingHVACComponent()) && (! containingZoneHVACComponent()) )
    {
      if( boost::optional<ModelObject> _waterInletModelObject = waterInletModelObject() )
      {
        Model _model = model();

        boost::optional<ControllerWaterCoil> oldController;

        oldController = controllerWaterCoil();

        if( oldController )
        {
          oldController->remove();
        }

        ControllerWaterCoil controller(_model);

        boost::optional<Node> coilWaterInletNode = _waterInletModelObject->optionalCast<Node>();

        OS_ASSERT(coilWaterInletNode);

        controller.setActuatorNode(coilWaterInletNode.get());

        if( boost::optional<ModelObject> mo = airOutletModelObject() )
        {
          if( boost::optional<Node> coilAirOutletNode = mo->optionalCast<Node>() )
          {
            controller.setSensorNode(coilAirOutletNode.get());
          }
        }

        controller.setAction("Reverse");
      }
    }
    
    return success;
  }

  boost::optional<ControllerWaterCoil> CoilCoolingWater_Impl::controllerWaterCoil()
  {
    boost::optional<Node> coilWaterInletNode;

    if( boost::optional<ModelObject> mo = waterInletModelObject() )
    {
      coilWaterInletNode = mo->optionalCast<Node>();
    }

    if( coilWaterInletNode )
    {
      std::vector<ControllerWaterCoil> controllers = this->model().getConcreteModelObjects<ControllerWaterCoil>();

      for( std::vector<ControllerWaterCoil>::iterator it = controllers.begin();
      it < controllers.end();
      ++it )
      {
        if( it->actuatorNode() == coilWaterInletNode )
        {
          return *it;
        }
      }
    }

    return boost::none;
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

    for( std::vector<AirLoopHVACUnitarySystem>::iterator it = airLoopHVACUnitarySystems.begin();
    it < airLoopHVACUnitarySystems.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coolingCoil = it->coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return *it;
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

    for( std::vector<ZoneHVACFourPipeFanCoil>::iterator it = zoneHVACFourPipeFanCoils.begin();
    it < zoneHVACFourPipeFanCoils.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
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

CoilCoolingWater::CoilCoolingWater(boost::shared_ptr<detail::CoilCoolingWater_Impl> p)
  : WaterToAirComponent(p)
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

