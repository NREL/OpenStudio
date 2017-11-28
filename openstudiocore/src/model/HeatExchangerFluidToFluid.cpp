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

#include "HeatExchangerFluidToFluid.hpp"
#include "HeatExchangerFluidToFluid_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatExchanger_FluidToFluid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatExchangerFluidToFluid_Impl::HeatExchangerFluidToFluid_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatExchangerFluidToFluid::iddObjectType());
  }

  HeatExchangerFluidToFluid_Impl::HeatExchangerFluidToFluid_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatExchangerFluidToFluid::iddObjectType());
  }

  HeatExchangerFluidToFluid_Impl::HeatExchangerFluidToFluid_Impl(const HeatExchangerFluidToFluid_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatExchangerFluidToFluid_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatExchangerFluidToFluid_Impl::iddObjectType() const {
    return HeatExchangerFluidToFluid::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HeatExchangerFluidToFluid_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_HeatExchanger_FluidToFluidFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("HeatExchangerFluidToFluid","Availability"));
    }
    return result;
  }

  boost::optional<Schedule> HeatExchangerFluidToFluid_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeatExchanger_FluidToFluidFields::AvailabilityScheduleName);
  }

  boost::optional<double> HeatExchangerFluidToFluid_Impl::loopDemandSideDesignFlowRate() const {
    return getDouble(OS_HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate,true);
  }

  bool HeatExchangerFluidToFluid_Impl::isLoopDemandSideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> HeatExchangerFluidToFluid_Impl::loopSupplySideDesignFlowRate() const {
    return getDouble(OS_HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate,true);
  }

  bool HeatExchangerFluidToFluid_Impl::isLoopSupplySideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string HeatExchangerFluidToFluid_Impl::heatExchangeModelType() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::HeatExchangeModelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isHeatExchangeModelTypeDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::HeatExchangeModelType);
  }

  boost::optional<double> HeatExchangerFluidToFluid_Impl::heatExchangerUFactorTimesAreaValue() const {
    return getDouble(OS_HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue,true);
  }

  bool HeatExchangerFluidToFluid_Impl::isHeatExchangerUFactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string HeatExchangerFluidToFluid_Impl::controlType() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::ControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isControlTypeDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::ControlType);
  }

  double HeatExchangerFluidToFluid_Impl::minimumTemperatureDifferencetoActivateHeatExchanger() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger);
  }

  std::string HeatExchangerFluidToFluid_Impl::heatTransferMeteringEndUseType() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isHeatTransferMeteringEndUseTypeDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType);
  }

  boost::optional<Node> HeatExchangerFluidToFluid_Impl::componentOverrideLoopSupplySideInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopSupplySideInletNode);
  }

  boost::optional<Node> HeatExchangerFluidToFluid_Impl::componentOverrideLoopDemandSideInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopDemandSideInletNode);
  }

  std::string HeatExchangerFluidToFluid_Impl::componentOverrideCoolingControlTemperatureMode() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isComponentOverrideCoolingControlTemperatureModeDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode);
  }

  double HeatExchangerFluidToFluid_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_FluidToFluidFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatExchangerFluidToFluid_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_HeatExchanger_FluidToFluidFields::SizingFactor);
  }

  boost::optional<double> HeatExchangerFluidToFluid_Impl::operationMinimumTemperatureLimit() const {
    return getDouble(OS_HeatExchanger_FluidToFluidFields::OperationMinimumTemperatureLimit,true);
  }

  boost::optional<double> HeatExchangerFluidToFluid_Impl::operationMaximumTemperatureLimit() const {
    return getDouble(OS_HeatExchanger_FluidToFluidFields::OperationMaximumTemperatureLimit,true);
  }

  bool HeatExchangerFluidToFluid_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_HeatExchanger_FluidToFluidFields::AvailabilityScheduleName,
                              "HeatExchangerFluidToFluid",
                              "Availability",
                              schedule);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setLoopDemandSideDesignFlowRate(boost::optional<double> loopDemandSideDesignFlowRate) {
    bool result(false);
    if (loopDemandSideDesignFlowRate) {
      result = setDouble(OS_HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate, loopDemandSideDesignFlowRate.get());
    }
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::autosizeLoopDemandSideDesignFlowRate() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setLoopSupplySideDesignFlowRate(boost::optional<double> loopSupplySideDesignFlowRate) {
    bool result(false);
    if (loopSupplySideDesignFlowRate) {
      result = setDouble(OS_HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate, loopSupplySideDesignFlowRate.get());
    }
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::autosizeLoopSupplySideDesignFlowRate() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setHeatExchangeModelType(std::string heatExchangeModelType) {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::HeatExchangeModelType, heatExchangeModelType);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetHeatExchangeModelType() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::HeatExchangeModelType, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setHeatExchangerUFactorTimesAreaValue(boost::optional<double> heatExchangerUFactorTimesAreaValue) {
    bool result(false);
    if (heatExchangerUFactorTimesAreaValue) {
      result = setDouble(OS_HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue, heatExchangerUFactorTimesAreaValue.get());
    }
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::autosizeHeatExchangerUFactorTimesAreaValue() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setControlType(std::string controlType) {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ControlType, controlType);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetControlType() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ControlType, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger) {
    bool result = setDouble(OS_HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger, minimumTemperatureDifferencetoActivateHeatExchanger);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetMinimumTemperatureDifferencetoActivateHeatExchanger() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setHeatTransferMeteringEndUseType(std::string heatTransferMeteringEndUseType) {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType, heatTransferMeteringEndUseType);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetHeatTransferMeteringEndUseType() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setComponentOverrideLoopSupplySideInletNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopSupplySideInletNode, node.get().handle());
    }
    else {
      resetComponentOverrideLoopSupplySideInletNode();
      result = true;
    }
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetComponentOverrideLoopSupplySideInletNode() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopSupplySideInletNode, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setComponentOverrideLoopDemandSideInletNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopDemandSideInletNode, node.get().handle());
    }
    else {
      resetComponentOverrideLoopDemandSideInletNode();
      result = true;
    }
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetComponentOverrideLoopDemandSideInletNode() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideLoopDemandSideInletNode, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setComponentOverrideCoolingControlTemperatureMode(std::string componentOverrideCoolingControlTemperatureMode) {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode, componentOverrideCoolingControlTemperatureMode);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetComponentOverrideCoolingControlTemperatureMode() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerFluidToFluid_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_HeatExchanger_FluidToFluidFields::SizingFactor, sizingFactor);
    return result;
  }

  void HeatExchangerFluidToFluid_Impl::resetSizingFactor() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  void HeatExchangerFluidToFluid_Impl::setOperationMinimumTemperatureLimit(boost::optional<double> operationMinimumTemperatureLimit) {
    bool result(false);
    if (operationMinimumTemperatureLimit) {
      result = setDouble(OS_HeatExchanger_FluidToFluidFields::OperationMinimumTemperatureLimit, operationMinimumTemperatureLimit.get());
    }
    else {
      resetOperationMinimumTemperatureLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void HeatExchangerFluidToFluid_Impl::resetOperationMinimumTemperatureLimit() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::OperationMinimumTemperatureLimit, "");
    OS_ASSERT(result);
  }

  void HeatExchangerFluidToFluid_Impl::setOperationMaximumTemperatureLimit(boost::optional<double> operationMaximumTemperatureLimit) {
    bool result(false);
    if (operationMaximumTemperatureLimit) {
      result = setDouble(OS_HeatExchanger_FluidToFluidFields::OperationMaximumTemperatureLimit, operationMaximumTemperatureLimit.get());
    }
    else {
      resetOperationMaximumTemperatureLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void HeatExchangerFluidToFluid_Impl::resetOperationMaximumTemperatureLimit() {
    bool result = setString(OS_HeatExchanger_FluidToFluidFields::OperationMaximumTemperatureLimit, "");
    OS_ASSERT(result);
  }

  unsigned HeatExchangerFluidToFluid_Impl::supplyInletPort()
  {
    return OS_HeatExchanger_FluidToFluidFields::LoopSupplySideInletNode;
  }

  unsigned HeatExchangerFluidToFluid_Impl::supplyOutletPort()
  {
    return OS_HeatExchanger_FluidToFluidFields::LoopSupplySideOutletNode;
  }

  unsigned HeatExchangerFluidToFluid_Impl::demandInletPort()
  {
    return OS_HeatExchanger_FluidToFluidFields::LoopDemandSideInletNode;
  }

  unsigned HeatExchangerFluidToFluid_Impl::demandOutletPort()
  {
    return OS_HeatExchanger_FluidToFluidFields::LoopDemandSideOutletNode;
  }

} // detail

HeatExchangerFluidToFluid::HeatExchangerFluidToFluid(const Model& model)
  : WaterToWaterComponent(HeatExchangerFluidToFluid::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatExchangerFluidToFluid_Impl>());

  autosizeLoopDemandSideDesignFlowRate();
  autosizeLoopSupplySideDesignFlowRate();
  setHeatExchangeModelType("Ideal");
  autosizeHeatExchangerUFactorTimesAreaValue();
  setControlType("UncontrolledOn");
  setMinimumTemperatureDifferencetoActivateHeatExchanger(0.01);
  setHeatTransferMeteringEndUseType("LoopToLoop");
  setComponentOverrideCoolingControlTemperatureMode("Loop");
  setSizingFactor(1.0);
  setOperationMinimumTemperatureLimit(0.0);
  setOperationMaximumTemperatureLimit(100.0);
}

IddObjectType HeatExchangerFluidToFluid::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatExchanger_FluidToFluid);
}

std::vector<std::string> HeatExchangerFluidToFluid::heatExchangeModelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_FluidToFluidFields::HeatExchangeModelType);
}

std::vector<std::string> HeatExchangerFluidToFluid::controlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_FluidToFluidFields::ControlType);
}

std::vector<std::string> HeatExchangerFluidToFluid::heatTransferMeteringEndUseTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType);
}

std::vector<std::string> HeatExchangerFluidToFluid::componentOverrideCoolingControlTemperatureModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode);
}

boost::optional<Schedule> HeatExchangerFluidToFluid::availabilitySchedule() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->availabilitySchedule();
}

boost::optional<double> HeatExchangerFluidToFluid::loopDemandSideDesignFlowRate() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->loopDemandSideDesignFlowRate();
}

bool HeatExchangerFluidToFluid::isLoopDemandSideDesignFlowRateAutosized() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isLoopDemandSideDesignFlowRateAutosized();
}

boost::optional<double> HeatExchangerFluidToFluid::loopSupplySideDesignFlowRate() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->loopSupplySideDesignFlowRate();
}

bool HeatExchangerFluidToFluid::isLoopSupplySideDesignFlowRateAutosized() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isLoopSupplySideDesignFlowRateAutosized();
}

std::string HeatExchangerFluidToFluid::heatExchangeModelType() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->heatExchangeModelType();
}

bool HeatExchangerFluidToFluid::isHeatExchangeModelTypeDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isHeatExchangeModelTypeDefaulted();
}

boost::optional<double> HeatExchangerFluidToFluid::heatExchangerUFactorTimesAreaValue() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->heatExchangerUFactorTimesAreaValue();
}

bool HeatExchangerFluidToFluid::isHeatExchangerUFactorTimesAreaValueAutosized() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isHeatExchangerUFactorTimesAreaValueAutosized();
}

std::string HeatExchangerFluidToFluid::controlType() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->controlType();
}

bool HeatExchangerFluidToFluid::isControlTypeDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isControlTypeDefaulted();
}

double HeatExchangerFluidToFluid::minimumTemperatureDifferencetoActivateHeatExchanger() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->minimumTemperatureDifferencetoActivateHeatExchanger();
}

bool HeatExchangerFluidToFluid::isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted();
}

std::string HeatExchangerFluidToFluid::heatTransferMeteringEndUseType() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->heatTransferMeteringEndUseType();
}

bool HeatExchangerFluidToFluid::isHeatTransferMeteringEndUseTypeDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isHeatTransferMeteringEndUseTypeDefaulted();
}

boost::optional<Node> HeatExchangerFluidToFluid::componentOverrideLoopSupplySideInletNode() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->componentOverrideLoopSupplySideInletNode();
}

boost::optional<Node> HeatExchangerFluidToFluid::componentOverrideLoopDemandSideInletNode() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->componentOverrideLoopDemandSideInletNode();
}

std::string HeatExchangerFluidToFluid::componentOverrideCoolingControlTemperatureMode() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->componentOverrideCoolingControlTemperatureMode();
}

bool HeatExchangerFluidToFluid::isComponentOverrideCoolingControlTemperatureModeDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isComponentOverrideCoolingControlTemperatureModeDefaulted();
}

double HeatExchangerFluidToFluid::sizingFactor() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->sizingFactor();
}

bool HeatExchangerFluidToFluid::isSizingFactorDefaulted() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->isSizingFactorDefaulted();
}

boost::optional<double> HeatExchangerFluidToFluid::operationMinimumTemperatureLimit() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->operationMinimumTemperatureLimit();
}

boost::optional<double> HeatExchangerFluidToFluid::operationMaximumTemperatureLimit() const {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->operationMaximumTemperatureLimit();
}

bool HeatExchangerFluidToFluid::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setAvailabilitySchedule(schedule);
}

void HeatExchangerFluidToFluid::resetAvailabilitySchedule() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetAvailabilitySchedule();
}

bool HeatExchangerFluidToFluid::setLoopDemandSideDesignFlowRate(double loopDemandSideDesignFlowRate) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setLoopDemandSideDesignFlowRate(loopDemandSideDesignFlowRate);
}

void HeatExchangerFluidToFluid::autosizeLoopDemandSideDesignFlowRate() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->autosizeLoopDemandSideDesignFlowRate();
}

bool HeatExchangerFluidToFluid::setLoopSupplySideDesignFlowRate(double loopSupplySideDesignFlowRate) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setLoopSupplySideDesignFlowRate(loopSupplySideDesignFlowRate);
}

void HeatExchangerFluidToFluid::autosizeLoopSupplySideDesignFlowRate() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->autosizeLoopSupplySideDesignFlowRate();
}

bool HeatExchangerFluidToFluid::setHeatExchangeModelType(std::string heatExchangeModelType) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setHeatExchangeModelType(heatExchangeModelType);
}

void HeatExchangerFluidToFluid::resetHeatExchangeModelType() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetHeatExchangeModelType();
}

bool HeatExchangerFluidToFluid::setHeatExchangerUFactorTimesAreaValue(double heatExchangerUFactorTimesAreaValue) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setHeatExchangerUFactorTimesAreaValue(heatExchangerUFactorTimesAreaValue);
}

void HeatExchangerFluidToFluid::autosizeHeatExchangerUFactorTimesAreaValue() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->autosizeHeatExchangerUFactorTimesAreaValue();
}

bool HeatExchangerFluidToFluid::setControlType(std::string controlType) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setControlType(controlType);
}

void HeatExchangerFluidToFluid::resetControlType() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetControlType();
}

bool HeatExchangerFluidToFluid::setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setMinimumTemperatureDifferencetoActivateHeatExchanger(minimumTemperatureDifferencetoActivateHeatExchanger);
}

void HeatExchangerFluidToFluid::resetMinimumTemperatureDifferencetoActivateHeatExchanger() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetMinimumTemperatureDifferencetoActivateHeatExchanger();
}

bool HeatExchangerFluidToFluid::setHeatTransferMeteringEndUseType(std::string heatTransferMeteringEndUseType) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setHeatTransferMeteringEndUseType(heatTransferMeteringEndUseType);
}

void HeatExchangerFluidToFluid::resetHeatTransferMeteringEndUseType() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetHeatTransferMeteringEndUseType();
}

bool HeatExchangerFluidToFluid::setComponentOverrideLoopSupplySideInletNode(const Node& node) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setComponentOverrideLoopSupplySideInletNode(node);
}

void HeatExchangerFluidToFluid::resetComponentOverrideLoopSupplySideInletNode() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetComponentOverrideLoopSupplySideInletNode();
}

bool HeatExchangerFluidToFluid::setComponentOverrideLoopDemandSideInletNode(const Node& node) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setComponentOverrideLoopDemandSideInletNode(node);
}

void HeatExchangerFluidToFluid::resetComponentOverrideLoopDemandSideInletNode() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetComponentOverrideLoopDemandSideInletNode();
}

bool HeatExchangerFluidToFluid::setComponentOverrideCoolingControlTemperatureMode(std::string componentOverrideCoolingControlTemperatureMode) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setComponentOverrideCoolingControlTemperatureMode(componentOverrideCoolingControlTemperatureMode);
}

void HeatExchangerFluidToFluid::resetComponentOverrideCoolingControlTemperatureMode() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetComponentOverrideCoolingControlTemperatureMode();
}

bool HeatExchangerFluidToFluid::setSizingFactor(double sizingFactor) {
  return getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setSizingFactor(sizingFactor);
}

void HeatExchangerFluidToFluid::resetSizingFactor() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetSizingFactor();
}

void HeatExchangerFluidToFluid::setOperationMinimumTemperatureLimit(double operationMinimumTemperatureLimit) {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setOperationMinimumTemperatureLimit(operationMinimumTemperatureLimit);
}

void HeatExchangerFluidToFluid::resetOperationMinimumTemperatureLimit() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetOperationMinimumTemperatureLimit();
}

void HeatExchangerFluidToFluid::setOperationMaximumTemperatureLimit(double operationMaximumTemperatureLimit) {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->setOperationMaximumTemperatureLimit(operationMaximumTemperatureLimit);
}

void HeatExchangerFluidToFluid::resetOperationMaximumTemperatureLimit() {
  getImpl<detail::HeatExchangerFluidToFluid_Impl>()->resetOperationMaximumTemperatureLimit();
}

/// @cond
HeatExchangerFluidToFluid::HeatExchangerFluidToFluid(std::shared_ptr<detail::HeatExchangerFluidToFluid_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

