/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  }  // namespace detail

  /** CoolingTowerVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoolingTower:VariableSpeed'. */
  class MODEL_API CoolingTowerVariableSpeed : public StraightComponent
  {
   public:
    explicit CoolingTowerVariableSpeed(const Model& model);

    virtual ~CoolingTowerVariableSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoolingTowerVariableSpeed(const CoolingTowerVariableSpeed& other) = default;
    CoolingTowerVariableSpeed(CoolingTowerVariableSpeed&& other) = default;
    CoolingTowerVariableSpeed& operator=(const CoolingTowerVariableSpeed&) = default;
    CoolingTowerVariableSpeed& operator=(CoolingTowerVariableSpeed&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> modelTypeValues();

    static std::vector<std::string> evaporationLossModeValues();

    static std::vector<std::string> blowdownCalculationModeValues();

    static std::vector<std::string> cellControlValues();

    boost::optional<std::string> modelType() const;

    bool setModelType(const std::string& modelType);

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

    bool setFanPowerRatioFunctionofAirFlowRateRatioCurve(const CurveCubic& curve);

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

    std::string evaporationLossMode() const;

    bool isEvaporationLossModeDefaulted() const;

    bool setEvaporationLossMode(const std::string& evaporationLossMode);

    void resetEvaporationLossMode();

    double evaporationLossFactor() const;

    bool isEvaporationLossFactorDefaulted() const;

    bool setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    boost::optional<double> driftLossPercent() const;

    bool setDriftLossPercent(double driftLossPercent);

    void resetDriftLossPercent();

    std::string blowdownCalculationMode() const;

    bool isBlowdownCalculationModeDefaulted() const;

    bool setBlowdownCalculationMode(const std::string& blowdownCalculationMode);

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

    std::string cellControl() const;

    bool isCellControlDefaulted() const;

    bool setCellControl(const std::string& cellControl);

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

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedDesignAirFlowRate() const;

    boost::optional<double> autosizedDesignFanPower() const;

    std::string endUseSubcategory() const;

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

   protected:
    /// @cond
    using ImplType = detail::CoolingTowerVariableSpeed_Impl;

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
  using OptionalCoolingTowerVariableSpeed = boost::optional<CoolingTowerVariableSpeed>;

  /** \relates CoolingTowerVariableSpeed*/
  using CoolingTowerVariableSpeedVector = std::vector<CoolingTowerVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COOLINGTOWERVARIABLESPEED_HPP
