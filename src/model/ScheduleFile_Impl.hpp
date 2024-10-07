/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEFILE_IMPL_HPP
#define MODEL_SCHEDULEFILE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval_Impl.hpp"
#include "../utilities/filetypes/CSVFile.hpp"

namespace openstudio {

namespace model {

  class ScheduleTypeLimits;
  class ExternalFile;

  namespace detail {

    /** ScheduleFile_Impl is a ScheduleBase_Impl that is the implementation class for ScheduleFile.*/
    class MODEL_API ScheduleFile_Impl : public ScheduleInterval_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleFile_Impl(const ScheduleFile_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleFile_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ResourceObject> resources() const override;

      // These are pure virtual methods that were defined in ScheduleBase_Impl

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;
      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;
      virtual bool resetScheduleTypeLimits() override;

      //@}
      /** @name Getters */
      //@{

      ExternalFile externalFile() const;

      int columnNumber() const;

      int rowstoSkipatTop() const;

      int numberofHoursofData() const;

      bool isNumberofHoursofDataDefaulted() const;

      std::string columnSeparator() const;

      char columnSeparatorChar() const;

      bool isColumnSeparatorDefaulted() const;

      bool interpolatetoTimestep() const;

      bool isInterpolatetoTimestepDefaulted() const;

      int minutesperItem() const;

      bool isMinutesperItemDefaulted() const;

      bool adjustScheduleforDaylightSavings() const;

      bool isAdjustScheduleforDaylightSavingsDefaulted() const;

      virtual openstudio::TimeSeries timeSeries() const override;

      /* FIXME! openstudio::TimeSeries timeSeries(unsigned columnIndex) const; */

      boost::optional<CSVFile> csvFile() const;

      bool translateFileWithRelativePath() const;

      bool isTranslateFileWithRelativePathDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setColumnNumber(int columnNumber);

      bool setRowstoSkipatTop(int rowstoSkipatTop);

      bool setNumberofHoursofData(int numberofHours);

      bool setColumnSeparator(const std::string& columnSeparator);

      void resetColumnSeparator();

      bool setInterpolatetoTimestep(bool interpolatetoTimestep);

      void resetInterpolatetoTimestep();

      bool setMinutesperItem(int minutesperItem);

      void resetMinutesperItem();

      bool setAdjustScheduleforDaylightSavings(bool adjustScheduleforDaylightSavings);

      void resetAdjustScheduleforDaylightSavings();

      virtual bool setTimeSeries(const openstudio::TimeSeries& timeSeries) override;

      /* FIXME! unsigned addTimeSeries(const openstudio::TimeSeries& timeSeries); */

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      bool setTranslateFileWithRelativePath(bool translateFileWithRelativePath);

      void resetTranslateFileWithRelativePath();

      //@}
      /** @name Other */
      //@{

      openstudio::path translatedFilePath() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleFile");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFILE_IMPL_HPP
