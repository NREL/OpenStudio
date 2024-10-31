/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctVAVHeatAndCoolReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:HeatAndCool:Reheat'. */
  class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctVAVHeatAndCoolReheat(const Model& model, const HVACComponent& reheatCoil);

    virtual ~AirTerminalSingleDuctVAVHeatAndCoolReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctVAVHeatAndCoolReheat(const AirTerminalSingleDuctVAVHeatAndCoolReheat& other) = default;
    AirTerminalSingleDuctVAVHeatAndCoolReheat(AirTerminalSingleDuctVAVHeatAndCoolReheat&& other) = default;
    AirTerminalSingleDuctVAVHeatAndCoolReheat& operator=(const AirTerminalSingleDuctVAVHeatAndCoolReheat&) = default;
    AirTerminalSingleDuctVAVHeatAndCoolReheat& operator=(AirTerminalSingleDuctVAVHeatAndCoolReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    double zoneMinimumAirFlowFraction() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    double convergenceTolerance() const;

    double maximumReheatAirTemperature() const;

    boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

    bool setReheatCoil(const HVACComponent& heatingCoilName);

    bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    bool setConvergenceTolerance(double convergenceTolerance);

    bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

    bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

    void resetMinimumAirFlowTurndownSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl;

    explicit AirTerminalSingleDuctVAVHeatAndCoolReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl> impl);

    friend class detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolReheat");
  };

  /** \relates AirTerminalSingleDuctVAVHeatAndCoolReheat*/
  using OptionalAirTerminalSingleDuctVAVHeatAndCoolReheat = boost::optional<AirTerminalSingleDuctVAVHeatAndCoolReheat>;

  /** \relates AirTerminalSingleDuctVAVHeatAndCoolReheat*/
  using AirTerminalSingleDuctVAVHeatAndCoolReheatVector = std::vector<AirTerminalSingleDuctVAVHeatAndCoolReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP
