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

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"

namespace openstudio {

namespace detail {
  class WorkflowStepValue_Impl;
  class WorkflowStepResult_Impl;
}  // namespace detail
class DateTime;
class Variant;
class MeasureType;
class UUID;

// Attribute is deprecated
class Attribute;

// clang-format off

/** \class StepResult
 *  \brief Possible outcomes of running a WorkflowStep.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM( StepResult,
  ((Skip)(Skipped)(-2))
  ((NA)(NotApplicable)(-1))
  ((Success)(Pass)(0))
  ((Fail)(Fail)(1))
);
 *  \endcode */
OPENSTUDIO_ENUM(StepResult,
  ((Skip)(Skipped)(-2))
  ((NA)(NotApplicable)(-1))
  ((Success)(Pass)(0))
  ((Fail)(Fail)(1))
);

// clang-format on

/** Class for storing a value set by OSRunner::registerValue. */
class UTILITIES_API WorkflowStepValue
{
 public:
  /** @name Constructors and Destructors */
  //@{

  WorkflowStepValue(const std::string& name, const Variant& value);
  WorkflowStepValue(const std::string& name, const std::string& value);
  WorkflowStepValue(const std::string& name, const char* value);
  WorkflowStepValue(const std::string& name, double value);
  WorkflowStepValue(const std::string& name, int value);
  WorkflowStepValue(const std::string& name, bool value);

  /// Construct from JSON formatted string
  static boost::optional<WorkflowStepValue> fromString(const std::string& s);

  /// Serialize to JSON formatted string
  std::string string() const;

  //@}
  /** @name Getters */
  //@{

  std::string name() const;

  std::string displayName() const;

  boost::optional<std::string> units() const;

  /// get the variant value type
  VariantType variantType() const;

  /// get value as a Variant
  Variant valueAsVariant() const;

  /// get value as a bool
  bool valueAsBoolean() const;

  /// get value as int
  int valueAsInteger() const;

  /// get value as double
  double valueAsDouble() const;

  /// get value as string
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

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::WorkflowStepValue_Impl;

  /** Protected constructor from impl. */
  WorkflowStepValue(std::shared_ptr<detail::WorkflowStepValue_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.WorkflowStepValue");

  std::shared_ptr<detail::WorkflowStepValue_Impl> m_impl;
};

/** Class for documenting the outcome of running a UserScript or a Ruleset. There is an overall
 *  result flag (available from value()), and a number of message types. */
class UTILITIES_API WorkflowStepResult
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. */
  WorkflowStepResult();

  /** Copy constructor. */
  WorkflowStepResult(const WorkflowStepResult& other);

  /// Construct from JSON formatted string
  static boost::optional<WorkflowStepResult> fromString(const std::string& s);

  /// Serialize to JSON formatted string
  std::string string() const;

  //@}
  /** @name Getters */
  //@{

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

  // method for backwards compatibility with OpenStudio 1.X
  // returns StepResult if initialized, if not initialized returns Success for backwards compatibility
  StepResult value() const;

  // method for backwards compatibility with OpenStudio 1.X
  std::vector<LogMessage> errors() const;

  // method for backwards compatibility with OpenStudio 1.X
  std::vector<LogMessage> warnings() const;

  // method for backwards compatibility with OpenStudio 1.X
  std::vector<LogMessage> info() const;

  // method for backwards compatibility with OpenStudio 1.X
  boost::optional<LogMessage> initialCondition() const;

  // method for backwards compatibility with OpenStudio 1.X
  boost::optional<LogMessage> finalCondition() const;

  // method for backwards compatibility with OpenStudio 1.X
  std::vector<Attribute> attributes() const;

  //@}
  /** @name Setters */
  //@{

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

  //@}

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::WorkflowStepResult_Impl;

  /** Protected constructor from impl. */
  WorkflowStepResult(std::shared_ptr<detail::WorkflowStepResult_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.WorkflowStepResult");

  std::shared_ptr<detail::WorkflowStepResult_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const WorkflowStepResult& workflowStepResult);

}  // namespace openstudio

#endif  // UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP
