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

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/data/Variant.hpp"
#include "../utilities/time/DateTime.hpp"

namespace openstudio {
namespace detail {

class UTILITIES_API WorkflowStepValue_Impl {
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

class UTILITIES_API WorkflowStepResult_Impl {
 public:

  WorkflowStepResult_Impl();

  std::string string() const;

  boost::optional<DateTime> startedAt() const;

  boost::optional<DateTime> completedAt() const;

  boost::optional<StepResult> stepResult() const;

  boost::optional<std::string> initialCondition() const;

  boost::optional<std::string> finalCondition() const;

  std::vector<std::string> stepErrors() const;

  std::vector<std::string> stepWarnings() const;

  std::vector<std::string> stepInfo() const;

  std::vector<WorkflowStepValue> stepValues() const;

  std::vector<openstudio::path> stepFiles() const;

  boost::optional<std::string> stdOut() const;

  boost::optional<std::string> stdErr() const;

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
   boost::optional<StepResult> m_stepResult;
   boost::optional<std::string> m_initialCondition;
   boost::optional<std::string> m_finalCondition;
   std::vector<std::string> m_stepErrors;
   std::vector<std::string> m_stepWarnings;
   std::vector<std::string> m_stepInfo;
   std::vector<WorkflowStepValue> m_stepValues;
   std::vector<openstudio::path> m_stepFiles;
   boost::optional<std::string> m_stdOut;
   boost::optional<std::string> m_stdErr;
};


} // detail
} // openstudio

#endif // UTILITIES_FILETYPES_WORKFLOWSTEPRESULT_IMPL_HPP
