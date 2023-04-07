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

#ifndef MODEL_GENERATORMICROTURBINE_HPP
#define MODEL_GENERATORMICROTURBINE_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"
#include "GeneratorMicroTurbineHeatRecovery.hpp"

namespace openstudio {

namespace model {

  class Curve;
  // For the optional Generator:MicroTurbine:HeatRecovery
  // It was broken out because that part needs to connect to a plant loop
  class StraightComponent;

  // TODO: add the tables class if they get added to OS later?
  //class DataTables // UniVariateTables and BiVariateTables

  //class Connection;

  namespace detail {

    class GeneratorMicroTurbine_Impl;

  }  // namespace detail

  /** GeneratorMicroTurbine is a Generator that wraps the OpenStudio IDD object 'OS:Generator:MicroTurbine'. */
  class MODEL_API GeneratorMicroTurbine : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // TODO: Could add a constructor with all the required inputs, but that's a lot of them

    explicit GeneratorMicroTurbine(const Model& model);

    virtual ~GeneratorMicroTurbine() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorMicroTurbine(const GeneratorMicroTurbine& other) = default;
    GeneratorMicroTurbine(GeneratorMicroTurbine&& other) = default;
    GeneratorMicroTurbine& operator=(const GeneratorMicroTurbine&) = default;
    GeneratorMicroTurbine& operator=(GeneratorMicroTurbine&&) = default;

    virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validFuelTypeValues();

    /** @name Getters */
    //@{

    // TODO: In E+ there's ElectricLoadCenter:GeneratorList where you'll specify the order of generators and the availability Schedule
    // here it's been moved to the Generator themselves
    boost::optional<Schedule> availabilitySchedule() const;

    double referenceElectricalPowerOutput() const;

    double minimumFullLoadElectricalPowerOutput() const;
    bool isMinimumFullLoadElectricalPowerOutputDefaulted() const;

    // This will default to referenceElectricalPowerOutput if not defined, like E+ does
    double maximumFullLoadElectricalPowerOutput() const;
    bool isMaximumFullLoadElectricalPowerOutputDefaulted() const;

    double referenceElectricalEfficiencyUsingLowerHeatingValue() const;

    double referenceCombustionAirInletTemperature() const;
    bool isReferenceCombustionAirInletTemperatureDefaulted() const;

    double referenceCombustionAirInletHumidityRatio() const;
    bool isReferenceCombustionAirInletHumidityRatioDefaulted() const;

    double referenceElevation() const;
    bool isReferenceElevationDefaulted() const;

    Curve electricalPowerFunctionofTemperatureandElevationCurve() const;

    Curve electricalEfficiencyFunctionofTemperatureCurve() const;

    Curve electricalEfficiencyFunctionofPartLoadRatioCurve() const;

    std::string fuelType() const;
    bool isFuelTypeDefaulted() const;

    double fuelHigherHeatingValue() const;
    bool isFuelHigherHeatingValueDefaulted() const;

    double fuelLowerHeatingValue() const;
    bool isFuelLowerHeatingValueDefaulted() const;

    double standbyPower() const;
    bool isStandbyPowerDefaulted() const;

    double ancillaryPower() const;
    bool isAncillaryPowerDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<Curve> ancillaryPowerFunctionofFuelInputCurve() const;

    // Optional Generator:MicroTurbine:HeatRecovery
    boost::optional<GeneratorMicroTurbineHeatRecovery> generatorMicroTurbineHeatRecovery() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //boost::optional<Connection> combustionAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    // boost::optional<Connection> combustionAirOutletNode() const;

    boost::optional<double> referenceExhaustAirMassFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirFlowRateFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirFlowRateFunctionofPartLoadRatioCurve() const;

    boost::optional<double> nominalExhaustAirOutletTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirTemperatureFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirTemperatureFunctionofPartLoadRatioCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);
    void resetAvailabilitySchedule();

    bool setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput);

    bool setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput);
    void resetMinimumFullLoadElectricalPowerOutput();

    bool setMaximumFullLoadElectricalPowerOutput(double maximumFullLoadElectricalPowerOutput);
    void resetMaximumFullLoadElectricalPowerOutput();

    bool setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue);

    bool setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature);
    void resetReferenceCombustionAirInletTemperature();

    bool setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio);
    void resetReferenceCombustionAirInletHumidityRatio();

    bool setReferenceElevation(double referenceElevation);
    void resetReferenceElevation();

    // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    bool setElectricalPowerFunctionofTemperatureandElevationCurve(const Curve& electricalPowerFunctionofTemperatureandElevationCurve);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& electricalEfficiencyFunctionofTemperatureCurve);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& electricalEfficiencyFunctionofPartLoadRatioCurve);

    bool setFuelType(const std::string& fuelType);
    void resetFuelType();

    bool setFuelHigherHeatingValue(double fuelHigherHeatingValue);
    void resetFuelHigherHeatingValue();

    bool setFuelLowerHeatingValue(double fuelLowerHeatingValue);
    void resetFuelLowerHeatingValue();

    bool setStandbyPower(double standbyPower);
    void resetStandbyPower();

    bool setAncillaryPower(double ancillaryPower);
    void resetAncillaryPower();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setAncillaryPowerFunctionofFuelInputCurve(const Curve& ancillaryPowerFunctionofFuelInputCurve);
    void resetAncillaryPowerFunctionofFuelInputCurve();

    // Optional Generator:MicroTurbine:HeatRecovery
    //bool setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery);
    //void resetGeneratorMicroTurbineHeatRecovery();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setCombustionAirInletNode(const Connection& connection);
    //void resetCombustionAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setCombustionAirOutletNode(const Connection& connection);
    //void resetCombustionAirOutletNode();

    bool setReferenceExhaustAirMassFlowRate(double referenceExhaustAirMassFlowRate);
    void resetReferenceExhaustAirMassFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofTemperatureCurve(const Curve& exhaustAirFlowRateFunctionofTemperatureCurve);
    void resetExhaustAirFlowRateFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const Curve& exhaustAirFlowRateFunctionofPartLoadRatioCurve);
    void resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();

    bool setNominalExhaustAirOutletTemperature(double nominalExhaustAirOutletTemperature);
    void resetNominalExhaustAirOutletTemperature();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofTemperatureCurve(const Curve& exhaustAirTemperatureFunctionofTemperatureCurve);
    void resetExhaustAirTemperatureFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const Curve& exhaustAirTemperatureFunctionofPartLoadRatioCurve);
    void resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorMicroTurbine_Impl;

    explicit GeneratorMicroTurbine(std::shared_ptr<detail::GeneratorMicroTurbine_Impl> impl);

    friend class detail::GeneratorMicroTurbine_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");
  };

  /** \relates GeneratorMicroTurbine*/
  using OptionalGeneratorMicroTurbine = boost::optional<GeneratorMicroTurbine>;

  /** \relates GeneratorMicroTurbine*/
  using GeneratorMicroTurbineVector = std::vector<GeneratorMicroTurbine>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORMICROTURBINE_HPP
