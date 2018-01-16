/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_YEARDESCRIPTION_IMPL_HPP
#define MODEL_YEARDESCRIPTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

namespace detail {

  /** YearDescription_Impl is a ParentObject_Impl that is the implementation class for YearDescription.*/
  class MODEL_API YearDescription_Impl : public ParentObject_Impl {






   public:

    /** @name Constructors and Destructors */
    //@{

    YearDescription_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    YearDescription_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    YearDescription_Impl(const YearDescription_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    virtual ~YearDescription_Impl() {}

    //@}
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    /** @name Getters */
    //@{

    boost::optional<int> calendarYear() const;

    std::string dayofWeekforStartDay() const;

    bool isDayofWeekforStartDayDefaulted() const;

    bool isLeapYear() const;

    bool isIsLeapYearDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCalendarYear(boost::optional<int> calendarYear);

    void resetCalendarYear();

    bool setDayofWeekforStartDay(std::string dayofWeekforStartDay);

    void resetDayofWeekforStartDay();

    bool setIsLeapYear(bool isLeapYear);

    void resetIsLeapYear();

    //@}

    /// Returns assumed year for date calculations.
    int assumedYear() const;

    /// Returns a date which may be used in this model.
    /// Will throw if dayOfMonth is not appropriate for monthOfYear.
    openstudio::Date makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth);
    openstudio::Date makeDate(unsigned monthOfYear, unsigned dayOfMonth);

    /// nth weekday of month generators
    openstudio::Date makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear);

    /// day of year
    openstudio::Date makeDate(unsigned dayOfYear);

   protected:
   private:

    void updateModelLeapYear(bool wasLeapYear, bool isLeapYear);

    REGISTER_LOGGER("openstudio.model.YearDescription");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_YEARDESCRIPTION_IMPL_HPP

