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

#ifndef MODEL_CONTROLLEROUTDOORAIR_HPP
#define MODEL_CONTROLLEROUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ControllerOutdoorAir_Impl;

  }  // namespace detail

  class CurveQuadratic;
  class ScheduleCompact;
  class ControllerMechanicalVentilation;
  class AirLoopHVACOutdoorAirSystem;
  class AirflowNetworkOutdoorAirflow;
  class AirflowNetworkCrack;

  class MODEL_API ControllerOutdoorAir : public ParentObject
  {
   public:
    explicit ControllerOutdoorAir(const Model& model);

    virtual ~ControllerOutdoorAir() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ControllerOutdoorAir(const ControllerOutdoorAir& other) = default;
    ControllerOutdoorAir(ControllerOutdoorAir&& other) = default;
    ControllerOutdoorAir& operator=(const ControllerOutdoorAir&) = default;
    ControllerOutdoorAir& operator=(ControllerOutdoorAir&&) = default;

    static IddObjectType iddObjectType();

    boost::optional<Schedule> minimumOutdoorAirSchedule() const;
    bool setMinimumOutdoorAirSchedule(Schedule& schedule);
    void resetMinimumOutdoorAirSchedule();

    boost::optional<Schedule> minimumFractionofOutdoorAirSchedule() const;
    bool setMinimumFractionofOutdoorAirSchedule(Schedule& schedule);
    void resetMinimumFractionofOutdoorAirSchedule();

    boost::optional<Schedule> maximumFractionofOutdoorAirSchedule() const;
    bool setMaximumFractionofOutdoorAirSchedule(Schedule& schedule);
    void resetMaximumFractionofOutdoorAirSchedule();

    boost::optional<Schedule> timeofDayEconomizerControlSchedule() const;
    bool setTimeofDayEconomizerControlSchedule(Schedule& schedule);
    void resetTimeofDayEconomizerControlSchedule();

    boost::optional<double> minimumOutdoorAirFlowRate() const;
    bool isMinimumOutdoorAirFlowRateAutosized() const;
    bool setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate);
    void autosizeMinimumOutdoorAirFlowRate();

    boost::optional<double> maximumOutdoorAirFlowRate() const;
    bool isMaximumOutdoorAirFlowRateAutosized() const;
    bool setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);
    void autosizeMaximumOutdoorAirFlowRate();

    ControllerMechanicalVentilation controllerMechanicalVentilation() const;
    bool setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation);

    std::string getEconomizerControlType() const;
    bool setEconomizerControlType(const std::string& value);

    std::string getEconomizerControlActionType() const;
    bool setEconomizerControlActionType(const std::string& value);

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> getEconomizerMaximumLimitDryBulbTemperature() const;
    bool setEconomizerMaximumLimitDryBulbTemperature(double value);
    void resetEconomizerMaximumLimitDryBulbTemperature();

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> getEconomizerMaximumLimitEnthalpy() const;
    bool setEconomizerMaximumLimitEnthalpy(double value);
    void resetEconomizerMaximumLimitEnthalpy();

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> getEconomizerMaximumLimitDewpointTemperature() const;
    bool setEconomizerMaximumLimitDewpointTemperature(double value);
    void resetEconomizerMaximumLimitDewpointTemperature();

    //QuadraticCurve getElectronicEnthalpyLimitCurve() const;
    //bool setElectronicEnthalpyLimitCurve(QuadraticCurve c);

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
    bool setEconomizerMinimumLimitDryBulbTemperature(double value);
    void resetEconomizerMinimumLimitDryBulbTemperature();

    std::string getLockoutType() const;
    bool setLockoutType(const std::string& value);

    std::string getMinimumLimitType() const;
    bool setMinimumLimitType(const std::string& value);

    boost::optional<bool> getHighHumidityControl() const;
    bool setHighHumidityControl(bool val);

    //Zone getHumidistatControlZone() const;
    //bool setHumidistatControlZone(Zone z)

    boost::optional<double> getHighHumidityOutdoorAirFlowRatio() const;
    bool setHighHumidityOutdoorAirFlowRatio(double v);

    boost::optional<bool> getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const;
    bool setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v);

    boost::optional<std::string> getHeatRecoveryBypassControlType() const;
    bool setHeatRecoveryBypassControlType(const std::string& v);

    boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

    AirflowNetworkOutdoorAirflow getAirflowNetworkOutdoorAirflow(const AirflowNetworkCrack& crack);
    boost::optional<AirflowNetworkOutdoorAirflow> airflowNetworkOutdoorAirflow() const;

    virtual std::vector<openstudio::IdfObject> remove();

    boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;

    boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

    void autosize();

    void applySizingValues();

   protected:
    using ImplType = detail::ControllerOutdoorAir_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit ControllerOutdoorAir(std::shared_ptr<detail::ControllerOutdoorAir_Impl> impl);

   private:
    CurveQuadratic getElectronicEnthalpyLimitCurve() const;

    ScheduleCompact getMinimumOutdoorAirSchedule() const;

    ScheduleCompact getMinimumFractionOfOutdoorAirSchedule() const;

    ScheduleCompact getMaximumFractionOfOutdoorAirSchedule() const;

    //Controller:MechanicalVentilation getMechanicalVentilationController() const;

    ScheduleCompact getTimeOfDayEconomizerControlSchedule() const;

    ControllerOutdoorAir(const Handle& handle, const Model& model);

    REGISTER_LOGGER("openstudio.model.ControllerOutdoorAir");
  };

  using OptionalControllerOutdoorAir = boost::optional<ControllerOutdoorAir>;

  using ControllerOutdoorAirVector = std::vector<ControllerOutdoorAir>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLEROUTDOORAIR_HPP
