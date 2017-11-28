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

#include "../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../model/CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "../model/Curve.hpp"
#include "../model/Curve_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveBiquadratic_Impl.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveQuadratic_Impl.hpp"
#include <utilities/idd/OS_Coil_Cooling_DX_MultiSpeed_StageData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXMultiSpeedStageData_Impl::CoilCoolingDXMultiSpeedStageData_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXMultiSpeedStageData::iddObjectType());
  }

  CoilCoolingDXMultiSpeedStageData_Impl::CoilCoolingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXMultiSpeedStageData::iddObjectType());
  }

  CoilCoolingDXMultiSpeedStageData_Impl::CoilCoolingDXMultiSpeedStageData_Impl(const CoilCoolingDXMultiSpeedStageData_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXMultiSpeedStageData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXMultiSpeedStageData_Impl::iddObjectType() const {
    return CoilCoolingDXMultiSpeedStageData::iddObjectType();
  }

  boost::optional<double> CoilCoolingDXMultiSpeedStageData_Impl::grossRatedTotalCoolingCapacity() const {
    return getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedTotalCoolingCapacity,true);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::isGrossRatedTotalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedTotalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingDXMultiSpeedStageData_Impl::grossRatedSensibleHeatRatio() const {
    return getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio,true);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::isGrossRatedSensibleHeatRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::grossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXMultiSpeedStageData_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedAirFlowRate,true);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporatorFanPowerPerVolumeFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::totalCoolingCapacityFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::nominalTimeforCondensateRemovaltoBegin() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::NominalTimeforCondensateRemovaltoBegin,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::maximumCyclingRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::MaximumCyclingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::latentCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::LatentCapacityTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::ratedWasteHeatFractionofPowerInput() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingDXMultiSpeedStageData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    auto curve = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurve);
    if (!curve) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Function of Temperature Curve attached.");
    }
    return curve.get();
  }

  double CoilCoolingDXMultiSpeedStageData_Impl::evaporativeCondenserEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXMultiSpeedStageData_Impl::evaporativeCondenserAirFlowRate() const {
    return getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate,true);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingDXMultiSpeedStageData_Impl::ratedEvaporativeCondenserPumpPowerConsumption() const {
    return getDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption,true);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setGrossRatedTotalCoolingCapacity(boost::optional<double> grossRatedTotalCoolingCapacity) {
    bool result(false);
    if (grossRatedTotalCoolingCapacity) {
      result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedTotalCoolingCapacity, grossRatedTotalCoolingCapacity.get());
    }
    return result;
  }

  void CoilCoolingDXMultiSpeedStageData_Impl::autosizeGrossRatedTotalCoolingCapacity() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedTotalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio) {
    bool result(false);
    if (grossRatedSensibleHeatRatio) {
      result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, grossRatedSensibleHeatRatio.get());
    }
    return result;
  }

  void CoilCoolingDXMultiSpeedStageData_Impl::autosizeGrossRatedSensibleHeatRatio() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::GrossRatedCoolingCOP, grossRatedCoolingCOP);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    return result;
  }

  void CoilCoolingDXMultiSpeedStageData_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporatorFanPowerPerVolumeFlowRate, ratedEvaporatorFanPowerPerVolumeFlowRate);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::TotalCoolingCapacityFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::TotalCoolingCapacityFunctionofFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::NominalTimeforCondensateRemovaltoBegin, nominalTimeforCondensateRemovaltoBegin);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setMaximumCyclingRate(double maximumCyclingRate) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::MaximumCyclingRate, maximumCyclingRate);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::LatentCapacityTimeConstant, latentCapacityTimeConstant);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput, ratedWasteHeatFractionofPowerInput);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
    return result;
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate) {
    bool result(false);
    if (evaporativeCondenserAirFlowRate) {
      result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate.get());
    }
    return result;
  }

  void CoilCoolingDXMultiSpeedStageData_Impl::autosizeEvaporativeCondenserAirFlowRate() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeedStageData_Impl::setRatedEvaporativeCondenserPumpPowerConsumption(boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption) {
    bool result(false);
    if (ratedEvaporativeCondenserPumpPowerConsumption) {
      result = setDouble(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, ratedEvaporativeCondenserPumpPowerConsumption.get());
    }
    return result;
  }

  void CoilCoolingDXMultiSpeedStageData_Impl::autosizeRatedEvaporativeCondenserPumpPowerConsumption() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<Curve> CoilCoolingDXMultiSpeedStageData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::TotalCoolingCapacityFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilCoolingDXMultiSpeedStageData_Impl::optionalTotalCoolingCapacityFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::TotalCoolingCapacityFunctionofFlowFractionCurve);
  }

  boost::optional<Curve> CoilCoolingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilCoolingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurve);
  }

  boost::optional<Curve> CoilCoolingDXMultiSpeedStageData_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurve);
  }

  std::vector<ModelObject> CoilCoolingDXMultiSpeedStageData_Impl::children() const {
    std::vector<ModelObject> result;

    result.push_back(totalCoolingCapacityFunctionofTemperatureCurve());
    result.push_back(totalCoolingCapacityFunctionofFlowFractionCurve());
    result.push_back(energyInputRatioFunctionofTemperatureCurve());
    result.push_back(energyInputRatioFunctionofFlowFractionCurve());
    result.push_back(partLoadFractionCorrelationCurve());
    result.push_back(wasteHeatFunctionofTemperatureCurve());

    return result;
  }

  ModelObject CoilCoolingDXMultiSpeedStageData_Impl::clone(Model model) const {
    auto t_clone = ModelObject_Impl::clone(model).cast<CoilCoolingDXMultiSpeedStageData>();

    return t_clone;
  }

} // detail

CoilCoolingDXMultiSpeedStageData::CoilCoolingDXMultiSpeedStageData(const Model& model)
  : ParentObject(CoilCoolingDXMultiSpeedStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>());

  autosizeGrossRatedTotalCoolingCapacity();
  autosizeGrossRatedSensibleHeatRatio();
  setGrossRatedCoolingCOP(3.0);
  autosizeRatedAirFlowRate();
  autosizeRatedEvaporativeCondenserPumpPowerConsumption();
  setNominalTimeforCondensateRemovaltoBegin(0.0);
  setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.0);
  setMaximumCyclingRate(0.0);
  setLatentCapacityTimeConstant(0.0);
  setRatedWasteHeatFractionofPowerInput(0.0);
  setEvaporativeCondenserEffectiveness(0.9);
  autosizeEvaporativeCondenserAirFlowRate();
  autosizeRatedEvaporativeCondenserPumpPowerConsumption();
  setRatedEvaporatorFanPowerPerVolumeFlowRate(773.3);
  setRatedWasteHeatFractionofPowerInput(0.5);

  CurveBiquadratic coolingCapacityFunctionofTemperature(model);
  coolingCapacityFunctionofTemperature.setCoefficient1Constant(0.766956);
  coolingCapacityFunctionofTemperature.setCoefficient2x(0.0107756);
  coolingCapacityFunctionofTemperature.setCoefficient3xPOW2(-0.0000414703);
  coolingCapacityFunctionofTemperature.setCoefficient4y(0.00134961);
  coolingCapacityFunctionofTemperature.setCoefficient5yPOW2(-0.000261144);
  coolingCapacityFunctionofTemperature.setCoefficient6xTIMESY(0.000457488);
  coolingCapacityFunctionofTemperature.setMinimumValueofx(17.0);
  coolingCapacityFunctionofTemperature.setMaximumValueofx(22.0);
  coolingCapacityFunctionofTemperature.setMinimumValueofy(13.0);
  coolingCapacityFunctionofTemperature.setMaximumValueofy(46.0);
  setTotalCoolingCapacityFunctionofTemperatureCurve(coolingCapacityFunctionofTemperature);

  CurveQuadratic coolingCapacityFuncionofFlowFraction(model);
  coolingCapacityFuncionofFlowFraction.setCoefficient1Constant(0.8);
  coolingCapacityFuncionofFlowFraction.setCoefficient2x(0.2);
  coolingCapacityFuncionofFlowFraction.setCoefficient3xPOW2(0.0);
  coolingCapacityFuncionofFlowFraction.setMinimumValueofx(0.5);
  coolingCapacityFuncionofFlowFraction.setMaximumValueofx(1.5);
  setTotalCoolingCapacityFunctionofFlowFractionCurve(coolingCapacityFuncionofFlowFraction);

  CurveBiquadratic energyInputRatioFunctionofTemperature(model);
  energyInputRatioFunctionofTemperature.setCoefficient1Constant(0.297145);
  energyInputRatioFunctionofTemperature.setCoefficient2x(0.0430933);
  energyInputRatioFunctionofTemperature.setCoefficient3xPOW2(-0.000748766);
  energyInputRatioFunctionofTemperature.setCoefficient4y(0.00597727);
  energyInputRatioFunctionofTemperature.setCoefficient5yPOW2(0.000482112);
  energyInputRatioFunctionofTemperature.setCoefficient6xTIMESY(-0.000956448);
  energyInputRatioFunctionofTemperature.setMinimumValueofx(17.0);
  energyInputRatioFunctionofTemperature.setMaximumValueofx(22.0);
  energyInputRatioFunctionofTemperature.setMinimumValueofy(13.0);
  energyInputRatioFunctionofTemperature.setMaximumValueofy(46.0);
  setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperature);

  CurveQuadratic energyInputRatioFunctionofFlowFraction(model);
  energyInputRatioFunctionofFlowFraction.setCoefficient1Constant(1.156);
  energyInputRatioFunctionofFlowFraction.setCoefficient2x(-0.1816);
  energyInputRatioFunctionofFlowFraction.setCoefficient3xPOW2(0.0256);
  energyInputRatioFunctionofFlowFraction.setMinimumValueofx(0.5);
  energyInputRatioFunctionofFlowFraction.setMaximumValueofx(1.5);
  setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFraction);

  CurveQuadratic partLoadFractionCorrelation(model);
  partLoadFractionCorrelation.setCoefficient1Constant(0.75);
  partLoadFractionCorrelation.setCoefficient2x(0.25);
  partLoadFractionCorrelation.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelation.setMinimumValueofx(0.0);
  partLoadFractionCorrelation.setMaximumValueofx(1.0);
  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelation);

  CurveBiquadratic wasteHeatFunctionofTemperature(model);
  wasteHeatFunctionofTemperature.setCoefficient1Constant(1);
  wasteHeatFunctionofTemperature.setCoefficient2x(0.0);
  wasteHeatFunctionofTemperature.setCoefficient3xPOW2(0.0);
  wasteHeatFunctionofTemperature.setCoefficient4y(0.0);
  wasteHeatFunctionofTemperature.setCoefficient5yPOW2(0.0);
  wasteHeatFunctionofTemperature.setCoefficient6xTIMESY(0.0);
  wasteHeatFunctionofTemperature.setMinimumValueofx(0.0);
  wasteHeatFunctionofTemperature.setMaximumValueofx(0.0);
  wasteHeatFunctionofTemperature.setMinimumValueofy(0.0);
  wasteHeatFunctionofTemperature.setMaximumValueofy(0.0);
  setWasteHeatFunctionofTemperatureCurve(wasteHeatFunctionofTemperature);
}

CoilCoolingDXMultiSpeedStageData::CoilCoolingDXMultiSpeedStageData(const Model& model,
  Curve& coolingCapacityFunctionofTemperature,
  Curve& coolingCapacityFuncionofFlowFraction,
  Curve& energyInputRatioFunctionofTemperature,
  Curve& energyInputRatioFunctionofFlowFraction,
  Curve& partLoadFractionCorrelation,
  Curve& wasteHeatFunctionofTemperature)
  : ParentObject(CoilCoolingDXMultiSpeedStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>());

  autosizeGrossRatedTotalCoolingCapacity();
  autosizeGrossRatedSensibleHeatRatio();
  setGrossRatedCoolingCOP(3.0);
  autosizeRatedAirFlowRate();
  autosizeRatedEvaporativeCondenserPumpPowerConsumption();
  setTotalCoolingCapacityFunctionofTemperatureCurve(coolingCapacityFunctionofTemperature);
  setTotalCoolingCapacityFunctionofFlowFractionCurve(coolingCapacityFuncionofFlowFraction);
  setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperature);
  setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFraction);
  setWasteHeatFunctionofTemperatureCurve(wasteHeatFunctionofTemperature);
  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelation);
  setNominalTimeforCondensateRemovaltoBegin(0.0);
  setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.0);
  setMaximumCyclingRate(0.0);
  setLatentCapacityTimeConstant(0.0);
  setRatedWasteHeatFractionofPowerInput(0.0);
  setEvaporativeCondenserEffectiveness(0.9);
  autosizeEvaporativeCondenserAirFlowRate();
  autosizeRatedEvaporativeCondenserPumpPowerConsumption();
  setRatedEvaporatorFanPowerPerVolumeFlowRate(773.3);
  setRatedWasteHeatFractionofPowerInput(0.5);
}

IddObjectType CoilCoolingDXMultiSpeedStageData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_MultiSpeed_StageData);
}

boost::optional<double> CoilCoolingDXMultiSpeedStageData::grossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->grossRatedTotalCoolingCapacity();
}

bool CoilCoolingDXMultiSpeedStageData::isGrossRatedTotalCoolingCapacityAutosized() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
}

boost::optional<double> CoilCoolingDXMultiSpeedStageData::grossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->grossRatedSensibleHeatRatio();
}

bool CoilCoolingDXMultiSpeedStageData::isGrossRatedSensibleHeatRatioAutosized() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->isGrossRatedSensibleHeatRatioAutosized();
}

double CoilCoolingDXMultiSpeedStageData::grossRatedCoolingCOP() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->grossRatedCoolingCOP();
}

boost::optional<double> CoilCoolingDXMultiSpeedStageData::ratedAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->ratedAirFlowRate();
}

bool CoilCoolingDXMultiSpeedStageData::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->isRatedAirFlowRateAutosized();
}

double CoilCoolingDXMultiSpeedStageData::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
}

Curve CoilCoolingDXMultiSpeedStageData::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

Curve CoilCoolingDXMultiSpeedStageData::totalCoolingCapacityFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->totalCoolingCapacityFunctionofFlowFractionCurve();
}

Curve CoilCoolingDXMultiSpeedStageData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilCoolingDXMultiSpeedStageData::energyInputRatioFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofFlowFractionCurve();
}

Curve CoilCoolingDXMultiSpeedStageData::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->partLoadFractionCorrelationCurve();
}

double CoilCoolingDXMultiSpeedStageData::nominalTimeforCondensateRemovaltoBegin() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->nominalTimeforCondensateRemovaltoBegin();
}

double CoilCoolingDXMultiSpeedStageData::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
}

double CoilCoolingDXMultiSpeedStageData::maximumCyclingRate() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->maximumCyclingRate();
}

double CoilCoolingDXMultiSpeedStageData::latentCapacityTimeConstant() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->latentCapacityTimeConstant();
}

double CoilCoolingDXMultiSpeedStageData::ratedWasteHeatFractionofPowerInput() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->ratedWasteHeatFractionofPowerInput();
}

Curve CoilCoolingDXMultiSpeedStageData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

double CoilCoolingDXMultiSpeedStageData::evaporativeCondenserEffectiveness() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->evaporativeCondenserEffectiveness();
}

boost::optional<double> CoilCoolingDXMultiSpeedStageData::evaporativeCondenserAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->evaporativeCondenserAirFlowRate();
}

bool CoilCoolingDXMultiSpeedStageData::isEvaporativeCondenserAirFlowRateAutosized() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
}

boost::optional<double> CoilCoolingDXMultiSpeedStageData::ratedEvaporativeCondenserPumpPowerConsumption() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->ratedEvaporativeCondenserPumpPowerConsumption();
}

bool CoilCoolingDXMultiSpeedStageData::isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->isRatedEvaporativeCondenserPumpPowerConsumptionAutosized();
}

bool CoilCoolingDXMultiSpeedStageData::setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setGrossRatedTotalCoolingCapacity(grossRatedTotalCoolingCapacity);
}

void CoilCoolingDXMultiSpeedStageData::autosizeGrossRatedTotalCoolingCapacity() {
  getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
}

bool CoilCoolingDXMultiSpeedStageData::setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setGrossRatedSensibleHeatRatio(grossRatedSensibleHeatRatio);
}

void CoilCoolingDXMultiSpeedStageData::autosizeGrossRatedSensibleHeatRatio() {
  getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->autosizeGrossRatedSensibleHeatRatio();
}

bool CoilCoolingDXMultiSpeedStageData::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
}

bool CoilCoolingDXMultiSpeedStageData::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilCoolingDXMultiSpeedStageData::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilCoolingDXMultiSpeedStageData::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate(ratedEvaporatorFanPowerPerVolumeFlowRate);
}

bool CoilCoolingDXMultiSpeedStageData::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setTotalCoolingCapacityFunctionofFlowFractionCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofFlowFractionCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setNominalTimeforCondensateRemovaltoBegin(nominalTimeforCondensateRemovaltoBegin);
}

bool CoilCoolingDXMultiSpeedStageData::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
}

bool CoilCoolingDXMultiSpeedStageData::setMaximumCyclingRate(double maximumCyclingRate) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}

bool CoilCoolingDXMultiSpeedStageData::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setLatentCapacityTimeConstant(latentCapacityTimeConstant);
}

bool CoilCoolingDXMultiSpeedStageData::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setRatedWasteHeatFractionofPowerInput(ratedWasteHeatFractionofPowerInput);
}

bool CoilCoolingDXMultiSpeedStageData::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXMultiSpeedStageData::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
}

bool CoilCoolingDXMultiSpeedStageData::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setEvaporativeCondenserAirFlowRate(evaporativeCondenserAirFlowRate);
}

void CoilCoolingDXMultiSpeedStageData::autosizeEvaporativeCondenserAirFlowRate() {
  getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
}

bool CoilCoolingDXMultiSpeedStageData::setRatedEvaporativeCondenserPumpPowerConsumption(double ratedEvaporativeCondenserPumpPowerConsumption) {
  return getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->setRatedEvaporativeCondenserPumpPowerConsumption(ratedEvaporativeCondenserPumpPowerConsumption);
}

void CoilCoolingDXMultiSpeedStageData::autosizeRatedEvaporativeCondenserPumpPowerConsumption() {
  getImpl<detail::CoilCoolingDXMultiSpeedStageData_Impl>()->autosizeRatedEvaporativeCondenserPumpPowerConsumption();
}

/// @cond
CoilCoolingDXMultiSpeedStageData::CoilCoolingDXMultiSpeedStageData(std::shared_ptr<detail::CoilCoolingDXMultiSpeedStageData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

