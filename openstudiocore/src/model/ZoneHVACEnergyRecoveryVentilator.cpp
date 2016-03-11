/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ZoneHVACEnergyRecoveryVentilator.hpp"
#include "ZoneHVACEnergyRecoveryVentilator_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Model.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "FanOnOff.hpp"
#include "ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_EnergyRecoveryVentilator_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACEnergyRecoveryVentilator_Impl::ZoneHVACEnergyRecoveryVentilator_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACEnergyRecoveryVentilator::iddObjectType());
  }

  ZoneHVACEnergyRecoveryVentilator_Impl::ZoneHVACEnergyRecoveryVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACEnergyRecoveryVentilator::iddObjectType());
  }

  ZoneHVACEnergyRecoveryVentilator_Impl::ZoneHVACEnergyRecoveryVentilator_Impl(const ZoneHVACEnergyRecoveryVentilator_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACEnergyRecoveryVentilator_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACEnergyRecoveryVentilator_Impl::iddObjectType() const {
    return ZoneHVACEnergyRecoveryVentilator::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACEnergyRecoveryVentilator_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACEnergyRecoveryVentilator","Availability"));
    }
    return result;
  }

  ModelObject ZoneHVACEnergyRecoveryVentilator_Impl::clone(Model model) const
  {
    auto cloneObject = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACEnergyRecoveryVentilator>();

    auto const supplyFanClone = supplyAirFan().clone(model).cast<HVACComponent>();
    cloneObject.setSupplyAirFan(supplyFanClone);

    auto const exhaustFanClone = exhaustAirFan().clone(model).cast<HVACComponent>();
    cloneObject.setExhaustAirFan(exhaustFanClone);

    auto const heatExchangerClone = heatExchanger().clone(model).cast<HVACComponent>();
    cloneObject.setHeatExchanger(heatExchangerClone);

    if (auto const controllerObj = controller()) {
      auto const controllerClone = controllerObj->clone(model).cast<ZoneHVACEnergyRecoveryVentilatorController>();
      cloneObject.setController(controllerClone);
    }

    return cloneObject;
  }

  std::vector<ModelObject> ZoneHVACEnergyRecoveryVentilator_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (auto const intermediate = optionalSupplyAirFan()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = optionalExhaustAirFan()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = optionalHeatExchanger()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = controller()) {
      result.push_back(intermediate.get());
    }
    return result;
  }

  unsigned ZoneHVACEnergyRecoveryVentilator_Impl::inletPort() const
  {
    return OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AirInletNodeName;
  }

  unsigned ZoneHVACEnergyRecoveryVentilator_Impl::outletPort() const
  {
    return OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AirOutletNodeName;
  }

  Schedule ZoneHVACEnergyRecoveryVentilator_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  HVACComponent ZoneHVACEnergyRecoveryVentilator_Impl::heatExchanger() const {
    boost::optional<HVACComponent> value = optionalHeatExchanger();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heat Exchanger attached.");
    }
    return value.get();
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilator_Impl::supplyAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate,true);
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::isSupplyAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilator_Impl::exhaustAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate,true);
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::isExhaustAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  HVACComponent ZoneHVACEnergyRecoveryVentilator_Impl::supplyAirFan() const {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return value.get();
  }

  HVACComponent ZoneHVACEnergyRecoveryVentilator_Impl::exhaustAirFan() const {
    boost::optional<HVACComponent> value = optionalExhaustAirFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Exhaust Air Fan attached.");
    }
    return value.get();
  }

  boost::optional<ZoneHVACEnergyRecoveryVentilatorController> ZoneHVACEnergyRecoveryVentilator_Impl::controller() const {
    return getObject<ModelObject>().getModelObjectTarget<ZoneHVACEnergyRecoveryVentilatorController>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName);
  }

  double ZoneHVACEnergyRecoveryVentilator_Impl::ventilationRateperUnitFloorArea() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperUnitFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACEnergyRecoveryVentilator_Impl::ventilationRateperOccupant() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperOccupant,true);
    OS_ASSERT(value);
    return value.get();
  }

  // boost::optional<ModelObject> ZoneHVACEnergyRecoveryVentilator_Impl::availabilityManagerList() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityManagerListName);
  // }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName,
                              "ZoneHVACEnergyRecoveryVentilator",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent) {
    bool result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::HeatExchangerName, hxAirToAirSensibleAndLatent.handle());
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setSupplyAirFlowRate(boost::optional<double> supplyAirFlowRate) {
    bool result(false);
    if (supplyAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate, supplyAirFlowRate.get());
    }
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilator_Impl::autosizeSupplyAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setExhaustAirFlowRate(boost::optional<double> exhaustAirFlowRate) {
    bool result(false);
    if (exhaustAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate, exhaustAirFlowRate.get());
    }
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilator_Impl::autosizeExhaustAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setSupplyAirFan(const HVACComponent& supplyAirFan) {
    bool result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFanName, supplyAirFan.handle());
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setExhaustAirFan(const HVACComponent& exhaustAirFan) {
    bool result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFanName, exhaustAirFan.handle());
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setController(const boost::optional<ZoneHVACEnergyRecoveryVentilatorController>& zoneHVACEnergyRecoveryVentilatorController) {
    bool result(false);
    if (zoneHVACEnergyRecoveryVentilatorController) {
      result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName, zoneHVACEnergyRecoveryVentilatorController.get().handle());
    }
    else {
      resetController();
      result = true;
    }
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilator_Impl::resetController() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea) {
    bool result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperUnitFloorArea, ventilationRateperUnitFloorArea);
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilator_Impl::setVentilationRateperOccupant(double ventilationRateperOccupant) {
    bool result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperOccupant, ventilationRateperOccupant);
    return result;
  }

  // bool ZoneHVACEnergyRecoveryVentilator_Impl::setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists) {
  //   bool result(false);
  //   if (systemAvailabilityManagerLists) {
  //     result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityManagerListName, systemAvailabilityManagerLists.get().handle());
  //   }
  //   else {
  //     resetAvailabilityManagerList();
  //     result = true;
  //   }
  //   return result;
  // }

  // void ZoneHVACEnergyRecoveryVentilator_Impl::resetAvailabilityManagerList() {
  //   bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityManagerListName, "");
  //   OS_ASSERT(result);
  // }

  boost::optional<Schedule> ZoneHVACEnergyRecoveryVentilator_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACEnergyRecoveryVentilator_Impl::optionalHeatExchanger() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::HeatExchangerName);
  }

  boost::optional<HVACComponent> ZoneHVACEnergyRecoveryVentilator_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFanName);
  }

  boost::optional<HVACComponent> ZoneHVACEnergyRecoveryVentilator_Impl::optionalExhaustAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFanName);
  }

} // detail

ZoneHVACEnergyRecoveryVentilator::ZoneHVACEnergyRecoveryVentilator(const Model& model)
  : ZoneHVACComponent(ZoneHVACEnergyRecoveryVentilator::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>());

  auto heatExchanger = HeatExchangerAirToAirSensibleAndLatent( model );
  // The Supply Air Outlet Temperature Control must be "No" in the heat exchanger, otherwise
  // we need to add a Setpoint Manager on the Supply Air Outlet Node of the heat exchanger.
  heatExchanger.setSupplyAirOutletTemperatureControl( false );
  auto supplyAirFan = FanOnOff( model );
  auto exhaustAirFan = FanOnOff( model );

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setHeatExchanger( heatExchanger );
  OS_ASSERT(ok);
  autosizeSupplyAirFlowRate();
  autosizeExhaustAirFlowRate();
  ok = setSupplyAirFan( supplyAirFan );
  OS_ASSERT(ok);
  ok = setExhaustAirFan( exhaustAirFan );
  OS_ASSERT(ok);
  ok = setVentilationRateperUnitFloorArea( 0.000508 );
  OS_ASSERT(ok);
  ok = setVentilationRateperOccupant( 0.00236 );
  OS_ASSERT(ok);
}

ZoneHVACEnergyRecoveryVentilator::ZoneHVACEnergyRecoveryVentilator(
  const Model& model,
  const HVACComponent& heatExchanger,
  const HVACComponent& supplyAirFan,
  const HVACComponent& exhaustAirFan
  )
  : ZoneHVACComponent(ZoneHVACEnergyRecoveryVentilator::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>());

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  if ( auto _heatExchanger = heatExchanger.optionalCast<HeatExchangerAirToAirSensibleAndLatent>() ) {
    // The Supply Air Outlet Temperature Control must be "No" in the heat exchanger, otherwise
    // we need to add a Setpoint Manager on the Supply Air Outlet Node of the heat exchanger.
    _heatExchanger->setSupplyAirOutletTemperatureControl( false );
  }
  ok = setHeatExchanger( heatExchanger );
  OS_ASSERT(ok);
  autosizeSupplyAirFlowRate();
  autosizeExhaustAirFlowRate();
  ok = setSupplyAirFan( supplyAirFan );
  OS_ASSERT(ok);
  ok = setExhaustAirFan( exhaustAirFan );
  OS_ASSERT(ok);
  ok = setVentilationRateperUnitFloorArea( 0.000508 );
  OS_ASSERT(ok);
  ok = setVentilationRateperOccupant( 0.00236 );
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACEnergyRecoveryVentilator::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator);
}

Schedule ZoneHVACEnergyRecoveryVentilator::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->availabilitySchedule();
}

HVACComponent ZoneHVACEnergyRecoveryVentilator::heatExchanger() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->heatExchanger();
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilator::supplyAirFlowRate() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->supplyAirFlowRate();
}

bool ZoneHVACEnergyRecoveryVentilator::isSupplyAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->isSupplyAirFlowRateAutosized();
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilator::exhaustAirFlowRate() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->exhaustAirFlowRate();
}

bool ZoneHVACEnergyRecoveryVentilator::isExhaustAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->isExhaustAirFlowRateAutosized();
}

HVACComponent ZoneHVACEnergyRecoveryVentilator::supplyAirFan() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->supplyAirFan();
}

HVACComponent ZoneHVACEnergyRecoveryVentilator::exhaustAirFan() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->exhaustAirFan();
}

boost::optional<ZoneHVACEnergyRecoveryVentilatorController> ZoneHVACEnergyRecoveryVentilator::controller() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->controller();
}

double ZoneHVACEnergyRecoveryVentilator::ventilationRateperUnitFloorArea() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->ventilationRateperUnitFloorArea();
}

double ZoneHVACEnergyRecoveryVentilator::ventilationRateperOccupant() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->ventilationRateperOccupant();
}

// boost::optional<ModelObject> ZoneHVACEnergyRecoveryVentilator::availabilityManagerList() const {
//   return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->availabilityManagerList();
// }

bool ZoneHVACEnergyRecoveryVentilator::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACEnergyRecoveryVentilator::setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setHeatExchanger(hxAirToAirSensibleAndLatent);
}

bool ZoneHVACEnergyRecoveryVentilator::setSupplyAirFlowRate(double supplyAirFlowRate) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setSupplyAirFlowRate(supplyAirFlowRate);
}

void ZoneHVACEnergyRecoveryVentilator::autosizeSupplyAirFlowRate() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->autosizeSupplyAirFlowRate();
}

bool ZoneHVACEnergyRecoveryVentilator::setExhaustAirFlowRate(double exhaustAirFlowRate) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setExhaustAirFlowRate(exhaustAirFlowRate);
}

void ZoneHVACEnergyRecoveryVentilator::autosizeExhaustAirFlowRate() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->autosizeExhaustAirFlowRate();
}

bool ZoneHVACEnergyRecoveryVentilator::setSupplyAirFan(const HVACComponent& fansOnOff) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setSupplyAirFan(fansOnOff);
}

bool ZoneHVACEnergyRecoveryVentilator::setExhaustAirFan(const HVACComponent& fansOnOff) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setExhaustAirFan(fansOnOff);
}

bool ZoneHVACEnergyRecoveryVentilator::setController(const ZoneHVACEnergyRecoveryVentilatorController& zoneHVACEnergyRecoveryVentilatorController) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setController(zoneHVACEnergyRecoveryVentilatorController);
}

void ZoneHVACEnergyRecoveryVentilator::resetController() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->resetController();
}

bool ZoneHVACEnergyRecoveryVentilator::setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setVentilationRateperUnitFloorArea(ventilationRateperUnitFloorArea);
}

bool ZoneHVACEnergyRecoveryVentilator::setVentilationRateperOccupant(double ventilationRateperOccupant) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setVentilationRateperOccupant(ventilationRateperOccupant);
}

// bool ZoneHVACEnergyRecoveryVentilator::setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists) {
//   return getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->setAvailabilityManagerList(systemAvailabilityManagerLists);
// }

// void ZoneHVACEnergyRecoveryVentilator::resetAvailabilityManagerList() {
//   getImpl<detail::ZoneHVACEnergyRecoveryVentilator_Impl>()->resetAvailabilityManagerList();
// }

/// @cond
ZoneHVACEnergyRecoveryVentilator::ZoneHVACEnergyRecoveryVentilator(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilator_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

