/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "FluidCoolerSingleSpeed.hpp"
#include "FluidCoolerSingleSpeed_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_FluidCooler_SingleSpeed_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Unit.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FluidCoolerSingleSpeed_Impl::FluidCoolerSingleSpeed_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FluidCoolerSingleSpeed::iddObjectType());
  }

  FluidCoolerSingleSpeed_Impl::FluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FluidCoolerSingleSpeed::iddObjectType());
  }

  FluidCoolerSingleSpeed_Impl::FluidCoolerSingleSpeed_Impl(const FluidCoolerSingleSpeed_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FluidCoolerSingleSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FluidCoolerSingleSpeed_Impl::iddObjectType() const {
    return FluidCoolerSingleSpeed::iddObjectType();
  }

  unsigned FluidCoolerSingleSpeed_Impl::inletPort()
  {
    return  OS_FluidCooler_SingleSpeedFields::WaterInletNode;
  }

  unsigned FluidCoolerSingleSpeed_Impl::outletPort()
  {
    return  OS_FluidCooler_SingleSpeedFields::WaterOutletNode;
  }

  std::string FluidCoolerSingleSpeed_Impl::performanceInputMethod() const {
    boost::optional<std::string> value = getString(OS_FluidCooler_SingleSpeedFields::PerformanceInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerSingleSpeed_Impl::designAirFlowRateUfactorTimesAreaValue() const {
    return getDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue,true);
  }

  bool FluidCoolerSingleSpeed_Impl::isDesignAirFlowRateUfactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FluidCoolerSingleSpeed_Impl::nominalCapacity() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_SingleSpeedFields::NominalCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerSingleSpeed_Impl::designEnteringAirTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    boost::optional<double> value = getDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FluidCoolerSingleSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_FluidCooler_SingleSpeedFields::DesignWaterFlowRate,true);
  }

  bool FluidCoolerSingleSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_SingleSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerSingleSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRate,true);
  }

  bool FluidCoolerSingleSpeed_Impl::isDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> FluidCoolerSingleSpeed_Impl::designAirFlowRateFanPower() const {
    return getDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower,true);
  }

  bool FluidCoolerSingleSpeed_Impl::isDesignAirFlowRateFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<Node> FluidCoolerSingleSpeed_Impl::outdoorAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_FluidCooler_SingleSpeedFields::OutdoorAirInletNode);
  }

  bool FluidCoolerSingleSpeed_Impl::setPerformanceInputMethod(std::string performanceInputMethod) {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::PerformanceInputMethod, performanceInputMethod);
    return result;
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignAirFlowRateUfactorTimesAreaValue(boost::optional<double> designAirFlowRateUfactorTimesAreaValue) {
    bool result(false);
    if (designAirFlowRateUfactorTimesAreaValue) {
      result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, designAirFlowRateUfactorTimesAreaValue.get());
    }
    return result;
  }

  void FluidCoolerSingleSpeed_Impl::autosizeDesignAirFlowRateUfactorTimesAreaValue() {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerSingleSpeed_Impl::setNominalCapacity(double nominalCapacity) {
    bool result = setDouble(OS_FluidCooler_SingleSpeedFields::NominalCapacity, nominalCapacity);
    return result;
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
    bool result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, designEnteringWaterTemperature);
    return result;
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
    bool result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, designEnteringAirTemperature);
    return result;
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
    bool result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, designEnteringAirWetbulbTemperature);
    return result;
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    return result;
  }

  void FluidCoolerSingleSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result(false);
    if (designAirFlowRate) {
      result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    }
    return result;
  }

  void FluidCoolerSingleSpeed_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerSingleSpeed_Impl::setDesignAirFlowRateFanPower(boost::optional<double> designAirFlowRateFanPower) {
    bool result(false);
    if (designAirFlowRateFanPower) {
      result = setDouble(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, designAirFlowRateFanPower.get());
    }
    return result;
  }

  void FluidCoolerSingleSpeed_Impl::autosizeDesignAirFlowRateFanPower() {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, "autosize");
    OS_ASSERT(result);
  }

  bool FluidCoolerSingleSpeed_Impl::setOutdoorAirInletNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_FluidCooler_SingleSpeedFields::OutdoorAirInletNode, node.get().handle());
    }
    else {
      resetOutdoorAirInletNode();
      result = true;
    }
    return result;
  }

  void FluidCoolerSingleSpeed_Impl::resetOutdoorAirInletNode() {
    bool result = setString(OS_FluidCooler_SingleSpeedFields::OutdoorAirInletNode, "");
    OS_ASSERT(result);
  }

  bool FluidCoolerSingleSpeed_Impl::addToNode(Node & node)
  {
    if(auto plant = node.plantLoop()) {
      if( plant->supplyComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

FluidCoolerSingleSpeed::FluidCoolerSingleSpeed(const Model& model)
  : StraightComponent(FluidCoolerSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FluidCoolerSingleSpeed_Impl>());

  setPerformanceInputMethod("NominalCapacity");
  autosizeDesignAirFlowRateUfactorTimesAreaValue();
  setNominalCapacity(58601.0);
  setDesignEnteringWaterTemperature(51.67);
  setDesignEnteringAirTemperature(35);
  setDesignEnteringAirWetbulbTemperature(25.6);
  setDesignWaterFlowRate(0.001388);
  setDesignAirFlowRate(9.911);
  autosizeDesignAirFlowRateFanPower();
}

IddObjectType FluidCoolerSingleSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_FluidCooler_SingleSpeed);
}

std::vector<std::string> FluidCoolerSingleSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_FluidCooler_SingleSpeedFields::PerformanceInputMethod);
}

std::string FluidCoolerSingleSpeed::performanceInputMethod() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->performanceInputMethod();
}

boost::optional<double> FluidCoolerSingleSpeed::designAirFlowRateUfactorTimesAreaValue() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designAirFlowRateUfactorTimesAreaValue();
}

bool FluidCoolerSingleSpeed::isDesignAirFlowRateUfactorTimesAreaValueAutosized() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateUfactorTimesAreaValueAutosized();
}

double FluidCoolerSingleSpeed::nominalCapacity() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->nominalCapacity();
}

double FluidCoolerSingleSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designEnteringWaterTemperature();
}

double FluidCoolerSingleSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designEnteringAirTemperature();
}

double FluidCoolerSingleSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

boost::optional<double> FluidCoolerSingleSpeed::designWaterFlowRate() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designWaterFlowRate();
}

bool FluidCoolerSingleSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> FluidCoolerSingleSpeed::designAirFlowRate() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designAirFlowRate();
}

bool FluidCoolerSingleSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> FluidCoolerSingleSpeed::designAirFlowRateFanPower() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->designAirFlowRateFanPower();
}

bool FluidCoolerSingleSpeed::isDesignAirFlowRateFanPowerAutosized() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateFanPowerAutosized();
}

boost::optional<Node> FluidCoolerSingleSpeed::outdoorAirInletNode() const {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->outdoorAirInletNode();
}

bool FluidCoolerSingleSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

bool FluidCoolerSingleSpeed::setDesignAirFlowRateUfactorTimesAreaValue(double designAirFlowRateUfactorTimesAreaValue) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRateUfactorTimesAreaValue(designAirFlowRateUfactorTimesAreaValue);
}

void FluidCoolerSingleSpeed::autosizeDesignAirFlowRateUfactorTimesAreaValue() {
  getImpl<detail::FluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRateUfactorTimesAreaValue();
}

bool FluidCoolerSingleSpeed::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setNominalCapacity(nominalCapacity);
}

bool FluidCoolerSingleSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

bool FluidCoolerSingleSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

bool FluidCoolerSingleSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

bool FluidCoolerSingleSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void FluidCoolerSingleSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::FluidCoolerSingleSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool FluidCoolerSingleSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void FluidCoolerSingleSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::FluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool FluidCoolerSingleSpeed::setDesignAirFlowRateFanPower(double designAirFlowRateFanPower) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRateFanPower(designAirFlowRateFanPower);
}

void FluidCoolerSingleSpeed::autosizeDesignAirFlowRateFanPower() {
  getImpl<detail::FluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRateFanPower();
}

bool FluidCoolerSingleSpeed::setOutdoorAirInletNode(const Node& node) {
  return getImpl<detail::FluidCoolerSingleSpeed_Impl>()->setOutdoorAirInletNode(node);
}

void FluidCoolerSingleSpeed::resetOutdoorAirInletNode() {
  getImpl<detail::FluidCoolerSingleSpeed_Impl>()->resetOutdoorAirInletNode();
}

/// @cond
FluidCoolerSingleSpeed::FluidCoolerSingleSpeed(std::shared_ptr<detail::FluidCoolerSingleSpeed_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

