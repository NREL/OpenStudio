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

#ifndef MODEL_DEFAULTSCHEDULESET_IMPL_HPP
#define MODEL_DEFAULTSCHEDULESET_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class DefaultScheduleType;
  class DefaultScheduleSet;

  namespace detail {

    /** DefaultScheduleSet_Impl is a ResourceObject_Impl that is the implementation class for DefaultScheduleSet.*/
    class MODEL_API DefaultScheduleSet_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultScheduleSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultScheduleSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultScheduleSet_Impl(const DefaultScheduleSet_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultScheduleSet_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      /** @name Getters */
      //@{

      boost::optional<Schedule> hoursofOperationSchedule() const;

      boost::optional<Schedule> numberofPeopleSchedule() const;

      boost::optional<Schedule> peopleActivityLevelSchedule() const;

      boost::optional<Schedule> lightingSchedule() const;

      boost::optional<Schedule> electricEquipmentSchedule() const;

      boost::optional<Schedule> gasEquipmentSchedule() const;

      boost::optional<Schedule> hotWaterEquipmentSchedule() const;

      boost::optional<Schedule> steamEquipmentSchedule() const;

      boost::optional<Schedule> otherEquipmentSchedule() const;

      boost::optional<Schedule> ventilationperPersonSchedule() const;

      boost::optional<Schedule> ventilationperFloorAreaSchedule() const;

      boost::optional<Schedule> infiltrationSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setHoursofOperationSchedule(Schedule& schedule);

      void resetHoursofOperationSchedule();

      bool setNumberofPeopleSchedule(Schedule& schedule);

      void resetNumberofPeopleSchedule();

      bool setPeopleActivityLevelSchedule(Schedule& schedule);

      void resetPeopleActivityLevelSchedule();

      bool setLightingSchedule(Schedule& schedule);

      void resetLightingSchedule();

      bool setElectricEquipmentSchedule(Schedule& schedule);

      void resetElectricEquipmentSchedule();

      bool setGasEquipmentSchedule(Schedule& schedule);

      void resetGasEquipmentSchedule();

      bool setHotWaterEquipmentSchedule(Schedule& schedule);

      void resetHotWaterEquipmentSchedule();

      bool setSteamEquipmentSchedule(Schedule& schedule);

      void resetSteamEquipmentSchedule();

      bool setOtherEquipmentSchedule(Schedule& schedule);

      void resetOtherEquipmentSchedule();

      bool setVentilationperPersonSchedule(Schedule& schedule);

      void resetVentilationperPersonSchedule();

      bool setVentilationperFloorAreaSchedule(Schedule& schedule);

      void resetVentilationperFloorAreaSchedule();

      bool setInfiltrationSchedule(Schedule& schedule);

      void resetInfiltrationSchedule();

      //@}
      /** @name Other */
      //@{

      /// Returns the default schedule of a particular type.
      boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

      /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
      void merge(const DefaultScheduleSet& other);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DefaultScheduleSet");

      boost::optional<ModelObject> hoursofOperationScheduleAsModelObject() const;
      boost::optional<ModelObject> numberofPeopleScheduleAsModelObject() const;
      boost::optional<ModelObject> peopleActivityLevelScheduleAsModelObject() const;
      boost::optional<ModelObject> lightingScheduleAsModelObject() const;
      boost::optional<ModelObject> electricEquipmentScheduleAsModelObject() const;
      boost::optional<ModelObject> gasEquipmentScheduleAsModelObject() const;
      boost::optional<ModelObject> hotWaterEquipmentScheduleAsModelObject() const;
      boost::optional<ModelObject> steamEquipmentScheduleAsModelObject() const;
      boost::optional<ModelObject> otherEquipmentScheduleAsModelObject() const;
      boost::optional<ModelObject> ventilationperPersonScheduleAsModelObject() const;
      boost::optional<ModelObject> ventilationperFloorAreaScheduleAsModelObject() const;
      boost::optional<ModelObject> infiltrationScheduleAsModelObject() const;

      bool setHoursofOperationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setNumberofPeopleScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPeopleActivityLevelScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setLightingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setElectricEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setGasEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHotWaterEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setInfiltrationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSteamEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setOtherEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSCHEDULESET_IMPL_HPP
