/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
#define MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP

#include "Thermostat.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ThermostatSetpointDualSetpoint_Impl;
  }

  class Schedule;

  /** ThermostatSetpointDualSetpoint is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:ThermostatSetpoint:DualSetpoint'. */
  class MODEL_API ThermostatSetpointDualSetpoint : public Thermostat
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermostatSetpointDualSetpoint(const Model& model);

    virtual ~ThermostatSetpointDualSetpoint() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& other) = default;
    ThermostatSetpointDualSetpoint(ThermostatSetpointDualSetpoint&& other) = default;
    ThermostatSetpointDualSetpoint& operator=(const ThermostatSetpointDualSetpoint&) = default;
    ThermostatSetpointDualSetpoint& operator=(ThermostatSetpointDualSetpoint&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    boost::optional<Schedule> coolingSetpointTemperatureSchedule() const;

    /** This ends up in the ZoneControl:Thermostat object during ForwardTranslation */
    double temperatureDifferenceBetweenCutoutAndSetpoint() const;
    bool isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted() const;

    /** \deprecated */
    boost::optional<Schedule> getHeatingSchedule() const;

    /** \deprecated */
    boost::optional<Schedule> getCoolingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setCoolingSetpointTemperatureSchedule(Schedule& schedule);

    void resetCoolingSetpointTemperatureSchedule();

    bool setTemperatureDifferenceBetweenCutoutAndSetpoint(double deltaT);

    /** \deprecated */
    bool setHeatingSchedule(Schedule& s);

    /** \deprecated */
    void resetHeatingSchedule();

    /** \deprecated */
    bool setCoolingSchedule(Schedule& s);

    /** \deprecated */
    void resetCoolingSchedule();

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    using ImplType = detail::ThermostatSetpointDualSetpoint_Impl;

    explicit ThermostatSetpointDualSetpoint(std::shared_ptr<detail::ThermostatSetpointDualSetpoint_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ThermostatSetpointDualSetpoint");
  };

  using OptionalThermostatSetpointDualSetpoint = boost::optional<ThermostatSetpointDualSetpoint>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOSTATSETPOINTDUALSETPOINT_HPP
