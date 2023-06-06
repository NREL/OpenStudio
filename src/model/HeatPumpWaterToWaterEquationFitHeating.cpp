/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "Model.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include "HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>

#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
    }

    HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
    }

    HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const HeatPumpWaterToWaterEquationFitHeating_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpWaterToWaterEquationFitHeating_Impl::outputVariableNames() const {
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

    IddObjectType HeatPumpWaterToWaterEquationFitHeating_Impl::iddObjectType() const {
      return HeatPumpWaterToWaterEquationFitHeating::iddObjectType();
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::referenceLoadSideFlowRate() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, true);
    }

    double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedLoadSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::isReferenceLoadSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::referenceSourceSideFlowRate() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, true);
    }

    double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedSourceSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::isReferenceSourceSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::ratedHeatingCapacity() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, true);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::isRatedHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::ratedHeatingPowerConsumption() const {
      return getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, true);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::isRatedHeatingPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpWaterToWaterEquationFitHeating_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpWaterToWaterEquationFitHeating_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HeatPumpWaterToWaterEquationFitCooling> HeatPumpWaterToWaterEquationFitHeating_Impl::companionCoolingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpWaterToWaterEquationFitCooling>(
        OS_HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, referenceLoadSideFlowRate);
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, ratedLoadSideFlowRate);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::autosizeReferenceLoadSideFlowRate() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, referenceSourceSideFlowRate);
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, ratedSourceSideFlowRate);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::autosizeReferenceSourceSideFlowRate() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, ratedHeatingCapacity);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::autosizeRatedHeatingCapacity() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, ratedHeatingPowerConsumption);
      return result;
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::autosizeRatedHeatingPowerConsumption() {
      bool result = setString(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      bool result =
        setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SizingFactor, sizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP) {
      return this->setPointer(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName, companionHP.handle());
    }

    CurveQuadLinear HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
      bool result = setPointer(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCurveName, heatingCapacityCurve.handle());
      return result;
    }

    CurveQuadLinear HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve) {
      bool result =
        setPointer(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCurveName, heatingCompressorPowerCurve.handle());
      return result;
    }

    unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyInletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyOutletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandInletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandOutletPort() const {
      return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName;
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::autosizedReferenceLoadSideFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::autosizedReferenceSourceSideFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::autosizedRatedHeatingCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::autosizedRatedHeatingPowerConsumption() const {
      return getAutosizedValue("Design Size Heating Power Consumption", "W");
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::autosize() {
      autosizeReferenceLoadSideFlowRate();
      autosizeReferenceSourceSideFlowRate();
      autosizeRatedHeatingCapacity();
      autosizeRatedHeatingPowerConsumption();
    }

    void HeatPumpWaterToWaterEquationFitHeating_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedReferenceLoadSideFlowRate();
      if (val) {
        setReferenceLoadSideFlowRate(val.get());
      }

      val = autosizedReferenceSourceSideFlowRate();
      if (val) {
        setReferenceSourceSideFlowRate(val.get());
      }

      val = autosizedRatedHeatingCapacity();
      if (val) {
        setRatedHeatingCapacity(val.get());
      }

      val = autosizedRatedHeatingPowerConsumption();
      if (val) {
        setRatedHeatingPowerConsumption(val.get());
      }
    }

    ComponentType HeatPumpWaterToWaterEquationFitHeating_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> HeatPumpWaterToWaterEquationFitHeating_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> HeatPumpWaterToWaterEquationFitHeating_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      result.insert(FuelType::Electricity);
      if (auto p_ = secondaryPlantLoop()) {
        for (auto& ft : p_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> HeatPumpWaterToWaterEquationFitHeating_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      result.insert(AppGFuelType::HeatPump);  // TODO: is that right?
      if (auto p_ = secondaryPlantLoop()) {
        for (auto& ft : p_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(const Model& model, const CurveQuadLinear& heatingCapacityCurve,
                                                                                 const CurveQuadLinear& heatingCompressorPowerCurve)
    : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeRatedHeatingCapacity();
    autosizeRatedHeatingPowerConsumption();

    bool ok = setHeatingCapacityCurve(heatingCapacityCurve);
    OS_ASSERT(ok);

    ok = setHeatingCompressorPowerCurve(heatingCompressorPowerCurve);
    OS_ASSERT(ok);

    setReferenceCoefficientofPerformance(7.5);
    setSizingFactor(1.0);
    // Trick to make IG not do weird things
    setString(22, "");
  }

  HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(const Model& model)
    : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeRatedHeatingCapacity();
    autosizeRatedHeatingPowerConsumption();

    CurveQuadLinear heatingCapacityCurve(model);
    heatingCapacityCurve.setCoefficient1Constant(-3.33491153);
    heatingCapacityCurve.setCoefficient2w(-0.51451946);
    heatingCapacityCurve.setCoefficient3x(4.51592706);
    heatingCapacityCurve.setCoefficient4y(0.01797107);
    heatingCapacityCurve.setCoefficient5z(0.155797661);
    bool ok = setHeatingCapacityCurve(heatingCapacityCurve);
    OS_ASSERT(ok);

    CurveQuadLinear heatingCompressorPowerCurve(model);
    heatingCompressorPowerCurve.setCoefficient1Constant(-8.93121751);
    heatingCompressorPowerCurve.setCoefficient2w(8.57035762);
    heatingCompressorPowerCurve.setCoefficient3x(1.29660976);
    heatingCompressorPowerCurve.setCoefficient4y(-0.21629222);
    heatingCompressorPowerCurve.setCoefficient5z(0.033862378);
    ok = setHeatingCompressorPowerCurve(heatingCapacityCurve);
    OS_ASSERT(ok);

    setReferenceCoefficientofPerformance(7.5);
    setSizingFactor(1.0);
    // Trick to make IG not do weird things
    setString(22, "");
  }

  IddObjectType HeatPumpWaterToWaterEquationFitHeating::iddObjectType() {
    return {IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating};
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::referenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->referenceLoadSideFlowRate();
  }

  double HeatPumpWaterToWaterEquationFitHeating::ratedLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedLoadSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::isReferenceLoadSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->isReferenceLoadSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::referenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->referenceSourceSideFlowRate();
  }

  double HeatPumpWaterToWaterEquationFitHeating::ratedSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedSourceSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::isReferenceSourceSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->isReferenceSourceSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::ratedHeatingCapacity() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedHeatingCapacity();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::isRatedHeatingCapacityAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->isRatedHeatingCapacityAutosized();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::ratedHeatingPowerConsumption() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedHeatingPowerConsumption();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::isRatedHeatingPowerConsumptionAutosized() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->isRatedHeatingPowerConsumptionAutosized();
  }

  double HeatPumpWaterToWaterEquationFitHeating::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpWaterToWaterEquationFitHeating::sizingFactor() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->sizingFactor();
  }

  boost::optional<HeatPumpWaterToWaterEquationFitCooling> HeatPumpWaterToWaterEquationFitHeating::companionCoolingHeatPump() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->companionCoolingHeatPump();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedLoadSideFlowRate(ratedLoadSideFlowRate);
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setReferenceLoadSideFlowRate(referenceLoadSideFlowRate);
  }

  void HeatPumpWaterToWaterEquationFitHeating::autosizeReferenceLoadSideFlowRate() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizeReferenceLoadSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedSourceSideFlowRate(ratedSourceSideFlowRate);
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setReferenceSourceSideFlowRate(referenceSourceSideFlowRate);
  }

  void HeatPumpWaterToWaterEquationFitHeating::autosizeReferenceSourceSideFlowRate() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizeReferenceSourceSideFlowRate();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
  }

  void HeatPumpWaterToWaterEquationFitHeating::autosizeRatedHeatingCapacity() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizeRatedHeatingCapacity();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedHeatingPowerConsumption(ratedHeatingPowerConsumption);
  }

  void HeatPumpWaterToWaterEquationFitHeating::autosizeRatedHeatingPowerConsumption() {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizeRatedHeatingPowerConsumption();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setCompanionCoolingHeatPump(companionHP);
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::autosizedReferenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizedReferenceLoadSideFlowRate();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::autosizedReferenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizedReferenceSourceSideFlowRate();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::autosizedRatedHeatingCapacity() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizedRatedHeatingCapacity();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating::autosizedRatedHeatingPowerConsumption() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->autosizedRatedHeatingPowerConsumption();
  }

  CurveQuadLinear HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCurve(heatingCapacityCurve);
  }

  CurveQuadLinear HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve() const {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve) {
    return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCurve(heatingCompressorPowerCurve);
  }

  /// @cond
  HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(
    std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}

  // DEPRECATED
  double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient1() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().coefficient1Constant() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().setCoefficient1Constant(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient1Constant(heatingCapacityCoefficient1);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient2() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().coefficient2w() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().setCoefficient2w(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient2w(heatingCapacityCoefficient2);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient3() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().coefficient3x() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().setCoefficient3x(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient3x(heatingCapacityCoefficient3);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient4() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().coefficient4y() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().setCoefficient4y(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient4y(heatingCapacityCoefficient4);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient5() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().coefficient5z() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCurve().setCoefficient5z(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient5z(heatingCapacityCoefficient5);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient1() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().coefficient1Constant() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().setCoefficient1Constant(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient1Constant(heatingCompressorPowerCoefficient1);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient2() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().coefficient2w() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().setCoefficient2w(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient2w(heatingCompressorPowerCoefficient2);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient3() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().coefficient3x() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().setCoefficient3x(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient3x(heatingCompressorPowerCoefficient3);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient4() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().coefficient4y() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().setCoefficient4y(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient4y(heatingCompressorPowerCoefficient4);
  }

  double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient5() const {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().coefficient5z() instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
    DEPRECATED_AT_MSG(3, 2, 0, "Use HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCurve().setCoefficient5z(double) instead.");
    CurveQuadLinear curve = getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient5z(heatingCompressorPowerCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
