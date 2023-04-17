/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~AirTerminalSingleDuctConstantVolumeNoReheat() = default;
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
