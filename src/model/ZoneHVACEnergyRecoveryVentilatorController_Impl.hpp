/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;

  namespace detail {

    /** ZoneHVACEnergyRecoveryVentilatorController_Impl is a ParentObject_Impl that is the implementation class for ZoneHVACEnergyRecoveryVentilatorController.*/
    class MODEL_API ZoneHVACEnergyRecoveryVentilatorController_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACEnergyRecoveryVentilatorController_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACEnergyRecoveryVentilatorController_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACEnergyRecoveryVentilatorController_Impl(const ZoneHVACEnergyRecoveryVentilatorController_Impl& other, Model_Impl* model,
                                                      bool keepHandle);

      virtual ~ZoneHVACEnergyRecoveryVentilatorController_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
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

      bool setTemperatureHighLimit(boost::optional<double> temperatureHighLimit);

      void resetTemperatureHighLimit();

      bool setTemperatureLowLimit(boost::optional<double> temperatureLowLimit);

      void resetTemperatureLowLimit();

      bool setEnthalpyHighLimit(boost::optional<double> enthalpyHighLimit);

      void resetEnthalpyHighLimit();

      bool setDewpointTemperatureLimit(boost::optional<double> dewpointTemperatureLimit);

      void resetDewpointTemperatureLimit();

      bool setElectronicEnthalpyLimitCurve(const boost::optional<Curve>& curve);

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
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilatorController");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP
