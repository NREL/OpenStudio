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
