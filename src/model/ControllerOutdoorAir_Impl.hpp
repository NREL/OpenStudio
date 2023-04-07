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

#ifndef MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP
#define MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CurveQuadratic;
  class ScheduleCompact;
  class AirLoopHVACOutdoorAirSystem;
  class ControllerMechanicalVentilation;
  class AirflowNetworkOutdoorAirflow;

  namespace detail {

    class MODEL_API ControllerOutdoorAir_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      ControllerOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ControllerOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      ControllerOutdoorAir_Impl(const ControllerOutdoorAir_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ControllerOutdoorAir_Impl() = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      ModelObject clone(Model model) const override;

      std::string getEconomizerControlType() const;
      bool setEconomizerControlType(const std::string& value);

      std::string getEconomizerControlActionType() const;
      bool setEconomizerControlActionType(const std::string& value);

      //get needs to return a boost optional double since "" is a valid input
      boost::optional<double> getEconomizerMaximumLimitDryBulbTemperature() const;
      bool setEconomizerMaximumLimitDryBulbTemperature(boost::optional<double> value);

      //get needs to return a boost optional double since "" is a valid input
      boost::optional<double> getEconomizerMaximumLimitEnthalpy() const;
      bool setEconomizerMaximumLimitEnthalpy(boost::optional<double> value);

      //get needs to return a boost optional double since "" is a valid input
      boost::optional<double> getEconomizerMaximumLimitDewpointTemperature() const;
      bool setEconomizerMaximumLimitDewpointTemperature(boost::optional<double> value);

      //QuadraticCurve getElectronicEnthalpyLimitCurve() const;
      //bool setElectronicEnthalpyLimitCurve(QuadraticCurve c);

      //get needs to return a boost optional double since "" is a valid input
      boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
      bool setEconomizerMinimumLimitDryBulbTemperature(boost::optional<double> value);

      std::string getLockoutType() const;
      bool setLockoutType(const std::string& value);

      std::string getMinimumLimitType() const;
      bool setMinimumLimitType(const std::string& value);

      boost::optional<bool> getHighHumidityControl() const;
      bool setHighHumidityControl(bool val);

      //Zone getHumidistatControlZone() const;
      //bool setHumidistatControlZone(Zone z)

      OptionalDouble getHighHumidityOutdoorAirFlowRatio() const;
      bool setHighHumidityOutdoorAirFlowRatio(double v);

      boost::optional<bool> getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const;
      bool setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v);

      OptionalString getHeatRecoveryBypassControlType() const;
      bool setHeatRecoveryBypassControlType(const std::string& v);

      CurveQuadratic getElectronicEnthalpyLimitCurve() const;

      ScheduleCompact getMinimumOutdoorAirSchedule() const;

      ScheduleCompact getMinimumFractionOfOutdoorAirSchedule() const;

      ScheduleCompact getMaximumFractionOfOutdoorAirSchedule() const;

      ScheduleCompact getTimeOfDayEconomizerControlSchedule() const;

      boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

      boost::optional<double> minimumOutdoorAirFlowRate() const;

      bool isMinimumOutdoorAirFlowRateAutosized() const;

      bool setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate);

      void autosizeMinimumOutdoorAirFlowRate();

      boost::optional<double> maximumOutdoorAirFlowRate() const;

      bool isMaximumOutdoorAirFlowRateAutosized() const;

      bool setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

      void autosizeMaximumOutdoorAirFlowRate();

      ControllerMechanicalVentilation controllerMechanicalVentilation() const;

      bool setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation);

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

      std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      AirflowNetworkOutdoorAirflow getAirflowNetworkOutdoorAirflow(const AirflowNetworkCrack& crack);
      boost::optional<AirflowNetworkOutdoorAirflow> airflowNetworkOutdoorAirflow() const;

      boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;

      boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

      void autosize();

      void applySizingValues();

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

     private:
      boost::optional<ControllerMechanicalVentilation> optionalControllerMechanicalVentilation() const;
      boost::optional<ModelObject> controllerMechanicalVentilationAsModelObject() const;
      bool setControllerMechanicalVentilationAsModelObject(const boost::optional<ModelObject>& modelObject);

      REGISTER_LOGGER("openstudio.model.ControllerOutdoorAir");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP
