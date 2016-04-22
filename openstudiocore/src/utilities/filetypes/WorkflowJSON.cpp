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
    Json::Value json;
    bool parsingSuccessful = reader.parse(s, json);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON cannot be processed, " << errors);
    }
    parse("", json);
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
    Json::Value json;
    bool parsingSuccessful = reader.parse(ifs, json);
    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();
      LOG_AND_THROW("WorkflowJSON '" << toString(m_path) << "' cannot be processed, " << errors);
    }
    parse("", json);
  }

  std::string WorkflowJSON_Impl::string(bool includeHash) const
  {
    Json::Value root = json(true);

    Json::StyledWriter writer;
    std::string result = writer.write(root);

    return result;
  }

  std::string WorkflowJSON_Impl::hash() const
  {
    boost::optional<Attribute> attribute = getAttribute("hash");
    if (attribute && attribute->valueType().value() == AttributeValueType::String){
      return attribute->valueAsString();
    }
    return std::string();
  }

  std::string WorkflowJSON_Impl::computeHash() const
  {
    Json::Value root = json(false);

    Json::StyledWriter writer;
    std::string result = writer.write(root);

    return checksum(result);
  }

  bool WorkflowJSON_Impl::checkForUpdates()
  {
    std::string h1 = hash();
    std::string h2 = computeHash();
    bool result = (h1 != h2);
    if (result){
      setAttribute("hash", h2);
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

  openstudio::path WorkflowJSON_Impl::path() const
  {
    return m_path;
  }

  openstudio::path WorkflowJSON_Impl::rootPath() const
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

  openstudio::path WorkflowJSON_Impl::seedPath() const
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

  openstudio::path WorkflowJSON_Impl::weatherPath() const
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

  openstudio::path WorkflowJSON_Impl::measuresDir() const
  {
    openstudio::path rootPath = this->rootPath();
    if (rootPath.empty()){
      return openstudio::path();
    }
    return rootPath / toPath("measures/");
  }

  std::vector<Attribute> WorkflowJSON_Impl::attributes() const
  {
    return m_attributes;
  }

  boost::optional<Attribute> WorkflowJSON_Impl::getAttribute(const std::string& name) const
  {
    for (const Attribute& attribute : m_attributes){
      if (attribute.name() == name){
        return attribute;
      }
    }
    return boost::none;
  }

  bool WorkflowJSON_Impl::removeAttribute(const std::string& name)
  {
    m_attributes.erase(std::remove_if(m_attributes.begin(),
                                      m_attributes.end(),
                                      [&name](const Attribute& attribute){return (attribute.name() == name); }),
                      m_attributes.end());
    return true;
  }

  bool WorkflowJSON_Impl::setAttribute(const Attribute& attribute)
  {
    removeAttribute(attribute.name());
    m_attributes.push_back(attribute);
    return true;
  }

  bool WorkflowJSON_Impl::setAttribute(const std::string& name, bool value)
  {
    if (name != "steps"){
      return setAttribute(Attribute(name, value));
    }
    return false;
  }

  bool WorkflowJSON_Impl::setAttribute(const std::string& name, double value)
  {
    if (name != "steps"){
      return setAttribute(Attribute(name, value));
    }
    return false;
  }

  bool WorkflowJSON_Impl::setAttribute(const std::string& name, int value)
  {
    if (name != "steps"){
      return setAttribute(Attribute(name, value));
    }
    return false;
  }

  bool WorkflowJSON_Impl::setAttribute(const std::string& name, const std::string& value)
  {
    if (name != "steps"){
      return setAttribute(Attribute(name, value));
    }
    return false;
  }

  void WorkflowJSON_Impl::clearAttributes()
  {
    m_attributes.clear();
  }

  std::vector<WorkflowStep> WorkflowJSON_Impl::workflowSteps() const
  {
    return m_workflowSteps;
  }

  bool WorkflowJSON_Impl::setWorkflowSteps(const std::vector<WorkflowStep>& steps)
  {
    m_workflowSteps = steps;
    return true;
  }

  Attribute WorkflowJSON_Impl::parse(const std::string& name, const Json::Value& json)
  {
    bool topLevel = name.empty();

    std::vector<openstudio::Attribute> attributeVector;

    if (json.isObject()) {
      std::vector<std::string> members = json.getMemberNames();
      for (auto it = members.begin(); it != members.end(); ++it){
        Json::Value child = json[*it];
        Attribute attribute = parse(*it, child);
        attributeVector.push_back(attribute);
        if (topLevel){
          m_attributes.push_back(attribute);
        }
      }
    } else if (json.isArray()) {
      unsigned n = json.size();
      for (unsigned i = 0; i < n; ++i){
        Json::Value child = json[i];
        Attribute attribute = parse("obj", child);
        attributeVector.push_back(attribute);
        if (topLevel){
          m_attributes.push_back(attribute);
        }
      }
    } else if (json.isString()) {
      std::string val = json.asString();
      Attribute attribute(name, val);
      return attribute;
    } else if (json.isBool()) {
      bool val = json.asBool();
      Attribute attribute(name, val);
      return attribute;
    } else if (json.isInt()) {
      int val = json.asInt();
      Attribute attribute(name, val);
      return attribute;
    } else if (json.isUInt()) {
      int val = (int)json.asUInt();
      Attribute attribute(name, val);
      return attribute;
    } else if (json.isDouble()) {
      double val = json.asDouble();
      Attribute attribute(name, val);
      return attribute;
    } else{
    }

    Attribute attribute(name, attributeVector);

    if (topLevel){
      //std::cout << attribute.toString();
      boost::optional<Attribute> attribute = getAttribute("steps");
      if (attribute){

        if (attribute->valueType().value() == AttributeValueType::AttributeVector){
          std::vector<Attribute> steps = attribute->valueAsAttributeVector();
          for (const Attribute& step : steps){
            try{
              WorkflowStep workflowStep(step);
              m_workflowSteps.push_back(workflowStep);
            } catch (const std::exception&){

            }
          }
        }

        removeAttribute("steps");
      }
    }

    return attribute;
  }

  Json::Value WorkflowJSON_Impl::json(bool includeHash) const
  {
    Json::Value result(Json::objectValue);

    for (const Attribute& attribute : m_attributes){
      if (!includeHash && attribute.name() == "hash"){
        continue;
      }

      if (attribute.valueType().value() == AttributeValueType::Boolean){
        result[attribute.name()] = Json::Value(attribute.valueAsBoolean());
      } else if (attribute.valueType().value() == AttributeValueType::Double){
        result[attribute.name()] = Json::Value(attribute.valueAsDouble());
      } else if (attribute.valueType().value() == AttributeValueType::Integer){
        result[attribute.name()] = Json::Value(attribute.valueAsInteger());
      } else if (attribute.valueType().value() == AttributeValueType::Unsigned){
        result[attribute.name()] = Json::Value((int)attribute.valueAsUnsigned());
      } else if (attribute.valueType().value() == AttributeValueType::String){
        result[attribute.name()] = Json::Value(attribute.valueAsString());
      }
    }

    Json::Value steps(Json::arrayValue);
    for (const WorkflowStep& workflowStep : m_workflowSteps){
      Json::Value step(Json::objectValue);
      step["measure_dir_name"] = Json::Value(workflowStep.measureDirName());

      Json::Value arguments(Json::arrayValue);
      for (const auto argument : workflowStep.arguments()){
        Json::Value object(Json::objectValue);
        object["name"] = Json::Value(argument.first);
        if (argument.second.variantType().value() == VariantType::Boolean){
          object["value"] = Json::Value(argument.second.valueAsBoolean());
        } else if (argument.second.variantType().value() == VariantType::Double){
          object["value"] = Json::Value(argument.second.valueAsDouble());
        } else if (argument.second.variantType().value() == VariantType::Integer){
          object["value"] = Json::Value(argument.second.valueAsInteger());
        } else if (argument.second.variantType().value() == VariantType::String){
          object["value"] = Json::Value(argument.second.valueAsString());
        }
        arguments.append(object);
      }
      step["arguments"] = arguments;
      steps.append(step);
    }

    result["steps"] = steps;

    return result;
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

openstudio::path WorkflowJSON::path() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->path();
}

openstudio::path WorkflowJSON::rootPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->rootPath();
}

openstudio::path WorkflowJSON::seedPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->seedPath();
}

openstudio::path WorkflowJSON::weatherPath() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->weatherPath();
}

openstudio::path WorkflowJSON::measuresDir() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->measuresDir();
}

std::vector<Attribute> WorkflowJSON::attributes() const
{
  return getImpl<detail::WorkflowJSON_Impl>()->attributes();
}

boost::optional<Attribute> WorkflowJSON::getAttribute(const std::string& name) const
{
  return getImpl<detail::WorkflowJSON_Impl>()->getAttribute(name);
}

bool WorkflowJSON::removeAttribute(const std::string& name)
{
  return getImpl<detail::WorkflowJSON_Impl>()->removeAttribute(name);
}

bool WorkflowJSON::setAttribute(const Attribute& attribute)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setAttribute(attribute);
}

bool WorkflowJSON::setAttribute(const std::string& name, bool value)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setAttribute(name, value);
}

bool WorkflowJSON::setAttribute(const std::string& name, double value)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setAttribute(name, value);
}

bool WorkflowJSON::setAttribute(const std::string& name, int value)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setAttribute(name, value);
}

bool WorkflowJSON::setAttribute(const std::string& name, const std::string& value)
{
  return getImpl<detail::WorkflowJSON_Impl>()->setAttribute(name, value);
}

void WorkflowJSON::clearAttributes()
{
  getImpl<detail::WorkflowJSON_Impl>()->clearAttributes();
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
