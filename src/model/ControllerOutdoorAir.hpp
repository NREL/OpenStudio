/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLEROUTDOORAIR_HPP
#define MODEL_CONTROLLEROUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ControllerOutdoorAir_Impl;

  }  // namespace detail

  class Curve;
  class ScheduleCompact;
  class ControllerMechanicalVentilation;
  class AirLoopHVACOutdoorAirSystem;
  class AirflowNetworkOutdoorAirflow;
  class AirflowNetworkCrack;
  class ThermalZone;

  class MODEL_API ControllerOutdoorAir : public ParentObject
  {
   public:
    explicit ControllerOutdoorAir(const Model& model);

    virtual ~ControllerOutdoorAir() override = default;
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

    boost::optional<Curve> electronicEnthalpyLimitCurve() const;
    bool setElectronicEnthalpyLimitCurve(const Curve& curve);
    void resetElectronicEnthalpyLimitCurve();

    //get needs to return a boost optional double since "" is a valid input
    boost::optional<double> getEconomizerMinimumLimitDryBulbTemperature() const;
    bool setEconomizerMinimumLimitDryBulbTemperature(double value);
    void resetEconomizerMinimumLimitDryBulbTemperature();

    std::string getLockoutType() const;
    bool setLockoutType(const std::string& value);

    std::string getMinimumLimitType() const;
    bool setMinimumLimitType(const std::string& value);

    boost::optional<bool> getHighHumidityControl() const;
    OS_DEPRECATED(3, 8, 0) bool setHighHumidityControl(bool val);

    boost::optional<ThermalZone> humidistatControlZone() const;
    bool setHumidistatControlZone(const ThermalZone& thermalZone);
    void resetHumidistatControlZone();

    double getHighHumidityOutdoorAirFlowRatio() const;
    bool setHighHumidityOutdoorAirFlowRatio(double v);

    bool getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const;
    bool setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v);

    boost::optional<std::string> getHeatRecoveryBypassControlType() const;
    bool setHeatRecoveryBypassControlType(const std::string& v);

    std::string economizerOperationStaging() const;
    bool setEconomizerOperationStaging(const std::string& v);

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
    ControllerOutdoorAir(const Handle& handle, const Model& model);

    REGISTER_LOGGER("openstudio.model.ControllerOutdoorAir");
  };

  using OptionalControllerOutdoorAir = boost::optional<ControllerOutdoorAir>;

  using ControllerOutdoorAirVector = std::vector<ControllerOutdoorAir>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLEROUTDOORAIR_HPP
