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

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Checksum.hpp"

#include <boost/optional.hpp>

#include <string>
#include <fstream>

namespace openstudio{
namespace detail{

  WorkflowJSON_Impl::WorkflowJSON_Impl()
  {}

  WorkflowJSON_Impl::WorkflowJSON_Impl(const std::string& s)
    : m_path()
  {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s, m_value);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON cannot be processed, " << errors);
    }
  }

  WorkflowJSON_Impl::WorkflowJSON_Impl(const openstudio::path& p)
    : m_path(p)
  {
    if (!boost::filesystem::exists(m_path) || !boost::filesystem::is_regular_file(m_path)){
      LOG_AND_THROW("Path '" << m_path << "' is not a WorkflowJSON file");
    }

    // open file
    std::ifstream ifs(openstudio::toString(m_path));

    Json::Reader reader;
    bool parsingSuccessful = reader.parse(ifs, m_value);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON '" << toString(m_path) << "' cannot be processed, " << errors);
    }
  }

  std::string WorkflowJSON_Impl::string(bool includeHash) const
  {
    Json::Value clone(m_value);
    if (!includeHash){
      clone.removeMember("hash");
    }

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

  boost::optional<openstudio::path> WorkflowJSON_Impl::oswPath() const
  {
    if (m_path.empty()){
      return boost::none;
    }
    return m_path;
  }

  openstudio::path WorkflowJSON_Impl::oswDir() const
  {
    if (m_path.empty()){
      return boost::filesystem::current_path();
    }
    return m_path.parent_path();
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
      return boost::filesystem::canonical(result, oswDir());
    }
    return result;
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::filePaths() const
  {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);
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
        result.push_back(boost::filesystem::canonical(path));
      }else{
        result.push_back(boost::filesystem::canonical(path, absoluteRootDir()));
      }
    }
    return result;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const openstudio::path& file)
  {
    for (const auto& path : absoluteFilePaths()){
      openstudio::path p = path / file;
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
        return p;
      }
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findFile(const std::string& fileName)
  {
    return findFile(toPath(fileName));
  }

  std::vector<openstudio::path> WorkflowJSON_Impl::measurePaths() const
  {
    std::vector<openstudio::path> result;

    Json::Value defaultValue(Json::arrayValue);
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
        result.push_back(boost::filesystem::canonical(path));
      } else{
        result.push_back(boost::filesystem::canonical(path, absoluteRootDir()));
      }
    }
    return result;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const openstudio::path& measureDir)
  {
    for (const auto& path : absoluteMeasurePaths()){
      openstudio::path p = path / measureDir;
      if (boost::filesystem::exists(p) && boost::filesystem::is_directory(p)){
        return p;
      }
    }
    return boost::none;
  }

  boost::optional<openstudio::path> WorkflowJSON_Impl::findMeasure(const std::string& measureDirName)
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
    std::vector<WorkflowStep> result;

    Json::Value defaultValue(Json::arrayValue);
    Json::Value steps = m_value.get("steps", defaultValue);

    Json::ArrayIndex n = steps.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      Json::Value step = steps[i];
      Json::Value measureDirName = step["measure_dir_name"];

      WorkflowStep workflowStep(measureDirName.asString());

      Json::Value arguments = step["arguments"];
      Json::ArrayIndex n2 = arguments.size();
      for (Json::ArrayIndex j = 0; j < n2; ++j){
        Json::Value argument = arguments[j];
        Json::Value name = argument["name"];
        Json::Value value = argument["value"];

        if (value.isBool()){
          workflowStep.setArgument(name.asString(), value.asBool());
        } else if (value.isIntegral()){
          workflowStep.setArgument(name.asString(), value.asInt());
        } else if (value.isDouble()){
          workflowStep.setArgument(name.asString(), value.asDouble());
        } else{
          workflowStep.setArgument(name.asString(), value.asString());
        }
      }

      result.push_back(workflowStep);
    }

    return result;
  }

  bool WorkflowJSON_Impl::setWorkflowSteps(const std::vector<WorkflowStep>& workflowSteps)
  {
    Json::Value steps(Json::arrayValue);
    for (const auto& workflowStep : workflowSteps){
      Json::Value step(Json::objectValue);
      step["measure_dir_name"] = Json::Value(workflowStep.measureDirName());
      // todo
    }
    m_value["steps"] = steps;
    return true;
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
  return WorkflowJSON(std::shared_ptr<detail::WorkflowJSON_Impl>(new detail::WorkflowJSON_Impl(*m_impl)));
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

boost::optional<openstudio::path> WorkflowJSON::oswPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->oswPath();
}

openstudio::path WorkflowJSON::oswDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->oswDir();
}

openstudio::path WorkflowJSON::rootDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->rootDir();
}

openstudio::path WorkflowJSON::absoluteRootDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteRootDir();
}

std::vector<openstudio::path> WorkflowJSON::filePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->filePaths();
}

std::vector<openstudio::path> WorkflowJSON::absoluteFilePaths() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->absoluteFilePaths();
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const openstudio::path& file)
{
  return getImpl<detail::WorkflowJSON_Impl>()->findFile(file);
}

boost::optional<openstudio::path> WorkflowJSON::findFile(const std::string& fileName)
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

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const openstudio::path& measureDir)
{
  return getImpl<detail::WorkflowJSON_Impl>()->findMeasure(measureDir);
}

boost::optional<openstudio::path> WorkflowJSON::findMeasure(const std::string& measureDirName)
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

} // openstudio
