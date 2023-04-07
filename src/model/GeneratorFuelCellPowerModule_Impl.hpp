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

#ifndef MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP
#define MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CurveQuadratic;
  class ThermalZone;
  class Node;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellPowerModule_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellPowerModule.*/
    class MODEL_API GeneratorFuelCellPowerModule_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellPowerModule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellPowerModule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellPowerModule_Impl(const GeneratorFuelCellPowerModule_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellPowerModule_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //@}
      /** @name Getters */
      //@{

      std::string efficiencyCurveMode() const;

      CurveQuadratic efficiencyCurve() const;

      double nominalEfficiency() const;

      double nominalElectricalPower() const;

      double numberofStopsatStartofSimulation() const;

      double cyclingPerformanceDegradationCoefficient() const;

      double numberofRunHoursatBeginningofSimulation() const;

      double accumulatedRunTimeDegradationCoefficient() const;

      double runTimeDegradationInitiationTimeThreshold() const;

      double powerUpTransientLimit() const;

      double powerDownTransientLimit() const;

      double startUpTime() const;

      double startUpFuel() const;

      double startUpElectricityConsumption() const;

      double startUpElectricityProduced() const;

      double shutDownTime() const;

      double shutDownFuel() const;

      double shutDownElectricityConsumption() const;

      double ancillaryElectricityConstantTerm() const;

      double ancillaryElectricityLinearTerm() const;

      std::string skinLossCalculationMode() const;

      boost::optional<ThermalZone> zone() const;

      double skinLossRadiativeFraction() const;

      double constantSkinLossRate() const;

      double skinLossUFactorTimesAreaTerm() const;

      boost::optional<CurveQuadratic> skinLossQuadraticCurve() const;

      double dilutionAirFlowRate() const;

      double stackHeatlosstoDilutionAir() const;

      boost::optional<Node> dilutionInletAirNode() const;

      boost::optional<Node> dilutionOutletAirNode() const;

      double minimumOperatingPoint() const;

      double maximumOperatingPoint() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setEfficiencyCurveMode(const std::string& efficiencyCurveMode);

      void resetEfficiencyCurveMode();

      bool setEfficiencyCurve(const CurveQuadratic& quadraticCurve);

      bool setNominalEfficiency(double nominalEfficiency);

      void resetNominalEfficiency();

      bool setNominalElectricalPower(double nominalElectricalPower);

      void resetNominalElectricalPower();

      bool setNumberofStopsatStartofSimulation(double numberofStopsatStartofSimulation);

      void resetNumberofStopsatStartofSimulation();

      bool setCyclingPerformanceDegradationCoefficient(double cyclingPerformanceDegradationCoefficient);

      void resetCyclingPerformanceDegradationCoefficient();

      bool setNumberofRunHoursatBeginningofSimulation(double numberofRunHoursatBeginningofSimulation);

      void resetNumberofRunHoursatBeginningofSimulation();

      bool setAccumulatedRunTimeDegradationCoefficient(double accumulatedRunTimeDegradationCoefficient);

      void resetAccumulatedRunTimeDegradationCoefficient();

      bool setRunTimeDegradationInitiationTimeThreshold(double runTimeDegradationInitiationTimeThreshold);

      void resetRunTimeDegradationInitiationTimeThreshold();

      bool setPowerUpTransientLimit(double powerUpTransientLimit);

      void resetPowerUpTransientLimit();

      bool setPowerDownTransientLimit(double powerDownTransientLimit);

      void resetPowerDownTransientLimit();

      bool setStartUpTime(double startUpTime);

      void resetStartUpTime();

      bool setStartUpFuel(double startUpFuel);

      void resetStartUpFuel();

      bool setStartUpElectricityConsumption(double startUpElectricityConsumption);

      void resetStartUpElectricityConsumption();

      bool setStartUpElectricityProduced(double startUpElectricityProduced);

      void resetStartUpElectricityProduced();

      bool setShutDownTime(double shutDownTime);

      void resetShutDownTime();

      bool setShutDownFuel(double shutDownFuel);

      void resetShutDownFuel();

      bool setShutDownElectricityConsumption(double shutDownElectricityConsumption);

      void resetShutDownElectricityConsumption();

      bool setAncillaryElectricityConstantTerm(double ancillaryElectricityConstantTerm);

      void resetAncillaryElectricityConstantTerm();

      bool setAncillaryElectricityLinearTerm(double ancillaryElectricityLinearTerm);

      void resetAncillaryElectricityLinearTerm();

      bool setSkinLossCalculationMode(const std::string& skinLossCalculationMode);

      void resetSkinLossCalculationMode();

      bool setZone(const ThermalZone& zone);

      void resetZone();

      bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

      void resetSkinLossRadiativeFraction();

      bool setConstantSkinLossRate(double constantSkinLossRate);

      void resetConstantSkinLossRate();

      bool setSkinLossUFactorTimesAreaTerm(double skinLossUFactorTimesAreaTerm);

      void resetSkinLossUFactorTimesAreaTerm();

      bool setSkinLossQuadraticCurve(const CurveQuadratic& quadraticCurves);

      void resetSkinLossQuadraticCurve();

      bool setDilutionAirFlowRate(double dilutionAirFlowRate);

      void resetDilutionAirFlowRate();

      bool setStackHeatlosstoDilutionAir(double stackHeatlosstoDilutionAir);

      void resetStackHeatlosstoDilutionAir();

      bool setDilutionInletAirNode(const Node& connection);

      void resetDilutionInletAirNode();

      bool setDilutionOutletAirNode(const Node& connection);

      void resetDilutionOutletAirNode();

      bool setMinimumOperatingPoint(double minimumOperatingPoint);

      void resetMinimumOperatingPoint();

      bool setMaximumOperatingPoint(double maximumOperatingPoint);

      void resetMaximumOperatingPoint();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellPowerModule");

      boost::optional<CurveQuadratic> optionalEfficiencyCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP
