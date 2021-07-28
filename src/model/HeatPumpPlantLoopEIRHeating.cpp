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
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_PlantLoop_EIR_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "HeatPumpPlantLoopEIRCooling.hpp"
#include "HeatPumpPlantLoopEIRCooling_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpPlantLoopEIRHeating::iddObjectType());
    }

    HeatPumpPlantLoopEIRHeating_Impl::HeatPumpPlantLoopEIRHeating_Impl(const HeatPumpPlantLoopEIRHeating_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpPlantLoopEIRHeating_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heat Pump Electricity Energy",
                                                   "Heat Pump Load Side Heat Transfer Energy",
                                                   "Heat Pump Source Side Heat Transfer Energy",
                                                   "Heat Pump Electricity Rate",
                                                   "Heat Pump Load Side Heat Transfer Rate",
                                                   "Heat Pump Source Side Heat Transfer Rate",
                                                   "Heat Pump Load Side Outlet Temperature",
                                                   "Heat Pump Load Side Inlet Temperature",
                                                   "Heat Pump Source Side Outlet Temperature",
                                                   "Heat Pump Source Side Inlet Temperature",
                                                   "Heat Pump Load Side Mass Flow Rate",
                                                   "Heat Pump Source Side Mass Flow Rate"};
      return result;
    }

    IddObjectType HeatPumpPlantLoopEIRHeating_Impl::iddObjectType() const {
      return HeatPumpPlantLoopEIRHeating::iddObjectType();
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::supplyOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideOutletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRHeating_Impl::demandOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideOutletNodeName;
    }

    std::string HeatPumpPlantLoopEIRHeating_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating_Impl::companionCoolingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpPlantLoopEIRCooling>(
        OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName);
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceLoadSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceLoadSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceSourceSideFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceSourceSideFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::referenceCapacity() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, true);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::isReferenceCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, true);
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

    Curve HeatPumpPlantLoopEIRHeating_Impl::capacityModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRHeating_Impl::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRHeating_Impl::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
      WorkspaceObject wo =
        getTarget(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCondenserType(std::string condenserType) {
      return setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType, condenserType);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
      return this->setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::CompanionHeatPumpName, companionHP.handle());
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, referenceLoadSideFlowRate);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceLoadSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::LoadSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, referenceSourceSideFlowRate);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceSourceSideFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::SourceSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceCapacity(double referenceCapacity) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, referenceCapacity);
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosizeReferenceCapacity() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setSizingFactor(double sizingFactor) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_HeatingFields::SizingFactor, sizingFactor);
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::CapacityModifierFunctionofTemperatureCurveName,
                               capacityModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                               electricInputtoOutputRatioModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRHeating_Impl::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_HeatingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                               electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.handle());
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceLoadSideFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceSourceSideFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRHeating_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void HeatPumpPlantLoopEIRHeating_Impl::autosize() {
      autosizeReferenceLoadSideFlowRate();
      autosizeReferenceSourceSideFlowRate();
      autosizeReferenceCapacity();
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

      val = autosizedReferenceCapacity();
      if (val) {
        setReferenceCapacity(val.get());
      }
    }

  }  // namespace detail

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model, const Curve& capacityModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeReferenceCapacity();

    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s capacity modifier function of temperature curve to "
                                     << capacityModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of temperature curve to "
                                     << electricInputtoOutputRatioModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of part load ratio curve to "
                                     << electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.briefDescription() << ".");
    }

    setCondenserType("WaterSource");
    setReferenceCoefficientofPerformance(7.5);  // IDD default
    setSizingFactor(1.0);
  }

  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>());

    autosizeReferenceLoadSideFlowRate();
    autosizeReferenceSourceSideFlowRate();
    autosizeReferenceCapacity();

    CurveBiquadratic capacityModifierFunctionofTemperatureCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    capacityModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    capacityModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveBiquadratic electricInputtoOutputRatioModifierFunctionofTemperatureCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveQuadratic electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMinimumValueofx(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMaximumValueofx(1);
    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    OS_ASSERT(ok);

    setCondenserType("WaterSource");
    setReferenceCoefficientofPerformance(7.5);
    setSizingFactor(1.0);
  }

  IddObjectType HeatPumpPlantLoopEIRHeating::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating);
  }

  std::vector<std::string> HeatPumpPlantLoopEIRHeating::validCondenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_PlantLoop_EIR_HeatingFields::CondenserType);
  }

  std::string HeatPumpPlantLoopEIRHeating::condenserType() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->condenserType();
  }

  boost::optional<HeatPumpPlantLoopEIRCooling> HeatPumpPlantLoopEIRHeating::companionCoolingHeatPump() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->companionCoolingHeatPump();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceLoadSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceLoadSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceSourceSideFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceSourceSideFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::referenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::isReferenceCapacityAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->isReferenceCapacityAutosized();
  }

  double HeatPumpPlantLoopEIRHeating::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpPlantLoopEIRHeating::sizingFactor() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->sizingFactor();
  }

  Curve HeatPumpPlantLoopEIRHeating::capacityModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->capacityModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRHeating::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRHeating::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
  }

  bool HeatPumpPlantLoopEIRHeating::setCondenserType(std::string condenserType) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCondenserType(condenserType);
  }

  bool HeatPumpPlantLoopEIRHeating::setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCompanionCoolingHeatPump(companionHP);
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceLoadSideFlowRate(referenceLoadSideFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceLoadSideFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceLoadSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceSourceSideFlowRate(referenceSourceSideFlowRate);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceSourceSideFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceSourceSideFlowRate();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceCapacity(double referenceCapacity) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  void HeatPumpPlantLoopEIRHeating::autosizeReferenceCapacity() {
    getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizeReferenceCapacity();
  }

  bool HeatPumpPlantLoopEIRHeating::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpPlantLoopEIRHeating::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpPlantLoopEIRHeating::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setCapacityModifierFunctionofTemperatureCurve(
      capacityModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRHeating::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceLoadSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceLoadSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceSourceSideFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceSourceSideFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRHeating::autosizedReferenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRHeating_Impl>()->autosizedReferenceCapacity();
  }

  /// @cond
  HeatPumpPlantLoopEIRHeating::HeatPumpPlantLoopEIRHeating(std::shared_ptr<detail::HeatPumpPlantLoopEIRHeating_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
