/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit:SpeedData'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model);

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
      const Model& model, const Curve& totalCoolingCapacityFunctionofTemperature, const Curve& totalCoolingCapacityFunctionofAirFlowFraction,
      const Curve& totalCoolingCapacityFunctionofWaterFlowFraction, const Curve& energyInputRatioFunctionofTemperature,
      const Curve& energyInputRatioFunctionofAirFlowFraction, const Curve& energyInputRatioFunctionofWaterFlowFraction,
      const Curve& wasteHeatFunctionofTemperature);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& other) =
      default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double referenceUnitGrossRatedTotalCoolingCapacity() const;

    double referenceUnitGrossRatedSensibleHeatRatio() const;

    double referenceUnitGrossRatedCoolingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    double referenceUnitRatedWaterFlowRate() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

    Curve totalCoolingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

    double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

    bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

    bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

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
    using ImplType = detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
      std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData =
    boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedDataVector =
    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
