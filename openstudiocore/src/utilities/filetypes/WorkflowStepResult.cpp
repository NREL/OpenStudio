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

#include "WorkflowStepResult.hpp"
#include "WorkflowStepResult_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <jsoncpp/json.h>

namespace openstudio {
namespace detail {

  WorkflowStepResult_Impl::WorkflowStepResult_Impl()
  {}

  boost::optional<DateTime> WorkflowStepResult_Impl::startedAt() const
  {
    return m_startedAt;
  }

  boost::optional<DateTime> WorkflowStepResult_Impl::completedAt() const
  {
    return m_completedAt;
  }

  boost::optional<StepResult> WorkflowStepResult_Impl::stepResult() const
  {
    return m_stepResult;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::initialCondition() const
  {
    return m_initialCondition;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::finalCondition() const
  {
    return m_finalCondition;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepErrors() const
  {
    return m_stepErrors;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepWarnings() const
  {
    return m_stepWarnings;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepInfo() const
  {
    return m_stepInfo;
  }

  std::vector<std::pair<std::string, Variant> > WorkflowStepResult_Impl::stepValues() const
  {
    return m_stepValues;
  }

  std::vector<openstudio::path> WorkflowStepResult_Impl::stepFiles() const
  {
    return m_stepFiles;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stdOut() const
  {
    return m_stdOut;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stdErr() const
  {
    return m_stdErr;
  }

  void WorkflowStepResult_Impl::setStartedAt(const DateTime& dateTime)
  {
    m_startedAt = dateTime;
  }

  void WorkflowStepResult_Impl::resetStartedAt()
  {
    m_startedAt.reset();
  }

  void WorkflowStepResult_Impl::setCompletedAt(const DateTime& dateTime)
  {
    m_completedAt = dateTime;
  }

  void WorkflowStepResult_Impl::resetCompletedAt()
  {
    m_completedAt.reset();
  }

  void WorkflowStepResult_Impl::setStepResult(const StepResult& result)
  {
    m_stepResult = result;
  }

  void WorkflowStepResult_Impl::resetStepResult()
  {
    m_stepResult.reset();
  }

  void WorkflowStepResult_Impl::setInitialCondition(const std::string& initialCondition)
  {
    m_initialCondition = initialCondition;
  }

  void WorkflowStepResult_Impl::resetInitialCondition()
  {
    m_initialCondition.reset();
  }

  void WorkflowStepResult_Impl::setFinalCondition(const std::string& finalCondition)
  {
    m_finalCondition = finalCondition;
  }

  void WorkflowStepResult_Impl::resetFinalCondition()
  {
    m_finalCondition.reset();
  }

  void WorkflowStepResult_Impl::addStepError(const std::string& error)
  {
    m_stepErrors.push_back(error);
  }

  void WorkflowStepResult_Impl::resetStepErrors()
  {
    m_stepErrors.clear();
  }

  void WorkflowStepResult_Impl::addStepWarning(const std::string& warning)
  {
    m_stepWarnings.push_back(warning);
  }

  void WorkflowStepResult_Impl::resetStepWarnings()
  {
    m_stepWarnings.clear();
  }

  void WorkflowStepResult_Impl::addStepInfo(const std::string& info)
  {
    m_stepInfo.push_back(info);
  }

  void WorkflowStepResult_Impl::resetStepInfo()
  {
    m_stepInfo.clear();
  }

  void WorkflowStepResult_Impl::addStepValue(const std::pair<std::string, Variant>& value)
  {
    m_stepValues.push_back(value);
  }

  void WorkflowStepResult_Impl::addStepValue(const std::string& name, const Variant& value)
  {
    std::pair<std::string, Variant> p(name, value);
    addStepValue(p);
  }

  void WorkflowStepResult_Impl::resetStepValues()
  {
    m_stepValues.clear();
  }

  void WorkflowStepResult_Impl::addStepFile(const openstudio::path& path)
  {
    m_stepFiles.push_back(path);
  }

  void WorkflowStepResult_Impl::resetStepFiles()
  {
    m_stepFiles.clear();
  }

  void WorkflowStepResult_Impl::setStdOut(const std::string& stdOut)
  {
    m_stdOut = stdOut;
  }

  void WorkflowStepResult_Impl::resetStdOut()
  {
    m_stdOut.reset();
  }

  void WorkflowStepResult_Impl::setStdErr(const std::string& stdErr)
  {
    m_stdErr = stdErr;
  }

  void WorkflowStepResult_Impl::resetStdErr()
  {
    m_stdErr.reset();
  }

} // detail

WorkflowStepResult::WorkflowStepResult()
  : m_impl(std::shared_ptr<detail::WorkflowStepResult_Impl>(new detail::WorkflowStepResult_Impl))
{
  OS_ASSERT(getImpl<detail::WorkflowStepResult_Impl>());
}

WorkflowStepResult::WorkflowStepResult(const WorkflowStepResult& other)
  : m_impl(other.m_impl)
{}

WorkflowStepResult::WorkflowStepResult(std::shared_ptr<detail::WorkflowStepResult_Impl> impl)
  : m_impl(impl)
{}

boost::optional<WorkflowStepResult> WorkflowStepResult::fromString(const std::string& s)
{
  Json::Reader reader;
  Json::Value value;
  bool parsingSuccessful = reader.parse(s, value);
  if (!parsingSuccessful){
    return boost::none;
  }

  WorkflowStepResult result;

  try{  

    if (value.isMember("started_at")){
      boost::optional<DateTime> dateTime = DateTime::fromISO8601(value["started_at"].asString());
      if (dateTime){
        result.setStartedAt(*dateTime);
      }
    }

    if (value.isMember("completed_at")){
      boost::optional<DateTime> dateTime = DateTime::fromISO8601(value["completed_at"].asString());
      if (dateTime){
        result.setCompletedAt(*dateTime);
      }
    }

    if (value.isMember("step_result")){
      StepResult stepResult(value["step_result"].asString());
      result.setStepResult(stepResult);
    }

    if (value.isMember("initial_condition")){
      result.setInitialCondition(value["initial_condition"].asString());
    }

    if (value.isMember("final_condition")){
      result.setFinalCondition(value["final_condition"].asString());
    }

    Json::Value defaultArrayValue(Json::arrayValue);
    Json::ArrayIndex n;

    Json::Value errors = value.get("step_errors", defaultArrayValue);
    n = errors.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.addStepError(errors[i].asString());
    }
  
    Json::Value warnings = value.get("step_warnings", defaultArrayValue);
    n = warnings.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.addStepWarning(warnings[i].asString());
    }

    Json::Value info = value.get("step_info", defaultArrayValue);
    n = info.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.addStepInfo(info[i].asString());
    }

    Json::Value stepValues = value.get("step_values", defaultArrayValue);
    n = stepValues.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      std::string name = stepValues[i]["name"].asString();
      Json::Value v = stepValues[i]["value"];
      if (v.isString()){
        result.addStepValue(name, Variant(v.asString()));
      } else if (v.isIntegral()){
        result.addStepValue(name, Variant(v.asInt()));
      } else if (v.isDouble()){
        result.addStepValue(name, Variant(v.asDouble()));
      } else if (v.isBool()){
        result.addStepValue(name, Variant(v.asBool()));
      } else{
        //error
      }
    }

    Json::Value files = value.get("step_files", defaultArrayValue);
    n = files.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.addStepFile(toPath(files[i].asString()));
    }

    if (value.isMember("stdout")){
      result.setStdOut(value["stdout"].asString());
    }

    if (value.isMember("stderr")){
      result.setStdErr(value["stderr"].asString());
    }

  } catch (const std::exception&){
    return boost::none;
  }

  return result;
}

std::string WorkflowStepResult::string() const
{
  Json::Value value(Json::objectValue);
  bool complete = false;
 
  if (startedAt()){
    value["started_at"] = startedAt()->toISO8601();
  }

  if (completedAt()){
    complete = true;
    value["completed_at"] = completedAt()->toISO8601();
  }

  if (complete){
    if (stepResult()){
      value["step_result"] = stepResult()->valueName();
    }else{
      // error
    }
  }

  if (initialCondition()){
    value["initial_condition"] = initialCondition().get();
  }

  if (finalCondition()){
    value["final_condition"] = finalCondition().get();
  }

  if (complete || (stepErrors().size() > 0)){
    Json::Value errors(Json::arrayValue);
    for (const auto& stepError : stepErrors()){
      errors.append(stepError);
    }
    value["step_errors"] = errors;
  }

  if (complete || (stepWarnings().size() > 0)){
    Json::Value warnings(Json::arrayValue);
    for (const auto& stepWarning : stepWarnings()){
      warnings.append(stepWarning);
    }
    value["step_warnings"] = warnings;
  }

  if (complete || (stepInfo().size() > 0)){
    Json::Value info(Json::arrayValue);
    for (const auto& stepI : stepInfo()){
      info.append(stepI);
    }
    value["step_info"] = info;
  }

  if (complete || (stepValues().size() > 0)){
    Json::Value values(Json::arrayValue);
    for (const auto& stepValue : stepValues()){
      Json::Value v(Json::objectValue);
      v["name"] = stepValue.first;

      if (stepValue.second.variantType() == VariantType::String){
        v["value"] = stepValue.second.valueAsString();
      }else if (stepValue.second.variantType() == VariantType::Double){
        v["value"] = stepValue.second.valueAsDouble();
      }else if (stepValue.second.variantType() == VariantType::Integer){
        v["value"] = stepValue.second.valueAsInteger();
      }else if (stepValue.second.variantType() == VariantType::Boolean){
        v["value"] = stepValue.second.valueAsBoolean();
      }
      values.append(v);
    }
    value["step_values"] = values;
  }

  if (complete || (stepFiles().size() > 0)){
    Json::Value files(Json::arrayValue);
    for (const auto& stepFile : stepFiles()){
      files.append(toString(stepFile));
    }
    value["step_files"] = files;
  }

  if (stdOut()){
    value["stdout"] = stdOut().get();
  }

  if (stdErr()){
    value["stderr"] = stdErr().get();
  }

  Json::StyledWriter writer;
  std::string result = writer.write(value);

  return result;
}

boost::optional<DateTime> WorkflowStepResult::startedAt() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->startedAt();
}

boost::optional<DateTime> WorkflowStepResult::completedAt() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->completedAt();
}

boost::optional<StepResult> WorkflowStepResult::stepResult() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepResult();
}

boost::optional<std::string> WorkflowStepResult::initialCondition() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->initialCondition();
}

boost::optional<std::string> WorkflowStepResult::finalCondition() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->finalCondition();
}

std::vector<std::string> WorkflowStepResult::stepErrors() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepErrors();
}

std::vector<std::string> WorkflowStepResult::stepWarnings() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepWarnings();
}

std::vector<std::string> WorkflowStepResult::stepInfo() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepInfo();
}

std::vector<std::pair<std::string, Variant> > WorkflowStepResult::stepValues() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepValues();
}

std::vector<openstudio::path> WorkflowStepResult::stepFiles() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepFiles();
}

boost::optional<std::string> WorkflowStepResult::stdOut() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stdOut();
}

boost::optional<std::string> WorkflowStepResult::stdErr() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stdErr();
}

void WorkflowStepResult::setStartedAt(const DateTime& dateTime)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setStartedAt(dateTime);
}

void WorkflowStepResult::resetStartedAt()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStartedAt();
}

void WorkflowStepResult::setCompletedAt(const DateTime& dateTime)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setCompletedAt(dateTime);
}

void WorkflowStepResult::resetCompletedAt()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetCompletedAt();
}

void WorkflowStepResult::setStepResult(const StepResult& result)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setStepResult(result);
}

void WorkflowStepResult::resetStepResult()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepResult();
}

void WorkflowStepResult::setInitialCondition(const std::string& initialCondition)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setInitialCondition(initialCondition);
}

void WorkflowStepResult::resetInitialCondition()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetInitialCondition();
}

void WorkflowStepResult::setFinalCondition(const std::string& finalCondition)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setFinalCondition(finalCondition);
}

void WorkflowStepResult::resetFinalCondition()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetFinalCondition();
}

void WorkflowStepResult::addStepError(const std::string& error)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepError(error);
}

void WorkflowStepResult::resetStepErrors()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepErrors();
}

void WorkflowStepResult::addStepWarning(const std::string& warning)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepWarning(warning);
}

void WorkflowStepResult::resetStepWarnings()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepWarnings();
}

void WorkflowStepResult::addStepInfo(const std::string& info)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepInfo(info);
}

void WorkflowStepResult::resetStepInfo()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepInfo();
}

void WorkflowStepResult::addStepValue(const std::pair<std::string, Variant>& value)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepValue(value);
}

void WorkflowStepResult::addStepValue(const std::string& name, const Variant& value)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepValue(name, value);
}

void WorkflowStepResult::resetStepValues()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepValues();
}

void WorkflowStepResult::addStepFile(const openstudio::path& path)
{
  getImpl<detail::WorkflowStepResult_Impl>()->addStepFile(path);
}

void WorkflowStepResult::resetStepFiles()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepFiles();
}

void WorkflowStepResult::setStdOut(const std::string& stdOut)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setStdOut(stdOut);
}

void WorkflowStepResult::resetStdOut()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStdOut();
}

void WorkflowStepResult::setStdErr(const std::string& stdErr)
{
  getImpl<detail::WorkflowStepResult_Impl>()->setStdErr(stdErr);
}

void WorkflowStepResult::resetStdErr()
{
  getImpl<detail::WorkflowStepResult_Impl>()->resetStdErr();
}

std::ostream& operator<<(std::ostream& os, const WorkflowStepResult& workflowStepResult)
{
  os << workflowStepResult.string();
  return os;
}

} // openstudio
