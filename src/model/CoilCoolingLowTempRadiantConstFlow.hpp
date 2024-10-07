/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class CoilCoolingLowTempRadiantConstFlow_Impl;

  }  // namespace detail

  /** CoilCoolingLowTempRadiantConstFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:LowTemperatureRadiant:ConstantFlow'. */
  class MODEL_API CoilCoolingLowTempRadiantConstFlow : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingLowTempRadiantConstFlow(const Model& model, Schedule& coolingHighWaterTemperatureSchedule,
                                       Schedule& coolingLowWaterTemperatureSchedule, Schedule& coolingHighControlTemperatureSchedule,
                                       Schedule& coolingLowControlTemperatureSchedule);

    virtual ~CoilCoolingLowTempRadiantConstFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingLowTempRadiantConstFlow(const CoilCoolingLowTempRadiantConstFlow& other) = default;
    CoilCoolingLowTempRadiantConstFlow(CoilCoolingLowTempRadiantConstFlow&& other) = default;
    CoilCoolingLowTempRadiantConstFlow& operator=(const CoilCoolingLowTempRadiantConstFlow&) = default;
    CoilCoolingLowTempRadiantConstFlow& operator=(CoilCoolingLowTempRadiantConstFlow&&) = default;

    //virtual unsigned inletPort() const;

    //virtual unsigned outletPort() const;
    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condensationControlTypeValues();

    /** @name Getters */
    //@{

    /** The following four schedules: cooling high and low water temperature schedules, cooling high and low control temperature schedules
   define the high and low limits of a zone radiant system's desired inlet/supply cooling water temperatures and the high and low limits of a zone's cooling control temperature setpoints.
   If the space radiant cooling control temperature exceeds the cooling control temperature setpoint high limit, the inlet cooling water temperature is reduced to its low limit. If the space
   radiant cooling control temperature drops below its cooling control temperature low limit, the radiant cooling loop is shut off. If the space radiant cooling control temperature varies between
   the cooling control temperature high and low limits, the desired inlet cooling water temperature is linearly interpolated between the high and low cooling water temperature limits. The desired water
   temperature is achieved by the zone 3-way valve for mixing/bypassing controls.
  */

    boost::optional<Schedule> coolingHighWaterTemperatureSchedule() const;

    boost::optional<Schedule> coolingLowWaterTemperatureSchedule() const;

    boost::optional<Schedule> coolingHighControlTemperatureSchedule() const;

    boost::optional<Schedule> coolingLowControlTemperatureSchedule() const;

    std::string condensationControlType() const;

    bool isCondensationControlTypeDefaulted() const;

    double condensationControlDewpointOffset() const;

    bool isCondensationControlDewpointOffsetDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoolingHighWaterTemperatureSchedule(Schedule& schedule);

    void resetCoolingHighWaterTemperatureSchedule();

    bool setCoolingLowWaterTemperatureSchedule(Schedule& schedule);

    void resetCoolingLowWaterTemperatureSchedule();

    bool setCoolingHighControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingHighControlTemperatureSchedule();

    bool setCoolingLowControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingLowControlTemperatureSchedule();

    bool setCondensationControlType(const std::string& condensationControlType);

    void resetCondensationControlType();

    bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

    void resetCondensationControlDewpointOffset();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingLowTempRadiantConstFlow_Impl;

    explicit CoilCoolingLowTempRadiantConstFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantConstFlow_Impl> impl);

    friend class detail::CoilCoolingLowTempRadiantConstFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantConstFlow");
  };

  /** \relates CoilCoolingLowTempRadiantConstFlow*/
  using OptionalCoilCoolingLowTempRadiantConstFlow = boost::optional<CoilCoolingLowTempRadiantConstFlow>;

  /** \relates CoilCoolingLowTempRadiantConstFlow*/
  using CoilCoolingLowTempRadiantConstFlowVector = std::vector<CoilCoolingLowTempRadiantConstFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP
