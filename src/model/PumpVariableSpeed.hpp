/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_PUMPVARIABLESPEED_HPP
#define MODEL_PUMPVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;

  namespace detail {

    class PumpVariableSpeed_Impl;

  }  // namespace detail

  /** PumpVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Pump:VariableSpeed'. */
  class MODEL_API PumpVariableSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PumpVariableSpeed(const Model& model);

    virtual ~PumpVariableSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PumpVariableSpeed(const PumpVariableSpeed& other) = default;
    PumpVariableSpeed(PumpVariableSpeed&& other) = default;
    PumpVariableSpeed& operator=(const PumpVariableSpeed&) = default;
    PumpVariableSpeed& operator=(PumpVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> pumpControlTypeValues();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** \deprecated */
    static std::vector<std::string> validPumpControlTypeValues();

    static std::vector<std::string> vfdControlTypeValues();

    /** @name Getters */
    //@{

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Maximum Flow Rate" **/
    boost::optional<double> ratedFlowRate() const;

    bool isRatedFlowRateDefaulted() const;

    bool isRatedFlowRateAutosized() const;

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Pump Head" **/
    double ratedPumpHead() const;

    bool isRatedPumpHeadDefaulted() const;

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Power Consumption" **/
    boost::optional<double> ratedPowerConsumption() const;

    bool isRatedPowerConsumptionDefaulted() const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

    double coefficient1ofthePartLoadPerformanceCurve() const;

    bool isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient2ofthePartLoadPerformanceCurve() const;

    bool isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient3ofthePartLoadPerformanceCurve() const;

    bool isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const;

    double coefficient4ofthePartLoadPerformanceCurve() const;

    bool isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const;

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Minimum Flow Rate" **/
    double minimumFlowRate() const;

    bool isMinimumFlowRateDefaulted() const;

    std::string pumpControlType() const;

    bool isPumpControlTypeDefaulted() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<Curve> pumpCurve() const;

    boost::optional<double> impellerDiameter() const;

    boost::optional<std::string> vFDControlType() const;

    boost::optional<Schedule> pumpRPMSchedule() const;

    boost::optional<Schedule> minimumPressureSchedule() const;

    boost::optional<Schedule> maximumPressureSchedule() const;

    boost::optional<Schedule> minimumRPMSchedule() const;

    boost::optional<Schedule> maximumRPMSchedule() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedFlowRate(double ratedFlowRate);

    void resetRatedFlowRate();

    void autosizeRatedFlowRate();

    bool setRatedPumpHead(double ratedPumpHead);

    void resetRatedPumpHead();

    bool setRatedPowerConsumption(double ratedPowerConsumption);

    void resetRatedPowerConsumption();

    void autosizeRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    bool setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

    void resetCoefficient1ofthePartLoadPerformanceCurve();

    bool setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

    void resetCoefficient2ofthePartLoadPerformanceCurve();

    bool setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

    void resetCoefficient3ofthePartLoadPerformanceCurve();

    bool setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

    void resetCoefficient4ofthePartLoadPerformanceCurve();

    bool setMinimumFlowRate(double minimumFlowRate);

    void resetMinimumFlowRate();

    bool setPumpControlType(const std::string& pumpControlType);

    void resetPumpControlType();

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    /** Returns false if curve is not a CurveLinear, CurveQuadratic, CurveCubic or CurveQuartic. */
    bool setPumpCurve(const Curve& curve);

    void resetPumpCurve();

    bool setImpellerDiameter(double impellerDiameter);

    void resetImpellerDiameter();

    bool setVFDControlType(const std::string& vFDControlType);

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

    boost::optional<double> autosizedRatedFlowRate() const;

    boost::optional<double> autosizedRatedPowerConsumption() const;

    std::string designPowerSizingMethod() const;

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    double designElectricPowerPerUnitFlowRate() const;

    bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

    double designShaftPowerPerUnitFlowRatePerUnitHead() const;

    bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

    boost::optional<ThermalZone> zone() const;

    bool setZone(const ThermalZone& thermalZone);

    void resetZone();

    double skinLossRadiativeFraction() const;

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    double designMinimumFlowRateFraction() const;

    bool setDesignMinimumFlowRateFraction(double designMinimumFlowRateFraction);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
   protected:
    /// @cond
    using ImplType = detail::PumpVariableSpeed_Impl;

    friend class detail::PumpVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit PumpVariableSpeed(std::shared_ptr<detail::PumpVariableSpeed_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PumpVariableSpeed");
  };

  /** \relates PumpVariableSpeed*/
  using OptionalPumpVariableSpeed = boost::optional<PumpVariableSpeed>;

  /** \relates PumpVariableSpeed*/
  using PumpVariableSpeedVector = std::vector<PumpVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPVARIABLESPEED_HPP
