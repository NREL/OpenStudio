/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ScheduleFile.hpp"
#include "ScheduleFile_Impl.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Schedule_File_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/filetypes/CSVFile.hpp"

#include <unordered_map>

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleFile_Impl::ScheduleFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleFile::iddObjectType());
    }

    ScheduleFile_Impl::ScheduleFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleFile::iddObjectType());
    }

    ScheduleFile_Impl::ScheduleFile_Impl(const ScheduleFile_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ScheduleFile_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ScheduleFile_Impl::iddObjectType() const {
      return ScheduleFile::iddObjectType();
    }

    std::vector<ResourceObject> ScheduleFile_Impl::resources() const {
      std::vector<ResourceObject> result;
      result.push_back(externalFile());
      return result;
    }

    boost::optional<ScheduleTypeLimits> ScheduleFile_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_FileFields::ScheduleTypeLimitsName);
    }

    ExternalFile ScheduleFile_Impl::externalFile() const {
      auto value = getObject<ModelObject>().getModelObjectTarget<ExternalFile>(OS_Schedule_FileFields::ExternalFileName);
      OS_ASSERT(value);
      return value.get();
    }

    int ScheduleFile_Impl::columnNumber() const {
      boost::optional<int> value = getInt(OS_Schedule_FileFields::ColumnNumber, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ScheduleFile_Impl::rowstoSkipatTop() const {
      boost::optional<int> value = getInt(OS_Schedule_FileFields::RowstoSkipatTop, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<int> ScheduleFile_Impl::numberofHoursofData() const {
      return getInt(OS_Schedule_FileFields::NumberofHoursofData, true);
    }

    bool ScheduleFile_Impl::isNumberofHoursofDataDefaulted() const {
      return isEmpty(OS_Schedule_FileFields::NumberofHoursofData);
    }

    std::string ScheduleFile_Impl::columnSeparator() const {
      boost::optional<std::string> value = getString(OS_Schedule_FileFields::ColumnSeparator, true);
      OS_ASSERT(value);
      return value.get();
    }

    char ScheduleFile_Impl::columnSeparatorChar() const {
      static const std::unordered_map<std::string, char> lookup({{"Comma", ','}, {"Tab", '\t'}, {"Fixed", ' '}, {"Space", ' '}, {"Semicolon", ';'}});
      boost::optional<std::string> value = getString(OS_Schedule_FileFields::ColumnSeparator, true);
      OS_ASSERT(value);
      auto it = lookup.find(value.get());
      if (it == std::end(lookup)) {
        // Invalid separator
        return '\0';
      }
      return it->second;
    }

    bool ScheduleFile_Impl::isColumnSeparatorDefaulted() const {
      return isEmpty(OS_Schedule_FileFields::ColumnSeparator);
    }

    bool ScheduleFile_Impl::interpolatetoTimestep() const {
      boost::optional<std::string> value = getString(OS_Schedule_FileFields::InterpolatetoTimestep, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleFile_Impl::isInterpolatetoTimestepDefaulted() const {
      return isEmpty(OS_Schedule_FileFields::InterpolatetoTimestep);
    }

    boost::optional<std::string> ScheduleFile_Impl::minutesperItem() const {
      return getString(OS_Schedule_FileFields::MinutesperItem, true);
    }

    bool ScheduleFile_Impl::isMinutesperItemDefaulted() const {
      return isEmpty(OS_Schedule_FileFields::MinutesperItem);
    }

    bool ScheduleFile_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      bool result = setPointer(OS_Schedule_FileFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
      return result;
    }

    bool ScheduleFile_Impl::resetScheduleTypeLimits() {
      bool result = setString(OS_Schedule_FileFields::ScheduleTypeLimitsName, "");
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleFile_Impl::setColumnNumber(int columnNumber) {
      bool result = setInt(OS_Schedule_FileFields::ColumnNumber, columnNumber);
      return result;
    }

    bool ScheduleFile_Impl::setRowstoSkipatTop(int rowstoSkipatTop) {
      bool result = setInt(OS_Schedule_FileFields::RowstoSkipatTop, rowstoSkipatTop);
      return result;
    }

    bool ScheduleFile_Impl::setNumberofHoursofData(int numberofHours) {
      bool result = setInt(OS_Schedule_FileFields::NumberofHoursofData, numberofHours);
      return result;
    }

    bool ScheduleFile_Impl::setColumnSeparator(const std::string& columnSeparator) {
      bool result = setString(OS_Schedule_FileFields::ColumnSeparator, columnSeparator);
      return result;
    }

    void ScheduleFile_Impl::resetColumnSeparator() {
      bool result = setString(OS_Schedule_FileFields::ColumnSeparator, "");
      OS_ASSERT(result);
    }

    bool ScheduleFile_Impl::setInterpolatetoTimestep(bool interpolatetoTimestep) {
      bool result = false;
      if (interpolatetoTimestep) {
        result = setString(OS_Schedule_FileFields::InterpolatetoTimestep, "Yes");
      } else {
        result = setString(OS_Schedule_FileFields::InterpolatetoTimestep, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void ScheduleFile_Impl::resetInterpolatetoTimestep() {
      bool result = setString(OS_Schedule_FileFields::InterpolatetoTimestep, "");
      OS_ASSERT(result);
    }

    bool ScheduleFile_Impl::setMinutesperItem(const std::string& minutesperItem) {
      bool result = setString(OS_Schedule_FileFields::MinutesperItem, minutesperItem);
      return result;
    }

    void ScheduleFile_Impl::resetMinutesperItem() {
      bool result = setString(OS_Schedule_FileFields::MinutesperItem, "");
      OS_ASSERT(result);
    }

    boost::optional<CSVFile> ScheduleFile_Impl::csvFile() const {
      boost::optional<CSVFile> csvFile;
      ExternalFile externalFile = this->externalFile();
      csvFile = CSVFile::load(externalFile.filePath());
      return csvFile;
    }

    /* FIXME!
  openstudio::TimeSeries ScheduleFile_Impl::timeSeries(unsigned columnIndex) const
  { 
    // need to catch integers less than or equal to 0
    // need to ensure that first column is dateTimes
    
    boost::optional<CSVFile> csvFile;
    ExternalFile externalFile = this->externalFile();
    csvFile = CSVFile::load(externalFile.filePath());    
    
    std::vector<DateTime> dateTimes = csvFile->getColumnAsDateTimes(0);
    std::vector<double> values = csvFile->getColumnAsDoubleVector(columnIndex);
    Vector vectorValues(values.size());
    unsigned i = 0;
    for (double value : values) {
      vectorValues[i] = value;
      ++i;
    }

    Time delta = dateTimes[1] - dateTimes[0];
    Time intervalLength(delta.days(), delta.hours(), delta.minutes(), delta.seconds());
    TimeSeries result(dateTimes[0].date(), intervalLength, vectorValues, "");

    return result;
  }
  */

    /* FIXME!
  unsigned ScheduleFile_Impl::addTimeSeries(const openstudio::TimeSeries& timeSeries)
  {
    // need to ensure that first column is dateTimes
    // need to ensure that length of timeSeries equals length of dateTimes

    boost::optional<CSVFile> csvFile;
    ExternalFile externalFile = this->externalFile();
    csvFile = CSVFile::load(externalFile.filePath());
    unsigned columnIndex = csvFile->addColumn(timeSeries.values());
    // save?

    return columnIndex;
  }
  */

    openstudio::TimeSeries ScheduleFile_Impl::timeSeries() const {
      openstudio::TimeSeries result;
      /* FIXME!
    Date startDate(openstudio::MonthOfYear(this->startMonth()), this->startDay());
    Time intervalLength(0, 0, this->intervalLength());
    Vector values(this->numExtensibleGroups());
    unsigned i = 0;
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
      OptionalDouble x = group.getDouble(0);
      OS_ASSERT(x);
      values[i] = *x;
      ++i;
    }
    TimeSeries result(startDate, intervalLength, values, "");
    result.setOutOfRangeValue(this->outOfRangeValue());
    */
      return result;
    }

    bool ScheduleFile_Impl::setTimeSeries(const openstudio::TimeSeries& timeSeries) {
      /* FIXME!
    // check the interval
    boost::optional<openstudio::Time> intervalTime = timeSeries.intervalLength();
    if (!intervalTime) {
      return false;
    }
    // check the interval
    double intervalLength = intervalTime->totalMinutes();
    if (intervalLength - floor(intervalLength) > 0) {
      return false;
    }
    // check the interval
    if (intervalTime->totalDays() > 1) {
      return false;
    }
    // check that first report is whole number of intervals from start date
    DateTime firstReportDateTime = timeSeries.firstReportDateTime();
    Date startDate = firstReportDateTime.date();
    Time firstReportTime = firstReportDateTime.time();
    double numIntervalsToFirstReport = std::max(1.0, firstReportTime.totalMinutes() / intervalLength);
    if (numIntervalsToFirstReport - floor(numIntervalsToFirstReport) > 0) {
      return false;
    }
    // check the values
    openstudio::Vector values = timeSeries.values();
    for (const auto& value : values) {
      // Get the position
      int pos = &value - &values[0];
      // Check validity, cannot be NaN, Inf, etc
      if (std::isinf(value)) {
        LOG(Warn, "There is Infinity on position " << pos << " in the timeSeries provided for " << this->briefDescription());
        return false;
      } else if (std::isnan(value)) {
        LOG(Warn, "There is a NaN on position " << pos << " in the timeSeries provided for " << this->briefDescription());
        return false;
      }
    }
    // at this point we are going to change the object
    clearExtensibleGroups(false);
    // set the interval
    this->setIntervalLength(intervalLength, false);
    // set the start date
    this->setStartMonth(startDate.monthOfYear().value(), false);
    this->setStartDay(startDate.dayOfMonth(), false);
    // set the out of range value
    double outOfRangeValue = timeSeries.outOfRangeValue();
    // add in numIntervalsToFirstReport-1 outOfRangeValues to pad the timeseries
    for (unsigned i = 0; i < numIntervalsToFirstReport - 1; ++i) {
      std::vector<std::string> temp;
      temp.push_back(toString(outOfRangeValue));
      ModelExtensibleGroup group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());
    }
    // set the values
    for (unsigned i = 0; i < values.size(); ++i) {
      std::vector<std::string> temp;
      temp.push_back(toString(values[i]));
      ModelExtensibleGroup group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());
    }
    this->emitChangeSignals();
    return true;
    */
      return false;
    }

    void ScheduleFile_Impl::ensureNoLeapDays() {
      /* FIXME!
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_Schedule_FixedIntervalFields::StartMonth);
    if (month && (month.get() == 2)) {
      day = this->getInt(OS_Schedule_FixedIntervalFields::StartDay);
      if (day && (day.get() == 29)) {
        this->setInt(OS_Schedule_FixedIntervalFields::StartDay, 28);
      }
    }*/
    }

  }  // namespace detail

  ScheduleFile::ScheduleFile(const ExternalFile& externalfile, int column, int rowsToSkip)
    : ScheduleInterval(ScheduleFile::iddObjectType(), externalfile.model()) {
    OS_ASSERT(getImpl<detail::ScheduleFile_Impl>());
    bool ok;
    ok = setPointer(OS_Schedule_FileFields::ExternalFileName, externalfile.handle());
    OS_ASSERT(ok);
    ok = setColumnNumber(column);
    OS_ASSERT(ok);
    ok = setRowstoSkipatTop(rowsToSkip);
    OS_ASSERT(ok);
  }

  IddObjectType ScheduleFile::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Schedule_File);
  }

  std::vector<std::string> ScheduleFile::minutesperItemValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Schedule_FileFields::MinutesperItem);
  }

  boost::optional<ScheduleTypeLimits> ScheduleFile::scheduleTypeLimits() const {
    return getImpl<detail::ScheduleFile_Impl>()->scheduleTypeLimits();
  }

  ExternalFile ScheduleFile::externalFile() const {
    return getImpl<detail::ScheduleFile_Impl>()->externalFile();
  }

  int ScheduleFile::columnNumber() const {
    return getImpl<detail::ScheduleFile_Impl>()->columnNumber();
  }

  int ScheduleFile::rowstoSkipatTop() const {
    return getImpl<detail::ScheduleFile_Impl>()->rowstoSkipatTop();
  }

  boost::optional<int> ScheduleFile::numberofHoursofData() const {
    return getImpl<detail::ScheduleFile_Impl>()->numberofHoursofData();
  }

  bool ScheduleFile::isNumberofHoursofDataDefaulted() const {
    return getImpl<detail::ScheduleFile_Impl>()->isNumberofHoursofDataDefaulted();
  }

  std::string ScheduleFile::columnSeparator() const {
    return getImpl<detail::ScheduleFile_Impl>()->columnSeparator();
  }

  bool ScheduleFile::isColumnSeparatorDefaulted() const {
    return getImpl<detail::ScheduleFile_Impl>()->isColumnSeparatorDefaulted();
  }

  bool ScheduleFile::interpolatetoTimestep() const {
    return getImpl<detail::ScheduleFile_Impl>()->interpolatetoTimestep();
  }

  bool ScheduleFile::isInterpolatetoTimestepDefaulted() const {
    return getImpl<detail::ScheduleFile_Impl>()->isInterpolatetoTimestepDefaulted();
  }

  boost::optional<std::string> ScheduleFile::minutesperItem() const {
    return getImpl<detail::ScheduleFile_Impl>()->minutesperItem();
  }

  bool ScheduleFile::isMinutesperItemDefaulted() const {
    return getImpl<detail::ScheduleFile_Impl>()->isMinutesperItemDefaulted();
  }

  boost::optional<CSVFile> ScheduleFile::csvFile() const {
    return getImpl<detail::ScheduleFile_Impl>()->csvFile();
  }

  /* FIXME!
openstudio::TimeSeries ScheduleFile::timeSeries(unsigned columnIndex) const {
  return getImpl<detail::ScheduleFile_Impl>()->timeSeries(columnIndex);
}
*/

  /* FIXME!
unsigned ScheduleFile::addTimeSeries(const openstudio::TimeSeries& timeSeries) {
  return getImpl<detail::ScheduleFile_Impl>()->addTimeSeries(timeSeries);
}
*/

  bool ScheduleFile::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    return getImpl<detail::ScheduleFile_Impl>()->setScheduleTypeLimits(scheduleTypeLimits);
  }

  bool ScheduleFile::resetScheduleTypeLimits() {
    return getImpl<detail::ScheduleFile_Impl>()->resetScheduleTypeLimits();
  }

  bool ScheduleFile::setColumnNumber(int columnNumber) {
    return getImpl<detail::ScheduleFile_Impl>()->setColumnNumber(columnNumber);
  }

  bool ScheduleFile::setRowstoSkipatTop(int rowstoSkipatTop) {
    return getImpl<detail::ScheduleFile_Impl>()->setRowstoSkipatTop(rowstoSkipatTop);
  }

  bool ScheduleFile::setNumberofHoursofData(int numberofHours) {
    return getImpl<detail::ScheduleFile_Impl>()->setNumberofHoursofData(numberofHours);
  }

  bool ScheduleFile::setColumnSeparator(const std::string& columnSeparator) {
    return getImpl<detail::ScheduleFile_Impl>()->setColumnSeparator(columnSeparator);
  }

  void ScheduleFile::resetColumnSeparator() {
    getImpl<detail::ScheduleFile_Impl>()->resetColumnSeparator();
  }

  bool ScheduleFile::setInterpolatetoTimestep(bool interpolatetoTimestep) {
    return getImpl<detail::ScheduleFile_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
  }

  void ScheduleFile::resetInterpolatetoTimestep() {
    getImpl<detail::ScheduleFile_Impl>()->resetInterpolatetoTimestep();
  }

  bool ScheduleFile::setMinutesperItem(const std::string& minutesperItem) {
    return getImpl<detail::ScheduleFile_Impl>()->setMinutesperItem(minutesperItem);
  }

  void ScheduleFile::resetMinutesperItem() {
    getImpl<detail::ScheduleFile_Impl>()->resetMinutesperItem();
  }

  /// @cond
  ScheduleFile::ScheduleFile(std::shared_ptr<detail::ScheduleFile_Impl> impl) : ScheduleInterval(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
