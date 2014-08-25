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

#ifndef MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP
#define MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;
class Schedule;
class WaterStorageTank;
class Connection;

namespace detail {

  /** CoolingTowerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoolingTowerTwoSpeed.*/
  class MODEL_API CoolingTowerTwoSpeed_Impl : public StraightComponent_Impl {
    Q_OBJECT;
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection waterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection waterOutletNode() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> highFanSpeedUFactorTimesAreaValue() const;

    bool isHighFanSpeedUFactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutocalculated() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    bool isLowFanSpeedAirFlowRateSizingFactorDefaulted() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutocalculated() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    bool isLowFanSpeedFanPowerSizingFactorDefaulted() const;

    boost::optional<double> lowFanSpeedUFactorTimesAreaValue() const;

    bool isLowFanSpeedUFactorTimesAreaValueAutocalculated() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    bool isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted() const;

    boost::optional<double> freeConvectionRegimeAirFlowRate() const;

    bool isFreeConvectionRegimeAirFlowRateDefaulted() const;

    bool isFreeConvectionRegimeAirFlowRateAutocalculated() const;

    double freeConvectionRegimeAirFlowRateSizingFactor() const;

    bool isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted() const;

    boost::optional<double> freeConvectionRegimeUFactorTimesAreaValue() const;

    bool isFreeConvectionRegimeUFactorTimesAreaValueDefaulted() const;

    bool isFreeConvectionRegimeUFactorTimesAreaValueAutocalculated() const;

    double freeConvectionUFactorTimesAreaValueSizingFactor() const;

    bool isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted() const;

    std::string performanceInputMethod() const;

    bool isPerformanceInputMethodDefaulted() const;

    double heatRejectionCapacityandNominalCapacitySizingRatio() const;

    bool isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted() const;

    boost::optional<double> highSpeedNominalCapacity() const;

    boost::optional<double> lowSpeedNominalCapacity() const;

    bool isLowSpeedNominalCapacityAutocalculated() const;

    double lowSpeedNominalCapacitySizingFactor() const;

    bool isLowSpeedNominalCapacitySizingFactorDefaulted() const;

    boost::optional<double> freeConvectionNominalCapacity() const;

    bool isFreeConvectionNominalCapacityAutocalculated() const;

    double freeConvectionNominalCapacitySizingFactor() const;

    bool isFreeConvectionNominalCapacitySizingFactorDefaulted() const;

    double basinHeaterCapacity() const;

    bool isBasinHeaterCapacityDefaulted() const;

    double basinHeaterSetpointTemperature() const;

    bool isBasinHeaterSetpointTemperatureDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    boost::optional<std::string> evaporationLossMode() const;

    double evaporationLossFactor() const;

    bool isEvaporationLossFactorDefaulted() const;

    double driftLossPercent() const;

    bool isDriftLossPercentDefaulted() const;

    boost::optional<std::string> blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    bool isBlowdownConcentrationRatioDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
    boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> outdoorAirInletNode() const;

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

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setWaterInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setWaterOutletNode(const Connection& connection);

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

    void autocalculateLowFanSpeedAirFlowRate();

    bool setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    void resetLowFanSpeedAirFlowRateSizingFactor();

    bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

    void autocalculateLowFanSpeedFanPower();

    bool setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    void resetLowFanSpeedFanPowerSizingFactor();

    bool setLowFanSpeedUFactorTimesAreaValue(boost::optional<double> lowFanSpeedUFactorTimesAreaValue);

    void resetLowFanSpeedUFactorTimesAreaValue();

    void autocalculateLowFanSpeedUFactorTimesAreaValue();

    bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    void resetLowFanSpeedUFactorTimesAreaSizingFactor();

    bool setFreeConvectionRegimeAirFlowRate(boost::optional<double> freeConvectionRegimeAirFlowRate);

    void resetFreeConvectionRegimeAirFlowRate();

    void autocalculateFreeConvectionRegimeAirFlowRate();

    bool setFreeConvectionRegimeAirFlowRateSizingFactor(double freeConvectionRegimeAirFlowRateSizingFactor);

    void resetFreeConvectionRegimeAirFlowRateSizingFactor();

    bool setFreeConvectionRegimeUFactorTimesAreaValue(boost::optional<double> freeConvectionRegimeUFactorTimesAreaValue);

    void resetFreeConvectionRegimeUFactorTimesAreaValue();

    void autocalculateFreeConvectionRegimeUFactorTimesAreaValue();

    bool setFreeConvectionUFactorTimesAreaValueSizingFactor(double freeConvectionUFactorTimesAreaValueSizingFactor);

    void resetFreeConvectionUFactorTimesAreaValueSizingFactor();

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    void resetPerformanceInputMethod();

    void setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

    void resetHeatRejectionCapacityandNominalCapacitySizingRatio();

    bool setHighSpeedNominalCapacity(boost::optional<double> highSpeedNominalCapacity);

    void resetHighSpeedNominalCapacity();

    bool setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity);

    void resetLowSpeedNominalCapacity();

    void autocalculateLowSpeedNominalCapacity();

    bool setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

    void resetLowSpeedNominalCapacitySizingFactor();

    bool setFreeConvectionNominalCapacity(boost::optional<double> freeConvectionNominalCapacity);

    void resetFreeConvectionNominalCapacity();

    void autocalculateFreeConvectionNominalCapacity();

    bool setFreeConvectionNominalCapacitySizingFactor(double freeConvectionNominalCapacitySizingFactor);

    void resetFreeConvectionNominalCapacitySizingFactor();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setEvaporationLossMode(boost::optional<std::string> evaporationLossMode);

    void resetEvaporationLossMode();

    void setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(double driftLossPercent);

    void resetDriftLossPercent();

    bool setBlowdownCalculationMode(boost::optional<std::string> blowdownCalculationMode);

    void resetBlowdownCalculationMode();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    void resetSupplyWaterStorageTank();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOutdoorAirInletNode(const boost::optional<Connection>& connection);

    void resetOutdoorAirInletNode();

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

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoolingTowerTwoSpeed");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalWaterInletNode() const;
    boost::optional<Connection> optionalWaterOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERTWOSPEED_IMPL_HPP

