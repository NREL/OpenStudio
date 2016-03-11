/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
// class WaterStorageTank;

namespace detail {

  /** EvaporativeFluidCoolerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeFluidCoolerTwoSpeed.*/
  class MODEL_API EvaporativeFluidCoolerTwoSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EvaporativeFluidCoolerTwoSpeed_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    EvaporativeFluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    EvaporativeFluidCoolerTwoSpeed_Impl(const EvaporativeFluidCoolerTwoSpeed_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~EvaporativeFluidCoolerTwoSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutosized() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutosized() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    double designSprayWaterFlowRate() const;

    std::string performanceInputMethod() const;

    boost::optional<std::string> outdoorAirInletNodeName() const;

    double heatRejectionCapacityandNominalCapacitySizingRatio() const;

    boost::optional<double> highSpeedStandardDesignCapacity() const;

    boost::optional<double> lowSpeedStandardDesignCapacity() const;

    bool isLowSpeedStandardDesignCapacityAutosized() const;

    double lowSpeedStandardCapacitySizingFactor() const;

    boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

    bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

    bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highSpeedUserSpecifiedDesignCapacity() const;

    boost::optional<double> lowSpeedUserSpecifiedDesignCapacity() const;

    bool isLowSpeedUserSpecifiedDesignCapacityAutosized() const;

    double lowSpeedUserSpecifiedDesignCapacitySizingFactor() const;

    boost::optional<double> designEnteringWaterTemperature() const;

    boost::optional<double> designEnteringAirTemperature() const;

    boost::optional<double> designEnteringAirWetbulbTemperature() const;

    double highSpeedSizingFactor() const;

    std::string evaporationLossMode() const;

    boost::optional<double> evaporationLossFactor() const;

    double driftLossPercent() const;

    std::string blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

    void autosizeHighFanSpeedAirFlowRate();

    bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

    void autosizeHighFanSpeedFanPower();

    bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

    void autosizeLowFanSpeedAirFlowRate();

    void setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

    void autosizeLowFanSpeedFanPower();

    void setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

    bool setPerformanceInputMethod(std::string performanceInputMethod);

    void setOutdoorAirInletNodeName(boost::optional<std::string> outdoorAirInletNodeName);

    void resetOutdoorAirInletNodeName();

    void setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

    bool setHighSpeedStandardDesignCapacity(boost::optional<double> highSpeedStandardDesignCapacity);

    void resetHighSpeedStandardDesignCapacity();

    bool setLowSpeedStandardDesignCapacity(boost::optional<double> lowSpeedStandardDesignCapacity);

    void resetLowSpeedStandardDesignCapacity();

    void autosizeLowSpeedStandardDesignCapacity();

    void setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor);

    bool setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue);

    void resetHighFanSpeedUfactorTimesAreaValue();

    void autosizeHighFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue);

    void resetLowFanSpeedUfactorTimesAreaValue();

    void autosizeLowFanSpeedUfactorTimesAreaValue();

    void setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setHighSpeedUserSpecifiedDesignCapacity(boost::optional<double> highSpeedUserSpecifiedDesignCapacity);

    void resetHighSpeedUserSpecifiedDesignCapacity();

    bool setLowSpeedUserSpecifiedDesignCapacity(boost::optional<double> lowSpeedUserSpecifiedDesignCapacity);

    void resetLowSpeedUserSpecifiedDesignCapacity();

    void autosizeLowSpeedUserSpecifiedDesignCapacity();

    void setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor);

    bool setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature);

    void resetDesignEnteringWaterTemperature();

    bool setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature);

    void resetDesignEnteringAirWetbulbTemperature();

    bool setHighSpeedSizingFactor(double highSpeedSizingFactor);

    bool setEvaporationLossMode(std::string evaporationLossMode);

    void setEvaporationLossFactor(boost::optional<double> evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(double driftLossPercent);

    bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerTwoSpeed");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP

