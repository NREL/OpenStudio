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

#include "WorkflowJSON.hpp"
#include "WorkflowJSON_Impl.hpp"

#include "WorkflowStep.hpp"
#include "WorkflowStepResult.hpp"

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Checksum.hpp"
#include "../time/DateTime.hpp"

#include <boost/optional.hpp>

#include <string>
#include <fstream>

namespace openstudio{
namespace detail{

  WorkflowJSON_Impl::WorkflowJSON_Impl()
  {
    m_value["created_at"] = DateTime::nowUTC().toISO8601();
    m_value["steps"] = Json::Value(Json::arrayValue);
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl(const std::string& s)
  {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s, m_value);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON cannot be processed, " << errors);
    }

    parseSteps();
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl(const openstudio::path& p)
  {
    if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p)){
      LOG_AND_THROW("Path '" << p << "' is not a WorkflowJSON file");
    }

    // open file
    std::ifstream ifs(openstudio::toString(p));

    Json::Reader reader;
    bool parsingSuccessful = reader.parse(ifs, m_value);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON '" << toString(p) << "' cannot be processed, " << errors);
    }

    parseSteps();

    setOswPath(p);
  }

  WorkflowJSON WorkflowJSON_Impl::clone() const
  {
    WorkflowJSON result(this->string(true));
      
    boost::optional<openstudio::path> oswPath = this->oswPath();
    if (oswPath){
      result.setOswPath(*oswPath);
    } else{
      result.setOswDir(this->oswDir());
    }

    return result;
  }

  std::string WorkflowJSON_Impl::string(bool includeHash) const
  {
    Json::Value clone(m_value);
    if (!includeHash){
      clone.removeMember("hash");
    }

    Json::Value steps(Json::arrayValue);
    for (const auto& step : m_steps){

      Json::Reader reader;
      Json::Value stepValue;
      bool parsingSuccessful = reader.parse(step.string(), stepValue);
      if (parsingSuccessful){
        steps.append(stepValue);
      }
    }
    clone["steps"] = steps;

    Json::StyledWriter writer;
    std::string result = writer.write(clone);

    return result;
  }

  std::string WorkflowJSON_Impl::hash() const
  {
    Json::Value defaultValue("");
    Json::Value h = m_value.get("hash", defaultValue);

    return h.asString();
  }

  std::string WorkflowJSON_Impl::computeHash() const
  {
    return checksum(string(false));
  }

  bool WorkflowJSON_Impl::checkForUpdates()
  {
    std::string h1 = hash();
    std::string h2 = computeHash();
    bool result = (h1 != h2);
    if (result){
      m_value["hash"] = h2;
      onUpdate();
    }
    return result;
  }

  bool WorkflowJSON_Impl::save() const
  {
    openstudio::path p = path();
    if (p.empty()){
      return false;
    }
    return saveAs(p);
  }

  bool WorkflowJSON_Impl::saveAs(const openstudio::path& p) const
  {
    if (makeParentFolder(p)) {
      std::ofstream outFile(openstudio::toString(p));
      if (outFile) {
        try {
          outFile << string();
          outFile.close();
          return true;
        } catch (...) {
          LOG(Error, "Unable to write file to path '" << toString(p) << "'.");
          return false;
        }
      }
    }

    LOG(Error, "Unable to write file to path '" << toString(p) << "', because parent directory "
        << "could not be created.");

    return false;
  }
  
  void WorkflowJSON_Impl::reset()
  {
    m_value.removeMember("started_at");
    m_value.removeMember("completed_at");
    m_value.removeMember("completed_status");
    m_value.removeMember("current_step");
    onUpdate();
  }

  void WorkflowJSON_Impl::start()
  {
    m_value["started_at"] = DateTime::nowUTC().toISO8601();
    m_value["current_step"] = 0;
    for (auto& step : workflowSteps()){
      step.resetResult();
    }
    onUpdate();
  }

  unsigned WorkflowJSON_Impl::currentStepIndex() const
  {
    Json::Value defaultValue(0);
    Json::Value index = m_value.get("current_step", defaultValue);
    return index.asUInt();
  }

  boost::optional<WorkflowStep> WorkflowJSON_Impl::currentStep() const
  {
    unsigned index = currentStepIndex();
    std::vector<WorkflowStep> steps = this->workflowSteps();

    if (index >= steps.size()){
      return boost::none;
    }

    return steps[index];
  }

  bool WorkflowJSON_Impl::incrementStep()
  {
    unsigned index = currentStepIndex();
    m_value["current_step"] = index + 1;
    onUpdate();
    return currentStep();
  }

  boost::optional<std::string> WorkflowJSON_Impl::completedStatus() const
  {
    Json::Value defaultValue("");
    Json::Value status = m_value.get("completed_status", defaultValue);
    std::string result = status.asString();
    if (result.empty()){
      return boost::none;
    }
    return result;
  }

  void WorkflowJSON_Impl::setCompletedStatus(const std::string& status)
  {
    m_value["completed_status"] = status;
    m_value["completed_at"] = DateTime::nowUTC().toISO8601();
    onUpdate();
  }

  boost::optional<DateTime> WorkflowJSON_Impl::createdAt() const
  {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("created_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()){
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::startedAt() const
  {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("started_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()){
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::updatedAt() const
  {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("updated_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()){
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<DateTime> WorkflowJSON_Impl::completedAt() const
  {
    Json::Value defaultValue("");
    Json::Value value = m_value.get("completed_at", defaultValue);
    std::string str = value.asString();
    if (str.empty()){
      return boost::none;
    }
    return DateTime::fromISO8601(str);
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::oswPath() const
  {
    if (m_oswFilename.empty() || m_oswDir.empty()){
      return boost::none;
    }
    return m_oswDir / m_oswFilename;
  }

  bool WorkflowJSON_Impl::setOswPath(const openstudio::path& path)
  {
    if (path.is_absolute()){
      m_oswFilename = path.filename();
      m_oswDir = path.parent_path();
      onUpdate();
      return true;
    }
    return false;
  }

  openstudio::path WorkflowJSON_Impl::oswDir() const
  {
    if (m_oswDir.empty()){
      return boost::filesystem::current_path();
    }
    return m_oswDir;
  }

  bool WorkflowJSON_Impl::setOswDir(const openstudio::path& path)
  {
    if (path.is_absolute()){
      m_oswDir = path;
      onUpdate();
      return true;
    }
    return false;
  }

  openstudio::path WorkflowJSON_Impl::rootDir() const
  {
    Json::Value defaultValue(".");
    Json::Value root = m_value.get("root", defaultValue);
    return toPath(root.asString());
  }

  openstudio::path WorkflowJSON_Impl::absoluteRootDir() const
  {
    openstudio::path result = rootDir();
    if (result.is_relative()){
      return boost::filesystem::absolute(result, oswDir());
    }
    return result;
  }

  openstudio::path WorkflowJSON_Impl::runDir() const
  {
    Json::Value defaultValue("./run");
    Json::Value runDirectory = m_value.get("run_directory", defaultValue);
    return toPath(runDirectory.asString());
  }

  openstudio::path WorkflowJSON_Impl::absoluteRunDir() const
  {
    openstudio::path result = runDir();
    if (result.is_relative()){
      return boost::filesystem::absolute(result, absoluteRootDir());
    }
    return result;
  }

  openstudio::path WorkflowJSON_Impl::outPath() const
  {
    Json::Value defaultValue("out.osw");
    Json::Value path = m_value.get("out_name", defaultValue);
    return toPath(path.asString());
  }

  openstudio::path WorkflowJSON_Impl::absoluteOutPath() const
  {
    openstudio::path result = outPath();
    if (result.is_relative()){
      return boost::filesystem::absolute(result, oswDir());
    }
    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::filePaths() const
  {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);
    defaultValue.append("./files");
    defaultValue.append("./weather");
    defaultValue.append("../../files");
    defaultValue.append("../../weather");
    defaultValue.append("./");

    Json::Value paths = m_value.get("file_paths", defaultValue);

    Json::ArrayIndex n = paths.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.push_back(toPath(paths[i].asString()));
    }

    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::absoluteFilePaths() const
  {
    std::vector<openstudio::path> result;
    for (const auto& path : filePaths()){
      if (path.is_absolute()){
        result.push_back(path);
      }else{
        result.push_back(boost::filesystem::absolute(path, absoluteRootDir()));
      }
    }
    return result;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const openstudio::path& file) const
  {
    for (const auto& path : absoluteFilePaths()){
      openstudio::path p = path / file;
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
        return boost::filesystem::canonical(p);
      }
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const std::string& fileName) const
  {
    return findFile(toPath(fileName));
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::measurePaths() const
  {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);
    defaultValue.append("./measures");
    defaultValue.append("../../measures");
    defaultValue.append("./");

    Json::Value paths = m_value.get("measure_paths", defaultValue);

    Json::ArrayIndex n = paths.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      result.push_back(toPath(paths[i].asString()));
    }

    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::absoluteMeasurePaths() const
  {
    std::vector<openstudio::path> result;
    for (const auto& path : measurePaths()){
      if (path.is_absolute()){
        result.push_back(path);
      } else{
        result.push_back(boost::filesystem::absolute(path, absoluteRootDir()));
      }
    }
    return result;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const openstudio::path& measureDir) const
  {
    for (const auto& path : absoluteMeasurePaths()){
      openstudio::path p = path / measureDir;
      if (boost::filesystem::exists(p) && boost::filesystem::is_directory(p)){
        return boost::filesystem::canonical(p);
      }
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const std::string& measureDirName) const
  {
    return findMeasure(toPath(measureDirName));
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::seedFile() const
  {
    Json::Value defaultValue("");
    Json::Value seed = m_value.get("seed_file", defaultValue);
    std::string result = seed.asString();
    if (result.empty()){
      return boost::none;
    }
    return toPath(result);
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::weatherFile() const
  {
    Json::Value defaultValue("");
    Json::Value weather = m_value.get("weather_file", defaultValue);
    std::string result = weather.asString();
    if (result.empty()){
      return boost::none;
    }
    return toPath(result);
  }

  std::vector<WorkflowStep> WorkflowJSON_Impl::workflowSteps() const
  {
    return m_steps;
  }
  
  bool WorkflowJSON_Impl::setWorkflowSteps(const std::vector<WorkflowStep>& workflowSteps)
  {
    m_steps = workflowSteps;
    onUpdate();
    return true;
  }

  void WorkflowJSON_Impl::onUpdate()
  {
    m_value["updated_at"] = DateTime::nowUTC().toISO8601();
  }

  void WorkflowJSON_Impl::parseSteps()
  {
    m_steps.clear();

    Json::Value defaultValue(Json::arrayValue);
    Json::Value steps = m_value.get("steps", defaultValue);

    Json::ArrayIndex n = steps.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      Json::Value step = steps[i];

      Json::FastWriter writer;
      std::string s = writer.write(step);

      boost::optional<WorkflowStep> workflowStep = WorkflowStep::fromString(s);
      if (workflowStep){
        m_steps.push_back(*workflowStep);
      }else{
        LOG_AND_THROW("Step " << i << " cannot be processed");
      }
    }

    m_value.removeMember("steps");

  }

} // detail

WorkflowJSON::WorkflowJSON()
  : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl()))
{}

WorkflowJSON::WorkflowJSON(const std::string& s)
  : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl(s)))
{}

WorkflowJSON::WorkflowJSON(const openstudio::path& p)
  : m_impl(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl(p)))
{}

WorkflowJSON::WorkflowJSON(std::shared_ptr<detail::WorkflowJSON_Impl> impl)
  : m_impl(impl)
{}

WorkflowJSON WorkflowJSON::clone() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->clone();
}

boost::optional<WorkflowJSON> WorkflowJSON::load(const std::string& s)
{
  boost::optional<WorkflowJSON> result;
  try {
    result = WorkflowJSON(s);
  } catch (const std::exception&){
  }
  return result;
}

boost::optional<WorkflowJSON> WorkflowJSON::load(const openstudio::path& p)
{
  boost::optional<WorkflowJSON> result;
  try {
    result = WorkflowJSON(p);
  } catch (const std::exception&){
  }
  return result;
}

std::string WorkflowJSON::string(bool includeHash) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->string(includeHash);
}

std::string WorkflowJSON::hash() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->hash();
}

std::string WorkflowJSON::computeHash() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->computeHash();
}

bool WorkflowJSON::checkForUpdates()
{
  return getImpl<detail::WorkflowJSON_Impl>()->checkForUpdates();
}

bool WorkflowJSON::save() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->save();
}

bool WorkflowJSON::saveAs(const openstudio::path& p) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->saveAs(p);
}

void WorkflowJSON::reset()
{
  getImpl<detail::WorkflowJSON_Impl>()->reset();
}

void WorkflowJSON::start()
{
  getImpl<detail::WorkflowJSON_Impl>()->start();
}

unsigned WorkflowJSON::currentStepIndex() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->currentStepIndex();
}

boost::optional<WorkflowStep> WorkflowJSON::currentStep() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->currentStep();
}

bool WorkflowJSON::incrementStep()
{
  return getImpl<detail::WorkflowJSON_Impl>()->incrementStep();
}

boost::optional<std::string> WorkflowJSON::completedStatus() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->completedStatus();
}

void WorkflowJSON::setCompletedStatus(const std::string& status)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setCompletedStatus(status);
}

boost::optional<DateTime> WorkflowJSON::createdAt() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->createdAt();
}

boost::optional<DateTime> WorkflowJSON::startedAt() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->startedAt();
}

boost::optional<DateTime> WorkflowJSON::updatedAt() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->updatedAt();
}

boost::optional<DateTime> WorkflowJSON::completedAt() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->completedAt();
}

boost::optional<openstudio::path> WorkflowJSON::oswPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->oswPath();
}

bool WorkflowJSON::setOswPath(const openstudio::path& path)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setOswPath(path);
}

openstudio::path WorkflowJSON::oswDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->oswDir();
}

bool WorkflowJSON::setOswDir(const openstudio::path& path)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setOswDir(path);
}

openstudio::path WorkflowJSON::rootDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->rootDir();
}

openstudio::path WorkflowJSON::absoluteRootDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteRootDir();
}

openstudio::path WorkflowJSON::runDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->runDir();
}

openstudio::path WorkflowJSON::absoluteRunDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteRunDir();
}

openstudio::path WorkflowJSON::outPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->outPath();
}

openstudio::path WorkflowJSON::absoluteOutPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteOutPath();
}

std::vector<openstudio::path> WorkflowJSON::filePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->filePaths();
}

std::vector<openstudio::path> WorkflowJSON::absoluteFilePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteFilePaths();
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const openstudio::path& file) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->findFile(file);
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const std::string& fileName) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->findFile(fileName);
}

std::vector<openstudio::path> WorkflowJSON::measurePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->measurePaths();
}

std::vector<openstudio::path> WorkflowJSON::absoluteMeasurePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteMeasurePaths();
}

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const openstudio::path& measureDir) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->findMeasure(measureDir);
}

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const std::string& measureDirName) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->findMeasure(measureDirName);
}

boost::optional<openstudio::path> WorkflowJSON::seedFile() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->seedFile();
}

boost::optional<openstudio::path> WorkflowJSON::weatherFile() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->weatherFile();
}

std::vector<WorkflowStep> WorkflowJSON::workflowSteps() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->workflowSteps();
}

bool WorkflowJSON::setWorkflowSteps(const std::vector<WorkflowStep>& steps)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setWorkflowSteps(steps);
}

std::ostream& operator<<(std::ostream& os, const WorkflowJSON& workflowJSON)
{
  os << workflowJSON.string();
  return os;
}

} // openstudio
