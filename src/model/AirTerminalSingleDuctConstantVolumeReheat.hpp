/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;
  class HVACComponent;

  namespace detail {

    class AirTerminalSingleDuctConstantVolumeReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctConstantVolumeReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:Reheat'. */
  class MODEL_API AirTerminalSingleDuctConstantVolumeReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctConstantVolumeReheat(const Model& model, Schedule& availabilitySchedule, HVACComponent& coil);

    virtual ~AirTerminalSingleDuctConstantVolumeReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctConstantVolumeReheat(const AirTerminalSingleDuctConstantVolumeReheat& other) = default;
    AirTerminalSingleDuctConstantVolumeReheat(AirTerminalSingleDuctConstantVolumeReheat&& other) = default;
    AirTerminalSingleDuctConstantVolumeReheat& operator=(const AirTerminalSingleDuctConstantVolumeReheat&) = default;
    AirTerminalSingleDuctConstantVolumeReheat& operator=(AirTerminalSingleDuctConstantVolumeReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: HeatingCoilName.
    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    double maximumReheatAirTemperature() const;

    bool isMaximumReheatAirTemperatureDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void resetMaximumAirFlowRate();

    void autosizeMaximumAirFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: HeatingCoilName.
    bool setReheatCoil(const HVACComponent& coil);

    void resetReheatCoil();

    bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    void resetMinimumHotWaterorSteamFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

    void resetMaximumReheatAirTemperature();

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctConstantVolumeReheat_Impl;

    explicit AirTerminalSingleDuctConstantVolumeReheat(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl> impl);

    friend class detail::AirTerminalSingleDuctConstantVolumeReheat_Impl;
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeReheat");
  };

  /** \relates AirTerminalSingleDuctConstantVolumeReheat*/
  using OptionalAirTerminalSingleDuctConstantVolumeReheat = boost::optional<AirTerminalSingleDuctConstantVolumeReheat>;

  /** \relates AirTerminalSingleDuctConstantVolumeReheat*/
  using AirTerminalSingleDuctConstantVolumeReheatVector = std::vector<AirTerminalSingleDuctConstantVolumeReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP
