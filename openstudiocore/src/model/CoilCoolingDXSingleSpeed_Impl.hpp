/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    virtual ModelObject clone(Model model) const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

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

    bool addToNode(Node & node) override;

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
