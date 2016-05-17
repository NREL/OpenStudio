/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP

#include "../UtilitiesAPI.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"


namespace openstudio {

namespace detail{
  class WorkflowStepResult_Impl;
}
class DateTime;
class Variant;

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

/** Class for documenting the outcome of running a UserScript or a Ruleset. There is an overall
 *  result flag (available from value()), and a number of message types. */
class UTILITIES_API WorkflowStepResult {
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

  boost::optional<StepResult> stepResult() const;

  boost::optional<std::string> initialCondition() const;

  boost::optional<std::string> finalCondition() const;

  std::vector<std::string> stepErrors() const;

  std::vector<std::string> stepWarnings() const;

  std::vector<std::string> stepInfo() const;

  std::vector<std::pair<std::string, Variant> > stepValues() const;

  std::vector<openstudio::path> stepFiles() const;

  boost::optional<std::string> stdOut() const;

  boost::optional<std::string> stdErr() const;

  //@}
  /** @name Setters */
  //@{

  void setStartedAt(const DateTime& dateTime);
  void resetStartedAt();

  void setCompletedAt(const DateTime& dateTime);
  void resetCompletedAt();

  void setStepResult(const StepResult& result);
  void resetStepResult();

  void setInitialCondition(const std::string& initialCondition);
  void resetInitialCondition();

  void setFinalCondition(const std::string& finalCondition);
  void resetFinalCondition();

  void addStepError(const std::string& error);
  void resetStepErrors();

  void addStepWarning(const std::string& warning);
  void resetStepWarnings();

  void addStepInfo(const std::string& info);
  void resetStepInfo();

  void addStepValue(const std::pair<std::string, Variant>& value);
  void addStepValue(const std::string& name, const Variant& value);
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
  template<typename T>
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

} // openstudio

#endif // UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_HPP
