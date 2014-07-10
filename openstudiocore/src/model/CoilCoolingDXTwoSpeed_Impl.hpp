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

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

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

    bool addToNode(Node & node);

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

    virtual boost::optional<HVACComponent> containingHVACComponent() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP
