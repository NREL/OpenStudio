/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLPOWERMODULE_HPP
#define MODEL_GENERATORFUELCELLPOWERMODULE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"

namespace openstudio {

namespace model {

  class CurveQuadratic;
  class ThermalZone;
  class Node;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellPowerModule_Impl;

  }  // namespace detail

  /** GeneratorFuelCellPowerModule is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:PowerModule'. */
  class MODEL_API GeneratorFuelCellPowerModule : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellPowerModule(const Model& model, const ThermalZone& heatlossZone, const Node& dilutionInletAirNode,
                                          const Node& dilutionOutletAirNode);

    explicit GeneratorFuelCellPowerModule(const Model& model, const CurveQuadratic& efficiencyCurve, const ThermalZone& heatlossZone,
                                          const Node& dilutionInletAirNode, const Node& dilutionOutletAirNode);

    explicit GeneratorFuelCellPowerModule(const Model& model, const CurveQuadratic& efficiencyCurve, const ThermalZone& heatlossZone,
                                          const Node& dilutionInletAirNode, const Node& dilutionOutletAirNode, const CurveQuadratic& skinlossCurve);

    explicit GeneratorFuelCellPowerModule(const Model& model, const CurveQuadratic& efficiencyCurve);

    explicit GeneratorFuelCellPowerModule(const Model& model);

    virtual ~GeneratorFuelCellPowerModule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellPowerModule(const GeneratorFuelCellPowerModule& other) = default;
    GeneratorFuelCellPowerModule(GeneratorFuelCellPowerModule&& other) = default;
    GeneratorFuelCellPowerModule& operator=(const GeneratorFuelCellPowerModule&) = default;
    GeneratorFuelCellPowerModule& operator=(GeneratorFuelCellPowerModule&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> efficiencyCurveModeValues();

    static std::vector<std::string> skinLossCalculationModeValues();

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
    /// @cond
    using ImplType = detail::GeneratorFuelCellPowerModule_Impl;

    explicit GeneratorFuelCellPowerModule(std::shared_ptr<detail::GeneratorFuelCellPowerModule_Impl> impl);

    friend class detail::GeneratorFuelCellPowerModule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellPowerModule");
  };

  /** \relates GeneratorFuelCellPowerModule*/
  using OptionalGeneratorFuelCellPowerModule = boost::optional<GeneratorFuelCellPowerModule>;

  /** \relates GeneratorFuelCellPowerModule*/
  using GeneratorFuelCellPowerModuleVector = std::vector<GeneratorFuelCellPowerModule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLPOWERMODULE_HPP
