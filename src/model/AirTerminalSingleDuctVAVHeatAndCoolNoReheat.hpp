/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctVAVHeatAndCoolNoReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:HeatAndCool:NoReheat'. */
  class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolNoReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const Model& model);

    virtual ~AirTerminalSingleDuctVAVHeatAndCoolNoReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat& other) = default;
    AirTerminalSingleDuctVAVHeatAndCoolNoReheat(AirTerminalSingleDuctVAVHeatAndCoolNoReheat&& other) = default;
    AirTerminalSingleDuctVAVHeatAndCoolNoReheat& operator=(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat&) = default;
    AirTerminalSingleDuctVAVHeatAndCoolNoReheat& operator=(AirTerminalSingleDuctVAVHeatAndCoolNoReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    double zoneMinimumAirFlowFraction() const;

    boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

    bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

    void resetMinimumAirFlowTurndownSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl;

    explicit AirTerminalSingleDuctVAVHeatAndCoolNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl> impl);

    friend class detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolNoReheat");
  };

  /** \relates AirTerminalSingleDuctVAVHeatAndCoolNoReheat*/
  using OptionalAirTerminalSingleDuctVAVHeatAndCoolNoReheat = boost::optional<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>;

  /** \relates AirTerminalSingleDuctVAVHeatAndCoolNoReheat*/
  using AirTerminalSingleDuctVAVHeatAndCoolNoReheatVector = std::vector<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP
