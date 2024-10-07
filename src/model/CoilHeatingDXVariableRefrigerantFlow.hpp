/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class CoilHeatingDXVariableRefrigerantFlow_Impl;

  }  // namespace detail

  /** CoilHeatingDXVariableRefrigerantFlow is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableRefrigerantFlow'. */
  class MODEL_API CoilHeatingDXVariableRefrigerantFlow : public HVACComponent
  {
   public:
    explicit CoilHeatingDXVariableRefrigerantFlow(const Model& model);

    virtual ~CoilHeatingDXVariableRefrigerantFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXVariableRefrigerantFlow(const CoilHeatingDXVariableRefrigerantFlow& other) = default;
    CoilHeatingDXVariableRefrigerantFlow(CoilHeatingDXVariableRefrigerantFlow&& other) = default;
    CoilHeatingDXVariableRefrigerantFlow& operator=(const CoilHeatingDXVariableRefrigerantFlow&) = default;
    CoilHeatingDXVariableRefrigerantFlow& operator=(CoilHeatingDXVariableRefrigerantFlow&&) = default;

    static IddObjectType iddObjectType();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> ratedTotalHeatingCapacity() const;

    bool isRatedTotalHeatingCapacityAutosized() const;

    bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

    void autosizeRatedTotalHeatingCapacity();

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    Curve heatingCapacityRatioModifierFunctionofTemperatureCurve() const;

    bool setHeatingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve);

    Curve heatingCapacityModifierFunctionofFlowFractionCurve() const;

    bool setHeatingCapacityModifierFunctionofFlowFractionCurve(const Curve& curve);

    boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

    boost::optional<double> autosizedRatedAirFlowRate() const;

   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXVariableRefrigerantFlow_Impl;

    explicit CoilHeatingDXVariableRefrigerantFlow(std::shared_ptr<detail::CoilHeatingDXVariableRefrigerantFlow_Impl> impl);

    friend class detail::CoilHeatingDXVariableRefrigerantFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableRefrigerantFlow");
  };

  /** \relates CoilHeatingDXVariableRefrigerantFlow*/
  using OptionalCoilHeatingDXVariableRefrigerantFlow = boost::optional<CoilHeatingDXVariableRefrigerantFlow>;

  /** \relates CoilHeatingDXVariableRefrigerantFlow*/
  using CoilHeatingDXVariableRefrigerantFlowVector = std::vector<CoilHeatingDXVariableRefrigerantFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_HPP
