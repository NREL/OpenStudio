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

#ifndef MODEL_COOLINGTOWERVARIABLESPEED_IMPL_HPP
#define MODEL_COOLINGTOWERVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class CurveCubic;
  class Schedule;

  namespace detail {

    /** CoolingTowerVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoolingTowerVariableSpeed.*/
    class MODEL_API CoolingTowerVariableSpeed_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoolingTowerVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoolingTowerVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoolingTowerVariableSpeed_Impl(const CoolingTowerVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoolingTowerVariableSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> modelType() const;

      boost::optional<ModelObject> modelCoefficient() const;

      boost::optional<double> designInletAirWetBulbTemperature() const;

      boost::optional<double> designApproachTemperature() const;

      boost::optional<double> designRangeTemperature() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> designAirFlowRate() const;

      bool isDesignAirFlowRateAutosized() const;

      boost::optional<double> designFanPower() const;

      bool isDesignFanPowerAutosized() const;

      boost::optional<CurveCubic> fanPowerRatioFunctionofAirFlowRateRatioCurve() const;

      boost::optional<double> minimumAirFlowRateRatio() const;

      boost::optional<double> fractionofTowerCapacityinFreeConvectionRegime() const;

      boost::optional<double> basinHeaterCapacity() const;

      boost::optional<double> basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      std::string evaporationLossMode() const;

      bool isEvaporationLossModeDefaulted() const;

      double evaporationLossFactor() const;

      bool isEvaporationLossFactorDefaulted() const;

      boost::optional<double> driftLossPercent() const;

      std::string blowdownCalculationMode() const;

      bool isBlowdownCalculationModeDefaulted() const;

      boost::optional<double> blowdownConcentrationRatio() const;

      boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

      boost::optional<int> numberofCells() const;

      std::string cellControl() const;

      bool isCellControlDefaulted() const;

      boost::optional<double> cellMinimumWaterFlowRateFraction() const;

      boost::optional<double> cellMaximumWaterFlowRateFraction() const;

      boost::optional<double> sizingFactor() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedDesignFanPower() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setModelType(boost::optional<std::string> modelType);

      void resetModelType();

      bool setModelCoefficient(const boost::optional<ModelObject>& variableSpeedTowerCoefficient);

      void resetModelCoefficient();

      bool setDesignInletAirWetBulbTemperature(boost::optional<double> designInletAirWetBulbTemperature);

      void resetDesignInletAirWetBulbTemperature();

      bool setDesignApproachTemperature(boost::optional<double> designApproachTemperature);

      void resetDesignApproachTemperature();

      bool setDesignRangeTemperature(boost::optional<double> designRangeTemperature);

      void resetDesignRangeTemperature();

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void autosizeDesignWaterFlowRate();

      bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

      void autosizeDesignAirFlowRate();

      bool setDesignFanPower(boost::optional<double> designFanPower);

      void autosizeDesignFanPower();

      bool setFanPowerRatioFunctionofAirFlowRateRatioCurve(const boost::optional<CurveCubic>& curve);

      void resetFanPowerRatioFunctionofAirFlowRateRatioCurve();

      bool setMinimumAirFlowRateRatio(boost::optional<double> minimumAirFlowRateRatio);

      void resetMinimumAirFlowRateRatio();

      bool setFractionofTowerCapacityinFreeConvectionRegime(boost::optional<double> fractionofTowerCapacityinFreeConvectionRegime);

      void resetFractionofTowerCapacityinFreeConvectionRegime();

      bool setBasinHeaterCapacity(boost::optional<double> basinHeaterCapacity);

      void resetBasinHeaterCapacity();

      bool setBasinHeaterSetpointTemperature(boost::optional<double> basinHeaterSetpointTemperature);

      void resetBasinHeaterSetpointTemperature();

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setEvaporationLossMode(boost::optional<std::string> evaporationLossMode);

      void resetEvaporationLossMode();

      bool setEvaporationLossFactor(double evaporationLossFactor);

      void resetEvaporationLossFactor();

      bool setDriftLossPercent(boost::optional<double> driftLossPercent);

      void resetDriftLossPercent();

      bool setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode);

      void resetBlowdownCalculationMode();

      bool setBlowdownConcentrationRatio(boost::optional<double> blowdownConcentrationRatio);

      void resetBlowdownConcentrationRatio();

      bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

      void resetBlowdownMakeupWaterUsageSchedule();

      bool setNumberofCells(boost::optional<int> numberofCells);

      void resetNumberofCells();

      bool setCellControl(boost::optional<std::string> cellControl);

      void resetCellControl();

      bool setCellMinimumWaterFlowRateFraction(boost::optional<double> cellMinimumWaterFlowRateFraction);

      void resetCellMinimumWaterFlowRateFraction();

      bool setCellMaximumWaterFlowRateFraction(boost::optional<double> cellMaximumWaterFlowRateFraction);

      void resetCellMaximumWaterFlowRateFraction();

      bool setSizingFactor(boost::optional<double> sizingFactor);

      void resetSizingFactor();

      ModelObject clone(Model model) const override;

      std::vector<IddObjectType> allowableChildTypes() const override;

      std::vector<ModelObject> children() const override;

      bool addToNode(Node& node) override;

      std::string endUseSubcategory() const;

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

     private:
      REGISTER_LOGGER("openstudio.model.CoolingTowerVariableSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COOLINGTOWERVARIABLESPEED_IMPL_HPP
