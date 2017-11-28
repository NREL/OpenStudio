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

#include "Model.hpp"
#include "CoolingTowerVariableSpeed.hpp"
#include "CoolingTowerVariableSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_CoolingTower_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoolingTowerVariableSpeed_Impl::CoolingTowerVariableSpeed_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoolingTowerVariableSpeed::iddObjectType());
  }

  CoolingTowerVariableSpeed_Impl::CoolingTowerVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoolingTowerVariableSpeed::iddObjectType());
  }

  CoolingTowerVariableSpeed_Impl::CoolingTowerVariableSpeed_Impl(const CoolingTowerVariableSpeed_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoolingTowerVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoolingTowerVariableSpeed_Impl::iddObjectType() const {
    return CoolingTowerVariableSpeed::iddObjectType();
  }

  ModelObject CoolingTowerVariableSpeed_Impl::clone(Model model) const
  {
    CoolingTowerVariableSpeed newTower = ModelObject_Impl::clone(model).cast<CoolingTowerVariableSpeed>();

    if( boost::optional<ModelObject> mo = modelCoefficient() )
    {
      newTower.setModelCoefficient(mo->clone(model));
    }

    return newTower;
  }

  std::vector<IddObjectType> CoolingTowerVariableSpeed_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_CoolingTowerPerformance_YorkCalc);
    result.push_back(IddObjectType::OS_CoolingTowerPerformance_CoolTools);
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  std::vector<ModelObject> CoolingTowerVariableSpeed_Impl::children() const
  {
    std::vector<ModelObject> result;
    if(boost::optional<ModelObject> mo = modelCoefficient())
    {
      result.push_back(mo.get());
    }

    if(boost::optional<CurveCubic> mo = fanPowerRatioFunctionofAirFlowRateRatioCurve() )
    {
      result.push_back(mo.get());
    }

    return result;
  }

  std::vector<ScheduleTypeKey> CoolingTowerVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CoolingTower_VariableSpeedFields::BasinHeaterOperatingSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerVariableSpeed","Basin Heater Operating Schedule"));
    }
    if (std::find(b,e,OS_CoolingTower_VariableSpeedFields::BlowdownMakeupWaterUsageSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoolingTowerVariableSpeed","Blowdown Makeup Water Usage Schedule"));
    }
    return result;
  }

  boost::optional<std::string> CoolingTowerVariableSpeed_Impl::modelType() const {
    return getString(OS_CoolingTower_VariableSpeedFields::ModelType,true);
  }

  boost::optional<ModelObject> CoolingTowerVariableSpeed_Impl::modelCoefficient() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_CoolingTower_VariableSpeedFields::ModelCoefficient);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designInletAirWetBulbTemperature() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignInletAirWetBulbTemperature,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designApproachTemperature() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignApproachTemperature,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designRangeTemperature() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignRangeTemperature,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignWaterFlowRate,true);
  }

  bool CoolingTowerVariableSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_VariableSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignAirFlowRate,true);
  }

  bool CoolingTowerVariableSpeed_Impl::isDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_VariableSpeedFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::designFanPower() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DesignFanPower,true);
  }

  bool CoolingTowerVariableSpeed_Impl::isDesignFanPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_CoolingTower_VariableSpeedFields::DesignFanPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<CurveCubic> CoolingTowerVariableSpeed_Impl::fanPowerRatioFunctionofAirFlowRateRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_CoolingTower_VariableSpeedFields::FanPowerRatioFunctionofAirFlowRateRatioCurve);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::minimumAirFlowRateRatio() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::MinimumAirFlowRateRatio,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::fractionofTowerCapacityinFreeConvectionRegime() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::FractionofTowerCapacityinFreeConvectionRegime,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::basinHeaterCapacity() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::BasinHeaterCapacity,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::basinHeaterSetpointTemperature() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::BasinHeaterSetpointTemperature,true);
  }

  boost::optional<Schedule> CoolingTowerVariableSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_VariableSpeedFields::BasinHeaterOperatingSchedule);
  }

  boost::optional<std::string> CoolingTowerVariableSpeed_Impl::evaporationLossMode() const {
    return getString(OS_CoolingTower_VariableSpeedFields::EvaporationLossMode,true);
  }

  double CoolingTowerVariableSpeed_Impl::evaporationLossFactor() const {
    boost::optional<double> value = getDouble(OS_CoolingTower_VariableSpeedFields::EvaporationLossFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoolingTowerVariableSpeed_Impl::isEvaporationLossFactorDefaulted() const {
    return isEmpty(OS_CoolingTower_VariableSpeedFields::EvaporationLossFactor);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::driftLossPercent() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::DriftLossPercent,true);
  }

  boost::optional<std::string> CoolingTowerVariableSpeed_Impl::blowdownCalculationMode() const {
    return getString(OS_CoolingTower_VariableSpeedFields::BlowdownCalculationMode,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::blowdownConcentrationRatio() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::BlowdownConcentrationRatio,true);
  }

  boost::optional<Schedule> CoolingTowerVariableSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CoolingTower_VariableSpeedFields::BlowdownMakeupWaterUsageSchedule);
  }

  boost::optional<int> CoolingTowerVariableSpeed_Impl::numberofCells() const {
    return getInt(OS_CoolingTower_VariableSpeedFields::NumberofCells,true);
  }

  boost::optional<std::string> CoolingTowerVariableSpeed_Impl::cellControl() const {
    return getString(OS_CoolingTower_VariableSpeedFields::CellControl,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::cellMinimumWaterFlowRateFraction() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::CellMinimumWaterFlowRateFraction,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::cellMaximumWaterFlowRateFraction() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::CellMaximumWaterFlowRateFraction,true);
  }

  boost::optional<double> CoolingTowerVariableSpeed_Impl::sizingFactor() const {
    return getDouble(OS_CoolingTower_VariableSpeedFields::SizingFactor,true);
  }

  bool CoolingTowerVariableSpeed_Impl::setModelType(boost::optional<std::string> modelType) {
    bool result(false);
    if (modelType) {
      result = setString(OS_CoolingTower_VariableSpeedFields::ModelType, modelType.get());
    }
    else {
      resetModelType();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetModelType() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::ModelType, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setModelCoefficient(const boost::optional<ModelObject>& variableSpeedTowerCoefficient) {
    bool result(false);
    if (variableSpeedTowerCoefficient) {
      result = setPointer(OS_CoolingTower_VariableSpeedFields::ModelCoefficient, variableSpeedTowerCoefficient.get().handle());
    }
    else {
      resetModelCoefficient();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetModelCoefficient() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::ModelCoefficient, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignInletAirWetBulbTemperature(boost::optional<double> designInletAirWetBulbTemperature) {
    bool result(false);
    if (designInletAirWetBulbTemperature) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignInletAirWetBulbTemperature, designInletAirWetBulbTemperature.get());
    }
    else {
      resetDesignInletAirWetBulbTemperature();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetDesignInletAirWetBulbTemperature() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignInletAirWetBulbTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignApproachTemperature(boost::optional<double> designApproachTemperature) {
    bool result(false);
    if (designApproachTemperature) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignApproachTemperature, designApproachTemperature.get());
    }
    else {
      resetDesignApproachTemperature();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetDesignApproachTemperature() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignApproachTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignRangeTemperature(boost::optional<double> designRangeTemperature) {
    bool result(false);
    if (designRangeTemperature) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignRangeTemperature, designRangeTemperature.get());
    }
    else {
      resetDesignRangeTemperature();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetDesignRangeTemperature() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignRangeTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result(false);
    if (designAirFlowRate) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setDesignFanPower(boost::optional<double> designFanPower) {
    bool result(false);
    if (designFanPower) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DesignFanPower, designFanPower.get());
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::autosizeDesignFanPower() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DesignFanPower, "autosize");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setFanPowerRatioFunctionofAirFlowRateRatioCurve(const boost::optional<CurveCubic>& cubicCurves) {
    bool result(false);
    if (cubicCurves) {
      result = setPointer(OS_CoolingTower_VariableSpeedFields::FanPowerRatioFunctionofAirFlowRateRatioCurve, cubicCurves.get().handle());
    }
    else {
      resetFanPowerRatioFunctionofAirFlowRateRatioCurve();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetFanPowerRatioFunctionofAirFlowRateRatioCurve() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::FanPowerRatioFunctionofAirFlowRateRatioCurve, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setMinimumAirFlowRateRatio(boost::optional<double> minimumAirFlowRateRatio) {
    bool result(false);
    if (minimumAirFlowRateRatio) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::MinimumAirFlowRateRatio, minimumAirFlowRateRatio.get());
    }
    else {
      resetMinimumAirFlowRateRatio();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetMinimumAirFlowRateRatio() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::MinimumAirFlowRateRatio, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setFractionofTowerCapacityinFreeConvectionRegime(boost::optional<double> fractionofTowerCapacityinFreeConvectionRegime) {
    bool result(false);
    if (fractionofTowerCapacityinFreeConvectionRegime) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::FractionofTowerCapacityinFreeConvectionRegime, fractionofTowerCapacityinFreeConvectionRegime.get());
    }
    else {
      resetFractionofTowerCapacityinFreeConvectionRegime();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetFractionofTowerCapacityinFreeConvectionRegime() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::FractionofTowerCapacityinFreeConvectionRegime, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBasinHeaterCapacity(boost::optional<double> basinHeaterCapacity) {
    bool result(false);
    if (basinHeaterCapacity) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::BasinHeaterCapacity, basinHeaterCapacity.get());
    }
    else {
      resetBasinHeaterCapacity();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBasinHeaterCapacity() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BasinHeaterCapacity, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBasinHeaterSetpointTemperature(boost::optional<double> basinHeaterSetpointTemperature) {
    bool result(false);
    if (basinHeaterSetpointTemperature) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature.get());
    }
    else {
      resetBasinHeaterSetpointTemperature();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBasinHeaterSetpointTemperature() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BasinHeaterSetpointTemperature, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_VariableSpeedFields::BasinHeaterOperatingSchedule,
                              "CoolingTowerVariableSpeed",
                              "Basin Heater Operating Schedule",
                              schedule);
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BasinHeaterOperatingSchedule, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setEvaporationLossMode(boost::optional<std::string> evaporationLossMode) {
    bool result(false);
    if (evaporationLossMode) {
      result = setString(OS_CoolingTower_VariableSpeedFields::EvaporationLossMode, evaporationLossMode.get());
    }
    else {
      resetEvaporationLossMode();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetEvaporationLossMode() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::EvaporationLossMode, "");
    OS_ASSERT(result);
  }

  void CoolingTowerVariableSpeed_Impl::setEvaporationLossFactor(double evaporationLossFactor) {
    bool result = setDouble(OS_CoolingTower_VariableSpeedFields::EvaporationLossFactor, evaporationLossFactor);
    OS_ASSERT(result);
  }

  void CoolingTowerVariableSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::EvaporationLossFactor, "");
    OS_ASSERT(result);
  }

  void CoolingTowerVariableSpeed_Impl::setDriftLossPercent(boost::optional<double> driftLossPercent) {
    bool result(false);
    if (driftLossPercent) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::DriftLossPercent, driftLossPercent.get());
    }
    else {
      resetDriftLossPercent();
      result = true;
    }
    OS_ASSERT(result);
  }

  void CoolingTowerVariableSpeed_Impl::resetDriftLossPercent() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::DriftLossPercent, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode) {
    bool result(false);
    if (blowdownCalculationMode) {
      result = setString(OS_CoolingTower_VariableSpeedFields::BlowdownCalculationMode, blowdownCalculationMode.get());
    }
    else {
      resetBlowdownCalculationMode();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBlowdownCalculationMode() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BlowdownCalculationMode, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBlowdownConcentrationRatio(boost::optional<double> blowdownConcentrationRatio) {
    bool result(false);
    if (blowdownConcentrationRatio) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio.get());
    }
    else {
      resetBlowdownConcentrationRatio();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BlowdownConcentrationRatio, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CoolingTower_VariableSpeedFields::BlowdownMakeupWaterUsageSchedule,
                              "CoolingTowerVariableSpeed",
                              "Blowdown Makeup Water Usage Schedule",
                              schedule);
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::BlowdownMakeupWaterUsageSchedule, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setNumberofCells(boost::optional<int> numberofCells) {
    bool result(false);
    if (numberofCells) {
      result = setInt(OS_CoolingTower_VariableSpeedFields::NumberofCells, numberofCells.get());
    }
    else {
      resetNumberofCells();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetNumberofCells() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::NumberofCells, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setCellControl(boost::optional<std::string> cellControl) {
    bool result(false);
    if (cellControl) {
      result = setString(OS_CoolingTower_VariableSpeedFields::CellControl, cellControl.get());
    }
    else {
      resetCellControl();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetCellControl() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::CellControl, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setCellMinimumWaterFlowRateFraction(boost::optional<double> cellMinimumWaterFlowRateFraction) {
    bool result(false);
    if (cellMinimumWaterFlowRateFraction) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::CellMinimumWaterFlowRateFraction, cellMinimumWaterFlowRateFraction.get());
    }
    else {
      resetCellMinimumWaterFlowRateFraction();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetCellMinimumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::CellMinimumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setCellMaximumWaterFlowRateFraction(boost::optional<double> cellMaximumWaterFlowRateFraction) {
    bool result(false);
    if (cellMaximumWaterFlowRateFraction) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::CellMaximumWaterFlowRateFraction, cellMaximumWaterFlowRateFraction.get());
    }
    else {
      resetCellMaximumWaterFlowRateFraction();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetCellMaximumWaterFlowRateFraction() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::CellMaximumWaterFlowRateFraction, "");
    OS_ASSERT(result);
  }

  bool CoolingTowerVariableSpeed_Impl::setSizingFactor(boost::optional<double> sizingFactor) {
    bool result(false);
    if (sizingFactor) {
      result = setDouble(OS_CoolingTower_VariableSpeedFields::SizingFactor, sizingFactor.get());
    }
    else {
      resetSizingFactor();
      result = true;
    }
    return result;
  }

  void CoolingTowerVariableSpeed_Impl::resetSizingFactor() {
    bool result = setString(OS_CoolingTower_VariableSpeedFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  unsigned CoolingTowerVariableSpeed_Impl::inletPort()
  {
    return OS_CoolingTower_VariableSpeedFields::WaterInletNode;
  }

  unsigned CoolingTowerVariableSpeed_Impl::outletPort()
  {
    return OS_CoolingTower_VariableSpeedFields::WaterOutletNode;
  }

  bool CoolingTowerVariableSpeed_Impl::addToNode(Node & node)
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

} // detail

CoolingTowerVariableSpeed::CoolingTowerVariableSpeed(const Model& model)
  : StraightComponent(CoolingTowerVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoolingTowerVariableSpeed_Impl>());

  setModelType("CoolToolsCrossFlow");
  setDesignInletAirWetBulbTemperature(25.5556);
  setDesignApproachTemperature(3.8889);
  setDesignRangeTemperature(5.5556);
  autosizeDesignWaterFlowRate();
  autosizeDesignAirFlowRate();
  autosizeDesignFanPower();
  setMinimumAirFlowRateRatio(0.2000);
  setFractionofTowerCapacityinFreeConvectionRegime(0.1250);
  setBasinHeaterCapacity(0.0000);
  setBasinHeaterSetpointTemperature(2.0000);
  setEvaporationLossMode("SaturatedExit");
  setEvaporationLossFactor(0.2000);
  setDriftLossPercent(0.0080);
  setBlowdownCalculationMode("ConcentrationRatio");
  setBlowdownConcentrationRatio(3.0000);
  setSizingFactor(1.0000);

  CurveCubic curve(model);
  curve.setName(name().get() + " Fan Power Ratio Curve");
  curve.setCoefficient1Constant(-0.0093);
  curve.setCoefficient2x(0.0512);
  curve.setCoefficient3xPOW2(-0.0838);
  curve.setCoefficient4xPOW3(1.0419);
  curve.setMinimumValueofx(0.1500);
  curve.setMaximumValueofx(1.0000);

  setFanPowerRatioFunctionofAirFlowRateRatioCurve(curve);
}

IddObjectType CoolingTowerVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoolingTower_VariableSpeed);
}

std::vector<std::string> CoolingTowerVariableSpeed::modelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_VariableSpeedFields::ModelType);
}

std::vector<std::string> CoolingTowerVariableSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_VariableSpeedFields::EvaporationLossMode);
}

std::vector<std::string> CoolingTowerVariableSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_VariableSpeedFields::BlowdownCalculationMode);
}

std::vector<std::string> CoolingTowerVariableSpeed::cellControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CoolingTower_VariableSpeedFields::CellControl);
}

boost::optional<std::string> CoolingTowerVariableSpeed::modelType() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->modelType();
}

boost::optional<ModelObject> CoolingTowerVariableSpeed::modelCoefficient() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->modelCoefficient();
}

boost::optional<double> CoolingTowerVariableSpeed::designInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designInletAirWetBulbTemperature();
}

boost::optional<double> CoolingTowerVariableSpeed::designApproachTemperature() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designApproachTemperature();
}

boost::optional<double> CoolingTowerVariableSpeed::designRangeTemperature() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designRangeTemperature();
}

boost::optional<double> CoolingTowerVariableSpeed::designWaterFlowRate() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designWaterFlowRate();
}

bool CoolingTowerVariableSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> CoolingTowerVariableSpeed::designAirFlowRate() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designAirFlowRate();
}

bool CoolingTowerVariableSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> CoolingTowerVariableSpeed::designFanPower() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->designFanPower();
}

bool CoolingTowerVariableSpeed::isDesignFanPowerAutosized() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->isDesignFanPowerAutosized();
}

boost::optional<CurveCubic> CoolingTowerVariableSpeed::fanPowerRatioFunctionofAirFlowRateRatioCurve() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->fanPowerRatioFunctionofAirFlowRateRatioCurve();
}

boost::optional<double> CoolingTowerVariableSpeed::minimumAirFlowRateRatio() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->minimumAirFlowRateRatio();
}

boost::optional<double> CoolingTowerVariableSpeed::fractionofTowerCapacityinFreeConvectionRegime() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->fractionofTowerCapacityinFreeConvectionRegime();
}

boost::optional<double> CoolingTowerVariableSpeed::basinHeaterCapacity() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->basinHeaterCapacity();
}

boost::optional<double> CoolingTowerVariableSpeed::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->basinHeaterSetpointTemperature();
}

boost::optional<Schedule> CoolingTowerVariableSpeed::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->basinHeaterOperatingSchedule();
}

boost::optional<std::string> CoolingTowerVariableSpeed::evaporationLossMode() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->evaporationLossMode();
}

double CoolingTowerVariableSpeed::evaporationLossFactor() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->evaporationLossFactor();
}

bool CoolingTowerVariableSpeed::isEvaporationLossFactorDefaulted() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->isEvaporationLossFactorDefaulted();
}

boost::optional<double> CoolingTowerVariableSpeed::driftLossPercent() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->driftLossPercent();
}

boost::optional<std::string> CoolingTowerVariableSpeed::blowdownCalculationMode() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->blowdownCalculationMode();
}

boost::optional<double> CoolingTowerVariableSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->blowdownConcentrationRatio();
}

boost::optional<Schedule> CoolingTowerVariableSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

boost::optional<int> CoolingTowerVariableSpeed::numberofCells() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->numberofCells();
}

boost::optional<std::string> CoolingTowerVariableSpeed::cellControl() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->cellControl();
}

boost::optional<double> CoolingTowerVariableSpeed::cellMinimumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->cellMinimumWaterFlowRateFraction();
}

boost::optional<double> CoolingTowerVariableSpeed::cellMaximumWaterFlowRateFraction() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->cellMaximumWaterFlowRateFraction();
}

boost::optional<double> CoolingTowerVariableSpeed::sizingFactor() const {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->sizingFactor();
}

bool CoolingTowerVariableSpeed::setModelType(std::string modelType) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setModelType(modelType);
}

void CoolingTowerVariableSpeed::resetModelType() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetModelType();
}

bool CoolingTowerVariableSpeed::setModelCoefficient(const ModelObject& variableSpeedTowerCoefficient) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setModelCoefficient(variableSpeedTowerCoefficient);
}

void CoolingTowerVariableSpeed::resetModelCoefficient() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetModelCoefficient();
}

bool CoolingTowerVariableSpeed::setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignInletAirWetBulbTemperature(designInletAirWetBulbTemperature);
}

void CoolingTowerVariableSpeed::resetDesignInletAirWetBulbTemperature() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetDesignInletAirWetBulbTemperature();
}

bool CoolingTowerVariableSpeed::setDesignApproachTemperature(double designApproachTemperature) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignApproachTemperature(designApproachTemperature);
}

void CoolingTowerVariableSpeed::resetDesignApproachTemperature() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetDesignApproachTemperature();
}

bool CoolingTowerVariableSpeed::setDesignRangeTemperature(double designRangeTemperature) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignRangeTemperature(designRangeTemperature);
}

void CoolingTowerVariableSpeed::resetDesignRangeTemperature() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetDesignRangeTemperature();
}

bool CoolingTowerVariableSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void CoolingTowerVariableSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool CoolingTowerVariableSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void CoolingTowerVariableSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool CoolingTowerVariableSpeed::setDesignFanPower(double designFanPower) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDesignFanPower(designFanPower);
}

void CoolingTowerVariableSpeed::autosizeDesignFanPower() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->autosizeDesignFanPower();
}

bool CoolingTowerVariableSpeed::setFanPowerRatioFunctionofAirFlowRateRatioCurve(const CurveCubic& curve) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setFanPowerRatioFunctionofAirFlowRateRatioCurve(curve);
}

void CoolingTowerVariableSpeed::resetFanPowerRatioFunctionofAirFlowRateRatioCurve() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetFanPowerRatioFunctionofAirFlowRateRatioCurve();
}

bool CoolingTowerVariableSpeed::setMinimumAirFlowRateRatio(double minimumAirFlowRateRatio) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setMinimumAirFlowRateRatio(minimumAirFlowRateRatio);
}

void CoolingTowerVariableSpeed::resetMinimumAirFlowRateRatio() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetMinimumAirFlowRateRatio();
}

bool CoolingTowerVariableSpeed::setFractionofTowerCapacityinFreeConvectionRegime(double fractionofTowerCapacityinFreeConvectionRegime) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setFractionofTowerCapacityinFreeConvectionRegime(fractionofTowerCapacityinFreeConvectionRegime);
}

void CoolingTowerVariableSpeed::resetFractionofTowerCapacityinFreeConvectionRegime() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetFractionofTowerCapacityinFreeConvectionRegime();
}

bool CoolingTowerVariableSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

void CoolingTowerVariableSpeed::resetBasinHeaterCapacity() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBasinHeaterCapacity();
}

bool CoolingTowerVariableSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

void CoolingTowerVariableSpeed::resetBasinHeaterSetpointTemperature() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBasinHeaterSetpointTemperature();
}

bool CoolingTowerVariableSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void CoolingTowerVariableSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

bool CoolingTowerVariableSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void CoolingTowerVariableSpeed::resetEvaporationLossMode() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetEvaporationLossMode();
}

void CoolingTowerVariableSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void CoolingTowerVariableSpeed::resetEvaporationLossFactor() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetEvaporationLossFactor();
}

void CoolingTowerVariableSpeed::setDriftLossPercent(double driftLossPercent) {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

void CoolingTowerVariableSpeed::resetDriftLossPercent() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetDriftLossPercent();
}

bool CoolingTowerVariableSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

void CoolingTowerVariableSpeed::resetBlowdownCalculationMode() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBlowdownCalculationMode();
}

bool CoolingTowerVariableSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void CoolingTowerVariableSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool CoolingTowerVariableSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void CoolingTowerVariableSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

bool CoolingTowerVariableSpeed::setNumberofCells(int numberofCells) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setNumberofCells(numberofCells);
}

void CoolingTowerVariableSpeed::resetNumberofCells() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetNumberofCells();
}

bool CoolingTowerVariableSpeed::setCellControl(std::string cellControl) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setCellControl(cellControl);
}

void CoolingTowerVariableSpeed::resetCellControl() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetCellControl();
}

bool CoolingTowerVariableSpeed::setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setCellMinimumWaterFlowRateFraction(cellMinimumWaterFlowRateFraction);
}

void CoolingTowerVariableSpeed::resetCellMinimumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetCellMinimumWaterFlowRateFraction();
}

bool CoolingTowerVariableSpeed::setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setCellMaximumWaterFlowRateFraction(cellMaximumWaterFlowRateFraction);
}

void CoolingTowerVariableSpeed::resetCellMaximumWaterFlowRateFraction() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetCellMaximumWaterFlowRateFraction();
}

bool CoolingTowerVariableSpeed::setSizingFactor(double sizingFactor) {
  return getImpl<detail::CoolingTowerVariableSpeed_Impl>()->setSizingFactor(sizingFactor);
}

void CoolingTowerVariableSpeed::resetSizingFactor() {
  getImpl<detail::CoolingTowerVariableSpeed_Impl>()->resetSizingFactor();
}

/// @cond
CoolingTowerVariableSpeed::CoolingTowerVariableSpeed(std::shared_ptr<detail::CoolingTowerVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

