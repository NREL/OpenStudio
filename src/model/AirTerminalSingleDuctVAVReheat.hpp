/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "ModelAPI.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class HVACComponent;

  namespace detail {
    class AirTerminalSingleDuctVAVReheat_Impl;
  }  // namespace detail

  class MODEL_API AirTerminalSingleDuctVAVReheat : public StraightComponent
  {

   public:
    explicit AirTerminalSingleDuctVAVReheat(const Model& model, Schedule& availabilitySchedule, HVACComponent& coil);

    virtual ~AirTerminalSingleDuctVAVReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctVAVReheat(const AirTerminalSingleDuctVAVReheat& other) = default;
    AirTerminalSingleDuctVAVReheat(AirTerminalSingleDuctVAVReheat&& other) = default;
    AirTerminalSingleDuctVAVReheat& operator=(const AirTerminalSingleDuctVAVReheat&) = default;
    AirTerminalSingleDuctVAVReheat& operator=(AirTerminalSingleDuctVAVReheat&&) = default;

    static IddObjectType iddObjectType();

    /** Returns the reheat coil */
    HVACComponent reheatCoil() const;

    /** Sets the reheat coil.  Returns true if the supplied coil was accecpted */
    bool setReheatCoil(HVACComponent& coil);

    /** Returns the Schedule referred to by the AvailabilityScheduleName field. */
    Schedule availabilitySchedule() const;

    /** Sets the Schedule referred to by the AvailabilityScheduleName field. */
    bool setAvailabilitySchedule(Schedule& schedule);

    /** Returns the value of the MaximumAirFlowRate field. */
    boost::optional<double> maximumAirFlowRate() const;

    /** Sets the value of the MaximumAirFlowRate field. */
    bool setMaximumAirFlowRate(double value);

    /** Sets the value of the MaximumAirFlowRate field to AutoSize */
    void autosizeMaximumAirFlowRate();

    /** Returns true of the MaximumAirFlowRate field is set to AutoSize */
    bool isMaximumAirFlowRateAutosized() const;

    /** Returns the value of the MaximumAirFlowRate field. */
    std::string zoneMinimumAirFlowInputMethod();
    /** deprecated **/
    OS_DEPRECATED(3, 0, 1) std::string zoneMinimumAirFlowMethod();

    /** Sets the value of the MaximumAirFlowRate field.
   *  Options are FixedFlowRate and Scheduled.
   */
    bool setZoneMinimumAirFlowInputMethod(const std::string& value);
    /** deprecated **/
    OS_DEPRECATED(3, 0, 1) bool setZoneMinimumAirFlowMethod(const std::string& value);

    /** Returns the value of the ConstantMinimumAirFlowFraction field. */
    boost::optional<double> constantMinimumAirFlowFraction() const;

    /** Sets the value of the ConstantMinimumAirFlowFraction field. */
    bool setConstantMinimumAirFlowFraction(double value);

    /** Sets the value of the ConstantMinimumAirFlowFraction field to Autosize */
    void autosizeConstantMinimumAirFlowFraction();

    /** Returns true of the ConstantMinimumAirFlowFraction field is set to Autosize */
    bool isConstantMinimumAirFlowFractionAutosized() const;

    /** Returns the value of the FixedMinimumAirFlowRate field. */
    boost::optional<double> fixedMinimumAirFlowRate() const;

    /** Sets the value of the FixedMinimumAirFlowRate field. */
    bool setFixedMinimumAirFlowRate(double value);

    /** Sets the value of the FixedMinimumAirFlowRate field to Autosize */
    void autosizeFixedMinimumAirFlowRate();

    /** Returns true of the FixedMinimumAirFlowRate field is set to Autosize */
    bool isFixedMinimumAirFlowRateAutosized() const;

    /** Returns the Schedule referred to by the MinimumAirFlowFractionScheduleName field. */
    boost::optional<Schedule> minimumAirFlowFractionSchedule() const;

    /** Sets the Schedule referred to by the MinimumAirFlowFractionScheduleName field. */
    bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

    void resetMinimumAirFlowFractionSchedule();

    /** Returns the Schedule referred to by the MinimumAirFlowTurndownScheduleName field. */
    boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

    /** Sets the Schedule referred to by the MinimumAirFlowTurndownScheduleName field. */
    bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

    void resetMinimumAirFlowTurndownSchedule();

    /** Returns the value of the MaximumHotWaterOrSteamFlowRate field. */
    boost::optional<double> maximumHotWaterOrSteamFlowRate();

    /** Sets the value of the MaximumHotWaterOrSteamFlowRate field. */
    bool setMaximumHotWaterOrSteamFlowRate(double value);

    /** Sets the value of the MaximumHotWaterOrSteamFlowRate field to AutoSize */
    void autosizeMaximumHotWaterOrSteamFlowRate();

    /** Returns true of the MaximumHotWaterOrSteamFlowRate field is set to AutoSize */
    bool isMaximumHotWaterOrSteamFlowRateAutosized() const;

    /** Returns the value of the MinimumHotWaterOrSteamFlowRate field. */
    double minimumHotWaterOrSteamFlowRate();

    /** Sets the value of the MinimumHotWaterOrSteamFlowRate field. */
    bool setMinimumHotWaterOrStreamFlowRate(double value);

    /** Returns the value of the ConvergenceTolerance field. */
    double convergenceTolerance();

    /** Sets the value of the ConvergenceTolerance field. */
    bool setConvergenceTolerance(double value);

    /** Returns the value of the DamperHeatingAction field. */
    std::string damperHeatingAction();

    /** Sets the value of the DamperHeatingAction field.
   *  Options are Normal and Reverse.
   */
    bool setDamperHeatingAction(const std::string& value);

    /** Returns the value of the MaximumFlowPerZoneFloorAreaDuringReheat field. */
    boost::optional<double> maximumFlowPerZoneFloorAreaDuringReheat();

    /** Sets the value of the MaximumFlowPerZoneFloorAreaDuringReheat field. */
    bool setMaximumFlowPerZoneFloorAreaDuringReheat(double value);

    /** Sets the value of the MaximumFlowPerZoneFloorAreaDuringReheat field to AutoSize */
    void autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

    /** Returns true of the MaximumFlowPerZoneFloorAreaDuringReheat field is set to AutoSize */
    bool isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const;

    /** Resets the MaximumFlowPerZoneFloorAreaDuringReheat field to no value */
    void resetMaximumFlowPerZoneFloorAreaDuringReheat();

    /** Returns the value of the MaximumFlowFractionDuringReheat field. */
    boost::optional<double> maximumFlowFractionDuringReheat();

    /** Sets the value of the MaximumFlowFractionDuringReheat field. */
    bool setMaximumFlowFractionDuringReheat(double value);

    /** Sets the value of the MaximumFlowFractionDuringReheat field to AutoSize */
    void autosizeMaximumFlowFractionDuringReheat();

    /** Returns true of the MaximumFlowFractionDuringReheat field is set to AutoSize */
    bool isMaximumFlowFractionDuringReheatAutosized() const;

    /** Reset MaximumFlowFractionDuringReheat to no value */
    void resetMaximumFlowFractionDuringReheat();

    /** Returns the value of the MaximumReheatAirTemperature field. */
    double maximumReheatAirTemperature();

    /** Sets the value of the MaximumReheatAirTemperature field. */
    bool setMaximumReheatAirTemperature(double value);

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;

    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    boost::optional<double> autosizedConstantMinimumAirFlowFraction() const;

    boost::optional<double> autosizedFixedMinimumAirFlowRate() const;

    boost::optional<double> autosizedMaximumHotWaterOrSteamFlowRate() const;

    boost::optional<double> autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const;

    boost::optional<double> autosizedMaximumFlowFractionDuringReheat() const;

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    /// @cond

    using ImplType = detail::AirTerminalSingleDuctVAVReheat_Impl;

    explicit AirTerminalSingleDuctVAVReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVReheat_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVReheat");

    /// @endcond
  };

  using OptionalAirTerminalSingleDuctVAVReheat = boost::optional<AirTerminalSingleDuctVAVReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP
