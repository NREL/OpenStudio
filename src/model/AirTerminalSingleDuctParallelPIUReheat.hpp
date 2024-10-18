/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class AirTerminalSingleDuctParallelPIUReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctParallelPIUReheat is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ParallelPIU:Reheat'. */
  class MODEL_API AirTerminalSingleDuctParallelPIUReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctParallelPIUReheat(const Model& model, Schedule& schedule, HVACComponent& fan, HVACComponent& reheatCoil);

    virtual ~AirTerminalSingleDuctParallelPIUReheat() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctParallelPIUReheat(const AirTerminalSingleDuctParallelPIUReheat& other) = default;
    AirTerminalSingleDuctParallelPIUReheat(AirTerminalSingleDuctParallelPIUReheat&& other) = default;
    AirTerminalSingleDuctParallelPIUReheat& operator=(const AirTerminalSingleDuctParallelPIUReheat&) = default;
    AirTerminalSingleDuctParallelPIUReheat& operator=(AirTerminalSingleDuctParallelPIUReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanControlTypeValues();

    static std::vector<std::string> heatingControlTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumPrimaryAirFlowRate() const;

    bool isMaximumPrimaryAirFlowRateAutosized() const;

    boost::optional<double> maximumSecondaryAirFlowRate() const;

    bool isMaximumSecondaryAirFlowRateAutosized() const;

    boost::optional<double> minimumPrimaryAirFlowFraction() const;

    bool isMinimumPrimaryAirFlowFractionAutosized() const;

    boost::optional<double> fanOnFlowFraction() const;

    bool isFanOnFlowFractionAutosized() const;

    boost::optional<Node> secondaryAirInletNode() const;

    unsigned secondaryAirInletPort() const;

    HVACComponent fan() const;

    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    std::string fanControlType() const;

    double minimumFanTurnDownRatio() const;

    std::string heatingControlType() const;

    double designHeatingDischargeAirTemperature() const;

    double highLimitHeatingDischargeAirTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate);

    void autosizeMaximumPrimaryAirFlowRate();

    bool setMaximumSecondaryAirFlowRate(double maximumSecondaryAirFlowRate);

    void autosizeMaximumSecondaryAirFlowRate();

    bool setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction);

    void autosizeMinimumPrimaryAirFlowFraction();

    bool setFanOnFlowFraction(double fanOnFlowFraction);

    void autosizeFanOnFlowFraction();

    bool setFan(HVACComponent& hvacComponent);

    bool setReheatCoil(HVACComponent& hvacComponent);

    bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    void resetMinimumHotWaterorSteamFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    bool setFanControlType(const std::string& fanControlType);

    bool setMinimumFanTurnDownRatio(double minimumFanTurnDownRatio);

    bool setHeatingControlType(const std::string& heatingControlType);

    bool setDesignHeatingDischargeAirTemperature(double designHeatingDischargeAirTemperature);

    bool setHighLimitHeatingDischargeAirTemperature(double highLimitHeatingDischargeAirTemperature);

    boost::optional<double> autosizedMaximumPrimaryAirFlowRate() const;

    boost::optional<double> autosizedMaximumSecondaryAirFlowRate() const;

    boost::optional<double> autosizedMinimumPrimaryAirFlowFraction() const;

    boost::optional<double> autosizedFanOnFlowFraction() const;

    boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctParallelPIUReheat_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    friend class detail::AirTerminalSingleDuctParallelPIUReheat_Impl;

    explicit AirTerminalSingleDuctParallelPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctParallelPIUReheat_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctParallelPIUReheat");
  };

  /** \relates AirTerminalSingleDuctParallelPIUReheat*/
  using OptionalAirTerminalSingleDuctParallelPIUReheat = boost::optional<AirTerminalSingleDuctParallelPIUReheat>;

  /** \relates AirTerminalSingleDuctParallelPIUReheat*/
  using AirTerminalSingleDuctParallelPIUReheatVector = std::vector<AirTerminalSingleDuctParallelPIUReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP
