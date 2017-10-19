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

#include "HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const HeatPumpWaterToWaterEquationFitHeating_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpWaterToWaterEquationFitHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpWaterToWaterEquationFitHeating_Impl::iddObjectType() const {
    return HeatPumpWaterToWaterEquationFitHeating::iddObjectType();
  }

  boost::optional<double> HeatPumpWaterToWaterEquationFitHeating_Impl::referenceLoadSideFlowRate() const {
    return getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate,true);
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedLoadSideFlowRate() const {
    if( auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate,true) ) {
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
    if( auto value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, true) ) {
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

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
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
    return getObject<ModelObject>().getModelObjectTarget<HeatPumpWaterToWaterEquationFitCooling>(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName);
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

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1, heatingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2, heatingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3, heatingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4, heatingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5, heatingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1, heatingCompressorPowerCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2, heatingCompressorPowerCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3, heatingCompressorPowerCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4, heatingCompressorPowerCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5, heatingCompressorPowerCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SizingFactor, sizingFactor);
    OS_ASSERT(result);
  }

  bool HeatPumpWaterToWaterEquationFitHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP)
  {
    return this->setPointer(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName, companionHP.handle());
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName;
  }

} // detail

HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(const Model& model)
  : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>());

  autosizeReferenceLoadSideFlowRate();
  autosizeReferenceSourceSideFlowRate();
  autosizeRatedHeatingCapacity();
  autosizeRatedHeatingPowerConsumption();
  setHeatingCapacityCoefficient1(-3.33491153);
  setHeatingCapacityCoefficient2(-0.51451946);
  setHeatingCapacityCoefficient3(4.51592706);
  setHeatingCapacityCoefficient4(0.01797107);
  setHeatingCapacityCoefficient5(0.155797661);
  setHeatingCompressorPowerCoefficient1(-8.93121751);
  setHeatingCompressorPowerCoefficient2(8.57035762);
  setHeatingCompressorPowerCoefficient3(1.29660976);
  setHeatingCompressorPowerCoefficient4(-0.21629222);
  setHeatingCompressorPowerCoefficient5(0.033862378);
  setReferenceCoefficientofPerformance(7.5);
  setSizingFactor(1.0);
  // Trick to make IG not do weird things
  setString(22,"");
}

IddObjectType HeatPumpWaterToWaterEquationFitHeating::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating);
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

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient1();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient2();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient3();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient4();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient5();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient1();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient2();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient3();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient4();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient5();
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

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient1(heatingCapacityCoefficient1);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient2(heatingCapacityCoefficient2);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient3(heatingCapacityCoefficient3);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient4(heatingCapacityCoefficient4);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient5(heatingCapacityCoefficient5);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient1(heatingCompressorPowerCoefficient1);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient2(heatingCompressorPowerCoefficient2);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient3(heatingCompressorPowerCoefficient3);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient4(heatingCompressorPowerCoefficient4);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient5(heatingCompressorPowerCoefficient5);
}

void HeatPumpWaterToWaterEquationFitHeating::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
}

void HeatPumpWaterToWaterEquationFitHeating::setSizingFactor(double sizingFactor) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setSizingFactor(sizingFactor);
}

bool HeatPumpWaterToWaterEquationFitHeating::setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setCompanionCoolingHeatPump(companionHP);
}


/// @cond
HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

