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

#ifndef MODEL_ZONEMIXING_IMPL_HPP
#define MODEL_ZONEMIXING_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Space;
  class ThermalZone;

  namespace detail {

    /** ZoneMixing_Impl is a ModelObject_Impl that is the implementation class for ZoneMixing.*/
    class MODEL_API ZoneMixing_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneMixing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneMixing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneMixing_Impl(const ZoneMixing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneMixing_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool setParent(ParentObject& newParent) override;

      bool hardSize();

      //@}
      /** @name Getters */
      //@{

      boost::optional<ThermalZone> zone() const;
      boost::optional<Space> space() const;
      ModelObject zoneOrSpace() const;

      Schedule schedule() const;

      std::string designFlowRateCalculationMethod() const;

      boost::optional<double> designFlowRate() const;

      boost::optional<double> flowRateperFloorArea() const;

      boost::optional<double> flowRateperPerson() const;

      boost::optional<double> airChangesperHour() const;

      boost::optional<ThermalZone> sourceZone() const;
      boost::optional<Space> sourceSpace() const;
      boost::optional<ModelObject> sourceZoneOrSpace() const;

      boost::optional<double> deltaTemperature() const;

      boost::optional<Schedule> deltaTemperatureSchedule() const;

      boost::optional<Schedule> minimumReceivingTemperatureSchedule() const;

      boost::optional<Schedule> maximumReceivingTemperatureSchedule() const;

      boost::optional<Schedule> minimumSourceTemperatureSchedule() const;

      boost::optional<Schedule> maximumSourceTemperatureSchedule() const;

      boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

      boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setZone(const ThermalZone& zone);
      bool setSpace(const Space& space);

      bool setSchedule(Schedule& schedule);

      bool setDesignFlowRate(double designFlowRate);

      bool setFlowRateperFloorArea(double flowRateperFloorArea);

      bool setFlowRateperPerson(double flowRateperPerson);

      bool setAirChangesperHour(double airChangesperHour);

      bool setSourceZone(const ThermalZone& zone);
      bool setSourceSpace(const Space& space);
      void resetSourceZoneOrSpace();

      bool setDeltaTemperature(double deltaTemperature);

      void resetDeltaTemperature();

      bool setDeltaTemperatureSchedule(Schedule& schedule);

      void resetDeltaTemperatureSchedule();

      bool setMinimumReceivingTemperatureSchedule(Schedule& schedule);

      void resetMinimumReceivingTemperatureSchedule();

      bool setMaximumReceivingTemperatureSchedule(Schedule& schedule);

      void resetMaximumReceivingTemperatureSchedule();

      bool setMinimumSourceTemperatureSchedule(Schedule& schedule);

      void resetMinimumSourceTemperatureSchedule();

      bool setMaximumSourceTemperatureSchedule(Schedule& schedule);

      void resetMaximumSourceTemperatureSchedule();

      bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

      void resetMinimumOutdoorTemperatureSchedule();

      bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

      void resetMaximumOutdoorTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneMixing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEMIXING_IMPL_HPP
