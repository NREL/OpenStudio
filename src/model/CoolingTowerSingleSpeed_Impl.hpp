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

#ifndef MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP
#define MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    /** CoolingTowerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoolingTowerSingleSpeed.*/
    class MODEL_API CoolingTowerSingleSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoolingTowerSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoolingTowerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoolingTowerSingleSpeed_Impl(const CoolingTowerSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoolingTowerSingleSpeed_Impl() = default;

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

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> designAirFlowRate() const;

      bool isDesignAirFlowRateAutosized() const;

      boost::optional<double> fanPoweratDesignAirFlowRate() const;

      bool isFanPoweratDesignAirFlowRateAutosized() const;

      boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate() const;

      bool isUFactorTimesAreaValueatDesignAirFlowRateAutosized() const;

      boost::optional<double> airFlowRateinFreeConvectionRegime() const;

      bool isAirFlowRateinFreeConvectionRegimeDefaulted() const;

      bool isAirFlowRateinFreeConvectionRegimeAutosized() const;

      boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate() const;

      bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted() const;

      bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() const;

      std::string performanceInputMethod() const;

      bool isPerformanceInputMethodDefaulted() const;

      boost::optional<double> nominalCapacity() const;

      boost::optional<double> freeConvectionCapacity() const;

      double basinHeaterCapacity() const;

      bool isBasinHeaterCapacityDefaulted() const;

      double basinHeaterSetpointTemperature() const;

      bool isBasinHeaterSetpointTemperatureDefaulted() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      std::string evaporationLossMode() const;

      bool isEvaporationLossModeDefaulted() const;

      double evaporationLossFactor() const;

      bool isEvaporationLossFactorDefaulted() const;

      double driftLossPercent() const;

      bool isDriftLossPercentDefaulted() const;

      std::string blowdownCalculationMode() const;

      bool isBlowdownCalculationModeDefaulted() const;

      double blowdownConcentrationRatio() const;

      bool isBlowdownConcentrationRatioDefaulted() const;

      boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

      std::string capacityControl() const;

      bool isCapacityControlDefaulted() const;

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

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedFanPoweratDesignAirFlowRate() const;

      boost::optional<double> autosizedUFactorTimesAreaValueatDesignAirFlowRate() const;

      boost::optional<double> autosizedAirFlowRateinFreeConvectionRegime() const;

      boost::optional<double> autosizedUFactorTimesAreaValueatFreeConvectionAirFlowRate() const;

      boost::optional<double> autosizedDesignApproachTemperature() const;

      boost::optional<double> autosizedDesignRangeTemperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void resetDesignWaterFlowRate();

      void autosizeDesignWaterFlowRate();

      bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

      void autosizeDesignAirFlowRate();

      bool setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate);

      void autosizeFanPoweratDesignAirFlowRate();

      bool setUFactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate);

      void resetUFactorTimesAreaValueatDesignAirFlowRate();

      void autosizeUFactorTimesAreaValueatDesignAirFlowRate();

      bool setAirFlowRateinFreeConvectionRegime(boost::optional<double> airFlowRateinFreeConvectionRegime);

      void resetAirFlowRateinFreeConvectionRegime();

      void autosizeAirFlowRateinFreeConvectionRegime();

      bool setUFactorTimesAreaValueatFreeConvectionAirFlowRate(boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate);

      void resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();

      void autosizeUFactorTimesAreaValueatFreeConvectionAirFlowRate();

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      void resetPerformanceInputMethod();

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      bool setFreeConvectionCapacity(boost::optional<double> freeConvectionCapacity);

      void resetFreeConvectionCapacity();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      void resetBasinHeaterCapacity();

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      void resetBasinHeaterSetpointTemperature();

      bool setBasinHeaterOperatingSchedule(Schedule& basinHeaterOperatingSchedule);

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

      bool setBlowdownMakeupWaterUsageSchedule(Schedule& blowdownMakeupWaterUsageSchedule);

      void resetBlowdownMakeupWaterUsageSchedule();

      bool setCapacityControl(const std::string& capacityControl);

      void resetCapacityControl();

      bool setNumberofCells(int numberofCells);

      void resetNumberofCells();

      bool setCellControl(const std::string& cellControl);

      void resetCellControl();

      bool setCellMinimumWaterFlowRateFraction(double cellMinimumWaterFlowRateFraction);

      void resetCellMinimumWaterFlowRateFraction();

      bool setCellMaximumWaterFlowRateFraction(double cellMaximumWaterFlowRateFraction);

      void resetCellMaximumWaterFlowRateFraction();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool addToNode(Node& node) override;

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

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoolingTowerSingleSpeed");

      std::vector<std::string> performanceInputMethodValues() const;
      std::vector<std::string> evaporationLossModeValues() const;
      std::vector<std::string> blowdownCalculationModeValues() const;
      std::vector<std::string> capacityControlValues() const;
      std::vector<std::string> cellControlValues() const;
      boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;
      boost::optional<ModelObject> blowdownMakeupWaterUsageScheduleAsModelObject() const;

      bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COOLINGTOWERSINGLESPEED_IMPL_HPP
