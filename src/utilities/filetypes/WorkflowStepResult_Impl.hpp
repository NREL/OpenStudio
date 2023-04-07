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

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"
#include "../time/DateTime.hpp"
#include "../bcl/BCLMeasure.hpp"

namespace openstudio {
namespace detail {

  class UTILITIES_API WorkflowStepValue_Impl
  {
   public:
    WorkflowStepValue_Impl(const std::string& name, const Variant& value);

    std::string string() const;

    //@}
    /** @name Getters */
    //@{

    std::string name() const;

    std::string displayName() const;

    boost::optional<std::string> units() const;

    VariantType variantType() const;

    Variant valueAsVariant() const;

    bool valueAsBoolean() const;

    int valueAsInteger() const;

    double valueAsDouble() const;

    std::string valueAsString() const;

    //@}
    /** @name Setters */
    //@{

    void setName(const std::string& name);

    void setDisplayName(const std::string& displayName);
    void resetDisplayName();

    void setUnits(const std::string& units);
    void resetUnits();

    //@}

   private:
    REGISTER_LOGGER("openstudio.WorkflowStepValue");

    std::string m_name;
    Variant m_value;
    boost::optional<std::string> m_displayName;
    boost::optional<std::string> m_units;
  };

  class UTILITIES_API WorkflowStepResult_Impl
  {
   public:
    WorkflowStepResult_Impl();

    std::string string() const;

    boost::optional<DateTime> startedAt() const;

    boost::optional<DateTime> completedAt() const;

    boost::optional<MeasureType> measureType() const;

    boost::optional<std::string> measureName() const;

    boost::optional<std::string> measureId() const;

    boost::optional<UUID> measureUUID() const;

    boost::optional<std::string> measureVersionId() const;

    boost::optional<UUID> measureVersionUUID() const;

    boost::optional<DateTime> measureVersionModified() const;

    boost::optional<std::string> measureXmlChecksum() const;

    boost::optional<std::string> measureClassName() const;

    boost::optional<std::string> measureDisplayName() const;

    boost::optional<std::string> measureTaxonomy() const;

    boost::optional<StepResult> stepResult() const;

    boost::optional<std::string> stepInitialCondition() const;

    boost::optional<std::string> stepFinalCondition() const;

    std::vector<std::string> stepErrors() const;

    std::vector<std::string> stepWarnings() const;

    std::vector<std::string> stepInfo() const;

    std::vector<WorkflowStepValue> stepValues() const;

    std::vector<openstudio::path> stepFiles() const;

    boost::optional<std::string> stdOut() const;

    boost::optional<std::string> stdErr() const;

    // methods for backwards compatibility with OpenStudio 1.X
    StepResult value() const;
    std::vector<LogMessage> errors() const;
    std::vector<LogMessage> warnings() const;
    std::vector<LogMessage> info() const;
    boost::optional<LogMessage> initialCondition() const;
    boost::optional<LogMessage> finalCondition() const;
    std::vector<Attribute> attributes() const;

    void setStartedAt(const DateTime& dateTime);
    void resetStartedAt();

    void setCompletedAt(const DateTime& dateTime);
    void resetCompletedAt();

    bool setMeasureType(const MeasureType& measureType);
    void resetMeasureType();

    bool setMeasureName(const std::string& name);
    void resetMeasureName();

    bool setMeasureId(const std::string& id);
    void resetMeasureId();

    bool setMeasureUUID(const UUID& uuid);
    void resetMeasureUUID();

    bool setMeasureVersionId(const std::string& id);
    void resetMeasureVersionId();

    bool setMeasureVersionUUID(const UUID& uuid);
    void resetMeasureVersionUUID();

    bool setMeasureVersionModified(const DateTime& versionModified);
    void resetMeasureVersionModified();

    bool setMeasureXmlChecksum(const std::string& checksum);
    void resetMeasureXmlChecksum();

    bool setMeasureClassName(const std::string& className);
    void resetMeasureClassName();

    bool setMeasureDisplayName(const std::string& displayName);
    void resetMeasureDisplayName();

    bool setMeasureTaxonomy(const std::string& taxonomy);
    void resetMeasureTaxonomy();

    void setStepResult(const StepResult& result);
    void resetStepResult();

    void setStepInitialCondition(const std::string& initialCondition);
    void resetStepInitialCondition();

    void setStepFinalCondition(const std::string& finalCondition);
    void resetStepFinalCondition();

    void addStepError(const std::string& error);
    void resetStepErrors();

    void addStepWarning(const std::string& warning);
    void resetStepWarnings();

    void addStepInfo(const std::string& info);
    void resetStepInfo();

    void addStepValue(const WorkflowStepValue& value);
    void resetStepValues();

    void addStepFile(const openstudio::path& path);
    void resetStepFiles();

    void setStdOut(const std::string& stdOut);
    void resetStdOut();

    void setStdErr(const std::string& stdErr);
    void resetStdErr();

   private:
    REGISTER_LOGGER("openstudio.WorkflowStepResult");

    boost::optional<DateTime> m_startedAt;
    boost::optional<DateTime> m_completedAt;
    boost::optional<MeasureType> m_measureType;
    boost::optional<std::string> m_measureName;
    boost::optional<std::string> m_measureId;
    boost::optional<std::string> m_measureVersionId;
    boost::optional<std::string> m_measureVersionModified;
    boost::optional<std::string> m_measureXmlChecksum;
    boost::optional<std::string> m_measureClassName;
    boost::optional<std::string> m_measureDisplayName;
    boost::optional<std::string> m_measureTaxonomy;
    boost::optional<StepResult> m_stepResult;
    boost::optional<std::string> m_stepInitialCondition;
    boost::optional<std::string> m_stepFinalCondition;
    std::vector<std::string> m_stepErrors;
    std::vector<std::string> m_stepWarnings;
    std::vector<std::string> m_stepInfo;
    std::vector<WorkflowStepValue> m_stepValues;
    std::vector<openstudio::path> m_stepFiles;
    boost::optional<std::string> m_stdOut;
    boost::optional<std::string> m_stdErr;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP
