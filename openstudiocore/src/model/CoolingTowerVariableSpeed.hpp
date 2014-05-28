/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COOLINGTOWERVARIABLESPEED_HPP
#define MODEL_COOLINGTOWERVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class CurveCubic;

namespace detail {

  class CoolingTowerVariableSpeed_Impl;

} // detail

/** CoolingTowerVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoolingTower:VariableSpeed'. */
class MODEL_API CoolingTowerVariableSpeed : public StraightComponent {
 public:

  explicit CoolingTowerVariableSpeed(const Model& model);

  virtual ~CoolingTowerVariableSpeed() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> modelTypeValues();

  static std::vector<std::string> evaporationLossModeValues();

  static std::vector<std::string> blowdownCalculationModeValues();

  static std::vector<std::string> cellControlValues();

  boost::optional<std::string> modelType() const;

  bool setModelType(std::string modelType);

  void resetModelType();

  /** Returns CoolingTowerPerformanceYorkCalc, CoolingTowerPerformanceCoolTools, or boost::none 
   */
  boost::optional<ModelObject> modelCoefficient() const;

  /** Accepts CoolingTowerPerformanceYorkCalc or CoolingTowerPerformanceCoolTools
   */
  bool setModelCoefficient(const ModelObject& variableSpeedTowerCoefficient);

  void resetModelCoefficient();

  boost::optional<double> designInletAirWetBulbTemperature() const;

  bool setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature);

  void resetDesignInletAirWetBulbTemperature();

  boost::optional<double> designApproachTemperature() const;

  bool setDesignApproachTemperature(double designApproachTemperature);

  void resetDesignApproachTemperature();

  boost::optional<double> designRangeTemperature() const;

  bool setDesignRangeTemperature(double designRangeTemperature);

  void resetDesignRangeTemperature();

  boost::optional<double> designWaterFlowRate() const;

  bool isDesignWaterFlowRateAutosized() const;

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void autosizeDesignWaterFlowRate();

  boost::optional<double> designAirFlowRate() const;

  bool isDesignAirFlowRateAutosized() const;

  bool setDesignAirFlowRate(double designAirFlowRate);

  void autosizeDesignAirFlowRate();

  boost::optional<double> designFanPower() const;

  bool isDesignFanPowerAutosized() const;

  bool setDesignFanPower(double designFanPower);

  void autosizeDesignFanPower();

  boost::optional<CurveCubic> fanPowerRatioFunctionofAirFlowRateRatioCurve() const;

  bool setFanPowerRatioFunctionofAirFlowRateRatioCurve(const CurveCubic & curve);

  void resetFanPowerRatioFunctionofAirFlowRateRatioCurve();

  boost::optional<double> minimumAirFlowRateRatio() const;

  bool setMinimumAirFlowRateRatio(double minimumAirFlowRateRatio);

  void resetMinimumAirFlowRateRatio();

  boost::optional<double> fractionofTowerCapacityinFreeConvectionRegime() const;

  bool setFractionofTowerCapacityinFreeConvectionRegime(double fractionofTowerCapacityinFreeConvectionRegime);

  void resetFractionofTowerCapacityinFreeConvectionRegime();

  boost::optional<double> basinHeaterCapacity() const;

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  boost::optional<double> basinHeaterSetpointTemperature() const;

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();

  boost::optional<std::string> evaporationLossMode() const;

  bool setEvaporationLossMode(std::string evaporationLossMode);

  void resetEvaporationLossMode();

  double evaporationLossFactor() const;

  bool isEvaporationLossFactorDefaulted() const;

  void setEvaporationLossFactor(double evaporationLossFactor);

  void resetEvaporationLossFactor();

  boost::optional<double> driftLossPercent() const;

  void setDriftLossPercent(double driftLossPercent);

  void resetDriftLossPercent();

  boost::optional<std::string> blowdownCalculationMode() const;

  bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

  void resetBlowdownCalculationMode();

  boost::optional<double> blowdownConcentrationRatio() const;

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

  void resetBlowdownConcentrationRatio();

  boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

  bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

  void resetBlowdownMakeupWaterUsageSchedule();

  boost::optional<int> numberofCells() const;

  bool setNumberofCells(int numberofCells);

  void resetNumberofCells();

  boost::optional<std::string> cellControl() const;

  bool setCellControl(std::string cellControl);

  void resetCellControl();

  boost::optional<double> cellMinimumWaterFlowRateFraction() const;

  bool setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction);

  void resetCellMinimumWaterFlowRateFraction();

  boost::optional<double> cellMaximumWaterFlowRateFraction() const;

  bool setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction);

  void resetCellMaximumWaterFlowRateFraction();

  boost::optional<double> sizingFactor() const;

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

 protected:

  /// @cond
  typedef detail::CoolingTowerVariableSpeed_Impl ImplType;

  explicit CoolingTowerVariableSpeed(std::shared_ptr<detail::CoolingTowerVariableSpeed_Impl> impl);

  friend class detail::CoolingTowerVariableSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.CoolingTowerVariableSpeed");
};

/** \relates CoolingTowerVariableSpeed*/
typedef boost::optional<CoolingTowerVariableSpeed> OptionalCoolingTowerVariableSpeed;

/** \relates CoolingTowerVariableSpeed*/
typedef std::vector<CoolingTowerVariableSpeed> CoolingTowerVariableSpeedVector;

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERVARIABLESPEED_HPP

