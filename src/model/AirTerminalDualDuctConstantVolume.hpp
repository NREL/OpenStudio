/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP
#define MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;

  namespace detail {

    class AirTerminalDualDuctConstantVolume_Impl;

  }  // namespace detail

  /** AirTerminalDualDuctConstantVolume is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:ConstantVolume'. */
  class MODEL_API AirTerminalDualDuctConstantVolume : public Mixer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalDualDuctConstantVolume(const Model& model);

    virtual ~AirTerminalDualDuctConstantVolume() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalDualDuctConstantVolume(const AirTerminalDualDuctConstantVolume& other) = default;
    AirTerminalDualDuctConstantVolume(AirTerminalDualDuctConstantVolume&& other) = default;
    AirTerminalDualDuctConstantVolume& operator=(const AirTerminalDualDuctConstantVolume&) = default;
    AirTerminalDualDuctConstantVolume& operator=(AirTerminalDualDuctConstantVolume&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

    // Note Schedules are passed by reference, not const reference.
    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    //@}
    /** @name Other */
    //@{

    /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
   * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
   * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
   * but here in the terminal interface we use the language for transparency with the idf.
   */
    boost::optional<Node> hotAirInletNode() const;

    /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
   * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
   */
    boost::optional<Node> coldAirInletNode() const;

    /* Queries the autosized maximum terminal air flow rate from the SQL file */
    boost::optional<double> autosizedMaximumAirFlowRate() const;

    /* Autosize all autosizeable fields */
    // void autosize();

    /* Hard applies the autosized values from SQL file */
    // void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalDualDuctConstantVolume_Impl;

    explicit AirTerminalDualDuctConstantVolume(std::shared_ptr<detail::AirTerminalDualDuctConstantVolume_Impl> impl);

    friend class detail::AirTerminalDualDuctConstantVolume_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctConstantVolume");
  };

  /** \relates AirTerminalDualDuctConstantVolume*/
  using OptionalAirTerminalDualDuctConstantVolume = boost::optional<AirTerminalDualDuctConstantVolume>;

  /** \relates AirTerminalDualDuctConstantVolume*/
  using AirTerminalDualDuctConstantVolumeVector = std::vector<AirTerminalDualDuctConstantVolume>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP
