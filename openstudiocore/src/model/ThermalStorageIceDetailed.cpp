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

#include "ThermalStorageIceDetailed.hpp"
#include "ThermalStorageIceDetailed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadraticLinear.hpp"
#include "CurveQuadraticLinear_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ThermalStorage_Ice_Detailed_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalStorageIceDetailed_Impl::ThermalStorageIceDetailed_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalStorageIceDetailed::iddObjectType());
  }

  ThermalStorageIceDetailed_Impl::ThermalStorageIceDetailed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalStorageIceDetailed::iddObjectType());
  }

  ThermalStorageIceDetailed_Impl::ThermalStorageIceDetailed_Impl(const ThermalStorageIceDetailed_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ThermalStorageIceDetailed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ThermalStorageIceDetailed_Impl::iddObjectType() const {
    return ThermalStorageIceDetailed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ThermalStorageIceDetailed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ThermalStorage_Ice_DetailedFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ThermalStorageIceDetailed","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> ThermalStorageIceDetailed_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ThermalStorage_Ice_DetailedFields::AvailabilitySchedule);
  }

  double ThermalStorageIceDetailed_Impl::capacity() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::Capacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve ThermalStorageIceDetailed_Impl::dischargingCurve() const {
    auto value = optionalDischargingCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Discharging Curve attached.");
    }
    return value.get();
  }

  Curve ThermalStorageIceDetailed_Impl::chargingCurve() const {
    auto value = optionalChargingCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Charging Curve attached.");
    }
    return value.get();
  }

  double ThermalStorageIceDetailed_Impl::timestepoftheCurveData() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::TimestepoftheCurveData,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageIceDetailed_Impl::parasiticElectricLoadDuringDischarging() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringDischarging,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageIceDetailed_Impl::parasiticElectricLoadDuringCharging() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringCharging,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageIceDetailed_Impl::tankLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::TankLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageIceDetailed_Impl::freezingTemperatureofStorageMedium() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_Ice_DetailedFields::FreezingTemperatureofStorageMedium,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ThermalStorageIceDetailed_Impl::thawProcessIndicator() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_Ice_DetailedFields::ThawProcessIndicator,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStorageIceDetailed_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ThermalStorage_Ice_DetailedFields::AvailabilitySchedule,
                              "ThermalStorageIceDetailed",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void ThermalStorageIceDetailed_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ThermalStorage_Ice_DetailedFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  void ThermalStorageIceDetailed_Impl::setCapacity(double capacity) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::Capacity, capacity);
    OS_ASSERT(result);
  }

  bool ThermalStorageIceDetailed_Impl::setDischargingCurve(const Curve& dischargingCurve) {
    bool result = setPointer(OS_ThermalStorage_Ice_DetailedFields::DischargingCurve, dischargingCurve.handle());
    return result;
  }

  bool ThermalStorageIceDetailed_Impl::setChargingCurve(const Curve& chargingCurve) {
    bool result = setPointer(OS_ThermalStorage_Ice_DetailedFields::ChargingCurve, chargingCurve.handle());
    return result;
  }

  void ThermalStorageIceDetailed_Impl::setTimestepoftheCurveData(double timestepoftheCurveData) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::TimestepoftheCurveData, timestepoftheCurveData);
    OS_ASSERT(result);
  }

  void ThermalStorageIceDetailed_Impl::setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringDischarging, parasiticElectricLoadDuringDischarging);
    OS_ASSERT(result);
  }

  void ThermalStorageIceDetailed_Impl::setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringCharging, parasiticElectricLoadDuringCharging);
    OS_ASSERT(result);
  }

  void ThermalStorageIceDetailed_Impl::setTankLossCoefficient(double tankLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::TankLossCoefficient, tankLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageIceDetailed_Impl::setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium) {
    bool result = setDouble(OS_ThermalStorage_Ice_DetailedFields::FreezingTemperatureofStorageMedium, freezingTemperatureofStorageMedium);
    OS_ASSERT(result);
  }

  bool ThermalStorageIceDetailed_Impl::setThawProcessIndicator(std::string thawProcessIndicator) {
    bool result = setString(OS_ThermalStorage_Ice_DetailedFields::ThawProcessIndicator, thawProcessIndicator);
    return result;
  }

  boost::optional<Curve> ThermalStorageIceDetailed_Impl::optionalDischargingCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ThermalStorage_Ice_DetailedFields::DischargingCurve);
  }

  boost::optional<Curve> ThermalStorageIceDetailed_Impl::optionalChargingCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ThermalStorage_Ice_DetailedFields::ChargingCurve);
  }

  unsigned ThermalStorageIceDetailed_Impl::inletPort() {
    return OS_ThermalStorage_Ice_DetailedFields::InletNodeName;
  }

  unsigned ThermalStorageIceDetailed_Impl::outletPort() {
    return OS_ThermalStorage_Ice_DetailedFields::OutletNodeName;
  }

  bool ThermalStorageIceDetailed_Impl::addToNode(Node & node)
  {
    if( auto plant = node.plantLoop() ) {
      if( ! plant->demandComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

  ModelObject ThermalStorageIceDetailed_Impl::clone(Model model) const
  {
    auto newMo = StraightComponent_Impl::clone(model).cast<ThermalStorageIceDetailed>();

    return newMo;
  }

  std::vector<ModelObject> ThermalStorageIceDetailed_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(dischargingCurve());
    result.push_back(chargingCurve());

    return result;
  }

} // detail

ThermalStorageIceDetailed::ThermalStorageIceDetailed(const Model& model)
  : StraightComponent(ThermalStorageIceDetailed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalStorageIceDetailed_Impl>());

  {
    CurveQuadraticLinear curve(model);  
    curve.setCoefficient1Constant(0.0);
    curve.setCoefficient2x(0.09);
    curve.setCoefficient3xPOW2(-0.15);
    curve.setCoefficient4y(0.612);
    curve.setCoefficient5xTIMESY(-0.324);
    curve.setCoefficient6xPOW2TIMESY(-0.216);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    curve.setMinimumValueofy(0.0);
    curve.setMaximumValueofy(9.9);

    setDischargingCurve(curve);
  }

  {
    CurveQuadraticLinear curve(model);
    curve.setCoefficient1Constant(0.0);
    curve.setCoefficient2x(0.09);
    curve.setCoefficient3xPOW2(-0.15);
    curve.setCoefficient4y(0.612);
    curve.setCoefficient5xTIMESY(-0.324);
    curve.setCoefficient6xPOW2TIMESY(-0.216);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    curve.setMinimumValueofy(0.0);
    curve.setMaximumValueofy(9.9);

    setChargingCurve(curve);
  }

  setCapacity(0.5);
  setTimestepoftheCurveData(1.0);
  setParasiticElectricLoadDuringDischarging(0.0001);
  setParasiticElectricLoadDuringCharging(0.0002);
  setTankLossCoefficient(0.0003);
  setFreezingTemperatureofStorageMedium(0.0);
  setThawProcessIndicator("OutsideMelt");
}

IddObjectType ThermalStorageIceDetailed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ThermalStorage_Ice_Detailed);
}

std::vector<std::string> ThermalStorageIceDetailed::thawProcessIndicatorValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalStorage_Ice_DetailedFields::ThawProcessIndicator);
}

boost::optional<Schedule> ThermalStorageIceDetailed::availabilitySchedule() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->availabilitySchedule();
}

double ThermalStorageIceDetailed::capacity() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->capacity();
}

Curve ThermalStorageIceDetailed::dischargingCurve() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->dischargingCurve();
}

Curve ThermalStorageIceDetailed::chargingCurve() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->chargingCurve();
}

double ThermalStorageIceDetailed::timestepoftheCurveData() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->timestepoftheCurveData();
}

double ThermalStorageIceDetailed::parasiticElectricLoadDuringDischarging() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->parasiticElectricLoadDuringDischarging();
}

double ThermalStorageIceDetailed::parasiticElectricLoadDuringCharging() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->parasiticElectricLoadDuringCharging();
}

double ThermalStorageIceDetailed::tankLossCoefficient() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->tankLossCoefficient();
}

double ThermalStorageIceDetailed::freezingTemperatureofStorageMedium() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->freezingTemperatureofStorageMedium();
}

std::string ThermalStorageIceDetailed::thawProcessIndicator() const {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->thawProcessIndicator();
}

bool ThermalStorageIceDetailed::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->setAvailabilitySchedule(schedule);
}

void ThermalStorageIceDetailed::resetAvailabilitySchedule() {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->resetAvailabilitySchedule();
}

void ThermalStorageIceDetailed::setCapacity(double capacity) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setCapacity(capacity);
}

bool ThermalStorageIceDetailed::setDischargingCurve(const Curve& dischargingCurve) {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->setDischargingCurve(dischargingCurve);
}

bool ThermalStorageIceDetailed::setChargingCurve(const Curve& chargingCurve) {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->setChargingCurve(chargingCurve);
}

void ThermalStorageIceDetailed::setTimestepoftheCurveData(double timestepoftheCurveData) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setTimestepoftheCurveData(timestepoftheCurveData);
}

void ThermalStorageIceDetailed::setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setParasiticElectricLoadDuringDischarging(parasiticElectricLoadDuringDischarging);
}

void ThermalStorageIceDetailed::setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setParasiticElectricLoadDuringCharging(parasiticElectricLoadDuringCharging);
}

void ThermalStorageIceDetailed::setTankLossCoefficient(double tankLossCoefficient) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setTankLossCoefficient(tankLossCoefficient);
}

void ThermalStorageIceDetailed::setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium) {
  getImpl<detail::ThermalStorageIceDetailed_Impl>()->setFreezingTemperatureofStorageMedium(freezingTemperatureofStorageMedium);
}

bool ThermalStorageIceDetailed::setThawProcessIndicator(std::string thawProcessIndicator) {
  return getImpl<detail::ThermalStorageIceDetailed_Impl>()->setThawProcessIndicator(thawProcessIndicator);
}

/// @cond
ThermalStorageIceDetailed::ThermalStorageIceDetailed(std::shared_ptr<detail::ThermalStorageIceDetailed_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

