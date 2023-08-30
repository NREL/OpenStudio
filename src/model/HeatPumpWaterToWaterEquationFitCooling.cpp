/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "Model.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include "HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>

#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
    }

    HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
    }

    HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpWaterToWaterEquationFitCooling_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Water to Water Heat Pump Electricity Energy",
                                                   "Water to Water Heat Pump Load Side Heat Transfer Energy",
                                                   "Water to Water Heat Pump Source Side Heat Transfer Energy",
                                                   "Water to Water Heat Pump Electricity Rate",
                                                   "Water to Water Heat Pump Load Side Heat Transfer Rate",
                                                   "Water to Water Heat Pump Source Side Heat Transfer Rate",
                                                   "Water to Water Heat Pump Load Side Outlet Temperature",
                                                   "Water to Water Heat Pump Load Side Inlet Temperature",
                                                   "Water to Water Heat Pump Source Side Outlet Temperature",
                                                   "Water to Water Heat Pump Source Side Inlet Temperature",
                                                   "Water to Water Heat Pump Load Side Mass Flow Rate",
                                                   "Water to Water Heat Pump Source Side Mass Flow Rate"};
      return result;
    }

    IddObjectType HeatPumpWaterToWaterEquationFitCooling_Impl::iddObjectType() const {
      return HeatPumpWaterToWaterEquationFitCooling::iddObjectType();
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::referenceLoadSideFlowRate() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, true);
    }

    double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedLoadSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::isReferenceLoadSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::referenceSourceSideFlowRate() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, true);
    }

    double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedSourceSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::isReferenceSourceSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::ratedCoolingCapacity() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, true);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::isRatedCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::ratedCoolingPowerConsumption() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, true);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::isRatedCoolingPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpWaterToWaterEquationFitCooling_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpWaterToWaterEquationFitCooling_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HeatPumpWaterToWaterEquationFitHeating> HeatPumpWaterToWaterEquationFitCooling_Impl::companionHeatingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpWaterToWaterEquationFitHeating>(
        OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, referenceLoadSideFlowRate);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::autosizeReferenceLoadSideFlowRate() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, referenceSourceSideFlowRate);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::autosizeReferenceSourceSideFlowRate() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedCoolingCapacity(double ratedCoolingCapacity) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, ratedCoolingCapacity);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::autosizeRatedCoolingCapacity() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, ratedCoolingPowerConsumption);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::autosizeRatedCoolingPowerConsumption() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      bool result =
        setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SizingFactor, sizingFactor);
      OS_ASSERT(result);
      return result;
    }

    CurveQuadLinear HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve) {
      bool result = setPointer(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCurveName, coolingCapacityCurve.handle());
      return result;
    }

    CurveQuadLinear HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve) {
      bool result =
        setPointer(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCurveName, coolingCompressorPowerCurve.handle());
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitCooling_Impl::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP) {
      return this->setPointer(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName, companionHP.handle());
    }

    unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyInletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyOutletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandInletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandOutletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::autosizedReferenceLoadSideFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::autosizedReferenceSourceSideFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::autosizedRatedCoolingCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::autosizedRatedCoolingPowerConsumption() const {
      return getAutosizedValue("Design Size Cooling Power Consumption", "W");
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::autosize() {
      autosizeReferenceLoadSideFlowRate();
      autosizeReferenceSourceSideFlowRate();
      autosizeRatedCoolingCapacity();
      autosizeRatedCoolingPowerConsumption();
    }

    void HeatPumpWaterToWaterEquationFitCooling_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedReferenceLoadSideFlowRate();
      if (val) {
        setReferenceLoadSideFlowRate(val.get());
      }

      val = autosizedReferenceSourceSideFlowRate();
      if (val) {
        setReferenceSourceSideFlowRate(val.get());
      }

      val = autosizedRatedCoolingCapacity();
      if (val) {
        setRatedCoolingCapacity(val.get());
      }

      val = autosizedRatedCoolingPowerConsumption();
      if (val) {
        setRatedCoolingPowerConsumption(val.get());
      }
    }

    ComponentType HeatPumpWaterToWaterEquationFitCooling_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> HeatPumpWaterToWaterEquationFitCooling_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      result.insert(FuelType::Electricity);
      if (auto p_ = secondaryPlantLoop()) {
        for (auto& ft : p_->coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> HeatPumpWaterToWaterEquationFitCooling_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> HeatPumpWaterToWaterEquationFitCooling_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(const Model& model, const CurveQuadLinear& coolingCapacityCurve,
                                                                                 const CurveQuadLinear& coolingCompressorPowerCurve)
    : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeRatedCoolingCapacity();
    autosizeRatedCoolingPowerConsumption();

    bool ok = setCoolingCapacityCurve(coolingCapacityCurve);
    OS_ASSERT(ok);

    ok = setCoolingCompressorPowerCurve(coolingCompressorPowerCurve);
    OS_ASSERT(ok);

    setReferenceCoefficientofPerformance(8.0);
    setSizingFactor(1.0);
    // Trick to make IG not do weird things
    setString(22, "");
  }

  HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(const Model& model)
    : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeRatedCoolingCapacity();
    autosizeRatedCoolingPowerConsumption();

    CurveQuadLinear coolingCapacityCurve(model);
    coolingCapacityCurve.setCoefficient1Constant(-1.52030596);
    coolingCapacityCurve.setCoefficient2w(3.46625667);
    coolingCapacityCurve.setCoefficient3x(-1.32267797);
    coolingCapacityCurve.setCoefficient4y(0.09395678);
    coolingCapacityCurve.setCoefficient5z(0.038975504);
    bool ok = setCoolingCapacityCurve(coolingCapacityCurve);
    OS_ASSERT(ok);

    CurveQuadLinear coolingCompressorPowerCurve(model);
    coolingCompressorPowerCurve.setCoefficient1Constant(-8.59564386);
    coolingCompressorPowerCurve.setCoefficient2w(0.96265085);
    coolingCompressorPowerCurve.setCoefficient3x(8.69489229);
    coolingCompressorPowerCurve.setCoefficient4y(0.02501669);
    coolingCompressorPowerCurve.setCoefficient5z(-0.20132665);
    ok = setCoolingCompressorPowerCurve(coolingCompressorPowerCurve);
    OS_ASSERT(ok);

    setReferenceCoefficientofPerformance(8.0);
    setSizingFactor(1.0);
    // Trick to make IG not do weird things
    setString(22, "");
  }

  IddObjectType HeatPumpWaterToWaterEquationFitCooling::iddObjectType() {
    return {IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling};
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::referenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->referenceLoadSideFlowRate();
  }

  double HeatPumpWaterToWaterEquationFitCooling::ratedLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedLoadSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::isReferenceLoadSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->isReferenceLoadSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::referenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->referenceSourceSideFlowRate();
  }

  double HeatPumpWaterToWaterEquationFitCooling::ratedSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedSourceSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::isReferenceSourceSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->isReferenceSourceSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::ratedCoolingCapacity() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedCoolingCapacity();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::isRatedCoolingCapacityAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->isRatedCoolingCapacityAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::ratedCoolingPowerConsumption() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedCoolingPowerConsumption();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::isRatedCoolingPowerConsumptionAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->isRatedCoolingPowerConsumptionAutosized();
  }

  double HeatPumpWaterToWaterEquationFitCooling::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpWaterToWaterEquationFitCooling::sizingFactor() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->sizingFactor();
  }

  boost::optional<HeatPumpWaterToWaterEquationFitHeating> HeatPumpWaterToWaterEquationFitCooling::companionHeatingHeatPump() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->companionHeatingHeatPump();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceLoadSideFlowRate(referenceLoadSideFlowRate);
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceLoadSideFlowRate(ratedLoadSideFlowRate);
  }

  void HeatPumpWaterToWaterEquationFitCooling::autosizeReferenceLoadSideFlowRate() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizeReferenceLoadSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceSourceSideFlowRate(referenceSourceSideFlowRate);
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceSourceSideFlowRate(ratedSourceSideFlowRate);
  }

  void HeatPumpWaterToWaterEquationFitCooling::autosizeReferenceSourceSideFlowRate() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizeReferenceSourceSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setRatedCoolingCapacity(double ratedCoolingCapacity) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedCoolingCapacity(ratedCoolingCapacity);
  }

  void HeatPumpWaterToWaterEquationFitCooling::autosizeRatedCoolingCapacity() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizeRatedCoolingCapacity();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedCoolingPowerConsumption(ratedCoolingPowerConsumption);
  }

  void HeatPumpWaterToWaterEquationFitCooling::autosizeRatedCoolingPowerConsumption() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizeRatedCoolingPowerConsumption();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCompanionHeatingHeatPump(companionHP);
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::autosizedReferenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizedReferenceLoadSideFlowRate();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::autosizedReferenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizedReferenceSourceSideFlowRate();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::autosizedRatedCoolingCapacity() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizedRatedCoolingCapacity();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling::autosizedRatedCoolingPowerConsumption() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->autosizedRatedCoolingPowerConsumption();
  }

  CurveQuadLinear HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCurve(coolingCapacityCurve);
  }

  CurveQuadLinear HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCurve(coolingCompressorPowerCurve);
  }

  /// @cond
  HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(
    std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}

  // DEPRECATED
  double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient1() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().coefficient1Constant() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().setCoefficient1Constant(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient1Constant(coolingCapacityCoefficient1);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient2() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().coefficient2w() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().setCoefficient2w(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient2w(coolingCapacityCoefficient2);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient3() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().coefficient3x() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().setCoefficient3x(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient3x(coolingCapacityCoefficient3);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient4() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().coefficient4y() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().setCoefficient4y(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient4y(coolingCapacityCoefficient4);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient5() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().coefficient5z() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCurve().setCoefficient5z(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient5z(coolingCapacityCoefficient5);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient1() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().coefficient1Constant() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().setCoefficient1Constant(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient1Constant(coolingCompressorPowerCoefficient1);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient2() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().coefficient2w() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().setCoefficient2w(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient2w(coolingCompressorPowerCoefficient2);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient3() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().coefficient3x() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().setCoefficient3x(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient3x(coolingCompressorPowerCoefficient3);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient4() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().coefficient4y() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().setCoefficient4y(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient4y(coolingCompressorPowerCoefficient4);
  }

  double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient5() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().coefficient5z() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCurve().setCoefficient5z(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient5z(coolingCompressorPowerCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
