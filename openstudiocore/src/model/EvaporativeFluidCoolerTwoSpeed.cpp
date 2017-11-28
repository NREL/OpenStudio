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

#include "EvaporativeFluidCoolerTwoSpeed.hpp"
#include "EvaporativeFluidCoolerTwoSpeed_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
// #include "WaterStorageTank.hpp"
// #include "WaterStorageTank_Impl.hpp"
#include "Node.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EvaporativeFluidCooler_TwoSpeed_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EvaporativeFluidCoolerTwoSpeed_Impl::EvaporativeFluidCoolerTwoSpeed_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EvaporativeFluidCoolerTwoSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerTwoSpeed_Impl::EvaporativeFluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EvaporativeFluidCoolerTwoSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerTwoSpeed_Impl::EvaporativeFluidCoolerTwoSpeed_Impl(const EvaporativeFluidCoolerTwoSpeed_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EvaporativeFluidCoolerTwoSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EvaporativeFluidCoolerTwoSpeed_Impl::iddObjectType() const {
    return EvaporativeFluidCoolerTwoSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> EvaporativeFluidCoolerTwoSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("EvaporativeFluidCoolerTwoSpeed","Blowdown Makeup Water Usage"));
    }
    return result;
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::addToNode(Node & node)
  {
    if(auto plant = node.plantLoop()) {
      if( plant->supplyComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode(node);
      }
    }
    return false;
  }

  unsigned EvaporativeFluidCoolerTwoSpeed_Impl::inletPort()
  {
    return OS_EvaporativeFluidCooler_TwoSpeedFields::WaterInletNodeName;
  }

  unsigned EvaporativeFluidCoolerTwoSpeed_Impl::outletPort()
  {
    return OS_EvaporativeFluidCooler_TwoSpeedFields::WaterOutletNodeName;
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::highFanSpeedAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isHighFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::highFanSpeedFanPower() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isHighFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isLowFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedAirFlowRateSizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedFanPower() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isLowFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedFanPowerSizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::designSprayWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignSprayWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string EvaporativeFluidCoolerTwoSpeed_Impl::performanceInputMethod() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::PerformanceInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> EvaporativeFluidCoolerTwoSpeed_Impl::outdoorAirInletNodeName() const {
    return getString(OS_EvaporativeFluidCooler_TwoSpeedFields::OutdoorAirInletNodeName,true);
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::heatRejectionCapacityandNominalCapacitySizingRatio() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::highSpeedStandardDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedStandardDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::lowSpeedStandardDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isLowSpeedStandardDesignCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::lowSpeedStandardCapacitySizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::highFanSpeedUfactorTimesAreaValue() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isHighFanSpeedUfactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedUfactorTimesAreaValue() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isLowFanSpeedUfactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::lowFanSpeedUFactorTimesAreaSizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::highSpeedUserSpecifiedDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedUserSpecifiedDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::lowSpeedUserSpecifiedDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity,true);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::isLowSpeedUserSpecifiedDesignCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::lowSpeedUserSpecifiedDesignCapacitySizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::designEnteringWaterTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::designEnteringAirTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature,true);
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::highSpeedSizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string EvaporativeFluidCoolerTwoSpeed_Impl::evaporationLossMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> EvaporativeFluidCoolerTwoSpeed_Impl::evaporationLossFactor() const {
    return getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossFactor,true);
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DriftLossPercent,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string EvaporativeFluidCoolerTwoSpeed_Impl::blowdownCalculationMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownCalculationMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double EvaporativeFluidCoolerTwoSpeed_Impl::blowdownConcentrationRatio() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownConcentrationRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> EvaporativeFluidCoolerTwoSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

  // boost::optional<WaterStorageTank> EvaporativeFluidCoolerTwoSpeed_Impl::supplyWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_EvaporativeFluidCooler_TwoSpeedFields::SupplyWaterStorageTankName);
  // }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate) {
    bool result(false);
    if (highFanSpeedAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, highFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower) {
    bool result(false);
    if (highFanSpeedFanPower) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower, highFanSpeedFanPower.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedFanPower() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate) {
    bool result(false);
    if (lowFanSpeedAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, lowFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, lowFanSpeedAirFlowRateSizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower) {
    bool result(false);
    if (lowFanSpeedFanPower) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower, lowFanSpeedFanPower.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedFanPower() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower, "autosize");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, lowFanSpeedFanPowerSizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignSprayWaterFlowRate, designSprayWaterFlowRate);
    return result;
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setPerformanceInputMethod(std::string performanceInputMethod) {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::PerformanceInputMethod, performanceInputMethod);
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setOutdoorAirInletNodeName(boost::optional<std::string> outdoorAirInletNodeName) {
    bool result(false);
    if (outdoorAirInletNodeName) {
      result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::OutdoorAirInletNodeName, outdoorAirInletNodeName.get());
    }
    else {
      resetOutdoorAirInletNodeName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetOutdoorAirInletNodeName() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::OutdoorAirInletNodeName, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio, heatRejectionCapacityandNominalCapacitySizingRatio);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighSpeedStandardDesignCapacity(boost::optional<double> highSpeedStandardDesignCapacity) {
    bool result(false);
    if (highSpeedStandardDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedStandardDesignCapacity, highSpeedStandardDesignCapacity.get());
    }
    else {
      resetHighSpeedStandardDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetHighSpeedStandardDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedStandardDesignCapacity, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setLowSpeedStandardDesignCapacity(boost::optional<double> lowSpeedStandardDesignCapacity) {
    bool result(false);
    if (lowSpeedStandardDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, lowSpeedStandardDesignCapacity.get());
    }
    else {
      resetLowSpeedStandardDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetLowSpeedStandardDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeLowSpeedStandardDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, "autosize");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardCapacitySizingFactor, lowSpeedStandardCapacitySizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue) {
    bool result(false);
    if (highFanSpeedUfactorTimesAreaValue) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, highFanSpeedUfactorTimesAreaValue.get());
    }
    else {
      resetHighFanSpeedUfactorTimesAreaValue();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetHighFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeHighFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue) {
    bool result(false);
    if (lowFanSpeedUfactorTimesAreaValue) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, lowFanSpeedUfactorTimesAreaValue.get());
    }
    else {
      resetLowFanSpeedUfactorTimesAreaValue();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetLowFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeLowFanSpeedUfactorTimesAreaValue() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, lowFanSpeedUFactorTimesAreaSizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    else {
      resetDesignWaterFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighSpeedUserSpecifiedDesignCapacity(boost::optional<double> highSpeedUserSpecifiedDesignCapacity) {
    bool result(false);
    if (highSpeedUserSpecifiedDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedUserSpecifiedDesignCapacity, highSpeedUserSpecifiedDesignCapacity.get());
    }
    else {
      resetHighSpeedUserSpecifiedDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetHighSpeedUserSpecifiedDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedUserSpecifiedDesignCapacity, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setLowSpeedUserSpecifiedDesignCapacity(boost::optional<double> lowSpeedUserSpecifiedDesignCapacity) {
    bool result(false);
    if (lowSpeedUserSpecifiedDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, lowSpeedUserSpecifiedDesignCapacity.get());
    }
    else {
      resetLowSpeedUserSpecifiedDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetLowSpeedUserSpecifiedDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::autosizeLowSpeedUserSpecifiedDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, "autosize");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacitySizingFactor, lowSpeedUserSpecifiedDesignCapacitySizingFactor);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature) {
    bool result(false);
    if (designEnteringWaterTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, designEnteringWaterTemperature.get());
    }
    else {
      resetDesignEnteringWaterTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetDesignEnteringWaterTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature) {
    bool result(false);
    if (designEnteringAirTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, designEnteringAirTemperature.get());
    }
    else {
      resetDesignEnteringAirTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetDesignEnteringAirTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature) {
    bool result(false);
    if (designEnteringAirWetbulbTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, designEnteringAirWetbulbTemperature.get());
    }
    else {
      resetDesignEnteringAirWetbulbTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetDesignEnteringAirWetbulbTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, "");
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setHighSpeedSizingFactor(double highSpeedSizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::HighSpeedSizingFactor, highSpeedSizingFactor);
    return result;
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setEvaporationLossMode(std::string evaporationLossMode) {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossMode, evaporationLossMode);
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setEvaporationLossFactor(boost::optional<double> evaporationLossFactor) {
    bool result(false);
    if (evaporationLossFactor) {
      result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossFactor, evaporationLossFactor.get());
    }
    else {
      resetEvaporationLossFactor();
      result = true;
    }
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossFactor, "");
    OS_ASSERT(result);
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::DriftLossPercent, driftLossPercent);
    OS_ASSERT(result);
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownCalculationMode, blowdownCalculationMode);
    return result;
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
    bool result = setDouble(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio);
    return result;
  }

  bool EvaporativeFluidCoolerTwoSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName,
                              "EvaporativeFluidCoolerTwoSpeed",
                              "Blowdown Makeup Water Usage",
                              schedule);
    return result;
  }

  void EvaporativeFluidCoolerTwoSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName, "");
    OS_ASSERT(result);
  }

  // bool EvaporativeFluidCoolerTwoSpeed_Impl::setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_EvaporativeFluidCooler_TwoSpeedFields::SupplyWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetSupplyWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void EvaporativeFluidCoolerTwoSpeed_Impl::resetSupplyWaterStorageTank() {
  //   bool result = setString(OS_EvaporativeFluidCooler_TwoSpeedFields::SupplyWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

} // detail

EvaporativeFluidCoolerTwoSpeed::EvaporativeFluidCoolerTwoSpeed(const Model& model)
  : StraightComponent(EvaporativeFluidCoolerTwoSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>());

  bool ok = true;
  autosizeHighFanSpeedAirFlowRate();
  autosizeHighFanSpeedFanPower();
  autosizeLowFanSpeedAirFlowRate();
  setLowFanSpeedAirFlowRateSizingFactor( 0.5 );
  autosizeLowFanSpeedFanPower();
  setLowFanSpeedFanPowerSizingFactor( 0.5 );
  ok = setDesignSprayWaterFlowRate( 0.002208 );
  OS_ASSERT(ok);
  ok = setPerformanceInputMethod( "UFactorTimesAreaAndDesignWaterFlowRate" );
  OS_ASSERT(ok);
  setHeatRejectionCapacityandNominalCapacitySizingRatio( 1.25 );
  autosizeLowSpeedUserSpecifiedDesignCapacity();
  autosizeLowSpeedStandardDesignCapacity();
  setLowSpeedStandardCapacitySizingFactor( 0.5 );
  autosizeHighFanSpeedUfactorTimesAreaValue();
  autosizeLowFanSpeedUfactorTimesAreaValue();
  setLowFanSpeedUFactorTimesAreaSizingFactor( 0.6 );
  setLowSpeedUserSpecifiedDesignCapacitySizingFactor( 0.5 );
  ok = setHighSpeedSizingFactor( 1.0 );
  OS_ASSERT(ok);
  ok = setEvaporationLossMode( "SaturatedExit" );
  OS_ASSERT(ok);
  setDriftLossPercent( 0.008 );
  ok = setBlowdownCalculationMode( "ConcentrationRatio" );
  OS_ASSERT(ok);
  ok = setBlowdownConcentrationRatio( 3.0 );
  OS_ASSERT(ok);
  autosizeDesignWaterFlowRate();
}

IddObjectType EvaporativeFluidCoolerTwoSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed);
}

std::vector<std::string> EvaporativeFluidCoolerTwoSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_TwoSpeedFields::PerformanceInputMethod);
}

std::vector<std::string> EvaporativeFluidCoolerTwoSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossMode);
}

std::vector<std::string> EvaporativeFluidCoolerTwoSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_TwoSpeedFields::BlowdownCalculationMode);
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::highFanSpeedAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highFanSpeedAirFlowRate();
}

bool EvaporativeFluidCoolerTwoSpeed::isHighFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isHighFanSpeedAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::highFanSpeedFanPower() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highFanSpeedFanPower();
}

bool EvaporativeFluidCoolerTwoSpeed::isHighFanSpeedFanPowerAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isHighFanSpeedFanPowerAutosized();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::lowFanSpeedAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedAirFlowRate();
}

bool EvaporativeFluidCoolerTwoSpeed::isLowFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isLowFanSpeedAirFlowRateAutosized();
}

double EvaporativeFluidCoolerTwoSpeed::lowFanSpeedAirFlowRateSizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedAirFlowRateSizingFactor();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::lowFanSpeedFanPower() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedFanPower();
}

bool EvaporativeFluidCoolerTwoSpeed::isLowFanSpeedFanPowerAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isLowFanSpeedFanPowerAutosized();
}

double EvaporativeFluidCoolerTwoSpeed::lowFanSpeedFanPowerSizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedFanPowerSizingFactor();
}

double EvaporativeFluidCoolerTwoSpeed::designSprayWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->designSprayWaterFlowRate();
}

std::string EvaporativeFluidCoolerTwoSpeed::performanceInputMethod() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->performanceInputMethod();
}

boost::optional<std::string> EvaporativeFluidCoolerTwoSpeed::outdoorAirInletNodeName() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->outdoorAirInletNodeName();
}

double EvaporativeFluidCoolerTwoSpeed::heatRejectionCapacityandNominalCapacitySizingRatio() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->heatRejectionCapacityandNominalCapacitySizingRatio();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::highSpeedStandardDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highSpeedStandardDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::lowSpeedStandardDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowSpeedStandardDesignCapacity();
}

bool EvaporativeFluidCoolerTwoSpeed::isLowSpeedStandardDesignCapacityAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isLowSpeedStandardDesignCapacityAutosized();
}

double EvaporativeFluidCoolerTwoSpeed::lowSpeedStandardCapacitySizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowSpeedStandardCapacitySizingFactor();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::highFanSpeedUfactorTimesAreaValue() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highFanSpeedUfactorTimesAreaValue();
}

bool EvaporativeFluidCoolerTwoSpeed::isHighFanSpeedUfactorTimesAreaValueAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isHighFanSpeedUfactorTimesAreaValueAutosized();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::lowFanSpeedUfactorTimesAreaValue() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedUfactorTimesAreaValue();
}

bool EvaporativeFluidCoolerTwoSpeed::isLowFanSpeedUfactorTimesAreaValueAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isLowFanSpeedUfactorTimesAreaValueAutosized();
}

double EvaporativeFluidCoolerTwoSpeed::lowFanSpeedUFactorTimesAreaSizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowFanSpeedUFactorTimesAreaSizingFactor();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::designWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->designWaterFlowRate();
}

bool EvaporativeFluidCoolerTwoSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::highSpeedUserSpecifiedDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highSpeedUserSpecifiedDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::lowSpeedUserSpecifiedDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowSpeedUserSpecifiedDesignCapacity();
}

bool EvaporativeFluidCoolerTwoSpeed::isLowSpeedUserSpecifiedDesignCapacityAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->isLowSpeedUserSpecifiedDesignCapacityAutosized();
}

double EvaporativeFluidCoolerTwoSpeed::lowSpeedUserSpecifiedDesignCapacitySizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->lowSpeedUserSpecifiedDesignCapacitySizingFactor();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->designEnteringWaterTemperature();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->designEnteringAirTemperature();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

double EvaporativeFluidCoolerTwoSpeed::highSpeedSizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->highSpeedSizingFactor();
}

std::string EvaporativeFluidCoolerTwoSpeed::evaporationLossMode() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->evaporationLossMode();
}

boost::optional<double> EvaporativeFluidCoolerTwoSpeed::evaporationLossFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->evaporationLossFactor();
}

double EvaporativeFluidCoolerTwoSpeed::driftLossPercent() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->driftLossPercent();
}

std::string EvaporativeFluidCoolerTwoSpeed::blowdownCalculationMode() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->blowdownCalculationMode();
}

double EvaporativeFluidCoolerTwoSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->blowdownConcentrationRatio();
}

boost::optional<Schedule> EvaporativeFluidCoolerTwoSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

// boost::optional<WaterStorageTank> EvaporativeFluidCoolerTwoSpeed::supplyWaterStorageTank() const {
//   return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->supplyWaterStorageTank();
// }

bool EvaporativeFluidCoolerTwoSpeed::setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighFanSpeedAirFlowRate(highFanSpeedAirFlowRate);
}

void EvaporativeFluidCoolerTwoSpeed::autosizeHighFanSpeedAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedAirFlowRate();
}

bool EvaporativeFluidCoolerTwoSpeed::setHighFanSpeedFanPower(double highFanSpeedFanPower) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighFanSpeedFanPower(highFanSpeedFanPower);
}

void EvaporativeFluidCoolerTwoSpeed::autosizeHighFanSpeedFanPower() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedFanPower();
}

bool EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRate(lowFanSpeedAirFlowRate);
}

void EvaporativeFluidCoolerTwoSpeed::autosizeLowFanSpeedAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedAirFlowRate();
}

void EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRateSizingFactor(lowFanSpeedAirFlowRateSizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedFanPower(double lowFanSpeedFanPower) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedFanPower(lowFanSpeedFanPower);
}

void EvaporativeFluidCoolerTwoSpeed::autosizeLowFanSpeedFanPower() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedFanPower();
}

void EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedFanPowerSizingFactor(lowFanSpeedFanPowerSizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDesignSprayWaterFlowRate(designSprayWaterFlowRate);
}

bool EvaporativeFluidCoolerTwoSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void EvaporativeFluidCoolerTwoSpeed::setOutdoorAirInletNodeName(std::string outdoorAirInletNodeName) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setOutdoorAirInletNodeName(outdoorAirInletNodeName);
}

void EvaporativeFluidCoolerTwoSpeed::resetOutdoorAirInletNodeName() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetOutdoorAirInletNodeName();
}

void EvaporativeFluidCoolerTwoSpeed::setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHeatRejectionCapacityandNominalCapacitySizingRatio(heatRejectionCapacityandNominalCapacitySizingRatio);
}

bool EvaporativeFluidCoolerTwoSpeed::setHighSpeedStandardDesignCapacity(double highSpeedStandardDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighSpeedStandardDesignCapacity(highSpeedStandardDesignCapacity);
}

void EvaporativeFluidCoolerTwoSpeed::resetHighSpeedStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetHighSpeedStandardDesignCapacity();
}

bool EvaporativeFluidCoolerTwoSpeed::setLowSpeedStandardDesignCapacity(double lowSpeedStandardDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowSpeedStandardDesignCapacity(lowSpeedStandardDesignCapacity);
}

void EvaporativeFluidCoolerTwoSpeed::resetLowSpeedStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetLowSpeedStandardDesignCapacity();
}

void EvaporativeFluidCoolerTwoSpeed::autosizeLowSpeedStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeLowSpeedStandardDesignCapacity();
}

void EvaporativeFluidCoolerTwoSpeed::setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowSpeedStandardCapacitySizingFactor(lowSpeedStandardCapacitySizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighFanSpeedUfactorTimesAreaValue(highFanSpeedUfactorTimesAreaValue);
}

void EvaporativeFluidCoolerTwoSpeed::resetHighFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetHighFanSpeedUfactorTimesAreaValue();
}

void EvaporativeFluidCoolerTwoSpeed::autosizeHighFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeHighFanSpeedUfactorTimesAreaValue();
}

bool EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedUfactorTimesAreaValue(lowFanSpeedUfactorTimesAreaValue);
}

void EvaporativeFluidCoolerTwoSpeed::resetLowFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetLowFanSpeedUfactorTimesAreaValue();
}

void EvaporativeFluidCoolerTwoSpeed::autosizeLowFanSpeedUfactorTimesAreaValue() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeLowFanSpeedUfactorTimesAreaValue();
}

void EvaporativeFluidCoolerTwoSpeed::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowFanSpeedUFactorTimesAreaSizingFactor(lowFanSpeedUFactorTimesAreaSizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void EvaporativeFluidCoolerTwoSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetDesignWaterFlowRate();
}

void EvaporativeFluidCoolerTwoSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool EvaporativeFluidCoolerTwoSpeed::setHighSpeedUserSpecifiedDesignCapacity(double highSpeedUserSpecifiedDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighSpeedUserSpecifiedDesignCapacity(highSpeedUserSpecifiedDesignCapacity);
}

void EvaporativeFluidCoolerTwoSpeed::resetHighSpeedUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetHighSpeedUserSpecifiedDesignCapacity();
}

bool EvaporativeFluidCoolerTwoSpeed::setLowSpeedUserSpecifiedDesignCapacity(double lowSpeedUserSpecifiedDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowSpeedUserSpecifiedDesignCapacity(lowSpeedUserSpecifiedDesignCapacity);
}

void EvaporativeFluidCoolerTwoSpeed::resetLowSpeedUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetLowSpeedUserSpecifiedDesignCapacity();
}

void EvaporativeFluidCoolerTwoSpeed::autosizeLowSpeedUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->autosizeLowSpeedUserSpecifiedDesignCapacity();
}

void EvaporativeFluidCoolerTwoSpeed::setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setLowSpeedUserSpecifiedDesignCapacitySizingFactor(lowSpeedUserSpecifiedDesignCapacitySizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

void EvaporativeFluidCoolerTwoSpeed::resetDesignEnteringWaterTemperature() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetDesignEnteringWaterTemperature();
}

bool EvaporativeFluidCoolerTwoSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

void EvaporativeFluidCoolerTwoSpeed::resetDesignEnteringAirTemperature() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetDesignEnteringAirTemperature();
}

bool EvaporativeFluidCoolerTwoSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

void EvaporativeFluidCoolerTwoSpeed::resetDesignEnteringAirWetbulbTemperature() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetDesignEnteringAirWetbulbTemperature();
}

bool EvaporativeFluidCoolerTwoSpeed::setHighSpeedSizingFactor(double highSpeedSizingFactor) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setHighSpeedSizingFactor(highSpeedSizingFactor);
}

bool EvaporativeFluidCoolerTwoSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void EvaporativeFluidCoolerTwoSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void EvaporativeFluidCoolerTwoSpeed::resetEvaporationLossFactor() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetEvaporationLossFactor();
}

void EvaporativeFluidCoolerTwoSpeed::setDriftLossPercent(double driftLossPercent) {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

bool EvaporativeFluidCoolerTwoSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

bool EvaporativeFluidCoolerTwoSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

bool EvaporativeFluidCoolerTwoSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void EvaporativeFluidCoolerTwoSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

// bool EvaporativeFluidCoolerTwoSpeed::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
// }

// void EvaporativeFluidCoolerTwoSpeed::resetSupplyWaterStorageTank() {
//   getImpl<detail::EvaporativeFluidCoolerTwoSpeed_Impl>()->resetSupplyWaterStorageTank();
// }

/// @cond
EvaporativeFluidCoolerTwoSpeed::EvaporativeFluidCoolerTwoSpeed(std::shared_ptr<detail::EvaporativeFluidCoolerTwoSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

