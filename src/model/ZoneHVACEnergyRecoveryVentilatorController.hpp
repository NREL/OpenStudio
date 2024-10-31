/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Schedule;

  namespace detail {

    class ZoneHVACEnergyRecoveryVentilatorController_Impl;

  }  // namespace detail

  /** ZoneHVACEnergyRecoveryVentilatorController is a ParentObject that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EnergyRecoveryVentilator:Controller'. */
  class MODEL_API ZoneHVACEnergyRecoveryVentilatorController : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACEnergyRecoveryVentilatorController(const Model& model);

    virtual ~ZoneHVACEnergyRecoveryVentilatorController() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACEnergyRecoveryVentilatorController(const ZoneHVACEnergyRecoveryVentilatorController& other) = default;
    ZoneHVACEnergyRecoveryVentilatorController(ZoneHVACEnergyRecoveryVentilatorController&& other) = default;
    ZoneHVACEnergyRecoveryVentilatorController& operator=(const ZoneHVACEnergyRecoveryVentilatorController&) = default;
    ZoneHVACEnergyRecoveryVentilatorController& operator=(ZoneHVACEnergyRecoveryVentilatorController&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> exhaustAirTemperatureLimitValues();

    static std::vector<std::string> exhaustAirEnthalpyLimitValues();

    /** @name Getters */
    //@{

    boost::optional<double> temperatureHighLimit() const;

    boost::optional<double> temperatureLowLimit() const;

    boost::optional<double> enthalpyHighLimit() const;

    boost::optional<double> dewpointTemperatureLimit() const;

    boost::optional<Curve> electronicEnthalpyLimitCurve() const;

    std::string exhaustAirTemperatureLimit() const;

    std::string exhaustAirEnthalpyLimit() const;

    boost::optional<Schedule> timeofDayEconomizerFlowControlSchedule() const;

    bool highHumidityControlFlag() const;

    double highHumidityOutdoorAirFlowRatio() const;

    bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio() const;

    //@}
    /** @name Setters */
    //@{

    bool setTemperatureHighLimit(double temperatureHighLimit);

    void resetTemperatureHighLimit();

    bool setTemperatureLowLimit(double temperatureLowLimit);

    void resetTemperatureLowLimit();

    bool setEnthalpyHighLimit(double enthalpyHighLimit);

    void resetEnthalpyHighLimit();

    bool setDewpointTemperatureLimit(double dewpointTemperatureLimit);

    void resetDewpointTemperatureLimit();

    bool setElectronicEnthalpyLimitCurve(const Curve& curve);

    void resetElectronicEnthalpyLimitCurve();

    bool setExhaustAirTemperatureLimit(const std::string& exhaustAirTemperatureLimit);

    bool setExhaustAirEnthalpyLimit(const std::string& exhaustAirEnthalpyLimit);

    bool setTimeofDayEconomizerFlowControlSchedule(Schedule& schedule);

    void resetTimeofDayEconomizerFlowControlSchedule();

    bool setHighHumidityControlFlag(bool highHumidityControlFlag);

    bool setHighHumidityOutdoorAirFlowRatio(double highHumidityOutdoorAirFlowRatio);

    bool setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACEnergyRecoveryVentilatorController_Impl;

    explicit ZoneHVACEnergyRecoveryVentilatorController(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl> impl);

    friend class detail::ZoneHVACEnergyRecoveryVentilatorController_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilatorController");
  };

  /** \relates ZoneHVACEnergyRecoveryVentilatorController*/
  using OptionalZoneHVACEnergyRecoveryVentilatorController = boost::optional<ZoneHVACEnergyRecoveryVentilatorController>;

  /** \relates ZoneHVACEnergyRecoveryVentilatorController*/
  using ZoneHVACEnergyRecoveryVentilatorControllerVector = std::vector<ZoneHVACEnergyRecoveryVentilatorController>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_HPP
