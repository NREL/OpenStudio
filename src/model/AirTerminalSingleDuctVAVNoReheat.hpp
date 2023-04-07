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

    virtual ~AirTerminalSingleDuctVAVNoReheat() = default;
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
