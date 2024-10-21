/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSRunner.hpp"

#include "OSArgument.hpp"
#include "OSMeasure.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/filetypes/WorkflowStep_Impl.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/math/FloatCompare.hpp"

#include <fmt/format.h>
#include <cstdio>
#include <cstdlib>

namespace openstudio {
namespace measure {

  OSRunner::OSRunner(const WorkflowJSON& workflow)
    : m_workflow(workflow),
      m_startedStep(false),
      m_streamsCaptured(false),
      m_unitsPreference("IP"),
      m_languagePreference("en"),
      m_halted(false),
      m_originalStdOut(nullptr),
      m_originalStdErr(nullptr) {}

  OSRunner::~OSRunner() {
    restoreStreams();
  }

  WorkflowJSON OSRunner::workflow() const {
    // return a clone because the workflow is immutable
    return m_workflow.clone();
  }

  alfalfa::AlfalfaJSON OSRunner::alfalfa() const {
    return m_alfalfa;
  }

  std::string OSRunner::unitsPreference() const {
    return m_unitsPreference;
  }

  std::string OSRunner::languagePreference() const {
    return m_languagePreference;
  }

  WorkflowStepResult OSRunner::result() const {
    return m_result;
  }

  bool OSRunner::halted() const {
    return m_halted;
  }

  boost::optional<openstudio::model::Model> OSRunner::lastOpenStudioModel() const {
    if (m_lastOpenStudioModel) {
      return m_lastOpenStudioModel;
    }

    if (m_lastOpenStudioModelPath) {
      m_lastOpenStudioModel = model::Model::load(*m_lastOpenStudioModelPath);
    }

    return m_lastOpenStudioModel;
  }

  boost::optional<openstudio::Workspace> OSRunner::lastEnergyPlusWorkspace() const {
    if (m_lastEnergyPlusWorkspace) {
      return m_lastEnergyPlusWorkspace;
    }

    if (m_lastEnergyPlusWorkspacePath) {
      m_lastEnergyPlusWorkspace = Workspace::load(*m_lastEnergyPlusWorkspacePath);
    }

    return m_lastEnergyPlusWorkspace;
  }

  boost::optional<openstudio::SqlFile> OSRunner::lastEnergyPlusSqlFile() const {
    if (m_lastEnergyPlusSqlFile) {
      return m_lastEnergyPlusSqlFile;
    }

    if (m_lastEnergyPlusSqlFilePath) {
      try {
        m_lastEnergyPlusSqlFile = SqlFile(*m_lastEnergyPlusSqlFilePath);
      } catch (const std::exception&) {
      }
    }

    return m_lastEnergyPlusSqlFile;
  }

  boost::optional<openstudio::EpwFile> OSRunner::lastEpwFile() const {
    if (m_lastEpwFile) {
      return m_lastEpwFile;
    }

    if (m_lastEpwFilePath) {
      try {
        m_lastEpwFile = EpwFile(*m_lastEpwFilePath);
      } catch (const std::exception&) {
      }
    }

    return m_lastEpwFile;
  }

  boost::optional<openstudio::path> OSRunner::lastEpwFilePath() const {
    return m_lastEpwFilePath;
  }

  bool OSRunner::inSelection(const openstudio::model::ModelObject& /*modelObject*/) const {
    return true;
  }

  std::map<std::string, OSArgument> OSRunner::getUserInput(std::vector<OSArgument>& arguments) const {
    return convertOSArgumentVectorToMap(arguments);
  }

  void OSRunner::reset() {
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

  bool OSRunner::incrementStep() {
    // DLM: increment step is called when halted, just skips the steps
    //if (m_halted){
    //  LOG(Error, "Wokflow halted, cannot increment step");
    //  return false;
    //}
    if (!m_startedStep) {
      LOG(Error, "Not prepared for step");
      return false;
    }
    boost::optional<WorkflowStep> currentStep = m_workflow.currentStep();
    if (!currentStep) {
      LOG(Error, "Cannot find current Workflow Step");
      return false;
    }

    if (!m_result.stepResult()) {
      // must have been skipped
      m_result.setStepResult(StepResult::Skip);
    }

    // restore stdout and stderr
    m_result.setStdOut(m_bufferStdOut.str());
    m_result.setStdErr(m_bufferStdErr.str());
    restoreStreams();

    // check for created files

    // get list of new files
    if (m_currentDir) {
      const openstudio::path absoluteRootDir = m_workflow.absoluteRootDir();
      if (boost::filesystem::exists(*m_currentDir) && boost::filesystem::is_directory(*m_currentDir)) {
        for (boost::filesystem::directory_iterator dir_iter(*m_currentDir), end_iter; dir_iter != end_iter; ++dir_iter) {
          if (boost::filesystem::is_regular_file(dir_iter->status())) {
            if (m_currentDirFiles.find(dir_iter->path()) == m_currentDirFiles.end()) {

              const openstudio::path& fPath = dir_iter->path();
              OS_ASSERT(fPath.is_absolute());

              // DLM: we need to figure out what these should be
              // are they absolute, relative to root dir, relative to measure dir, valid after reports have been packed up?
              //try{
              //  path = relativePath(path, absoluteRootDir);
              //} catch (const std::exception&){
              //}
              m_result.addStepFile(fPath);
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

  void OSRunner::prepareForMeasureRun(const OSMeasure& /*measure*/) {
    prepareForMeasureRun();
  }

  void OSRunner::prepareForMeasureRun() {
    if (m_halted) {
      LOG(Error, "Wokflow halted, cannot prepate for measure run");
      return;
    }
    if (m_startedStep) {
      LOG(Error, "Step already started");
      return;
    }
    const boost::optional<WorkflowStep> currentStep = m_workflow.currentStep();
    if (!currentStep) {
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
    if (boost::filesystem::exists(*m_currentDir) && boost::filesystem::is_directory(*m_currentDir)) {
      for (boost::filesystem::directory_iterator dir_iter(*m_currentDir), end_iter; dir_iter != end_iter; ++dir_iter) {
        if (boost::filesystem::is_regular_file(dir_iter->status())) {
          m_currentDirFiles.insert(dir_iter->path());
        }
      }
    }
  }

  bool OSRunner::registerMsgAlsoLogs() const {
    return m_registerMsgAlsoLogs;
  }

  void OSRunner::setRegisterMsgAlsoLogs(bool registerMsgAlsoLogs) {
    m_registerMsgAlsoLogs = registerMsgAlsoLogs;
  }

  void OSRunner::registerError(const std::string& message) {
    m_result.setStepResult(StepResult::Fail);
    m_result.addStepError(message);
    if (m_registerMsgAlsoLogs) {
      LOG(Error, message);
    }
  }

  bool OSRunner::registerWarning(const std::string& message) {
    m_result.addStepWarning(message);
    if (m_registerMsgAlsoLogs) {
      LOG(Warn, message);
    }
    return true;
  }

  bool OSRunner::registerInfo(const std::string& message) {
    m_result.addStepInfo(message);
    if (m_registerMsgAlsoLogs) {
      LOG(Info, message);
    }
    return true;
  }

  void OSRunner::registerAsNotApplicable(const std::string& message) {
    m_result.setStepResult(StepResult::NA);
    m_result.addStepInfo(message);
    // even though openstudio-workflow-gem didn't do that that one, still doing it
    if (m_registerMsgAlsoLogs) {
      LOG(Warn, message);
    }
  }

  void OSRunner::registerInitialCondition(const std::string& message) {
    m_result.setStepInitialCondition(message);
    if (m_registerMsgAlsoLogs) {
      LOG(Info, message);
    }
  }

  void OSRunner::registerFinalCondition(const std::string& message) {
    m_result.setStepFinalCondition(message);
    if (m_registerMsgAlsoLogs) {
      LOG(Info, message);
    }
  }

  void OSRunner::registerValue(const std::string& name, bool value) {
    const WorkflowStepValue stepValue(cleanValueName(name), value);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, bool value) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, double value) {
    const WorkflowStepValue stepValue(cleanValueName(name), value);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, double value, const std::string& units) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setUnits(units);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, double value) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, double value, const std::string& units) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    stepValue.setUnits(units);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, int value) {
    const WorkflowStepValue stepValue(cleanValueName(name), value);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, int value, const std::string& units) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setUnits(units);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, int value) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, int value, const std::string& units) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    stepValue.setUnits(units);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& value) {
    const WorkflowStepValue stepValue(cleanValueName(name), value);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::registerValue(const std::string& name, const std::string& displayName, const std::string& value) {
    WorkflowStepValue stepValue(cleanValueName(name), value);
    stepValue.setDisplayName(displayName);
    m_result.addStepValue(stepValue);
  }

  void OSRunner::haltWorkflow(const std::string& completedStatus) {
    m_halted = true;
    m_workflow.setCompletedStatus(completedStatus);
  }

  void OSRunner::createProgressBar(const std::string& text) const {}

  void OSRunner::updateProgress(double value) const {}

  void OSRunner::destroyProgressBar() const {}

  bool OSRunner::validateUserArguments(const std::vector<OSArgument>& script_arguments, const std::map<std::string, OSArgument>& user_arguments) {
    bool result = true;
    std::vector<WorkflowStepValue> stepValues;
    for (const OSArgument& script_argument : script_arguments) {
      auto it = user_arguments.find(script_argument.name());
      if (it == user_arguments.end()) {
        // script_argument is not in user_arguments
        // this is only okay for purely optional arguments
        if (script_argument.required() || script_argument.hasDefaultValue()) {
          registerError(fmt::format("Argument '{}' is required or has a default value, but is not in user_arguments.", script_argument.name()));
          result = false;
        }
      } else {
        // script_argument is in user_arguments
        const OSArgument& user_argument = it->second;

        // check that names still match
        if (user_argument.name() != script_argument.name()) {
          registerWarning(fmt::format("User argument name '{}' does not match map key ", user_argument.name()));
        }

        // check that types still match
        if (user_argument.type() != script_argument.type()) {
          registerError(fmt::format("User argument type {} does not match script argument type {}.", user_argument.type().valueName(),
                                    script_argument.type().valueName()));
          result = false;
        }

        //  check that we have values
        if ((script_argument.required()) && !(user_argument.hasValue() || user_argument.hasDefaultValue())) {
          registerError(fmt::format("Script argument '{}' is required, but the user argument does not have a value or default value set.",
                                    script_argument.name()));
          result = false;
        }

        // check for default value mismatch
        if (script_argument.hasDefaultValue() && !user_argument.hasDefaultValue()) {
          registerWarning(fmt::format("Script argument '{}' has a default value, but the user-supplied version does not.", script_argument.name()));
        }
        if (!script_argument.hasDefaultValue() && user_argument.hasDefaultValue()) {
          registerWarning(
            fmt::format("Script argument '{}' does not have a default value, but the user-supplied version does.", script_argument.name()));
        }
        if (script_argument.hasDefaultValue() && user_argument.hasDefaultValue() && (user_argument.type() == script_argument.type())) {
          std::stringstream ss;
          ss << "The default value of script argument " << '\n'
             << script_argument << '\n'
             << "does not match that of the corresponding user argument " << '\n'
             << user_argument << ".";
          switch (script_argument.type().value()) {
            case OSArgumentType::Boolean:
              if (user_argument.defaultValueAsBool() != script_argument.defaultValueAsBool()) {
                registerWarning(ss.str());
              }
              break;
            case OSArgumentType::Double:
              if (user_argument.defaultValueAsDouble() != script_argument.defaultValueAsDouble()) {
                registerWarning(ss.str());
              }
              break;
            case OSArgumentType::Integer:
              if (user_argument.defaultValueAsInteger() != script_argument.defaultValueAsInteger()) {
                registerWarning(ss.str());
              }
              break;
            case OSArgumentType::String:
            case OSArgumentType::Choice:
            case OSArgumentType::Path:
              if (user_argument.defaultValueAsString() != script_argument.defaultValueAsString()) {
                registerWarning(ss.str());
              }
              break;
            default:
              OS_ASSERT(false);
          }
        }

        // check for domain mismatch
        if (script_argument.hasDomain() && !user_argument.hasDomain()) {
          registerWarning(
            fmt::format("Script argument '{}' has a specified domain, but the user-supplied version does not.", script_argument.name()));
        }
        if (!script_argument.hasDomain() && user_argument.hasDomain()) {
          registerWarning(
            fmt::format("Script argument '{}' does not have a specified domain, but the user-supplied version does.", script_argument.name()));
        }
        if (script_argument.hasDomain() && user_argument.hasDomain() && (user_argument.type() == script_argument.type())) {
          std::stringstream ss;
          ss << "The domain of script argument " << '\n' << script_argument << '\n';
          ss << "does not match that of the corresponding user argument " << '\n' << user_argument << ".";
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
        }

        // success, set the values
        if (result) {
          auto typeValue = user_argument.type().value();
          if (typeValue == OSArgumentType::Boolean) {
            if (user_argument.hasValue()) {
              stepValues.emplace_back(user_argument.name(), user_argument.valueAsBool());
            } else if (user_argument.hasDefaultValue()) {
              stepValues.emplace_back(user_argument.name(), user_argument.defaultValueAsBool());
            }
          } else if (typeValue == OSArgumentType::Double) {
            if (user_argument.hasValue() || user_argument.hasDefaultValue()) {
              auto value = user_argument.hasValue() ? user_argument.valueAsDouble() : user_argument.defaultValueAsDouble();
              if (script_argument.hasDomain()) {
                // Validate it's in the domain
                auto domain = script_argument.domainAsDouble();
                auto& low = domain.front();
                auto& high = domain.back();
                if (value < low || value > high) {
                  registerError(fmt::format("{} User argument '{}' has a value '{}' that is not in the domain [{}, {}].",
                                            user_argument.type().valueName(), script_argument.name(), value, low, high));
                  result = false;
                }
              }
              if (result) {
                stepValues.emplace_back(user_argument.name(), value);
              }
            }
          } else if (typeValue == OSArgumentType::Integer) {
            if (user_argument.hasValue() || user_argument.hasDefaultValue()) {
              auto value = user_argument.hasValue() ? user_argument.valueAsInteger() : user_argument.defaultValueAsInteger();
              if (script_argument.hasDomain()) {
                // Validate it's in the domain
                auto domain = script_argument.domainAsInteger();
                auto& low = domain.front();
                auto& high = domain.back();
                if (value < low || value > high) {
                  registerError(fmt::format("{} User argument '{}' has a value '{}' that is not in the domain [{}, {}].",
                                            user_argument.type().valueName(), script_argument.name(), value, low, high));
                  result = false;
                }
              }
              if (result) {
                stepValues.emplace_back(user_argument.name(), value);
              }
            }
          } else if ((typeValue == OSArgumentType::String) || (typeValue == OSArgumentType::Choice) || (typeValue == OSArgumentType::Path)) {
            if (user_argument.hasValue()) {
              stepValues.emplace_back(user_argument.name(), user_argument.valueAsString());
            } else if (user_argument.hasDefaultValue()) {
              stepValues.emplace_back(user_argument.name(), user_argument.defaultValueAsString());
            }
          } else {
            OS_ASSERT(false);
          }
        }  // end if result
      }
    }  // end for loop on arguments

    if (result) {
      for (const auto& stepValue : stepValues) {
        m_result.addStepValue(stepValue);
      }
    }

    return result;
  }

  bool OSRunner::getBoolArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments) {
    std::stringstream ss;

    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsBool();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsBool();
      }
    }

    ss << "No value found for argument '" << argument_name << "'.";
    if (it != user_arguments.end()) {
      ss << " Full argument as passed in by user:" << '\n' << it->second;
    }
    registerError(ss.str());
    LOG_AND_THROW(ss.str());
    return false;
  }

  boost::optional<bool> OSRunner::getOptionalBoolArgumentValue(const std::string& argument_name,
                                                               const std::map<std::string, OSArgument>& user_arguments) {
    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsBool();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsBool();
      }
    }

    return boost::none;
  }

  double OSRunner::getDoubleArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments) {
    std::stringstream ss;

    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsDouble();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsDouble();
      }
    }

    ss << "No value found for argument '" << argument_name << "'.";
    if (it != user_arguments.end()) {
      ss << " Full argument as passed in by user:" << '\n' << it->second;
    }
    registerError(ss.str());
    LOG_AND_THROW(ss.str());
    return 0.0;
  }

  boost::optional<double> OSRunner::getOptionalDoubleArgumentValue(const std::string& argument_name,
                                                                   const std::map<std::string, OSArgument>& user_arguments) {
    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsDouble();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsDouble();
      }
    }

    return boost::none;
  }

  int OSRunner::getIntegerArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments) {
    std::stringstream ss;

    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsInteger();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsInteger();
      }
    }

    ss << "No value found for argument '" << argument_name << "'.";
    if (it != user_arguments.end()) {
      ss << " Full argument as passed in by user:" << '\n' << it->second;
    }
    registerError(ss.str());
    LOG_AND_THROW(ss.str());
    return 0;
  }

  boost::optional<int> OSRunner::getOptionalIntegerArgumentValue(const std::string& argument_name,
                                                                 const std::map<std::string, OSArgument>& user_arguments) {
    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsInteger();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsInteger();
      }
    }

    return boost::none;
  }

  std::string OSRunner::getStringArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments) {
    std::stringstream ss;

    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsString();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsString();
      }
    }

    ss << "No value found for argument '" << argument_name << "'.";
    if (it != user_arguments.end()) {
      ss << " Full argument as passed in by user:" << '\n' << it->second;
    }
    registerError(ss.str());
    LOG_AND_THROW(ss.str());
    return {};
  }

  boost::optional<std::string> OSRunner::getOptionalStringArgumentValue(const std::string& argument_name,
                                                                        const std::map<std::string, OSArgument>& user_arguments) {
    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsString();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsString();
      }
    }

    return boost::none;
  }

  openstudio::path OSRunner::getPathArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments) {
    std::stringstream ss;

    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsPath();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsPath();
      }
    }

    ss << "No value found for argument '" << argument_name << "'.";
    if (it != user_arguments.end()) {
      ss << " Full argument as passed in by user:" << '\n' << it->second;
    }
    registerError(ss.str());
    LOG_AND_THROW(ss.str());
    return {};
  }

  boost::optional<openstudio::path> OSRunner::getOptionalPathArgumentValue(const std::string& argument_name,
                                                                           const std::map<std::string, OSArgument>& user_arguments) {
    auto it = user_arguments.find(argument_name);
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        return it->second.valueAsPath();
      } else if (it->second.hasDefaultValue()) {
        return it->second.defaultValueAsPath();
      }
    }

    return boost::none;
  }

  boost::optional<openstudio::WorkspaceObject>
    OSRunner::getOptionalWorkspaceObjectChoiceValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments,
                                                    const openstudio::Workspace& workspace)

  {
    auto it = user_arguments.find(argument_name);
    std::string handleString;
    if (it != user_arguments.end()) {
      if (it->second.hasValue()) {
        handleString = it->second.valueAsString();
      } else if (it->second.hasDefaultValue()) {
        handleString = it->second.defaultValueAsString();
      }
    }

    Handle handle = toUUID(handleString);
    boost::optional<WorkspaceObject> result = workspace.getObject(handle);

    return result;
  }

  Json::Value OSRunner::getArgumentValues(std::vector<OSArgument>& script_arguments, const std::map<std::string, OSArgument>& user_arguments) {
    // This function aims to replace OsLib_HelperMethods.createRunVariables
    // TODO: should this call validateUserArguments like the ruby counterpart? At least it validates that the defaults are supposed to match etc
    if (!validateUserArguments(script_arguments, user_arguments)) {
      registerError("Invalid argument values.");
      LOG_AND_THROW("Invalid argument values.");
    }

    Json::Value argument_values;
    for (const OSArgument& script_argument : script_arguments) {
      const std::string name = script_argument.name();
      auto it = user_arguments.find(name);
      if (it != user_arguments.end()) {
        const auto& arg = it->second;
        if (arg.hasValue()) {
          argument_values[name] = arg.valueAsJSON();
        } else if (arg.hasDefaultValue()) {
          // TODO: is this wanted?
          // JJR: I believe it is.
          argument_values[name] = arg.defaultValueAsJSON();
        }
      }
    }
    return argument_values;
  }

  Json::Value OSRunner::getPastStepValuesForMeasure(const std::string& measureName) const {
    Json::Value step_values(Json::objectValue);
    for (const MeasureStep& step : subsetCastVector<MeasureStep>(m_workflow.workflowSteps())) {
      boost::optional<WorkflowStepResult> stepResult_ = step.result();
      if (!stepResult_ || !stepResult_->stepResult() || stepResult_->stepResult().get() != StepResult::Success) {
        continue;
      }
      // TODO: should we match on any of these three?
      if (istringEqual(measureName, step.measureDirName())) {  // The directory name, eg `IncreaseWallRValue`
        LOG(Trace, "Step matches on measureDirName");
      } else if (auto s_ = step.name(); s_.is_initialized() && istringEqual(measureName, *s_)) {  // An optional, abritrary one
        LOG(Trace, "Step matches on name");
      } else if (auto s_ = stepResult_->measureName();
                 s_.is_initialized()
                 && istringEqual(measureName, *s_)) {  // The xml one, eg `increase_insulation_r_value_for_exterior_walls_by_percentage`
        LOG(Trace, "Step matches on Step Result's measureName");
      } else {
        continue;
      }

      for (const WorkflowStepValue& step_value : stepResult_->stepValues()) {
        Json::Value root = step_value.toJSON();
        if (auto value = root["value"]) {
          step_values[step_value.name()] = value;
        }
      }
      break;
    }
    return step_values;
  }

  Json::Value OSRunner::getPastStepValuesForName(const std::string& stepName) const {
    // This function aims to replace OsLib_HelperMethods.check_upstream_measure_for_arg
    Json::Value step_values(Json::objectValue);
    for (const MeasureStep& step : subsetCastVector<MeasureStep>(m_workflow.workflowSteps())) {
      boost::optional<WorkflowStepResult> stepResult_ = step.result();
      if (!stepResult_ || !stepResult_->stepResult() || stepResult_->stepResult().get() != StepResult::Success) {
        continue;
      }

      std::string measure_name = step.measureDirName();  // The directory name, eg `IncreaseWallRValue`
      if (auto s_ = step.name()) {                       // An optional, abritrary one
        measure_name = std::move(*s_);
      }
      // if (auto s_ = stepResult_->measureName()) {  // The xml one, eg `increase_insulation_r_value_for_exterior_walls_by_percentage`
      //   measure_name = std::move(*s_);
      // }

      for (const WorkflowStepValue& stepValue : stepResult_->stepValues()) {
        if (istringEqual(stepName, stepValue.name())) {
          Json::Value root = stepValue.toJSON();
          if (auto value = root["value"]) {
            step_values[measure_name] = value;
            // TODO: check_upstream_measure_for_arg was breaking early, and returning a {:value => value, :measure_name => measure_name} format
          }
        }
      }
    }
    return step_values;
  }

  void OSRunner::setLastOpenStudioModel(const openstudio::model::Model& lastOpenStudioModel) {
    m_lastOpenStudioModel = lastOpenStudioModel;
    m_lastOpenStudioModelPath.reset();
  }

  void OSRunner::resetLastOpenStudioModel() {
    m_lastOpenStudioModel.reset();
    m_lastOpenStudioModelPath.reset();
  }

  void OSRunner::setLastOpenStudioModelPath(const openstudio::path& lastOpenStudioModelPath) {
    if (openstudio::filesystem::exists(lastOpenStudioModelPath)) {
      m_lastOpenStudioModelPath = openstudio::filesystem::canonical(lastOpenStudioModelPath);
    } else {
      m_lastOpenStudioModelPath = lastOpenStudioModelPath;
    }
    m_lastOpenStudioModel.reset();
  }

  void OSRunner::resetLastOpenStudioModelPath() {
    m_lastOpenStudioModel.reset();
    m_lastOpenStudioModelPath.reset();
  }

  void OSRunner::setLastEnergyPlusWorkspace(const openstudio::Workspace& lastEnergyPlusWorkspace) {
    m_lastEnergyPlusWorkspace = lastEnergyPlusWorkspace;
    m_lastEnergyPlusWorkspacePath.reset();
  }

  void OSRunner::resetLastEnergyPlusWorkspace() {
    m_lastEnergyPlusWorkspace.reset();
    m_lastEnergyPlusWorkspacePath.reset();
  }

  void OSRunner::setLastEnergyPlusWorkspacePath(const openstudio::path& lastEnergyPlusWorkspacePath) {
    if (openstudio::filesystem::exists(lastEnergyPlusWorkspacePath)) {
      m_lastEnergyPlusWorkspacePath = openstudio::filesystem::canonical(lastEnergyPlusWorkspacePath);
    } else {
      m_lastEnergyPlusWorkspacePath = lastEnergyPlusWorkspacePath;
    }
    m_lastEnergyPlusWorkspace.reset();
  }

  void OSRunner::resetLastEnergyPlusWorkspacePath() {
    m_lastEnergyPlusWorkspace.reset();
    m_lastEnergyPlusWorkspacePath.reset();
  }

  void OSRunner::setLastEnergyPlusSqlFilePath(const openstudio::path& lastEnergyPlusSqlFilePath) {
    if (openstudio::filesystem::exists(lastEnergyPlusSqlFilePath)) {
      m_lastEnergyPlusSqlFilePath = openstudio::filesystem::canonical(lastEnergyPlusSqlFilePath);
    } else {
      m_lastEnergyPlusSqlFilePath = lastEnergyPlusSqlFilePath;
    }
    m_lastEnergyPlusSqlFile.reset();
  }

  void OSRunner::resetLastEnergyPlusSqlFilePath() {
    m_lastEnergyPlusSqlFilePath.reset();
    m_lastEnergyPlusSqlFile.reset();
  }

  void OSRunner::setLastEpwFilePath(const openstudio::path& lastEpwFilePath) {
    if (openstudio::filesystem::exists(lastEpwFilePath)) {
      m_lastEpwFilePath = openstudio::filesystem::canonical(lastEpwFilePath);
    } else {
      m_lastEpwFilePath = lastEpwFilePath;
    }
    m_lastEpwFile.reset();
  }

  void OSRunner::resetLastEpwFilePath() {
    m_lastEpwFilePath.reset();
    m_lastEpwFile.reset();
  }

  bool OSRunner::setUnitsPreference(const std::string& unitsPreference) {
    m_unitsPreference = unitsPreference;
    return true;
  }

  void OSRunner::resetUnitsPreference() {
    m_unitsPreference = "IP";
  }

  bool OSRunner::setLanguagePreference(const std::string& languagePreference) {
    m_languagePreference = languagePreference;
    return true;
  }

  void OSRunner::resetLanguagePreference() {
    m_languagePreference = "en";
  }

  std::string OSRunner::cleanValueName(const std::string& name) {
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

  void OSRunner::captureStreams() {
    if (m_streamsCaptured) {
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

  void OSRunner::restoreStreams() {
    if (!m_streamsCaptured) {
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

}  // namespace measure
}  // namespace openstudio
