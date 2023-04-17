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

#ifndef MODEL_GENERATORMICROTURBINE_IMPL_HPP
#define MODEL_GENERATORMICROTURBINE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  // For the optional Generator:MicroTurbine:HeatRecovery
  // It was broken out because that part needs to connect to a plant loop
  class StraightComponent;

  // TODO: add the tables class if they get added to OS later?
  //class DataTables // UniVariateTables and BiVariateTables

  namespace detail {

    /** GeneratorMicroTurbine_Impl is a Generator_Impl that is the implementation class for GeneratorMicroTurbine.*/
    class MODEL_API GeneratorMicroTurbine_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorMicroTurbine_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbine_Impl(const GeneratorMicroTurbine_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorMicroTurbine_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::string generatorObjectType() const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      // Will clone also the mchpHR if any
      virtual ModelObject clone(Model model) const override;

      // Will also remove the mchpHR (and remove it from loop) if any
      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

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

      // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
      Curve electricalPowerFunctionofTemperatureandElevationCurve() const;

      // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
      Curve electricalEfficiencyFunctionofTemperatureCurve() const;

      // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
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
      //boost::optional<Connection> combustionAirOutletNode() const;

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

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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

      // Private setter
      bool setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery);
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
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORMICROTURBINE_IMPL_HPP
