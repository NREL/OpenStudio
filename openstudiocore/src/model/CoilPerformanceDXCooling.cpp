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

#include "CoilPerformanceDXCooling.hpp"
#include "CoilPerformanceDXCooling_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_CoilPerformance_DX_Cooling_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilPerformanceDXCooling_Impl::CoilPerformanceDXCooling_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilPerformanceDXCooling::iddObjectType());
  }

  CoilPerformanceDXCooling_Impl::CoilPerformanceDXCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilPerformanceDXCooling::iddObjectType());
  }

  CoilPerformanceDXCooling_Impl::CoilPerformanceDXCooling_Impl(const CoilPerformanceDXCooling_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilPerformanceDXCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilPerformanceDXCooling_Impl::iddObjectType() const {
    return CoilPerformanceDXCooling::iddObjectType();
  }

  boost::optional<double> CoilPerformanceDXCooling_Impl::grossRatedTotalCoolingCapacity() const {
    return getDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity,true);
  }

  bool CoilPerformanceDXCooling_Impl::isGrossRatedTotalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilPerformanceDXCooling_Impl::grossRatedSensibleHeatRatio() const {
    return getDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio,true);
  }

  bool CoilPerformanceDXCooling_Impl::isGrossRatedSensibleHeatRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilPerformanceDXCooling_Impl::grossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilPerformanceDXCooling_Impl::ratedAirFlowRate() const {
    return getDouble(OS_CoilPerformance_DX_CoolingFields::RatedAirFlowRate,true);
  }

  bool CoilPerformanceDXCooling_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilPerformanceDXCooling_Impl::fractionofAirFlowBypassedAroundCoil() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::FractionofAirFlowBypassedAroundCoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilPerformanceDXCooling_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilPerformanceDXCooling_Impl::totalCoolingCapacityFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilPerformanceDXCooling_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilPerformanceDXCooling_Impl::energyInputRatioFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilPerformanceDXCooling_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  double CoilPerformanceDXCooling_Impl::nominalTimeforCondensateRemovaltoBegin() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::NominalTimeforCondensateRemovaltoBegin,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilPerformanceDXCooling_Impl::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilPerformanceDXCooling_Impl::maximumCyclingRate() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::MaximumCyclingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilPerformanceDXCooling_Impl::latentCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::LatentCapacityTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> CoilPerformanceDXCooling_Impl::condenserAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_CoilPerformance_DX_CoolingFields::CondenserAirInletNode);
  }

  std::string CoilPerformanceDXCooling_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilPerformanceDXCooling_Impl::evaporativeCondenserEffectiveness() const {
    boost::optional<double> value = getDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilPerformanceDXCooling_Impl::evaporativeCondenserAirFlowRate() const {
    return getDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate,true);
  }

  bool CoilPerformanceDXCooling_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilPerformanceDXCooling_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption,true);
  }

  bool CoilPerformanceDXCooling_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::sensibleHeatRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::sensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofFlowFractionCurve);
  }

  bool CoilPerformanceDXCooling_Impl::setGrossRatedTotalCoolingCapacity(boost::optional<double> grossRatedTotalCoolingCapacity) {
    bool result(false);
    if (grossRatedTotalCoolingCapacity) {
      result = setDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity, grossRatedTotalCoolingCapacity.get());
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::autosizeGrossRatedTotalCoolingCapacity() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio) {
    bool result(false);
    if (grossRatedSensibleHeatRatio) {
      result = setDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio, grossRatedSensibleHeatRatio.get());
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::autosizeGrossRatedSensibleHeatRatio() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio, "autosize");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::GrossRatedCoolingCOP, grossRatedCoolingCOP);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_CoilPerformance_DX_CoolingFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setFractionofAirFlowBypassedAroundCoil(double fractionofAirFlowBypassedAroundCoil) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::FractionofAirFlowBypassedAroundCoil, fractionofAirFlowBypassedAroundCoil);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_CoilPerformance_DX_CoolingFields::PartLoadFractionCorrelationCurve, curve.handle());
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::NominalTimeforCondensateRemovaltoBegin, nominalTimeforCondensateRemovaltoBegin);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setMaximumCyclingRate(double maximumCyclingRate) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::MaximumCyclingRate, maximumCyclingRate);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::LatentCapacityTimeConstant, latentCapacityTimeConstant);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setCondenserAirInletNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_CoilPerformance_DX_CoolingFields::CondenserAirInletNode, node.get().handle());
    }
    else {
      resetCondenserAirInletNode();
      result = true;
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::resetCondenserAirInletNode() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::CondenserAirInletNode, "");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setCondenserType(std::string condenserType) {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::CondenserType, condenserType);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    bool result = setDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
    return result;
  }

  bool CoilPerformanceDXCooling_Impl::setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate) {
    bool result(false);
    if (evaporativeCondenserAirFlowRate) {
      result = setDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate.get());
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::autosizeEvaporativeCondenserAirFlowRate() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
    bool result(false);
    if (evaporativeCondenserPumpRatedPowerConsumption) {
      result = setDouble(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption, evaporativeCondenserPumpRatedPowerConsumption.get());
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setSensibleHeatRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofTemperatureCurve, curve.get().handle());
    }
    else {
      resetSensibleHeatRatioFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::resetSensibleHeatRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofTemperatureCurve, "");
    OS_ASSERT(result);
  }

  bool CoilPerformanceDXCooling_Impl::setSensibleHeatRatioFunctionofFlowFractionCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofFlowFractionCurve, curve.get().handle());
    }
    else {
      resetSensibleHeatRatioFunctionofFlowFractionCurve();
      result = true;
    }
    return result;
  }

  void CoilPerformanceDXCooling_Impl::resetSensibleHeatRatioFunctionofFlowFractionCurve() {
    bool result = setString(OS_CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofFlowFractionCurve, "");
    OS_ASSERT(result);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::optionalTotalCoolingCapacityFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofFlowFractionCurve);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::optionalEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofFlowFractionCurve);
  }

  boost::optional<Curve> CoilPerformanceDXCooling_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_CoilPerformance_DX_CoolingFields::PartLoadFractionCorrelationCurve);
  }

  std::vector<ModelObject> CoilPerformanceDXCooling_Impl::children() const {
    std::vector<ModelObject> result;
    {
      auto mo = totalCoolingCapacityFunctionofTemperatureCurve();
      result.push_back(mo);
    }
    {
      auto mo = totalCoolingCapacityFunctionofFlowFractionCurve();
      result.push_back(mo);
    }
    {
      auto mo = energyInputRatioFunctionofTemperatureCurve();
      result.push_back(mo);
    }
    {
      auto mo = energyInputRatioFunctionofFlowFractionCurve();
      result.push_back(mo);
    }
    {
      auto mo = partLoadFractionCorrelationCurve();
      result.push_back(mo);
    }

    if( auto mo = sensibleHeatRatioFunctionofTemperatureCurve() ) {
      result.push_back(mo.get());
    }
    if( auto mo = sensibleHeatRatioFunctionofFlowFractionCurve() ) {
      result.push_back(mo.get());
    }
    return result;
  }

  ModelObject CoilPerformanceDXCooling_Impl::clone(Model model) const {
    auto newObject = ModelObject_Impl::clone(model).cast<CoilPerformanceDXCooling>();

    return newObject;
  }

} // detail

CoilPerformanceDXCooling::CoilPerformanceDXCooling(const Model& model,
  const Curve& coolingCapacityFunctionofTemperature,
  const Curve& coolingCapacityFunctionofFlowFraction,
  const Curve& energyInputRatioFunctionofTemperature,
  const Curve& energyInputRatioFunctionofFlowFraction,
  const Curve& partLoadFractionCorrelation)
  : ParentObject(CoilPerformanceDXCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilPerformanceDXCooling_Impl>());

  autosizeGrossRatedTotalCoolingCapacity();
  autosizeGrossRatedSensibleHeatRatio();
  setGrossRatedCoolingCOP(3.0);
  autosizeRatedAirFlowRate();
  setFractionofAirFlowBypassedAroundCoil(0.0);
  setNominalTimeforCondensateRemovaltoBegin(1000.0);
  setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(1.5);
  setMaximumCyclingRate(3.0);
  setLatentCapacityTimeConstant(45);
  setCondenserType("AirCooled");
  setEvaporativeCondenserEffectiveness(0.9);
  autosizeEvaporativeCondenserAirFlowRate();
  autosizeEvaporativeCondenserPumpRatedPowerConsumption();

  setTotalCoolingCapacityFunctionofTemperatureCurve(coolingCapacityFunctionofTemperature);
  setTotalCoolingCapacityFunctionofFlowFractionCurve(coolingCapacityFunctionofFlowFraction);
  setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperature);
  setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFraction);
  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelation);
}

CoilPerformanceDXCooling::CoilPerformanceDXCooling(const Model& model)
  : ParentObject(CoilPerformanceDXCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilPerformanceDXCooling_Impl>());

  autosizeGrossRatedTotalCoolingCapacity();
  autosizeGrossRatedSensibleHeatRatio();
  setGrossRatedCoolingCOP(3.0);
  autosizeRatedAirFlowRate();
  setFractionofAirFlowBypassedAroundCoil(0.0);
  setNominalTimeforCondensateRemovaltoBegin(1000.0);
  setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(1.5);
  setMaximumCyclingRate(3.0);
  setLatentCapacityTimeConstant(45);
  setCondenserType("AirCooled");
  setEvaporativeCondenserEffectiveness(0.9);
  autosizeEvaporativeCondenserAirFlowRate();
  autosizeEvaporativeCondenserPumpRatedPowerConsumption();

  {
    CurveBiquadratic curve(model);
    curve.setCoefficient1Constant(1.067939449);
    curve.setCoefficient2x(-0.031261829);
    curve.setCoefficient3xPOW2(0.001974308);
    curve.setCoefficient4y(-0.002726426);
    curve.setCoefficient5yPOW2(-5.52654E-05);
    curve.setCoefficient6xTIMESY(-6.31169E-05);
    curve.setMinimumValueofx(-100);
    curve.setMaximumValueofx(100);
    curve.setMinimumValueofy(-100);
    curve.setMaximumValueofy(100);
    setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(0.8);
    curve.setCoefficient2x(0.2);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.5);
    curve.setMaximumValueofx(1.5);
    setTotalCoolingCapacityFunctionofFlowFractionCurve(curve);
  }

  {
    CurveBiquadratic curve(model);
    curve.setCoefficient1Constant(0.174059889);
    curve.setCoefficient2x(0.022281508);
    curve.setCoefficient3xPOW2(-0.000134077);
    curve.setCoefficient4y(0.028298025);
    curve.setCoefficient5yPOW2(0.000485106);
    curve.setCoefficient6xTIMESY(-0.001677095);
    curve.setMinimumValueofx(-100);
    curve.setMaximumValueofx(100);
    curve.setMinimumValueofy(-100);
    curve.setMaximumValueofy(100);
    setEnergyInputRatioFunctionofTemperatureCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1552);
    curve.setCoefficient2x(0.1808);
    curve.setCoefficient3xPOW2(.0256);
    curve.setMinimumValueofx(.5);
    curve.setMaximumValueofx(1.5);
    setEnergyInputRatioFunctionofFlowFractionCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(0.85);
    curve.setCoefficient2x(0.15);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setPartLoadFractionCorrelationCurve(curve);
  }
}

IddObjectType CoilPerformanceDXCooling::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoilPerformance_DX_Cooling);
}

std::vector<std::string> CoilPerformanceDXCooling::condenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoilPerformance_DX_CoolingFields::CondenserType);
}

boost::optional<double> CoilPerformanceDXCooling::grossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->grossRatedTotalCoolingCapacity();
}

bool CoilPerformanceDXCooling::isGrossRatedTotalCoolingCapacityAutosized() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
}

boost::optional<double> CoilPerformanceDXCooling::grossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->grossRatedSensibleHeatRatio();
}

bool CoilPerformanceDXCooling::isGrossRatedSensibleHeatRatioAutosized() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->isGrossRatedSensibleHeatRatioAutosized();
}

double CoilPerformanceDXCooling::grossRatedCoolingCOP() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->grossRatedCoolingCOP();
}

boost::optional<double> CoilPerformanceDXCooling::ratedAirFlowRate() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->ratedAirFlowRate();
}

bool CoilPerformanceDXCooling::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->isRatedAirFlowRateAutosized();
}

double CoilPerformanceDXCooling::fractionofAirFlowBypassedAroundCoil() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->fractionofAirFlowBypassedAroundCoil();
}

Curve CoilPerformanceDXCooling::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

Curve CoilPerformanceDXCooling::totalCoolingCapacityFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->totalCoolingCapacityFunctionofFlowFractionCurve();
}

Curve CoilPerformanceDXCooling::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilPerformanceDXCooling::energyInputRatioFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->energyInputRatioFunctionofFlowFractionCurve();
}

Curve CoilPerformanceDXCooling::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->partLoadFractionCorrelationCurve();
}

double CoilPerformanceDXCooling::nominalTimeforCondensateRemovaltoBegin() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->nominalTimeforCondensateRemovaltoBegin();
}

double CoilPerformanceDXCooling::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
}

double CoilPerformanceDXCooling::maximumCyclingRate() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->maximumCyclingRate();
}

double CoilPerformanceDXCooling::latentCapacityTimeConstant() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->latentCapacityTimeConstant();
}

boost::optional<Node> CoilPerformanceDXCooling::condenserAirInletNode() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->condenserAirInletNode();
}

std::string CoilPerformanceDXCooling::condenserType() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->condenserType();
}

double CoilPerformanceDXCooling::evaporativeCondenserEffectiveness() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->evaporativeCondenserEffectiveness();
}

boost::optional<double> CoilPerformanceDXCooling::evaporativeCondenserAirFlowRate() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->evaporativeCondenserAirFlowRate();
}

bool CoilPerformanceDXCooling::isEvaporativeCondenserAirFlowRateAutosized() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
}

boost::optional<double> CoilPerformanceDXCooling::evaporativeCondenserPumpRatedPowerConsumption() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
}

bool CoilPerformanceDXCooling::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
}

boost::optional<Curve> CoilPerformanceDXCooling::sensibleHeatRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->sensibleHeatRatioFunctionofTemperatureCurve();
}

boost::optional<Curve> CoilPerformanceDXCooling::sensibleHeatRatioFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->sensibleHeatRatioFunctionofFlowFractionCurve();
}

bool CoilPerformanceDXCooling::setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setGrossRatedTotalCoolingCapacity(grossRatedTotalCoolingCapacity);
}

void CoilPerformanceDXCooling::autosizeGrossRatedTotalCoolingCapacity() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
}

bool CoilPerformanceDXCooling::setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setGrossRatedSensibleHeatRatio(grossRatedSensibleHeatRatio);
}

void CoilPerformanceDXCooling::autosizeGrossRatedSensibleHeatRatio() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->autosizeGrossRatedSensibleHeatRatio();
}

bool CoilPerformanceDXCooling::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
}

bool CoilPerformanceDXCooling::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilPerformanceDXCooling::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilPerformanceDXCooling::setFractionofAirFlowBypassedAroundCoil(double fractionofAirFlowBypassedAroundCoil) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setFractionofAirFlowBypassedAroundCoil(fractionofAirFlowBypassedAroundCoil);
}

bool CoilPerformanceDXCooling::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilPerformanceDXCooling::setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setTotalCoolingCapacityFunctionofFlowFractionCurve(curve);
}

bool CoilPerformanceDXCooling::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilPerformanceDXCooling::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setEnergyInputRatioFunctionofFlowFractionCurve(curve);
}

bool CoilPerformanceDXCooling::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

bool CoilPerformanceDXCooling::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setNominalTimeforCondensateRemovaltoBegin(nominalTimeforCondensateRemovaltoBegin);
}

bool CoilPerformanceDXCooling::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
}

bool CoilPerformanceDXCooling::setMaximumCyclingRate(double maximumCyclingRate) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}

bool CoilPerformanceDXCooling::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setLatentCapacityTimeConstant(latentCapacityTimeConstant);
}

bool CoilPerformanceDXCooling::setCondenserAirInletNode(const Node& node) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setCondenserAirInletNode(node);
}

void CoilPerformanceDXCooling::resetCondenserAirInletNode() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->resetCondenserAirInletNode();
}

bool CoilPerformanceDXCooling::setCondenserType(std::string condenserType) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setCondenserType(condenserType);
}

bool CoilPerformanceDXCooling::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
}

bool CoilPerformanceDXCooling::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setEvaporativeCondenserAirFlowRate(evaporativeCondenserAirFlowRate);
}

void CoilPerformanceDXCooling::autosizeEvaporativeCondenserAirFlowRate() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
}

bool CoilPerformanceDXCooling::setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(evaporativeCondenserPumpRatedPowerConsumption);
}

void CoilPerformanceDXCooling::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
}

bool CoilPerformanceDXCooling::setSensibleHeatRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setSensibleHeatRatioFunctionofTemperatureCurve(curve);
}

void CoilPerformanceDXCooling::resetSensibleHeatRatioFunctionofTemperatureCurve() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->resetSensibleHeatRatioFunctionofTemperatureCurve();
}

bool CoilPerformanceDXCooling::setSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilPerformanceDXCooling_Impl>()->setSensibleHeatRatioFunctionofFlowFractionCurve(curve);
}

void CoilPerformanceDXCooling::resetSensibleHeatRatioFunctionofFlowFractionCurve() {
  getImpl<detail::CoilPerformanceDXCooling_Impl>()->resetSensibleHeatRatioFunctionofFlowFractionCurve();
}

/// @cond
CoilPerformanceDXCooling::CoilPerformanceDXCooling(std::shared_ptr<detail::CoilPerformanceDXCooling_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

