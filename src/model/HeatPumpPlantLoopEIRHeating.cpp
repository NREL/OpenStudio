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

#include "HeatPumpPlantLoopEIRHeating.hpp"
#include "HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "Model.hpp"
#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "HeatPumpPlantLoopEIRCooling.hpp"
#include "HeatPumpPlantLoopEIRCooling_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const HeatPumpPlantLoopEIRHeating_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpPlantLoopEIRHeating_Impl::outputVariableNames() const {
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

    IddObjectType HeatPumpPlantLoopEIRHeating_Impl::iddObjectType() const {
      return HeatPumpPlantLoopEIRHeating::iddObjectType();
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceLoadSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, true);
    }

    double HeatPumpPlantLoopEIRHeating_Impl::ratedLoadSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceLoadSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceSourceSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, true);
    }

    double HeatPumpPlantLoopEIRHeating_Impl::ratedSourceSideFlowRate() const {
      if (auto value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, true)) {
        return value.get();
      } else {
        return -999.0;
      }
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceSourceSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::ratedHeatingCapacity() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingCapacity, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isRatedHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::ratedHeatingPowerConsumption() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingPowerConsumption, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isRatedHeatingPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpPlantLoopEIRHeating_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRHeating_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating_Impl::companionCoolingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpPlantLoopEIRCooling>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionCoolingHeatPumpName);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, referenceLoadSideFlowRate);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, ratedLoadSideFlowRate);
      return result;
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceLoadSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceLoadSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, referenceSourceSideFlowRate);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, ratedSourceSideFlowRate);
      return result;
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceSourceSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceSourceSideFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingCapacity, ratedHeatingCapacity);
      return result;
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeRatedHeatingCapacity() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingPowerConsumption, ratedHeatingPowerConsumption);
      return result;
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeRatedHeatingPowerConsumption() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceHeatingPowerConsumption, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      bool result =
        setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, sizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
      return this->setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionCoolingHeatPumpName, companionHP.handle());
    }

    CurveQuadLinear HeatPumpPlantLoopEIRHeating_Impl::heatingCapacityCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingCapacityCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingCapacityCurveName, heatingCapacityCurve.handle());
      return result;
    }

    CurveQuadLinear HeatPumpPlantLoopEIRHeating_Impl::heatingCompressorPowerCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingCompressorPowerCurveName).get();
      return wo.cast<CurveQuadLinear>();
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::HeatingCompressorPowerCurveName, heatingCompressorPowerCurve.handle());
      return result;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceLoadSideFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceSourceSideFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedRatedHeatingCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedRatedHeatingPowerConsumption() const {
      return getAutosizedValue("Design Size Heating Power Consumption", "W");
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosize() {
      autosizeReferenceLoadSideFlowRate();
      autosizeReferenceSourceSideFlowRate();
      autosizeRatedHeatingCapacity();
      autosizeRatedHeatingPowerConsumption();
    }

    void HeatPumpPlantLoopEIRHeating_Impl::applySizingValues() {
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

  }  // namespace detail

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model, const CurveQuadLinear& heatingCapacityCurve,
                                                                                 const CurveQuadLinear& heatingCompressorPowerCurve)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

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

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

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

  IddObjectType HeatPumpPlantLoopEIRHeating::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating);
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceLoadSideFlowRate();
  }

  double HeatPumpPlantLoopEIRHeating::ratedLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->ratedLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceLoadSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceLoadSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceSourceSideFlowRate();
  }

  double HeatPumpPlantLoopEIRHeating::ratedSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->ratedSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceSourceSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceSourceSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::ratedHeatingCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->ratedHeatingCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::isRatedHeatingCapacityAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isRatedHeatingCapacityAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::ratedHeatingPowerConsumption() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->ratedHeatingPowerConsumption();
  }

  bool HeatPumpPlantLoopEIRHeating::isRatedHeatingPowerConsumptionAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isRatedHeatingPowerConsumptionAutosized();
  }

  double HeatPumpPlantLoopEIRHeating::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpPlantLoopEIRHeating::sizingFactor() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sizingFactor();
  }

  boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating::companionCoolingHeatPump() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->companionCoolingHeatPump();
  }

  bool HeatPumpPlantLoopEIRHeating::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setRatedLoadSideFlowRate(ratedLoadSideFlowRate);
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceLoadSideFlowRate(referenceLoadSideFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceLoadSideFlowRate() {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setRatedSourceSideFlowRate(ratedSourceSideFlowRate);
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceSourceSideFlowRate(referenceSourceSideFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceSourceSideFlowRate() {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeRatedHeatingCapacity() {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeRatedHeatingCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setRatedHeatingPowerConsumption(ratedHeatingPowerConsumption);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeRatedHeatingPowerConsumption() {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeRatedHeatingPowerConsumption();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpPlantLoopEIRHeating::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpPlantLoopEIRHeating::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCompanionCoolingHeatPump(companionHP);
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceLoadSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceSourceSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedRatedHeatingCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedRatedHeatingCapacity();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedRatedHeatingPowerConsumption() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedRatedHeatingPowerConsumption();
  }

  CurveQuadLinear HeatPumpPlantLoopEIRHeating::heatingCapacityCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatingCapacityCurve(heatingCapacityCurve);
  }

  CurveQuadLinear HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setHeatingCompressorPowerCurve(heatingCompressorPowerCurve);
  }

  /// @cond
  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(
    std::shared_ptr<detail::HeatPumpPlantLoopEIRHeating_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}

  // DEPRECATED
  double HeatPumpPlantLoopEIRHeating::heatingCapacityCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient1Constant(heatingCapacityCoefficient1);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCapacityCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient2w(heatingCapacityCoefficient2);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCapacityCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient3x(heatingCapacityCoefficient3);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCapacityCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient4y(heatingCapacityCoefficient4);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCapacityCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCapacityCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCapacityCurve();
    return curve.setCoefficient5z(heatingCapacityCoefficient5);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCoefficient1() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().coefficient1Constant() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient1Constant();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
    LOG(
      Warn,
      "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().setCoefficient1Constant(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient1Constant(heatingCompressorPowerCoefficient1);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCoefficient2() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().coefficient2w() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient2w();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().setCoefficient2w(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient2w(heatingCompressorPowerCoefficient2);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCoefficient3() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().coefficient3x() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient3x();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().setCoefficient3x(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient3x(heatingCompressorPowerCoefficient3);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCoefficient4() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().coefficient4y() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient4y();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().setCoefficient4y(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient4y(heatingCompressorPowerCoefficient4);
  }

  double HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCoefficient5() const {
    LOG(Warn, "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().coefficient5z() instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.coefficient5z();
  }

  bool HeatPumpPlantLoopEIRHeating::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
    LOG(Warn,
        "This method is deprecated. Use HeatPumpPlantLoopEIRHeating::heatingCompressorPowerCurve().setCoefficient5z(double) instead.")
    CurveQuadLinear curve = getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->heatingCompressorPowerCurve();
    return curve.setCoefficient5z(heatingCompressorPowerCoefficient5);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
