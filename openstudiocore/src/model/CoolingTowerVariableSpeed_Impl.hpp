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
  class MODEL_API CoolingTowerVariableSpeed_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    CoolingTowerVariableSpeed_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

    CoolingTowerVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    CoolingTowerVariableSpeed_Impl(const CoolingTowerVariableSpeed_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~CoolingTowerVariableSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

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

    boost::optional<std::string> evaporationLossMode() const;

    double evaporationLossFactor() const;

    bool isEvaporationLossFactorDefaulted() const;

    boost::optional<double> driftLossPercent() const;

    boost::optional<std::string> blowdownCalculationMode() const;

    boost::optional<double> blowdownConcentrationRatio() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    boost::optional<int> numberofCells() const;

    boost::optional<std::string> cellControl() const;

    boost::optional<double> cellMinimumWaterFlowRateFraction() const;

    boost::optional<double> cellMaximumWaterFlowRateFraction() const;

    boost::optional<double> sizingFactor() const;

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

    void setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(boost::optional<double> driftLossPercent);

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

    ModelObject clone(Model model) const;

    std::vector<IddObjectType> allowableChildTypes() const;

    std::vector<ModelObject> children() const;

    bool addToNode(Node & node);

   private:

    REGISTER_LOGGER("openstudio.model.CoolingTowerVariableSpeed");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERVARIABLESPEED_IMPL_HPP

