/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

class Schedule;
//class WaterStorageTank;

namespace detail {

  /** EvaporativeFluidCoolerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeFluidCoolerSingleSpeed.*/
  class MODEL_API EvaporativeFluidCoolerSingleSpeed_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> designAirFlowRate READ designAirFlowRate WRITE setDesignAirFlowRate);
    Q_PROPERTY(bool isDesignAirFlowRateAutosized READ isDesignAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> fanPoweratDesignAirFlowRate READ fanPoweratDesignAirFlowRate WRITE setFanPoweratDesignAirFlowRate);
    Q_PROPERTY(bool isFanPoweratDesignAirFlowRateAutosized READ isFanPoweratDesignAirFlowRateAutosized);

    Q_PROPERTY(double designSprayWaterFlowRate READ designSprayWaterFlowRate WRITE setDesignSprayWaterFlowRate);

    Q_PROPERTY(boost::optional<std::string> performanceInputMethod READ performanceInputMethod WRITE setPerformanceInputMethod RESET resetPerformanceInputMethod);
    Q_PROPERTY(std::vector<std::string> performanceInputMethodValues READ performanceInputMethodValues);

    Q_PROPERTY(boost::optional<double> standardDesignCapacity READ standardDesignCapacity WRITE setStandardDesignCapacity RESET resetStandardDesignCapacity);

    Q_PROPERTY(boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate READ ufactorTimesAreaValueatDesignAirFlowRate WRITE setUfactorTimesAreaValueatDesignAirFlowRate RESET resetUfactorTimesAreaValueatDesignAirFlowRate);
    Q_PROPERTY(bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized READ isUfactorTimesAreaValueatDesignAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> designWaterFlowRate READ designWaterFlowRate WRITE setDesignWaterFlowRate RESET resetDesignWaterFlowRate);
    Q_PROPERTY(bool isDesignWaterFlowRateAutosized READ isDesignWaterFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> userSpecifiedDesignCapacity READ userSpecifiedDesignCapacity WRITE setUserSpecifiedDesignCapacity RESET resetUserSpecifiedDesignCapacity);
    Q_PROPERTY(boost::optional<double> designEnteringWaterTemperature READ designEnteringWaterTemperature WRITE setDesignEnteringWaterTemperature RESET resetDesignEnteringWaterTemperature);

    Q_PROPERTY(boost::optional<double> designEnteringAirTemperature READ designEnteringAirTemperature WRITE setDesignEnteringAirTemperature RESET resetDesignEnteringAirTemperature);

    Q_PROPERTY(boost::optional<double> designEnteringAirWetbulbTemperature READ designEnteringAirWetbulbTemperature WRITE setDesignEnteringAirWetbulbTemperature RESET resetDesignEnteringAirWetbulbTemperature);

    Q_PROPERTY(std::string capacityControl READ capacityControl WRITE setCapacityControl RESET resetCapacityControl);
    Q_PROPERTY(bool isCapacityControlDefaulted READ isCapacityControlDefaulted);
    Q_PROPERTY(std::vector<std::string> capacityControlValues READ capacityControlValues);

    Q_PROPERTY(double sizingFactor READ sizingFactor WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(bool isSizingFactorDefaulted READ isSizingFactorDefaulted);

    Q_PROPERTY(std::string evaporationLossMode READ evaporationLossMode WRITE setEvaporationLossMode RESET resetEvaporationLossMode);
    Q_PROPERTY(bool isEvaporationLossModeDefaulted READ isEvaporationLossModeDefaulted);
    Q_PROPERTY(std::vector<std::string> evaporationLossModeValues READ evaporationLossModeValues);

    Q_PROPERTY(boost::optional<double> evaporationLossFactor READ evaporationLossFactor WRITE setEvaporationLossFactor RESET resetEvaporationLossFactor);

    Q_PROPERTY(double driftLossPercent READ driftLossPercent WRITE setDriftLossPercent RESET resetDriftLossPercent);
    Q_PROPERTY(bool isDriftLossPercentDefaulted READ isDriftLossPercentDefaulted);

    Q_PROPERTY(std::string blowdownCalculationMode READ blowdownCalculationMode WRITE setBlowdownCalculationMode RESET resetBlowdownCalculationMode);
    Q_PROPERTY(bool isBlowdownCalculationModeDefaulted READ isBlowdownCalculationModeDefaulted);
    Q_PROPERTY(std::vector<std::string> blowdownCalculationModeValues READ blowdownCalculationModeValues);

    Q_PROPERTY(double blowdownConcentrationRatio READ blowdownConcentrationRatio WRITE setBlowdownConcentrationRatio RESET resetBlowdownConcentrationRatio);
    Q_PROPERTY(bool isBlowdownConcentrationRatioDefaulted READ isBlowdownConcentrationRatioDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
//    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> waterInletNode READ waterInletNodeAsModelObject WRITE setWaterInletNodeAsModelObject);
//    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> waterOutletNode READ waterOutletNodeAsModelObject WRITE setWaterOutletNodeAsModelObject);
//    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> outdoorAirInletNode READ outdoorAirInletNodeAsModelObject WRITE setOutdoorAirInletNodeAsModelObject RESET resetOutdoorAirInletNode);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> blowdownMakeupWaterUsageSchedule READ blowdownMakeupWaterUsageScheduleAsModelObject WRITE setBlowdownMakeupWaterUsageScheduleAsModelObject RESET resetBlowdownMakeupWaterUsageSchedule);
 //   Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyWaterStorageTank READ supplyWaterStorageTankAsModelObject WRITE setSupplyWaterStorageTankAsModelObject RESET resetSupplyWaterStorageTank);
   public:
    /** @name Constructors and Destructors */
    //@{

    EvaporativeFluidCoolerSingleSpeed_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    EvaporativeFluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    EvaporativeFluidCoolerSingleSpeed_Impl(const EvaporativeFluidCoolerSingleSpeed_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~EvaporativeFluidCoolerSingleSpeed_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);
    
    //@}
    /** @name Getters */
    //@{
    boost::optional<double> designAirFlowRate() const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> fanPoweratDesignAirFlowRate() const;

    bool isFanPoweratDesignAirFlowRateAutosized() const;

    double designSprayWaterFlowRate() const;

    boost::optional<std::string> performanceInputMethod() const;

    boost::optional<double> standardDesignCapacity() const;

    boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate() const;

    bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> userSpecifiedDesignCapacity() const;

    boost::optional<double> designEnteringWaterTemperature() const;

    boost::optional<double> designEnteringAirTemperature() const;

    boost::optional<double> designEnteringAirWetbulbTemperature() const;

    std::string capacityControl() const;

    bool isCapacityControlDefaulted() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    std::string evaporationLossMode() const;

    bool isEvaporationLossModeDefaulted() const;

    boost::optional<double> evaporationLossFactor() const;

    double driftLossPercent() const;

    bool isDriftLossPercentDefaulted() const;

    std::string blowdownCalculationMode() const;

    bool isBlowdownCalculationModeDefaulted() const;

    double blowdownConcentrationRatio() const;

    bool isBlowdownConcentrationRatioDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
   // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate);

    void autosizeFanPoweratDesignAirFlowRate();

    bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

    bool setPerformanceInputMethod(boost::optional<std::string> performanceInputMethod);

    void resetPerformanceInputMethod();

    void resetOutdoorAirInletNode();

    bool setStandardDesignCapacity(boost::optional<double> standardDesignCapacity);

    void resetStandardDesignCapacity();

    bool setUfactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate);

    void resetUfactorTimesAreaValueatDesignAirFlowRate();

    void autosizeUfactorTimesAreaValueatDesignAirFlowRate();

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setUserSpecifiedDesignCapacity(boost::optional<double> userSpecifiedDesignCapacity);

    void resetUserSpecifiedDesignCapacity();

    bool setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature);

    void resetDesignEnteringWaterTemperature();

    bool setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature);

    void resetDesignEnteringAirWetbulbTemperature();

    bool setCapacityControl(std::string capacityControl);

    void resetCapacityControl();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool setEvaporationLossMode(std::string evaporationLossMode);

    void resetEvaporationLossMode();

    void setEvaporationLossFactor(boost::optional<double> evaporationLossFactor);

    void resetEvaporationLossFactor();

    void setDriftLossPercent(double driftLossPercent);

    void resetDriftLossPercent();

    bool setBlowdownCalculationMode(std::string blowdownCalculationMode);

    void resetBlowdownCalculationMode();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
    //bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    //void resetSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerSingleSpeed");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals

    std::vector<std::string> performanceInputMethodValues() const;
    std::vector<std::string> capacityControlValues() const;
    std::vector<std::string> evaporationLossModeValues() const;
    std::vector<std::string> blowdownCalculationModeValues() const;

    //boost::optional<ModelObject> waterInletNodeAsModelObject() const;
    //boost::optional<ModelObject> waterOutletNodeAsModelObject() const;
    //boost::optional<ModelObject> outdoorAirInletNodeAsModelObject() const;
    boost::optional<ModelObject> blowdownMakeupWaterUsageScheduleAsModelObject() const;
    //boost::optional<ModelObject> supplyWaterStorageTankAsModelObject() const;

    //bool setWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setOutdoorAirInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    //bool setSupplyWaterStorageTankAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP

