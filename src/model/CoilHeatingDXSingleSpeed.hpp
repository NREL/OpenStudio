/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXSINGLESPEED_HPP
#define MODEL_COILHEATINGDXSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkEquivalentDuct;
  class Curve;
  class Schedule;

  namespace detail {

    class CoilHeatingDXSingleSpeed_Impl;

  }  // namespace detail

  /** CoilHeatingDXSingleSpeed is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:Coil:Heating:DX:SingleSpeed'. */
  class MODEL_API CoilHeatingDXSingleSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDXSingleSpeed(const Model& model, Schedule& availabilitySchedule, Curve& totalHeatingCapacityFunctionofTemperatureCurve,
                                      Curve& totalHeatingCapacityFunctionofFlowFractionCurve, Curve& energyInputRatioFunctionofTemperatureCurve,
                                      Curve& energyInputRatioFunctionofFlowFractionCurve, Curve& partLoadFractionCorrelationCurve);

    explicit CoilHeatingDXSingleSpeed(const Model& model);

    virtual ~CoilHeatingDXSingleSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXSingleSpeed(const CoilHeatingDXSingleSpeed& other) = default;
    CoilHeatingDXSingleSpeed(CoilHeatingDXSingleSpeed&& other) = default;
    CoilHeatingDXSingleSpeed& operator=(const CoilHeatingDXSingleSpeed&) = default;
    CoilHeatingDXSingleSpeed& operator=(CoilHeatingDXSingleSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDefrostStrategyValues();

    static std::vector<std::string> validDefrostControlValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedTotalHeatingCapacity() const;

    bool isRatedTotalHeatingCapacityAutosized() const;

    double ratedCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    OS_DEPRECATED(3, 5, 0) double ratedSupplyFanPowerPerVolumeFlowRate() const;

    double ratedSupplyFanPowerPerVolumeFlowRate2017() const;

    double ratedSupplyFanPowerPerVolumeFlowRate2023() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

    double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    bool isCrankcaseHeaterCapacityDefaulted() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const;

    std::string defrostStrategy() const;

    bool isDefrostStrategyDefaulted() const;

    std::string defrostControl() const;

    bool isDefrostControlDefaulted() const;

    double defrostTimePeriodFraction() const;

    bool isDefrostTimePeriodFractionDefaulted() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityDefaulted() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

    void autosizeRatedTotalHeatingCapacity();

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedCOP(double ratedCOP);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    OS_DEPRECATED(3, 5, 0) bool setRatedSupplyFanPowerPerVolumeFlowRate(double ratedSupplyFanPowerPerVolumeFlowRate);

    bool setRatedSupplyFanPowerPerVolumeFlowRate2017(double ratedSupplyFanPowerPerVolumeFlowRate2017);

    bool setRatedSupplyFanPowerPerVolumeFlowRate2023(double ratedSupplyFanPowerPerVolumeFlowRate2023);

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

    bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

    void resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    void resetCrankcaseHeaterCapacity();

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();

    bool setDefrostStrategy(const std::string& defrostStrategy);

    void resetDefrostStrategy();

    bool setDefrostControl(const std::string& defrostControl);

    void resetDefrostControl();

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    void resetDefrostTimePeriodFraction();

    bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

    void resetResistiveDefrostHeaterCapacity();

    void autosizeResistiveDefrostHeaterCapacity();

    //@}
    /** @name Curves */
    //@{

    // A5 , \field Total Heating Capacity Function of Temperature Curve Name
    // \object-list BiquadraticQuadraticCubicCurves
    Curve totalHeatingCapacityFunctionofTemperatureCurve() const;
    bool setTotalHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
    // \object-list QuadraticCubicCurves
    Curve totalHeatingCapacityFunctionofFlowFractionCurve() const;
    bool setTotalHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
    // \object-list BiquadraticQuadraticCubicCurves
    Curve energyInputRatioFunctionofTemperatureCurve() const;
    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
    // \object-list QuadraticCubicCurves
    Curve energyInputRatioFunctionofFlowFractionCurve() const;
    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    // A9 , \field Part Load Fraction Correlation Curve Name
    // \object-list QuadraticCubicCurves
    Curve partLoadFractionCorrelationCurve() const;
    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
    // \object-list BiquadraticCurves
    // not a required curve so it needs a reset
    boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;
    bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);
    void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

    boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

    //@}

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXSingleSpeed_Impl;

    friend class detail::CoilHeatingDXSingleSpeed_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit CoilHeatingDXSingleSpeed(std::shared_ptr<detail::CoilHeatingDXSingleSpeed_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXSingleSpeed");
  };

  /** \relates CoilHeatingDXSingleSpeed*/
  using OptionalCoilHeatingDXSingleSpeed = boost::optional<CoilHeatingDXSingleSpeed>;

  /** \relates CoilHeatingDXSingleSpeed*/
  using CoilHeatingDXSingleSpeedVector = std::vector<CoilHeatingDXSingleSpeed>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXSINGLESPEED_HPP
