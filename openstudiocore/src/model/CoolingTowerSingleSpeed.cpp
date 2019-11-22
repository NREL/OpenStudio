/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoolingTowerSingleSpeed.hpp"
#include "CoolingTowerSingleSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "SizingPlant.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_CoolingTower_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoolingTowerSingleSpeed_Impl::CoolingTowerSingleSpeed_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoolingTowerSingleSpeed::iddObjectType());
  }

  CoolingTowerSingleSpeed_Impl::CoolingTowerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoolingTowerSingleSpeed::iddObjectType());
  }

  CoolingTowerSingleSpeed_Impl::CoolingTowerSingleSpeed_Impl(const CoolingTowerSingleSpeed_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoolingTowerSingleSpeed_Impl::outputVariableNames() const
  {
    // Until this changes, static
    static std::vector<std::string> result{

    // Common Variables
    "Cooling Tower Fan Electric Power",
    "Cooling Tower Fan Electric Energy",
    "Cooling Tower Heat Transfer Rate",
    "Cooling Tower Inlet Temperature",
    "Cooling Tower Outlet Temperature",
    "Cooling Tower Mass Flow Rate",
    "Cooling Tower Bypass Fraction",
    "Cooling Tower Fan Cycling Ratio",
    "Cooling Tower Operating Cells Count",

    // When Mains Water is used
    "Cooling Tower Make Up Water Volume Flow Rate",
    "Cooling Tower Make Up Water Volume",
    "Cooling Tower Make Up Mains Water Volume",

    // When storage tank water is used:
    // Supply Water Storage Tank Name isn't implemented in OpenStudio
    // TODO: Revisit if need be
    //"Cooling Tower Make Up Water Volume Flow Rate",
    //"Cooling Tower Make Up Water Volume",
    //"Cooling Tower Storage Tank Water Volume Flow Rate",
    //"Cooling Tower Storage Tank Water Volume",
    //"Cooling Tower Starved Storage Tank Water Volume Flow Rate",
    //"Cooling Tower Starved Storage Tank Water Volume",
    //"Cooling Tower Make Up Mains Water Volume",
    //"Cooling Tower Water Evaporation Volume Flow Rate",
    //"Cooling Tower Water Evaporation Volume",
    //"Cooling Tower Water Drift Volume Flow Rate",
    //"Cooling Tower Water Drift Volume",
    //"Cooling Tower Water Blowdown Volume Flow Rate",
    //"Cooling Tower Water Blowdown Volume",

    // if specified
    // TODO: @DLM: the return type of this method needs to change to std::vector<std::string> in ModelObject
    // if (this->basinHeaterCapacity() > 0) {
      "Cooling Tower Basin Heater Electric Power",
      "Cooling Tower Basin Heater Electric Energy"
    };

    return result;
  }

  IddObjectType CoolingTowerSingleSpeed_Impl::iddObjectType() const {
    return CoolingTowerSingleSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoolingTowerSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerSingleSpeed","Basin Heater Operating"));
    }
    if (std::find(b,e,OS_CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerSingleSpeed","Blowdown Makeup Water Usage"));
    }
    return result;
  }

  unsigned CoolingTowerSingleSpeed_Impl::inletPort() const
  {
    return OS_CoolingTower_SingleSpeedFields::WaterInletNodeName;
  }

  unsigned CoolingTowerSingleSpeed_Impl::outletPort() const
  {
    return OS_CoolingTower_SingleSpeedFields::WaterOutletNodeName;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getDesignWaterFlowRate(bool returnIP) const {
    OptionalDouble value = designWaterFlowRate();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = designAirFlowRate();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::fanPoweratDesignAirFlowRate() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getFanPoweratDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = fanPoweratDesignAirFlowRate();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isFanPoweratDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatDesignAirFlowRate() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getUFactorTimesAreaValueatDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = uFactorTimesAreaValueatDesignAirFlowRate();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isUFactorTimesAreaValueatDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::airFlowRateinFreeConvectionRegime() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getAirFlowRateinFreeConvectionRegime(bool returnIP) const {
    OptionalDouble value = airFlowRateinFreeConvectionRegime();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isAirFlowRateinFreeConvectionRegimeDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime);
  }

  bool CoolingTowerSingleSpeed_Impl::isAirFlowRateinFreeConvectionRegimeAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatFreeConvectionAirFlowRate() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getUFactorTimesAreaValueatFreeConvectionAirFlowRate(bool returnIP) const {
    OptionalDouble value = uFactorTimesAreaValueatFreeConvectionAirFlowRate();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, value, returnIP);
  }

  bool CoolingTowerSingleSpeed_Impl::isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate);
  }

  bool CoolingTowerSingleSpeed_Impl::isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string CoolingTowerSingleSpeed_Impl::performanceInputMethod() const {
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::PerformanceInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isPerformanceInputMethodDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::PerformanceInputMethod);
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::nominalCapacity() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::NominalCapacity,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::NominalCapacity, value, returnIP);
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::freeConvectionCapacity() const {
    return getDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity,true);
  }

  OSOptionalQuantity CoolingTowerSingleSpeed_Impl::getFreeConvectionCapacity(bool returnIP) const {
    OptionalDouble value = freeConvectionCapacity();
    return getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity, value, returnIP);
  }

  double CoolingTowerSingleSpeed_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getBasinHeaterCapacity(bool returnIP) const {
    OptionalDouble value = basinHeaterCapacity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isBasinHeaterCapacityDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity);
  }

  double CoolingTowerSingleSpeed_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getBasinHeaterSetpointTemperature(bool returnIP) const {
    OptionalDouble value = basinHeaterSetpointTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isBasinHeaterSetpointTemperatureDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature);
  }

  boost::optional<Schedule> CoolingTowerSingleSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName);
  }

  boost::optional<std::string> CoolingTowerSingleSpeed_Impl::evaporationLossMode() const {
    return getString(OS_CoolingTower_SingleSpeedFields::EvaporationLossMode,true);
  }

  double CoolingTowerSingleSpeed_Impl::evaporationLossFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getEvaporationLossFactor(bool returnIP) const {
    OptionalDouble value = evaporationLossFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isEvaporationLossFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor);
  }

  double CoolingTowerSingleSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::DriftLossPercent,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getDriftLossPercent(bool returnIP) const {
    OptionalDouble value = driftLossPercent();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::DriftLossPercent, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isDriftLossPercentDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::DriftLossPercent);
  }

  boost::optional<std::string> CoolingTowerSingleSpeed_Impl::blowdownCalculationMode() const {
    return getString(OS_CoolingTower_SingleSpeedFields::BlowdownCalculationMode,true);
  }

  double CoolingTowerSingleSpeed_Impl::blowdownConcentrationRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getBlowdownConcentrationRatio(bool returnIP) const {
    OptionalDouble value = blowdownConcentrationRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isBlowdownConcentrationRatioDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio);
  }

  boost::optional<Schedule> CoolingTowerSingleSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

  std::string CoolingTowerSingleSpeed_Impl::capacityControl() const {
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::CapacityControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isCapacityControlDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::CapacityControl);
  }

  int CoolingTowerSingleSpeed_Impl::numberofCells() const {
    boost::optional<int> value = getInt(OS_CoolingTower_SingleSpeedFields::NumberofCells,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isNumberofCellsDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::NumberofCells);
  }

  std::string CoolingTowerSingleSpeed_Impl::cellControl() const {
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::CellControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isCellControlDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::CellControl);
  }

  double CoolingTowerSingleSpeed_Impl::cellMinimumWaterFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getCellMinimumWaterFlowRateFraction(bool returnIP) const {
    OptionalDouble value = cellMinimumWaterFlowRateFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isCellMinimumWaterFlowRateFractionDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction);
  }

  double CoolingTowerSingleSpeed_Impl::cellMaximumWaterFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getCellMaximumWaterFlowRateFraction(bool returnIP) const {
    OptionalDouble value = cellMaximumWaterFlowRateFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isCellMaximumWaterFlowRateFractionDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction);
  }

  double CoolingTowerSingleSpeed_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_SingleSpeedFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity CoolingTowerSingleSpeed_Impl::getSizingFactor(bool returnIP) const {
    OptionalDouble value = sizingFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_CoolingTower_SingleSpeedFields::SizingFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_SingleSpeedFields::SizingFactor);
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result = false;
    if (designWaterFlowRate) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignWaterFlowRate(const OSOptionalQuantity& designWaterFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (designWaterFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate,designWaterFlowRate.get());
      if (value) {
        result = setDesignWaterFlowRate(value);
      }
    }
    else {
      result = setDesignWaterFlowRate(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result = false;
    if (designAirFlowRate) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignAirFlowRate(const OSOptionalQuantity& designAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (designAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate,designAirFlowRate.get());
      if (value) {
        result = setDesignAirFlowRate(value);
      }
    }
    else {
      result = setDesignAirFlowRate(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::DesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate) {
    bool result = false;
    if (fanPoweratDesignAirFlowRate) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate, fanPoweratDesignAirFlowRate.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setFanPoweratDesignAirFlowRate(const OSOptionalQuantity& fanPoweratDesignAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (fanPoweratDesignAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate,fanPoweratDesignAirFlowRate.get());
      if (value) {
        result = setFanPoweratDesignAirFlowRate(value);
      }
    }
    else {
      result = setFanPoweratDesignAirFlowRate(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::autosizeFanPoweratDesignAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::FanPoweratDesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setUFactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate) {
    bool result = false;
    if (uFactorTimesAreaValueatDesignAirFlowRate) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, uFactorTimesAreaValueatDesignAirFlowRate.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setUFactorTimesAreaValueatDesignAirFlowRate(const OSOptionalQuantity& uFactorTimesAreaValueatDesignAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (uFactorTimesAreaValueatDesignAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate,uFactorTimesAreaValueatDesignAirFlowRate.get());
      if (value) {
        result = setUFactorTimesAreaValueatDesignAirFlowRate(value);
      }
    }
    else {
      result = setUFactorTimesAreaValueatDesignAirFlowRate(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetUFactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, "");
    OS_ASSERT(result);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeUFactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setAirFlowRateinFreeConvectionRegime(boost::optional<double> airFlowRateinFreeConvectionRegime) {
    bool result = false;
    if (airFlowRateinFreeConvectionRegime) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, airFlowRateinFreeConvectionRegime.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setAirFlowRateinFreeConvectionRegime(const OSOptionalQuantity& airFlowRateinFreeConvectionRegime) {
    bool result(false);
    OptionalDouble value;
    if (airFlowRateinFreeConvectionRegime.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime,airFlowRateinFreeConvectionRegime.get());
      if (value) {
        result = setAirFlowRateinFreeConvectionRegime(value);
      }
    }
    else {
      result = setAirFlowRateinFreeConvectionRegime(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetAirFlowRateinFreeConvectionRegime() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, "");
    OS_ASSERT(result);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeAirFlowRateinFreeConvectionRegime() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::AirFlowRateinFreeConvectionRegime, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setUFactorTimesAreaValueatFreeConvectionAirFlowRate(boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate) {
    bool result = false;
    if (uFactorTimesAreaValueatFreeConvectionAirFlowRate) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, uFactorTimesAreaValueatFreeConvectionAirFlowRate.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setUFactorTimesAreaValueatFreeConvectionAirFlowRate(const OSOptionalQuantity& uFactorTimesAreaValueatFreeConvectionAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (uFactorTimesAreaValueatFreeConvectionAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate,uFactorTimesAreaValueatFreeConvectionAirFlowRate.get());
      if (value) {
        result = setUFactorTimesAreaValueatFreeConvectionAirFlowRate(value);
      }
    }
    else {
      result = setUFactorTimesAreaValueatFreeConvectionAirFlowRate(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetUFactorTimesAreaValueatFreeConvectionAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, "");
    OS_ASSERT(result);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::UFactorTimesAreaValueatFreeConvectionAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setPerformanceInputMethod(std::string performanceInputMethod) {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::PerformanceInputMethod, performanceInputMethod);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetPerformanceInputMethod() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::PerformanceInputMethod, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result = false;
    if (nominalCapacity) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::NominalCapacity, nominalCapacity.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::NominalCapacity, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setNominalCapacity(const OSOptionalQuantity& nominalCapacity) {
    bool result(false);
    OptionalDouble value;
    if (nominalCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::NominalCapacity,nominalCapacity.get());
      if (value) {
        result = setNominalCapacity(value);
      }
    }
    else {
      result = setNominalCapacity(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetNominalCapacity() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::NominalCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setFreeConvectionCapacity(boost::optional<double> freeConvectionCapacity) {
    bool result = false;
    if (freeConvectionCapacity) {
      result = setDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity, freeConvectionCapacity.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity, "");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setFreeConvectionCapacity(const OSOptionalQuantity& freeConvectionCapacity) {
    bool result(false);
    OptionalDouble value;
    if (freeConvectionCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity,freeConvectionCapacity.get());
      if (value) {
        result = setFreeConvectionCapacity(value);
      }
    }
    else {
      result = setFreeConvectionCapacity(value);
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetFreeConvectionCapacity() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::FreeConvectionCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterCapacity(const Quantity& basinHeaterCapacity) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity,basinHeaterCapacity);
    if (!value) {
      return false;
    }
    return setBasinHeaterCapacity(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetBasinHeaterCapacity() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BasinHeaterCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterSetpointTemperature(const Quantity& basinHeaterSetpointTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature,basinHeaterSetpointTemperature);
    if (!value) {
      return false;
    }
    return setBasinHeaterSetpointTemperature(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetBasinHeaterSetpointTemperature() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName,
                              "CoolingTowerSingleSpeed",
                              "Basin Heater Operating",
                              schedule);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setEvaporationLossMode(boost::optional<std::string> evaporationLossMode) {
    bool result = false;
    if (evaporationLossMode) {
      result = setString(OS_CoolingTower_SingleSpeedFields::EvaporationLossMode, evaporationLossMode.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::EvaporationLossMode, "");
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetEvaporationLossMode() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::EvaporationLossMode, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setEvaporationLossFactor(double evaporationLossFactor) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor, evaporationLossFactor);
    OS_ASSERT(result);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setEvaporationLossFactor(const Quantity& evaporationLossFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor,evaporationLossFactor);
    if (!value) {
      return false;
    }
    setEvaporationLossFactor(value.get());

    return true;
  }

  void CoolingTowerSingleSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::EvaporationLossFactor, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::DriftLossPercent, driftLossPercent);
    OS_ASSERT(result);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setDriftLossPercent(const Quantity& driftLossPercent) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::DriftLossPercent,driftLossPercent);
    if (!value) {
      return false;
    }

    setDriftLossPercent(value.get());

    return true;
  }

  void CoolingTowerSingleSpeed_Impl::resetDriftLossPercent() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::DriftLossPercent, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode) {
    bool result = false;
    if (blowdownCalculationMode) {
      result = setString(OS_CoolingTower_SingleSpeedFields::BlowdownCalculationMode, blowdownCalculationMode.get());
    } else {
      result = setString(OS_CoolingTower_SingleSpeedFields::BlowdownCalculationMode, "");
    }
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetBlowdownCalculationMode() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BlowdownCalculationMode, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio,blowdownConcentrationRatio);
    if (!value) {
      return false;
    }
    return setBlowdownConcentrationRatio(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,
                              "CoolingTowerSingleSpeed",
                              "Blowdown Makeup Water Usage",
                              schedule);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setCapacityControl(std::string capacityControl) {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CapacityControl, capacityControl);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetCapacityControl() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CapacityControl, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setNumberofCells(int numberofCells) {
    bool result = setInt(OS_CoolingTower_SingleSpeedFields::NumberofCells, numberofCells);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetNumberofCells() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::NumberofCells, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setCellControl(std::string cellControl) {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CellControl, cellControl);
    return result;
  }

  void CoolingTowerSingleSpeed_Impl::resetCellControl() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CellControl, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction, cellMinimumWaterFlowRateFraction);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setCellMinimumWaterFlowRateFraction(const Quantity& cellMinimumWaterFlowRateFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction,cellMinimumWaterFlowRateFraction);
    if (!value) {
      return false;
    }
    return setCellMinimumWaterFlowRateFraction(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetCellMinimumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction, cellMaximumWaterFlowRateFraction);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setCellMaximumWaterFlowRateFraction(const Quantity& cellMaximumWaterFlowRateFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction,cellMaximumWaterFlowRateFraction);
    if (!value) {
      return false;
    }
    return setCellMaximumWaterFlowRateFraction(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetCellMaximumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerSingleSpeed_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_CoolingTower_SingleSpeedFields::SizingFactor, sizingFactor);
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setSizingFactor(const Quantity& sizingFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_CoolingTower_SingleSpeedFields::SizingFactor,sizingFactor);
    if (!value) {
      return false;
    }
    return setSizingFactor(value.get());
  }

  void CoolingTowerSingleSpeed_Impl::resetSizingFactor() {
    bool result = setString(OS_CoolingTower_SingleSpeedFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::designWaterFlowRate_SI() const {
    return getDesignWaterFlowRate(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::designWaterFlowRate_IP() const {
    return getDesignWaterFlowRate(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::designAirFlowRate_SI() const {
    return getDesignAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::designAirFlowRate_IP() const {
    return getDesignAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::fanPoweratDesignAirFlowRate_SI() const {
    return getFanPoweratDesignAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::fanPoweratDesignAirFlowRate_IP() const {
    return getFanPoweratDesignAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatDesignAirFlowRate_SI() const {
    return getUFactorTimesAreaValueatDesignAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatDesignAirFlowRate_IP() const {
    return getUFactorTimesAreaValueatDesignAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::airFlowRateinFreeConvectionRegime_SI() const {
    return getAirFlowRateinFreeConvectionRegime(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::airFlowRateinFreeConvectionRegime_IP() const {
    return getAirFlowRateinFreeConvectionRegime(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatFreeConvectionAirFlowRate_SI() const {
    return getUFactorTimesAreaValueatFreeConvectionAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::uFactorTimesAreaValueatFreeConvectionAirFlowRate_IP() const {
    return getUFactorTimesAreaValueatFreeConvectionAirFlowRate(true);
  }

  std::vector<std::string> CoolingTowerSingleSpeed_Impl::performanceInputMethodValues() const {
    return CoolingTowerSingleSpeed::performanceInputMethodValues();
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::nominalCapacity_SI() const {
    return getNominalCapacity(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::nominalCapacity_IP() const {
    return getNominalCapacity(true);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::freeConvectionCapacity_SI() const {
    return getFreeConvectionCapacity(false);
  }

  openstudio::OSOptionalQuantity CoolingTowerSingleSpeed_Impl::freeConvectionCapacity_IP() const {
    return getFreeConvectionCapacity(true);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::basinHeaterCapacity_SI() const {
    return getBasinHeaterCapacity(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::basinHeaterCapacity_IP() const {
    return getBasinHeaterCapacity(true);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::basinHeaterSetpointTemperature_SI() const {
    return getBasinHeaterSetpointTemperature(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::basinHeaterSetpointTemperature_IP() const {
    return getBasinHeaterSetpointTemperature(true);
  }

  std::vector<std::string> CoolingTowerSingleSpeed_Impl::evaporationLossModeValues() const {
    return CoolingTowerSingleSpeed::evaporationLossModeValues();
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::evaporationLossFactor_SI() const {
    return getEvaporationLossFactor(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::evaporationLossFactor_IP() const {
    return getEvaporationLossFactor(true);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::driftLossPercent_SI() const {
    return getDriftLossPercent(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::driftLossPercent_IP() const {
    return getDriftLossPercent(true);
  }

  std::vector<std::string> CoolingTowerSingleSpeed_Impl::blowdownCalculationModeValues() const {
    return CoolingTowerSingleSpeed::blowdownCalculationModeValues();
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::blowdownConcentrationRatio_SI() const {
    return getBlowdownConcentrationRatio(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::blowdownConcentrationRatio_IP() const {
    return getBlowdownConcentrationRatio(true);
  }

  std::vector<std::string> CoolingTowerSingleSpeed_Impl::capacityControlValues() const {
    return CoolingTowerSingleSpeed::capacityControlValues();
  }

  std::vector<std::string> CoolingTowerSingleSpeed_Impl::cellControlValues() const {
    return CoolingTowerSingleSpeed::cellControlValues();
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::cellMinimumWaterFlowRateFraction_SI() const {
    return getCellMinimumWaterFlowRateFraction(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::cellMinimumWaterFlowRateFraction_IP() const {
    return getCellMinimumWaterFlowRateFraction(true);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::cellMaximumWaterFlowRateFraction_SI() const {
    return getCellMaximumWaterFlowRateFraction(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::cellMaximumWaterFlowRateFraction_IP() const {
    return getCellMaximumWaterFlowRateFraction(true);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::sizingFactor_SI() const {
    return getSizingFactor(false);
  }

  openstudio::Quantity CoolingTowerSingleSpeed_Impl::sizingFactor_IP() const {
    return getSizingFactor(true);
  }

  boost::optional<ModelObject> CoolingTowerSingleSpeed_Impl::basinHeaterOperatingScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = basinHeaterOperatingSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> CoolingTowerSingleSpeed_Impl::blowdownMakeupWaterUsageScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = blowdownMakeupWaterUsageSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setBasinHeaterOperatingSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetBasinHeaterOperatingSchedule();
    }
    return true;
  }

  bool CoolingTowerSingleSpeed_Impl::setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool CoolingTowerSingleSpeed_Impl::addToNode(Node & node)
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

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedDesignWaterFlowRate() const {
    return getAutosizedValue("Design Water Flow Rate", "m3/s");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedDesignAirFlowRate() const {
    return getAutosizedValue("Design Air Flow Rate", "m3/s");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedFanPoweratDesignAirFlowRate() const {
    return getAutosizedValue("Fan Power at Design Air Flow Rate", "W");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedUFactorTimesAreaValueatDesignAirFlowRate() const {
    return getAutosizedValue("U-Factor Times Area Value at Design Air Flow Rate", "W/C");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedAirFlowRateinFreeConvectionRegime() const {
    return getAutosizedValue("Free Convection Regime Air Flow Rate", "m3/s");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate() const {
    return getAutosizedValue("Free Convection U-Factor Times Area Value", "W/K");
  }


  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedDesignApproachTemperature() const {
    boost::optional<double> result;

    // Calculate Approach as design wet bulb temp - EWT (from SizingPlant)
    if (boost::optional<PlantLoop> pl = this->plantLoop()) {
      SizingPlant sz = pl->sizingPlant();
      double EWT = sz.designLoopExitTemperature() - sz.loopDesignTemperatureDifference();
      result = EWT - designInletAirWetBulbTemperature();
    }
    return result;
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::autosizedDesignRangeTemperature() const {
    boost::optional<double> result;

    // Return the SizingPlant DeltaT
    if (boost::optional<PlantLoop> pl = this->plantLoop()) {
      SizingPlant sz = pl->sizingPlant();
      result = sz.loopDesignTemperatureDifference();
    }
    return result;
  }


  void CoolingTowerSingleSpeed_Impl::autosize() {
    autosizeDesignWaterFlowRate();
    autosizeDesignAirFlowRate();
    autosizeFanPoweratDesignAirFlowRate();
    autosizeUFactorTimesAreaValueatDesignAirFlowRate();
    autosizeAirFlowRateinFreeConvectionRegime();
    autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();
    autosizeDesignRangeTemperature();
    autosizeDesignApproachTemperature();
  }

  void CoolingTowerSingleSpeed_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignWaterFlowRate();
    if (val) {
      setDesignWaterFlowRate(val.get());
    }

    val = autosizedDesignAirFlowRate();
    if (val) {
      setDesignAirFlowRate(val.get());
    }

    val = autosizedFanPoweratDesignAirFlowRate();
    if (val) {
      setFanPoweratDesignAirFlowRate(val.get());
    }

    val = autosizedUFactorTimesAreaValueatDesignAirFlowRate();
    if (val) {
      setUFactorTimesAreaValueatDesignAirFlowRate(val.get());
    }

    val = autosizedAirFlowRateinFreeConvectionRegime();
    if (val) {
      setAirFlowRateinFreeConvectionRegime(val.get());
    }

    val = autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate();
    if (val) {
      setUFactorTimesAreaValueatFreeConvectionAirFlowRate(val.get());
    }

    val = autosizedDesignApproachTemperature();
    if (val) {
      setDesignApproachTemperature(val.get());
    }

    val = autosizedDesignRangeTemperature();
    if (val) {
      setDesignRangeTemperature(val.get());
    }
  }

  double CoolingTowerSingleSpeed_Impl::freeConvectionAirFlowRateSizingFactor() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionAirFlowRateSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setFreeConvectionAirFlowRateSizingFactor(double freeConvectionAirFlowRateSizingFactor) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionAirFlowRateSizingFactor,freeConvectionAirFlowRateSizingFactor);
  }

  double CoolingTowerSingleSpeed_Impl::freeConvectionUFactorTimesAreaValueSizingFactor() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor,freeConvectionUFactorTimesAreaValueSizingFactor);
  }

  double CoolingTowerSingleSpeed_Impl::heatRejectionCapacityAndNominalCapacitySizingRatio() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setHeatRejectionCapacityAndNominalCapacitySizingRatio(double heatRejectionCapacityAndNominalCapacitySizingRatio) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,heatRejectionCapacityAndNominalCapacitySizingRatio);
  }

  double CoolingTowerSingleSpeed_Impl::freeConvectionNominalCapacitySizingFactor() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionNominalCapacitySizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::FreeConvectionNominalCapacitySizingFactor,freeConvectionNominalCapacitySizingFactor);
  }

  double CoolingTowerSingleSpeed_Impl::designInletAirDryBulbTemperature() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::DesignInletAirDryBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignInletAirDryBulbTemperature(double designInletAirDryBulbTemperature) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::DesignInletAirDryBulbTemperature,designInletAirDryBulbTemperature);
  }

  double CoolingTowerSingleSpeed_Impl::designInletAirWetBulbTemperature() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::DesignInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::DesignInletAirWetBulbTemperature,designInletAirWetBulbTemperature);
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::designApproachTemperature() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::DesignApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isDesignApproachTemperatureAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::DesignApproachTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignApproachTemperature(double designApproachTemperature) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::DesignApproachTemperature,designApproachTemperature);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeDesignApproachTemperature() {
    setString(OS_CoolingTower_SingleSpeedFields::DesignApproachTemperature,"autosize");
  }

  boost::optional<double> CoolingTowerSingleSpeed_Impl::designRangeTemperature() const {
    auto value = getDouble(OS_CoolingTower_SingleSpeedFields::DesignRangeTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::isDesignRangeTemperatureAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_SingleSpeedFields::DesignRangeTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoolingTowerSingleSpeed_Impl::setDesignRangeTemperature(double designRangeTemperature) {
    return setDouble(OS_CoolingTower_SingleSpeedFields::DesignRangeTemperature,designRangeTemperature);
  }

  void CoolingTowerSingleSpeed_Impl::autosizeDesignRangeTemperature() {
    setString(OS_CoolingTower_SingleSpeedFields::DesignRangeTemperature,"autosize");
  }

  std::string CoolingTowerSingleSpeed_Impl::endUseSubcategory() const {
    auto value = getString(OS_CoolingTower_SingleSpeedFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerSingleSpeed_Impl::setEndUseSubcategory(const std::string & endUseSubcategory) {
    return setString(OS_CoolingTower_SingleSpeedFields::EndUseSubcategory,endUseSubcategory);
  }

} // detail

CoolingTowerSingleSpeed::CoolingTowerSingleSpeed(const Model& model)
  : StraightComponent(CoolingTowerSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoolingTowerSingleSpeed_Impl>());

  autosizeDesignWaterFlowRate();

  autosizeDesignAirFlowRate();

  autosizeFanPoweratDesignAirFlowRate();

  autosizeFanPoweratDesignAirFlowRate();

  autosizeUFactorTimesAreaValueatDesignAirFlowRate();

  autosizeAirFlowRateinFreeConvectionRegime();

  autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();

  setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");

  setNominalCapacity(0.0);

  setFreeConvectionCapacity(0.0);

  setBasinHeaterCapacity(0.0);

  setBasinHeaterSetpointTemperature(0.0);

  setString(OS_CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName,"");

  setEvaporationLossMode("LossFactor");

  setEvaporationLossFactor(0.2);

  setDriftLossPercent(0.008);

  setBlowdownCalculationMode("ConcentrationRatio");

  setBlowdownConcentrationRatio(3.0);

  setString(OS_CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,"");

  setString(OS_CoolingTower_SingleSpeedFields::OutdoorAirInletNodeName,"");

  setCapacityControl("FanCycling");

  setNumberofCells(1);

  setCellControl("MinimalCell");

  setCellMinimumWaterFlowRateFraction(0.33);

  setCellMaximumWaterFlowRateFraction(2.5);

  setSizingFactor(1.0);

  setFreeConvectionAirFlowRateSizingFactor(0.1);

  setFreeConvectionUFactorTimesAreaValueSizingFactor(0.1);

  setHeatRejectionCapacityAndNominalCapacitySizingRatio(1.25);

  setFreeConvectionNominalCapacitySizingFactor(0.1);

  setDesignInletAirDryBulbTemperature(35.0);

  setDesignInletAirWetBulbTemperature(25.6);

  autosizeDesignApproachTemperature();

  autosizeDesignRangeTemperature();

  setEndUseSubcategory("General");
}

IddObjectType CoolingTowerSingleSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoolingTower_SingleSpeed);
}

std::vector<std::string> CoolingTowerSingleSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_SingleSpeedFields::PerformanceInputMethod);
}

std::vector<std::string> CoolingTowerSingleSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_SingleSpeedFields::EvaporationLossMode);
}

std::vector<std::string> CoolingTowerSingleSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_SingleSpeedFields::BlowdownCalculationMode);
}

std::vector<std::string> CoolingTowerSingleSpeed::capacityControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_SingleSpeedFields::CapacityControl);
}

std::vector<std::string> CoolingTowerSingleSpeed::cellControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_SingleSpeedFields::CellControl);
}

boost::optional<double> CoolingTowerSingleSpeed::designWaterFlowRate() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designWaterFlowRate();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getDesignWaterFlowRate(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getDesignWaterFlowRate(returnIP);
}

bool CoolingTowerSingleSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> CoolingTowerSingleSpeed::designAirFlowRate() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designAirFlowRate();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getDesignAirFlowRate(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getDesignAirFlowRate(returnIP);
}

bool CoolingTowerSingleSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> CoolingTowerSingleSpeed::fanPoweratDesignAirFlowRate() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->fanPoweratDesignAirFlowRate();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getFanPoweratDesignAirFlowRate(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getFanPoweratDesignAirFlowRate(returnIP);
}

bool CoolingTowerSingleSpeed::isFanPoweratDesignAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isFanPoweratDesignAirFlowRateAutosized();
}

boost::optional<double> CoolingTowerSingleSpeed::uFactorTimesAreaValueatDesignAirFlowRate() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->uFactorTimesAreaValueatDesignAirFlowRate();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getUFactorTimesAreaValueatDesignAirFlowRate(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getUFactorTimesAreaValueatDesignAirFlowRate(returnIP);
}

bool CoolingTowerSingleSpeed::isUFactorTimesAreaValueatDesignAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isUFactorTimesAreaValueatDesignAirFlowRateAutosized();
}

boost::optional<double> CoolingTowerSingleSpeed::airFlowRateinFreeConvectionRegime() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->airFlowRateinFreeConvectionRegime();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getAirFlowRateinFreeConvectionRegime(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getAirFlowRateinFreeConvectionRegime(returnIP);
}

bool CoolingTowerSingleSpeed::isAirFlowRateinFreeConvectionRegimeDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isAirFlowRateinFreeConvectionRegimeDefaulted();
}

bool CoolingTowerSingleSpeed::isAirFlowRateinFreeConvectionRegimeAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isAirFlowRateinFreeConvectionRegimeAutosized();
}

boost::optional<double> CoolingTowerSingleSpeed::uFactorTimesAreaValueatFreeConvectionAirFlowRate() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->uFactorTimesAreaValueatFreeConvectionAirFlowRate();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getUFactorTimesAreaValueatFreeConvectionAirFlowRate(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getUFactorTimesAreaValueatFreeConvectionAirFlowRate(returnIP);
}

bool CoolingTowerSingleSpeed::isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted();
}

bool CoolingTowerSingleSpeed::isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized();
}

std::string CoolingTowerSingleSpeed::performanceInputMethod() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->performanceInputMethod();
}

bool CoolingTowerSingleSpeed::isPerformanceInputMethodDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isPerformanceInputMethodDefaulted();
}

boost::optional<double> CoolingTowerSingleSpeed::nominalCapacity() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->nominalCapacity();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getNominalCapacity(returnIP);
}

boost::optional<double> CoolingTowerSingleSpeed::freeConvectionCapacity() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->freeConvectionCapacity();
}

OSOptionalQuantity CoolingTowerSingleSpeed::getFreeConvectionCapacity(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getFreeConvectionCapacity(returnIP);
}

double CoolingTowerSingleSpeed::basinHeaterCapacity() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->basinHeaterCapacity();
}

Quantity CoolingTowerSingleSpeed::getBasinHeaterCapacity(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getBasinHeaterCapacity(returnIP);
}

bool CoolingTowerSingleSpeed::isBasinHeaterCapacityDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isBasinHeaterCapacityDefaulted();
}

double CoolingTowerSingleSpeed::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->basinHeaterSetpointTemperature();
}

Quantity CoolingTowerSingleSpeed::getBasinHeaterSetpointTemperature(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getBasinHeaterSetpointTemperature(returnIP);
}

bool CoolingTowerSingleSpeed::isBasinHeaterSetpointTemperatureDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isBasinHeaterSetpointTemperatureDefaulted();
}

boost::optional<Schedule> CoolingTowerSingleSpeed::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->basinHeaterOperatingSchedule();
}

boost::optional<std::string> CoolingTowerSingleSpeed::evaporationLossMode() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->evaporationLossMode();
}

double CoolingTowerSingleSpeed::evaporationLossFactor() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->evaporationLossFactor();
}

Quantity CoolingTowerSingleSpeed::getEvaporationLossFactor(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getEvaporationLossFactor(returnIP);
}

bool CoolingTowerSingleSpeed::isEvaporationLossFactorDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isEvaporationLossFactorDefaulted();
}

double CoolingTowerSingleSpeed::driftLossPercent() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->driftLossPercent();
}

Quantity CoolingTowerSingleSpeed::getDriftLossPercent(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getDriftLossPercent(returnIP);
}

bool CoolingTowerSingleSpeed::isDriftLossPercentDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isDriftLossPercentDefaulted();
}

boost::optional<std::string> CoolingTowerSingleSpeed::blowdownCalculationMode() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->blowdownCalculationMode();
}

double CoolingTowerSingleSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->blowdownConcentrationRatio();
}

Quantity CoolingTowerSingleSpeed::getBlowdownConcentrationRatio(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getBlowdownConcentrationRatio(returnIP);
}

bool CoolingTowerSingleSpeed::isBlowdownConcentrationRatioDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isBlowdownConcentrationRatioDefaulted();
}

boost::optional<Schedule> CoolingTowerSingleSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

std::string CoolingTowerSingleSpeed::capacityControl() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->capacityControl();
}

bool CoolingTowerSingleSpeed::isCapacityControlDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isCapacityControlDefaulted();
}

int CoolingTowerSingleSpeed::numberofCells() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->numberofCells();
}

bool CoolingTowerSingleSpeed::isNumberofCellsDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isNumberofCellsDefaulted();
}

std::string CoolingTowerSingleSpeed::cellControl() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->cellControl();
}

bool CoolingTowerSingleSpeed::isCellControlDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isCellControlDefaulted();
}

double CoolingTowerSingleSpeed::cellMinimumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->cellMinimumWaterFlowRateFraction();
}

Quantity CoolingTowerSingleSpeed::getCellMinimumWaterFlowRateFraction(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getCellMinimumWaterFlowRateFraction(returnIP);
}

bool CoolingTowerSingleSpeed::isCellMinimumWaterFlowRateFractionDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isCellMinimumWaterFlowRateFractionDefaulted();
}

double CoolingTowerSingleSpeed::cellMaximumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->cellMaximumWaterFlowRateFraction();
}

Quantity CoolingTowerSingleSpeed::getCellMaximumWaterFlowRateFraction(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getCellMaximumWaterFlowRateFraction(returnIP);
}

bool CoolingTowerSingleSpeed::isCellMaximumWaterFlowRateFractionDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isCellMaximumWaterFlowRateFractionDefaulted();
}

double CoolingTowerSingleSpeed::sizingFactor() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->sizingFactor();
}

Quantity CoolingTowerSingleSpeed::getSizingFactor(bool returnIP) const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->getSizingFactor(returnIP);
}

bool CoolingTowerSingleSpeed::isSizingFactorDefaulted() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isSizingFactorDefaulted();
}

bool CoolingTowerSingleSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

bool CoolingTowerSingleSpeed::setDesignWaterFlowRate(const Quantity& designWaterFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void CoolingTowerSingleSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetDesignWaterFlowRate();
}

void CoolingTowerSingleSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool CoolingTowerSingleSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

bool CoolingTowerSingleSpeed::setDesignAirFlowRate(const Quantity& designAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void CoolingTowerSingleSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool CoolingTowerSingleSpeed::setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
}

bool CoolingTowerSingleSpeed::setFanPoweratDesignAirFlowRate(const Quantity& fanPoweratDesignAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
}

void CoolingTowerSingleSpeed::autosizeFanPoweratDesignAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeFanPoweratDesignAirFlowRate();
}

bool CoolingTowerSingleSpeed::setUFactorTimesAreaValueatDesignAirFlowRate(double uFactorTimesAreaValueatDesignAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setUFactorTimesAreaValueatDesignAirFlowRate(uFactorTimesAreaValueatDesignAirFlowRate);
}

bool CoolingTowerSingleSpeed::setUFactorTimesAreaValueatDesignAirFlowRate(const Quantity& uFactorTimesAreaValueatDesignAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setUFactorTimesAreaValueatDesignAirFlowRate(uFactorTimesAreaValueatDesignAirFlowRate);
}

void CoolingTowerSingleSpeed::resetUFactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetUFactorTimesAreaValueatDesignAirFlowRate();
}

void CoolingTowerSingleSpeed::autosizeUFactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeUFactorTimesAreaValueatDesignAirFlowRate();
}

bool CoolingTowerSingleSpeed::setAirFlowRateinFreeConvectionRegime(double airFlowRateinFreeConvectionRegime) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setAirFlowRateinFreeConvectionRegime(airFlowRateinFreeConvectionRegime);
}

bool CoolingTowerSingleSpeed::setAirFlowRateinFreeConvectionRegime(const Quantity& airFlowRateinFreeConvectionRegime) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setAirFlowRateinFreeConvectionRegime(airFlowRateinFreeConvectionRegime);
}

void CoolingTowerSingleSpeed::resetAirFlowRateinFreeConvectionRegime() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetAirFlowRateinFreeConvectionRegime();
}

void CoolingTowerSingleSpeed::autosizeAirFlowRateinFreeConvectionRegime() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeAirFlowRateinFreeConvectionRegime();
}

bool CoolingTowerSingleSpeed::setUFactorTimesAreaValueatFreeConvectionAirFlowRate(double uFactorTimesAreaValueatFreeConvectionAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setUFactorTimesAreaValueatFreeConvectionAirFlowRate(uFactorTimesAreaValueatFreeConvectionAirFlowRate);
}

bool CoolingTowerSingleSpeed::setUFactorTimesAreaValueatFreeConvectionAirFlowRate(const Quantity& uFactorTimesAreaValueatFreeConvectionAirFlowRate) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setUFactorTimesAreaValueatFreeConvectionAirFlowRate(uFactorTimesAreaValueatFreeConvectionAirFlowRate);
}

void CoolingTowerSingleSpeed::resetUFactorTimesAreaValueatFreeConvectionAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();
}

void CoolingTowerSingleSpeed::autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();
}

bool CoolingTowerSingleSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void CoolingTowerSingleSpeed::resetPerformanceInputMethod() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetPerformanceInputMethod();
}

bool CoolingTowerSingleSpeed::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setNominalCapacity(nominalCapacity);
}

bool CoolingTowerSingleSpeed::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setNominalCapacity(nominalCapacity);
}

void CoolingTowerSingleSpeed::resetNominalCapacity() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetNominalCapacity();
}

bool CoolingTowerSingleSpeed::setFreeConvectionCapacity(double freeConvectionCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFreeConvectionCapacity(freeConvectionCapacity);
}

bool CoolingTowerSingleSpeed::setFreeConvectionCapacity(const Quantity& freeConvectionCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFreeConvectionCapacity(freeConvectionCapacity);
}

void CoolingTowerSingleSpeed::resetFreeConvectionCapacity() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetFreeConvectionCapacity();
}

bool CoolingTowerSingleSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

bool CoolingTowerSingleSpeed::setBasinHeaterCapacity(const Quantity& basinHeaterCapacity) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

void CoolingTowerSingleSpeed::resetBasinHeaterCapacity() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBasinHeaterCapacity();
}

bool CoolingTowerSingleSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

bool CoolingTowerSingleSpeed::setBasinHeaterSetpointTemperature(const Quantity& basinHeaterSetpointTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

void CoolingTowerSingleSpeed::resetBasinHeaterSetpointTemperature() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBasinHeaterSetpointTemperature();
}

bool CoolingTowerSingleSpeed::setBasinHeaterOperatingSchedule(Schedule& basinHeaterOperatingSchedule) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBasinHeaterOperatingSchedule(basinHeaterOperatingSchedule);
}

void CoolingTowerSingleSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

bool CoolingTowerSingleSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void CoolingTowerSingleSpeed::resetEvaporationLossMode() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetEvaporationLossMode();
}

bool CoolingTowerSingleSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

bool CoolingTowerSingleSpeed::setEvaporationLossFactor(const Quantity& evaporationLossFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void CoolingTowerSingleSpeed::resetEvaporationLossFactor() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetEvaporationLossFactor();
}

bool CoolingTowerSingleSpeed::setDriftLossPercent(double driftLossPercent) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

bool CoolingTowerSingleSpeed::setDriftLossPercent(const Quantity& driftLossPercent) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

void CoolingTowerSingleSpeed::resetDriftLossPercent() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetDriftLossPercent();
}

bool CoolingTowerSingleSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

void CoolingTowerSingleSpeed::resetBlowdownCalculationMode() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBlowdownCalculationMode();
}

bool CoolingTowerSingleSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

bool CoolingTowerSingleSpeed::setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void CoolingTowerSingleSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool CoolingTowerSingleSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& blowdownMakeupWaterUsageSchedule) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(blowdownMakeupWaterUsageSchedule);
}

void CoolingTowerSingleSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

bool CoolingTowerSingleSpeed::setCapacityControl(std::string capacityControl) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCapacityControl(capacityControl);
}

void CoolingTowerSingleSpeed::resetCapacityControl() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetCapacityControl();
}

bool CoolingTowerSingleSpeed::setNumberofCells(int numberofCells) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setNumberofCells(numberofCells);
}

void CoolingTowerSingleSpeed::resetNumberofCells() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetNumberofCells();
}

bool CoolingTowerSingleSpeed::setCellControl(std::string cellControl) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCellControl(cellControl);
}

void CoolingTowerSingleSpeed::resetCellControl() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetCellControl();
}

bool CoolingTowerSingleSpeed::setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCellMinimumWaterFlowRateFraction(cellMinimumWaterFlowRateFraction);
}

bool CoolingTowerSingleSpeed::setCellMinimumWaterFlowRateFraction(const Quantity& cellMinimumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCellMinimumWaterFlowRateFraction(cellMinimumWaterFlowRateFraction);
}

void CoolingTowerSingleSpeed::resetCellMinimumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetCellMinimumWaterFlowRateFraction();
}

bool CoolingTowerSingleSpeed::setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCellMaximumWaterFlowRateFraction(cellMaximumWaterFlowRateFraction);
}

bool CoolingTowerSingleSpeed::setCellMaximumWaterFlowRateFraction(const Quantity& cellMaximumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setCellMaximumWaterFlowRateFraction(cellMaximumWaterFlowRateFraction);
}

void CoolingTowerSingleSpeed::resetCellMaximumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetCellMaximumWaterFlowRateFraction();
}

bool CoolingTowerSingleSpeed::setSizingFactor(double sizingFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setSizingFactor(sizingFactor);
}

bool CoolingTowerSingleSpeed::setSizingFactor(const Quantity& sizingFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setSizingFactor(sizingFactor);
}

void CoolingTowerSingleSpeed::resetSizingFactor() {
  getImpl<detail::CoolingTowerSingleSpeed_Impl>()->resetSizingFactor();
}

double CoolingTowerSingleSpeed::freeConvectionAirFlowRateSizingFactor() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->freeConvectionAirFlowRateSizingFactor();
}

bool CoolingTowerSingleSpeed::setFreeConvectionAirFlowRateSizingFactor(double freeConvectionAirFlowRateSizingFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFreeConvectionAirFlowRateSizingFactor(freeConvectionAirFlowRateSizingFactor);
}

double CoolingTowerSingleSpeed::freeConvectionUFactorTimesAreaValueSizingFactor() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->freeConvectionUFactorTimesAreaValueSizingFactor();
}

bool CoolingTowerSingleSpeed::setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFreeConvectionUFactorTimesAreaValueSizingFactor(freeConvectionUFactorTimesAreaValueSizingFactor);
}

double CoolingTowerSingleSpeed::heatRejectionCapacityAndNominalCapacitySizingRatio() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->heatRejectionCapacityAndNominalCapacitySizingRatio();
}

bool CoolingTowerSingleSpeed::setHeatRejectionCapacityAndNominalCapacitySizingRatio(double heatRejectionCapacityAndNominalCapacitySizingRatio) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setHeatRejectionCapacityAndNominalCapacitySizingRatio(heatRejectionCapacityAndNominalCapacitySizingRatio);
}

double CoolingTowerSingleSpeed::freeConvectionNominalCapacitySizingFactor() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->freeConvectionNominalCapacitySizingFactor();
}

bool CoolingTowerSingleSpeed::setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setFreeConvectionNominalCapacitySizingFactor(freeConvectionNominalCapacitySizingFactor);
}

double CoolingTowerSingleSpeed::designInletAirDryBulbTemperature() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designInletAirWetBulbTemperature();
}

bool CoolingTowerSingleSpeed::setDesignInletAirDryBulbTemperature(double designInletAirDryBulbTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignInletAirDryBulbTemperature(designInletAirDryBulbTemperature);
}

double CoolingTowerSingleSpeed::designInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designInletAirWetBulbTemperature();
}

bool CoolingTowerSingleSpeed::setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignInletAirWetBulbTemperature(designInletAirWetBulbTemperature);
}

boost::optional<double> CoolingTowerSingleSpeed::designApproachTemperature() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designApproachTemperature();
}

bool CoolingTowerSingleSpeed::setDesignApproachTemperature(double designApproachTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignApproachTemperature(designApproachTemperature);
}

void CoolingTowerSingleSpeed::autosizeDesignApproachTemperature() {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeDesignApproachTemperature();
}

boost::optional<double> CoolingTowerSingleSpeed::designRangeTemperature() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->designRangeTemperature();
}

bool CoolingTowerSingleSpeed::setDesignRangeTemperature(double designRangeTemperature) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setDesignRangeTemperature(designRangeTemperature);
}

void CoolingTowerSingleSpeed::autosizeDesignRangeTemperature() {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizeDesignRangeTemperature();
}

std::string CoolingTowerSingleSpeed::endUseSubcategory() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->endUseSubcategory();
}

bool CoolingTowerSingleSpeed::setEndUseSubcategory(const std::string & endUseSubcategory) {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

bool CoolingTowerSingleSpeed::isDesignRangeTemperatureAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isDesignRangeTemperatureAutosized();
}

bool CoolingTowerSingleSpeed::isDesignApproachTemperatureAutosized() const {
  return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->isDesignApproachTemperatureAutosized();
}

/// @cond
CoolingTowerSingleSpeed::CoolingTowerSingleSpeed(std::shared_ptr<detail::CoolingTowerSingleSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

  boost::optional<double> CoolingTowerSingleSpeed::autosizedDesignWaterFlowRate() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedDesignWaterFlowRate();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedDesignAirFlowRate() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedDesignAirFlowRate();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedFanPoweratDesignAirFlowRate() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedFanPoweratDesignAirFlowRate();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedUFactorTimesAreaValueatDesignAirFlowRate() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedUFactorTimesAreaValueatDesignAirFlowRate();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedAirFlowRateinFreeConvectionRegime() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedAirFlowRateinFreeConvectionRegime();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedDesignApproachTemperature() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedDesignApproachTemperature();
  }

  boost::optional<double> CoolingTowerSingleSpeed::autosizedDesignRangeTemperature() const {
    return getImpl<detail::CoolingTowerSingleSpeed_Impl>()->autosizedDesignRangeTemperature();
  }

} // model
} // openstudio
