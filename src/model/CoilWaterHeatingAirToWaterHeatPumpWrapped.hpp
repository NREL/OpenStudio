/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPumpWrapped is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:Wrapped'. */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpWrapped : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPumpWrapped(const Model& model);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpWrapped() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPumpWrapped(const CoilWaterHeatingAirToWaterHeatPumpWrapped& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpWrapped(CoilWaterHeatingAirToWaterHeatPumpWrapped&& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpWrapped& operator=(const CoilWaterHeatingAirToWaterHeatPumpWrapped&) = default;
    CoilWaterHeatingAirToWaterHeatPumpWrapped& operator=(CoilWaterHeatingAirToWaterHeatPumpWrapped&&) = default;

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

    double ratedCondenserWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutocalculated() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCOPFunctionofTemperatureCurve() const;

    Curve heatingCOPFunctionofAirFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedCOP(double ratedCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

    bool setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autocalculateRatedEvaporatorAirFlowRate();

    bool setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPumpWrapped(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpWrapped");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPumpWrapped*/
  using OptionalCoilWaterHeatingAirToWaterHeatPumpWrapped = boost::optional<CoilWaterHeatingAirToWaterHeatPumpWrapped>;

  /** \relates CoilWaterHeatingAirToWaterHeatPumpWrapped*/
  using CoilWaterHeatingAirToWaterHeatPumpWrappedVector = std::vector<CoilWaterHeatingAirToWaterHeatPumpWrapped>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP
