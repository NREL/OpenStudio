/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

    std::string endUseSubcategory() const;

    bool setRatedFlowRate(boost::optional<double> ratedFlowRate);

    bool setRatedFlowRate(const OSOptionalQuantity& ratedFlowRate);

    void resetRatedFlowRate();

    void autosizeRatedFlowRate();

    bool setRatedPumpHead(double ratedPumpHead);

    bool setRatedPumpHead(const Quantity& ratedPumpHead);

    void resetRatedPumpHead();

    bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

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

    bool setImpellerDiameter(boost::optional<double> impellerDiameter);

    bool setImpellerDiameter(const OSOptionalQuantity& impellerDiameter);

    void resetImpellerDiameter();

    bool setRotationalSpeed(boost::optional<double> rotationalSpeed);

    bool setRotationalSpeed(const OSOptionalQuantity& rotationalSpeed);

    void resetRotationalSpeed();

    bool setZone(const boost::optional<ThermalZone>& thermalZone);

    void resetZone();

    bool setSkinLossRadiativeFraction(boost::optional<double> skinLossRadiativeFraction);

    bool setSkinLossRadiativeFraction(const OSOptionalQuantity& skinLossRadiativeFraction);

    void resetSkinLossRadiativeFraction();

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    bool addToNode(Node & node) override;

    boost::optional<double> autosizedRatedFlowRate() const ;

    boost::optional<double> autosizedRatedPowerConsumption() const ;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    std::string designPowerSizingMethod() const;

    bool setDesignPowerSizingMethod(const std::string & designPowerSizingMethod);

    double designElectricPowerPerUnitFlowRate() const;

    bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

    double designShaftPowerPerUnitFlowRatePerUnitHead() const;

    bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    bool setEndUseSubcategory(const std::string & endUseSubcategory);

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
