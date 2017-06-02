/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "OSMeasure.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace openstudio {
namespace measure {

OSRunner::OSRunner(const WorkflowJSON& workflow)
  : m_workflow(workflow), m_startedStep(false), m_streamsCaptured(false), 
    m_unitsPreference("IP"), m_languagePreference("en"), m_halted(false),
    m_originalStdOut(nullptr), m_originalStdErr(nullptr)
{
}

OSRunner::~OSRunner()
{
  restoreStreams();
}

WorkflowJSON OSRunner::workflow() const
{
  // return a clone because the workflow is immutable
  return m_workflow.clone();
}

std::string OSRunner::unitsPreference() const
{
  return m_unitsPreference;
}

std::string OSRunner::languagePreference() const
{
  return m_languagePreference;
}

WorkflowStepResult OSRunner::result() const {
  return m_result;
}

bool OSRunner::halted() const {
  return m_halted;
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

void OSRunner::reset()
{
  m_workflow.reset();
  m_startedStep = false;

  restoreStreams();

  //m_unitsPreference // do not reset
  //m_languagePreference; // do not reset

  m_halted = false;

  m_result = WorkflowStepResult();

  m_lastOpenStudioModel.reset();
  m_lastOpenStudioModelPath.reset();
  m_lastEnergyPlusWorkspace.reset();
  m_lastEnergyPlusWorkspacePath.reset();
  m_lastEnergyPlusSqlFile.reset();
  m_lastEnergyPlusSqlFilePath.reset();
  m_lastEpwFile.reset();
  m_lastEpwFilePath.reset();

  m_currentDir.reset();
  m_currentDirFiles.clear();
}

bool OSRunner::incrementStep()
{
  // DLM: increment step is called when halted, just skips the steps
  //if (m_halted){
  //  LOG(Error, "Wokflow halted, cannot increment step");
  //  return false;
  //}
  if (!m_startedStep){
    LOG(Error, "Not prepared for step");
    return false;
  }
  boost::optional<WorkflowStep> currentStep = m_workflow.currentStep();
  if (!currentStep){
    LOG(Error, "Cannot find current Workflow Step");
    return false;
  }

  if (!m_result.stepResult()){
    // must have been skipped
    m_result.setStepResult(StepResult::Skip);
  }

  // restore stdout and stderr
  m_result.setStdOut(m_bufferStdOut.str());
  m_result.setStdErr(m_bufferStdErr.str());
  restoreStreams();

  // check for created files
  
  // get list of new files
  if (m_currentDir){
    openstudio::path absoluteRootDir = m_workflow.absoluteRootDir();
    if (boost::filesystem::exists(*m_currentDir) && boost::filesystem::is_directory(*m_currentDir))
    {
      for (boost::filesystem::directory_iterator dir_iter(*m_currentDir), end_iter; dir_iter != end_iter; ++dir_iter)
      {
        if (boost::filesystem::is_regular_file(dir_iter->status()))
        {
          if (m_currentDirFiles.find(dir_iter->path()) == m_currentDirFiles.end()){

            openstudio::path path = dir_iter->path();
            OS_ASSERT(path.is_absolute());

            // DLM: we need to figure out what these should be
            // are they absolute, relative to root dir, relative to measure dir, valid after reports have been packed up?
            //try{
            //  path = relativePath(path, absoluteRootDir);
            //} catch (const std::exception&){
            //}
            m_result.addStepFile(path);
          }
        }
      }
    }
  }
  m_currentDir.reset();
  m_currentDirFiles.clear();

  m_result.setCompletedAt(DateTime::nowUTC());
    
  currentStep->setResult(m_result);
  
  m_result = WorkflowStepResult();
  m_startedStep = false;

  return m_workflow.incrementStep();
}

//void OSRunner::setCurrentStep(unsigned currentStep)
//{
//  m_previousResults.resize(currentStep);
//  m_result = WorkflowStepResult();
//  m_result.setStepResult(StepResult::Skip);
//
//  m_currentStep = currentStep;
//}

void OSRunner::prepareForMeasureRun(const OSMeasure& measure) {
  if (m_halted){
    LOG(Error, "Wokflow halted, cannot prepate for measure run");
    return;
  }
  if (m_startedStep){
    LOG(Error, "Step already started");
    return;
  }
  boost::optional<WorkflowStep> currentStep = m_workflow.currentStep();
  if (!currentStep){
    LOG(Error, "Cannot find current Workflow Step");
    return;
  }

  m_startedStep = true;

  // create a new result
  m_result = WorkflowStepResult();
  m_result.setStartedAt(DateTime::nowUTC());
  m_result.setStepResult(StepResult::Success);

  // capture std out and err
  captureStreams();

  // get initial list of files
  m_currentDir = boost::filesystem::current_path();
  if (boost::filesystem::exists(*m_currentDir) && boost::filesystem::is_directory(*m_currentDir))
  {
    for( boost::filesystem::directory_iterator dir_iter(*m_currentDir), end_iter ; dir_iter != end_iter ; ++dir_iter)
    {
      if (boost::filesystem::is_regular_file(dir_iter->status()) )
      {
        m_currentDirFiles.insert(dir_iter->path());
      }
    }
  }
}

void OSRunner::registerError(const std::string& message) {
  m_result.setStepResult(StepResult::Fail);
  m_result.addStepError(message);
}

bool OSRunner::registerWarning(const std::string& message) {
  m_result.addStepWarning(message);
  return true;
}

bool OSRunner::registerInfo(const std::string& message) {
  m_result.addStepInfo(message);
  return true;
}

void OSRunner::registerAsNotApplicable(const std::string& message) {
  m_result.setStepResult(StepResult::NA);
  m_result.addStepInfo(message);
}

void OSRunner::registerInitialCondition(const std::string& message) {
  m_result.setStepInitialCondition(message);
}

void OSRunner::registerFinalCondition(const std::string& message) {
  m_result.setStepFinalCondition(message);
}


void OSRunner::registerValue(const std::string& name, bool value) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             bool value)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name, double value) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name, double value, const std::string& units) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setUnits(units);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             double value,
                             const std::string& units)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  stepValue.setUnits(units);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name, int value) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  m_result.addStepValue(stepValue);}

void OSRunner::registerValue(const std::string& name, int value, const std::string& units) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setUnits(units);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             int value,
                             const std::string& units)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  stepValue.setUnits(units);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name, const std::string& value) {
  WorkflowStepValue stepValue(cleanValueName(name),value);
  m_result.addStepValue(stepValue);
}

void OSRunner::registerValue(const std::string& name,
                             const std::string& displayName,
                             const std::string& value)
{
  WorkflowStepValue stepValue(cleanValueName(name),value);
  stepValue.setDisplayName(displayName);
  m_result.addStepValue(stepValue);
}

void OSRunner::haltWorkflow(const std::string& completedStatus)
{
  m_halted = true;
  m_workflow.setCompletedStatus(completedStatus);
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
  std::vector<WorkflowStepValue> stepValues;
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
        switch(user_argument.type().value()) {
          case OSArgumentType::Boolean :
            if (user_argument.hasValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.valueAsBool()));
            } else if (user_argument.hasDefaultValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.defaultValueAsBool()));
            }
           break;
          case OSArgumentType::Double :
            if (user_argument.hasValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.valueAsDouble()));
            } else if (user_argument.hasDefaultValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.defaultValueAsDouble()));
            }
           break;
          case OSArgumentType::Integer :
            if (user_argument.hasValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.valueAsInteger()));
            } else if (user_argument.hasDefaultValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.defaultValueAsInteger()));
            }
           break;
          case OSArgumentType::String :
          case OSArgumentType::Choice :
          case OSArgumentType::Path :
            if (user_argument.hasValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.valueAsString()));
            } else if (user_argument.hasDefaultValue()) {
              stepValues.push_back(WorkflowStepValue(user_argument.name(),user_argument.defaultValueAsString()));
            }
           break;
          default:
            OS_ASSERT(false);
        }
      }
    }
  }

  if (result) {
    for (const auto& stepValue : stepValues) {
      m_result.addStepValue(stepValue);
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
  if (openstudio::filesystem::exists(lastOpenStudioModelPath)){
    m_lastOpenStudioModelPath = openstudio::filesystem::canonical(lastOpenStudioModelPath);
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
  if (openstudio::filesystem::exists(lastEnergyPlusWorkspacePath)){
    m_lastEnergyPlusWorkspacePath = openstudio::filesystem::canonical(lastEnergyPlusWorkspacePath);
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
  if (openstudio::filesystem::exists(lastEnergyPlusSqlFilePath)){
    m_lastEnergyPlusSqlFilePath = openstudio::filesystem::canonical(lastEnergyPlusSqlFilePath);
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
  if (openstudio::filesystem::exists(lastEpwFilePath)){
    m_lastEpwFilePath = openstudio::filesystem::canonical(lastEpwFilePath);
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

bool OSRunner::setUnitsPreference(const std::string& unitsPreference)
{
  m_unitsPreference = unitsPreference;
  return true;
}

void OSRunner::resetUnitsPreference()
{
  m_unitsPreference = "IP";
}

bool OSRunner::setLanguagePreference(const std::string& languagePreference)
{
  m_languagePreference = languagePreference;
  return true;
}

void OSRunner::resetLanguagePreference()
{
  m_languagePreference = "en";
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

void OSRunner::captureStreams()
{
  if (m_streamsCaptured){
    return;
  }
  m_streamsCaptured = true;

  std::cout.flush();
  std::cout.rdbuf()->pubsync();
  m_originalStdOut = std::cout.rdbuf();
  m_bufferStdOut.str("");
  std::cout.rdbuf(m_bufferStdOut.rdbuf());

  std::cerr.flush();
  std::cerr.rdbuf()->pubsync();
  m_originalStdErr = std::cerr.rdbuf();
  m_bufferStdErr.str("");
  std::cerr.rdbuf(m_bufferStdErr.rdbuf());
}

void OSRunner::restoreStreams()
{
  if (!m_streamsCaptured){
    return;
  }
  m_streamsCaptured = false;

  OS_ASSERT(m_originalStdOut);
  OS_ASSERT(m_originalStdErr);

  std::cout.rdbuf(m_originalStdOut);
  std::cout << m_bufferStdOut.str();
  std::cout.flush();
  m_originalStdOut = nullptr;
  m_bufferStdOut.str("");

  std::cerr.rdbuf(m_originalStdErr);
  std::cerr << m_bufferStdErr.str();
  std::cerr.flush();
  m_originalStdErr = nullptr;
  m_bufferStdErr.str("");

}

} // measure
} // openstudio
