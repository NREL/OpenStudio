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

#include "CoolingTowerTwoSpeed.hpp"
#include "CoolingTowerTwoSpeed_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "PlantLoop.hpp"
#include "Node.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_CoolingTower_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoolingTowerTwoSpeed_Impl::CoolingTowerTwoSpeed_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoolingTowerTwoSpeed::iddObjectType());
  }

  CoolingTowerTwoSpeed_Impl::CoolingTowerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoolingTowerTwoSpeed::iddObjectType());
  }

  CoolingTowerTwoSpeed_Impl::CoolingTowerTwoSpeed_Impl(const CoolingTowerTwoSpeed_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoolingTowerTwoSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoolingTowerTwoSpeed_Impl::iddObjectType() const {
    return CoolingTowerTwoSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoolingTowerTwoSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CoolingTower_TwoSpeedFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerTwoSpeed","Basin Heater Operating"));
    }
    if (std::find(b,e,OS_CoolingTower_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerTwoSpeed","Blowdown Makeup Water Usage"));
    }
    return result;
  }

  unsigned CoolingTowerTwoSpeed_Impl::inletPort()
  {
    return OS_CoolingTower_TwoSpeedFields::WaterInletNodeName;
  }

  unsigned CoolingTowerTwoSpeed_Impl::outletPort()
  {
    return OS_CoolingTower_TwoSpeedFields::WaterOutletNodeName;
  }

  bool CoolingTowerTwoSpeed_Impl::addToNode(Node & node)
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

  boost::optional<double> CoolingTowerTwoSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::DesignWaterFlowRate,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::highFanSpeedAirFlowRate() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isHighFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::highFanSpeedFanPower() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedFanPower,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isHighFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::highFanSpeedUFactorTimesAreaValue() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isHighFanSpeedUFactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::lowFanSpeedAirFlowRate() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::lowFanSpeedAirFlowRateSizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedAirFlowRateSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::lowFanSpeedFanPower() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPower,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::lowFanSpeedFanPowerSizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedFanPowerSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::lowFanSpeedUFactorTimesAreaValue() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedUFactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::lowFanSpeedUFactorTimesAreaSizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::freeConvectionRegimeAirFlowRate() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionRegimeAirFlowRateDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate);
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionRegimeAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::freeConvectionRegimeAirFlowRateSizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::freeConvectionRegimeUFactorTimesAreaValue() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionRegimeUFactorTimesAreaValueDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue);
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionRegimeUFactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::freeConvectionUFactorTimesAreaValueSizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor);
  }

  std::string CoolingTowerTwoSpeed_Impl::performanceInputMethod() const {
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::PerformanceInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isPerformanceInputMethodDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::PerformanceInputMethod);
  }

  double CoolingTowerTwoSpeed_Impl::heatRejectionCapacityandNominalCapacitySizingRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::highSpeedNominalCapacity() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::HighSpeedNominalCapacity,true);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::lowSpeedNominalCapacity() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isLowSpeedNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::lowSpeedNominalCapacitySizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isLowSpeedNominalCapacitySizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor);
  }

  boost::optional<double> CoolingTowerTwoSpeed_Impl::freeConvectionNominalCapacity() const {
    return getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity,true);
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "AutoSize");
    }
    return result;
  }

  double CoolingTowerTwoSpeed_Impl::freeConvectionNominalCapacitySizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isFreeConvectionNominalCapacitySizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacitySizingFactor);
  }

  double CoolingTowerTwoSpeed_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isBasinHeaterCapacityDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::BasinHeaterCapacity);
  }

  double CoolingTowerTwoSpeed_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isBasinHeaterSetpointTemperatureDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::BasinHeaterSetpointTemperature);
  }

  boost::optional<Schedule> CoolingTowerTwoSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_TwoSpeedFields::BasinHeaterOperatingScheduleName);
  }

  boost::optional<std::string> CoolingTowerTwoSpeed_Impl::evaporationLossMode() const {
    return getString(OS_CoolingTower_TwoSpeedFields::EvaporationLossMode,true);
  }

  double CoolingTowerTwoSpeed_Impl::evaporationLossFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::EvaporationLossFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isEvaporationLossFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::EvaporationLossFactor);
  }

  double CoolingTowerTwoSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::DriftLossPercent,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isDriftLossPercentDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::DriftLossPercent);
  }

  boost::optional<std::string> CoolingTowerTwoSpeed_Impl::blowdownCalculationMode() const {
    return getString(OS_CoolingTower_TwoSpeedFields::BlowdownCalculationMode,true);
  }

  double CoolingTowerTwoSpeed_Impl::blowdownConcentrationRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::BlowdownConcentrationRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isBlowdownConcentrationRatioDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::BlowdownConcentrationRatio);
  }

  boost::optional<Schedule> CoolingTowerTwoSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

  // boost::optional<WaterStorageTank> CoolingTowerTwoSpeed_Impl::supplyWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_CoolingTower_TwoSpeedFields::SupplyWaterStorageTankName);
  // }

  int CoolingTowerTwoSpeed_Impl::numberofCells() const {
    boost::optional<int> value = getInt(OS_CoolingTower_TwoSpeedFields::NumberofCells,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isNumberofCellsDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::NumberofCells);
  }

  std::string CoolingTowerTwoSpeed_Impl::cellControl() const {
    boost::optional<std::string> value = getString(OS_CoolingTower_TwoSpeedFields::CellControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isCellControlDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::CellControl);
  }

  double CoolingTowerTwoSpeed_Impl::cellMinimumWaterFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::CellMinimumWaterFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isCellMinimumWaterFlowRateFractionDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::CellMinimumWaterFlowRateFraction);
  }

  double CoolingTowerTwoSpeed_Impl::cellMaximumWaterFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::CellMaximumWaterFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isCellMaximumWaterFlowRateFractionDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::CellMaximumWaterFlowRateFraction);
  }

  double CoolingTowerTwoSpeed_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_TwoSpeedFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerTwoSpeed_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_TwoSpeedFields::SizingFactor);
  }

  bool CoolingTowerTwoSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    else {
      resetDesignWaterFlowRate();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::DesignWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate) {
    bool result(false);
    if (highFanSpeedAirFlowRate) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate, highFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::autosizeHighFanSpeedAirFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower) {
    bool result(false);
    if (highFanSpeedFanPower) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedFanPower, highFanSpeedFanPower.get());
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::autosizeHighFanSpeedFanPower() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedFanPower, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setHighFanSpeedUFactorTimesAreaValue(boost::optional<double> highFanSpeedUFactorTimesAreaValue) {
    bool result(false);
    if (highFanSpeedUFactorTimesAreaValue) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, highFanSpeedUFactorTimesAreaValue.get());
    }
    else {
      resetHighFanSpeedUFactorTimesAreaValue();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetHighFanSpeedUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeHighFanSpeedUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate) {
    bool result(false);
    if (lowFanSpeedAirFlowRate) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate, lowFanSpeedAirFlowRate.get());
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::autosizeLowFanSpeedAirFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, lowFanSpeedAirFlowRateSizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowFanSpeedAirFlowRateSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower) {
    bool result(false);
    if (lowFanSpeedFanPower) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPower, lowFanSpeedFanPower.get());
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::autosizeLowFanSpeedFanPower() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPower, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, lowFanSpeedFanPowerSizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowFanSpeedFanPowerSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedUFactorTimesAreaValue(boost::optional<double> lowFanSpeedUFactorTimesAreaValue) {
    bool result(false);
    if (lowFanSpeedUFactorTimesAreaValue) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, lowFanSpeedUFactorTimesAreaValue.get());
    }
    else {
      resetLowFanSpeedUFactorTimesAreaValue();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowFanSpeedUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeLowFanSpeedUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, lowFanSpeedUFactorTimesAreaSizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowFanSpeedUFactorTimesAreaSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionRegimeAirFlowRate(boost::optional<double> freeConvectionRegimeAirFlowRate) {
    bool result(false);
    if (freeConvectionRegimeAirFlowRate) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, freeConvectionRegimeAirFlowRate.get());
    }
    else {
      resetFreeConvectionRegimeAirFlowRate();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionRegimeAirFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeFreeConvectionRegimeAirFlowRate() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionRegimeAirFlowRateSizingFactor(double freeConvectionRegimeAirFlowRateSizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor, freeConvectionRegimeAirFlowRateSizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionRegimeAirFlowRateSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionRegimeUFactorTimesAreaValue(boost::optional<double> freeConvectionRegimeUFactorTimesAreaValue) {
    bool result(false);
    if (freeConvectionRegimeUFactorTimesAreaValue) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, freeConvectionRegimeUFactorTimesAreaValue.get());
    }
    else {
      resetFreeConvectionRegimeUFactorTimesAreaValue();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionRegimeUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeFreeConvectionRegimeUFactorTimesAreaValue() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor, freeConvectionUFactorTimesAreaValueSizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionUFactorTimesAreaValueSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setPerformanceInputMethod(std::string performanceInputMethod) {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::PerformanceInputMethod, performanceInputMethod);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetPerformanceInputMethod() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::PerformanceInputMethod, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio, heatRejectionCapacityandNominalCapacitySizingRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::resetHeatRejectionCapacityandNominalCapacitySizingRatio() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setHighSpeedNominalCapacity(boost::optional<double> highSpeedNominalCapacity) {
    bool result(false);
    if (highSpeedNominalCapacity) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::HighSpeedNominalCapacity, highSpeedNominalCapacity.get());
    }
    else {
      resetHighSpeedNominalCapacity();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetHighSpeedNominalCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::HighSpeedNominalCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity) {
    bool result(false);
    if (lowSpeedNominalCapacity) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, lowSpeedNominalCapacity.get());
    }
    else {
      resetLowSpeedNominalCapacity();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowSpeedNominalCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeLowSpeedNominalCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, lowSpeedNominalCapacitySizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetLowSpeedNominalCapacitySizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionNominalCapacity(boost::optional<double> freeConvectionNominalCapacity) {
    bool result(false);
    if (freeConvectionNominalCapacity) {
      result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, freeConvectionNominalCapacity.get());
    }
    else {
      resetFreeConvectionNominalCapacity();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionNominalCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::autosizeFreeConvectionNominalCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, "AutoSize");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacitySizingFactor, freeConvectionNominalCapacitySizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetFreeConvectionNominalCapacitySizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacitySizingFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBasinHeaterCapacity() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BasinHeaterCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBasinHeaterSetpointTemperature() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BasinHeaterSetpointTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_TwoSpeedFields::BasinHeaterOperatingScheduleName,
                              "CoolingTowerTwoSpeed",
                              "Basin Heater Operating",
                              schedule);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setEvaporationLossMode(boost::optional<std::string> evaporationLossMode) {
    bool result(false);
    if (evaporationLossMode) {
      result = setString(OS_CoolingTower_TwoSpeedFields::EvaporationLossMode, evaporationLossMode.get());
    }
    else {
      resetEvaporationLossMode();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetEvaporationLossMode() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::EvaporationLossMode, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::setEvaporationLossFactor(double evaporationLossFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::EvaporationLossFactor, evaporationLossFactor);
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::EvaporationLossFactor, "");
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::DriftLossPercent, driftLossPercent);
    OS_ASSERT(result);
  }

  void CoolingTowerTwoSpeed_Impl::resetDriftLossPercent() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::DriftLossPercent, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode) {
    bool result(false);
    if (blowdownCalculationMode) {
      result = setString(OS_CoolingTower_TwoSpeedFields::BlowdownCalculationMode, blowdownCalculationMode.get());
    }
    else {
      resetBlowdownCalculationMode();
      result = true;
    }
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBlowdownCalculationMode() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BlowdownCalculationMode, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BlowdownConcentrationRatio, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName,
                              "CoolingTowerTwoSpeed",
                              "Blowdown Makeup Water Usage",
                              schedule);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName, "");
    OS_ASSERT(result);
  }

  // bool CoolingTowerTwoSpeed_Impl::setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_CoolingTower_TwoSpeedFields::SupplyWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetSupplyWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void CoolingTowerTwoSpeed_Impl::resetSupplyWaterStorageTank() {
  //   bool result = setString(OS_CoolingTower_TwoSpeedFields::SupplyWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  bool CoolingTowerTwoSpeed_Impl::setNumberofCells(int numberofCells) {
    bool result = setInt(OS_CoolingTower_TwoSpeedFields::NumberofCells, numberofCells);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetNumberofCells() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::NumberofCells, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setCellControl(std::string cellControl) {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::CellControl, cellControl);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetCellControl() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::CellControl, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::CellMinimumWaterFlowRateFraction, cellMinimumWaterFlowRateFraction);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetCellMinimumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::CellMinimumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::CellMaximumWaterFlowRateFraction, cellMaximumWaterFlowRateFraction);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetCellMaximumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::CellMaximumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerTwoSpeed_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_CoolingTower_TwoSpeedFields::SizingFactor, sizingFactor);
    return result;
  }

  void CoolingTowerTwoSpeed_Impl::resetSizingFactor() {
    bool result = setString(OS_CoolingTower_TwoSpeedFields::SizingFactor, "");
    OS_ASSERT(result);
  }

} // detail

CoolingTowerTwoSpeed::CoolingTowerTwoSpeed(const Model& model)
  : StraightComponent(CoolingTowerTwoSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoolingTowerTwoSpeed_Impl>());

  autosizeDesignWaterFlowRate();
  autosizeHighFanSpeedAirFlowRate();
  autosizeHighFanSpeedFanPower();
  autosizeHighFanSpeedUFactorTimesAreaValue();
  autosizeLowFanSpeedAirFlowRate();
  autosizeLowFanSpeedFanPower();
  autosizeLowFanSpeedUFactorTimesAreaValue();
  setSizingFactor(1.0);
}

IddObjectType CoolingTowerTwoSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoolingTower_TwoSpeed);
}

std::vector<std::string> CoolingTowerTwoSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_TwoSpeedFields::PerformanceInputMethod);
}

std::vector<std::string> CoolingTowerTwoSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_TwoSpeedFields::EvaporationLossMode);
}

std::vector<std::string> CoolingTowerTwoSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_TwoSpeedFields::BlowdownCalculationMode);
}

std::vector<std::string> CoolingTowerTwoSpeed::cellControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_TwoSpeedFields::CellControl);
}

boost::optional<double> CoolingTowerTwoSpeed::designWaterFlowRate() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->designWaterFlowRate();
}

bool CoolingTowerTwoSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> CoolingTowerTwoSpeed::highFanSpeedAirFlowRate() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->highFanSpeedAirFlowRate();
}

bool CoolingTowerTwoSpeed::isHighFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isHighFanSpeedAirFlowRateAutosized();
}

boost::optional<double> CoolingTowerTwoSpeed::highFanSpeedFanPower() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->highFanSpeedFanPower();
}

bool CoolingTowerTwoSpeed::isHighFanSpeedFanPowerAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isHighFanSpeedFanPowerAutosized();
}

boost::optional<double> CoolingTowerTwoSpeed::highFanSpeedUFactorTimesAreaValue() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->highFanSpeedUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::isHighFanSpeedUFactorTimesAreaValueAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isHighFanSpeedUFactorTimesAreaValueAutosized();
}

boost::optional<double> CoolingTowerTwoSpeed::lowFanSpeedAirFlowRate() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedAirFlowRate();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedAirFlowRateAutosized();
}

double CoolingTowerTwoSpeed::lowFanSpeedAirFlowRateSizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedAirFlowRateSizingFactor();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedAirFlowRateSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedAirFlowRateSizingFactorDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::lowFanSpeedFanPower() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedFanPower();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedFanPowerAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedFanPowerAutosized();
}

double CoolingTowerTwoSpeed::lowFanSpeedFanPowerSizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedFanPowerSizingFactor();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedFanPowerSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedFanPowerSizingFactorDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::lowFanSpeedUFactorTimesAreaValue() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedUFactorTimesAreaValueAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedUFactorTimesAreaValueAutosized();
}

double CoolingTowerTwoSpeed::lowFanSpeedUFactorTimesAreaSizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowFanSpeedUFactorTimesAreaSizingFactor();
}

bool CoolingTowerTwoSpeed::isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::freeConvectionRegimeAirFlowRate() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionRegimeAirFlowRate();
}

bool CoolingTowerTwoSpeed::isFreeConvectionRegimeAirFlowRateDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionRegimeAirFlowRateDefaulted();
}

bool CoolingTowerTwoSpeed::isFreeConvectionRegimeAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionRegimeAirFlowRateAutosized();
}

double CoolingTowerTwoSpeed::freeConvectionRegimeAirFlowRateSizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionRegimeAirFlowRateSizingFactor();
}

bool CoolingTowerTwoSpeed::isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::freeConvectionRegimeUFactorTimesAreaValue() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionRegimeUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::isFreeConvectionRegimeUFactorTimesAreaValueDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionRegimeUFactorTimesAreaValueDefaulted();
}

bool CoolingTowerTwoSpeed::isFreeConvectionRegimeUFactorTimesAreaValueAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionRegimeUFactorTimesAreaValueAutosized();
}

double CoolingTowerTwoSpeed::freeConvectionUFactorTimesAreaValueSizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionUFactorTimesAreaValueSizingFactor();
}

bool CoolingTowerTwoSpeed::isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted();
}

std::string CoolingTowerTwoSpeed::performanceInputMethod() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->performanceInputMethod();
}

bool CoolingTowerTwoSpeed::isPerformanceInputMethodDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isPerformanceInputMethodDefaulted();
}

double CoolingTowerTwoSpeed::heatRejectionCapacityandNominalCapacitySizingRatio() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->heatRejectionCapacityandNominalCapacitySizingRatio();
}

bool CoolingTowerTwoSpeed::isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::highSpeedNominalCapacity() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->highSpeedNominalCapacity();
}

boost::optional<double> CoolingTowerTwoSpeed::lowSpeedNominalCapacity() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowSpeedNominalCapacity();
}

bool CoolingTowerTwoSpeed::isLowSpeedNominalCapacityAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowSpeedNominalCapacityAutosized();
}

double CoolingTowerTwoSpeed::lowSpeedNominalCapacitySizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->lowSpeedNominalCapacitySizingFactor();
}

bool CoolingTowerTwoSpeed::isLowSpeedNominalCapacitySizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isLowSpeedNominalCapacitySizingFactorDefaulted();
}

boost::optional<double> CoolingTowerTwoSpeed::freeConvectionNominalCapacity() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionNominalCapacity();
}

bool CoolingTowerTwoSpeed::isFreeConvectionNominalCapacityAutosized() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionNominalCapacityAutosized();
}

double CoolingTowerTwoSpeed::freeConvectionNominalCapacitySizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->freeConvectionNominalCapacitySizingFactor();
}

bool CoolingTowerTwoSpeed::isFreeConvectionNominalCapacitySizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isFreeConvectionNominalCapacitySizingFactorDefaulted();
}

double CoolingTowerTwoSpeed::basinHeaterCapacity() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->basinHeaterCapacity();
}

bool CoolingTowerTwoSpeed::isBasinHeaterCapacityDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isBasinHeaterCapacityDefaulted();
}

double CoolingTowerTwoSpeed::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->basinHeaterSetpointTemperature();
}

bool CoolingTowerTwoSpeed::isBasinHeaterSetpointTemperatureDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isBasinHeaterSetpointTemperatureDefaulted();
}

boost::optional<Schedule> CoolingTowerTwoSpeed::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->basinHeaterOperatingSchedule();
}

boost::optional<std::string> CoolingTowerTwoSpeed::evaporationLossMode() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->evaporationLossMode();
}

double CoolingTowerTwoSpeed::evaporationLossFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->evaporationLossFactor();
}

bool CoolingTowerTwoSpeed::isEvaporationLossFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isEvaporationLossFactorDefaulted();
}

double CoolingTowerTwoSpeed::driftLossPercent() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->driftLossPercent();
}

bool CoolingTowerTwoSpeed::isDriftLossPercentDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isDriftLossPercentDefaulted();
}

boost::optional<std::string> CoolingTowerTwoSpeed::blowdownCalculationMode() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->blowdownCalculationMode();
}

double CoolingTowerTwoSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->blowdownConcentrationRatio();
}

bool CoolingTowerTwoSpeed::isBlowdownConcentrationRatioDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isBlowdownConcentrationRatioDefaulted();
}

boost::optional<Schedule> CoolingTowerTwoSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

// boost::optional<WaterStorageTank> CoolingTowerTwoSpeed::supplyWaterStorageTank() const {
//   return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->supplyWaterStorageTank();
// }

int CoolingTowerTwoSpeed::numberofCells() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->numberofCells();
}

bool CoolingTowerTwoSpeed::isNumberofCellsDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isNumberofCellsDefaulted();
}

std::string CoolingTowerTwoSpeed::cellControl() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->cellControl();
}

bool CoolingTowerTwoSpeed::isCellControlDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isCellControlDefaulted();
}

double CoolingTowerTwoSpeed::cellMinimumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->cellMinimumWaterFlowRateFraction();
}

bool CoolingTowerTwoSpeed::isCellMinimumWaterFlowRateFractionDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isCellMinimumWaterFlowRateFractionDefaulted();
}

double CoolingTowerTwoSpeed::cellMaximumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->cellMaximumWaterFlowRateFraction();
}

bool CoolingTowerTwoSpeed::isCellMaximumWaterFlowRateFractionDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isCellMaximumWaterFlowRateFractionDefaulted();
}

double CoolingTowerTwoSpeed::sizingFactor() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->sizingFactor();
}

bool CoolingTowerTwoSpeed::isSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->isSizingFactorDefaulted();
}

bool CoolingTowerTwoSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void CoolingTowerTwoSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetDesignWaterFlowRate();
}

void CoolingTowerTwoSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool CoolingTowerTwoSpeed::setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setHighFanSpeedAirFlowRate(highFanSpeedAirFlowRate);
}

void CoolingTowerTwoSpeed::autosizeHighFanSpeedAirFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeHighFanSpeedAirFlowRate();
}

bool CoolingTowerTwoSpeed::setHighFanSpeedFanPower(double highFanSpeedFanPower) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setHighFanSpeedFanPower(highFanSpeedFanPower);
}

void CoolingTowerTwoSpeed::autosizeHighFanSpeedFanPower() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeHighFanSpeedFanPower();
}

bool CoolingTowerTwoSpeed::setHighFanSpeedUFactorTimesAreaValue(double highFanSpeedUFactorTimesAreaValue) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setHighFanSpeedUFactorTimesAreaValue(highFanSpeedUFactorTimesAreaValue);
}

void CoolingTowerTwoSpeed::resetHighFanSpeedUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetHighFanSpeedUFactorTimesAreaValue();
}

void CoolingTowerTwoSpeed::autosizeHighFanSpeedUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeHighFanSpeedUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRate(lowFanSpeedAirFlowRate);
}

void CoolingTowerTwoSpeed::autosizeLowFanSpeedAirFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeLowFanSpeedAirFlowRate();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedAirFlowRateSizingFactor(lowFanSpeedAirFlowRateSizingFactor);
}

void CoolingTowerTwoSpeed::resetLowFanSpeedAirFlowRateSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowFanSpeedAirFlowRateSizingFactor();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedFanPower(double lowFanSpeedFanPower) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedFanPower(lowFanSpeedFanPower);
}

void CoolingTowerTwoSpeed::autosizeLowFanSpeedFanPower() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeLowFanSpeedFanPower();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedFanPowerSizingFactor(lowFanSpeedFanPowerSizingFactor);
}

void CoolingTowerTwoSpeed::resetLowFanSpeedFanPowerSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowFanSpeedFanPowerSizingFactor();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedUFactorTimesAreaValue(double lowFanSpeedUFactorTimesAreaValue) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedUFactorTimesAreaValue(lowFanSpeedUFactorTimesAreaValue);
}

void CoolingTowerTwoSpeed::resetLowFanSpeedUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowFanSpeedUFactorTimesAreaValue();
}

void CoolingTowerTwoSpeed::autosizeLowFanSpeedUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeLowFanSpeedUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowFanSpeedUFactorTimesAreaSizingFactor(lowFanSpeedUFactorTimesAreaSizingFactor);
}

void CoolingTowerTwoSpeed::resetLowFanSpeedUFactorTimesAreaSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowFanSpeedUFactorTimesAreaSizingFactor();
}

bool CoolingTowerTwoSpeed::setFreeConvectionRegimeAirFlowRate(double freeConvectionRegimeAirFlowRate) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionRegimeAirFlowRate(freeConvectionRegimeAirFlowRate);
}

void CoolingTowerTwoSpeed::resetFreeConvectionRegimeAirFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionRegimeAirFlowRate();
}

void CoolingTowerTwoSpeed::autosizeFreeConvectionRegimeAirFlowRate() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeFreeConvectionRegimeAirFlowRate();
}

bool CoolingTowerTwoSpeed::setFreeConvectionRegimeAirFlowRateSizingFactor(double freeConvectionRegimeAirFlowRateSizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionRegimeAirFlowRateSizingFactor(freeConvectionRegimeAirFlowRateSizingFactor);
}

void CoolingTowerTwoSpeed::resetFreeConvectionRegimeAirFlowRateSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionRegimeAirFlowRateSizingFactor();
}

bool CoolingTowerTwoSpeed::setFreeConvectionRegimeUFactorTimesAreaValue(double freeConvectionRegimeUFactorTimesAreaValue) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionRegimeUFactorTimesAreaValue(freeConvectionRegimeUFactorTimesAreaValue);
}

void CoolingTowerTwoSpeed::resetFreeConvectionRegimeUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionRegimeUFactorTimesAreaValue();
}

void CoolingTowerTwoSpeed::autosizeFreeConvectionRegimeUFactorTimesAreaValue() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeFreeConvectionRegimeUFactorTimesAreaValue();
}

bool CoolingTowerTwoSpeed::setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionUFactorTimesAreaValueSizingFactor(freeConvectionUFactorTimesAreaValueSizingFactor);
}

void CoolingTowerTwoSpeed::resetFreeConvectionUFactorTimesAreaValueSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionUFactorTimesAreaValueSizingFactor();
}

bool CoolingTowerTwoSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void CoolingTowerTwoSpeed::resetPerformanceInputMethod() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetPerformanceInputMethod();
}

void CoolingTowerTwoSpeed::setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio) {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setHeatRejectionCapacityandNominalCapacitySizingRatio(heatRejectionCapacityandNominalCapacitySizingRatio);
}

void CoolingTowerTwoSpeed::resetHeatRejectionCapacityandNominalCapacitySizingRatio() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetHeatRejectionCapacityandNominalCapacitySizingRatio();
}

bool CoolingTowerTwoSpeed::setHighSpeedNominalCapacity(double highSpeedNominalCapacity) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setHighSpeedNominalCapacity(highSpeedNominalCapacity);
}

void CoolingTowerTwoSpeed::resetHighSpeedNominalCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetHighSpeedNominalCapacity();
}

bool CoolingTowerTwoSpeed::setLowSpeedNominalCapacity(double lowSpeedNominalCapacity) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowSpeedNominalCapacity(lowSpeedNominalCapacity);
}

void CoolingTowerTwoSpeed::resetLowSpeedNominalCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowSpeedNominalCapacity();
}

void CoolingTowerTwoSpeed::autosizeLowSpeedNominalCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeLowSpeedNominalCapacity();
}

bool CoolingTowerTwoSpeed::setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setLowSpeedNominalCapacitySizingFactor(lowSpeedNominalCapacitySizingFactor);
}

void CoolingTowerTwoSpeed::resetLowSpeedNominalCapacitySizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetLowSpeedNominalCapacitySizingFactor();
}

bool CoolingTowerTwoSpeed::setFreeConvectionNominalCapacity(double freeConvectionNominalCapacity) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionNominalCapacity(freeConvectionNominalCapacity);
}

void CoolingTowerTwoSpeed::resetFreeConvectionNominalCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionNominalCapacity();
}

void CoolingTowerTwoSpeed::autosizeFreeConvectionNominalCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->autosizeFreeConvectionNominalCapacity();
}

bool CoolingTowerTwoSpeed::setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setFreeConvectionNominalCapacitySizingFactor(freeConvectionNominalCapacitySizingFactor);
}

void CoolingTowerTwoSpeed::resetFreeConvectionNominalCapacitySizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetFreeConvectionNominalCapacitySizingFactor();
}

bool CoolingTowerTwoSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

void CoolingTowerTwoSpeed::resetBasinHeaterCapacity() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBasinHeaterCapacity();
}

bool CoolingTowerTwoSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

void CoolingTowerTwoSpeed::resetBasinHeaterSetpointTemperature() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBasinHeaterSetpointTemperature();
}

bool CoolingTowerTwoSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void CoolingTowerTwoSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

bool CoolingTowerTwoSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void CoolingTowerTwoSpeed::resetEvaporationLossMode() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetEvaporationLossMode();
}

void CoolingTowerTwoSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void CoolingTowerTwoSpeed::resetEvaporationLossFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetEvaporationLossFactor();
}

void CoolingTowerTwoSpeed::setDriftLossPercent(double driftLossPercent) {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

void CoolingTowerTwoSpeed::resetDriftLossPercent() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetDriftLossPercent();
}

bool CoolingTowerTwoSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

void CoolingTowerTwoSpeed::resetBlowdownCalculationMode() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBlowdownCalculationMode();
}

bool CoolingTowerTwoSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void CoolingTowerTwoSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool CoolingTowerTwoSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void CoolingTowerTwoSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

// bool CoolingTowerTwoSpeed::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
// }

// void CoolingTowerTwoSpeed::resetSupplyWaterStorageTank() {
//   getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetSupplyWaterStorageTank();
// }

bool CoolingTowerTwoSpeed::setNumberofCells(int numberofCells) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setNumberofCells(numberofCells);
}

void CoolingTowerTwoSpeed::resetNumberofCells() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetNumberofCells();
}

bool CoolingTowerTwoSpeed::setCellControl(std::string cellControl) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setCellControl(cellControl);
}

void CoolingTowerTwoSpeed::resetCellControl() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetCellControl();
}

bool CoolingTowerTwoSpeed::setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setCellMinimumWaterFlowRateFraction(cellMinimumWaterFlowRateFraction);
}

void CoolingTowerTwoSpeed::resetCellMinimumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetCellMinimumWaterFlowRateFraction();
}

bool CoolingTowerTwoSpeed::setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setCellMaximumWaterFlowRateFraction(cellMaximumWaterFlowRateFraction);
}

void CoolingTowerTwoSpeed::resetCellMaximumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetCellMaximumWaterFlowRateFraction();
}

bool CoolingTowerTwoSpeed::setSizingFactor(double sizingFactor) {
  return getImpl<detail::CoolingTowerTwoSpeed_Impl>()->setSizingFactor(sizingFactor);
}

void CoolingTowerTwoSpeed::resetSizingFactor() {
  getImpl<detail::CoolingTowerTwoSpeed_Impl>()->resetSizingFactor();
}

/// @cond
CoolingTowerTwoSpeed::CoolingTowerTwoSpeed(std::shared_ptr<detail::CoolingTowerTwoSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

