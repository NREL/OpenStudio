/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeed is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed'. */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeed : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const Model& model);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& operator=(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& operator=(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    double ratedWaterHeatingCapacity() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserInletWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutocalculated() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutocalculated() const;

    std::string evaporatorFanPowerIncludedinRatedCOP() const;

    std::string condenserPumpPowerIncludedinRatedCOP() const;

    std::string condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletWetDryBulbTemperature);

    bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autocalculateRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate);

    void autocalculateRatedCondenserWaterFlowRate();

    bool setEvaporatorFanPowerIncludedinRatedCOP(const std::string& evaporatorFanPowerIncludedinRatedCOP);

    bool setCondenserPumpPowerIncludedinRatedCOP(const std::string& condenserPumpPowerIncludedinRatedCOP);

    bool setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(const std::string& condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

    bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autocalculatedRatedEvaporatorAirFlowRate() const;

    boost::optional<double> autocalculatedRatedCondenserWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeed");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeed*/
  using OptionalCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = boost::optional<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>;

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeed*/
  using CoilWaterHeatingAirToWaterHeatPumpVariableSpeedVector = std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP
