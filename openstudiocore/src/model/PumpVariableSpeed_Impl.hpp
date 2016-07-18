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

#ifndef MODEL_PUMPVARIABLESPEED_IMPL_HPP
#define MODEL_PUMPVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  /** PumpVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for 
   *  PumpVariableSpeed.*/
  class MODEL_API PumpVariableSpeed_Impl : public StraightComponent_Impl {
    
    
    
    
    
    

    
    
    
    

    
    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    
    

    
    
    

    
    
    

    
    

    
    
    
    
    
    
    
   public:

    /** @name Constructors and Destructors */
    //@{

    PumpVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    PumpVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    PumpVariableSpeed_Impl(const PumpVariableSpeed_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~PumpVariableSpeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> ratedFlowRate() const;

    OSOptionalQuantity getRatedFlowRate(bool returnIP=false) const;

    bool isRatedFlowRateDefaulted() const;

    bool isRatedFlowRateAutosized() const;

    double ratedPumpHead() const;

    Quantity getRatedPumpHead(bool returnIP=false) const;

    bool isRatedPumpHeadDefaulted() const;

    boost::optional<double> ratedPowerConsumption() const;

    OSOptionalQuantity getRatedPowerConsumption(bool returnIP=false) const;

    bool isRatedPowerConsumptionDefaulted() const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    Quantity getMotorEfficiency(bool returnIP=false) const;

    bool isMotorEfficiencyDefaulted() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    Quantity getFractionofMotorInefficienciestoFluidStream(bool returnIP=false) const;

    bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

    double coefficient1ofthePartLoadPerformanceCurve() const;

    Quantity getCoefficient1ofthePartLoadPerformanceCurve(bool returnIP=false) const;

    bool isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient2ofthePartLoadPerformanceCurve() const;

    Quantity getCoefficient2ofthePartLoadPerformanceCurve(bool returnIP=false) const;

    bool isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient3ofthePartLoadPerformanceCurve() const;

    Quantity getCoefficient3ofthePartLoadPerformanceCurve(bool returnIP=false) const;

    bool isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient4ofthePartLoadPerformanceCurve() const;

    Quantity getCoefficient4ofthePartLoadPerformanceCurve(bool returnIP=false) const;

    bool isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const;

    double minimumFlowRate() const;

    Quantity getMinimumFlowRate(bool returnIP=false) const;

    bool isMinimumFlowRateDefaulted() const;

    std::string pumpControlType() const;

    bool isPumpControlTypeDefaulted() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<Curve> pumpCurve() const;

    boost::optional<double> impellerDiameter() const;

    OSOptionalQuantity getImpellerDiameter(bool returnIP=false) const;

    boost::optional<std::string> vFDControlType() const;

    boost::optional<Schedule> pumpRPMSchedule() const;

    boost::optional<Schedule> minimumPressureSchedule() const;

    boost::optional<Schedule> maximumPressureSchedule() const;

    boost::optional<Schedule> minimumRPMSchedule() const;

    boost::optional<Schedule> maximumRPMSchedule() const;

    //@}
    /** @name Setters */
    //@{

    void setRatedFlowRate(boost::optional<double> ratedFlowRate);

    bool setRatedFlowRate(const OSOptionalQuantity& ratedFlowRate);

    void resetRatedFlowRate();

    void autosizeRatedFlowRate();

    void setRatedPumpHead(double ratedPumpHead);

    bool setRatedPumpHead(const Quantity& ratedPumpHead);

    void resetRatedPumpHead();

    void setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

    bool setRatedPowerConsumption(const OSOptionalQuantity& ratedPowerConsumption);

    void resetRatedPowerConsumption();

    void autosizeRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    bool setMotorEfficiency(const Quantity& motorEfficiency);

    void resetMotorEfficiency();

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    bool setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    void setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

    bool setCoefficient1ofthePartLoadPerformanceCurve(const Quantity& coefficient1ofthePartLoadPerformanceCurve);

    void resetCoefficient1ofthePartLoadPerformanceCurve();

    void setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

    bool setCoefficient2ofthePartLoadPerformanceCurve(const Quantity& coefficient2ofthePartLoadPerformanceCurve);

    void resetCoefficient2ofthePartLoadPerformanceCurve();

    void setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

    bool setCoefficient3ofthePartLoadPerformanceCurve(const Quantity& coefficient3ofthePartLoadPerformanceCurve);

    void resetCoefficient3ofthePartLoadPerformanceCurve();

    void setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

    bool setCoefficient4ofthePartLoadPerformanceCurve(const Quantity& coefficient4ofthePartLoadPerformanceCurve);

    void resetCoefficient4ofthePartLoadPerformanceCurve();

    void setMinimumFlowRate(double minimumFlowRate);

    bool setMinimumFlowRate(const Quantity& minimumFlowRate);

    void resetMinimumFlowRate();

    bool setPumpControlType(std::string pumpControlType);

    void resetPumpControlType();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setPumpCurve(const Curve& curve);

    void resetPumpCurve();

    void setImpellerDiameter(boost::optional<double> impellerDiameter);

    bool setImpellerDiameter(const OSOptionalQuantity& impellerDiameter);

    void resetImpellerDiameter();

    bool setVFDControlType(boost::optional<std::string> vFDControlType);

    void resetVFDControlType();

    bool setPumpRPMSchedule(Schedule& schedule);

    void resetPumpRPMSchedule();

    bool setMinimumPressureSchedule(Schedule& schedule);

    void resetMinimumPressureSchedule();

    bool setMaximumPressureSchedule(Schedule& schedule);

    void resetMaximumPressureSchedule();

    bool setMinimumRPMSchedule(Schedule& schedule);

    void resetMinimumRPMSchedule();

    bool setMaximumRPMSchedule(Schedule& schedule);

    void resetMaximumRPMSchedule();

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.PumpVariableSpeed");

    openstudio::OSOptionalQuantity ratedFlowRate_SI() const;
    openstudio::OSOptionalQuantity ratedFlowRate_IP() const;
    openstudio::Quantity ratedPumpHead_SI() const;
    openstudio::Quantity ratedPumpHead_IP() const;
    openstudio::OSOptionalQuantity ratedPowerConsumption_SI() const;
    openstudio::OSOptionalQuantity ratedPowerConsumption_IP() const;
    openstudio::Quantity motorEfficiency_SI() const;
    openstudio::Quantity motorEfficiency_IP() const;
    openstudio::Quantity fractionofMotorInefficienciestoFluidStream_SI() const;
    openstudio::Quantity fractionofMotorInefficienciestoFluidStream_IP() const;
    openstudio::Quantity coefficient1ofthePartLoadPerformanceCurve_SI() const;
    openstudio::Quantity coefficient1ofthePartLoadPerformanceCurve_IP() const;
    openstudio::Quantity coefficient2ofthePartLoadPerformanceCurve_SI() const;
    openstudio::Quantity coefficient2ofthePartLoadPerformanceCurve_IP() const;
    openstudio::Quantity coefficient3ofthePartLoadPerformanceCurve_SI() const;
    openstudio::Quantity coefficient3ofthePartLoadPerformanceCurve_IP() const;
    openstudio::Quantity coefficient4ofthePartLoadPerformanceCurve_SI() const;
    openstudio::Quantity coefficient4ofthePartLoadPerformanceCurve_IP() const;
    openstudio::Quantity minimumFlowRate_SI() const;
    openstudio::Quantity minimumFlowRate_IP() const;
    std::vector<std::string> pumpControlTypeValues() const;
    openstudio::OSOptionalQuantity impellerDiameter_SI() const;
    openstudio::OSOptionalQuantity impellerDiameter_IP() const;
    std::vector<std::string> vfdControlTypeValues() const;

    boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;
    boost::optional<ModelObject> pumpCurveAsModelObject() const;
    boost::optional<ModelObject> pumpRPMScheduleAsModelObject() const;
    boost::optional<ModelObject> minimumPressureScheduleAsModelObject() const;
    boost::optional<ModelObject> maximumPressureScheduleAsModelObject() const;
    boost::optional<ModelObject> minimumRPMScheduleAsModelObject() const;
    boost::optional<ModelObject> maximumRPMScheduleAsModelObject() const;

    bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPumpRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setMinimumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setMaximumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setMinimumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setMaximumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PUMPVARIABLESPEED_IMPL_HPP

