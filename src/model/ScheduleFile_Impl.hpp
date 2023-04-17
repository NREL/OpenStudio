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

      virtual ~ScheduleFile_Impl() = default;

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

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleFile");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFILE_IMPL_HPP
