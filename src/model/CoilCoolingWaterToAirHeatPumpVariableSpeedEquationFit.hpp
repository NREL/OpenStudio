/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

  namespace detail {

    class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model);

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model, const Curve& partLoadFraction);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& operator=(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& operator=(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double fanDelayTime() const;

    bool useHotGasReheat() const;

    Curve energyPartLoadFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setFanDelayTime(double fanDelayTime);

    bool setUseHotGasReheat(bool useHotGasReheat);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(
      std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
  using OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit = boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>;

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
  using CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitVector = std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
