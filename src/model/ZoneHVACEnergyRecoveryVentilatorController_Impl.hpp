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

      virtual ~ZoneHVACEnergyRecoveryVentilatorController_Impl() = default;

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
