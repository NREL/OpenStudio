/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

  }  // namespace detail

  /** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit:SpeedData'. */
  class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model);

    explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model, const Curve& heatingCapacityFunctionofTemperature,
                                                                            const Curve& totalHeatingCapacityFunctionofAirFlowFraction,
                                                                            const Curve& heatingCapacityFunctionofWaterFlowFraction,
                                                                            const Curve& energyInputRatioFunctionofTemperature,
                                                                            const Curve& energyInputRatioFunctionofAirFlowFraction,
                                                                            const Curve& energyInputRatioFunctionofWaterFlowFraction,
                                                                            const Curve& wasteHeatFunctionofTemperature);

    virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& other) =
      default;
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&& other) = default;
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&) = default;
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double referenceUnitGrossRatedHeatingCapacity() const;

    double referenceUnitGrossRatedHeatingCOP() const;

    double referenceUnitRatedAirFlow() const;

    double referenceUnitRatedWaterFlowRate() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

    double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

    bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

    bool setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

    bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

    explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
      std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl);

    friend class detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");
  };

  /** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using OptionalCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData =
    boost::optional<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

  /** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedDataVector =
    std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
