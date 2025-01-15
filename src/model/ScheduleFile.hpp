/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~ScheduleFile() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleFile(const ScheduleFile& other) = default;
    ScheduleFile(ScheduleFile&& other) = default;
    ScheduleFile& operator=(const ScheduleFile&) = default;
    ScheduleFile& operator=(ScheduleFile&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> columnSeparatorValues();
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

    bool translateFileWithRelativePath() const;

    bool isTranslateFileWithRelativePathDefaulted() const;

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

    OS_DEPRECATED(3, 2, 0) bool setMinutesperItem(const std::string& minutesperItem);

    bool setMinutesperItem(int minutesperItem);

    void resetMinutesperItem();

    bool setAdjustScheduleforDaylightSavings(bool adjustScheduleforDaylightSavings);

    void resetAdjustScheduleforDaylightSavings();

    /* FIXME! unsigned addTimeSeries(const openstudio::TimeSeries& timeSeries); */

    bool setTranslateFileWithRelativePath(bool translateFileWithRelativePath);

    void resetTranslateFileWithRelativePath();

    //@}
    /** @name Other */
    //@{

    // Depending on the value of 'Translate File With Relative Path', returns an absolute or a relative path
    openstudio::path translatedFilePath() const;

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
