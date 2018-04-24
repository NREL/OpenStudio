/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "EvaporativeFluidCoolerSingleSpeed.hpp"
#include "EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EvaporativeFluidCoolerSingleSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EvaporativeFluidCoolerSingleSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const EvaporativeFluidCoolerSingleSpeed_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EvaporativeFluidCoolerSingleSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      "Cooling Tower Fan Electric Power",
      "Cooling Tower Fan Electric Energy",
      "Cooling Tower Heat Transfer Rate",
      "Cooling Tower Inlet Temperature",
      "Cooling Tower Outlet Temperature",
      "Cooling Tower Mass Flow Rate",
      "Cooling Tower Bypass Fraction",
      "Cooling Tower Make Up Water Volume Flow Rate",
      "Cooling Tower Make Up Water Volume",
      "Cooling Tower Water Evaporation Volume Flow Rate",
      "Cooling Tower Water Evaporation Volume",
      "Cooling Tower Water Drift Volume Flow Rate",
      "Evaporative Fluid Cooler Water Drift",
      "Cooling Tower Water Blowdown Volume Flow Rate",
      "Cooling Tower Water Blowdown Volume",
      "Cooling Tower Make Up Mains Water Volume"
      // If Supply Water Storage Tank Name is specified:
      // TODO: not implemented for now
      //"Cooling Tower Storage Tank Water Volume Flow Rate",
      //"Cooling Tower Storage Tank Water Volume",
      //"Cooling Tower Starved Storage Tank Water Volume Flow Rate",
      //"Cooling Tower Starved Storage Tank Water Volume"
    };
    return result;
  }

  IddObjectType EvaporativeFluidCoolerSingleSpeed_Impl::iddObjectType() const {
    return EvaporativeFluidCoolerSingleSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> EvaporativeFluidCoolerSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("EvaporativeFluidCoolerSingleSpeed","Blowdown Makeup Water Usage"));
    }
    return result;
  }

    unsigned EvaporativeFluidCoolerSingleSpeed_Impl::inletPort() const
  {
    return OS_EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName;
  }

  unsigned EvaporativeFluidCoolerSingleSpeed_Impl::outletPort() const
  {
    return OS_EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::addToNode(Node& node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::fanPoweratDesignAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isFanPoweratDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::designSprayWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethod() const {
    return getString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::standardDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::ufactorTimesAreaValueatDesignAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::userSpecifiedDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,true);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::capacityControl() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isCapacityControlDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isEvaporationLossModeDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossFactor() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor,true);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDriftLossPercentDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::blowdownCalculationMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isBlowdownCalculationModeDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::blowdownConcentrationRatio() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isBlowdownConcentrationRatioDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio);
  }

  boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result(false);
    if (designAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate) {
    bool result(false);
    if (fanPoweratDesignAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, fanPoweratDesignAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeFanPoweratDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate, designSprayWaterFlowRate);
    return result;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setPerformanceInputMethod(boost::optional<std::string> performanceInputMethod) {
    bool result(false);
    if (performanceInputMethod) {
      result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod, performanceInputMethod.get());
    }
    else {
      resetPerformanceInputMethod();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetPerformanceInputMethod() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setStandardDesignCapacity(boost::optional<double> standardDesignCapacity) {
    bool result(false);
    if (standardDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity, standardDesignCapacity.get());
    }
    else {
      resetStandardDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetStandardDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setUfactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate) {
    bool result(false);
    if (ufactorTimesAreaValueatDesignAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, ufactorTimesAreaValueatDesignAirFlowRate.get());
    }
    else {
      resetUfactorTimesAreaValueatDesignAirFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetUfactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeUfactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    else {
      resetDesignWaterFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setUserSpecifiedDesignCapacity(boost::optional<double> userSpecifiedDesignCapacity) {
    bool result(false);
    if (userSpecifiedDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity, userSpecifiedDesignCapacity.get());
    }
    else {
      resetUserSpecifiedDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetUserSpecifiedDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature) {
    bool result(false);
    if (designEnteringWaterTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, designEnteringWaterTemperature.get());
    }
    else {
      resetDesignEnteringWaterTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringWaterTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature) {
    bool result(false);
    if (designEnteringAirTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, designEnteringAirTemperature.get());
    }
    else {
      resetDesignEnteringAirTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringAirTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature) {
    bool result(false);
    if (designEnteringAirWetbulbTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, designEnteringAirWetbulbTemperature.get());
    }
    else {
      resetDesignEnteringAirWetbulbTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringAirWetbulbTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setCapacityControl(std::string capacityControl) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl, capacityControl);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetCapacityControl() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor, sizingFactor);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetSizingFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setEvaporationLossMode(std::string evaporationLossMode) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode, evaporationLossMode);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetEvaporationLossMode() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setEvaporationLossFactor(boost::optional<double> evaporationLossFactor) {
    bool result(false);
    if (evaporationLossFactor) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, evaporationLossFactor.get());
    }
    else {
      resetEvaporationLossFactor();
      result = true;
    }
    OS_ASSERT(result);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, driftLossPercent);
    OS_ASSERT(result);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDriftLossPercent() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode, blowdownCalculationMode);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownCalculationMode() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,
                              "EvaporativeFluidCoolerSingleSpeed",
                              "Blowdown Makeup Water Usage",
                              schedule);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethodValues() const {
    return EvaporativeFluidCoolerSingleSpeed::performanceInputMethodValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::capacityControlValues() const {
    return EvaporativeFluidCoolerSingleSpeed::capacityControlValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::evaporationLossModeValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownCalculationModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::blowdownCalculationModeValues();
  }

  boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = blowdownMakeupWaterUsageSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setBlowdownMakeupWaterUsageSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetBlowdownMakeupWaterUsageSchedule();
    }
    return true;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::autosizedDesignAirFlowRate() const {
    return getAutosizedValue("Design Air Flow Rate", "m3/s");
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::autosizedFanPoweratDesignAirFlowRate() const {
    return getAutosizedValue("Fan Power at Design Air Flow Rate", "W");
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::autosizedUfactorTimesAreaValueatDesignAirFlowRate() const {
    return getAutosizedValue("U-Factor Times Area Value at Design Air Flow Rate", "W/C");
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::autosizedDesignWaterFlowRate() const {
    return getAutosizedValue("Design Water Flow Rate", "m3/s");
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosize() {
    autosizeDesignAirFlowRate();
    autosizeFanPoweratDesignAirFlowRate();
    autosizeUfactorTimesAreaValueatDesignAirFlowRate();
    autosizeDesignWaterFlowRate();
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignAirFlowRate();
    if (val) {
      setDesignAirFlowRate(val.get());
    }

    val = autosizedFanPoweratDesignAirFlowRate();
    if (val) {
      setFanPoweratDesignAirFlowRate(val.get());
    }

    val = autosizedUfactorTimesAreaValueatDesignAirFlowRate();
    if (val) {
      setUfactorTimesAreaValueatDesignAirFlowRate(val.get());
    }

    val = autosizedDesignWaterFlowRate();
    if (val) {
      setDesignWaterFlowRate(val.get());
    }

  }

} // detail

EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(const Model& model)
  : StraightComponent(EvaporativeFluidCoolerSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>());

  autosizeDesignAirFlowRate();
  autosizeFanPoweratDesignAirFlowRate();
  setDesignSprayWaterFlowRate(0.03);
  setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");
  resetStandardDesignCapacity();
  autosizeUfactorTimesAreaValueatDesignAirFlowRate();
  autosizeDesignWaterFlowRate();
  resetUserSpecifiedDesignCapacity();
  resetDesignEnteringWaterTemperature();
  resetDesignEnteringAirTemperature();
  resetDesignEnteringAirWetbulbTemperature();
  setCapacityControl("FanCycling");
  setSizingFactor(1.0);
  setEvaporationLossMode("SaturatedExit");
  setDriftLossPercent(0.008);
  setBlowdownCalculationMode("ConcentrationRatio");
  setBlowdownConcentrationRatio(3.0);
  resetBlowdownMakeupWaterUsageSchedule();
  setString(OS_EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName,"");
}

IddObjectType EvaporativeFluidCoolerSingleSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::capacityControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode);
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::fanPoweratDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->fanPoweratDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isFanPoweratDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isFanPoweratDesignAirFlowRateAutosized();
}

double EvaporativeFluidCoolerSingleSpeed::designSprayWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designSprayWaterFlowRate();
}

boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed::performanceInputMethod() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->performanceInputMethod();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::standardDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->standardDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::ufactorTimesAreaValueatDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->ufactorTimesAreaValueatDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isUfactorTimesAreaValueatDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designWaterFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::userSpecifiedDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->userSpecifiedDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringWaterTemperature();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirTemperature();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

std::string EvaporativeFluidCoolerSingleSpeed::capacityControl() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->capacityControl();
}

bool EvaporativeFluidCoolerSingleSpeed::isCapacityControlDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isCapacityControlDefaulted();
}

double EvaporativeFluidCoolerSingleSpeed::sizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->sizingFactor();
}

bool EvaporativeFluidCoolerSingleSpeed::isSizingFactorDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isSizingFactorDefaulted();
}

std::string EvaporativeFluidCoolerSingleSpeed::evaporationLossMode() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->evaporationLossMode();
}

bool EvaporativeFluidCoolerSingleSpeed::isEvaporationLossModeDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isEvaporationLossModeDefaulted();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::evaporationLossFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->evaporationLossFactor();
}

double EvaporativeFluidCoolerSingleSpeed::driftLossPercent() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->driftLossPercent();
}

bool EvaporativeFluidCoolerSingleSpeed::isDriftLossPercentDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDriftLossPercentDefaulted();
}

std::string EvaporativeFluidCoolerSingleSpeed::blowdownCalculationMode() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownCalculationMode();
}

bool EvaporativeFluidCoolerSingleSpeed::isBlowdownCalculationModeDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isBlowdownCalculationModeDefaulted();
}

double EvaporativeFluidCoolerSingleSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownConcentrationRatio();
}

bool EvaporativeFluidCoolerSingleSpeed::isBlowdownConcentrationRatioDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isBlowdownConcentrationRatioDefaulted();
}

boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::autosizeFanPoweratDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeFanPoweratDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignSprayWaterFlowRate(designSprayWaterFlowRate);
}

bool EvaporativeFluidCoolerSingleSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void EvaporativeFluidCoolerSingleSpeed::resetPerformanceInputMethod() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetPerformanceInputMethod();
}

bool EvaporativeFluidCoolerSingleSpeed::setStandardDesignCapacity(double standardDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setStandardDesignCapacity(standardDesignCapacity);
}

void EvaporativeFluidCoolerSingleSpeed::resetStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetStandardDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUfactorTimesAreaValueatDesignAirFlowRate(ufactorTimesAreaValueatDesignAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::resetUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUfactorTimesAreaValueatDesignAirFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeUfactorTimesAreaValueatDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignWaterFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUserSpecifiedDesignCapacity(userSpecifiedDesignCapacity);
}

void EvaporativeFluidCoolerSingleSpeed::resetUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUserSpecifiedDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringWaterTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringWaterTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringAirTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringAirTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringAirWetbulbTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringAirWetbulbTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setCapacityControl(std::string capacityControl) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setCapacityControl(capacityControl);
}

void EvaporativeFluidCoolerSingleSpeed::resetCapacityControl() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetCapacityControl();
}

bool EvaporativeFluidCoolerSingleSpeed::setSizingFactor(double sizingFactor) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setSizingFactor(sizingFactor);
}

void EvaporativeFluidCoolerSingleSpeed::resetSizingFactor() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetSizingFactor();
}

bool EvaporativeFluidCoolerSingleSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void EvaporativeFluidCoolerSingleSpeed::resetEvaporationLossMode() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetEvaporationLossMode();
}

bool EvaporativeFluidCoolerSingleSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void EvaporativeFluidCoolerSingleSpeed::resetEvaporationLossFactor() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetEvaporationLossFactor();
}

bool EvaporativeFluidCoolerSingleSpeed::setDriftLossPercent(double driftLossPercent) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

void EvaporativeFluidCoolerSingleSpeed::resetDriftLossPercent() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDriftLossPercent();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownCalculationMode() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownCalculationMode();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

/// @cond
EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(std::shared_ptr<detail::EvaporativeFluidCoolerSingleSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed::autosizedDesignAirFlowRate() const {
    return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizedDesignAirFlowRate();
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed::autosizedFanPoweratDesignAirFlowRate() const {
    return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizedFanPoweratDesignAirFlowRate();
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed::autosizedUfactorTimesAreaValueatDesignAirFlowRate() const {
    return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizedUfactorTimesAreaValueatDesignAirFlowRate();
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed::autosizedDesignWaterFlowRate() const {
    return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizedDesignWaterFlowRate();
  }

} // model
} // openstudio
