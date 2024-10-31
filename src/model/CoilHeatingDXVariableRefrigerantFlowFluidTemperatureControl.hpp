/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
#define MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

  }  // namespace detail

  /** CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableRefrigerantFlow:FluidTemperatureControl'. */
  class MODEL_API CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model);

    virtual ~CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(const CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl& other) = default;
    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl&& other) = default;
    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl&
      operator=(const CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl&) = default;
    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl& operator=(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedTotalHeatingCapacity() const;

    bool isRatedTotalHeatingCapacityAutosized() const;

    boost::optional<double> autosizedRatedTotalHeatingCapacity();

    double indoorUnitReferenceSubcooling() const;

    Curve indoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

    void autosizeRatedTotalHeatingCapacity();

    bool setIndoorUnitReferenceSubcooling(double indoorUnitReferenceSubcooling);

    bool setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

    explicit CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(
      std::shared_ptr<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl);

    friend class detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl");
  };

  /** \relates CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl*/
  using OptionalCoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl =
    boost::optional<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>;

  /** \relates CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl*/
  using CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControlVector = std::vector<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
