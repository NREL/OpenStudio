/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilCoolingDXCurveFitSpeed.hpp"
#include "CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXCurveFitSpeed::iddObjectType());
  }

  CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXCurveFitSpeed::iddObjectType());
  }

  CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const CoilCoolingDXCurveFitSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXCurveFitSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXCurveFitSpeed_Impl::iddObjectType() const {
    return CoilCoolingDXCurveFitSpeed::iddObjectType();
  }

  double CoilCoolingDXCurveFitSpeed_Impl::grossTotalCoolingCapacityFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXCurveFitSpeed_Impl::evaporatorAirFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXCurveFitSpeed_Impl::condenserAirFlowRateFraction() const {
    return getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction,true);
  }

  boost::optional<double> CoilCoolingDXCurveFitSpeed_Impl::grossSensibleHeatRatio() const {
    return getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio,true);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::isGrossSensibleHeatRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingDXCurveFitSpeed_Impl::autosizedGrossSensibleHeatRatio() {
    return getAutosizedValue("TODO_CHECK_SQL Gross Sensible Heat Ratio", "");
  }

  double CoilCoolingDXCurveFitSpeed_Impl::grossCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXCurveFitSpeed_Impl::activeFractionofCoilFaceArea() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::isActiveFractionofCoilFaceAreaDefaulted() const {
    return isEmpty(OS_Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea);
  }

  double CoilCoolingDXCurveFitSpeed_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXCurveFitSpeed_Impl::evaporativeCondenserPumpPowerFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::isEvaporativeCondenserPumpPowerFractionDefaulted() const {
    return isEmpty(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction);
  }

  double CoilCoolingDXCurveFitSpeed_Impl::evaporativeCondenserEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::totalCoolingCapacityModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::totalCoolingCapacityModifierFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::energyInputRatioModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::energyInputRatioModifierFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::partLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve);
  }

  double CoilCoolingDXCurveFitSpeed_Impl::ratedWasteHeatFractionofPowerInput() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::wasteHeatModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::sensibleHeatRatioModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve);
  }

  Curve CoilCoolingDXCurveFitSpeed_Impl::sensibleHeatRatioModifierFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setGrossTotalCoolingCapacityFraction(double grossTotalCoolingCapacityFraction) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction, grossTotalCoolingCapacityFraction);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setEvaporatorAirFlowRateFraction(double evaporatorAirFlowRateFraction) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction, evaporatorAirFlowRateFraction);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setCondenserAirFlowRateFraction(double condenserAirFlowRateFraction) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, condenserAirFlowRateFraction);
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetCondenserAirFlowRateFraction() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setGrossSensibleHeatRatio(double grossSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, grossSensibleHeatRatio);
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::autosizeGrossSensibleHeatRatio() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setGrossCoolingCOP(double grossCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP, grossCoolingCOP);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setActiveFractionofCoilFaceArea(double activeFractionofCoilFaceArea) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, activeFractionofCoilFaceArea);
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetActiveFractionofCoilFaceArea() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate, ratedEvaporatorFanPowerPerVolumeFlowRate);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setEvaporativeCondenserPumpPowerFraction(double evaporativeCondenserPumpPowerFraction) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, evaporativeCondenserPumpPowerFraction);
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetEvaporativeCondenserPumpPowerFraction() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setTotalCoolingCapacityModifierFunctionofTemperatureCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetTotalCoolingCapacityModifierFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setEnergyInputRatioModifierFunctionofTemperatureCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetEnergyInputRatioModifierFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetPartLoadFractionCorrelationCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
    bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, ratedWasteHeatFractionofPowerInput);
    return result;
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setWasteHeatModifierFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve, bivariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetWasteHeatModifierFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setSensibleHeatRatioModifierFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve, bivariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetSensibleHeatRatioModifierFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXCurveFitSpeed_Impl::setSensibleHeatRatioModifierFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve, univariateFunctions.handle());
    return result;
  }

  void CoilCoolingDXCurveFitSpeed_Impl::resetSensibleHeatRatioModifierFunctionofFlowFractionCurve() {
    bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve, "");
    OS_ASSERT(result);
  }

  void CoilCoolingDXCurveFitSpeed_Impl::autosize() {
    autosizedGrossSensibleHeatRatio();
  }

  void CoilCoolingDXCurveFitSpeed_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedGrossSensibleHeatRatio();
    if (val) {
      setGrossSensibleHeatRatio(val.get());
    }

  }

} // detail

CoilCoolingDXCurveFitSpeed::CoilCoolingDXCurveFitSpeed(const Model& model)
  : ParentObject(CoilCoolingDXCurveFitSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setGrossTotalCoolingCapacityFraction();
  OS_ASSERT(ok);
  // ok = setEvaporatorAirFlowRateFraction();
  OS_ASSERT(ok);
  // ok = setGrossSensibleHeatRatio();
  OS_ASSERT(ok);
  // ok = setGrossCoolingCOP();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorFanPowerPerVolumeFlowRate();
  OS_ASSERT(ok);
  // ok = setEvaporativeCondenserEffectiveness();
  OS_ASSERT(ok);
  // ok = setRatedWasteHeatFractionofPowerInput();
  OS_ASSERT(ok);
}

CoilCoolingDXCurveFitSpeed::CoilCoolingDXCurveFitSpeed(const Model& model,
  Curve& totalCoolingCapacityModifierFunctionofTemperatureCurve,
  Curve& totalCoolingCapacityModifierFunctionofAirFlowFractionCurve,
  Curve& energyInputRatioModifierFunctionofTemperatureCurve,
  Curve& energyInputRatioModifierFunctionofAirFlowFractionCurve,
  Curve& partLoadFractionCorrelationCurve,
  Curve& wasteHeatModifierFunctionofTemperatureCurve)
  : ParentObject(CoilCoolingDXCurveFitSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setGrossTotalCoolingCapacityFraction();
  OS_ASSERT(ok);
  // ok = setEvaporatorAirFlowRateFraction();
  OS_ASSERT(ok);
  // ok = setGrossSensibleHeatRatio();
  OS_ASSERT(ok);
  // ok = setGrossCoolingCOP();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorFanPowerPerVolumeFlowRate();
  OS_ASSERT(ok);
  // ok = setEvaporativeCondenserEffectiveness();
  OS_ASSERT(ok);
  // ok = setRatedWasteHeatFractionofPowerInput();
  OS_ASSERT(ok);
}

IddObjectType CoilCoolingDXCurveFitSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_CurveFit_Speed);
}

double CoilCoolingDXCurveFitSpeed::grossTotalCoolingCapacityFraction() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->grossTotalCoolingCapacityFraction();
}

double CoilCoolingDXCurveFitSpeed::evaporatorAirFlowRateFraction() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->evaporatorAirFlowRateFraction();
}

boost::optional<double> CoilCoolingDXCurveFitSpeed::condenserAirFlowRateFraction() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->condenserAirFlowRateFraction();
}

boost::optional<double> CoilCoolingDXCurveFitSpeed::grossSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->grossSensibleHeatRatio();
}

bool CoilCoolingDXCurveFitSpeed::isGrossSensibleHeatRatioAutosized() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->isGrossSensibleHeatRatioAutosized();
}

  boost::optional <double> CoilCoolingDXCurveFitSpeed::autosizedGrossSensibleHeatRatio() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedGrossSensibleHeatRatio();
  }

double CoilCoolingDXCurveFitSpeed::grossCoolingCOP() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->grossCoolingCOP();
}

double CoilCoolingDXCurveFitSpeed::activeFractionofCoilFaceArea() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->activeFractionofCoilFaceArea();
}

bool CoilCoolingDXCurveFitSpeed::isActiveFractionofCoilFaceAreaDefaulted() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->isActiveFractionofCoilFaceAreaDefaulted();
}

double CoilCoolingDXCurveFitSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
}

double CoilCoolingDXCurveFitSpeed::evaporativeCondenserPumpPowerFraction() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->evaporativeCondenserPumpPowerFraction();
}

bool CoilCoolingDXCurveFitSpeed::isEvaporativeCondenserPumpPowerFractionDefaulted() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->isEvaporativeCondenserPumpPowerFractionDefaulted();
}

double CoilCoolingDXCurveFitSpeed::evaporativeCondenserEffectiveness() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->evaporativeCondenserEffectiveness();
}

Curve CoilCoolingDXCurveFitSpeed::totalCoolingCapacityModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->totalCoolingCapacityModifierFunctionofTemperatureCurve();
}

Curve CoilCoolingDXCurveFitSpeed::totalCoolingCapacityModifierFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->totalCoolingCapacityModifierFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingDXCurveFitSpeed::energyInputRatioModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->energyInputRatioModifierFunctionofTemperatureCurve();
}

Curve CoilCoolingDXCurveFitSpeed::energyInputRatioModifierFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->energyInputRatioModifierFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingDXCurveFitSpeed::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->partLoadFractionCorrelationCurve();
}

double CoilCoolingDXCurveFitSpeed::ratedWasteHeatFractionofPowerInput() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->ratedWasteHeatFractionofPowerInput();
}

Curve CoilCoolingDXCurveFitSpeed::wasteHeatModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->wasteHeatModifierFunctionofTemperatureCurve();
}

Curve CoilCoolingDXCurveFitSpeed::sensibleHeatRatioModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->sensibleHeatRatioModifierFunctionofTemperatureCurve();
}

Curve CoilCoolingDXCurveFitSpeed::sensibleHeatRatioModifierFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->sensibleHeatRatioModifierFunctionofFlowFractionCurve();
}

bool CoilCoolingDXCurveFitSpeed::setGrossTotalCoolingCapacityFraction(double grossTotalCoolingCapacityFraction) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setGrossTotalCoolingCapacityFraction(grossTotalCoolingCapacityFraction);
}

bool CoilCoolingDXCurveFitSpeed::setEvaporatorAirFlowRateFraction(double evaporatorAirFlowRateFraction) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEvaporatorAirFlowRateFraction(evaporatorAirFlowRateFraction);
}

bool CoilCoolingDXCurveFitSpeed::setCondenserAirFlowRateFraction(double condenserAirFlowRateFraction) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setCondenserAirFlowRateFraction(condenserAirFlowRateFraction);
}

void CoilCoolingDXCurveFitSpeed::resetCondenserAirFlowRateFraction() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetCondenserAirFlowRateFraction();
}

bool CoilCoolingDXCurveFitSpeed::setGrossSensibleHeatRatio(double grossSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setGrossSensibleHeatRatio(grossSensibleHeatRatio);
}

void CoilCoolingDXCurveFitSpeed::autosizeGrossSensibleHeatRatio() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->autosizeGrossSensibleHeatRatio();
}

bool CoilCoolingDXCurveFitSpeed::setGrossCoolingCOP(double grossCoolingCOP) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setGrossCoolingCOP(grossCoolingCOP);
}

bool CoilCoolingDXCurveFitSpeed::setActiveFractionofCoilFaceArea(double activeFractionofCoilFaceArea) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setActiveFractionofCoilFaceArea(activeFractionofCoilFaceArea);
}

void CoilCoolingDXCurveFitSpeed::resetActiveFractionofCoilFaceArea() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetActiveFractionofCoilFaceArea();
}

bool CoilCoolingDXCurveFitSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate(ratedEvaporatorFanPowerPerVolumeFlowRate);
}

bool CoilCoolingDXCurveFitSpeed::setEvaporativeCondenserPumpPowerFraction(double evaporativeCondenserPumpPowerFraction) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEvaporativeCondenserPumpPowerFraction(evaporativeCondenserPumpPowerFraction);
}

void CoilCoolingDXCurveFitSpeed::resetEvaporativeCondenserPumpPowerFraction() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetEvaporativeCondenserPumpPowerFraction();
}

bool CoilCoolingDXCurveFitSpeed::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
}

bool CoilCoolingDXCurveFitSpeed::setTotalCoolingCapacityModifierFunctionofTemperatureCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setTotalCoolingCapacityModifierFunctionofTemperatureCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetTotalCoolingCapacityModifierFunctionofTemperatureCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetTotalCoolingCapacityModifierFunctionofTemperatureCurve();
}

bool CoilCoolingDXCurveFitSpeed::setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve();
}

bool CoilCoolingDXCurveFitSpeed::setEnergyInputRatioModifierFunctionofTemperatureCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEnergyInputRatioModifierFunctionofTemperatureCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetEnergyInputRatioModifierFunctionofTemperatureCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetEnergyInputRatioModifierFunctionofTemperatureCurve();
}

bool CoilCoolingDXCurveFitSpeed::setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve();
}

bool CoilCoolingDXCurveFitSpeed::setPartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setPartLoadFractionCorrelationCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetPartLoadFractionCorrelationCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetPartLoadFractionCorrelationCurve();
}

bool CoilCoolingDXCurveFitSpeed::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setRatedWasteHeatFractionofPowerInput(ratedWasteHeatFractionofPowerInput);
}

bool CoilCoolingDXCurveFitSpeed::setWasteHeatModifierFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setWasteHeatModifierFunctionofTemperatureCurve(bivariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetWasteHeatModifierFunctionofTemperatureCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetWasteHeatModifierFunctionofTemperatureCurve();
}

bool CoilCoolingDXCurveFitSpeed::setSensibleHeatRatioModifierFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setSensibleHeatRatioModifierFunctionofTemperatureCurve(bivariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetSensibleHeatRatioModifierFunctionofTemperatureCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetSensibleHeatRatioModifierFunctionofTemperatureCurve();
}

bool CoilCoolingDXCurveFitSpeed::setSensibleHeatRatioModifierFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setSensibleHeatRatioModifierFunctionofFlowFractionCurve(univariateFunctions);
}

void CoilCoolingDXCurveFitSpeed::resetSensibleHeatRatioModifierFunctionofFlowFractionCurve() {
  getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetSensibleHeatRatioModifierFunctionofFlowFractionCurve();
}

/// @cond
CoilCoolingDXCurveFitSpeed::CoilCoolingDXCurveFitSpeed(std::shared_ptr<detail::CoilCoolingDXCurveFitSpeed_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

