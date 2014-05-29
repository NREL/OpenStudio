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
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> designWaterFlowRate READ designWaterFlowRate WRITE setDesignWaterFlowRate RESET resetDesignWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity designWaterFlowRate_SI READ designWaterFlowRate_SI WRITE setDesignWaterFlowRate RESET resetDesignWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity designWaterFlowRate_IP READ designWaterFlowRate_IP WRITE setDesignWaterFlowRate RESET resetDesignWaterFlowRate);
    Q_PROPERTY(bool isDesignWaterFlowRateAutosized READ isDesignWaterFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> designAirFlowRate READ designAirFlowRate WRITE setDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity designAirFlowRate_SI READ designAirFlowRate_SI WRITE setDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity designAirFlowRate_IP READ designAirFlowRate_IP WRITE setDesignAirFlowRate);
    Q_PROPERTY(bool isDesignAirFlowRateAutosized READ isDesignAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> fanPoweratDesignAirFlowRate READ fanPoweratDesignAirFlowRate WRITE setFanPoweratDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity fanPoweratDesignAirFlowRate_SI READ fanPoweratDesignAirFlowRate_SI WRITE setFanPoweratDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity fanPoweratDesignAirFlowRate_IP READ fanPoweratDesignAirFlowRate_IP WRITE setFanPoweratDesignAirFlowRate);
    Q_PROPERTY(bool isFanPoweratDesignAirFlowRateAutosized READ isFanPoweratDesignAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> uFactorTimesAreaValueatDesignAirFlowRate READ uFactorTimesAreaValueatDesignAirFlowRate WRITE setUFactorTimesAreaValueatDesignAirFlowRate RESET resetUFactorTimesAreaValueatDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity uFactorTimesAreaValueatDesignAirFlowRate_SI READ uFactorTimesAreaValueatDesignAirFlowRate_SI WRITE setUFactorTimesAreaValueatDesignAirFlowRate RESET resetUFactorTimesAreaValueatDesignAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity uFactorTimesAreaValueatDesignAirFlowRate_IP READ uFactorTimesAreaValueatDesignAirFlowRate_IP WRITE setUFactorTimesAreaValueatDesignAirFlowRate RESET resetUFactorTimesAreaValueatDesignAirFlowRate);
    Q_PROPERTY(bool isUFactorTimesAreaValueatDesignAirFlowRateAutosized READ isUFactorTimesAreaValueatDesignAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> airFlowRateinFreeConvectionRegime READ airFlowRateinFreeConvectionRegime WRITE setAirFlowRateinFreeConvectionRegime RESET resetAirFlowRateinFreeConvectionRegime);
    Q_PROPERTY(openstudio::OSOptionalQuantity airFlowRateinFreeConvectionRegime_SI READ airFlowRateinFreeConvectionRegime_SI WRITE setAirFlowRateinFreeConvectionRegime RESET resetAirFlowRateinFreeConvectionRegime);
    Q_PROPERTY(openstudio::OSOptionalQuantity airFlowRateinFreeConvectionRegime_IP READ airFlowRateinFreeConvectionRegime_IP WRITE setAirFlowRateinFreeConvectionRegime RESET resetAirFlowRateinFreeConvectionRegime);
    Q_PROPERTY(bool isAirFlowRateinFreeConvectionRegimeDefaulted READ isAirFlowRateinFreeConvectionRegimeDefaulted);
    Q_PROPERTY(bool isAirFlowRateinFreeConvectionRegimeAutosized READ isAirFlowRateinFreeConvectionRegimeAutosized);

    Q_PROPERTY(boost::optional<double> uFactorTimesAreaValueatFreeConvectionAirFlowRate READ uFactorTimesAreaValueatFreeConvectionAirFlowRate WRITE setUFactorTimesAreaValueatFreeConvectionAirFlowRate RESET resetUFactorTimesAreaValueatFreeConvectionAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity uFactorTimesAreaValueatFreeConvectionAirFlowRate_SI READ uFactorTimesAreaValueatFreeConvectionAirFlowRate_SI WRITE setUFactorTimesAreaValueatFreeConvectionAirFlowRate RESET resetUFactorTimesAreaValueatFreeConvectionAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity uFactorTimesAreaValueatFreeConvectionAirFlowRate_IP READ uFactorTimesAreaValueatFreeConvectionAirFlowRate_IP WRITE setUFactorTimesAreaValueatFreeConvectionAirFlowRate RESET resetUFactorTimesAreaValueatFreeConvectionAirFlowRate);
    Q_PROPERTY(bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted READ isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted);
    Q_PROPERTY(bool isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized READ isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized);

    Q_PROPERTY(std::string performanceInputMethod READ performanceInputMethod WRITE setPerformanceInputMethod RESET resetPerformanceInputMethod);
    Q_PROPERTY(bool isPerformanceInputMethodDefaulted READ isPerformanceInputMethodDefaulted);
    Q_PROPERTY(std::vector<std::string> performanceInputMethodValues READ performanceInputMethodValues);

    Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_SI READ nominalCapacity_SI WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_IP READ nominalCapacity_IP WRITE setNominalCapacity RESET resetNominalCapacity);

    Q_PROPERTY(boost::optional<double> freeConvectionCapacity READ freeConvectionCapacity WRITE setFreeConvectionCapacity RESET resetFreeConvectionCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity freeConvectionCapacity_SI READ freeConvectionCapacity_SI WRITE setFreeConvectionCapacity RESET resetFreeConvectionCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity freeConvectionCapacity_IP READ freeConvectionCapacity_IP WRITE setFreeConvectionCapacity RESET resetFreeConvectionCapacity);

    Q_PROPERTY(double basinHeaterCapacity READ basinHeaterCapacity WRITE setBasinHeaterCapacity RESET resetBasinHeaterCapacity);
    Q_PROPERTY(openstudio::Quantity basinHeaterCapacity_SI READ basinHeaterCapacity_SI WRITE setBasinHeaterCapacity RESET resetBasinHeaterCapacity);
    Q_PROPERTY(openstudio::Quantity basinHeaterCapacity_IP READ basinHeaterCapacity_IP WRITE setBasinHeaterCapacity RESET resetBasinHeaterCapacity);
    Q_PROPERTY(bool isBasinHeaterCapacityDefaulted READ isBasinHeaterCapacityDefaulted);

    Q_PROPERTY(double basinHeaterSetpointTemperature READ basinHeaterSetpointTemperature WRITE setBasinHeaterSetpointTemperature RESET resetBasinHeaterSetpointTemperature);
    Q_PROPERTY(openstudio::Quantity basinHeaterSetpointTemperature_SI READ basinHeaterSetpointTemperature_SI WRITE setBasinHeaterSetpointTemperature RESET resetBasinHeaterSetpointTemperature);
    Q_PROPERTY(openstudio::Quantity basinHeaterSetpointTemperature_IP READ basinHeaterSetpointTemperature_IP WRITE setBasinHeaterSetpointTemperature RESET resetBasinHeaterSetpointTemperature);
    Q_PROPERTY(bool isBasinHeaterSetpointTemperatureDefaulted READ isBasinHeaterSetpointTemperatureDefaulted);

    Q_PROPERTY(boost::optional<std::string> evaporationLossMode READ evaporationLossMode WRITE setEvaporationLossMode RESET resetEvaporationLossMode);
    Q_PROPERTY(std::vector<std::string> evaporationLossModeValues READ evaporationLossModeValues);

    Q_PROPERTY(double evaporationLossFactor READ evaporationLossFactor WRITE setEvaporationLossFactor RESET resetEvaporationLossFactor);
    Q_PROPERTY(openstudio::Quantity evaporationLossFactor_SI READ evaporationLossFactor_SI WRITE setEvaporationLossFactor RESET resetEvaporationLossFactor);
    Q_PROPERTY(openstudio::Quantity evaporationLossFactor_IP READ evaporationLossFactor_IP WRITE setEvaporationLossFactor RESET resetEvaporationLossFactor);
    Q_PROPERTY(bool isEvaporationLossFactorDefaulted READ isEvaporationLossFactorDefaulted);

    Q_PROPERTY(double driftLossPercent READ driftLossPercent WRITE setDriftLossPercent RESET resetDriftLossPercent);
    Q_PROPERTY(openstudio::Quantity driftLossPercent_SI READ driftLossPercent_SI WRITE setDriftLossPercent RESET resetDriftLossPercent);
    Q_PROPERTY(openstudio::Quantity driftLossPercent_IP READ driftLossPercent_IP WRITE setDriftLossPercent RESET resetDriftLossPercent);
    Q_PROPERTY(bool isDriftLossPercentDefaulted READ isDriftLossPercentDefaulted);

    Q_PROPERTY(boost::optional<std::string> blowdownCalculationMode READ blowdownCalculationMode WRITE setBlowdownCalculationMode RESET resetBlowdownCalculationMode);
    Q_PROPERTY(std::vector<std::string> blowdownCalculationModeValues READ blowdownCalculationModeValues);

    Q_PROPERTY(double blowdownConcentrationRatio READ blowdownConcentrationRatio WRITE setBlowdownConcentrationRatio RESET resetBlowdownConcentrationRatio);
    Q_PROPERTY(openstudio::Quantity blowdownConcentrationRatio_SI READ blowdownConcentrationRatio_SI WRITE setBlowdownConcentrationRatio RESET resetBlowdownConcentrationRatio);
    Q_PROPERTY(openstudio::Quantity blowdownConcentrationRatio_IP READ blowdownConcentrationRatio_IP WRITE setBlowdownConcentrationRatio RESET resetBlowdownConcentrationRatio);
    Q_PROPERTY(bool isBlowdownConcentrationRatioDefaulted READ isBlowdownConcentrationRatioDefaulted);

    Q_PROPERTY(std::string capacityControl READ capacityControl WRITE setCapacityControl RESET resetCapacityControl);
    Q_PROPERTY(bool isCapacityControlDefaulted READ isCapacityControlDefaulted);
    Q_PROPERTY(std::vector<std::string> capacityControlValues READ capacityControlValues);

    Q_PROPERTY(int numberofCells READ numberofCells WRITE setNumberofCells RESET resetNumberofCells);
    Q_PROPERTY(bool isNumberofCellsDefaulted READ isNumberofCellsDefaulted);

    Q_PROPERTY(std::string cellControl READ cellControl WRITE setCellControl RESET resetCellControl);
    Q_PROPERTY(bool isCellControlDefaulted READ isCellControlDefaulted);
    Q_PROPERTY(std::vector<std::string> cellControlValues READ cellControlValues);

    Q_PROPERTY(double cellMinimumWaterFlowRateFraction READ cellMinimumWaterFlowRateFraction WRITE setCellMinimumWaterFlowRateFraction RESET resetCellMinimumWaterFlowRateFraction);
    Q_PROPERTY(openstudio::Quantity cellMinimumWaterFlowRateFraction_SI READ cellMinimumWaterFlowRateFraction_SI WRITE setCellMinimumWaterFlowRateFraction RESET resetCellMinimumWaterFlowRateFraction);
    Q_PROPERTY(openstudio::Quantity cellMinimumWaterFlowRateFraction_IP READ cellMinimumWaterFlowRateFraction_IP WRITE setCellMinimumWaterFlowRateFraction RESET resetCellMinimumWaterFlowRateFraction);
    Q_PROPERTY(bool isCellMinimumWaterFlowRateFractionDefaulted READ isCellMinimumWaterFlowRateFractionDefaulted);

    Q_PROPERTY(double cellMaximumWaterFlowRateFraction READ cellMaximumWaterFlowRateFraction WRITE setCellMaximumWaterFlowRateFraction RESET resetCellMaximumWaterFlowRateFraction);
    Q_PROPERTY(openstudio::Quantity cellMaximumWaterFlowRateFraction_SI READ cellMaximumWaterFlowRateFraction_SI WRITE setCellMaximumWaterFlowRateFraction RESET resetCellMaximumWaterFlowRateFraction);
    Q_PROPERTY(openstudio::Quantity cellMaximumWaterFlowRateFraction_IP READ cellMaximumWaterFlowRateFraction_IP WRITE setCellMaximumWaterFlowRateFraction RESET resetCellMaximumWaterFlowRateFraction);
    Q_PROPERTY(bool isCellMaximumWaterFlowRateFractionDefaulted READ isCellMaximumWaterFlowRateFractionDefaulted);

    Q_PROPERTY(double sizingFactor READ sizingFactor WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(openstudio::Quantity sizingFactor_SI READ sizingFactor_SI WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(openstudio::Quantity sizingFactor_IP READ sizingFactor_IP WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(bool isSizingFactorDefaulted READ isSizingFactorDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> basinHeaterOperatingSchedule READ basinHeaterOperatingScheduleAsModelObject WRITE setBasinHeaterOperatingScheduleAsModelObject RESET resetBasinHeaterOperatingSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> blowdownMakeupWaterUsageSchedule READ blowdownMakeupWaterUsageScheduleAsModelObject WRITE setBlowdownMakeupWaterUsageScheduleAsModelObject RESET resetBlowdownMakeupWaterUsageSchedule);

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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

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

    void setEvaporationLossFactor(double evaporationLossFactor);

    bool setEvaporationLossFactor(const Quantity& evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(double driftLossPercent);

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

    bool addToNode(Node & node);

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

