/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_FANCOMPONENTMODEL_IMPL_HPP
#define MODEL_FANCOMPONENTMODEL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;
class BivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;
class UnivariateFunctions;

namespace detail {

  /** FanComponentModel_Impl is a StraightComponent_Impl that is the implementation class for FanComponentModel.*/
  class MODEL_API FanComponentModel_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    FanComponentModel_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    FanComponentModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    FanComponentModel_Impl(const FanComponentModel_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~FanComponentModel_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection airInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection airOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule availabilitySchedule() const;

    double maximumFlowRate() const;

    double minimumFlowRate() const;

    double fanSizingFactor() const;

    double fanWheelDiameter() const;

    double fanOutletArea() const;

    double maximumFanStaticEfficiency() const;

    double eulerNumberatMaximumFanStaticEfficiency() const;

    double maximumDimensionlessFanAirflow() const;

    boost::optional<double> motorFanPulleyRatio() const;

    bool isMotorFanPulleyRatioAutosized() const;

    boost::optional <double> autosizedMotorFanPulleyRatio();

    boost::optional<double> beltMaximumTorque() const;

    bool isBeltMaximumTorqueAutosized() const;

    boost::optional <double> autosizedBeltMaximumTorque();

    double beltSizingFactor() const;

    double beltFractionalTorqueTransition() const;

    double motorMaximumSpeed() const;

    boost::optional<double> maximumMotorOutputPower() const;

    bool isMaximumMotorOutputPowerAutosized() const;

    boost::optional <double> autosizedMaximumMotorOutputPower();

    double motorSizingFactor() const;

    double motorInAirstreamFraction() const;

    boost::optional<std::string> vFDEfficiencyType() const;

    boost::optional<double> maximumVFDOutputPower() const;

    bool isMaximumVFDOutputPowerAutosized() const;

    boost::optional <double> autosizedMaximumVFDOutputPower();

    double vFDSizingFactor() const;

    // TODO: Check return type. From object lists, some candidates are: BivariateFunctions.
    BivariateFunctions fanPressureRiseCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    UnivariateFunctions ductStaticPressureResetCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    UnivariateFunctions normalizedFanStaticEfficiencyCurveNonStallRegion() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    UnivariateFunctions normalizedFanStaticEfficiencyCurveStallRegion() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    UnivariateFunctions normalizedDimensionlessAirflowCurveNonStallRegion() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    UnivariateFunctions normalizedDimensionlessAirflowCurveStallRegion() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> maximumBeltEfficiencyCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> normalizedBeltEfficiencyCurveRegion1() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> normalizedBeltEfficiencyCurveRegion2() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> normalizedBeltEfficiencyCurveRegion3() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> maximumMotorEfficiencyCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> normalizedMotorEfficiencyCurve() const;

    // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
    boost::optional<UnivariateFunctions> vFDEfficiencyCurve() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumFlowRate(double maximumFlowRate);

    bool setMinimumFlowRate(double minimumFlowRate);

    bool setFanSizingFactor(double fanSizingFactor);

    bool setFanWheelDiameter(double fanWheelDiameter);

    bool setFanOutletArea(double fanOutletArea);

    bool setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency);

    bool setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency);

    bool setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow);

    bool setMotorFanPulleyRatio(double motorFanPulleyRatio);

    void autosizeMotorFanPulleyRatio();

    bool setBeltMaximumTorque(double beltMaximumTorque);

    void autosizeBeltMaximumTorque();

    bool setBeltSizingFactor(double beltSizingFactor);

    bool setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition);

    bool setMotorMaximumSpeed(double motorMaximumSpeed);

    bool setMaximumMotorOutputPower(double maximumMotorOutputPower);

    void autosizeMaximumMotorOutputPower();

    bool setMotorSizingFactor(double motorSizingFactor);

    bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

    bool setVFDEfficiencyType(const std::string& vFDEfficiencyType);

    void resetVFDEfficiencyType();

    bool setMaximumVFDOutputPower(double maximumVFDOutputPower);

    void autosizeMaximumVFDOutputPower();

    bool setVFDSizingFactor(double vFDSizingFactor);

    // TODO: Check argument type. From object lists, some candidates are: BivariateFunctions.
    bool setFanPressureRiseCurve(const BivariateFunctions& bivariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setDuctStaticPressureResetCurve(const UnivariateFunctions& univariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedFanStaticEfficiencyCurveNonStallRegion(const UnivariateFunctions& univariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedFanStaticEfficiencyCurveStallRegion(const UnivariateFunctions& univariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedDimensionlessAirflowCurveNonStallRegion(const UnivariateFunctions& univariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedDimensionlessAirflowCurveStallRegion(const UnivariateFunctions& univariateFunctions);

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setMaximumBeltEfficiencyCurve(const UnivariateFunctions& univariateFunctions);

    void resetMaximumBeltEfficiencyCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedBeltEfficiencyCurveRegion1(const UnivariateFunctions& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion1();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedBeltEfficiencyCurveRegion2(const UnivariateFunctions& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion2();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedBeltEfficiencyCurveRegion3(const UnivariateFunctions& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion3();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setMaximumMotorEfficiencyCurve(const UnivariateFunctions& univariateFunctions);

    void resetMaximumMotorEfficiencyCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setNormalizedMotorEfficiencyCurve(const UnivariateFunctions& univariateFunctions);

    void resetNormalizedMotorEfficiencyCurve();

    // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
    bool setVFDEfficiencyCurve(const UnivariateFunctions& univariateFunctions);

    void resetVFDEfficiencyCurve();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    virtual void autosize() override;

    virtual void applySizingValues() override;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.FanComponentModel");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalAirInletNode() const;
    boost::optional<Connection> optionalAirOutletNode() const;
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<BivariateFunctions> optionalFanPressureRiseCurve() const;
    boost::optional<UnivariateFunctions> optionalDuctStaticPressureResetCurve() const;
    boost::optional<UnivariateFunctions> optionalNormalizedFanStaticEfficiencyCurveNonStallRegion() const;
    boost::optional<UnivariateFunctions> optionalNormalizedFanStaticEfficiencyCurveStallRegion() const;
    boost::optional<UnivariateFunctions> optionalNormalizedDimensionlessAirflowCurveNonStallRegion() const;
    boost::optional<UnivariateFunctions> optionalNormalizedDimensionlessAirflowCurveStallRegion() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FANCOMPONENTMODEL_IMPL_HPP

