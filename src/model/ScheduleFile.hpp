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

#ifndef MODEL_SCHEDULEFILE_HPP
#define MODEL_SCHEDULEFILE_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"
#include "../utilities/filetypes/CSVFile.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class ScheduleTypeLimits;
  class ExternalFile;

  namespace detail {

    class ScheduleFile_Impl;

  }  // namespace detail

  /** ScheduleFile is a ScheduleInterval that wraps the OpenStudio IDD object 'OS:Schedule:File'. */
  class MODEL_API ScheduleFile : public ScheduleInterval
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ScheduleFile(const ExternalFile& externalfile, int column = 1, int rowsToSkip = 0);

    ScheduleFile(const Model& model, const openstudio::path& filePath, int column = 1, int rowsToSkip = 0);

    virtual ~ScheduleFile() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleFile(const ScheduleFile& other) = default;
    ScheduleFile(ScheduleFile&& other) = default;
    ScheduleFile& operator=(const ScheduleFile&) = default;
    ScheduleFile& operator=(ScheduleFile&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> minutesperItemValues();

    /** @name Getters */
    //@{

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    ExternalFile externalFile() const;

    int columnNumber() const;

    int rowstoSkipatTop() const;

    // This should be returning `int` as it has a default
    boost::optional<int> numberofHoursofData() const;

    bool isNumberofHoursofDataDefaulted() const;

    std::string columnSeparator() const;

    bool isColumnSeparatorDefaulted() const;

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    // This should be returning `int` instead of boost::optional<std::string>...
    boost::optional<std::string> minutesperItem() const;

    bool isMinutesperItemDefaulted() const;

    bool adjustScheduleforDaylightSavings() const;

    bool isAdjustScheduleforDaylightSavingsDefaulted() const;

    /* FIXME! openstudio::TimeSeries timeSeries(unsigned columnIndex) const;*/

    boost::optional<CSVFile> csvFile() const;

    //@}
    /** @name Setters */
    //@{

    bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    bool resetScheduleTypeLimits();

    bool setColumnNumber(int columnNumber);

    bool setRowstoSkipatTop(int rowstoSkipatTop);

    bool setNumberofHoursofData(int numberofHours);

    bool setColumnSeparator(const std::string& columnSeparator);

    void resetColumnSeparator();

    bool setInterpolatetoTimestep(bool interpolatetoTimestep);

    void resetInterpolatetoTimestep();

    OS_DEPRECATED bool setMinutesperItem(const std::string& minutesperItem);

    bool setMinutesperItem(int minutesperItem);

    void resetMinutesperItem();

    bool setAdjustScheduleforDaylightSavings(bool adjustScheduleforDaylightSavings);

    void resetAdjustScheduleforDaylightSavings();

    /* FIXME! unsigned addTimeSeries(const openstudio::TimeSeries& timeSeries); */

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleFile_Impl;

    explicit ScheduleFile(std::shared_ptr<detail::ScheduleFile_Impl> impl);

    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleFile");
  };

  /** \relates ScheduleFile*/
  using OptionalScheduleFile = boost::optional<ScheduleFile>;

  /** \relates ScheduleFile*/
  using ScheduleFileVector = std::vector<ScheduleFile>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFILE_HPP
