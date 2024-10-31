/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPump_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPump is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump'.
 *  As of EnergyPlus version 8.4.0 this object maps to Coil:WaterHeating:AirToWaterHeatPump:Pumped in idf format.
 */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPump : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model);

    explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model, Curve& heatingCapacityFunctionofTemperatureCurve,
                                                Curve& heatingCapacityFunctionofAirFlowFractionCurve,
                                                Curve& heatingCapacityFunctionofWaterFlowFractionCurve, Curve& heatingCOPFunctionofTemperatureCurve,
                                                Curve& heatingCOPFunctionofAirFlowFractionCurve, Curve& heatingCOPFunctionofWaterFlowFractionCurve,
                                                Curve& partLoadFractionCorrelationCurve);

    virtual ~CoilWaterHeatingAirToWaterHeatPump() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPump(const CoilWaterHeatingAirToWaterHeatPump& other) = default;
    CoilWaterHeatingAirToWaterHeatPump(CoilWaterHeatingAirToWaterHeatPump&& other) = default;
    CoilWaterHeatingAirToWaterHeatPump& operator=(const CoilWaterHeatingAirToWaterHeatPump&) = default;
    CoilWaterHeatingAirToWaterHeatPump& operator=(CoilWaterHeatingAirToWaterHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> evaporatorAirTemperatureTypeforCurveObjectsValues();

    /** @name Getters */
    //@{

    double ratedHeatingCapacity() const;

    double ratedCOP() const;

    double ratedSensibleHeatRatio() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserInletWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutosized() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    bool condenserPumpPowerIncludedinRatedCOP() const;

    bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double condenserWaterPumpPower() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve heatingCOPFunctionofTemperatureCurve() const;

    Curve heatingCOPFunctionofAirFlowFractionCurve() const;

    Curve heatingCOPFunctionofWaterFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedCOP(double ratedCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

    bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autosizeRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate);

    void autosizeRatedCondenserWaterFlowRate();

    bool setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    bool setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

    bool setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedEvaporatorAirFlowRate() const;

    boost::optional<double> autosizedRatedCondenserWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPump_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPump(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPump_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPump*/
  using OptionalCoilWaterHeatingAirToWaterHeatPump = boost::optional<CoilWaterHeatingAirToWaterHeatPump>;

  /** \relates CoilWaterHeatingAirToWaterHeatPump*/
  using CoilWaterHeatingAirToWaterHeatPumpVector = std::vector<CoilWaterHeatingAirToWaterHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP
