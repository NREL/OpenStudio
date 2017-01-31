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

#ifndef MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class Information;
class Schedule;

namespace detail {

  // derive CoilCoolingDXTwoSpeed_Impl from ModelObject_Impl to override virtual methods
  class MODEL_API CoilCoolingDXTwoSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    CoilCoolingDXTwoSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    CoilCoolingDXTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    // copy constructor
    CoilCoolingDXTwoSpeed_Impl(const CoilCoolingDXTwoSpeed_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~CoilCoolingDXTwoSpeed_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedHighSpeedTotalCoolingCapacity() const;

    boost::optional<double> ratedHighSpeedSensibleHeatRatio() const;

    boost::optional<double> ratedHighSpeedCOP() const;

    boost::optional<double> ratedHighSpeedAirFlowRate() const;

    Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

    Curve energyInputRatioFunctionOfTemperatureCurve()const;

    Curve energyInputRatioFunctionOfFlowFractionCurve()const;

    Curve partLoadFractionCorrelationCurve()const;

    boost::optional<double> ratedLowSpeedTotalCoolingCapacity() const;

    boost::optional<double> ratedLowSpeedSensibleHeatRatio() const;

    boost::optional<double> ratedLowSpeedCOP() const;

    boost::optional<double> ratedLowSpeedAirFlowRate() const;

    Curve lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const;

    Curve lowSpeedEnergyInputRatioFunctionOfTemperatureCurve()const;

    boost::optional<std::string> condenserAirInletNodeName()const;

    std::string condenserType()const;

    boost::optional<double> highSpeedEvaporativeCondenserEffectiveness()const;

    boost::optional<double> highSpeedEvaporativeCondenserAirFlowRate()const;

    boost::optional<double> highSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

    boost::optional<double> lowSpeedEvaporativeCondenserEffectiveness()const;

    boost::optional<double> lowSpeedEvaporativeCondenserAirFlowRate()const;

    boost::optional<double> lowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

    boost::optional<double> basinHeaterCapacity()const;

    boost::optional<double> basinHeaterSetpointTemperature()const;

    boost::optional<Schedule> basinHeaterOperatingSchedule()const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void setRatedHighSpeedTotalCoolingCapacity( boost::optional<double> value );

    void setRatedHighSpeedSensibleHeatRatio( boost::optional<double> value );

    void setRatedHighSpeedCOP( double value );

    void setRatedHighSpeedAirFlowRate( boost::optional<double> value );

    bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve );

    bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve );

    bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve );

    bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve );

    bool setPartLoadFractionCorrelationCurve(const Curve& curve );

    void setRatedLowSpeedTotalCoolingCapacity( boost::optional<double> value );

    void setRatedLowSpeedSensibleHeatRatio( boost::optional<double> value );

    void setRatedLowSpeedCOP( double value );

    void setRatedLowSpeedAirFlowRate( boost::optional<double> value );

    bool setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve );

    bool setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve );

    void setCondenserAirInletNodeName( const std::string& value );

    void setCondenserType( const std::string& value );

    void setHighSpeedEvaporativeCondenserEffectiveness( double value );

    void setHighSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

    void setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

    void setLowSpeedEvaporativeCondenserEffectiveness( double value );

    void setLowSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

    void setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

    //TODO
    // A14, \field Supply Water Storage Tank Name
    // supplyWaterStorageTankName
    // setSupplyWaterStorageTankName

    //TODO
    // A15, \field Condensate Collection Water Storage Tank Name
    // getCondensateCollectionWaterStorageTankName
    // setCondensateCollectionWaterStorageTankName

    void setBasinHeaterCapacity( double value );

    void setBasinHeaterSetpointTemperature( double value );

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool addToNode(Node & node) override;

    //@}
  private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoSpeed");

    // optional getters for children so can remove() if constructor fails
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionOfTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionOfFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    boost::optional<Curve> optionalLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve() const;
    boost::optional<Curve> optionalLowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP
