/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP
#define MODEL_CONTROLLEROUTDOORAIR_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class ScheduleCompact;
  class AirLoopHVACOutdoorAirSystem;
  class ControllerMechanicalVentilation;
  class AirflowNetworkOutdoorAirflow;
  class ThermalZone;

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
      virtual ~ControllerOutdoorAir_Impl() override = default;

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

      boost::optional<Curve> electronicEnthalpyLimitCurve() const;
      bool setElectronicEnthalpyLimitCurve(const Curve& curve);
      void resetElectronicEnthalpyLimitCurve();

      //get needs to return a boost optional double since "" is a valid input
      boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
      bool setEconomizerMinimumLimitDryBulbTemperature(boost::optional<double> value);

      std::string getLockoutType() const;
      bool setLockoutType(const std::string& value);

      std::string getMinimumLimitType() const;
      bool setMinimumLimitType(const std::string& value);

      boost::optional<bool> getHighHumidityControl() const;

      boost::optional<ThermalZone> humidistatControlZone() const;
      bool setHumidistatControlZone(const ThermalZone& thermalZone);
      void resetHumidistatControlZone();

      double getHighHumidityOutdoorAirFlowRatio() const;
      bool setHighHumidityOutdoorAirFlowRatio(double v);

      bool getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const;
      bool setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v);

      OptionalString getHeatRecoveryBypassControlType() const;
      bool setHeatRecoveryBypassControlType(const std::string& v);

      std::string economizerOperationStaging() const;
      bool setEconomizerOperationStaging(const std::string& v);

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
