/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BoilerHotWater.hpp"
#include "BoilerHotWater_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Boiler_HotWater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/DeprecatedHelpers.hpp"  // For deprecation

namespace openstudio {
namespace model {

  namespace detail {

    BoilerHotWater_Impl::BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == BoilerHotWater::iddObjectType());
    }

    BoilerHotWater_Impl::BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == BoilerHotWater::iddObjectType());
    }

    BoilerHotWater_Impl::BoilerHotWater_Impl(const BoilerHotWater_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& BoilerHotWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{

        // Common variables
        "Boiler Heating Rate", "Boiler Heating Energy", "Boiler Inlet Temperature", "Boiler Outlet Temperature", "Boiler Mass Flow Rate",
        "Boiler Parasitic Electric Power", "Boiler Ancillary Electricity Energy", "Boiler Part Load Ratio",

        // Fuel type specific
        // TODO: DLM: the return type of this method needs to change to std::vector<std::string> in ModelObject
        // until then, make this include all possible outputVariableNames for class regardless of fuelType
        // std::string fuelType = this->fuelType(,
        // if (fuelType == "Electricity") {
        "Boiler Electricity Rate", "Boiler Electricity Energy",
        // } else if (fuelType == "NaturalGas") {
        "Boiler NaturalGas Rate", "Boiler NaturalGas Energy",
        // } else if (fuelType == "Propane") {
        "Boiler Propane Rate", "Boiler Propane Energy",
        // } else if (fuelType == "FuelOilNo1") {
        "Boiler FuelOilNo1 Rate", "Boiler FuelOilNo1 Energy",
        // } else if (fuelType == "FuelOilNo2") {
        "Boiler FuelOilNo2 Rate", "Boiler FuelOilNo2 Energy",
        // } else if (fuelType == "Coal") {
        "Boiler Coal Rate", "Boiler Coal Energy",
        // } else if (fuelType == "Diesel") {
        "Boiler Diesel Rate", "Boiler Diesel Energy",
        // } else if (fuelType == "Gasoline") {
        "Boiler Gasoline Rate", "Boiler Gasoline Energy",
        // } else if (fuelType == "OtherFuel1") {
        "Boiler OtherFuel1 Rate", "Boiler OtherFuel1 Energy",
        // } else if (fuelType == "OtherFuel2") {
        "Boiler OtherFuel2 Rate", "Boiler OtherFuel2 Energy"
        // }
      };
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

    unsigned BoilerHotWater_Impl::inletPort() const {
      return OS_Boiler_HotWaterFields::BoilerWaterInletNodeName;
    }

    unsigned BoilerHotWater_Impl::outletPort() const {
      return OS_Boiler_HotWaterFields::BoilerWaterOutletNodeName;
    }

    std::string BoilerHotWater_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> BoilerHotWater_Impl::nominalCapacity() const {
      return getDouble(OS_Boiler_HotWaterFields::NominalCapacity, true);
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
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::NominalThermalEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<std::string> BoilerHotWater_Impl::efficiencyCurveTemperatureEvaluationVariable() const {
      return getString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, true);
    }

    boost::optional<Curve> BoilerHotWater_Impl::normalizedBoilerEfficiencyCurve() const {
      return getObject<BoilerHotWater>().getModelObjectTarget<Curve>(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName);
    }

    boost::optional<double> BoilerHotWater_Impl::designWaterFlowRate() const {
      return getDouble(OS_Boiler_HotWaterFields::DesignWaterFlowRate, true);
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
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isMinimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::MinimumPartLoadRatio);
    }

    double BoilerHotWater_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isMaximumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::MaximumPartLoadRatio);
    }

    double BoilerHotWater_Impl::optimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::OptimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isOptimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::OptimumPartLoadRatio);
    }

    double BoilerHotWater_Impl::waterOutletUpperTemperatureLimit() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isWaterOutletUpperTemperatureLimitDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit);
    }

    std::string BoilerHotWater_Impl::boilerFlowMode() const {
      boost::optional<std::string> value = getString(OS_Boiler_HotWaterFields::BoilerFlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isBoilerFlowModeDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::BoilerFlowMode);
    }

    double BoilerHotWater_Impl::onCycleParasiticElectricLoad() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::OnCycleParasiticElectricLoad, true);
      OS_ASSERT(value);
      return value.get();
    }

    double BoilerHotWater_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::isSizingFactorDefaulted() const {
      return isEmpty(OS_Boiler_HotWaterFields::SizingFactor);
    }

    double BoilerHotWater_Impl::offCycleParasiticFuelLoad() const {
      boost::optional<double> value = getDouble(OS_Boiler_HotWaterFields::OffCycleParasiticFuelLoad, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::setFuelType(const std::string& fuelType) {
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

    bool BoilerHotWater_Impl::setEfficiencyCurveTemperatureEvaluationVariable(
      boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable) {
      bool result = false;
      if (efficiencyCurveTemperatureEvaluationVariable) {
        result =
          setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, efficiencyCurveTemperatureEvaluationVariable.get());
      } else {
        result = setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, "");
      }
      return result;
    }

    void BoilerHotWater_Impl::resetEfficiencyCurveTemperatureEvaluationVariable() {
      bool result = setString(OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, "");
      OS_ASSERT(result);
    }

    bool BoilerHotWater_Impl::setNormalizedBoilerEfficiencyCurve(const boost::optional<Curve>& normalizedBoilerEfficiencyCurve) {
      if (normalizedBoilerEfficiencyCurve) {
        return setPointer(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName, normalizedBoilerEfficiencyCurve->handle());
      } else {
        return setString(OS_Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName, "");
      }
      return false;
    }

    void BoilerHotWater_Impl::resetNormalizedBoilerEfficiencyCurve() {
      bool ok = setNormalizedBoilerEfficiencyCurve(boost::none);
      OS_ASSERT(ok);
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

    bool BoilerHotWater_Impl::setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit) {
      bool result = setDouble(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, waterOutletUpperTemperatureLimit);
      OS_ASSERT(result);
      return result;
    }

    void BoilerHotWater_Impl::resetWaterOutletUpperTemperatureLimit() {
      bool result = setString(OS_Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, "");
      OS_ASSERT(result);
    }

    bool BoilerHotWater_Impl::setBoilerFlowMode(const std::string& boilerFlowMode) {
      bool result = false;
      if (istringEqual(boilerFlowMode, "VariableFlow")) {
        // Support legacy key
        result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode, "LeavingSetpointModulated");
      } else {
        result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode, boilerFlowMode);
      }
      return result;
    }

    void BoilerHotWater_Impl::resetBoilerFlowMode() {
      bool result = setString(OS_Boiler_HotWaterFields::BoilerFlowMode, "");
      OS_ASSERT(result);
    }

    bool BoilerHotWater_Impl::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
      const bool result = setDouble(OS_Boiler_HotWaterFields::OnCycleParasiticElectricLoad, onCycleParasiticElectricLoad);
      return result;
    }

    bool BoilerHotWater_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_Boiler_HotWaterFields::SizingFactor, sizingFactor);
      return result;
    }

    void BoilerHotWater_Impl::resetSizingFactor() {
      bool result = setString(OS_Boiler_HotWaterFields::SizingFactor, "");
      OS_ASSERT(result);
    }

    bool BoilerHotWater_Impl::setOffCycleParasiticFuelLoad(double offCycleParasiticFuelLoad) {
      bool result = setDouble(OS_Boiler_HotWaterFields::OffCycleParasiticFuelLoad, offCycleParasiticFuelLoad);
      return result;
    }

    bool BoilerHotWater_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          if (StraightComponent_Impl::addToNode(node)) {
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
        } else {
          return false;
        }
      } else {
        resetNormalizedBoilerEfficiencyCurve();
      }
      return true;
    }

    ModelObject BoilerHotWater_Impl::clone(Model model) const {
      return StraightComponent_Impl::clone(model);
    }

    boost::optional<double> BoilerHotWater_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> BoilerHotWater_Impl::autosizedDesignWaterFlowRate() const {
      return getAutosizedValue("Design Size Design Water Flow Rate", "m3/s");
    }

    void BoilerHotWater_Impl::autosize() {
      autosizeNominalCapacity();
      autosizeDesignWaterFlowRate();
    }

    void BoilerHotWater_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }

      val = autosizedDesignWaterFlowRate();
      if (val) {
        setDesignWaterFlowRate(val.get());
      }
    }

    std::string BoilerHotWater_Impl::endUseSubcategory() const {
      auto value = getString(OS_Boiler_HotWaterFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerHotWater_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_Boiler_HotWaterFields::EndUseSubcategory, endUseSubcategory);
    }

    ComponentType BoilerHotWater_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> BoilerHotWater_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> BoilerHotWater_Impl::heatingFuelTypes() const {
      return {FuelType(fuelType())};
    }

    std::vector<AppGFuelType> BoilerHotWater_Impl::appGHeatingFuelTypes() const {
      return {convertFuelTypeToAppG(FuelType(fuelType()))};
    }

  }  // namespace detail

  BoilerHotWater::BoilerHotWater(const Model& model) : StraightComponent(BoilerHotWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::BoilerHotWater_Impl>());

    setWaterOutletUpperTemperatureLimit(99.0);
    setBoilerFlowMode("ConstantFlow");
    setSizingFactor(1.0);
    setEndUseSubcategory("General");
    setOnCycleParasiticElectricLoad(0.0);
    setOffCycleParasiticFuelLoad(0.0);
  }

  IddObjectType BoilerHotWater::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Boiler_HotWater);
    return result;
  }

  std::vector<std::string> BoilerHotWater::validFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Boiler_HotWaterFields::FuelType);
  }

  std::vector<std::string> BoilerHotWater::validEfficiencyCurveTemperatureEvaluationVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable);
  }

  std::vector<std::string> BoilerHotWater::validBoilerFlowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Boiler_HotWaterFields::BoilerFlowMode);
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

  // DEPRECATED
  boost::optional<double> BoilerHotWater::parasiticElectricLoad() const {
    DEPRECATED_AT_MSG(3, 7, 0, "Use onCycleParasiticElectricLoad instead.");
    boost::optional<double> parasiticElectricLoad = getImpl<detail::BoilerHotWater_Impl>()->onCycleParasiticElectricLoad();
    return parasiticElectricLoad;
  }

  double BoilerHotWater::onCycleParasiticElectricLoad() const {
    return getImpl<detail::BoilerHotWater_Impl>()->onCycleParasiticElectricLoad();
  }

  double BoilerHotWater::sizingFactor() const {
    return getImpl<detail::BoilerHotWater_Impl>()->sizingFactor();
  }

  bool BoilerHotWater::isSizingFactorDefaulted() const {
    return getImpl<detail::BoilerHotWater_Impl>()->isSizingFactorDefaulted();
  }

  double BoilerHotWater::offCycleParasiticFuelLoad() const {
    return getImpl<detail::BoilerHotWater_Impl>()->offCycleParasiticFuelLoad();
  }

  bool BoilerHotWater::setFuelType(const std::string& fuelType) {
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

  bool BoilerHotWater::setEfficiencyCurveTemperatureEvaluationVariable(const std::string& efficiencyCurveTemperatureEvaluationVariable) {
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

  bool BoilerHotWater::setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit) {
    return getImpl<detail::BoilerHotWater_Impl>()->setWaterOutletUpperTemperatureLimit(waterOutletUpperTemperatureLimit);
  }

  void BoilerHotWater::resetWaterOutletUpperTemperatureLimit() {
    getImpl<detail::BoilerHotWater_Impl>()->resetWaterOutletUpperTemperatureLimit();
  }

  bool BoilerHotWater::setBoilerFlowMode(const std::string& boilerFlowMode) {
    return getImpl<detail::BoilerHotWater_Impl>()->setBoilerFlowMode(boilerFlowMode);
  }

  void BoilerHotWater::resetBoilerFlowMode() {
    getImpl<detail::BoilerHotWater_Impl>()->resetBoilerFlowMode();
  }

  // DEPRECATED
  bool BoilerHotWater::setParasiticElectricLoad(double parasiticElectricLoad) {
    DEPRECATED_AT_MSG(3, 7, 0, "Use setOnCycleParasiticElectricLoad instead.");
    return getImpl<detail::BoilerHotWater_Impl>()->setOnCycleParasiticElectricLoad(parasiticElectricLoad);
  }

  bool BoilerHotWater::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
    return getImpl<detail::BoilerHotWater_Impl>()->setOnCycleParasiticElectricLoad(onCycleParasiticElectricLoad);
  }

  // DEPRECATED
  void BoilerHotWater::resetParasiticElectricLoad() {
    DEPRECATED_AT_MSG(3, 7, 0, "OnCycleParasiticElectricLoad is now required.");
  }

  bool BoilerHotWater::setSizingFactor(double sizingFactor) {
    return getImpl<detail::BoilerHotWater_Impl>()->setSizingFactor(sizingFactor);
  }

  void BoilerHotWater::resetSizingFactor() {
    getImpl<detail::BoilerHotWater_Impl>()->resetSizingFactor();
  }

  std::string BoilerHotWater::endUseSubcategory() const {
    return getImpl<detail::BoilerHotWater_Impl>()->endUseSubcategory();
  }

  bool BoilerHotWater::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::BoilerHotWater_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  bool BoilerHotWater::setOffCycleParasiticFuelLoad(double offCycleParasiticFuelLoad) {
    return getImpl<detail::BoilerHotWater_Impl>()->setOffCycleParasiticFuelLoad(offCycleParasiticFuelLoad);
  }

  /// @cond
  BoilerHotWater::BoilerHotWater(std::shared_ptr<detail::BoilerHotWater_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> BoilerHotWater::autosizedNominalCapacity() const {
    return getImpl<detail::BoilerHotWater_Impl>()->autosizedNominalCapacity();
  }

  boost::optional<double> BoilerHotWater::autosizedDesignWaterFlowRate() const {
    return getImpl<detail::BoilerHotWater_Impl>()->autosizedDesignWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
