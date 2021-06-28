/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatPumpPlantLoopEIRCooling.hpp"
#include "HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "Model.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_PlantLoop_EIR_Cooling_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "HeatPumpPlantLoopEIRHeating.hpp"
#include "HeatPumpPlantLoopEIRHeating_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpPlantLoopEIRCooling::iddObjectType());
    }

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpPlantLoopEIRCooling::iddObjectType());
    }

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const HeatPumpPlantLoopEIRCooling_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpPlantLoopEIRCooling_Impl::outputVariableNames() const {
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

    IddObjectType HeatPumpPlantLoopEIRCooling_Impl::iddObjectType() const {
      return HeatPumpPlantLoopEIRCooling::iddObjectType();
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::referenceLoadSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceLoadSideFlowRate, true);
    }

    double HeatPumpPlantLoopEIRCooling_Impl::ratedLoadSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceLoadSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isReferenceLoadSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceLoadSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::referenceSourceSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceSourceSideFlowRate, true);
    }

    double HeatPumpPlantLoopEIRCooling_Impl::ratedSourceSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceSourceSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isReferenceSourceSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceSourceSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::ratedCoolingCapacity() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingCapacity, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isRatedCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::ratedCoolingPowerConsumption() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingPowerConsumption, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isRatedCoolingPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpPlantLoopEIRCooling_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRCooling_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HeatPumpWaterToWaterEquationFitHeating> HeatPumpPlantLoopEIRCooling_Impl::companionHeatingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpWaterToWaterEquationFitHeating>(
        OS_HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatingHeatPumpName);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceLoadSideFlowRate, referenceLoadSideFlowRate);
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeReferenceLoadSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceLoadSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceSourceSideFlowRate, referenceSourceSideFlowRate);
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeReferenceSourceSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceSourceSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setRatedCoolingCapacity(double ratedCoolingCapacity) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingCapacity, ratedCoolingCapacity);
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeRatedCoolingCapacity() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingPowerConsumption, ratedCoolingPowerConsumption);
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeRatedCoolingPowerConsumption() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoolingPowerConsumption, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      bool result =
        setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, sizingFactor);
      OS_ASSERT(result);
      return result;
    }

    CurveQuadLinear HeatPumpPlantLoopEIRCooling_Impl::coolingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_CoolingFields::CoolingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::CoolingCapacityCurveName, coolingCapacityCurve.handle());
      return result;
    }

    CurveQuadLinear HeatPumpPlantLoopEIRCooling_Impl::coolingCompressorPowerCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_CoolingFields::CoolingCompressorPowerCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::CoolingCompressorPowerCurveName, coolingCompressorPowerCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP) {
      return this->setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatingHeatPumpName, companionHP.handle());
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::supplyInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::supplyOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::demandInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::demandOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedReferenceLoadSideFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedReferenceSourceSideFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedRatedCoolingCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedRatedCoolingPowerConsumption() const {
      return getAutosizedValue("Design Size Cooling Power Consumption", "W");
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosize() {
      autosizeReferenceLoadSideFlowRate();
      autosizeReferenceSourceSideFlowRate();
      autosizeRatedCoolingCapacity();
      autosizeRatedCoolingPowerConsumption();
    }

    void HeatPumpPlantLoopEIRCooling_Impl::applySizingValues() {
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

  }  // namespace detail

  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(const Model& model, const CurveQuadLinear& coolingCapacityCurve,
                                                                                 const CurveQuadLinear& coolingCompressorPowerCurve)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>());

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

  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>());

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

  IddObjectType HeatPumpPlantLoopEIRCooling::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatPump_PlantLoop_EIR_Cooling);
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::referenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->referenceLoadSideFlowRate();
  }

  double HeatPumpPlantLoopEIRCooling::ratedLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->ratedLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::isReferenceLoadSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isReferenceLoadSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::referenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->referenceSourceSideFlowRate();
  }

  double HeatPumpPlantLoopEIRCooling::ratedSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->ratedSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::isReferenceSourceSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isReferenceSourceSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::ratedCoolingCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->ratedCoolingCapacity();
  }

  bool HeatPumpPlantLoopEIRCooling::isRatedCoolingCapacityAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isRatedCoolingCapacityAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::ratedCoolingPowerConsumption() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->ratedCoolingPowerConsumption();
  }

  bool HeatPumpPlantLoopEIRCooling::isRatedCoolingPowerConsumptionAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isRatedCoolingPowerConsumptionAutosized();
  }

  double HeatPumpPlantLoopEIRCooling::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpPlantLoopEIRCooling::sizingFactor() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sizingFactor();
  }

  boost::optional<HeatPumpWaterToWaterEquationFitHeating> HeatPumpPlantLoopEIRCooling::companionHeatingHeatPump() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->companionHeatingHeatPump();
  }

  bool HeatPumpPlantLoopEIRCooling::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceLoadSideFlowRate(referenceLoadSideFlowRate);
  }

  bool HeatPumpPlantLoopEIRCooling::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceLoadSideFlowRate(ratedLoadSideFlowRate);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeReferenceLoadSideFlowRate() {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeReferenceLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceSourceSideFlowRate(referenceSourceSideFlowRate);
  }

  bool HeatPumpPlantLoopEIRCooling::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceSourceSideFlowRate(ratedSourceSideFlowRate);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeReferenceSourceSideFlowRate() {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeReferenceSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::setRatedCoolingCapacity(double ratedCoolingCapacity) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setRatedCoolingCapacity(ratedCoolingCapacity);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeRatedCoolingCapacity() {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeRatedCoolingCapacity();
  }

  bool HeatPumpPlantLoopEIRCooling::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setRatedCoolingPowerConsumption(ratedCoolingPowerConsumption);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeRatedCoolingPowerConsumption() {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeRatedCoolingPowerConsumption();
  }

  bool HeatPumpPlantLoopEIRCooling::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpPlantLoopEIRCooling::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpPlantLoopEIRCooling::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCompanionHeatingHeatPump(companionHP);
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedReferenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedReferenceLoadSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedReferenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedReferenceSourceSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedRatedCoolingCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedRatedCoolingCapacity();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedRatedCoolingPowerConsumption() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedRatedCoolingPowerConsumption();
  }

  CurveQuadLinear HeatPumpPlantLoopEIRCooling::coolingCapacityCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCoolingCapacityCurve(coolingCapacityCurve);
  }

  CurveQuadLinear HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCoolingCompressorPowerCurve(coolingCompressorPowerCurve);
  }

  /// @cond
  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(
    std::shared_ptr<detail::HeatPumpPlantLoopEIRCooling_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}

  // DEPRECATED
  double HeatPumpPlantLoopEIRCooling::coolingCapacityCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient1Constant(coolingCapacityCoefficient1);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCapacityCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient2w(coolingCapacityCoefficient2);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCapacityCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient3x(coolingCapacityCoefficient3);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCapacityCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient4y(coolingCapacityCoefficient4);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCapacityCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCapacityCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCapacityCurve();
    return curve.setCoefficient5z(coolingCapacityCoefficient5);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
    LOG(
      Warn,
      "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient1Constant(coolingCompressorPowerCoefficient1);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient2w(coolingCompressorPowerCoefficient2);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient3x(coolingCompressorPowerCoefficient3);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient4y(coolingCompressorPowerCoefficient4);
  }

  double HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpPlantLoopEIRCooling::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRCooling::coolingCompressorPowerCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->coolingCompressorPowerCurve();
    return curve.setCoefficient5z(coolingCompressorPowerCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
