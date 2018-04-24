/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

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

  boost::optional<double> autosizedRatedHighSpeedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedHighSpeedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedHighSpeedAirFlowRate() const ;

  boost::optional<double> autosizedRatedLowSpeedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedLowSpeedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedLowSpeedAirFlowRate() const ;

  boost::optional<double> autosizedHighSpeedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const ;

  boost::optional<double> autosizedLowSpeedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedHighSpeedTotalCoolingCapacity( boost::optional<double> value );

    bool setRatedHighSpeedTotalCoolingCapacity( double value );

    bool setRatedHighSpeedSensibleHeatRatio( boost::optional<double> value );

    bool setRatedHighSpeedSensibleHeatRatio( double value );

    bool setRatedHighSpeedCOP( double value );

    bool setRatedHighSpeedAirFlowRate( boost::optional<double> value );

    bool setRatedHighSpeedAirFlowRate( double value );

    bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve );

    bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve );

    bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve );

    bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve );

    bool setPartLoadFractionCorrelationCurve(const Curve& curve );

    bool setRatedLowSpeedTotalCoolingCapacity( boost::optional<double> value );

    bool setRatedLowSpeedTotalCoolingCapacity( double value );

    bool setRatedLowSpeedSensibleHeatRatio( boost::optional<double> value );

    bool setRatedLowSpeedSensibleHeatRatio( double value );

    bool setRatedLowSpeedCOP( double value );

    bool setRatedLowSpeedAirFlowRate( boost::optional<double> value );

    bool setRatedLowSpeedAirFlowRate( double value );

    bool setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve );

    bool setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve );

    bool setCondenserAirInletNodeName( const std::string& value );

    bool setCondenserType( const std::string& value );

    bool setHighSpeedEvaporativeCondenserEffectiveness( double value );

    bool setHighSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

    bool setHighSpeedEvaporativeCondenserAirFlowRate( double value );

    bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

    bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value );

    bool setLowSpeedEvaporativeCondenserEffectiveness( double value );

    bool setLowSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

    bool setLowSpeedEvaporativeCondenserAirFlowRate( double value );

    bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

    bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value );

    //TODO
    // A14, \field Supply Water Storage Tank Name
    // supplyWaterStorageTankName
    // setSupplyWaterStorageTankName

    //TODO
    // A15, \field Condensate Collection Water Storage Tank Name
    // getCondensateCollectionWaterStorageTankName
    // setCondensateCollectionWaterStorageTankName

    bool setBasinHeaterCapacity( double value );

    bool setBasinHeaterSetpointTemperature( double value );

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
