/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {
    class CoilCoolingWaterToAirHeatPumpEquationFit_Impl;
  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:EquationFit'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */

    /** Constructs a new CoilCoolingWaterToAirHeatPumpEquationFit with required curve objects */
    CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model, const Curve& totalCoolingCapacityCurve, const Curve& sensibleCoolingCapacityCurve,
                                             const Curve& coolingPowerConsumptionCurve);

    /** Constructs a new CoilCoolingWaterToAirHeatPumpEquationFit object and places it inside the model.  The coil is fully initialized with all companion objects. */
    CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model);

    virtual ~CoilCoolingWaterToAirHeatPumpEquationFit() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterToAirHeatPumpEquationFit(const CoilCoolingWaterToAirHeatPumpEquationFit& other) = default;
    CoilCoolingWaterToAirHeatPumpEquationFit(CoilCoolingWaterToAirHeatPumpEquationFit&& other) = default;
    CoilCoolingWaterToAirHeatPumpEquationFit& operator=(const CoilCoolingWaterToAirHeatPumpEquationFit&) = default;
    CoilCoolingWaterToAirHeatPumpEquationFit& operator=(CoilCoolingWaterToAirHeatPumpEquationFit&&) = default;

    static IddObjectType iddObjectType();

    /** @name Getters */

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutosized() const;

    boost::optional<double> ratedWaterFlowRate() const;

    bool isRatedWaterFlowRateDefaulted() const;

    bool isRatedWaterFlowRateAutosized() const;

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityDefaulted() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> ratedSensibleCoolingCapacity() const;

    bool isRatedSensibleCoolingCapacityDefaulted() const;

    bool isRatedSensibleCoolingCapacityAutosized() const;

    double ratedCoolingCoefficientofPerformance() const;

    bool isRatedCoolingCoefficientofPerformanceDefaulted() const;

    double ratedEnteringWaterTemperature() const;

    double ratedEnteringAirDryBulbTemperature() const;

    double ratedEnteringAirWetBulbTemperature() const;

    Curve totalCoolingCapacityCurve() const;

    OS_DEPRECATED(3, 2, 0) double totalCoolingCapacityCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double totalCoolingCapacityCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double totalCoolingCapacityCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double totalCoolingCapacityCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double totalCoolingCapacityCoefficient5() const;

    Curve sensibleCoolingCapacityCurve() const;

    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient5() const;
    OS_DEPRECATED(3, 2, 0) double sensibleCoolingCapacityCoefficient6() const;

    Curve coolingPowerConsumptionCurve() const;

    OS_DEPRECATED(3, 2, 0) double coolingPowerConsumptionCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double coolingPowerConsumptionCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double coolingPowerConsumptionCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double coolingPowerConsumptionCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double coolingPowerConsumptionCoefficient5() const;

    Curve partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    bool isNominalTimeforCondensateRemovaltoBeginDefaulted() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    bool isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double fanDelayTime() const;

    /** @name Setters */

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(double ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

    void resetRatedTotalCoolingCapacity();

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleCoolingCapacity(double ratedSensibleCoolingCapacity);

    void resetRatedSensibleCoolingCapacity();

    void autosizeRatedSensibleCoolingCapacity();

    bool setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance);

    bool setRatedEnteringWaterTemperature(double ratedEnteringAirDryBulbTemperature);

    bool setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature);

    bool setRatedEnteringAirWetBulbTemperature(double ratedEnteringAirWetBulbTemperature);

    bool setTotalCoolingCapacityCurve(const Curve& totalCoolingCapacityCurve);

    OS_DEPRECATED(3, 2, 0) bool setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5);

    bool setSensibleCoolingCapacityCurve(const Curve& sensibleCoolingCapacityCurve);

    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5);
    OS_DEPRECATED(3, 2, 0) bool setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6);

    bool setCoolingPowerConsumptionCurve(const Curve& coolingPowerConsumptionCurve);

    OS_DEPRECATED(3, 2, 0) bool setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5);

    bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    void resetNominalTimeforCondensateRemovaltoBegin();

    bool
      setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    void resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setFanDelayTime(double fanDelayTime);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedRatedWaterFlowRate() const;

    boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedSensibleCoolingCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl;

    explicit CoilCoolingWaterToAirHeatPumpEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpEquationFit");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpEquationFit*/
  using OptionalCoilCoolingWaterToAirHeatPumpEquationFit = boost::optional<CoilCoolingWaterToAirHeatPumpEquationFit>;

  /** \relates CoilCoolingWaterToAirHeatPumpEquationFit*/
  using CoilCoolingWaterToAirHeatPumpEquationFitVector = std::vector<CoilCoolingWaterToAirHeatPumpEquationFit>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
