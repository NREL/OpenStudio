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

#ifndef MODEL_PUMPCONSTANTSPEED_IMPL_HPP
#define MODEL_PUMPCONSTANTSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Connection;
class Schedule;
class Curve;
class ThermalZone;

namespace detail {

  /** PumpConstantSpeed_Impl is a StraightComponent_Impl that is the implementation class for PumpConstantSpeed.*/
  class MODEL_API PumpConstantSpeed_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> ratedFlowRate READ ratedFlowRate WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedFlowRate_SI READ ratedFlowRate_SI WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedFlowRate_IP READ ratedFlowRate_IP WRITE setRatedFlowRate RESET resetRatedFlowRate);
    Q_PROPERTY(bool isRatedFlowRateAutosized READ isRatedFlowRateAutosized);

    Q_PROPERTY(double ratedPumpHead READ ratedPumpHead WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(openstudio::Quantity ratedPumpHead_SI READ ratedPumpHead_SI WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(openstudio::Quantity ratedPumpHead_IP READ ratedPumpHead_IP WRITE setRatedPumpHead RESET resetRatedPumpHead);
    Q_PROPERTY(bool isRatedPumpHeadDefaulted READ isRatedPumpHeadDefaulted);

    Q_PROPERTY(boost::optional<double> ratedPowerConsumption READ ratedPowerConsumption WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedPowerConsumption_SI READ ratedPowerConsumption_SI WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(openstudio::OSOptionalQuantity ratedPowerConsumption_IP READ ratedPowerConsumption_IP WRITE setRatedPowerConsumption RESET resetRatedPowerConsumption);
    Q_PROPERTY(bool isRatedPowerConsumptionAutosized READ isRatedPowerConsumptionAutosized);

    Q_PROPERTY(double motorEfficiency READ motorEfficiency WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(openstudio::Quantity motorEfficiency_SI READ motorEfficiency_SI WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(openstudio::Quantity motorEfficiency_IP READ motorEfficiency_IP WRITE setMotorEfficiency RESET resetMotorEfficiency);
    Q_PROPERTY(bool isMotorEfficiencyDefaulted READ isMotorEfficiencyDefaulted);

    Q_PROPERTY(double fractionofMotorInefficienciestoFluidStream READ fractionofMotorInefficienciestoFluidStream WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(openstudio::Quantity fractionofMotorInefficienciestoFluidStream_SI READ fractionofMotorInefficienciestoFluidStream_SI WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(openstudio::Quantity fractionofMotorInefficienciestoFluidStream_IP READ fractionofMotorInefficienciestoFluidStream_IP WRITE setFractionofMotorInefficienciestoFluidStream RESET resetFractionofMotorInefficienciestoFluidStream);
    Q_PROPERTY(bool isFractionofMotorInefficienciestoFluidStreamDefaulted READ isFractionofMotorInefficienciestoFluidStreamDefaulted);

    Q_PROPERTY(std::string pumpControlType READ pumpControlType WRITE setPumpControlType RESET resetPumpControlType);
    Q_PROPERTY(bool isPumpControlTypeDefaulted READ isPumpControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> pumpControlTypeValues READ pumpControlTypeValues);

    Q_PROPERTY(boost::optional<double> impellerDiameter READ impellerDiameter WRITE setImpellerDiameter RESET resetImpellerDiameter);
    Q_PROPERTY(openstudio::OSOptionalQuantity impellerDiameter_SI READ impellerDiameter_SI WRITE setImpellerDiameter RESET resetImpellerDiameter);
    Q_PROPERTY(openstudio::OSOptionalQuantity impellerDiameter_IP READ impellerDiameter_IP WRITE setImpellerDiameter RESET resetImpellerDiameter);

    Q_PROPERTY(boost::optional<double> rotationalSpeed READ rotationalSpeed WRITE setRotationalSpeed RESET resetRotationalSpeed);
    Q_PROPERTY(openstudio::OSOptionalQuantity rotationalSpeed_SI READ rotationalSpeed_SI WRITE setRotationalSpeed RESET resetRotationalSpeed);
    Q_PROPERTY(openstudio::OSOptionalQuantity rotationalSpeed_IP READ rotationalSpeed_IP WRITE setRotationalSpeed RESET resetRotationalSpeed);

    Q_PROPERTY(boost::optional<double> skinLossRadiativeFraction READ skinLossRadiativeFraction WRITE setSkinLossRadiativeFraction RESET resetSkinLossRadiativeFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity skinLossRadiativeFraction_SI READ skinLossRadiativeFraction_SI WRITE setSkinLossRadiativeFraction RESET resetSkinLossRadiativeFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity skinLossRadiativeFraction_IP READ skinLossRadiativeFraction_IP WRITE setSkinLossRadiativeFraction RESET resetSkinLossRadiativeFraction);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> pumpFlowRateSchedule READ pumpFlowRateScheduleAsModelObject WRITE setPumpFlowRateScheduleAsModelObject RESET resetPumpFlowRateSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> pumpCurve READ pumpCurveAsModelObject WRITE setPumpCurveAsModelObject RESET resetPumpCurve);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> zone READ zoneAsModelObject WRITE setZoneAsModelObject RESET resetZone);
   public:

    PumpConstantSpeed_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    PumpConstantSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    PumpConstantSpeed_Impl(const PumpConstantSpeed_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~PumpConstantSpeed_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    boost::optional<double> ratedFlowRate() const;

    OSOptionalQuantity getRatedFlowRate(bool returnIP=false) const;

    bool isRatedFlowRateAutosized() const;

    double ratedPumpHead() const;

    Quantity getRatedPumpHead(bool returnIP=false) const;

    bool isRatedPumpHeadDefaulted() const;

    boost::optional<double> ratedPowerConsumption() const;

    OSOptionalQuantity getRatedPowerConsumption(bool returnIP=false) const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    Quantity getMotorEfficiency(bool returnIP=false) const;

    bool isMotorEfficiencyDefaulted() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    Quantity getFractionofMotorInefficienciestoFluidStream(bool returnIP=false) const;

    bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

    std::string pumpControlType() const;

    bool isPumpControlTypeDefaulted() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<Curve> pumpCurve() const;

    boost::optional<double> impellerDiameter() const;

    OSOptionalQuantity getImpellerDiameter(bool returnIP=false) const;

    boost::optional<double> rotationalSpeed() const;

    OSOptionalQuantity getRotationalSpeed(bool returnIP=false) const;

    boost::optional<ThermalZone> zone() const;

    boost::optional<double> skinLossRadiativeFraction() const;

    OSOptionalQuantity getSkinLossRadiativeFraction(bool returnIP=false) const;

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

    bool setPumpControlType(std::string pumpControlType);

    void resetPumpControlType();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setPumpCurve(const boost::optional<Curve>& curve);

    void resetPumpCurve();

    void setImpellerDiameter(boost::optional<double> impellerDiameter);

    bool setImpellerDiameter(const OSOptionalQuantity& impellerDiameter);

    void resetImpellerDiameter();

    void setRotationalSpeed(boost::optional<double> rotationalSpeed);

    bool setRotationalSpeed(const OSOptionalQuantity& rotationalSpeed);

    void resetRotationalSpeed();

    bool setZone(const boost::optional<ThermalZone>& thermalZone);

    void resetZone();

    bool setSkinLossRadiativeFraction(boost::optional<double> skinLossRadiativeFraction);

    bool setSkinLossRadiativeFraction(const OSOptionalQuantity& skinLossRadiativeFraction);

    void resetSkinLossRadiativeFraction();

    unsigned inletPort();

    unsigned outletPort();

    bool addToNode(Node & node);

   protected:

   private:

    REGISTER_LOGGER("openstudio.model.PumpConstantSpeed");

    boost::optional<Connection> optionalInletNode() const;
    boost::optional<Connection> optionalOutletNode() const;

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
    std::vector<std::string> pumpControlTypeValues() const;
    openstudio::OSOptionalQuantity impellerDiameter_SI() const;
    openstudio::OSOptionalQuantity impellerDiameter_IP() const;
    openstudio::OSOptionalQuantity rotationalSpeed_SI() const;
    openstudio::OSOptionalQuantity rotationalSpeed_IP() const;
    openstudio::OSOptionalQuantity skinLossRadiativeFraction_SI() const;
    openstudio::OSOptionalQuantity skinLossRadiativeFraction_IP() const;

    boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;
    boost::optional<ModelObject> pumpCurveAsModelObject() const;
    boost::optional<ModelObject> zoneAsModelObject() const;

    bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PUMPCONSTANTSPEED_IMPL_HPP

