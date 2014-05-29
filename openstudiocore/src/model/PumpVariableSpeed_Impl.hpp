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
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> ratedFlowRate READ ratedFlowRate WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedFlowRate_SI READ ratedFlowRate_SI WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedFlowRate_IP READ ratedFlowRate_IP WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(bool isRatedFlowRateDefaulted READ isRatedFlowRateDefaulted);
    Q_PROPERTY(bool isRatedFlowRateAutosized READ isRatedFlowRateAutosized);

    Q_PROPERTY(double ratedPumpHead READ ratedPumpHead WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(openstudio::Quantity ratedPumpHead_SI READ ratedPumpHead_SI WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(openstudio::Quantity ratedPumpHead_IP READ ratedPumpHead_IP WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(bool isRatedPumpHeadDefaulted READ isRatedPumpHeadDefaulted);

    Q_PROPERTY(boost::optional<double> ratedPowerConsumption READ ratedPowerConsumption WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedPowerConsumption_SI READ ratedPowerConsumption_SI WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedPowerConsumption_IP READ ratedPowerConsumption_IP WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(bool isRatedPowerConsumptionDefaulted READ isRatedPowerConsumptionDefaulted);
    Q_PROPERTY(bool isRatedPowerConsumptionAutosized READ isRatedPowerConsumptionAutosized);

    Q_PROPERTY(double motorEfficiency READ motorEfficiency WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(openstudio::Quantity motorEfficiency_SI READ motorEfficiency_SI WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(openstudio::Quantity motorEfficiency_IP READ motorEfficiency_IP WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(bool isMotorEfficiencyDefaulted READ isMotorEfficiencyDefaulted);

    Q_PROPERTY(double fractionofMotorInefficienciestoFluidStream READ fractionofMotorInefficienciestoFluidStream WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(openstudio::Quantity fractionofMotorInefficienciestoFluidStream_SI READ fractionofMotorInefficienciestoFluidStream_SI WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(openstudio::Quantity fractionofMotorInefficienciestoFluidStream_IP READ fractionofMotorInefficienciestoFluidStream_IP WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(bool isFractionofMotorInefficienciestoFluidStreamDefaulted READ isFractionofMotorInefficienciestoFluidStreamDefaulted);

    Q_PROPERTY(double coefficient1ofthePartLoadPerformanceCurve READ coefficient1ofthePartLoadPerformanceCurve WRITE setCoefficient1ofthePartLoadPerformanceCurve RESET resetCoefficient1ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient1ofthePartLoadPerformanceCurve_SI READ coefficient1ofthePartLoadPerformanceCurve_SI WRITE setCoefficient1ofthePartLoadPerformanceCurve RESET resetCoefficient1ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient1ofthePartLoadPerformanceCurve_IP READ coefficient1ofthePartLoadPerformanceCurve_IP WRITE setCoefficient1ofthePartLoadPerformanceCurve RESET resetCoefficient1ofthePartLoadPerformanceCurve);
    Q_PROPERTY(bool isCoefficient1ofthePartLoadPerformanceCurveDefaulted READ isCoefficient1ofthePartLoadPerformanceCurveDefaulted);

    Q_PROPERTY(double coefficient2ofthePartLoadPerformanceCurve READ coefficient2ofthePartLoadPerformanceCurve WRITE setCoefficient2ofthePartLoadPerformanceCurve RESET resetCoefficient2ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient2ofthePartLoadPerformanceCurve_SI READ coefficient2ofthePartLoadPerformanceCurve_SI WRITE setCoefficient2ofthePartLoadPerformanceCurve RESET resetCoefficient2ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient2ofthePartLoadPerformanceCurve_IP READ coefficient2ofthePartLoadPerformanceCurve_IP WRITE setCoefficient2ofthePartLoadPerformanceCurve RESET resetCoefficient2ofthePartLoadPerformanceCurve);
    Q_PROPERTY(bool isCoefficient2ofthePartLoadPerformanceCurveDefaulted READ isCoefficient2ofthePartLoadPerformanceCurveDefaulted);

    Q_PROPERTY(double coefficient3ofthePartLoadPerformanceCurve READ coefficient3ofthePartLoadPerformanceCurve WRITE setCoefficient3ofthePartLoadPerformanceCurve RESET resetCoefficient3ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient3ofthePartLoadPerformanceCurve_SI READ coefficient3ofthePartLoadPerformanceCurve_SI WRITE setCoefficient3ofthePartLoadPerformanceCurve RESET resetCoefficient3ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient3ofthePartLoadPerformanceCurve_IP READ coefficient3ofthePartLoadPerformanceCurve_IP WRITE setCoefficient3ofthePartLoadPerformanceCurve RESET resetCoefficient3ofthePartLoadPerformanceCurve);
    Q_PROPERTY(bool isCoefficient3ofthePartLoadPerformanceCurveDefaulted READ isCoefficient3ofthePartLoadPerformanceCurveDefaulted);

    Q_PROPERTY(double coefficient4ofthePartLoadPerformanceCurve READ coefficient4ofthePartLoadPerformanceCurve WRITE setCoefficient4ofthePartLoadPerformanceCurve RESET resetCoefficient4ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient4ofthePartLoadPerformanceCurve_SI READ coefficient4ofthePartLoadPerformanceCurve_SI WRITE setCoefficient4ofthePartLoadPerformanceCurve RESET resetCoefficient4ofthePartLoadPerformanceCurve);
    Q_PROPERTY(openstudio::Quantity coefficient4ofthePartLoadPerformanceCurve_IP READ coefficient4ofthePartLoadPerformanceCurve_IP WRITE setCoefficient4ofthePartLoadPerformanceCurve RESET resetCoefficient4ofthePartLoadPerformanceCurve);
    Q_PROPERTY(bool isCoefficient4ofthePartLoadPerformanceCurveDefaulted READ isCoefficient4ofthePartLoadPerformanceCurveDefaulted);

    Q_PROPERTY(double minimumFlowRate READ minimumFlowRate WRITE setMinimumFlowRate RESET resetMinimumFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumFlowRate_SI READ minimumFlowRate_SI WRITE setMinimumFlowRate RESET resetMinimumFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumFlowRate_IP READ minimumFlowRate_IP WRITE setMinimumFlowRate RESET resetMinimumFlowRate);
    Q_PROPERTY(bool isMinimumFlowRateDefaulted READ isMinimumFlowRateDefaulted);

    Q_PROPERTY(std::string pumpControlType READ pumpControlType WRITE setPumpControlType RESET resetPumpControlType);
    Q_PROPERTY(bool isPumpControlTypeDefaulted READ isPumpControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> pumpControlTypeValues READ pumpControlTypeValues);

    Q_PROPERTY(boost::optional<double> impellerDiameter READ impellerDiameter WRITE setImpellerDiameter RESET resetImpellerDiameter);
    Q_PROPERTY(openstudio::OSOptionalQuantity impellerDiameter_SI READ impellerDiameter_SI WRITE setImpellerDiameter RESET resetImpellerDiameter);
    Q_PROPERTY(openstudio::OSOptionalQuantity impellerDiameter_IP READ impellerDiameter_IP WRITE setImpellerDiameter RESET resetImpellerDiameter);

    Q_PROPERTY(boost::optional<std::string> vFDControlType READ vFDControlType WRITE setVFDControlType RESET resetVFDControlType);
    Q_PROPERTY(std::vector<std::string> vfdControlTypeValues READ vfdControlTypeValues);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> pumpFlowRateSchedule READ pumpFlowRateScheduleAsModelObject WRITE setPumpFlowRateScheduleAsModelObject RESET resetPumpFlowRateSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> pumpCurve READ pumpCurveAsModelObject WRITE setPumpCurveAsModelObject RESET resetPumpCurve);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> pumpRPMSchedule READ pumpRPMScheduleAsModelObject WRITE setPumpRPMScheduleAsModelObject RESET resetPumpRPMSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> minimumPressureSchedule READ minimumPressureScheduleAsModelObject WRITE setMinimumPressureScheduleAsModelObject RESET resetMinimumPressureSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> maximumPressureSchedule READ maximumPressureScheduleAsModelObject WRITE setMaximumPressureScheduleAsModelObject RESET resetMaximumPressureSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> minimumRPMSchedule READ minimumRPMScheduleAsModelObject WRITE setMinimumRPMScheduleAsModelObject RESET resetMinimumRPMSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> maximumRPMSchedule READ maximumRPMScheduleAsModelObject WRITE setMaximumRPMScheduleAsModelObject RESET resetMaximumRPMSchedule);
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

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

