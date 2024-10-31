/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~GeneratorFuelCellPowerModule_Impl() override = default;

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
