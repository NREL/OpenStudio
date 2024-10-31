/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP
#define MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP

#include "Thermostat_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {
    class MODEL_API ThermostatSetpointDualSetpoint_Impl : public Thermostat_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ThermostatSetpointDualSetpoint_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ThermostatSetpointDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ThermostatSetpointDualSetpoint_Impl(const ThermostatSetpointDualSetpoint_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ThermostatSetpointDualSetpoint_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

      boost::optional<Schedule> coolingSetpointTemperatureSchedule() const;

      double temperatureDifferenceBetweenCutoutAndSetpoint() const;
      bool isTemperatureDifferenceBetweenCutoutAndSetpointDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

      void resetHeatingSetpointTemperatureSchedule();

      bool setCoolingSetpointTemperatureSchedule(Schedule& schedule);

      void resetCoolingSetpointTemperatureSchedule();

      bool setTemperatureDifferenceBetweenCutoutAndSetpoint(double deltaT);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ThermostatSetpointDualSetpoint");

      boost::optional<ModelObject> heatingSetpointTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingSetpointTemperatureScheduleAsModelObject() const;

      bool setHeatingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP
