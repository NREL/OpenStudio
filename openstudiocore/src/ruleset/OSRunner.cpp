/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "OSRunner.hpp"

#include "OSArgument.hpp"
#include "UserScript.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/StringHelpers.hpp"
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
  Attribute attribute(cleanValueName(name),value);
  attribute.setDisplayName(name);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             bool value)
{
  Attribute attribute(cleanValueName(name),value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, double value) {
  Attribute attribute(cleanValueName(name),value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, double value, const std::string& units) {
  Attribute attribute(cleanValueName(name),value,units);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value)
{
  Attribute attribute(cleanValueName(name),value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value,
                             const std::string& units)
{
  Attribute attribute(cleanValueName(name),value,units);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, int value) {
  Attribute attribute(cleanValueName(name),value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, int value, const std::string& units) {
  Attribute attribute(cleanValueName(name),value,units);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value)
{
  Attribute attribute(cleanValueName(name),value);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value,
                             const std::string& units)
{
  Attribute attribute(cleanValueName(name),value,units);
  attribute.setDisplayName(displayName);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name, const std::string& value) {
  Attribute attribute(cleanValueName(name),value);
  registerAttribute(attribute);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             const std::string& value)
{
  Attribute attribute(cleanValueName(name),value);
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

      // check that names still match 
      if (user_argument.name() != script_argument.name()) {
        ss << "User argument name '" << user_argument.name() << "' does not match map key ";
        ss << script_argument.name() << ".";
        registerWarning(ss.str()); ss.str("");
      }

      // check that types still match
      if (user_argument.type() != script_argument.type()) {
        ss << "User argument type " << user_argument.type().valueName() << " does not match ";
        ss << "script argument type " << script_argument.type().valueName() << ".";
        registerError(ss.str()); ss.str("");
        result = false;
      }

      //  check that we have values
      if ((script_argument.required()) &&
          !(user_argument.hasValue() || user_argument.hasDefaultValue()))
      {
        ss << "Script argument '" << script_argument.name() << "' is required, ";
        ss << "but the user argument does not have a value or default value set.";
        registerError(ss.str()); ss.str("");
        result = false;
      }

      // check for default value mismatch
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
            if (user_argument.defaultValueAsDouble() != script_argument.defaultValueAsDouble()) {
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

      // check for domain mismatch
      if (script_argument.hasDomain() && !user_argument.hasDomain()) {
        ss << "Script argument '" << script_argument.name() << "' has a specified domain, but the ";
        ss << "user-supplied version does not.";
        registerWarning(ss.str()); ss.str("");
      }
      if (!script_argument.hasDomain() && user_argument.hasDomain()) {
        ss << "Script argument '" << script_argument.name() << "' does not have a specified domain, ";
        ss << "but the user-supplied version does.";
        registerWarning(ss.str()); ss.str("");
      }
      if (script_argument.hasDomain() && user_argument.hasDomain() &&
        (user_argument.type() == script_argument.type()))
      {
        ss << "The domain of script argument " << std::endl << script_argument << std::endl;
        ss << "does not match that of the corresponding user argument " << std::endl << user_argument << ".";
        switch (script_argument.type().value()) {
        case OSArgumentType::Boolean:
          // DLM: should bool's even have domains?
          if (user_argument.domainAsBool() != script_argument.domainAsBool()) {
            registerWarning(ss.str());
          }
          break;
        case OSArgumentType::Double:
          if (user_argument.domainAsDouble() != script_argument.domainAsDouble()) {
            registerWarning(ss.str());
          }
          break;
        case OSArgumentType::Quantity:
          // DLM: deprecated forget about getting this to compile
          //if (user_argument.domainAsQuantity() != script_argument.domainAsQuantity()) {
          //  registerWarning(ss.str());
          //}
          break;
        case OSArgumentType::Integer:
          if (user_argument.domainAsInteger() != script_argument.domainAsInteger()) {
            registerWarning(ss.str());
          }
          break;
        case OSArgumentType::String:
        case OSArgumentType::Choice:
        case OSArgumentType::Path:
          if (user_argument.domainAsString() != script_argument.domainAsString()) {
            registerWarning(ss.str());
          }
          break;
        default:
          OS_ASSERT(false);
        }
        ss.str("");
      }

      // success, set the values
      if (result) {
        Quantity q;
        switch(user_argument.type().value()) {
          case OSArgumentType::Boolean :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsBool()));
            } else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsBool()));
            }
           break;
          case OSArgumentType::Double :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsDouble()));
            } else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsDouble()));
            }
           break;
          case OSArgumentType::Quantity :
            if (user_argument.hasValue()) {
              q = user_argument.valueAsQuantity();
            } else if (user_argument.hasDefaultValue()) {
              q = user_argument.defaultValueAsQuantity();
            }
            argumentValueAttributes.push_back(Attribute(user_argument.name(),q.value(),q.units().print()));
           break;
          case OSArgumentType::Integer :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsInteger()));
            } else if (user_argument.hasDefaultValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.defaultValueAsInteger()));
            }
           break;
          case OSArgumentType::String :
          case OSArgumentType::Choice :
          case OSArgumentType::Path :
            if (user_argument.hasValue()) {
              argumentValueAttributes.push_back(Attribute(user_argument.name(),user_argument.valueAsString()));
            } else if (user_argument.hasDefaultValue()) {
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
  if (boost::filesystem::exists(lastOpenStudioModelPath)){
    m_lastOpenStudioModelPath = boost::filesystem::canonical(lastOpenStudioModelPath);
  }else{
    m_lastOpenStudioModelPath = lastOpenStudioModelPath;
  }
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
  if (boost::filesystem::exists(lastEnergyPlusWorkspacePath)){
    m_lastEnergyPlusWorkspacePath = boost::filesystem::canonical(lastEnergyPlusWorkspacePath);
  }else{
    m_lastEnergyPlusWorkspacePath = lastEnergyPlusWorkspacePath;
  }
  m_lastEnergyPlusWorkspace.reset();
}

void OSRunner::resetLastEnergyPlusWorkspacePath()
{
  m_lastEnergyPlusWorkspace.reset();
  m_lastEnergyPlusWorkspacePath.reset();
}

void OSRunner::setLastEnergyPlusSqlFilePath(const openstudio::path& lastEnergyPlusSqlFilePath)
{
  if (boost::filesystem::exists(lastEnergyPlusSqlFilePath)){
    m_lastEnergyPlusSqlFilePath = boost::filesystem::canonical(lastEnergyPlusSqlFilePath);
  }else{
    m_lastEnergyPlusSqlFilePath = lastEnergyPlusSqlFilePath;
  }
  m_lastEnergyPlusSqlFile.reset();
}

void OSRunner::resetLastEnergyPlusSqlFilePath()
{
  m_lastEnergyPlusSqlFilePath.reset();
  m_lastEnergyPlusSqlFile.reset();
}

void OSRunner::setLastEpwFilePath(const openstudio::path& lastEpwFilePath)
{
  if (boost::filesystem::exists(lastEpwFilePath)){
    m_lastEpwFilePath = boost::filesystem::canonical(lastEpwFilePath);
  }else{
    m_lastEpwFilePath = lastEpwFilePath;
  }
  m_lastEpwFile.reset();
}

void OSRunner::resetLastEpwFilePath()
{
  m_lastEpwFilePath.reset();
  m_lastEpwFile.reset();
}

std::string OSRunner::cleanValueName(const std::string& name) const
{
  static const boost::regex allowableCharacters("[^0-9a-zA-Z]");
  static const boost::regex leadingUnderscore("^_+");
  static const boost::regex trailingUnderscore("_+$");
  static const boost::regex leadingNumber("^([0-9])");
  static const boost::regex multipleUnderscore("[_]+");


  std::string result = boost::regex_replace(name, allowableCharacters, "_");
  result = boost::regex_replace(result, leadingUnderscore, "");
  result = boost::regex_replace(result, trailingUnderscore, "");
  result = boost::regex_replace(result, leadingNumber, "_$1");
  result = boost::regex_replace(result, multipleUnderscore, "_");
  result = toUnderscoreCase(result);
  return result;
}

} // ruleset
} // openstudio
