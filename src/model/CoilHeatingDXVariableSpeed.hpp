/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLESPEED_HPP
#define MODEL_COILHEATINGDXVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class CoilHeatingDXVariableSpeedSpeedData;
  class Curve;

  namespace detail {

    class CoilHeatingDXVariableSpeed_Impl;

  }  // namespace detail

  /** CoilHeatingDXVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableSpeed'. */
  class MODEL_API CoilHeatingDXVariableSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDXVariableSpeed(const Model& model);

    explicit CoilHeatingDXVariableSpeed(const Model& model, const Curve& partLoadFraction);

    virtual ~CoilHeatingDXVariableSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXVariableSpeed(const CoilHeatingDXVariableSpeed& other) = default;
    CoilHeatingDXVariableSpeed(CoilHeatingDXVariableSpeed&& other) = default;
    CoilHeatingDXVariableSpeed& operator=(const CoilHeatingDXVariableSpeed&) = default;
    CoilHeatingDXVariableSpeed& operator=(CoilHeatingDXVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> defrostStrategyValues();

    static std::vector<std::string> defrostControlValues();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

    bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    Curve energyPartLoadFractionCurve() const;

    boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

    double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    std::string defrostStrategy() const;

    std::string defrostControl() const;

    double defrostTimePeriodFraction() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel);

    void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    bool setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor);

    void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

    bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setDefrostStrategy(const std::string& defrostStrategy);

    bool setDefrostControl(const std::string& defrostControl);

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

    void autosizeResistiveDefrostHeaterCapacity();

    //@}
    /** @name Other */
    //@{

    std::vector<CoilHeatingDXVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedRatedHeatingCapacityAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXVariableSpeed_Impl;

    explicit CoilHeatingDXVariableSpeed(std::shared_ptr<detail::CoilHeatingDXVariableSpeed_Impl> impl);

    friend class detail::CoilHeatingDXVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeed");
  };

  /** \relates CoilHeatingDXVariableSpeed*/
  using OptionalCoilHeatingDXVariableSpeed = boost::optional<CoilHeatingDXVariableSpeed>;

  /** \relates CoilHeatingDXVariableSpeed*/
  using CoilHeatingDXVariableSpeedVector = std::vector<CoilHeatingDXVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLESPEED_HPP
