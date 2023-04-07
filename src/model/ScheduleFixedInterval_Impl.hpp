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

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ScheduleFixedInterval_Impl is a ScheduleInterval_Impl that is the implementation class for ScheduleFixedInterval.*/
    class MODEL_API ScheduleFixedInterval_Impl : public ScheduleInterval_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleFixedInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleFixedInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleFixedInterval_Impl(const ScheduleFixedInterval_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleFixedInterval_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      virtual openstudio::TimeSeries timeSeries() const override;

      virtual bool setTimeSeries(const openstudio::TimeSeries& timeSeries) override;

      //@}
      /** @name Getters */
      //@{

      bool interpolatetoTimestep() const;

      bool isInterpolatetoTimestepDefaulted() const;

      bool translatetoScheduleFile() const;

      bool isTranslatetoScheduleFileDefaulted() const;

      int intervalLength() const;

      double outOfRangeValue() const;

      bool isOutOfRangeValueDefaulted() const;

      int startMonth() const;

      int startDay() const;

      //@}
      /** @name Setters */
      //@{

      bool setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod = true);

      void resetInterpolatetoTimestep(bool driverMethod = true);

      bool setTranslatetoScheduleFile(bool translatetoScheduleFile, bool driverMethod = true);

      void resetTranslatetoScheduleFile(bool driverMethod = true);

      bool setIntervalLength(int intervalLength, bool driverMethod = true);

      bool setOutOfRangeValue(double outOfRangeValue, bool driverMethod = true);

      void resetOutOfRangeValue(bool driverMethod = true);

      bool setStartMonth(int startMonth, bool driverMethod = true);

      bool setStartDay(int startDay, bool driverMethod = true);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP
