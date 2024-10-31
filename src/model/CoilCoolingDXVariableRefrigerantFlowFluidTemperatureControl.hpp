/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

  }  // namespace detail

  /** CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl'. */
  class MODEL_API CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model);

    virtual ~CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(const CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl& other) = default;
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl&& other) = default;
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl&
      operator=(const CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl&) = default;
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl& operator=(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> autosizedRatedTotalCoolingCapacity();

    boost::optional<double> ratedSensibleHeatRatio() const;

    bool isRatedSensibleHeatRatioAutosized() const;

    boost::optional<double> autosizedRatedSensibleHeatRatio();

    double indoorUnitReferenceSuperheating() const;

    Curve indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    void autosizeRatedSensibleHeatRatio();

    bool setIndoorUnitReferenceSuperheating(double indoorUnitReferenceSuperheating);

    bool setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

    explicit CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(
      std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl);

    friend class detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl");
  };

  /** \relates CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl*/
  using OptionalCoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl =
    boost::optional<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>;

  /** \relates CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl*/
  using CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControlVector = std::vector<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
