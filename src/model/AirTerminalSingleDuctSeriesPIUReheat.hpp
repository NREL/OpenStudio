/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class AirTerminalSingleDuctSeriesPIUReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctSeriesPIUReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:SeriesPIU:Reheat'. */
  class MODEL_API AirTerminalSingleDuctSeriesPIUReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctSeriesPIUReheat(const Model& model, HVACComponent& fan, HVACComponent& reheatCoil);

    virtual ~AirTerminalSingleDuctSeriesPIUReheat() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctSeriesPIUReheat(const AirTerminalSingleDuctSeriesPIUReheat& other) = default;
    AirTerminalSingleDuctSeriesPIUReheat(AirTerminalSingleDuctSeriesPIUReheat&& other) = default;
    AirTerminalSingleDuctSeriesPIUReheat& operator=(const AirTerminalSingleDuctSeriesPIUReheat&) = default;
    AirTerminalSingleDuctSeriesPIUReheat& operator=(AirTerminalSingleDuctSeriesPIUReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    boost::optional<double> maximumPrimaryAirFlowRate() const;

    bool isMaximumPrimaryAirFlowRateAutosized() const;

    boost::optional<double> minimumPrimaryAirFlowFraction() const;

    bool isMinimumPrimaryAirFlowFractionAutosized() const;

    HVACComponent fan() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    double convergenceTolerance() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    bool setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate);

    void autosizeMaximumPrimaryAirFlowRate();

    bool setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction);

    void autosizeMinimumPrimaryAirFlowFraction();

    bool setFan(const HVACComponent& fan);

    bool setReheatCoil(const HVACComponent& coil);

    bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    bool setConvergenceTolerance(double convergenceTolerance);

    //@}
    /** @name Other */
    //@{

    unsigned secondaryAirInletPort() const;
    boost::optional<Node> secondaryAirInletNode() const;

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    boost::optional<double> autosizedMaximumPrimaryAirFlowRate() const;

    boost::optional<double> autosizedMinimumPrimaryAirFlowFraction() const;

    boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctSeriesPIUReheat_Impl;

    explicit AirTerminalSingleDuctSeriesPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl> impl);

    friend class detail::AirTerminalSingleDuctSeriesPIUReheat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctSeriesPIUReheat");
  };

  /** \relates AirTerminalSingleDuctSeriesPIUReheat*/
  using OptionalAirTerminalSingleDuctSeriesPIUReheat = boost::optional<AirTerminalSingleDuctSeriesPIUReheat>;

  /** \relates AirTerminalSingleDuctSeriesPIUReheat*/
  using AirTerminalSingleDuctSeriesPIUReheatVector = std::vector<AirTerminalSingleDuctSeriesPIUReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP
