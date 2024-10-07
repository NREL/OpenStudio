/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_HPP

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "ModelAPI.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class AirTerminalSingleDuctConstantVolumeNoReheat_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctConstantVolumeNoReheat is an interface to the EnergyPlus IDD object
 *  named "AirTerminal:SingleDuct:ConstantVolume:NoReheat"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  AirTerminal:SingleDuct:ConstantVolume:NoReheat objects in energyplus.
 */
  class MODEL_API AirTerminalSingleDuctConstantVolumeNoReheat : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new AirTerminalSingleDuctConstantVolumeNoReheat object and places it inside the
   *  model.  The object is fully initialized with all companion objects. */
    AirTerminalSingleDuctConstantVolumeNoReheat(const Model& model, Schedule& availabilitySchedule);

    virtual ~AirTerminalSingleDuctConstantVolumeNoReheat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctConstantVolumeNoReheat(const AirTerminalSingleDuctConstantVolumeNoReheat& other) = default;
    AirTerminalSingleDuctConstantVolumeNoReheat(AirTerminalSingleDuctConstantVolumeNoReheat&& other) = default;
    AirTerminalSingleDuctConstantVolumeNoReheat& operator=(const AirTerminalSingleDuctConstantVolumeNoReheat&) = default;
    AirTerminalSingleDuctConstantVolumeNoReheat& operator=(AirTerminalSingleDuctConstantVolumeNoReheat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** Returns the Schedule referred to by the AvailabilityScheduleName field. **/
    Schedule availabilitySchedule() const;

    /** Sets the Schedule referred to by the AvailabilityScheduleName field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> maximumAirFlowRate() const;

    bool isMaximumAirFlowRateAutosized() const;

    bool setMaximumAirFlowRate(double maximumAirFlowRate);

    void autosizeMaximumAirFlowRate();

    boost::optional<double> autosizedMaximumAirFlowRate() const;

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @cond
    using ImplType = detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl;

    explicit AirTerminalSingleDuctConstantVolumeNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeNoReheat");
    /// @endcond
  };

  using OptionalAirTerminalSingleDuctConstantVolumeNoReheat = boost::optional<AirTerminalSingleDuctConstantVolumeNoReheat>;

  using AirTerminalSingleDuctConstantVolumeNoReheatVector = std::vector<AirTerminalSingleDuctConstantVolumeNoReheat>;

  // In E+ 9.0.0, ATU SingleDuctUncontrolled was renamed to SingleDuctConstantVolumeNoReheat
  // To be more consistent with the naming convention of others ATU. We typedef for backwards compatibility
  using AirTerminalSingleDuctUncontrolled = AirTerminalSingleDuctConstantVolumeNoReheat;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMENOREHEAT_HPP
