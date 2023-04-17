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

#ifndef MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP
#define MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP

#include "ModelAPI.hpp"
#include "Thermostat_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** ZoneControlThermostatStagedDualSetpoint_Impl is a ModelObject_Impl that is the implementation class for ZoneControlThermostatStagedDualSetpoint.*/
    class MODEL_API ZoneControlThermostatStagedDualSetpoint_Impl : public Thermostat_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneControlThermostatStagedDualSetpoint_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneControlThermostatStagedDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneControlThermostatStagedDualSetpoint_Impl(const ZoneControlThermostatStagedDualSetpoint_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneControlThermostatStagedDualSetpoint_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      int numberofHeatingStages() const;

      boost::optional<Schedule> heatingTemperatureSetpointSchedule() const;

      double heatingThrottlingTemperatureRange() const;

      double stage1HeatingTemperatureOffset() const;

      double stage2HeatingTemperatureOffset() const;

      double stage3HeatingTemperatureOffset() const;

      double stage4HeatingTemperatureOffset() const;

      int numberofCoolingStages() const;

      boost::optional<Schedule> coolingTemperatureSetpointBaseSchedule() const;

      double coolingThrottlingTemperatureRange() const;

      double stage1CoolingTemperatureOffset() const;

      double stage2CoolingTemperatureOffset() const;

      double stage3CoolingTemperatureOffset() const;

      double stage4CoolingTemperatureOffset() const;

      //@}
      /** @name Setters */
      //@{

      bool setNumberofHeatingStages(int numberofHeatingStages);

      bool setHeatingTemperatureSetpointSchedule(Schedule& schedule);

      void resetHeatingTemperatureSetpointSchedule();

      bool setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange);

      bool setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset);

      bool setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset);

      bool setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset);

      bool setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset);

      bool setNumberofCoolingStages(int numberofCoolingStages);

      bool setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule);

      void resetCoolingTemperatureSetpointBaseSchedule();

      bool setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange);

      bool setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset);

      bool setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset);

      bool setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset);

      bool setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneControlThermostatStagedDualSetpoint");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP
