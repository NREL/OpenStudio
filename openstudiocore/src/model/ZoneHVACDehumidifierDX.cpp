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

#include "ZoneHVACDehumidifierDX.hpp"
#include "ZoneHVACDehumidifierDX_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"
// #include "WaterStorageTank.hpp"
// #include "WaterStorageTank_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_Dehumidifier_DX_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACDehumidifierDX_Impl::ZoneHVACDehumidifierDX_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACDehumidifierDX::iddObjectType());
  }

  ZoneHVACDehumidifierDX_Impl::ZoneHVACDehumidifierDX_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACDehumidifierDX::iddObjectType());
  }

  ZoneHVACDehumidifierDX_Impl::ZoneHVACDehumidifierDX_Impl(const ZoneHVACDehumidifierDX_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACDehumidifierDX_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACDehumidifierDX_Impl::iddObjectType() const {
    return ZoneHVACDehumidifierDX::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACDehumidifierDX_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_Dehumidifier_DXFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACDehumidifierDX","Availability"));
    }
    return result;
  }

  ModelObject ZoneHVACDehumidifierDX_Impl::clone(Model model) const
  {
    auto clone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACDehumidifierDX>();

    return clone;
  }

  std::vector<ModelObject> ZoneHVACDehumidifierDX_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (auto const intermediate = optionalWaterRemovalCurve()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = optionalEnergyFactorCurve()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = optionalPartLoadFractionCorrelationCurve()) {
      result.push_back(intermediate.get());
    }
    return result;
  }

  unsigned ZoneHVACDehumidifierDX_Impl::inletPort() const
  {
    return OS_ZoneHVAC_Dehumidifier_DXFields::AirInletNodeName;
  }

  unsigned ZoneHVACDehumidifierDX_Impl::outletPort() const
  {
    return OS_ZoneHVAC_Dehumidifier_DXFields::AirOutletNodeName;
  }

  Schedule ZoneHVACDehumidifierDX_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::ratedWaterRemoval() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedWaterRemoval,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::ratedEnergyFactor() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedEnergyFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::ratedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve ZoneHVACDehumidifierDX_Impl::waterRemovalCurve() const {
    boost::optional<Curve> value = optionalWaterRemovalCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Removal Curve attached.");
    }
    return value.get();
  }

  Curve ZoneHVACDehumidifierDX_Impl::energyFactorCurve() const {
    boost::optional<Curve> value = optionalEnergyFactorCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Factor Curve attached.");
    }
    return value.get();
  }

  Curve ZoneHVACDehumidifierDX_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::minimumDryBulbTemperatureforDehumidifierOperation() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::MinimumDryBulbTemperatureforDehumidifierOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::maximumDryBulbTemperatureforDehumidifierOperation() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::MaximumDryBulbTemperatureforDehumidifierOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACDehumidifierDX_Impl::offCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Dehumidifier_DXFields::OffCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  // boost::optional<WaterStorageTank> ZoneHVACDehumidifierDX_Impl::condensateCollectionWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_ZoneHVAC_Dehumidifier_DXFields::CondensateCollectionWaterStorageTankName);
  // }

  bool ZoneHVACDehumidifierDX_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_Dehumidifier_DXFields::AvailabilityScheduleName,
                              "ZoneHVACDehumidifierDX",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setRatedWaterRemoval(double ratedWaterRemoval) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedWaterRemoval, ratedWaterRemoval);
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setRatedEnergyFactor(double ratedEnergyFactor) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedEnergyFactor, ratedEnergyFactor);
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setRatedAirFlowRate(double ratedAirFlowRate) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::RatedAirFlowRate, ratedAirFlowRate);
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setWaterRemovalCurve(const Curve& curve) {
    bool result = setPointer(OS_ZoneHVAC_Dehumidifier_DXFields::WaterRemovalCurveName, curve.handle());
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setEnergyFactorCurve(const Curve& curve) {
    bool result = setPointer(OS_ZoneHVAC_Dehumidifier_DXFields::EnergyFactorCurveName, curve.handle());
    return result;
  }

  bool ZoneHVACDehumidifierDX_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_ZoneHVAC_Dehumidifier_DXFields::PartLoadFractionCorrelationCurveName, curve.handle());
    return result;
  }

  void ZoneHVACDehumidifierDX_Impl::setMinimumDryBulbTemperatureforDehumidifierOperation(double minimumDryBulbTemperatureforDehumidifierOperation) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::MinimumDryBulbTemperatureforDehumidifierOperation, minimumDryBulbTemperatureforDehumidifierOperation);
    OS_ASSERT(result);
  }

  void ZoneHVACDehumidifierDX_Impl::setMaximumDryBulbTemperatureforDehumidifierOperation(double maximumDryBulbTemperatureforDehumidifierOperation) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::MaximumDryBulbTemperatureforDehumidifierOperation, maximumDryBulbTemperatureforDehumidifierOperation);
    OS_ASSERT(result);
  }

  bool ZoneHVACDehumidifierDX_Impl::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
    bool result = setDouble(OS_ZoneHVAC_Dehumidifier_DXFields::OffCycleParasiticElectricLoad, offCycleParasiticElectricLoad);
    return result;
  }

  // bool ZoneHVACDehumidifierDX_Impl::setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_ZoneHVAC_Dehumidifier_DXFields::CondensateCollectionWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetCondensateCollectionWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void ZoneHVACDehumidifierDX_Impl::resetCondensateCollectionWaterStorageTank() {
  //   bool result = setString(OS_ZoneHVAC_Dehumidifier_DXFields::CondensateCollectionWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  boost::optional<Schedule> ZoneHVACDehumidifierDX_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Dehumidifier_DXFields::AvailabilityScheduleName);
  }

  boost::optional<Curve> ZoneHVACDehumidifierDX_Impl::optionalWaterRemovalCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ZoneHVAC_Dehumidifier_DXFields::WaterRemovalCurveName);
  }

  boost::optional<Curve> ZoneHVACDehumidifierDX_Impl::optionalEnergyFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ZoneHVAC_Dehumidifier_DXFields::EnergyFactorCurveName);
  }

  boost::optional<Curve> ZoneHVACDehumidifierDX_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ZoneHVAC_Dehumidifier_DXFields::PartLoadFractionCorrelationCurveName);
  }

} // detail

ZoneHVACDehumidifierDX::ZoneHVACDehumidifierDX(const Model& model)
  : ZoneHVACComponent(ZoneHVACDehumidifierDX::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACDehumidifierDX_Impl>());

  auto waterRemovalCurve = CurveBiquadratic(model);
  waterRemovalCurve.setCoefficient1Constant(-2.724878664080);
  waterRemovalCurve.setCoefficient2x(0.100711983591);
  waterRemovalCurve.setCoefficient3xPOW2(-0.000990538285);
  waterRemovalCurve.setCoefficient4y(0.050053043874);
  waterRemovalCurve.setCoefficient5yPOW2(-0.000203629282);
  waterRemovalCurve.setCoefficient6xTIMESY(-0.000341750531);
  waterRemovalCurve.setMinimumValueofx(21.0);
  waterRemovalCurve.setMaximumValueofx(32.22);
  waterRemovalCurve.setMinimumValueofy(40.0);
  waterRemovalCurve.setMaximumValueofy(80.0);

  auto energyFactorCurve = CurveBiquadratic(model);
  energyFactorCurve.setCoefficient1Constant(-2.388319068955);
  energyFactorCurve.setCoefficient2x(0.093047739452);
  energyFactorCurve.setCoefficient3xPOW2(-0.001369700327);
  energyFactorCurve.setCoefficient4y(0.066533716758);
  energyFactorCurve.setCoefficient5yPOW2(-0.000343198063);
  energyFactorCurve.setCoefficient6xTIMESY(-0.000562490295);
  energyFactorCurve.setMinimumValueofx(21.0);
  energyFactorCurve.setMaximumValueofx(32.22);
  energyFactorCurve.setMinimumValueofy(40.0);
  energyFactorCurve.setMaximumValueofy(80.0);

  auto partLoadFractionCurve = CurveQuadratic(model);
  partLoadFractionCurve.setCoefficient1Constant(0.95);
  partLoadFractionCurve.setCoefficient2x(0.05);
  partLoadFractionCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCurve.setMinimumValueofx(0.0);
  partLoadFractionCurve.setMaximumValueofx(1.0);

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setRatedWaterRemoval( 50.16 );
  OS_ASSERT(ok);
  ok = setRatedEnergyFactor( 3.412 );
  OS_ASSERT(ok);
  ok = setRatedAirFlowRate( 0.12036 );
  OS_ASSERT(ok);
  ok = setWaterRemovalCurve( waterRemovalCurve );
  OS_ASSERT(ok);
  ok = setEnergyFactorCurve( energyFactorCurve );
  OS_ASSERT(ok);
  ok = setPartLoadFractionCorrelationCurve( partLoadFractionCurve );
  OS_ASSERT(ok);
  setMinimumDryBulbTemperatureforDehumidifierOperation( 10.0 );
  setMaximumDryBulbTemperatureforDehumidifierOperation( 32.0 );
  ok = setOffCycleParasiticElectricLoad( 0.0 );
  OS_ASSERT(ok);
}

ZoneHVACDehumidifierDX::ZoneHVACDehumidifierDX(
  const Model& model,
  const Curve& waterRemovalCurve,
  const Curve& energyFactorCurve,
  const Curve& partLoadFractionCurve
  )
  : ZoneHVACComponent(ZoneHVACDehumidifierDX::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACDehumidifierDX_Impl>());

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setRatedWaterRemoval( 50.16 );
  OS_ASSERT(ok);
  ok = setRatedEnergyFactor( 3.412 );
  OS_ASSERT(ok);
  ok = setRatedAirFlowRate( 0.12036 );
  OS_ASSERT(ok);
  ok = setWaterRemovalCurve( waterRemovalCurve );
  OS_ASSERT(ok);
  ok = setEnergyFactorCurve( energyFactorCurve );
  OS_ASSERT(ok);
  ok = setPartLoadFractionCorrelationCurve( partLoadFractionCurve );
  OS_ASSERT(ok);
  setMinimumDryBulbTemperatureforDehumidifierOperation( 10.0 );
  setMaximumDryBulbTemperatureforDehumidifierOperation( 32.0 );
  ok = setOffCycleParasiticElectricLoad( 0.0 );
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACDehumidifierDX::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_Dehumidifier_DX);
}

Schedule ZoneHVACDehumidifierDX::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->availabilitySchedule();
}

double ZoneHVACDehumidifierDX::ratedWaterRemoval() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->ratedWaterRemoval();
}

double ZoneHVACDehumidifierDX::ratedEnergyFactor() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->ratedEnergyFactor();
}

double ZoneHVACDehumidifierDX::ratedAirFlowRate() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->ratedAirFlowRate();
}

Curve ZoneHVACDehumidifierDX::waterRemovalCurve() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->waterRemovalCurve();
}

Curve ZoneHVACDehumidifierDX::energyFactorCurve() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->energyFactorCurve();
}

Curve ZoneHVACDehumidifierDX::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->partLoadFractionCorrelationCurve();
}

double ZoneHVACDehumidifierDX::minimumDryBulbTemperatureforDehumidifierOperation() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->minimumDryBulbTemperatureforDehumidifierOperation();
}

double ZoneHVACDehumidifierDX::maximumDryBulbTemperatureforDehumidifierOperation() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->maximumDryBulbTemperatureforDehumidifierOperation();
}

double ZoneHVACDehumidifierDX::offCycleParasiticElectricLoad() const {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->offCycleParasiticElectricLoad();
}

// boost::optional<WaterStorageTank> ZoneHVACDehumidifierDX::condensateCollectionWaterStorageTank() const {
//   return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->condensateCollectionWaterStorageTank();
// }

bool ZoneHVACDehumidifierDX::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACDehumidifierDX::setRatedWaterRemoval(double ratedWaterRemoval) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setRatedWaterRemoval(ratedWaterRemoval);
}

bool ZoneHVACDehumidifierDX::setRatedEnergyFactor(double ratedEnergyFactor) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setRatedEnergyFactor(ratedEnergyFactor);
}

bool ZoneHVACDehumidifierDX::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

bool ZoneHVACDehumidifierDX::setWaterRemovalCurve(const Curve& curve) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setWaterRemovalCurve(curve);
}

bool ZoneHVACDehumidifierDX::setEnergyFactorCurve(const Curve& curve) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setEnergyFactorCurve(curve);
}

bool ZoneHVACDehumidifierDX::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

void ZoneHVACDehumidifierDX::setMinimumDryBulbTemperatureforDehumidifierOperation(double minimumDryBulbTemperatureforDehumidifierOperation) {
  getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setMinimumDryBulbTemperatureforDehumidifierOperation(minimumDryBulbTemperatureforDehumidifierOperation);
}

void ZoneHVACDehumidifierDX::setMaximumDryBulbTemperatureforDehumidifierOperation(double maximumDryBulbTemperatureforDehumidifierOperation) {
  getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setMaximumDryBulbTemperatureforDehumidifierOperation(maximumDryBulbTemperatureforDehumidifierOperation);
}

bool ZoneHVACDehumidifierDX::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
  return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setOffCycleParasiticElectricLoad(offCycleParasiticElectricLoad);
}

// bool ZoneHVACDehumidifierDX::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
// }

// void ZoneHVACDehumidifierDX::resetCondensateCollectionWaterStorageTank() {
//   getImpl<detail::ZoneHVACDehumidifierDX_Impl>()->resetCondensateCollectionWaterStorageTank();
// }

/// @cond
ZoneHVACDehumidifierDX::ZoneHVACDehumidifierDX(std::shared_ptr<detail::ZoneHVACDehumidifierDX_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

