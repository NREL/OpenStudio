/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed:SpeedData'. */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(const Model& model);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& operator=(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& operator=(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double ratedWaterHeatingCapacity() const;

    double ratedWaterHeatingCOP() const;

    double ratedSensibleHeatRatio() const;

    double referenceUnitRatedAirFlowRate() const;

    double referenceUnitRatedWaterFlowRate() const;

    double referenceUnitWaterPumpInputPowerAtRatedConditions() const;

    Curve totalWaterHeatingCapacityFunctionofTemperatureCurve() const;

    Curve totalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve copFunctionofTemperatureCurve() const;

    Curve copFunctionofAirFlowFractionCurve() const;

    Curve copFunctionofWaterFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

    bool setRatedWaterHeatingCOP(double ratedWaterHeatingCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setReferenceUnitWaterPumpInputPowerAtRatedConditions(double referenceUnitWaterPumpInputPowerAtRatedConditions);

    bool setTotalWaterHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(
      std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData*/
  using OptionalCoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData = boost::optional<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>;

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData*/
  using CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedDataVector = std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP
