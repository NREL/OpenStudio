/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Space.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_Baseboard_RadiantConvective_Water_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType());
  }

  ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType());
  }

  ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(const ZoneHVACBaseboardRadiantConvectiveWater_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACBaseboardRadiantConvectiveWater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACBaseboardRadiantConvectiveWater_Impl::iddObjectType() const {
    return ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACBaseboardRadiantConvectiveWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACBaseboardRadiantConvectiveWater","Availability"));
    }
    return result;
  }

  unsigned ZoneHVACBaseboardRadiantConvectiveWater_Impl::inletPort() const
  {
    return OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::InletNodeName;
  }

  unsigned ZoneHVACBaseboardRadiantConvectiveWater_Impl::outletPort() const
  {
    return OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::OutletNodeName;
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveWater_Impl::thermalZone() const
  {
    ModelObject thisObject = this->getObject<ModelObject>();
    auto const thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
    for( auto const & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        return thermalZone;
      }
    }
    return boost::none;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m || thermalZone.isPlenum() )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneHVACBaseboardRadiantConvectiveWater_Impl::removeFromThermalZone()
  {
    if ( auto thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

  std::vector<Surface> ZoneHVACBaseboardRadiantConvectiveWater_Impl::surfaces() const {    
    
    //vector to hold all of the surfaces that this radiant system is attached to
    std::vector<Surface> surfaces;

    //get the thermal zone this equipment belongs to
    if (auto const thermalZone = this->thermalZone()) {

      //loop through all the spaces in this zone
      for (auto const & space : thermalZone->spaces()){
    
        //loop through all the surfaces in this space
        for (auto const & surface : space.surfaces()){
          surfaces.push_back(surface);
        }
      }    
    }

    return surfaces;
  }

  // bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::addToNode(Node & node)
  // {
  //   return ZoneHVACComponent_Impl::addToNode(node);
  // }

  // ModelObject ZoneHVACBaseboardRadiantConvectiveWater_Impl::clone(Model model) const
  // {
  //   return ZoneHVACComponent_Impl::clone(node);
  // }

  Schedule ZoneHVACBaseboardRadiantConvectiveWater_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::ratedAverageWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedAverageWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::ratedWaterMassFlowRate() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedWaterMassFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACBaseboardRadiantConvectiveWater_Impl::heatingDesignCapacityMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACBaseboardRadiantConvectiveWater_Impl::heatingDesignCapacity() const {
    return getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity,true);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::isHeatingDesignCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::heatingDesignCapacityPerFloorArea() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityPerFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::fractionofAutosizedHeatingDesignCapacity() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofAutosizedHeatingDesignCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACBaseboardRadiantConvectiveWater_Impl::maximumWaterFlowRate() const {
    return getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate,true);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::isMaximumWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::convergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::ConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater_Impl::fractionofRadiantEnergyIncidentonPeople() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName,
                              "ZoneHVACBaseboardRadiantConvectiveWater",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedAverageWaterTemperature, ratedAverageWaterTemperature);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedWaterMassFlowRate, ratedWaterMassFlowRate);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
    bool result = setString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
    bool result(false);
    if (heatingDesignCapacity) {
      result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity, heatingDesignCapacity.get());
    }
    return result;
  }

  void ZoneHVACBaseboardRadiantConvectiveWater_Impl::autosizeHeatingDesignCapacity() {
    bool result = setString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
    return result;
  }

  void ZoneHVACBaseboardRadiantConvectiveWater_Impl::setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate) {
    bool result(false);
    if (maximumWaterFlowRate) {
      result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate, maximumWaterFlowRate.get());
    }
    OS_ASSERT(result);
  }

  void ZoneHVACBaseboardRadiantConvectiveWater_Impl::autosizeMaximumWaterFlowRate() {
    bool result = setString(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setConvergenceTolerance(double convergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::ConvergenceTolerance, convergenceTolerance);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionRadiant, fractionRadiant);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople, fractionofRadiantEnergyIncidentonPeople);
    return result;
  }

  boost::optional<Schedule> ZoneHVACBaseboardRadiantConvectiveWater_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName);
  }

} // detail

ZoneHVACBaseboardRadiantConvectiveWater::ZoneHVACBaseboardRadiantConvectiveWater(const Model& model)
  : ZoneHVACComponent(ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>());

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setRatedAverageWaterTemperature( 87.78 );
  OS_ASSERT(ok);
  ok = setRatedWaterMassFlowRate( 0.063 );
  OS_ASSERT(ok);
  ok = setHeatingDesignCapacityMethod( "HeatingDesignCapacity" );
  OS_ASSERT(ok);
  autosizeHeatingDesignCapacity();
  ok = setHeatingDesignCapacityPerFloorArea( 0 );
  OS_ASSERT(ok);
  ok = setFractionofAutosizedHeatingDesignCapacity( 1.0 );
  OS_ASSERT(ok);
  autosizeMaximumWaterFlowRate();
  ok = setConvergenceTolerance( 0.001 );
  OS_ASSERT(ok);
  ok = setFractionRadiant( 0.3 );
  OS_ASSERT(ok);
  ok = setFractionofRadiantEnergyIncidentonPeople( 0.3 );
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water);
}

std::vector<std::string> ZoneHVACBaseboardRadiantConvectiveWater::heatingDesignCapacityMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityMethod);
}

Schedule ZoneHVACBaseboardRadiantConvectiveWater::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->availabilitySchedule();
}

double ZoneHVACBaseboardRadiantConvectiveWater::ratedAverageWaterTemperature() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->ratedAverageWaterTemperature();
}

double ZoneHVACBaseboardRadiantConvectiveWater::ratedWaterMassFlowRate() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->ratedWaterMassFlowRate();
}

std::string ZoneHVACBaseboardRadiantConvectiveWater::heatingDesignCapacityMethod() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->heatingDesignCapacityMethod();
}

boost::optional<double> ZoneHVACBaseboardRadiantConvectiveWater::heatingDesignCapacity() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->heatingDesignCapacity();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::isHeatingDesignCapacityAutosized() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->isHeatingDesignCapacityAutosized();
}

double ZoneHVACBaseboardRadiantConvectiveWater::heatingDesignCapacityPerFloorArea() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->heatingDesignCapacityPerFloorArea();
}

double ZoneHVACBaseboardRadiantConvectiveWater::fractionofAutosizedHeatingDesignCapacity() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->fractionofAutosizedHeatingDesignCapacity();
}

boost::optional<double> ZoneHVACBaseboardRadiantConvectiveWater::maximumWaterFlowRate() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->maximumWaterFlowRate();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::isMaximumWaterFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->isMaximumWaterFlowRateAutosized();
}

double ZoneHVACBaseboardRadiantConvectiveWater::convergenceTolerance() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->convergenceTolerance();
}

double ZoneHVACBaseboardRadiantConvectiveWater::fractionRadiant() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->fractionRadiant();
}

double ZoneHVACBaseboardRadiantConvectiveWater::fractionofRadiantEnergyIncidentonPeople() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->fractionofRadiantEnergyIncidentonPeople();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setRatedAverageWaterTemperature(double ratedAverageWaterTemperature) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setRatedAverageWaterTemperature(ratedAverageWaterTemperature);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setRatedWaterMassFlowRate(double ratedWaterMassFlowRate) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setRatedWaterMassFlowRate(ratedWaterMassFlowRate);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setHeatingDesignCapacity(double heatingDesignCapacity) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
}

void ZoneHVACBaseboardRadiantConvectiveWater::autosizeHeatingDesignCapacity() {
  getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->autosizeHeatingDesignCapacity();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setFractionofAutosizedHeatingDesignCapacity(fractionofAutosizedHeatingDesignCapacity);
}

void ZoneHVACBaseboardRadiantConvectiveWater::setMaximumWaterFlowRate(double maximumWaterFlowRate) {
  getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setMaximumWaterFlowRate(maximumWaterFlowRate);
}

void ZoneHVACBaseboardRadiantConvectiveWater::autosizeMaximumWaterFlowRate() {
  getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->autosizeMaximumWaterFlowRate();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setConvergenceTolerance(double convergenceTolerance) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setFractionRadiant(fractionRadiant);
}

bool ZoneHVACBaseboardRadiantConvectiveWater::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setFractionofRadiantEnergyIncidentonPeople(fractionofRadiantEnergyIncidentonPeople);
}

boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveWater::thermalZone() {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->thermalZone();
}

bool ZoneHVACBaseboardRadiantConvectiveWater::addToThermalZone(ThermalZone & thermalZone) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACBaseboardRadiantConvectiveWater::removeFromThermalZone() {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->removeFromThermalZone();
}

/// @cond
ZoneHVACBaseboardRadiantConvectiveWater::ZoneHVACBaseboardRadiantConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

