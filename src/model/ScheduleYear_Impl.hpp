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

#ifndef MODEL_SCHEDULEYEAR_IMPL_HPP
#define MODEL_SCHEDULEYEAR_IMPL_HPP

#include "ModelAPI.hpp"
#include "Schedule_Impl.hpp"

namespace openstudio {

class Date;

namespace model {

  class ScheduleWeek;

  namespace detail {

    /** ScheduleYear_Impl is a Schedule_Impl that is the implementation class for ScheduleYear.*/
    class MODEL_API ScheduleYear_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleYear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleYear_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleYear_Impl(const ScheduleYear_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleYear_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      /** Returns empty vector, because individual ScheduleDay objects will check values() against
     *  bounds. */
      virtual std::vector<double> values() const override;

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      //@}
      /** @name Getters */
      //@{

      /// Returns a vector of dates marking the end of week schedules.
      /// These dates will be in order and have the same number of elements as scheduleWeeks.
      // DLM@20111018: This almost needs a RunPeriod as an argument to know the year description?
      std::vector<openstudio::Date> dates() const;

      /// Returns a vector of week schedules in the same order and with the same number of elements as dates.
      std::vector<ScheduleWeek> scheduleWeeks() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      /// Sets the given week schedule to be in effect until (inclusive) the date given starting the day after the
      /// previous date marker or January 1st if no previous date marker exists.  Replaces existing scheduleWeek
      /// for same date if it exists.
      bool addScheduleWeek(const openstudio::Date& untilDate, const ScheduleWeek& scheduleWeek);

      /// Clear all week schedules from this schedule.
      void clearScheduleWeeks();

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
      /** @name Other */
      //@{

      /// Returns the week schedule in effect at the given date.
      // DLM@20111018: Eventually allow "Nth day in Month" and other descriptors
      boost::optional<ScheduleWeek> getScheduleWeek(const openstudio::Date& date) const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleYear");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEYEAR_IMPL_HPP
