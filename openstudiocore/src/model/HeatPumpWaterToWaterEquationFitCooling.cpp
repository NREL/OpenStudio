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

#include "HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpWaterToWaterEquationFitCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpWaterToWaterEquationFitCooling_Impl::iddObjectType() const {
    return HeatPumpWaterToWaterEquationFitCooling::iddObjectType();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitCooling_Impl::referenceLoadSideFlowRate() const {
    return getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate,true);
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedLoadSideFlowRate() const {
    if( auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate,true) ) {
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
    if( auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, true) ) {
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

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
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
    return getObject<ModelObject>().getModelObjectTarget<HeatPumpWaterToWaterEquationFitHeating>(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName);
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

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1, coolingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2, coolingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3, coolingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4, coolingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5, coolingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1, coolingCompressorPowerCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2, coolingCompressorPowerCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3, coolingCompressorPowerCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4, coolingCompressorPowerCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5, coolingCompressorPowerCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SizingFactor, sizingFactor);
    OS_ASSERT(result);
  }

  bool HeatPumpWaterToWaterEquationFitCooling_Impl::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP)
  {
    return this->setPointer(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName, companionHP.handle());
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName;
  }

} // detail

HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(const Model& model)
  : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>());

  autosizeReferenceLoadSideFlowRate();
  autosizeReferenceSourceSideFlowRate();
  autosizeRatedCoolingCapacity();
  autosizeRatedCoolingPowerConsumption();
  setCoolingCapacityCoefficient1(-1.52030596);
  setCoolingCapacityCoefficient2(3.46625667);
  setCoolingCapacityCoefficient3(-1.32267797);
  setCoolingCapacityCoefficient4(0.09395678);
  setCoolingCapacityCoefficient5(0.038975504);
  setCoolingCompressorPowerCoefficient1(-8.59564386);
  setCoolingCompressorPowerCoefficient2(0.96265085);
  setCoolingCompressorPowerCoefficient3(8.69489229);
  setCoolingCompressorPowerCoefficient4(0.02501669);
  setCoolingCompressorPowerCoefficient5(-0.20132665);
  setReferenceCoefficientofPerformance(8.0);
  setSizingFactor(1.0);
  // Trick to make IG not do weird things
  setString(22,"");
}

IddObjectType HeatPumpWaterToWaterEquationFitCooling::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling);
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

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient1();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient2();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient3();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient4();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient5();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient1();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient2();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient3();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient4();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient5();
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

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient1(coolingCapacityCoefficient1);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient2(coolingCapacityCoefficient2);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient3(coolingCapacityCoefficient3);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient4(coolingCapacityCoefficient4);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient5(coolingCapacityCoefficient5);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient1(coolingCompressorPowerCoefficient1);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient2(coolingCompressorPowerCoefficient2);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient3(coolingCompressorPowerCoefficient3);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient4(coolingCompressorPowerCoefficient4);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient5(coolingCompressorPowerCoefficient5);
}

void HeatPumpWaterToWaterEquationFitCooling::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
}

void HeatPumpWaterToWaterEquationFitCooling::setSizingFactor(double sizingFactor) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setSizingFactor(sizingFactor);
}

bool HeatPumpWaterToWaterEquationFitCooling::setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCompanionHeatingHeatPump(companionHP);
}

/// @cond
HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

