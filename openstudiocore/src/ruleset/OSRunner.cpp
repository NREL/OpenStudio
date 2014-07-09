/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "OSRunner.hpp"

#include "OSArgument.hpp"
#include "UserScript.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace ruleset {

OSRunner::OSRunner()
{}

OSRunner::~OSRunner()
{}

OSResult OSRunner::result() const {
  return m_result;
}

boost::optional<openstudio::model::Model> OSRunner::lastOpenStudioModel() const
{
  if (m_lastOpenStudioModel){
    return m_lastOpenStudioModel;
  }

  if (m_lastOpenStudioModelPath){
    m_lastOpenStudioModel = model::Model::load(*m_lastOpenStudioModelPath);
  }

  return m_lastOpenStudioModel;
}

boost::optional<openstudio::Workspace> OSRunner::lastEnergyPlusWorkspace() const
{
  if (m_lastEnergyPlusWorkspace){
    return m_lastEnergyPlusWorkspace;
  }

  if (m_lastEnergyPlusWorkspacePath){
    m_lastEnergyPlusWorkspace = Workspace::load(*m_lastEnergyPlusWorkspacePath);
  }

  return m_lastEnergyPlusWorkspace;
}

boost::optional<openstudio::SqlFile> OSRunner::lastEnergyPlusSqlFile() const
{
  if (m_lastEnergyPlusSqlFile){
    return m_lastEnergyPlusSqlFile;
  }

  if (m_lastEnergyPlusSqlFilePath){
    try {
      m_lastEnergyPlusSqlFile = SqlFile(*m_lastEnergyPlusSqlFilePath);
    }catch(const std::exception&){
    }
  }

  return m_lastEnergyPlusSqlFile;
}

boost::optional<openstudio::EpwFile> OSRunner::lastEpwFile() const
{
  if (m_lastEpwFile){
    return m_lastEpwFile;
  }

  if (m_lastEpwFilePath){
    try {
      m_lastEpwFile = EpwFile(*m_lastEpwFilePath);
    }catch(const std::exception&){
    }
  }

  return m_lastEpwFile;
}

boost::optional<openstudio::path> OSRunner::lastEpwFilePath() const
{
  return m_lastEpwFilePath;
}

bool OSRunner::inSelection(const openstudio::model::ModelObject& modelObject) const {
  return true;
}  

std::map<std::string, OSArgument> OSRunner::getUserInput(std::vector<OSArgument>& arguments) const
{
  return convertOSArgumentVectorToMap(arguments);
}

void OSRunner::prepareForUserScriptRun(const UserScript& userScript) {
  m_result = OSResult();
  m_measureName = userScript.name();
  std::stringstream ss;
  ss << "openstudio.ruleset." << m_measureName;
  m_channel = ss.str();
}

void OSRunner::registerError(const std::string& message) {
  m_result.setValue(OSResultValue::Fail);
  m_result.addError(m_channel,message);
}

bool OSRunner::registerWarning(const std::string& message) {
  m_result.addWarning(m_channel,message);
  return true;
}

bool OSRunner::registerInfo(const std::string& message) {
  m_result.addInfo(m_channel,message);
  return true;
}

void OSRunner::registerAsNotApplicable(const std::string& message) {
  m_result.setValue(OSResultValue::NA);
  m_result.addInfo(m_channel,message);
}

void OSRunner::registerInitialCondition(const std::string& message) {
  m_result.setInitialCondition(m_channel, message);
}

void OSRunner::registerFinalCondition(const std::string& message) {
  m_result.setFinalCondition(m_channel, message);
}

void OSRunner::registerAttribute(Attribute& attribute) {
  attribute.setSource(m_measureName);
  m_result.appendAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, bool value) {
  Attribute attribute(name,value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             bool value)
{
  Attribute attribute(name,value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, double value) {
  Attribute attribute(name,value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, double value, const std::string& units) {
  Attribute attribute(name,value,units);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value)
{
  Attribute attribute(name,value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value,
                             const std::string& units)
{
  Attribute attribute(name,value,units);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, int value) {
  Attribute attribute(name,value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, int value, const std::string& units) {
  Attribute attribute(name,value,units);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value)
{
  Attribute attribute(name,value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value,
                             const std::string& units)
{
  Attribute attribute(name,value,units);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, const std::string& value) {
  Attribute attribute(name,value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             const std::string& value)
{
  Attribute attribute(name,value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::createProgressBar(const std::string& text) const {
}

void OSRunner::updateProgress(const double& value) const {
}

void OSRunner::destroyProgressBar() const {
}

bool OSRunner::validateUserArguments(const std::vector<OSArgument>& script_arguments,
                                     const std::map<std::string, OSArgument>& user_arguments)
{
  bool result(true);
  std::stringstream ss;
  AttributeVector argumentValueAttributes;
  for (const OSArgument& script_argument : script_arguments) {
    auto it = user_arguments.find(script_argument.name());
    if (it == user_arguments.end()) {
      // script_argument is not in user_arguments
      // this is only okay for purely optional arguments
      if (script_argument.required() || script_argument.hasDefaultValue()) {
        ss << "Argument " << script_argument.name() << " is required or has a default value, ";
        ss << "but is not in user_arguments.";
        registerError(ss.str()); ss.str("");
        result = false;
      }
    }
    else {
      // script_argument is in user_arguments
      OSArgument user_argument = it->second;
      if (user_argument.name() != script_argument.name()) {
        ss << "User argument name '" << user_argument.name() << "' does not match map key ";
        ss << script_argument.name() << ".";
        registerWarning(ss.str()); ss.str("");
      }
      if (user_argument.type() != script_argument.type()) {
        ss << "User argument type " << user_argument.type().valueName() << " does not match ";
        ss << "script argument type " << script_argument.type().valueName() << ".";
        registerError(ss.str()); ss.str("");
        result = false;
      }
      if ((script_argument.required() || script_argument.hasDefaultValue()) &&
          !(user_argument.hasValue() || user_argument.hasDefaultValue()))
      {
        ss << "Script argument '" << script_argument.name() << "' is required or has a default ";
        ss << "value, but the user argument does not have a value or default value set.";
        registerError(ss.str()); ss.str("");
        result = false;
      }
      if (script_argument.hasDefaultValue() && !user_argument.hasDefaultValue()) {
        ss << "Script argument '" << script_argument.name() << "' has a default value, but the ";
        ss << "user-supplied version does not.";
        registerWarning(ss.str()); ss.str("");
      }
      if (!script_argument.hasDefaultValue() && user_argument.hasDefaultValue()) {
        ss << "Script argument '" << script_argument.name() << "' does not have a default value, ";
        ss << "but the user-supplied version does.";
        registerWarning(ss.str()); ss.str("");
      }
      if (script_argument.hasDefaultValue() && user_argument.hasDefaultValue() &&
          (user_argument.type() == script_argument.type()))
      {
        ss << "The default value of script argument " << std::endl << script_argument << std::endl;
        ss << "does not match that of the corresponding user argument " << std::endl << user_argument << ".";
        switch (script_argument.type().value()) {
          case OSArgumentType::Boolean :
            if (user_argument.defaultValueAsBool() != script_argument.defaultValueAsBool()) {
              registerWarning(ss.str());
            }
            break;
          case OSArgumentType::Double :
            if (!equal<double>(user_argument.defaultValueAsDouble(),script_argument.defaultValueAsDouble())) {
              registerWarning(ss.str());
            }
            break;
          case OSArgumentType::Quantity :
            if (user_argument.defaultValueAsQuantity() != script_argument.defaultValueAsQuantity()) {
              registerWarning(ss.str());
            }
            break;
          case OSArgumentType::Integer :
            if (user_argument.defaultValueAsInteger() != script_argument.defaultValueAsInteger()) {
              registerWarning(ss.str());
            }
            break;
          case OSArgumentType::String :
          case OSArgumentType::Choice :
          case OSArgumentType::Path :
            if (user_argument.defaultValueAsString() != script_argument.defaultValueAsString()) {
              registerWarning(ss.str());
            }
            break;
          default :
            OS_ASSERT(false);
        }
        ss.str("");
      }
      if (result) {
        Quantity q;
        switch(user_argument.type().value()) {
          case OSArgumentType::Boolean :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsBool()));
            }
            else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsBool()));
            }
           break;
          case OSArgumentType::Double :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsDouble()));
            }
            else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsDouble()));
            }
           break;
          case OSArgumentType::Quantity :
            if (user_argument.hasValue()) {
              q = user_argument.valueAsQuantity();
            }
            else if (user_argument.hasDefaultValue()) {
              q = user_argument.defaultValueAsQuantity();
            }
            argumentValueAttributes.push_back(Attribute(user_argument.name(),q.value(),q.units().print()));
           break;
          case OSArgumentType::Integer :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsInteger()));
            }
            else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsInteger()));
            }
           break;
          case OSArgumentType::String :
          case OSArgumentType::Choice :
          case OSArgumentType::Path :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsString()));
            }
            else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsString()));
            }
           break;
          default:
            OS_ASSERT(false);
        }
      }
    }
  }

  if (result) {
    for (Attribute& attribute : argumentValueAttributes) {
      registerAttribute(attribute);
    }
  }

  return result;
}

bool OSRunner::getBoolArgumentValue(const std::string& argument_name,
                                    const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsBool();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsBool();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return false;
}

double OSRunner::getDoubleArgumentValue(const std::string& argument_name,
                                        const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsDouble();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsDouble();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return 0.0;
}

boost::optional<double> OSRunner::getOptionalDoubleArgumentValue(
    const std::string& argument_name,
    const std::map<std::string,OSArgument>& user_arguments)
{
  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsDouble();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsDouble();
    }
  }

  return boost::none;
}

Quantity OSRunner::getQuantityArgumentValue(const std::string& argument_name,
                                            const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsQuantity();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsQuantity();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return Quantity();
}

boost::optional<Quantity> OSRunner::getOptionalQuantityArgumentValue(
    const std::string& argument_name,
    const std::map<std::string,OSArgument>& user_arguments)
{
  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsQuantity();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsQuantity();
    }
  }

  return boost::none;
}

int OSRunner::getIntegerArgumentValue(const std::string& argument_name,
                                      const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsInteger();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsInteger();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return 0;
}

boost::optional<int> OSRunner::getOptionalIntegerArgumentValue(
    const std::string& argument_name,
    const std::map<std::string,OSArgument>& user_arguments)
{
  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsInteger();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsInteger();
    }
  }

  return boost::none;
}

std::string OSRunner::getStringArgumentValue(const std::string& argument_name,
                                             const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsString();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsString();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return std::string();
}

boost::optional<std::string> OSRunner::getOptionalStringArgumentValue(
    const std::string& argument_name,
    const std::map<std::string,OSArgument>& user_arguments)
{
  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsString();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsString();
    }
  }

  return boost::none;
}

openstudio::path OSRunner::getPathArgumentValue(const std::string& argument_name,
                                                const std::map<std::string,OSArgument>& user_arguments)
{
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsPath();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsPath();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << std::endl << it->second;
  }
  registerError(ss.str());
  LOG_AND_THROW(ss.str());
  return openstudio::path();
}

boost::optional<openstudio::path> OSRunner::getOptionalPathArgumentValue(
    const std::string& argument_name,
    const std::map<std::string,OSArgument>& user_arguments)
{
  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsPath();
    }
    else if (it->second.hasDefaultValue()) {
      return it->second.defaultValueAsPath();
    }
  }

  return boost::none;
}

boost::optional<openstudio::WorkspaceObject> OSRunner::getOptionalWorkspaceObjectChoiceValue(
  const std::string& argument_name,
  const std::map<std::string,OSArgument>& user_arguments, 
  const openstudio::Workspace& workspace)

{
  auto it = user_arguments.find(argument_name);
  std::string handleString;
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      handleString = it->second.valueAsString();
    }
    else if (it->second.hasDefaultValue()) {
      handleString = it->second.defaultValueAsString();
    }
  }

  Handle handle = toUUID(handleString);
  boost::optional<WorkspaceObject> result = workspace.getObject(handle);

  return result;
}

void OSRunner::setLastOpenStudioModel(const openstudio::model::Model& lastOpenStudioModel)
{
  m_lastOpenStudioModel = lastOpenStudioModel;
  m_lastOpenStudioModelPath.reset();
}

void OSRunner::resetLastOpenStudioModel()
{
  m_lastOpenStudioModel.reset();
  m_lastOpenStudioModelPath.reset();
}

void OSRunner::setLastOpenStudioModelPath(const openstudio::path& lastOpenStudioModelPath)
{
  m_lastOpenStudioModelPath = lastOpenStudioModelPath;
  m_lastOpenStudioModel.reset();
}

void OSRunner::resetLastOpenStudioModelPath()
{
  m_lastOpenStudioModel.reset();
  m_lastOpenStudioModelPath.reset();
}

void OSRunner::setLastEnergyPlusWorkspace(const openstudio::Workspace& lastEnergyPlusWorkspace)
{
  m_lastEnergyPlusWorkspace = lastEnergyPlusWorkspace;
  m_lastEnergyPlusWorkspacePath.reset();
}

void OSRunner::resetLastEnergyPlusWorkspace()
{
  m_lastEnergyPlusWorkspace.reset();
  m_lastEnergyPlusWorkspacePath.reset();
}

void OSRunner::setLastEnergyPlusWorkspacePath(const openstudio::path& lastEnergyPlusWorkspacePath)
{
  m_lastEnergyPlusWorkspacePath = lastEnergyPlusWorkspacePath;
  m_lastEnergyPlusWorkspace.reset();
}

void OSRunner::resetLastEnergyPlusWorkspacePath()
{
  m_lastEnergyPlusWorkspace.reset();
  m_lastEnergyPlusWorkspacePath.reset();
}

void OSRunner::setLastEnergyPlusSqlFilePath(const openstudio::path& lastEnergyPlusSqlFilePath)
{
  m_lastEnergyPlusSqlFilePath = lastEnergyPlusSqlFilePath;
  m_lastEnergyPlusSqlFile.reset();
}

void OSRunner::resetLastEnergyPlusSqlFilePath()
{
  m_lastEnergyPlusSqlFilePath.reset();
  m_lastEnergyPlusSqlFile.reset();
}

void OSRunner::setLastEpwFilePath(const openstudio::path& lastEpwFilePath)
{
  m_lastEpwFilePath = lastEpwFilePath;
  m_lastEpwFile.reset();
}

void OSRunner::resetLastEpwFilePath()
{
  m_lastEpwFilePath.reset();
  m_lastEpwFile.reset();
}

} // ruleset
} // openstudio
