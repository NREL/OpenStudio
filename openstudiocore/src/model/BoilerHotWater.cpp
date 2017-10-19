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

#include "BoilerHotWater.hpp"
#include "BoilerHotWater_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Boiler_HotWater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  BoilerHotWater_Impl::BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == BoilerHotWater::iddObjectType());
  }

  BoilerHotWater_Impl::BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == BoilerHotWater::iddObjectType());
  }

  BoilerHotWater_Impl::BoilerHotWater_Impl(const BoilerHotWater_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& BoilerHotWater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType BoilerHotWater_Impl::iddObjectType() const {
    return BoilerHotWater::iddObjectType();
  }

  std::vector<ModelObject> BoilerHotWater_Impl::children() const {
    ModelObjectVector result = StraightComponent_Impl::children();
    if (OptionalCurve curve = normalizedBoilerEfficiencyCurve()) {
      result.push_back(*curve);
    }
    return result;
  }

  unsigned BoilerHotWater_Impl::inletPort()
  {
    return OS_Boiler_HotWaterFields::BoilerWaterInletNodeName;
  }

  unsigned BoilerHotWater_Impl::outletPort()
  {
    return OS_Boiler_HotWaterFields::BoilerWaterOutletNodeName;
  }

  std::string BoilerHotWater_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> BoilerHotWater_Impl::nominalCapacity() const {
    return getDouble(OS_Boiler_HotWaterFields::NominalCapacity,true);
  }

  bool BoilerHotWater_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double BoilerHotWater_Impl::nominalThermalEfficiency() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::NominalThermalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> BoilerHotWater_Impl::efficiencyCurveTemperatureEvaluationVariable() const {
    return getString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable,true);
  }

  boost::optional<Curve> BoilerHotWater_Impl::normalizedBoilerEfficiencyCurve() const {
    return getObject<BoilerHotWater>().getModelObjectTarget<Curve>(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName);
  }

  boost::optional<double> BoilerHotWater_Impl::designWaterOutletTemperature() const {
    return getDouble(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature,true);
  }

  boost::optional<double> BoilerHotWater_Impl::designWaterFlowRate() const {
    return getDouble(OS_Boiler_HotWaterFields::DesignWaterFlowRate,true);
  }

  bool BoilerHotWater_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double BoilerHotWater_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isMinimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::MinimumPartLoadRatio);
  }

  double BoilerHotWater_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isMaximumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::MaximumPartLoadRatio);
  }

  double BoilerHotWater_Impl::optimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::OptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isOptimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::OptimumPartLoadRatio);
  }

  double BoilerHotWater_Impl::waterOutletUpperTemperatureLimit() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isWaterOutletUpperTemperatureLimitDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit);
  }

  std::string BoilerHotWater_Impl::boilerFlowMode() const {
    boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::BoilerFlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isBoilerFlowModeDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::BoilerFlowMode);
  }

  boost::optional<double> BoilerHotWater_Impl::parasiticElectricLoad() const {
    return getDouble(OS_Boiler_HotWaterFields::ParasiticElectricLoad,true);
  }

  double BoilerHotWater_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::SizingFactor);
  }

  bool BoilerHotWater_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_Boiler_HotWaterFields::FuelType, fuelType);
    return result;
  }

  bool BoilerHotWater_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result = false;
    if (nominalCapacity) {
      result = setDouble(OS_Boiler_HotWaterFields::NominalCapacity, nominalCapacity.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::NominalCapacity, "");
    }
    return result;
  }

  void BoilerHotWater_Impl::resetNominalCapacity() {
    bool result = setString(OS_Boiler_HotWaterFields::NominalCapacity, "");
    OS_ASSERT(result);
  }

  void BoilerHotWater_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Boiler_HotWaterFields::NominalCapacity, "Autosize");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setNominalThermalEfficiency(double nominalThermalEfficiency) {
    bool result = setDouble(OS_Boiler_HotWaterFields::NominalThermalEfficiency, nominalThermalEfficiency);
    return result;
  }

  bool BoilerHotWater_Impl::setEfficiencyCurveTemperatureEvaluationVariable(boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable) {
    bool result = false;
    if (efficiencyCurveTemperatureEvaluationVariable) {
      result = setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, efficiencyCurveTemperatureEvaluationVariable.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, "");
    }
    return result;
  }

  void BoilerHotWater_Impl::resetEfficiencyCurveTemperatureEvaluationVariable() {
    bool result = setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setNormalizedBoilerEfficiencyCurve(
        const boost::optional<Curve>& normalizedBoilerEfficiencyCurve)
  {
    if (normalizedBoilerEfficiencyCurve) {
      return setPointer(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName,normalizedBoilerEfficiencyCurve->handle());
    }
    else {
      return setString(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName,"");
    }
    return false;
  }

  void BoilerHotWater_Impl::resetNormalizedBoilerEfficiencyCurve() {
    bool ok = setNormalizedBoilerEfficiencyCurve(boost::none);
    OS_ASSERT(ok);
  }

  void BoilerHotWater_Impl::setDesignWaterOutletTemperature(boost::optional<double> designWaterOutletTemperature) {
    bool result = false;
    if (designWaterOutletTemperature) {
      result = setDouble(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, designWaterOutletTemperature.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, "");
    }
    OS_ASSERT(result);
  }

  void BoilerHotWater_Impl::resetDesignWaterOutletTemperature() {
    bool result = setString(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result = false;
    if (designWaterFlowRate) {
      result = setDouble(OS_Boiler_HotWaterFields::DesignWaterFlowRate, designWaterFlowRate.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::DesignWaterFlowRate, "");
    }
    return result;
  }

  void BoilerHotWater_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_Boiler_HotWaterFields::DesignWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void BoilerHotWater_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_Boiler_HotWaterFields::DesignWaterFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = setDouble(OS_Boiler_HotWaterFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetMinimumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::MinimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = setDouble(OS_Boiler_HotWaterFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetMaximumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::MaximumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    bool result = setDouble(OS_Boiler_HotWaterFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetOptimumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::OptimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  void BoilerHotWater_Impl::setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit) {
    bool result = setDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, waterOutletUpperTemperatureLimit);
    OS_ASSERT(result);
  }

  void BoilerHotWater_Impl::resetWaterOutletUpperTemperatureLimit() {
    bool result = setString(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setBoilerFlowMode(std::string boilerFlowMode) {
    bool result = false;
    if(istringEqual(boilerFlowMode,"VariableFlow"))
    {
      // Support legacy key
      result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode,"LeavingSetpointModulated");
    }
    else
    {
      result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode, boilerFlowMode);
    }
    return result;
  }

  void BoilerHotWater_Impl::resetBoilerFlowMode() {
    bool result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setParasiticElectricLoad(boost::optional<double> parasiticElectricLoad) {
    bool result = false;
    if (parasiticElectricLoad) {
      result = setDouble(OS_Boiler_HotWaterFields::ParasiticElectricLoad, parasiticElectricLoad.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::ParasiticElectricLoad, "");
    }
    return result;
  }

  void BoilerHotWater_Impl::resetParasiticElectricLoad() {
    bool result = setString(OS_Boiler_HotWaterFields::ParasiticElectricLoad, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_Boiler_HotWaterFields::SizingFactor, sizingFactor);
    return result;
  }

  void BoilerHotWater_Impl::resetSizingFactor() {
    bool result = setString(OS_Boiler_HotWaterFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  bool BoilerHotWater_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode(node) )
        {
          plant->setFluidType("Water");
          return true;
        }
      }
    }

    return false;
  }

  boost::optional<ModelObject> BoilerHotWater_Impl::normalizedBoilerEfficiencyCurveAsModelObject() const {
    OptionalModelObject result;
    OptionalCurve intermediate = normalizedBoilerEfficiencyCurve();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool BoilerHotWater_Impl::setNormalizedBoilerEfficiencyCurveAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalCurve intermediate = modelObject->optionalCast<Curve>();
      if (intermediate) {
        return setNormalizedBoilerEfficiencyCurve(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetNormalizedBoilerEfficiencyCurve();
    }
    return true;
  }

  ModelObject BoilerHotWater_Impl::clone(Model model) const
  {
    BoilerHotWater newBoiler = StraightComponent_Impl::clone(model).cast<BoilerHotWater>();

    return newBoiler;
  }

} // detail

BoilerHotWater::BoilerHotWater(const Model& model)
  : StraightComponent(BoilerHotWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::BoilerHotWater_Impl>());

  setWaterOutletUpperTemperatureLimit(99.0);

  setBoilerFlowMode("ConstantFlow");

  setParasiticElectricLoad(0.0);

  setSizingFactor(1.0);
}

IddObjectType BoilerHotWater::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Boiler_HotWater);
  return result;
}

std::vector<std::string> BoilerHotWater::validFuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Boiler_HotWaterFields::FuelType);
}

std::vector<std::string> BoilerHotWater::validEfficiencyCurveTemperatureEvaluationVariableValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable);
}

std::vector<std::string> BoilerHotWater::validBoilerFlowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Boiler_HotWaterFields::BoilerFlowMode);
}

std::string BoilerHotWater::fuelType() const {
  return getImpl<detail::BoilerHotWater_Impl>()->fuelType();
}

boost::optional<double> BoilerHotWater::nominalCapacity() const {
  return getImpl<detail::BoilerHotWater_Impl>()->nominalCapacity();
}

bool BoilerHotWater::isNominalCapacityAutosized() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isNominalCapacityAutosized();
}

double BoilerHotWater::nominalThermalEfficiency() const {
  return getImpl<detail::BoilerHotWater_Impl>()->nominalThermalEfficiency();
}

boost::optional<std::string> BoilerHotWater::efficiencyCurveTemperatureEvaluationVariable() const {
  return getImpl<detail::BoilerHotWater_Impl>()->efficiencyCurveTemperatureEvaluationVariable();
}

boost::optional<Curve> BoilerHotWater::normalizedBoilerEfficiencyCurve() const {
  return getImpl<detail::BoilerHotWater_Impl>()->normalizedBoilerEfficiencyCurve();
}

boost::optional<double> BoilerHotWater::designWaterOutletTemperature() const {
  return getImpl<detail::BoilerHotWater_Impl>()->designWaterOutletTemperature();
}

boost::optional<double> BoilerHotWater::designWaterFlowRate() const {
  return getImpl<detail::BoilerHotWater_Impl>()->designWaterFlowRate();
}

bool BoilerHotWater::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isDesignWaterFlowRateAutosized();
}

double BoilerHotWater::minimumPartLoadRatio() const {
  return getImpl<detail::BoilerHotWater_Impl>()->minimumPartLoadRatio();
}

bool BoilerHotWater::isMinimumPartLoadRatioDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isMinimumPartLoadRatioDefaulted();
}

double BoilerHotWater::maximumPartLoadRatio() const {
  return getImpl<detail::BoilerHotWater_Impl>()->maximumPartLoadRatio();
}

bool BoilerHotWater::isMaximumPartLoadRatioDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isMaximumPartLoadRatioDefaulted();
}

double BoilerHotWater::optimumPartLoadRatio() const {
  return getImpl<detail::BoilerHotWater_Impl>()->optimumPartLoadRatio();
}

bool BoilerHotWater::isOptimumPartLoadRatioDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isOptimumPartLoadRatioDefaulted();
}

double BoilerHotWater::waterOutletUpperTemperatureLimit() const {
  return getImpl<detail::BoilerHotWater_Impl>()->waterOutletUpperTemperatureLimit();
}

bool BoilerHotWater::isWaterOutletUpperTemperatureLimitDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isWaterOutletUpperTemperatureLimitDefaulted();
}

std::string BoilerHotWater::boilerFlowMode() const {
  return getImpl<detail::BoilerHotWater_Impl>()->boilerFlowMode();
}

bool BoilerHotWater::isBoilerFlowModeDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isBoilerFlowModeDefaulted();
}

boost::optional<double> BoilerHotWater::parasiticElectricLoad() const {
  return getImpl<detail::BoilerHotWater_Impl>()->parasiticElectricLoad();
}

double BoilerHotWater::sizingFactor() const {
  return getImpl<detail::BoilerHotWater_Impl>()->sizingFactor();
}

bool BoilerHotWater::isSizingFactorDefaulted() const {
  return getImpl<detail::BoilerHotWater_Impl>()->isSizingFactorDefaulted();
}

bool BoilerHotWater::setFuelType(std::string fuelType) {
  return getImpl<detail::BoilerHotWater_Impl>()->setFuelType(fuelType);
}

bool BoilerHotWater::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::BoilerHotWater_Impl>()->setNominalCapacity(nominalCapacity);
}

void BoilerHotWater::resetNominalCapacity() {
  getImpl<detail::BoilerHotWater_Impl>()->resetNominalCapacity();
}

void BoilerHotWater::autosizeNominalCapacity() {
  getImpl<detail::BoilerHotWater_Impl>()->autosizeNominalCapacity();
}

bool BoilerHotWater::setNominalThermalEfficiency(double nominalThermalEfficiency) {
  return getImpl<detail::BoilerHotWater_Impl>()->setNominalThermalEfficiency(nominalThermalEfficiency);
}

bool BoilerHotWater::setEfficiencyCurveTemperatureEvaluationVariable(std::string efficiencyCurveTemperatureEvaluationVariable) {
  return getImpl<detail::BoilerHotWater_Impl>()->setEfficiencyCurveTemperatureEvaluationVariable(efficiencyCurveTemperatureEvaluationVariable);
}

void BoilerHotWater::resetEfficiencyCurveTemperatureEvaluationVariable() {
  getImpl<detail::BoilerHotWater_Impl>()->resetEfficiencyCurveTemperatureEvaluationVariable();
}

bool BoilerHotWater::setNormalizedBoilerEfficiencyCurve(const Curve& normalizedBoilerEfficiencyCurve) {
  return getImpl<detail::BoilerHotWater_Impl>()->setNormalizedBoilerEfficiencyCurve(normalizedBoilerEfficiencyCurve);
}

void BoilerHotWater::resetNormalizedBoilerEfficiencyCurve() {
  getImpl<detail::BoilerHotWater_Impl>()->resetNormalizedBoilerEfficiencyCurve();
}

void BoilerHotWater::setDesignWaterOutletTemperature(double designWaterOutletTemperature) {
  getImpl<detail::BoilerHotWater_Impl>()->setDesignWaterOutletTemperature(designWaterOutletTemperature);
}

void BoilerHotWater::resetDesignWaterOutletTemperature() {
  getImpl<detail::BoilerHotWater_Impl>()->resetDesignWaterOutletTemperature();
}

bool BoilerHotWater::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::BoilerHotWater_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void BoilerHotWater::resetDesignWaterFlowRate() {
  getImpl<detail::BoilerHotWater_Impl>()->resetDesignWaterFlowRate();
}

void BoilerHotWater::autosizeDesignWaterFlowRate() {
  getImpl<detail::BoilerHotWater_Impl>()->autosizeDesignWaterFlowRate();
}

bool BoilerHotWater::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::BoilerHotWater_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

void BoilerHotWater::resetMinimumPartLoadRatio() {
  getImpl<detail::BoilerHotWater_Impl>()->resetMinimumPartLoadRatio();
}

bool BoilerHotWater::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::BoilerHotWater_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

void BoilerHotWater::resetMaximumPartLoadRatio() {
  getImpl<detail::BoilerHotWater_Impl>()->resetMaximumPartLoadRatio();
}

bool BoilerHotWater::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
  return getImpl<detail::BoilerHotWater_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
}

void BoilerHotWater::resetOptimumPartLoadRatio() {
  getImpl<detail::BoilerHotWater_Impl>()->resetOptimumPartLoadRatio();
}

void BoilerHotWater::setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit) {
  getImpl<detail::BoilerHotWater_Impl>()->setWaterOutletUpperTemperatureLimit(waterOutletUpperTemperatureLimit);
}

void BoilerHotWater::resetWaterOutletUpperTemperatureLimit() {
  getImpl<detail::BoilerHotWater_Impl>()->resetWaterOutletUpperTemperatureLimit();
}

bool BoilerHotWater::setBoilerFlowMode(std::string boilerFlowMode) {
  return getImpl<detail::BoilerHotWater_Impl>()->setBoilerFlowMode(boilerFlowMode);
}

void BoilerHotWater::resetBoilerFlowMode() {
  getImpl<detail::BoilerHotWater_Impl>()->resetBoilerFlowMode();
}

bool BoilerHotWater::setParasiticElectricLoad(double parasiticElectricLoad) {
  return getImpl<detail::BoilerHotWater_Impl>()->setParasiticElectricLoad(parasiticElectricLoad);
}

void BoilerHotWater::resetParasiticElectricLoad() {
  getImpl<detail::BoilerHotWater_Impl>()->resetParasiticElectricLoad();
}

bool BoilerHotWater::setSizingFactor(double sizingFactor) {
  return getImpl<detail::BoilerHotWater_Impl>()->setSizingFactor(sizingFactor);
}

void BoilerHotWater::resetSizingFactor() {
  getImpl<detail::BoilerHotWater_Impl>()->resetSizingFactor();
}

/// @cond
BoilerHotWater::BoilerHotWater(std::shared_ptr<detail::BoilerHotWater_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

