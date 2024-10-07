/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class CoilCoolingDXVariableRefrigerantFlow_Impl;

  }  // namespace detail

  /** CoilCoolingDXVariableRefrigerantFlow is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableRefrigerantFlow'. */
  class MODEL_API CoilCoolingDXVariableRefrigerantFlow : public HVACComponent
  {

   public:
    explicit CoilCoolingDXVariableRefrigerantFlow(const Model& model);

    virtual ~CoilCoolingDXVariableRefrigerantFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXVariableRefrigerantFlow(const CoilCoolingDXVariableRefrigerantFlow& other) = default;
    CoilCoolingDXVariableRefrigerantFlow(CoilCoolingDXVariableRefrigerantFlow&& other) = default;
    CoilCoolingDXVariableRefrigerantFlow& operator=(const CoilCoolingDXVariableRefrigerantFlow&) = default;
    CoilCoolingDXVariableRefrigerantFlow& operator=(CoilCoolingDXVariableRefrigerantFlow&&) = default;

    static IddObjectType iddObjectType();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

    void autosizeRatedTotalCoolingCapacity();

    boost::optional<double> ratedSensibleHeatRatio() const;

    bool isRatedSensibleHeatRatioAutosized() const;

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    void autosizeRatedSensibleHeatRatio();

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    Curve coolingCapacityRatioModifierFunctionofTemperatureCurve() const;

    bool setCoolingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& lcurve);

    Curve coolingCapacityModifierCurveFunctionofFlowFraction() const;

    bool setCoolingCapacityModifierCurveFunctionofFlowFraction(const Curve& lcurve);

    boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedSensibleHeatRatio() const;

    boost::optional<double> autosizedRatedAirFlowRate() const;

   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXVariableRefrigerantFlow_Impl;

    explicit CoilCoolingDXVariableRefrigerantFlow(std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlow_Impl> impl);

    friend class detail::CoilCoolingDXVariableRefrigerantFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlow");
  };

  /** \relates CoilCoolingDXVariableRefrigerantFlow*/
  using OptionalCoilCoolingDXVariableRefrigerantFlow = boost::optional<CoilCoolingDXVariableRefrigerantFlow>;

  /** \relates CoilCoolingDXVariableRefrigerantFlow*/
  using CoilCoolingDXVariableRefrigerantFlowVector = std::vector<CoilCoolingDXVariableRefrigerantFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP
