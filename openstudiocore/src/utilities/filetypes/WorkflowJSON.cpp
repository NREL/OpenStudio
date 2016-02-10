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

#include "../core/Assert.hpp"

#include <boost/optional.hpp>

#include <string>
#include <fstream>

namespace openstudio{

WorkflowStep::WorkflowStep(const std::string& measureDirName)
  : m_measureDirName(measureDirName)
{}

std::string WorkflowStep::measureDirName() const
{
  return m_measureDirName;
}

std::map<std::string, Variant> WorkflowStep::arguments() const
{
  return m_arguments;
}

boost::optional<Variant> WorkflowStep::getArgument(const std::string& name) const
{
  // todo
  return boost::none;
}

void WorkflowStep::setArgument(const std::string& name, const Variant& value)
{
  m_arguments.insert(std::make_pair(name, value));
}

void WorkflowStep::setArgument(const std::string& name, bool value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, double value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, int value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, const std::string& value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::removeArgument(const std::string& name)
{
  m_arguments.erase(name);
}

void WorkflowStep::clearArguments()
{
  m_arguments.clear();
}
  
WorkflowJSON::WorkflowJSON()
{}

WorkflowJSON::WorkflowJSON(const std::string& s)
  : m_path()
{
  Json::Reader reader;
  Json::Value json;
  bool parsingSuccessful = reader.parse(s, json);
  if (!parsingSuccessful){
    std::string errors = reader.getFormattedErrorMessages();
    LOG_AND_THROW("WorkflowJSON cannot be processed, " << errors);
  }

}

WorkflowJSON::WorkflowJSON(const openstudio::path& p)
  : m_path(p)
{
  if (!boost::filesystem::exists(m_path) || !boost::filesystem::is_regular_file(m_path)){
    LOG_AND_THROW("Path '" << m_path << "' is not a WorkflowJSON file");
  }

  // open file
  std::ifstream ifs(openstudio::toString(m_path));
  
  Json::Reader reader;
  Json::Value json;
  bool parsingSuccessful = reader.parse(ifs, json);
  if ( !parsingSuccessful ){
    std::string errors = reader.getFormattedErrorMessages();
    LOG_AND_THROW("WorkflowJSON '" << toString(m_path) << "' cannot be processed, " << errors);
  }
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
  }catch(const std::exception&){
  }
  return result;
}

std::string WorkflowJSON::string() const
{
  return "";
}

std::string WorkflowJSON::hash() const
{
  return "";
}

bool WorkflowJSON::save() const
{
  return false;
}

bool WorkflowJSON::saveAs(const openstudio::path& p) const
{
  return false;
}

openstudio::path WorkflowJSON::path() const
{
  return m_path;
}

openstudio::path WorkflowJSON::rootPath() const
{
  openstudio::path rootPath = toPath(".");
  boost::optional<Attribute> attribute = getAttribute("root");
  if (attribute && (AttributeValueType::String == attribute->valueType().value())){
    rootPath = toPath(attribute->valueAsString());
  }

  if (rootPath.is_absolute()){
    return rootPath;
  }

  if (m_path.empty()){
    return openstudio::path();
  } 

  return boost::filesystem::absolute(rootPath, m_path.parent_path());
}

openstudio::path WorkflowJSON::seedPath() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }

  std::string seedName;
  boost::optional<Attribute> attribute = getAttribute("seed");
  if (attribute && (AttributeValueType::String == attribute->valueType().value())){
    seedName = attribute->valueAsString();
  }

  if (seedName.empty()){
    return openstudio::path();
  }

  return rootPath / toPath("seed") / toPath(seedName);
}

openstudio::path WorkflowJSON::weatherPath() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }

  std::string weatherName;
  boost::optional<Attribute> attribute = getAttribute("weather_file");
  if (attribute && (AttributeValueType::String == attribute->valueType().value())){
    weatherName = attribute->valueAsString();
  }

  if (weatherName.empty()){
    return openstudio::path();
  }

  return rootPath / toPath("weather") / toPath(weatherName);
}

openstudio::path WorkflowJSON::measuresDir() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }
  return rootPath / toPath("measures/");
}

std::vector<Attribute> WorkflowJSON::attributes() const
{
  return m_attributes;
}

boost::optional<Attribute> WorkflowJSON::getAttribute(const std::string& name) const
{
  return boost::none;
}

void WorkflowJSON::removeAttribute(const std::string& name)
{

}

void WorkflowJSON::setAttribute(const Attribute& attribute)
{

}

void WorkflowJSON::clearAttributes()
{
  m_attributes.clear();
}

std::vector<WorkflowStep> WorkflowJSON::workflowSteps() const
{
  return m_workflowSteps;
}

void WorkflowJSON::setWorkflowSteps(const std::vector<WorkflowStep>& steps)
{
  m_workflowSteps = steps;
}

void WorkflowJSON::parse(const Json::Value& json)
{
  Json::Value root = json.get("root", ".");
  if (root.isConvertibleTo(Json::stringValue)){
    
  }
  

}

Json::Value WorkflowJSON::json(bool includeHash) const
{
  return Json::Value();
}

} // openstudio
