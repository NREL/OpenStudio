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

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CoilCoolingDXCurveFitOperatingMode.hpp"
#include "CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXCurveFitSpeed::iddObjectType());
    }

    CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXCurveFitSpeed::iddObjectType());
    }

    CoilCoolingDXCurveFitSpeed_Impl::CoilCoolingDXCurveFitSpeed_Impl(const CoilCoolingDXCurveFitSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXCurveFitSpeed_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CoilCoolingDXCurveFitSpeed_Impl::iddObjectType() const {
      return CoilCoolingDXCurveFitSpeed::iddObjectType();
    }

    ModelObject CoilCoolingDXCurveFitSpeed_Impl::clone(Model model) const {
      // clone the operating modes is already handle in ModelObject_Impl::clone since they are ResourceObjects
      // We don't do ParentObject_Impl::clone since it'll also CLONE the children...
      return ModelObject_Impl::clone(model);
    }

    std::vector<ModelObject> CoilCoolingDXCurveFitSpeed_Impl::children() const {
      std::vector<ModelObject> result;

      // These are ResourceObjects, so they shouldn't really be children except for OS App / IG
      if (auto _c = totalCoolingCapacityModifierFunctionofTemperatureCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = totalCoolingCapacityModifierFunctionofAirFlowFractionCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = energyInputRatioModifierFunctionofTemperatureCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = energyInputRatioModifierFunctionofAirFlowFractionCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = partLoadFractionCorrelationCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = wasteHeatModifierFunctionofTemperatureCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = sensibleHeatRatioModifierFunctionofTemperatureCurve()) {
        result.push_back(_c.get());
      }
      if (auto _c = sensibleHeatRatioModifierFunctionofFlowFractionCurve()) {
        result.push_back(_c.get());
      }

      return result;
    }

    std::vector<IdfObject> CoilCoolingDXCurveFitSpeed_Impl::remove() {
      for (auto& opMode : coilCoolingDXCurveFitOperatingModes()) {
        opMode.removeSpeed(getObject<CoilCoolingDXCurveFitSpeed>());
      }
      return ResourceObject_Impl::remove();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::grossTotalCoolingCapacityFraction() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::evaporatorAirFlowRateFraction() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::condenserAirFlowRateFraction() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXCurveFitSpeed_Impl::grossSensibleHeatRatio() const {
      return getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, true);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::isGrossSensibleHeatRatioAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilCoolingDXCurveFitSpeed_Impl::grossCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::activeFractionofCoilFaceArea() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::evaporativeCondenserPumpPowerFraction() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXCurveFitSpeed_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::totalCoolingCapacityModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::totalCoolingCapacityModifierFunctionofAirFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::energyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::energyInputRatioModifierFunctionofAirFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::partLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::wasteHeatModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve);
    }

    double CoilCoolingDXCurveFitSpeed_Impl::ratedWasteHeatFractionofPowerInput() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::sensibleHeatRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXCurveFitSpeed_Impl::sensibleHeatRatioModifierFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve);
    }

    std::vector<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitSpeed_Impl::coilCoolingDXCurveFitOperatingModes() const {
      return getObject<ModelObject>().getModelObjectSources<CoilCoolingDXCurveFitOperatingMode>(CoilCoolingDXCurveFitOperatingMode::iddObjectType());
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

    bool CoilCoolingDXCurveFitSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
      bool result =
        setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate, ratedEvaporatorFanPowerPerVolumeFlowRate);
      return result;
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setEvaporativeCondenserPumpPowerFraction(double evaporativeCondenserPumpPowerFraction) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, evaporativeCondenserPumpPowerFraction);
      return result;
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
      return result;
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setTotalCoolingCapacityModifierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetTotalCoolingCapacityModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetEnergyInputRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetPartLoadFractionCorrelationCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setWasteHeatModifierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetWasteHeatModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
      bool result = setDouble(OS_Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, ratedWasteHeatFractionofPowerInput);
      return result;
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setSensibleHeatRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetSensibleHeatRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXCurveFitSpeed_Impl::setSensibleHeatRatioModifierFunctionofFlowFractionCurve(const Curve& curve) {
      bool result = setPointer(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXCurveFitSpeed_Impl::resetSensibleHeatRatioModifierFunctionofFlowFractionCurve() {
      bool result = setString(OS_Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    boost::optional<double> CoilCoolingDXCurveFitSpeed_Impl::autosizedGrossSensibleHeatRatio() const {
      return getAutosizedValue("Design Size Gross Sensible Heat Ratio", "");
    }

    void CoilCoolingDXCurveFitSpeed_Impl::autosize() {
      autosizeGrossSensibleHeatRatio();
    }

    void CoilCoolingDXCurveFitSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedGrossSensibleHeatRatio();
      if (val) {
        setGrossSensibleHeatRatio(val.get());
      }
    }

  }  // namespace detail

  CoilCoolingDXCurveFitSpeed::CoilCoolingDXCurveFitSpeed(const Model& model) : ResourceObject(CoilCoolingDXCurveFitSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>());

    setGrossTotalCoolingCapacityFraction(1.0);
    setEvaporatorAirFlowRateFraction(1.0);
    setCondenserAirFlowRateFraction(1.0);
    autosizeGrossSensibleHeatRatio();
    setGrossCoolingCOP(3.0);
    setActiveFractionofCoilFaceArea(1.0);
    setRatedEvaporatorFanPowerPerVolumeFlowRate(773.3);
    setEvaporativeCondenserPumpPowerFraction(1.0);
    setEvaporativeCondenserEffectiveness(0.9);
    setRatedWasteHeatFractionofPowerInput(0.2);

    //CurveBiquadratic totalCoolingCapacityModifierFunctionofTemperatureCurve(model);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient1Constant(0.766956);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient2x(0.0107756);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.0000414703);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient4y(0.00134961);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(-0.000261144);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.000457488);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setMinimumValueofx(17.0);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setMaximumValueofx(22.0);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setMinimumValueofy(13.0);
    //totalCoolingCapacityModifierFunctionofTemperatureCurve.setMaximumValueofy(46.0);
    //setTotalCoolingCapacityModifierFunctionofTemperatureCurve(totalCoolingCapacityModifierFunctionofTemperatureCurve);

    //CurveQuadratic totalCoolingCapacityModifierFunctionofAirFlowFractionCurve(model);
    //totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient1Constant(0.8);
    //totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient2x(0.2);
    //totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient3xPOW2(0.0);
    //totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setMinimumValueofx(0.5);
    //totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setMaximumValueofx(1.5);
    //setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve);

    //CurveBiquadratic energyInputRatioModifierFunctionofTemperatureCurve(model);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient1Constant(0.297145);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient2x(0.0430933);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.000748766);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient4y(0.00597727);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000482112);
    //energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.000956448);
    //energyInputRatioModifierFunctionofTemperatureCurve.setMinimumValueofx(17.0);
    //energyInputRatioModifierFunctionofTemperatureCurve.setMaximumValueofx(22.0);
    //energyInputRatioModifierFunctionofTemperatureCurve.setMinimumValueofy(13.0);
    //energyInputRatioModifierFunctionofTemperatureCurve.setMaximumValueofy(46.0);
    //setEnergyInputRatioModifierFunctionofTemperatureCurve(energyInputRatioModifierFunctionofTemperatureCurve);

    //CurveQuadratic energyInputRatioModifierFunctionofAirFlowFractionCurve(model);
    //energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient1Constant(1.156);
    //energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient2x(-0.1816);
    //energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient3xPOW2(0.0256);
    //energyInputRatioModifierFunctionofAirFlowFractionCurve.setMinimumValueofx(0.5);
    //energyInputRatioModifierFunctionofAirFlowFractionCurve.setMaximumValueofx(1.5);
    //setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(energyInputRatioModifierFunctionofAirFlowFractionCurve);

    //CurveQuadratic partLoadFractionCorrelationCurve(model);
    //partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
    //partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
    //partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
    //partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
    //partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);
    //setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);

    //CurveBiquadratic wasteHeatModifierFunctionofTemperatureCurve(model);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient2x(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient4y(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setMinimumValueofx(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setMaximumValueofx(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setMinimumValueofy(0.0);
    //wasteHeatModifierFunctionofTemperatureCurve.setMaximumValueofy(0.0);
    //setWasteHeatModifierFunctionofTemperatureCurve(wasteHeatModifierFunctionofTemperatureCurve);
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

  double CoilCoolingDXCurveFitSpeed::condenserAirFlowRateFraction() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->condenserAirFlowRateFraction();
  }

  boost::optional<double> CoilCoolingDXCurveFitSpeed::grossSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->grossSensibleHeatRatio();
  }

  bool CoilCoolingDXCurveFitSpeed::isGrossSensibleHeatRatioAutosized() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->isGrossSensibleHeatRatioAutosized();
  }

  double CoilCoolingDXCurveFitSpeed::grossCoolingCOP() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->grossCoolingCOP();
  }

  double CoilCoolingDXCurveFitSpeed::activeFractionofCoilFaceArea() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->activeFractionofCoilFaceArea();
  }

  double CoilCoolingDXCurveFitSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
  }

  double CoilCoolingDXCurveFitSpeed::evaporativeCondenserPumpPowerFraction() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->evaporativeCondenserPumpPowerFraction();
  }

  double CoilCoolingDXCurveFitSpeed::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->evaporativeCondenserEffectiveness();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::totalCoolingCapacityModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->totalCoolingCapacityModifierFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::totalCoolingCapacityModifierFunctionofAirFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->totalCoolingCapacityModifierFunctionofAirFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::energyInputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->energyInputRatioModifierFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::energyInputRatioModifierFunctionofAirFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->energyInputRatioModifierFunctionofAirFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::partLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->partLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::wasteHeatModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->wasteHeatModifierFunctionofTemperatureCurve();
  }

  double CoilCoolingDXCurveFitSpeed::ratedWasteHeatFractionofPowerInput() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->ratedWasteHeatFractionofPowerInput();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::sensibleHeatRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->sensibleHeatRatioModifierFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXCurveFitSpeed::sensibleHeatRatioModifierFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->sensibleHeatRatioModifierFunctionofFlowFractionCurve();
  }

  std::vector<CoilCoolingDXCurveFitOperatingMode> CoilCoolingDXCurveFitSpeed::coilCoolingDXCurveFitOperatingModes() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->coilCoolingDXCurveFitOperatingModes();
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

  bool CoilCoolingDXCurveFitSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate(ratedEvaporatorFanPowerPerVolumeFlowRate);
  }

  bool CoilCoolingDXCurveFitSpeed::setEvaporativeCondenserPumpPowerFraction(double evaporativeCondenserPumpPowerFraction) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEvaporativeCondenserPumpPowerFraction(evaporativeCondenserPumpPowerFraction);
  }

  bool CoilCoolingDXCurveFitSpeed::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
  }

  bool CoilCoolingDXCurveFitSpeed::setTotalCoolingCapacityModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setTotalCoolingCapacityModifierFunctionofTemperatureCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetTotalCoolingCapacityModifierFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetTotalCoolingCapacityModifierFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetEnergyInputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setPartLoadFractionCorrelationCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setWasteHeatModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setWasteHeatModifierFunctionofTemperatureCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetWasteHeatModifierFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetWasteHeatModifierFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setRatedWasteHeatFractionofPowerInput(ratedWasteHeatFractionofPowerInput);
  }

  bool CoilCoolingDXCurveFitSpeed::setSensibleHeatRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setSensibleHeatRatioModifierFunctionofTemperatureCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetSensibleHeatRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetSensibleHeatRatioModifierFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXCurveFitSpeed::setSensibleHeatRatioModifierFunctionofFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->setSensibleHeatRatioModifierFunctionofFlowFractionCurve(curve);
  }

  void CoilCoolingDXCurveFitSpeed::resetSensibleHeatRatioModifierFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->resetSensibleHeatRatioModifierFunctionofFlowFractionCurve();
  }

  boost::optional<double> CoilCoolingDXCurveFitSpeed::autosizedGrossSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->autosizedGrossSensibleHeatRatio();
  }

  void CoilCoolingDXCurveFitSpeed::autosize() {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->autosize();
  }

  void CoilCoolingDXCurveFitSpeed::applySizingValues() {
    return getImpl<detail::CoilCoolingDXCurveFitSpeed_Impl>()->applySizingValues();
  }

  /// @cond
  CoilCoolingDXCurveFitSpeed::CoilCoolingDXCurveFitSpeed(std::shared_ptr<detail::CoilCoolingDXCurveFitSpeed_Impl> impl) : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
