/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP
#define MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoolingTowerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoolingTowerTwoSpeed.*/
  class MODEL_API CoolingTowerTwoSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoolingTowerTwoSpeed_Impl(const IdfObject& idfObject,
                              Model_Impl* model,
                              bool keepHandle);

    CoolingTowerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    CoolingTowerTwoSpeed_Impl(const CoolingTowerTwoSpeed_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    virtual ~CoolingTowerTwoSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> highFanSpeedUFactorTimesAreaValue() const;

    bool isHighFanSpeedUFactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutosized() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    bool isLowFanSpeedAirFlowRateSizingFactorDefaulted() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutosized() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    bool isLowFanSpeedFanPowerSizingFactorDefaulted() const;

    boost::optional<double> lowFanSpeedUFactorTimesAreaValue() const;

    bool isLowFanSpeedUFactorTimesAreaValueAutosized() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    bool isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted() const;

    boost::optional<double> freeConvectionRegimeAirFlowRate() const;

    bool isFreeConvectionRegimeAirFlowRateDefaulted() const;

    bool isFreeConvectionRegimeAirFlowRateAutosized() const;

    double freeConvectionRegimeAirFlowRateSizingFactor() const;

    bool isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted() const;

    boost::optional<double> freeConvectionRegimeUFactorTimesAreaValue() const;

    bool isFreeConvectionRegimeUFactorTimesAreaValueDefaulted() const;

    bool isFreeConvectionRegimeUFactorTimesAreaValueAutosized() const;

    double freeConvectionUFactorTimesAreaValueSizingFactor() const;

    bool isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted() const;

    std::string performanceInputMethod() const;

    bool isPerformanceInputMethodDefaulted() const;

    double heatRejectionCapacityandNominalCapacitySizingRatio() const;

    bool isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted() const;

    boost::optional<double> highSpeedNominalCapacity() const;

    boost::optional<double> lowSpeedNominalCapacity() const;

    bool isLowSpeedNominalCapacityAutosized() const;

    double lowSpeedNominalCapacitySizingFactor() const;

    bool isLowSpeedNominalCapacitySizingFactorDefaulted() const;

    boost::optional<double> freeConvectionNominalCapacity() const;

    bool isFreeConvectionNominalCapacityAutosized() const;

    double freeConvectionNominalCapacitySizingFactor() const;

    bool isFreeConvectionNominalCapacitySizingFactorDefaulted() const;

    double basinHeaterCapacity() const;

    bool isBasinHeaterCapacityDefaulted() const;

    double basinHeaterSetpointTemperature() const;

    bool isBasinHeaterSetpointTemperatureDefaulted() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    boost::optional<std::string> evaporationLossMode() const;

    double evaporationLossFactor() const;

    bool isEvaporationLossFactorDefaulted() const;

    double driftLossPercent() const;

    bool isDriftLossPercentDefaulted() const;

    boost::optional<std::string> blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    bool isBlowdownConcentrationRatioDefaulted() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    int numberofCells() const;

    bool isNumberofCellsDefaulted() const;

    std::string cellControl() const;

    bool isCellControlDefaulted() const;

    double cellMinimumWaterFlowRateFraction() const;

    bool isCellMinimumWaterFlowRateFractionDefaulted() const;

    double cellMaximumWaterFlowRateFraction() const;

    bool isCellMaximumWaterFlowRateFractionDefaulted() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedHighFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedHighFanSpeedFanPower() const;

    boost::optional<double> autosizedHighFanSpeedUFactorTimesAreaValue() const;

    boost::optional<double> autosizedLowFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedLowFanSpeedFanPower() const;

    boost::optional<double> autosizedLowFanSpeedUFactorTimesAreaValue() const;

    boost::optional<double> autosizedFreeConvectionRegimeAirFlowRate() const;

    boost::optional<double> autosizedFreeConvectionRegimeUFactorTimesAreaValue() const;

    boost::optional<double> autosizedLowSpeedNominalCapacity() const;

    boost::optional<double> autosizedFreeConvectionNominalCapacity() const;

    boost::optional<double> autosizedDesignApproachTemperature() const;

    boost::optional<double> autosizedDesignRangeTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

    void autosizeHighFanSpeedAirFlowRate();

    bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

    void autosizeHighFanSpeedFanPower();

    bool setHighFanSpeedUFactorTimesAreaValue(boost::optional<double> highFanSpeedUFactorTimesAreaValue);

    void resetHighFanSpeedUFactorTimesAreaValue();

    void autosizeHighFanSpeedUFactorTimesAreaValue();

    bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

    void autosizeLowFanSpeedAirFlowRate();

    bool setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    void resetLowFanSpeedAirFlowRateSizingFactor();

    bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

    void autosizeLowFanSpeedFanPower();

    bool setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    void resetLowFanSpeedFanPowerSizingFactor();

    bool setLowFanSpeedUFactorTimesAreaValue(boost::optional<double> lowFanSpeedUFactorTimesAreaValue);

    void resetLowFanSpeedUFactorTimesAreaValue();

    void autosizeLowFanSpeedUFactorTimesAreaValue();

    bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    void resetLowFanSpeedUFactorTimesAreaSizingFactor();

    bool setFreeConvectionRegimeAirFlowRate(boost::optional<double> freeConvectionRegimeAirFlowRate);

    void resetFreeConvectionRegimeAirFlowRate();

    void autosizeFreeConvectionRegimeAirFlowRate();

    bool setFreeConvectionRegimeAirFlowRateSizingFactor(double freeConvectionRegimeAirFlowRateSizingFactor);

    void resetFreeConvectionRegimeAirFlowRateSizingFactor();

    bool setFreeConvectionRegimeUFactorTimesAreaValue(boost::optional<double> freeConvectionRegimeUFactorTimesAreaValue);

    void resetFreeConvectionRegimeUFactorTimesAreaValue();

    void autosizeFreeConvectionRegimeUFactorTimesAreaValue();

    bool setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor);

    void resetFreeConvectionUFactorTimesAreaValueSizingFactor();

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    void resetPerformanceInputMethod();

    bool setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

    void resetHeatRejectionCapacityandNominalCapacitySizingRatio();

    bool setHighSpeedNominalCapacity(boost::optional<double> highSpeedNominalCapacity);

    void resetHighSpeedNominalCapacity();

    bool setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity);

    void resetLowSpeedNominalCapacity();

    void autosizeLowSpeedNominalCapacity();

    bool setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

    void resetLowSpeedNominalCapacitySizingFactor();

    bool setFreeConvectionNominalCapacity(boost::optional<double> freeConvectionNominalCapacity);

    void resetFreeConvectionNominalCapacity();

    void autosizeFreeConvectionNominalCapacity();

    bool setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor);

    void resetFreeConvectionNominalCapacitySizingFactor();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setEvaporationLossMode(boost::optional<std::string> evaporationLossMode);

    void resetEvaporationLossMode();

    bool setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    bool setDriftLossPercent(double driftLossPercent);

    void resetDriftLossPercent();

    bool setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode);

    void resetBlowdownCalculationMode();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    bool setNumberofCells(int numberofCells);

    void resetNumberofCells();

    bool setCellControl(std::string cellControl);

    void resetCellControl();

    bool setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction);

    void resetCellMinimumWaterFlowRateFraction();

    bool setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction);

    void resetCellMaximumWaterFlowRateFraction();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    double designInletAirDryBulbTemperature() const;
    bool setDesignInletAirDryBulbTemperature(double designInletAirDryBulbTemperature);

    double designInletAirWetBulbTemperature() const;
    bool setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature);

    boost::optional<double> designApproachTemperature() const;
    bool isDesignApproachTemperatureAutosized() const;
    bool setDesignApproachTemperature(double designApproachTemperature);
    void autosizeDesignApproachTemperature();

    boost::optional<double> designRangeTemperature() const;
    bool isDesignRangeTemperatureAutosized() const;
    bool setDesignRangeTemperature(double designRangeTemperature);
    void autosizeDesignRangeTemperature();

    std::string endUseSubcategory() const;
    bool setEndUseSubcategory(const std::string & endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoolingTowerTwoSpeed");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP
