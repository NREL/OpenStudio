/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class CoilHeatingLowTempRadiantConstFlow_Impl;

  }  // namespace detail

  /** CoilHeatingLowTempRadiantConstFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:LowTemperatureRadiant:ConstantFlow'.
   The object has following four schedules: heating high and low water temperature schedules, heating high and low control temperature schedules. These schedules
   define the high and low limits of a zone radiant system's desired inlet/supply heating water temperatures and the high and low limits of a zone's heating control temperature setpoints.
   If the space radiant heating control temperature drops below the heating control temperature setpoint low limit, the inlet heating water temperature is increased to its high limit. If the space
   radiant heating control temperature exceeds its heating control temperature high limit, the radiant heating loop is shut off. If the space radiant heating control temperature varies between
   the heating control temperature high and low limits, the desired inlet heating water temperature is linearly interpolated between the high and low heating water temperature limits. The desired water
   temperature is achieved by the zone 3-way valve for mixing/bypassing controls.
  */

  class MODEL_API CoilHeatingLowTempRadiantConstFlow : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingLowTempRadiantConstFlow(const Model& model, Schedule& heatingHighWaterTemperatureSchedule,
                                       Schedule& heatingLowWaterTemperatureSchedule, Schedule& heatingHighControlTemperatureSchedule,
                                       Schedule& heatingLowControlTemperatureSchedule);

    virtual ~CoilHeatingLowTempRadiantConstFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingLowTempRadiantConstFlow(const CoilHeatingLowTempRadiantConstFlow& other) = default;
    CoilHeatingLowTempRadiantConstFlow(CoilHeatingLowTempRadiantConstFlow&& other) = default;
    CoilHeatingLowTempRadiantConstFlow& operator=(const CoilHeatingLowTempRadiantConstFlow&) = default;
    CoilHeatingLowTempRadiantConstFlow& operator=(CoilHeatingLowTempRadiantConstFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> heatingHighWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowWaterTemperatureSchedule() const;

    boost::optional<Schedule> heatingHighControlTemperatureSchedule() const;

    boost::optional<Schedule> heatingLowControlTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingHighWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighWaterTemperatureSchedule();

    bool setHeatingLowWaterTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowWaterTemperatureSchedule();

    bool setHeatingHighControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingHighControlTemperatureSchedule();

    bool setHeatingLowControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingLowControlTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingLowTempRadiantConstFlow_Impl;

    explicit CoilHeatingLowTempRadiantConstFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantConstFlow_Impl> impl);

    friend class detail::CoilHeatingLowTempRadiantConstFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantConstFlow");
  };

  /** \relates CoilHeatingLowTempRadiantConstFlow*/
  using OptionalCoilHeatingLowTempRadiantConstFlow = boost::optional<CoilHeatingLowTempRadiantConstFlow>;

  /** \relates CoilHeatingLowTempRadiantConstFlow*/
  using CoilHeatingLowTempRadiantConstFlowVector = std::vector<CoilHeatingLowTempRadiantConstFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP
