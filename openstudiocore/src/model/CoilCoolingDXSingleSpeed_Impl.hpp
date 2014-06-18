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

#ifndef MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class Information;
class Schedule;

namespace detail {

  // derive CoilCoolingDXSingleSpeed_Impl from ModelObject_Impl to override virtual methods
  class MODEL_API CoilCoolingDXSingleSpeed_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> ratedCOP READ ratedCOP WRITE setRatedCOP);
    Q_PROPERTY(boost::optional<double> ratedEvaporatorFanPowerPerVolumeFlowRate READ ratedEvaporatorFanPowerPerVolumeFlowRate WRITE setRatedEvaporatorFanPowerPerVolumeFlowRate);
    Q_PROPERTY(boost::optional<double> nominalTimeForCondensateRemovalToBegin READ nominalTimeForCondensateRemovalToBegin WRITE setNominalTimeForCondensateRemovalToBegin);
    Q_PROPERTY(boost::optional<double> ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity READ ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity WRITE setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity);
    Q_PROPERTY(boost::optional<double> maximumCyclingRate READ maximumCyclingRate WRITE setMaximumCyclingRate);
    Q_PROPERTY(boost::optional<double> latentCapacityTimeConstant READ latentCapacityTimeConstant WRITE setLatentCapacityTimeConstant);
    Q_PROPERTY(boost::optional<std::string> condenserAirInletNodeName READ condenserAirInletNodeName WRITE setCondenserAirInletNodeName);
    Q_PROPERTY(std::string condenserType READ condenserType WRITE setCondenserType);
    Q_PROPERTY(boost::optional<double> evaporativeCondenserEffectiveness READ evaporativeCondenserEffectiveness WRITE setEvaporativeCondenserEffectiveness);
    Q_PROPERTY(boost::optional<double> evaporativeCondenserAirFlowRate READ evaporativeCondenserAirFlowRate WRITE setEvaporativeCondenserAirFlowRate);
    Q_PROPERTY(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption READ evaporativeCondenserPumpRatedPowerConsumption WRITE setEvaporativeCondenserPumpRatedPowerConsumption);
    Q_PROPERTY(boost::optional<double> crankcaseHeaterCapacity READ crankcaseHeaterCapacity WRITE setCrankcaseHeaterCapacity);
    Q_PROPERTY(boost::optional<double> maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation READ maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation WRITE setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation);
    Q_PROPERTY(boost::optional<double> basinHeaterCapacity READ basinHeaterCapacity WRITE setBasinHeaterCapacity);
    Q_PROPERTY(boost::optional<double> basinHeaterSetpointTemperature READ basinHeaterSetpointTemperature WRITE setBasinHeaterSetpointTemperature);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> basinHeaterOperatingSchedule READ basinHeaterOperatingScheduleAsModelObject WRITE setBasinHeaterOperatingScheduleAsModelObject RESET resetBasinHeaterOperatingSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    CoilCoolingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    CoilCoolingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    // copy constructor
    CoilCoolingDXSingleSpeed_Impl(const CoilCoolingDXSingleSpeed_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~CoilCoolingDXSingleSpeed_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule()const;

    boost::optional<double> ratedCOP() const;

    boost::optional<double> ratedEvaporatorFanPowerPerVolumeFlowRate() const ;

    Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

    Curve energyInputRatioFunctionOfTemperatureCurve() const;

    Curve energyInputRatioFunctionOfFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    boost::optional<double> nominalTimeForCondensateRemovalToBegin() const;

    boost::optional<double> ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity()const;

    boost::optional<double> maximumCyclingRate();

    boost::optional<double> latentCapacityTimeConstant()const;

    boost::optional<std::string> condenserAirInletNodeName()const;

    std::string condenserType()const;

    boost::optional<double> evaporativeCondenserEffectiveness()const;

    boost::optional<double> evaporativeCondenserAirFlowRate()const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption()const;

    boost::optional<double> crankcaseHeaterCapacity()const;

    boost::optional<double> maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const;

    boost::optional<double> basinHeaterCapacity()const;

    boost::optional<double> basinHeaterSetpointTemperature()const;

    boost::optional<Schedule> basinHeaterOperatingSchedule()const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule );

    void setRatedCOP( boost::optional<double> value );

    void setRatedEvaporatorFanPowerPerVolumeFlowRate( boost::optional<double> value );

    void setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve );

    void setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve );

    void setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve );

    void setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve );

    void setPartLoadFractionCorrelationCurve(const Curve& curve );

    void setNominalTimeForCondensateRemovalToBegin( boost::optional<double> value );

    void setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( boost::optional<double> value );

    void setMaximumCyclingRate( boost::optional<double> value );

    void setLatentCapacityTimeConstant( boost::optional<double> value );

    void setCondenserAirInletNodeName( const boost::optional<std::string>& value );

    void setCondenserType( const std::string& value );

    void setEvaporativeCondenserEffectiveness( boost::optional<double> value );

    void setEvaporativeCondenserAirFlowRate( boost::optional<double> value );

    void setEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

    void setCrankcaseHeaterCapacity( boost::optional<double> value );

    void setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( boost::optional<double> value );

    void setBasinHeaterCapacity( boost::optional<double> value );

    void setBasinHeaterSetpointTemperature( boost::optional<double> value );

    bool setBasinHeaterOperatingSchedule(Schedule& schedule );

    void resetBasinHeaterOperatingSchedule();


    boost::optional<double> ratedTotalCoolingCapacity() const;

    OSOptionalQuantity getRatedTotalCoolingCapacity(bool returnIP=false) const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

    bool setRatedTotalCoolingCapacity(const OSOptionalQuantity& ratedTotalCoolingCapacity);

    void autosizeRatedTotalCoolingCapacity();


    boost::optional<double> ratedSensibleHeatRatio() const;

    OSOptionalQuantity getRatedSensibleHeatRatio(bool returnIP=false) const;

    bool isRatedSensibleHeatRatioAutosized() const;

    bool setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio);

    bool setRatedSensibleHeatRatio(const OSOptionalQuantity& ratedSensibleHeatRatio);

    void autosizeRatedSensibleHeatRatio();


    boost::optional<double> ratedAirFlowRate() const;

    OSOptionalQuantity getRatedAirFlowRate(bool returnIP=false) const;

    bool isRatedAirFlowRateAutosized() const;

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    bool setRatedAirFlowRate(const OSOptionalQuantity& ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool addToNode(Node & node);

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeed");

    // optional getters for use by children so can remove() this object if the constructor fails
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionOfTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionOfFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP
