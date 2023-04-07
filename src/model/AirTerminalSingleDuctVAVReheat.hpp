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

    virtual ~AirTerminalSingleDuctVAVReheat() = default;
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
    OS_DEPRECATED std::string zoneMinimumAirFlowMethod();

    /** Sets the value of the MaximumAirFlowRate field.
   *  Options are FixedFlowRate and Scheduled.
   */
    bool setZoneMinimumAirFlowInputMethod(const std::string& value);
    /** deprecated **/
    OS_DEPRECATED bool setZoneMinimumAirFlowMethod(const std::string& value);

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
