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

    virtual ~AirTerminalSingleDuctConstantVolumeReheat() = default;
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
