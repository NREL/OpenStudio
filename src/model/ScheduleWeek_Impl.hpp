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

#ifndef MODEL_SCHEDULEWEEK_IMPL_HPP
#define MODEL_SCHEDULEWEEK_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ScheduleDay;

  namespace detail {

    /** ScheduleWeek_Impl is a ResourceObject_Impl that is the implementation class for ScheduleWeek.*/
    class MODEL_API ScheduleWeek_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleWeek_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleWeek_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleWeek_Impl(const ScheduleWeek_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleWeek_Impl() = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ScheduleDay> sundaySchedule() const;

      boost::optional<ScheduleDay> mondaySchedule() const;

      boost::optional<ScheduleDay> tuesdaySchedule() const;

      boost::optional<ScheduleDay> wednesdaySchedule() const;

      boost::optional<ScheduleDay> thursdaySchedule() const;

      boost::optional<ScheduleDay> fridaySchedule() const;

      boost::optional<ScheduleDay> saturdaySchedule() const;

      boost::optional<ScheduleDay> holidaySchedule() const;

      boost::optional<ScheduleDay> summerDesignDaySchedule() const;

      boost::optional<ScheduleDay> winterDesignDaySchedule() const;

      boost::optional<ScheduleDay> customDay1Schedule() const;

      boost::optional<ScheduleDay> customDay2Schedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSundaySchedule(const ScheduleDay& schedule);

      bool setMondaySchedule(const ScheduleDay& schedule);

      bool setTuesdaySchedule(const ScheduleDay& schedule);

      bool setWednesdaySchedule(const ScheduleDay& schedule);

      bool setThursdaySchedule(const ScheduleDay& schedule);

      bool setFridaySchedule(const ScheduleDay& schedule);

      bool setSaturdaySchedule(const ScheduleDay& schedule);

      bool setHolidaySchedule(const ScheduleDay& schedule);

      bool setSummerDesignDaySchedule(const ScheduleDay& schedule);

      bool setWinterDesignDaySchedule(const ScheduleDay& schedule);

      bool setCustomDay1Schedule(const ScheduleDay& schedule);

      bool setCustomDay2Schedule(const ScheduleDay& schedule);

      //@}

      /// Set schedules for all days.
      bool setAllSchedules(const ScheduleDay& schedule);

      /// Set schedules for all weekdays.
      bool setWeekdaySchedule(const ScheduleDay& schedule);

      /// Set schedules for all weekends.
      bool setWeekendSchedule(const ScheduleDay& schedule);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleWeek");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEWEEK_IMPL_HPP
