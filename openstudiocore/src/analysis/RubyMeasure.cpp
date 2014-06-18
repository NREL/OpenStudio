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

#include "RubyMeasure.hpp"
#include "RubyMeasure_Impl.hpp"

#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"

#include "../runmanager/lib/RubyJobUtils.hpp"
#include "../runmanager/lib/WorkItem.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../utilities/units/Quantity.hpp"

#include "../utilities/data/Attribute.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Json.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include <OpenStudio.hxx>

using namespace openstudio::ruleset;

namespace openstudio {
namespace analysis {

namespace detail {

  RubyMeasure_Impl::RubyMeasure_Impl(const BCLMeasure& bclMeasure, bool isSelected)
    : Measure_Impl(isSelected),
      m_bclMeasure(bclMeasure),
      m_bclMeasureDirectory(bclMeasure.directory()),
      m_bclMeasureUUID(bclMeasure.uuid()),
      m_bclMeasureVersionUUID(bclMeasure.versionUUID()),
      m_inputFileType(bclMeasure.inputFileType()),
      m_outputFileType(bclMeasure.outputFileType()),
      m_isUserScript(false)
  {
    OS_ASSERT((m_inputFileType == FileReferenceType::Unknown) || (m_inputFileType == FileReferenceType::OSM) || (m_inputFileType == FileReferenceType::IDF));
    OS_ASSERT((m_outputFileType == FileReferenceType::Unknown) || (m_outputFileType == FileReferenceType::OSM) || (m_outputFileType == FileReferenceType::IDF)  || (m_outputFileType == FileReferenceType::XML));
  }

  RubyMeasure_Impl::RubyMeasure_Impl(const openstudio::path& perturbationScript,
                                     const FileReferenceType& inputFileType,
                                     const FileReferenceType& outputFileType,
                                     bool isUserScript,
                                     bool isSelected)
    : Measure_Impl(isSelected),
      m_perturbationScript(FileReference(perturbationScript)),
      m_inputFileType(inputFileType),
      m_outputFileType(outputFileType),
      m_isUserScript(isUserScript)
  {
    OS_ASSERT(m_perturbationScript->fileType() == FileReferenceType::RB);
    OS_ASSERT((inputFileType == FileReferenceType::Unknown) ||
              (inputFileType == FileReferenceType::OSM) || 
              (inputFileType == FileReferenceType::IDF)); 
                                                                 
    OS_ASSERT((outputFileType == FileReferenceType::Unknown) ||
              (outputFileType == FileReferenceType::OSM) || 
              (outputFileType == FileReferenceType::IDF) ||
              (outputFileType == FileReferenceType::XML));
  }

  RubyMeasure_Impl::RubyMeasure_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      bool isSelected,
      const FileReference& perturbationScriptOrBCLMeasureDir,
      const FileReferenceType& inputFileType,
      const FileReferenceType& outputFileType,
      bool isUserScript,
      const std::vector<ruleset::OSArgument>& arguments,
      bool usesBCLMeasure)
    : Measure_Impl(uuid,versionUUID,name,displayName,description,isSelected),
      m_perturbationScript(perturbationScriptOrBCLMeasureDir),
      m_inputFileType(inputFileType),
      m_outputFileType(outputFileType),
      m_isUserScript(isUserScript),
      m_arguments(arguments)
  {
    if (usesBCLMeasure) {
      OS_ASSERT(m_perturbationScript->fileType() == FileReferenceType::Unknown);
      m_bclMeasureDirectory = m_perturbationScript->path();
      m_bclMeasureUUID = m_perturbationScript->uuid();
      m_bclMeasureVersionUUID = m_perturbationScript->versionUUID();
      m_perturbationScript.reset();
    }
  }

  RubyMeasure_Impl::RubyMeasure_Impl(const RubyMeasure_Impl &other)
    : Measure_Impl(other),
      m_bclMeasure(other.m_bclMeasure),
      m_bclMeasureDirectory(other.m_bclMeasureDirectory),
      m_bclMeasureUUID(other.m_bclMeasureUUID),
      m_bclMeasureVersionUUID(other.m_bclMeasureVersionUUID),
      m_inputFileType(other.m_inputFileType),
      m_outputFileType(other.m_outputFileType),
      m_isUserScript(other.isUserScript())
  {
    if (other.m_perturbationScript) {
      m_perturbationScript = other.m_perturbationScript->clone();
    }
    for (const ruleset::OSArgument& arg : other.arguments()) {
      m_arguments.push_back(arg.clone());
    }
  }

  AnalysisObject RubyMeasure_Impl::clone() const {
    std::shared_ptr<RubyMeasure_Impl> impl(new RubyMeasure_Impl(*this));
    return RubyMeasure(impl);
  }

  boost::optional<FileReferenceType> RubyMeasure_Impl::inputFileType() const {
    return m_inputFileType;
  }

  boost::optional<FileReferenceType> RubyMeasure_Impl::outputFileType() const {
    return m_outputFileType;
  }

  runmanager::WorkItem RubyMeasure_Impl::createWorkItem(
      const openstudio::path& rubyIncludeDirectory) const
  {
    runmanager::RubyJobBuilder rubyJobBuilder;

    bool done(false);
    if (usesBCLMeasure()) {
      if (OptionalBCLMeasure bclMeasure = this->bclMeasure()) {
        rubyJobBuilder = runmanager::RubyJobBuilder(*bclMeasure,arguments());
        done = true;
      }
    }

    if (!done) {
      if (m_isUserScript) {
        rubyJobBuilder = runmanager::RubyJobBuilder::createUserScriptRubyJob(
              perturbationScript().path(),
              arguments());
      }
      else if (usesBCLMeasure()) {
        // this is what happens if can't actually load the measure
        // basically delays failure
        // perhaps should just throw
        OS_ASSERT(m_bclMeasureDirectory);
        rubyJobBuilder = runmanager::RubyJobBuilder::createUserScriptRubyJob(
              *m_bclMeasureDirectory / toPath("measure.rb"),
              arguments());
      }
      else {
        rubyJobBuilder.setScriptFile(perturbationScript().path());
        for (const ruleset::OSArgument& argument : arguments()) {
          std::string argumentValue = argument.printValue(true);
          if (!argumentValue.empty()) {
            rubyJobBuilder.addScriptParameter(argument.name(),argumentValue);
          }
          else {
            rubyJobBuilder.addScriptParameter(argument.name());
          }
        }
      }

      // some default file names
      if (m_inputFileType == FileReferenceType::IDF) {
        rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
                                    openstudio::runmanager::FileSource("All"),
                                    ".*\\.idf",
                                    "in.idf");
      }
      else if (m_inputFileType == FileReferenceType::OSM) {
        rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
                                    openstudio::runmanager::FileSource("All"),
                                    ".*\\.osm",
                                    "in.osm");
      }

      // carry weather file through
      rubyJobBuilder.copyRequiredFiles(m_inputFileType.valueDescription(),
                                       m_outputFileType.valueDescription(),
                                       "in.epw");
    }

    // be able to access OpenStudio Ruby bindings
    rubyJobBuilder.setIncludeDir(rubyIncludeDirectory);

    return rubyJobBuilder.toWorkItem();
  }

  bool RubyMeasure_Impl::usesBCLMeasure() const {
    return m_bclMeasureDirectory;
  }

  boost::optional<BCLMeasure> RubyMeasure_Impl::bclMeasure() const {
    if (!usesBCLMeasure()) {
      // DLM: this was a bit harsh
      // ETH: no it isn't. usesBCLMeasure() determines whether this should be called or not.
      // then, if it can't be returned, you know the measure can't be loaded for some reason.
      // eventually, all other forms of RubyMeasure will be eliminated and the bool check
      // can go away.
      LOG_AND_THROW("This RubyMeasure is defined by script path, not by BCLMeasure.");
      return boost::none;
    }
    if (!m_bclMeasure) {
      m_bclMeasure = BCLMeasure::load(*m_bclMeasureDirectory);
      if (!m_bclMeasure) {
        LOG(Error, "Unable to load measure " << toString(*m_bclMeasureUUID) << " from "
            << toString(*m_bclMeasureDirectory) << ".");
      }
    }
    return m_bclMeasure;
  }

  openstudio::path RubyMeasure_Impl::bclMeasureDirectory() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyMeasure is defined by script path, not by BCLMeasure.");
    }
    return *m_bclMeasureDirectory;
  }

  UUID RubyMeasure_Impl::bclMeasureUUID() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyMeasure is defined by script path, not by BCLMeasure.");
    }
    return *m_bclMeasureUUID;
  }

  UUID RubyMeasure_Impl::bclMeasureVersionUUID() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyMeasure is defined by script path, not by BCLMeasure.");
    }
    return *m_bclMeasureVersionUUID;
  }

  FileReference RubyMeasure_Impl::perturbationScript() const {
    if (usesBCLMeasure()) {
      LOG_AND_THROW("This RubyMeasure is defined by BCLMeasure, not by script path.");
    }
    return m_perturbationScript.get();
  }

  std::vector<ruleset::OSArgument> RubyMeasure_Impl::arguments() const {
    return m_arguments;
  }

  bool RubyMeasure_Impl::isUserScript() const {
    return m_isUserScript;
  }

  bool RubyMeasure_Impl::hasIncompleteArguments() const {
    return !incompleteArguments().empty();
  }

  std::vector<ruleset::OSArgument> RubyMeasure_Impl::incompleteArguments() const {
    OSArgumentVector result;
    for (const OSArgument& arg : arguments()) {
      if (arg.required() && !(arg.hasValue() || arg.hasDefaultValue())) {
        result.push_back(arg);
      }
    }
    return result;
  }

  bool RubyMeasure_Impl::setMeasure(const BCLMeasure& bclMeasure) {
    FileReferenceType inputFileType = bclMeasure.inputFileType();
    FileReferenceType outputFileType = bclMeasure.outputFileType();
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }
    m_bclMeasure = bclMeasure;
    m_bclMeasureDirectory = bclMeasure.directory();
    m_bclMeasureUUID = bclMeasure.uuid();
    m_bclMeasureVersionUUID = bclMeasure.versionUUID();
    m_perturbationScript.reset();
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = false;
    clearArguments(); // emits onChange(AnalysisObject_Impl::InvalidatesResults)
    return true;
  }

  bool RubyMeasure_Impl::updateMeasure(const BCLMeasure& newVersion,
                                       std::vector<ruleset::OSArgument> newArguments)
  {
    if (!usesBCLMeasure() || (newVersion.uuid() != m_bclMeasureUUID.get())) {
      return false;
    }
    FileReferenceType inputFileType = newVersion.inputFileType();
    FileReferenceType outputFileType = newVersion.outputFileType();
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }

    m_bclMeasure = newVersion;
    m_bclMeasureDirectory = newVersion.directory();
    OS_ASSERT(m_bclMeasureUUID.get() == newVersion.uuid());
    m_bclMeasureVersionUUID = newVersion.versionUUID();
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = false;

    // preserve current argument values
    // otherwise, prefer newArguments definitions and order
    OSArgumentVector argsToSet;
    OSArgumentVector currentArgs = arguments();
    for (const OSArgument& newArg : newArguments) {
      OSArgument newArgClone = newArg.clone();
      // look for current value
      NameFinder<OSArgument> finder(newArgClone.name(),true);
      auto it = std::find_if(currentArgs.begin(),
                             currentArgs.end(),
                             finder);
      if (it != currentArgs.end()) {
        // already exists--try to preserve value
        if (it->hasValue()) {
          bool setResult(false);
          switch(it->type().value()) {
            case OSArgumentType::Boolean :
              setResult = newArgClone.setValue(it->valueAsBool());
              break;
            case OSArgumentType::Double :
              setResult = newArgClone.setValue(it->valueAsDouble());
              break;
            case OSArgumentType::Quantity :
              setResult = newArgClone.setValue(it->valueAsQuantity());
              break;
            case OSArgumentType::Integer :
              setResult = newArgClone.setValue(it->valueAsInteger());
              break;
            default :
              break;
          }
          if (!setResult) {
            // try string conversion by default or if failed
            setResult = newArgClone.setValue(it->valueAsString());
            if (!setResult) {
              LOG(Info,"Unable to preserve argument value for " << *it
                  << " during updateMeasure operation on RubyMeasure named " << name()
                  << " with measure " << newVersion.name() << " and new argument " << newArgClone);
            }
          }
        }
      }
      argsToSet.push_back(newArgClone);
    }
    setArguments(argsToSet);
    return true;
  }

  bool RubyMeasure_Impl::setPerturbationScript(const openstudio::path& script,
                                          const FileReferenceType& inputFileType,
                                          const FileReferenceType& outputFileType,
                                          bool isUserScript)
  {
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }
    m_bclMeasure.reset();
    m_bclMeasureDirectory.reset();
    m_bclMeasureUUID.reset();
    m_bclMeasureVersionUUID.reset();
    m_perturbationScript = FileReference(script);
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = isUserScript;
    clearArguments(); // emits onChange(AnalysisObject_Impl::InvalidatesResults)
    return true;
  }

  void RubyMeasure_Impl::setIsUserScript(bool isUserScript) {
    m_isUserScript = isUserScript;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyMeasure_Impl::addArgument(const std::string& name) {
    m_arguments.push_back(ruleset::OSArgument::makeBoolArgument(name));
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyMeasure_Impl::addArgument(const std::string& name, const std::string& value) {
    m_arguments.push_back(ruleset::OSArgument::makeStringArgument(name));
    m_arguments.back().setValue(value);
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyMeasure_Impl::addArgument(const ruleset::OSArgument &argument) {
    m_arguments.push_back(argument);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    if (argument.hasValue() || argument.hasDefaultValue()) {
      return true;
    }
    LOG(Warn,"Saving argument " << argument.name() << " with RubyMeasure, but it "
        << "currently does not have a value.");
    return false;
  }

  void RubyMeasure_Impl::setArgument(const ruleset::OSArgument& argument) {
    NameFinder<ruleset::OSArgument> finder(argument.name(),true);
    auto it = std::find_if(m_arguments.begin(),
                           m_arguments.end(),
                           finder);
    if (it != m_arguments.end()) {
      m_arguments.at(int(it - m_arguments.begin())) = argument;
    }
    else {
      m_arguments.push_back(argument.clone());
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyMeasure_Impl::setArguments(const std::vector<ruleset::OSArgument>& arguments) {
    m_arguments.clear();
    for (const ruleset::OSArgument& arg : arguments) {
      m_arguments.push_back(arg.clone());
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyMeasure_Impl::removeArgument(const std::string& argumentName) {
    NameFinder<ruleset::OSArgument> finder(argumentName,true);
    auto it = std::find_if(m_arguments.begin(),
                           m_arguments.end(),
                           finder);
    if (it != m_arguments.end()) {
      m_arguments.erase(it);
      onChange(AnalysisObject_Impl::InvalidatesResults);
      return true;
    }
    return false;
  }

  void RubyMeasure_Impl::clearArguments() {
    m_arguments.clear();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  QVariant RubyMeasure_Impl::toVariant() const {
    QVariantMap rubyMeasureData = Measure_Impl::toVariant().toMap();

    rubyMeasureData["measure_type"] = QString("RubyMeasure");
    if (m_bclMeasureDirectory) {
      rubyMeasureData["bcl_measure_directory"] = toQString(m_bclMeasureDirectory.get());
      rubyMeasureData["bcl_measure_uuid"] = toQString(removeBraces(m_bclMeasureUUID.get()));
      rubyMeasureData["bcl_measure_version_uuid"] = toQString(removeBraces(m_bclMeasureVersionUUID.get()));
    }
    else {
      OS_ASSERT(m_perturbationScript);
      rubyMeasureData["perturbation_script"] = openstudio::detail::toVariant(m_perturbationScript.get());
    }
    rubyMeasureData["input_file_type"] = toQString(m_inputFileType.valueName());
    rubyMeasureData["output_file_type"] = toQString(m_outputFileType.valueName());
    rubyMeasureData["is_user_script"] = m_isUserScript;
    if (!arguments().empty()) {
      QVariantList argumentsList;
      int index(0);
      for (const OSArgument& arg : arguments()) {
        QVariantMap argMap = ruleset::detail::toVariant(arg).toMap();
        argMap["argument_index"] = index;
        argumentsList.push_back(QVariant(argMap));
        ++index;
      }
      rubyMeasureData["arguments"] = QVariant(argumentsList);
    }

    return QVariant(rubyMeasureData);
  }

  RubyMeasure RubyMeasure_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    OptionalFileReference perturbationScriptOrBCLMeasureDir;
    if (map.contains("bcl_measure_directory")) {
      perturbationScriptOrBCLMeasureDir = FileReference(
            toUUID(map["bcl_measure_uuid"].toString().toStdString()),
            toUUID(map["bcl_measure_version_uuid"].toString().toStdString()),
            "",
            "",
            "",
            toPath(map["bcl_measure_directory"].toString()),
            FileReferenceType::Unknown,
            DateTime::now(),
            DateTime::now(),
            "",
            "");
    }
    else {
      perturbationScriptOrBCLMeasureDir = openstudio::detail::toFileReference(map["perturbation_script"],version);
    }

    OSArgumentVector arguments;
    if (map.contains("arguments")) {
      arguments = deserializeOrderedVector(
            map["arguments"].toList(),
            "argument_index",
            std::function<OSArgument (const QVariant&)>(std::bind(ruleset::detail::toOSArgument,std::placeholders::_1,version)));
    }

    return RubyMeasure(toUUID(map["uuid"].toString().toStdString()),
                       toUUID(map["version_uuid"].toString().toStdString()),
                       map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                       map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                       map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                       map["is_selected"].toBool(),
                       perturbationScriptOrBCLMeasureDir.get(),
                       FileReferenceType(map["input_file_type"].toString().toStdString()),
                       FileReferenceType(map["output_file_type"].toString().toStdString()),
                       map["is_user_script"].toBool(),
                       arguments,
                       map.contains("bcl_measure_directory"));
  }

  void RubyMeasure_Impl::updateInputPathData(const openstudio::path& originalBase,
                                             const openstudio::path& newBase)
  {
    openstudio::path temp;

    // BCL Measure Directory
    if (m_bclMeasureDirectory) {
      temp = relocatePath(*m_bclMeasureDirectory,originalBase,newBase);
      if (!temp.empty()) {
        m_bclMeasureDirectory = temp;
      }
    }

    // Perturbation Script
    if (m_perturbationScript) {
      temp = relocatePath(m_perturbationScript->path(),originalBase,newBase);
      if (!temp.empty()) {
        m_perturbationScript->setPath(temp);
      }
    }

    // Path Argument Values
    for (OSArgument& arg : m_arguments) {
      if ((arg.type() == OSArgumentType::Path) && arg.hasValue()) {
        temp = relocatePath(arg.valueAsPath(),originalBase,newBase);
        if (!temp.empty()) {
          arg.setValue(temp);
        }
      }
      // default value for path argument should not be absolute, so skip
    }
  }

  bool RubyMeasure_Impl::fileTypesAreCompatible(
      const FileReferenceType& proposedInputFileType,
      const FileReferenceType& proposedOutputFileType) const
  {
    if (OptionalAnalysisObject parent = this->parent()) {
      // Make sure inputFileType and outputFileType are okay
      if (OptionalMeasureGroup mg = parent->optionalCast<MeasureGroup>()) {
        Measure copyOfThis = getPublicObject<Measure>();
        if (!mg->fileTypesAreCompatible(copyOfThis,proposedInputFileType,proposedOutputFileType)) {
          return false;
        }
      }
      if (OptionalRubyContinuousVariable cvar = parent->optionalCast<RubyContinuousVariable>()) {
        RubyMeasure copyOfThis = getPublicObject<RubyMeasure>();
        if (!cvar->fileTypesAreCompatible(copyOfThis,proposedInputFileType,proposedOutputFileType)) {
          return false;
        }
      }
    }
    return true;
  }

} // detail

RubyMeasure::RubyMeasure(const BCLMeasure& bclMeasure, bool isSelected)
  : Measure(std::shared_ptr<detail::RubyMeasure_Impl>(
                           new detail::RubyMeasure_Impl(bclMeasure,isSelected)))
{}

RubyMeasure::RubyMeasure(const openstudio::path& perturbationScript,
                                   const FileReferenceType& inputFileType,
                                   const FileReferenceType& outputFileType,
                                   bool isUserScript,
                                   bool isSelected)
  : Measure(std::shared_ptr<detail::RubyMeasure_Impl>(
        new detail::RubyMeasure_Impl(perturbationScript,
                                     inputFileType,
                                     outputFileType,
                                     isUserScript,
                                     isSelected)))
{}

RubyMeasure::RubyMeasure(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& name,
                                   const std::string& displayName,
                                   const std::string& description,
                                   bool isSelected,
                                   const FileReference& perturbationScript,
                                   const FileReferenceType& inputFileType,
                                   const FileReferenceType& outputFileType,
                                   bool isUserScript,
                                   const std::vector<ruleset::OSArgument>& arguments,
                                   bool usesBCLMeasure)
  : Measure(std::shared_ptr<detail::RubyMeasure_Impl>(
        new detail::RubyMeasure_Impl(uuid,
                                     versionUUID,
                                     name,
                                     displayName,
                                     description,
                                     isSelected,
                                     perturbationScript,
                                     inputFileType,
                                     outputFileType,
                                     isUserScript,
                                     arguments,
                                     usesBCLMeasure)))
{}

bool RubyMeasure::usesBCLMeasure() const {
  return getImpl<detail::RubyMeasure_Impl>()->usesBCLMeasure();
}

boost::optional<BCLMeasure> RubyMeasure::bclMeasure() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasure();
}

boost::optional<BCLMeasure> RubyMeasure::measure() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasure();
}

openstudio::path RubyMeasure::bclMeasureDirectory() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureDirectory();
}

openstudio::path RubyMeasure::measureDirectory() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureDirectory();
}

UUID RubyMeasure::bclMeasureUUID() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureUUID();
}

UUID RubyMeasure::measureUUID() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureUUID();
}

UUID RubyMeasure::bclMeasureVersionUUID() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureVersionUUID();
}

UUID RubyMeasure::measureVersionUUID() const {
  return getImpl<detail::RubyMeasure_Impl>()->bclMeasureVersionUUID();
}

FileReference RubyMeasure::perturbationScript() const {
  return getImpl<detail::RubyMeasure_Impl>()->perturbationScript();
}

std::vector<ruleset::OSArgument> RubyMeasure::arguments() const {
  return getImpl<detail::RubyMeasure_Impl>()->arguments();
}

bool RubyMeasure::isUserScript() const {
  return getImpl<detail::RubyMeasure_Impl>()->isUserScript();
}

bool RubyMeasure::hasIncompleteArguments() const {
  return getImpl<detail::RubyMeasure_Impl>()->hasIncompleteArguments();
}

std::vector<ruleset::OSArgument> RubyMeasure::incompleteArguments() const {
  return getImpl<detail::RubyMeasure_Impl>()->incompleteArguments();
}

bool RubyMeasure::setMeasure(const BCLMeasure& measure) {
  return getImpl<detail::RubyMeasure_Impl>()->setMeasure(measure);
}

bool RubyMeasure::updateMeasure(const BCLMeasure& newVersion,
                                     std::vector<ruleset::OSArgument> newArguments)
{
  return getImpl<detail::RubyMeasure_Impl>()->updateMeasure(newVersion,
                                                                 newArguments);
}

bool RubyMeasure::setPerturbationScript(const openstudio::path& script,
                                        const FileReferenceType& inputFileType,
                                        const FileReferenceType& outputFileType,
                                        bool isUserScript)
{
  return getImpl<detail::RubyMeasure_Impl>()->setPerturbationScript(script,
                                                                    inputFileType,
                                                                    outputFileType,
                                                                    isUserScript);
}

void RubyMeasure::setIsUserScript(bool isUserScript) {
  getImpl<detail::RubyMeasure_Impl>()->setIsUserScript(isUserScript);
}

void RubyMeasure::addArgument(const std::string& name) {
  return getImpl<detail::RubyMeasure_Impl>()->addArgument(name);
}

void RubyMeasure::addArgument(const std::string& name, const std::string& value) {
  return getImpl<detail::RubyMeasure_Impl>()->addArgument(name,value);
}

bool RubyMeasure::addArgument(const ruleset::OSArgument &argument) {
  return getImpl<detail::RubyMeasure_Impl>()->addArgument(argument);
}

void RubyMeasure::setArgument(const ruleset::OSArgument& argument) {
  getImpl<detail::RubyMeasure_Impl>()->setArgument(argument);
}

void RubyMeasure::setArguments(const std::vector<ruleset::OSArgument>& arguments) {
  getImpl<detail::RubyMeasure_Impl>()->setArguments(arguments);
}

bool RubyMeasure::removeArgument(const std::string& argumentName) {
  return getImpl<detail::RubyMeasure_Impl>()->removeArgument(argumentName);
}

void RubyMeasure::clearArguments() {
  getImpl<detail::RubyMeasure_Impl>()->clearArguments();
}

/// @cond
RubyMeasure::RubyMeasure(std::shared_ptr<detail::RubyMeasure_Impl> impl)
  : Measure(impl)
{}
/// @endcond

} // analysis
} // openstudio

