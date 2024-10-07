/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    class AirTerminalSingleDuctVAVNoReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctVAVNoReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:NoReheat'. */
  class MODEL_API AirTerminalSingleDuctVAVNoReheat : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctVAVNoReheat(const Model& model, Schedule& schedule);

    virtual ~AirTerminalSingleDuctVAVNoReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctVAVNoReheat(const AirTerminalSingleDuctVAVNoReheat& other) = default;
    AirTerminalSingleDuctVAVNoReheat(AirTerminalSingleDuctVAVNoReheat&& other) = default;
    AirTerminalSingleDuctVAVNoReheat& operator=(const AirTerminalSingleDuctVAVNoReheat&) = default;
    AirTerminalSingleDuctVAVNoReheat& operator=(AirTerminalSingleDuctVAVNoReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> zoneMinimumAirFlowInputMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    boost::optional<std::string> zoneMinimumAirFlowInputMethod() const;

    boost::optional<double> constantMinimumAirFlowFraction() const;
    bool isConstantMinimumAirFlowFractionAutosized() const;
    bool isConstantMinimumAirFlowFractionDefaulted() const;

    boost::optional<double> fixedMinimumAirFlowRate() const;
    bool isFixedMinimumAirFlowRateAutosized() const;
    bool isFixedMinimumAirFlowRateDefaulted() const;

    boost::optional<Schedule> minimumAirFlowFractionSchedule() const;
    boost::optional<Schedule> minimumAirFlowTurndownSchedule() const;

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void resetMaximumAirFlowRate();

    void autosizeMaximumAirFlowRate();

    bool setZoneMinimumAirFlowInputMethod(const std::string& zoneMinimumAirFlowInputMethod);

    void resetZoneMinimumAirFlowInputMethod();

    bool setConstantMinimumAirFlowFraction(double constantMinimumAirFlowFraction);
    void autosizeConstantMinimumAirFlowFraction();
    void resetConstantMinimumAirFlowFraction();

    bool setFixedMinimumAirFlowRate(double fixedMinimumAirFlowRate);
    void autosizeFixedMinimumAirFlowRate();
    void resetFixedMinimumAirFlowRate();

    bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

    void resetMinimumAirFlowFractionSchedule();

    bool setMinimumAirFlowTurndownSchedule(Schedule& schedule);

    void resetMinimumAirFlowTurndownSchedule();

    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumAirFlowRate() const;
    boost::optional<double> autosizedConstantMinimumAirFlowFraction() const;
    boost::optional<double> autosizedFixedMinimumAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctVAVNoReheat_Impl;

    explicit AirTerminalSingleDuctVAVNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVNoReheat_Impl> impl);

    friend class detail::AirTerminalSingleDuctVAVNoReheat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVNoReheat");
  };

  /** \relates AirTerminalSingleDuctVAVNoReheat*/
  using OptionalAirTerminalSingleDuctVAVNoReheat = boost::optional<AirTerminalSingleDuctVAVNoReheat>;

  /** \relates AirTerminalSingleDuctVAVNoReheat*/
  using AirTerminalSingleDuctVAVNoReheatVector = std::vector<AirTerminalSingleDuctVAVNoReheat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP
