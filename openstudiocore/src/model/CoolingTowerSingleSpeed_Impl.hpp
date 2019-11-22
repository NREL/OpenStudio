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

#ifndef MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP
#define MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** CoolingTowerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoolingTowerSingleSpeed.*/
  class MODEL_API CoolingTowerSingleSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoolingTowerSingleSpeed_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoolingTowerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoolingTowerSingleSpeed_Impl(const CoolingTowerSingleSpeed_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~CoolingTowerSingleSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> designWaterFlowRate() const;

    OSOptionalQuantity getDesignWaterFlowRate(bool returnIP=false) const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> designAirFlowRate() const;

    OSOptionalQuantity getDesignAirFlowRate(bool returnIP=false) const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> fanPoweratDesignAirFlowRate() const;

    OSOptionalQuantity getFanPoweratDesignAirFlowRate(bool returnIP=false) const;

    bool isFanPoweratDesignAirFlowRateAutosized() const;

    boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate() const;

    OSOptionalQuantity getUFactorTimesAreaValueatDesignAirFlowRate(bool returnIP=false) const;

    bool isUFactorTimesAreaValueatDesignAirFlowRateAutosized() const;

    boost::optional<double> airFlowRateinFreeConvectionRegime() const;

    OSOptionalQuantity getAirFlowRateinFreeConvectionRegime(bool returnIP=false) const;

    bool isAirFlowRateinFreeConvectionRegimeDefaulted() const;

    bool isAirFlowRateinFreeConvectionRegimeAutosized() const;

    boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate() const;

    OSOptionalQuantity getUFactorTimesAreaValueatFreeConvectionAirFlowRate(bool returnIP=false) const;

    bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted() const;

    bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() const;

    std::string performanceInputMethod() const;

    bool isPerformanceInputMethodDefaulted() const;

    boost::optional<double> nominalCapacity() const;

    OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

    boost::optional<double> freeConvectionCapacity() const;

    OSOptionalQuantity getFreeConvectionCapacity(bool returnIP=false) const;

    double basinHeaterCapacity() const;

    Quantity getBasinHeaterCapacity(bool returnIP=false) const;

    bool isBasinHeaterCapacityDefaulted() const;

    double basinHeaterSetpointTemperature() const;

    Quantity getBasinHeaterSetpointTemperature(bool returnIP=false) const;

    bool isBasinHeaterSetpointTemperatureDefaulted() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    boost::optional<std::string> evaporationLossMode() const;

    double evaporationLossFactor() const;

    Quantity getEvaporationLossFactor(bool returnIP=false) const;

    bool isEvaporationLossFactorDefaulted() const;

    double driftLossPercent() const;

    Quantity getDriftLossPercent(bool returnIP=false) const;

    bool isDriftLossPercentDefaulted() const;

    boost::optional<std::string> blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    Quantity getBlowdownConcentrationRatio(bool returnIP=false) const;

    bool isBlowdownConcentrationRatioDefaulted() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    std::string capacityControl() const;

    bool isCapacityControlDefaulted() const;

    int numberofCells() const;

    bool isNumberofCellsDefaulted() const;

    std::string cellControl() const;

    bool isCellControlDefaulted() const;

    double cellMinimumWaterFlowRateFraction() const;

    Quantity getCellMinimumWaterFlowRateFraction(bool returnIP=false) const;

    bool isCellMinimumWaterFlowRateFractionDefaulted() const;

    double cellMaximumWaterFlowRateFraction() const;

    Quantity getCellMaximumWaterFlowRateFraction(bool returnIP=false) const;

    bool isCellMaximumWaterFlowRateFractionDefaulted() const;

    double sizingFactor() const;

    Quantity getSizingFactor(bool returnIP=false) const;

    bool isSizingFactorDefaulted() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const ;

    boost::optional<double> autosizedDesignAirFlowRate() const ;

    boost::optional<double> autosizedFanPoweratDesignAirFlowRate() const ;

    boost::optional<double> autosizedUFactorTimesAreaValueatDesignAirFlowRate() const ;

    boost::optional<double> autosizedAirFlowRateinFreeConvectionRegime() const ;

    boost::optional<double> autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate() const ;

    boost::optional<double> autosizedDesignApproachTemperature() const;

    boost::optional<double> autosizedDesignRangeTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    bool setDesignWaterFlowRate(const OSOptionalQuantity& designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

    bool setDesignAirFlowRate(const OSOptionalQuantity& designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate);

    bool setFanPoweratDesignAirFlowRate(const OSOptionalQuantity& fanPoweratDesignAirFlowRate);

    void autosizeFanPoweratDesignAirFlowRate();

    bool setUFactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate);

    bool setUFactorTimesAreaValueatDesignAirFlowRate(const OSOptionalQuantity& uFactorTimesAreaValueatDesignAirFlowRate);

    void resetUFactorTimesAreaValueatDesignAirFlowRate();

    void autosizeUFactorTimesAreaValueatDesignAirFlowRate();

    bool setAirFlowRateinFreeConvectionRegime(boost::optional<double> airFlowRateinFreeConvectionRegime);

    bool setAirFlowRateinFreeConvectionRegime(const OSOptionalQuantity& airFlowRateinFreeConvectionRegime);

    void resetAirFlowRateinFreeConvectionRegime();

    void autosizeAirFlowRateinFreeConvectionRegime();

    bool setUFactorTimesAreaValueatFreeConvectionAirFlowRate(boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate);

    bool setUFactorTimesAreaValueatFreeConvectionAirFlowRate(const OSOptionalQuantity& uFactorTimesAreaValueatFreeConvectionAirFlowRate);

    void resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();

    void autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    void resetPerformanceInputMethod();

    bool setNominalCapacity(boost::optional<double> nominalCapacity);

    bool setNominalCapacity(const OSOptionalQuantity& nominalCapacity);

    void resetNominalCapacity();

    bool setFreeConvectionCapacity(boost::optional<double> freeConvectionCapacity);

    bool setFreeConvectionCapacity(const OSOptionalQuantity& freeConvectionCapacity);

    void resetFreeConvectionCapacity();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterCapacity(const Quantity& basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterSetpointTemperature(const Quantity& basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    bool setBasinHeaterOperatingSchedule(Schedule& basinHeaterOperatingSchedule);

    void resetBasinHeaterOperatingSchedule();

    bool setEvaporationLossMode(boost::optional<std::string> evaporationLossMode);

    void resetEvaporationLossMode();

    bool setEvaporationLossFactor(double evaporationLossFactor);

    bool setEvaporationLossFactor(const Quantity& evaporationLossFactor);

    void resetEvaporationLossFactor();

    bool setDriftLossPercent(double driftLossPercent);

    bool setDriftLossPercent(const Quantity& driftLossPercent);

    void resetDriftLossPercent();

    bool setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode);

    void resetBlowdownCalculationMode();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    bool setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    bool setBlowdownMakeupWaterUsageSchedule(Schedule& blowdownMakeupWaterUsageSchedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    bool setCapacityControl(std::string capacityControl);

    void resetCapacityControl();

    bool setNumberofCells(int numberofCells);

    void resetNumberofCells();

    bool setCellControl(std::string cellControl);

    void resetCellControl();

    bool setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction);

    bool setCellMinimumWaterFlowRateFraction(const Quantity& cellMinimumWaterFlowRateFraction);

    void resetCellMinimumWaterFlowRateFraction();

    bool setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction);

    bool setCellMaximumWaterFlowRateFraction(const Quantity& cellMaximumWaterFlowRateFraction);

    void resetCellMaximumWaterFlowRateFraction();

    bool setSizingFactor(double sizingFactor);

    bool setSizingFactor(const Quantity& sizingFactor);

    void resetSizingFactor();

    bool addToNode(Node & node) override;

    double freeConvectionAirFlowRateSizingFactor() const;

    bool setFreeConvectionAirFlowRateSizingFactor(double freeConvectionAirFlowRateSizingFactor);

    double freeConvectionUFactorTimesAreaValueSizingFactor() const;

    bool setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor);

    double heatRejectionCapacityAndNominalCapacitySizingRatio() const;

    bool setHeatRejectionCapacityAndNominalCapacitySizingRatio(double heatRejectionCapacityAndNominalCapacitySizingRatio);

    double freeConvectionNominalCapacitySizingFactor() const;

    bool setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor);

    double designInletAirDryBulbTemperature() const;

    bool setDesignInletAirDryBulbTemperature(double designInletAirDryBulbTemperature);

    double designInletAirWetBulbTemperature() const;

    bool setDesignInletAirWetBulbTemperature(double designInletAirWetBulbTemperature);

    boost::optional<double> designApproachTemperature() const;

    bool isDesignApproachTemperatureAutosized() const;

    void autosizeDesignApproachTemperature();

    bool setDesignApproachTemperature(double designApproachTemperature);

    boost::optional<double> designRangeTemperature() const;

    bool isDesignRangeTemperatureAutosized() const;

    bool setDesignRangeTemperature(double designRangeTemperature);

    void autosizeDesignRangeTemperature();

    std::string endUseSubcategory() const;

    bool setEndUseSubcategory(const std::string & endUseSubcategory);

    //@}
   protected:
   private:

    REGISTER_LOGGER("openstudio.model.CoolingTowerSingleSpeed");

    openstudio::OSOptionalQuantity designWaterFlowRate_SI() const;
    openstudio::OSOptionalQuantity designWaterFlowRate_IP() const;
    openstudio::OSOptionalQuantity designAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity designAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity fanPoweratDesignAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity fanPoweratDesignAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity uFactorTimesAreaValueatDesignAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity uFactorTimesAreaValueatDesignAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity airFlowRateinFreeConvectionRegime_SI() const;
    openstudio::OSOptionalQuantity airFlowRateinFreeConvectionRegime_IP() const;
    openstudio::OSOptionalQuantity uFactorTimesAreaValueatFreeConvectionAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity uFactorTimesAreaValueatFreeConvectionAirFlowRate_IP() const;
    std::vector<std::string> performanceInputMethodValues() const;
    openstudio::OSOptionalQuantity nominalCapacity_SI() const;
    openstudio::OSOptionalQuantity nominalCapacity_IP() const;
    openstudio::OSOptionalQuantity freeConvectionCapacity_SI() const;
    openstudio::OSOptionalQuantity freeConvectionCapacity_IP() const;
    openstudio::Quantity basinHeaterCapacity_SI() const;
    openstudio::Quantity basinHeaterCapacity_IP() const;
    openstudio::Quantity basinHeaterSetpointTemperature_SI() const;
    openstudio::Quantity basinHeaterSetpointTemperature_IP() const;
    std::vector<std::string> evaporationLossModeValues() const;
    openstudio::Quantity evaporationLossFactor_SI() const;
    openstudio::Quantity evaporationLossFactor_IP() const;
    openstudio::Quantity driftLossPercent_SI() const;
    openstudio::Quantity driftLossPercent_IP() const;
    std::vector<std::string> blowdownCalculationModeValues() const;
    openstudio::Quantity blowdownConcentrationRatio_SI() const;
    openstudio::Quantity blowdownConcentrationRatio_IP() const;
    std::vector<std::string> capacityControlValues() const;
    std::vector<std::string> cellControlValues() const;
    openstudio::Quantity cellMinimumWaterFlowRateFraction_SI() const;
    openstudio::Quantity cellMinimumWaterFlowRateFraction_IP() const;
    openstudio::Quantity cellMaximumWaterFlowRateFraction_SI() const;
    openstudio::Quantity cellMaximumWaterFlowRateFraction_IP() const;
    openstudio::Quantity sizingFactor_SI() const;
    openstudio::Quantity sizingFactor_IP() const;

    boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;
    boost::optional<ModelObject> blowdownMakeupWaterUsageScheduleAsModelObject() const;

    bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model

} // openstudio

#endif // MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP
