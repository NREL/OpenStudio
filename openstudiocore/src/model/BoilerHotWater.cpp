/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/BoilerHotWater.hpp>
#include <model/BoilerHotWater_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Boiler_HotWater_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  BoilerHotWater_Impl::BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == BoilerHotWater::iddObjectType());
  }

  BoilerHotWater_Impl::BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == BoilerHotWater::iddObjectType());
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
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isMinimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::MinimumPartLoadRatio);
  }

  double BoilerHotWater_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::MaximumPartLoadRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isMaximumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::MaximumPartLoadRatio);
  }

  double BoilerHotWater_Impl::optimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::OptimumPartLoadRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isOptimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::OptimumPartLoadRatio);
  }

  double BoilerHotWater_Impl::waterOutletUpperTemperatureLimit() const {
    boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isWaterOutletUpperTemperatureLimitDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit);
  }

  std::string BoilerHotWater_Impl::boilerFlowMode() const {
    boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::BoilerFlowMode,true);
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
    return value.get();
  }

  bool BoilerHotWater_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_Boiler_HotWaterFields::SizingFactor);
  }

  bool BoilerHotWater_Impl::setFuelType(std::string fuelType) {
    bool result = false;
    result = setString(OS_Boiler_HotWaterFields::FuelType, fuelType);
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
    BOOST_ASSERT(result);
  }

  void BoilerHotWater_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Boiler_HotWaterFields::NominalCapacity, "Autosize");
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setNominalThermalEfficiency(double nominalThermalEfficiency) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::NominalThermalEfficiency, nominalThermalEfficiency);
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
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(ok);
  }

  void BoilerHotWater_Impl::setDesignWaterOutletTemperature(boost::optional<double> designWaterOutletTemperature) {
    bool result = false;
    if (designWaterOutletTemperature) {
      result = setDouble(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, designWaterOutletTemperature.get());
    } else {
      result = setString(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, "");
    }
    BOOST_ASSERT(result);
  }

  void BoilerHotWater_Impl::resetDesignWaterOutletTemperature() {
    bool result = setString(OS_Boiler_HotWaterFields::DesignWaterOutletTemperature, "");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void BoilerHotWater_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_Boiler_HotWaterFields::DesignWaterFlowRate, "Autosize");
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetMinimumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::MinimumPartLoadRatio, "");
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetMaximumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::MaximumPartLoadRatio, "");
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    return result;
  }

  void BoilerHotWater_Impl::resetOptimumPartLoadRatio() {
    bool result = setString(OS_Boiler_HotWaterFields::OptimumPartLoadRatio, "");
    BOOST_ASSERT(result);
  }

  void BoilerHotWater_Impl::setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, waterOutletUpperTemperatureLimit);
    BOOST_ASSERT(result);
  }

  void BoilerHotWater_Impl::resetWaterOutletUpperTemperatureLimit() {
    bool result = setString(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, "");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::setSizingFactor(double sizingFactor) {
    bool result = false;
    result = setDouble(OS_Boiler_HotWaterFields::SizingFactor, sizingFactor);
    return result;
  }

  void BoilerHotWater_Impl::resetSizingFactor() {
    bool result = setString(OS_Boiler_HotWaterFields::SizingFactor, "");
    BOOST_ASSERT(result);
  }

  bool BoilerHotWater_Impl::addToNode(Node & node)
  {
    if( node.airLoopHVAC() )
    {
      return false;
    }
    else
    {
      if( boost::optional<PlantLoop> plantLoop = node.plantLoop() )
      {
        if( plantLoop->demandComponent(node.handle()) )
        {
          return false;
        }
        else
        {
          bool result = StraightComponent_Impl::addToNode(node);

          if( result )
          {
            plantLoop->setFluidType("Water");  
          }

          return result;
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
    BoilerHotWater newBoiler = ModelObject_Impl::clone(model).cast<BoilerHotWater>();

    if( boost::optional<Curve> c = normalizedBoilerEfficiencyCurve() )
    {
      newBoiler.setNormalizedBoilerEfficiencyCurve(c->clone(model).cast<Curve>());
    }

    return newBoiler;
  }

} // detail

BoilerHotWater::BoilerHotWater(const Model& model)
  : StraightComponent(BoilerHotWater::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::BoilerHotWater_Impl>());

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
BoilerHotWater::BoilerHotWater(boost::shared_ptr<detail::BoilerHotWater_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

