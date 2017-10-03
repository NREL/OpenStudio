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
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

