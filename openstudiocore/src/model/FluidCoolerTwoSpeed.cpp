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

#include "FluidCoolerTwoSpeed.hpp"
#include "FluidCoolerTwoSpeed_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_FluidCooler_TwoSpeed_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Unit.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FluidCoolerTwoSpeed_Impl::FluidCoolerTwoSpeed_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FluidCoolerTwoSpeed::iddObjectType());
  }

  FluidCoolerTwoSpeed_Impl::FluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FluidCoolerTwoSpeed::iddObjectType());
  }

  FluidCoolerTwoSpeed_Impl::FluidCoolerTwoSpeed_Impl(const FluidCoolerTwoSpeed_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FluidCoolerTwoSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FluidCoolerTwoSpeed_Impl::iddObjectType() const {
    return FluidCoolerTwoSpeed::iddObjectType();
  }

  unsigned FluidCoolerTwoSpeed_Impl::inletPort()
  {
    return  OS_FluidCooler_TwoSpeedFields::WaterInletNode;
  }

  unsigned FluidCoolerTwoSpeed_Impl::outletPort()
  {
    return  OS_FluidCooler_TwoSpeedFields::WaterOutletNode;
  }

  std::string FluidCoolerTwoSpeed_Impl::performanceInputMethod() const {
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::PerformanceInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::highFanSpeedUfactorTimesAreaValue() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isHighFanSpeedUfactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::lowFanSpeedUfactorTimesAreaValue() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isLowFanSpeedUfactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FluidCoolerTwoSpeed_Impl::lowFanSpeedUFactorTimesAreaSizingFactor() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerTwoSpeed_Impl::highSpeedNominalCapacity() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::HighSpeedNominalCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::lowSpeedNominalCapacity() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isLowSpeedNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FluidCoolerTwoSpeed_Impl::lowSpeedNominalCapacitySizingFactor() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerTwoSpeed_Impl::designEnteringWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerTwoSpeed_Impl::designEnteringAirTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerTwoSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::DesignWaterFlowRate,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::highFanSpeedAirFlowRate() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isHighFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::highFanSpeedFanPower() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedFanPower,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isHighFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::lowFanSpeedAirFlowRate() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isLowFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FluidCoolerTwoSpeed_Impl::lowFanSpeedAirFlowRateSizingFactor() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerTwoSpeed_Impl::lowFanSpeedFanPower() const {
    return getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPower,true);
  }

  bool FluidCoolerTwoSpeed_Impl::isLowFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FluidCoolerTwoSpeed_Impl::lowFanSpeedFanPowerSizingFactor() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> FluidCoolerTwoSpeed_Impl::outdoorAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_FluidCooler_TwoSpeedFields::OutdoorAirInletNode);
  }

  bool FluidCoolerTwoSpeed_Impl::setPerformanceInputMethod(std::string performanceInputMethod) {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::PerformanceInputMethod, performanceInputMethod);
    return result;
  }

  bool FluidCoolerTwoSpeed_Impl::setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue) {
    bool result(false);
    if (highFanSpeedUfactorTimesAreaValue) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, highFanSpeedUfactorTimesAreaValue.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue) {
    bool result(false);
    if (lowFanSpeedUfactorTimesAreaValue) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, lowFanSpeedUfactorTimesAreaValue.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  void FluidCoolerTwoSpeed_Impl::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, lowFanSpeedUFactorTimesAreaSizingFactor);
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setHighSpeedNominalCapacity(double highSpeedNominalCapacity) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::HighSpeedNominalCapacity, highSpeedNominalCapacity);
    return result;
  }

  bool FluidCoolerTwoSpeed_Impl::setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity) {
    bool result(false);
    if (lowSpeedNominalCapacity) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, lowSpeedNominalCapacity.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeLowSpeedNominalCapacity() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  void FluidCoolerTwoSpeed_Impl::setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, lowSpeedNominalCapacitySizingFactor);
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, designEnteringWaterTemperature);
    return result;
  }

  bool FluidCoolerTwoSpeed_Impl::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, designEnteringAirTemperature);
    return result;
  }

  bool FluidCoolerTwoSpeed_Impl::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, designEnteringAirWetbulbTemperature);
    return result;
  }

  bool FluidCoolerTwoSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate) {
    bool result(false);
    if (highFanSpeedAirFlowRate) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, highFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedAirFlowRate() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower) {
    bool result(false);
    if (highFanSpeedFanPower) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, highFanSpeedFanPower.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedFanPower() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate) {
    bool result(false);
    if (lowFanSpeedAirFlowRate) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, lowFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedAirFlowRate() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void FluidCoolerTwoSpeed_Impl::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, lowFanSpeedAirFlowRateSizingFactor);
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower) {
    bool result(false);
    if (lowFanSpeedFanPower) {
      result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, lowFanSpeedFanPower.get());
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedFanPower() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, "autosize");
    OS_ASSERT(result);
  }

  void FluidCoolerTwoSpeed_Impl::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
    bool result = setDouble(OS_FluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, lowFanSpeedFanPowerSizingFactor);
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::setOutdoorAirInletNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_FluidCooler_TwoSpeedFields::OutdoorAirInletNode, node.get().handle());
    }
    else {
      resetOutdoorAirInletNode();
      result = true;
    }
    return result;
  }

  void FluidCoolerTwoSpeed_Impl::resetOutdoorAirInletNode() {
    bool result = setString(OS_FluidCooler_TwoSpeedFields::OutdoorAirInletNode, "");
    OS_ASSERT(result);
  }

  bool FluidCoolerTwoSpeed_Impl::addToNode(Node & node)
  {
    if(auto plant = node.plantLoop()) {
      if( plant->supplyComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

FluidCoolerTwoSpeed::FluidCoolerTwoSpeed(const Model& model)
  : StraightComponent(FluidCoolerTwoSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FluidCoolerTwoSpeed_Impl>());

  setPerformanceInputMethod("NominalCapacity");
  autosizeHighFanSpeedUfactorTimesAreaValue();
  autosizeLowFanSpeedUfactorTimesAreaValue();
  setLowFanSpeedUFactorTimesAreaSizingFactor(0.6);
  setHighSpeedNominalCapacity(58601.0);
  setLowSpeedNominalCapacity(28601.0);
  setLowSpeedNominalCapacitySizingFactor(0.5);
  setDesignEnteringWaterTemperature(51.67);
  setDesignEnteringAirTemperature(35);
  setDesignEnteringAirWetbulbTemperature(25.6);
  setDesignWaterFlowRate(0.001388);
  setHighFanSpeedAirFlowRate(9.911);
  autosizeHighFanSpeedFanPower();
  setLowFanSpeedAirFlowRate(4.955);
  setLowFanSpeedAirFlowRateSizingFactor(0.5);
  autosizeLowFanSpeedFanPower();
  setLowFanSpeedFanPowerSizingFactor(0.16);
}

IddObjectType FluidCoolerTwoSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_FluidCooler_TwoSpeed);
}

std::vector<std::string> FluidCoolerTwoSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_FluidCooler_TwoSpeedFields::PerformanceInputMethod);
}

std::string FluidCoolerTwoSpeed::performanceInputMethod() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->performanceInputMethod();
}

boost::optional<double> FluidCoolerTwoSpeed::highFanSpeedUfactorTimesAreaValue() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->highFanSpeedUfactorTimesAreaValue();
}

bool FluidCoolerTwoSpeed::isHighFanSpeedUfactorTimesAreaValueAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isHighFanSpeedUfactorTimesAreaValueAutosized();
}

boost::optional<double> FluidCoolerTwoSpeed::lowFanSpeedUfactorTimesAreaValue() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedUfactorTimesAreaValue();
}

bool FluidCoolerTwoSpeed::isLowFanSpeedUfactorTimesAreaValueAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isLowFanSpeedUfactorTimesAreaValueAutosized();
}

double FluidCoolerTwoSpeed::lowFanSpeedUFactorTimesAreaSizingFactor() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedUFactorTimesAreaSizingFactor();
}

double FluidCoolerTwoSpeed::highSpeedNominalCapacity() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->highSpeedNominalCapacity();
}

boost::optional<double> FluidCoolerTwoSpeed::lowSpeedNominalCapacity() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowSpeedNominalCapacity();
}

bool FluidCoolerTwoSpeed::isLowSpeedNominalCapacityAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isLowSpeedNominalCapacityAutosized();
}

double FluidCoolerTwoSpeed::lowSpeedNominalCapacitySizingFactor() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowSpeedNominalCapacitySizingFactor();
}

double FluidCoolerTwoSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->designEnteringWaterTemperature();
}

double FluidCoolerTwoSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->designEnteringAirTemperature();
}

double FluidCoolerTwoSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

boost::optional<double> FluidCoolerTwoSpeed::designWaterFlowRate() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->designWaterFlowRate();
}

bool FluidCoolerTwoSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> FluidCoolerTwoSpeed::highFanSpeedAirFlowRate() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->highFanSpeedAirFlowRate();
}

bool FluidCoolerTwoSpeed::isHighFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isHighFanSpeedAirFlowRateAutosized();
}

boost::optional<double> FluidCoolerTwoSpeed::highFanSpeedFanPower() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->highFanSpeedFanPower();
}

bool FluidCoolerTwoSpeed::isHighFanSpeedFanPowerAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isHighFanSpeedFanPowerAutosized();
}

boost::optional<double> FluidCoolerTwoSpeed::lowFanSpeedAirFlowRate() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedAirFlowRate();
}

bool FluidCoolerTwoSpeed::isLowFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isLowFanSpeedAirFlowRateAutosized();
}

double FluidCoolerTwoSpeed::lowFanSpeedAirFlowRateSizingFactor() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedAirFlowRateSizingFactor();
}

boost::optional<double> FluidCoolerTwoSpeed::lowFanSpeedFanPower() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedFanPower();
}

bool FluidCoolerTwoSpeed::isLowFanSpeedFanPowerAutosized() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->isLowFanSpeedFanPowerAutosized();
}

double FluidCoolerTwoSpeed::lowFanSpeedFanPowerSizingFactor() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->lowFanSpeedFanPowerSizingFactor();
}

boost::optional<Node> FluidCoolerTwoSpeed::outdoorAirInletNode() const {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->outdoorAirInletNode();
}

bool FluidCoolerTwoSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

bool FluidCoolerTwoSpeed::setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setHighFanSpeedUfactorTimesAreaValue(highFanSpeedUfactorTimesAreaValue);
}

void FluidCoolerTwoSpeed::autosizeHighFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedUfactorTimesAreaValue();
}

bool FluidCoolerTwoSpeed::setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedUfactorTimesAreaValue(lowFanSpeedUfactorTimesAreaValue);
}

void FluidCoolerTwoSpeed::autosizeLowFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedUfactorTimesAreaValue();
}

void FluidCoolerTwoSpeed::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedUFactorTimesAreaSizingFactor(lowFanSpeedUFactorTimesAreaSizingFactor);
}

bool FluidCoolerTwoSpeed::setHighSpeedNominalCapacity(double highSpeedNominalCapacity) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setHighSpeedNominalCapacity(highSpeedNominalCapacity);
}

bool FluidCoolerTwoSpeed::setLowSpeedNominalCapacity(double lowSpeedNominalCapacity) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowSpeedNominalCapacity(lowSpeedNominalCapacity);
}

void FluidCoolerTwoSpeed::autosizeLowSpeedNominalCapacity() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeLowSpeedNominalCapacity();
}

void FluidCoolerTwoSpeed::setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor) {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowSpeedNominalCapacitySizingFactor(lowSpeedNominalCapacitySizingFactor);
}

bool FluidCoolerTwoSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

bool FluidCoolerTwoSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

bool FluidCoolerTwoSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

bool FluidCoolerTwoSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void FluidCoolerTwoSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool FluidCoolerTwoSpeed::setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setHighFanSpeedAirFlowRate(highFanSpeedAirFlowRate);
}

void FluidCoolerTwoSpeed::autosizeHighFanSpeedAirFlowRate() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedAirFlowRate();
}

bool FluidCoolerTwoSpeed::setHighFanSpeedFanPower(double highFanSpeedFanPower) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setHighFanSpeedFanPower(highFanSpeedFanPower);
}

void FluidCoolerTwoSpeed::autosizeHighFanSpeedFanPower() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedFanPower();
}

bool FluidCoolerTwoSpeed::setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRate(lowFanSpeedAirFlowRate);
}

void FluidCoolerTwoSpeed::autosizeLowFanSpeedAirFlowRate() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedAirFlowRate();
}

void FluidCoolerTwoSpeed::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRateSizingFactor(lowFanSpeedAirFlowRateSizingFactor);
}

bool FluidCoolerTwoSpeed::setLowFanSpeedFanPower(double lowFanSpeedFanPower) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedFanPower(lowFanSpeedFanPower);
}

void FluidCoolerTwoSpeed::autosizeLowFanSpeedFanPower() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedFanPower();
}

void FluidCoolerTwoSpeed::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setLowFanSpeedFanPowerSizingFactor(lowFanSpeedFanPowerSizingFactor);
}

bool FluidCoolerTwoSpeed::setOutdoorAirInletNode(const Node& node) {
  return getImpl<detail::FluidCoolerTwoSpeed_Impl>()->setOutdoorAirInletNode(node);
}

void FluidCoolerTwoSpeed::resetOutdoorAirInletNode() {
  getImpl<detail::FluidCoolerTwoSpeed_Impl>()->resetOutdoorAirInletNode();
}

/// @cond
FluidCoolerTwoSpeed::FluidCoolerTwoSpeed(std::shared_ptr<detail::FluidCoolerTwoSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

