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

  /** Default constructor sets value() to StepResult::Success. */
   WorkflowStepResult();

  /// Construct from JSON formatted string
  static boost::optional<WorkflowStepResult> fromString();

  /// Serialize to JSON formatted string
  std::string string() const;

  //@}
  /** @name Getters */
  //@{

  DateTime startedAt();

  DateTime completedAt();

  StepResult stepResult() const;

  boost::optional<std::string> initialCondition() const;

  boost::optional<std::string> finalCondition() const;

  std::vector<std::string> stepErrors() const;

  std::vector<std::string> stepWarnings() const;

  std::vector<std::string> stepInfo() const;

  std::vector<std::pair<std::string, Variant> > stepValues() const;

  std::vector<openstudio::path> stepFiles() const;

  std::string stdOut() const;

  std::string stdErr() const;

  //@}
  /** @name Setters */
  //@{


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
