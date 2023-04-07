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

#ifndef MODEL_SCHEDULEDAY_IMPL_HPP
#define MODEL_SCHEDULEDAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase_Impl.hpp"

#include "../utilities/time/Time.hpp"

namespace openstudio {

namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    /** ScheduleDay_Impl is a ResourceObject_Impl that is the implementation class for ScheduleDay.*/
    class MODEL_API ScheduleDay_Impl : public ScheduleBase_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleDay_Impl(const ScheduleDay_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleDay_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<IdfObject> remove() override;

      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      bool isScheduleTypeLimitsDefaulted() const;

      bool interpolatetoTimestep() const;

      bool isInterpolatetoTimestepDefaulted() const;

      /// Returns a vector of times marking the end value intervals.
      /// These times will be in order and have the same number of elements as values.
      /// All times will be less than or equal to 1 day.
      std::vector<openstudio::Time> times() const;

      /// Returns a vector of values in the same order and with the same number of elements as times.
      virtual std::vector<double> values() const override;

      /// Returns the value in effect at the given time.  If time is less than 0 days or greater than 1 day, 0 is returned.
      double getValue(const openstudio::Time& time) const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setInterpolatetoTimestep(bool interpolatetoTimestep);

      void resetInterpolatetoTimestep();

      /// Sets the given week schedule to be in effect until (inclusive) the date given starting the day after the
      /// previous date marker or January 1st if no previous date marker exists.
      /// Returns false if time is less than 0 days or greater than 1 day.  Replaces existing value
      /// for same time if it exists.
      bool addValue(const openstudio::Time& untilTime, double value);

      boost::optional<double> removeValue(const openstudio::Time& time);

      /// Clear all values from this schedule.
      void clearValues();

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     protected:
      virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const override;

      virtual bool okToResetScheduleTypeLimits() const override;

      //private slots:
     private:
      void clearCachedVariables();

     private:
      REGISTER_LOGGER("openstudio.model.ScheduleDay");

      mutable boost::optional<std::vector<openstudio::Time>> m_cachedTimes;
      mutable boost::optional<std::vector<double>> m_cachedValues;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEDAY_IMPL_HPP
