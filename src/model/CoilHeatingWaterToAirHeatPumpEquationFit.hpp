/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilHeatingWaterToAirHeatPumpEquationFit_Impl;

  }  // namespace detail

  /** CoilHeatingWaterToAirHeatPumpEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:WaterToAirHeatPump:EquationFit'. */
  class MODEL_API CoilHeatingWaterToAirHeatPumpEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new CoilCoolingWaterToAirHeatPumpEquationFit with required curve objects */
    CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model, const Curve& heatingCapacityCurve, const Curve& heatingPowerConsumptionCurve);

    CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model);

    virtual ~CoilHeatingWaterToAirHeatPumpEquationFit() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterToAirHeatPumpEquationFit(const CoilHeatingWaterToAirHeatPumpEquationFit& other) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit(CoilHeatingWaterToAirHeatPumpEquationFit&& other) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit& operator=(const CoilHeatingWaterToAirHeatPumpEquationFit&) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit& operator=(CoilHeatingWaterToAirHeatPumpEquationFit&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutosized() const;

    boost::optional<double> ratedWaterFlowRate() const;

    bool isRatedWaterFlowRateDefaulted() const;

    bool isRatedWaterFlowRateAutosized() const;

    boost::optional<double> ratedHeatingCapacity() const;

    bool isRatedHeatingCapacityDefaulted() const;

    bool isRatedHeatingCapacityAutosized() const;

    double ratedHeatingCoefficientofPerformance() const;

    bool isRatedHeatingCoefficientofPerformanceDefaulted() const;

    double ratedEnteringWaterTemperature() const;

    double ratedEnteringAirDryBulbTemperature() const;

    double ratioofRatedHeatingCapacitytoRatedCoolingCapacity() const;

    Curve heatingCapacityCurve() const;

    OS_DEPRECATED(3, 2, 0) double heatingCapacityCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double heatingCapacityCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double heatingCapacityCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double heatingCapacityCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double heatingCapacityCoefficient5() const;

    Curve heatingPowerConsumptionCurve() const;

    OS_DEPRECATED(3, 2, 0) double heatingPowerConsumptionCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double heatingPowerConsumptionCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double heatingPowerConsumptionCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double heatingPowerConsumptionCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double heatingPowerConsumptionCoefficient5() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

    bool setRatedWaterFlowRate(double ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity);

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    void resetRatedHeatingCapacity();

    void autosizeRatedHeatingCapacity();

    bool setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance);

    void resetRatedHeatingCoefficientofPerformance();

    bool setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature);

    bool setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature);

    bool setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(double ratioofRatedHeatingCapacitytoRatedCoolingCapacity);

    bool setHeatingCapacityCurve(const Curve& heatingCapacityCurve);

    OS_DEPRECATED(3, 2, 0) bool setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    bool setHeatingPowerConsumptionCurve(const Curve& heatingPowerConsumptionCurve);

    OS_DEPRECATED(3, 2, 0) bool setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5);

    bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedRatedWaterFlowRate() const;

    boost::optional<double> autosizedRatedHeatingCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl;

    explicit CoilHeatingWaterToAirHeatPumpEquationFit(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl> impl);

    friend class detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpEquationFit");
  };

  /** \relates CoilHeatingWaterToAirHeatPumpEquationFit*/
  using OptionalCoilHeatingWaterToAirHeatPumpEquationFit = boost::optional<CoilHeatingWaterToAirHeatPumpEquationFit>;

  /** \relates CoilHeatingWaterToAirHeatPumpEquationFit*/
  using CoilHeatingWaterToAirHeatPumpEquationFitVector = std::vector<CoilHeatingWaterToAirHeatPumpEquationFit>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
