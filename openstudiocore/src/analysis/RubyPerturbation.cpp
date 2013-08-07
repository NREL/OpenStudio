/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>

#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable_Impl.hpp>

#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/units/Quantity.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/core/FileReference.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Finder.hpp>

#include <boost/foreach.hpp>

#include <OpenStudio.hxx>

using namespace openstudio::ruleset;

namespace openstudio {
namespace analysis {

namespace detail {

  RubyPerturbation_Impl::RubyPerturbation_Impl(const BCLMeasure& measure, bool isSelected)
    : DiscretePerturbation_Impl(isSelected),
      m_measure(measure),
      m_measureDirectory(measure.directory()),
      m_measureUUID(measure.uuid()),
      m_measureVersionUUID(measure.versionUUID()),
      m_inputFileType(measure.inputFileType()),
      m_outputFileType(measure.outputFileType()),
      m_isUserScript(false)
  {
    OS_ASSERT((m_inputFileType == FileReferenceType::OSM) || (m_inputFileType == FileReferenceType::IDF));
    OS_ASSERT((m_outputFileType == FileReferenceType::OSM) || (m_outputFileType == FileReferenceType::IDF));
  }

  RubyPerturbation_Impl::RubyPerturbation_Impl(const openstudio::path& perturbationScript,
                                               const FileReferenceType& inputFileType,
                                               const FileReferenceType& outputFileType,
                                               bool isUserScript,
                                               bool isSelected)
    : DiscretePerturbation_Impl(isSelected),
      m_perturbationScript(FileReference(perturbationScript)),
      m_inputFileType(inputFileType),
      m_outputFileType(outputFileType),
      m_isUserScript(isUserScript)
  {
    OS_ASSERT(m_perturbationScript->fileType() == FileReferenceType::RB);
    OS_ASSERT((inputFileType == FileReferenceType::OSM) || (inputFileType == FileReferenceType::IDF));
    OS_ASSERT((outputFileType == FileReferenceType::OSM) || (outputFileType == FileReferenceType::IDF));
  }

  RubyPerturbation_Impl::RubyPerturbation_Impl(
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
    : DiscretePerturbation_Impl(uuid,versionUUID,name,displayName,description,isSelected),
      m_perturbationScript(perturbationScriptOrBCLMeasureDir),
      m_inputFileType(inputFileType),
      m_outputFileType(outputFileType),
      m_isUserScript(isUserScript),
      m_arguments(arguments)
  {
    if (usesBCLMeasure) {
      OS_ASSERT(m_perturbationScript->fileType() == FileReferenceType::Unknown);
      m_measureDirectory = m_perturbationScript->path();
      m_measureUUID = m_perturbationScript->uuid();
      m_measureVersionUUID = m_perturbationScript->versionUUID();
      m_perturbationScript.reset();
    }
  }

  RubyPerturbation_Impl::RubyPerturbation_Impl(const RubyPerturbation_Impl &other)
    : DiscretePerturbation_Impl(other),
      m_measure(other.m_measure),
      m_measureDirectory(other.m_measureDirectory),
      m_measureUUID(other.m_measureUUID),
      m_measureVersionUUID(other.m_measureVersionUUID),
      m_inputFileType(other.m_inputFileType),
      m_outputFileType(other.m_outputFileType),
      m_isUserScript(other.isUserScript())
  {
    if (other.m_perturbationScript) {
      m_perturbationScript = other.m_perturbationScript->clone();
    }
    BOOST_FOREACH(const ruleset::OSArgument& arg,other.arguments()) {
      m_arguments.push_back(arg.clone());
    }
  }

  AnalysisObject RubyPerturbation_Impl::clone() const {
    boost::shared_ptr<RubyPerturbation_Impl> impl(new RubyPerturbation_Impl(*this));
    return RubyPerturbation(impl);
  }

  boost::optional<FileReferenceType> RubyPerturbation_Impl::inputFileType() const {
    return m_inputFileType;
  }

  boost::optional<FileReferenceType> RubyPerturbation_Impl::outputFileType() const {
    return m_outputFileType;
  }

  runmanager::WorkItem RubyPerturbation_Impl::createWorkItem(
      const openstudio::path& rubyIncludeDirectory) const
  {
    runmanager::RubyJobBuilder rubyJobBuilder;

    bool done(false);
    if (usesBCLMeasure()) {
      if (OptionalBCLMeasure measure = this->measure()) {
        rubyJobBuilder = runmanager::RubyJobBuilder(*measure,arguments());
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
        OS_ASSERT(m_measureDirectory);
        rubyJobBuilder = runmanager::RubyJobBuilder::createUserScriptRubyJob(
              *m_measureDirectory / toPath("measure.rb"),
              arguments());
      }
      else {
        rubyJobBuilder.setScriptFile(perturbationScript().path());
        BOOST_FOREACH(const ruleset::OSArgument& argument,arguments()) {
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

  bool RubyPerturbation_Impl::usesBCLMeasure() const {
    return m_measureDirectory;
  }

  boost::optional<BCLMeasure> RubyPerturbation_Impl::measure() const {
    if (!usesBCLMeasure()) {
      // DLM: this was a bit harsh
      // ETH: no it isn't. usesBCLMeasure() determines whether this should be called or not.
      // then, if it can't be returned, you know the measure can't be loaded for some reason.
      // eventually, all other forms of RubyPerturbation will be eliminated and the bool check
      // can go away.
      LOG_AND_THROW("This RubyPerturbation is defined by script path, not by BCLMeasure.");
      return boost::none;
    }
    if (!m_measure) {
      m_measure = BCLMeasure::load(*m_measureDirectory);
      if (!m_measure) {
        LOG(Error, "Unable to load measure " << toString(*m_measureUUID) << " from "
            << toString(*m_measureDirectory) << ".");
      }
    }
    return m_measure;
  }

  openstudio::path RubyPerturbation_Impl::measureDirectory() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyPerturbation is defined by script path, not by BCLMeasure.");
    }
    return *m_measureDirectory;
  }

  UUID RubyPerturbation_Impl::measureUUID() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyPerturbation is defined by script path, not by BCLMeasure.");
    }
    return *m_measureUUID;
  }

  UUID RubyPerturbation_Impl::measureVersionUUID() const {
    if (!usesBCLMeasure()) {
      LOG_AND_THROW("This RubyPerturbation is defined by script path, not by BCLMeasure.");
    }
    return *m_measureVersionUUID;
  }

  FileReference RubyPerturbation_Impl::perturbationScript() const {
    if (usesBCLMeasure()) {
      LOG_AND_THROW("This RubyPerturbation is defined by BCLMeasure, not by script path.");
    }
    return m_perturbationScript.get();
  }

  std::vector<ruleset::OSArgument> RubyPerturbation_Impl::arguments() const {
    return m_arguments;
  }

  bool RubyPerturbation_Impl::isUserScript() const {
    return m_isUserScript;
  }

  bool RubyPerturbation_Impl::hasIncompleteArguments() const {
    return !incompleteArguments().empty();
  }

  std::vector<ruleset::OSArgument> RubyPerturbation_Impl::incompleteArguments() const {
    OSArgumentVector result;
    BOOST_FOREACH(const OSArgument& arg,arguments()) {
      if (arg.required() && !(arg.hasValue() || arg.hasDefaultValue())) {
        result.push_back(arg);
      }
    }
    return result;
  }

  bool RubyPerturbation_Impl::setMeasure(const BCLMeasure& measure) {
    FileReferenceType inputFileType = measure.inputFileType();
    FileReferenceType outputFileType = measure.outputFileType();
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }
    m_measure = measure;
    m_measureDirectory = measure.directory();
    m_measureUUID = measure.uuid();
    m_measureVersionUUID = measure.versionUUID();
    m_perturbationScript.reset();
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = false;
    clearArguments(); // emits onChange(AnalysisObject_Impl::InvalidatesResults)
    return true;
  }

  bool RubyPerturbation_Impl::updateMeasure(const BCLMeasure& newVersion,
                                            std::vector<ruleset::OSArgument> newArguments)
  {
    if (!usesBCLMeasure() || (newVersion.uuid() != m_measureUUID.get())) {
      return false;
    }
    FileReferenceType inputFileType = newVersion.inputFileType();
    FileReferenceType outputFileType = newVersion.outputFileType();
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }

    m_measure = newVersion;
    m_measureDirectory = newVersion.directory();
    OS_ASSERT(m_measureUUID.get() == newVersion.uuid());
    m_measureVersionUUID = newVersion.versionUUID();
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = false;

    // preserve current argument values
    // otherwise, prefer newArguments definitions and order
    OSArgumentVector argsToSet;
    OSArgumentVector currentArgs = arguments();
    BOOST_FOREACH(const OSArgument& newArg,newArguments) {
      OSArgument newArgClone = newArg.clone();
      // look for current value
      NameFinder<OSArgument> finder(newArgClone.name(),true);
      ruleset::OSArgumentVector::iterator it = std::find_if(currentArgs.begin(),
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
                  << " during updateMeasure operation on RubyPerturbation named " << name()
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

  bool RubyPerturbation_Impl::setPerturbationScript(const openstudio::path& script,
                                                    const FileReferenceType& inputFileType,
                                                    const FileReferenceType& outputFileType,
                                                    bool isUserScript)
  {
    if (!fileTypesAreCompatible(inputFileType,outputFileType)) {
      return false;
    }
    m_measure.reset();
    m_measureDirectory.reset();
    m_measureUUID.reset();
    m_measureVersionUUID.reset();
    m_perturbationScript = FileReference(script);
    m_inputFileType = inputFileType;
    m_outputFileType = outputFileType;
    m_isUserScript = isUserScript;
    clearArguments(); // emits onChange(AnalysisObject_Impl::InvalidatesResults)
    return true;
  }

  void RubyPerturbation_Impl::setIsUserScript(bool isUserScript) {
    m_isUserScript = isUserScript;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyPerturbation_Impl::addArgument(const std::string& name) {
    m_arguments.push_back(ruleset::OSArgument::makeBoolArgument(name));
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyPerturbation_Impl::addArgument(const std::string& name, const std::string& value) {
    m_arguments.push_back(ruleset::OSArgument::makeStringArgument(name));
    m_arguments.back().setValue(value);
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyPerturbation_Impl::addArgument(const ruleset::OSArgument &argument) {
    m_arguments.push_back(argument);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    if (argument.hasValue() || argument.hasDefaultValue()) {
      return true;
    }
    LOG(Warn,"Saving argument " << argument.name() << " with RubyPerturbation, but it "
        << "currently does not have a value.");
    return false;
  }

  void RubyPerturbation_Impl::setArgument(const ruleset::OSArgument& argument) {
    NameFinder<ruleset::OSArgument> finder(argument.name(),true);
    ruleset::OSArgumentVector::iterator it = std::find_if(m_arguments.begin(),
                                                          m_arguments.end(),
                                                          finder);
    if (it != m_arguments.end()) {
      m_arguments.at(int(it - m_arguments.begin())) = argument;
    }
    else {
      m_arguments.push_back(argument);
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void RubyPerturbation_Impl::setArguments(const std::vector<ruleset::OSArgument>& arguments) {
    m_arguments = arguments;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyPerturbation_Impl::removeArgument(const std::string& argumentName) {
    NameFinder<ruleset::OSArgument> finder(argumentName,true);
    ruleset::OSArgumentVector::iterator it = std::find_if(m_arguments.begin(),
                                                          m_arguments.end(),
                                                          finder);
    if (it != m_arguments.end()) {
      m_arguments.erase(it);
      onChange(AnalysisObject_Impl::InvalidatesResults);
      return true;
    }
    return false;
  }

  void RubyPerturbation_Impl::clearArguments() {
    m_arguments.clear();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyPerturbation_Impl::fileTypesAreCompatible(
      const FileReferenceType& proposedInputFileType,
      const FileReferenceType& proposedOutputFileType) const
  {
    if (OptionalAnalysisObject parent = this->parent()) {
      // Make sure inputFileType and outputFileType are okay
      if (OptionalDiscreteVariable dvar = parent->optionalCast<DiscreteVariable>()) {
        DiscretePerturbation copyOfThis = getPublicObject<DiscretePerturbation>();
        if (!dvar->fileTypesAreCompatible(copyOfThis,proposedInputFileType,proposedOutputFileType)) {
          return false;
        }
      }
      if (OptionalRubyContinuousVariable cvar = parent->optionalCast<RubyContinuousVariable>()) {
        RubyPerturbation copyOfThis = getPublicObject<RubyPerturbation>();
        if (!cvar->fileTypesAreCompatible(copyOfThis,proposedInputFileType,proposedOutputFileType)) {
          return false;
        }
      }
    }
    return true;
  }

} // detail

RubyPerturbation::RubyPerturbation(const BCLMeasure& measure, bool isSelected)
  : DiscretePerturbation(boost::shared_ptr<detail::RubyPerturbation_Impl>(
                           new detail::RubyPerturbation_Impl(measure,isSelected)))
{}

RubyPerturbation::RubyPerturbation(const openstudio::path& perturbationScript,
                                   const FileReferenceType& inputFileType,
                                   const FileReferenceType& outputFileType,
                                   bool isUserScript,
                                   bool isSelected)
  : DiscretePerturbation(boost::shared_ptr<detail::RubyPerturbation_Impl>(
        new detail::RubyPerturbation_Impl(perturbationScript,
                                          inputFileType,
                                          outputFileType,
                                          isUserScript,
                                          isSelected)))
{}

RubyPerturbation::RubyPerturbation(const UUID& uuid,
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
  : DiscretePerturbation(boost::shared_ptr<detail::RubyPerturbation_Impl>(
        new detail::RubyPerturbation_Impl(uuid,
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

bool RubyPerturbation::usesBCLMeasure() const {
  return getImpl<detail::RubyPerturbation_Impl>()->usesBCLMeasure();
}

boost::optional<BCLMeasure> RubyPerturbation::measure() const {
  return getImpl<detail::RubyPerturbation_Impl>()->measure();
}

openstudio::path RubyPerturbation::measureDirectory() const {
  return getImpl<detail::RubyPerturbation_Impl>()->measureDirectory();
}

UUID RubyPerturbation::measureUUID() const {
  return getImpl<detail::RubyPerturbation_Impl>()->measureUUID();
}

UUID RubyPerturbation::measureVersionUUID() const {
  return getImpl<detail::RubyPerturbation_Impl>()->measureVersionUUID();
}

FileReference RubyPerturbation::perturbationScript() const {
  return getImpl<detail::RubyPerturbation_Impl>()->perturbationScript();
}

std::vector<ruleset::OSArgument> RubyPerturbation::arguments() const {
  return getImpl<detail::RubyPerturbation_Impl>()->arguments();
}

bool RubyPerturbation::isUserScript() const {
  return getImpl<detail::RubyPerturbation_Impl>()->isUserScript();
}

bool RubyPerturbation::hasIncompleteArguments() const {
  return getImpl<detail::RubyPerturbation_Impl>()->hasIncompleteArguments();
}

std::vector<ruleset::OSArgument> RubyPerturbation::incompleteArguments() const {
  return getImpl<detail::RubyPerturbation_Impl>()->incompleteArguments();
}

bool RubyPerturbation::setMeasure(const BCLMeasure& measure) {
  return getImpl<detail::RubyPerturbation_Impl>()->setMeasure(measure);
}

bool RubyPerturbation::updateMeasure(const BCLMeasure& newVersion,
                                     std::vector<ruleset::OSArgument> newArguments)
{
  return getImpl<detail::RubyPerturbation_Impl>()->updateMeasure(newVersion,
                                                                 newArguments);
}

bool RubyPerturbation::setPerturbationScript(const openstudio::path& script,
                                             const FileReferenceType& inputFileType,
                                             const FileReferenceType& outputFileType,
                                             bool isUserScript)
{
  return getImpl<detail::RubyPerturbation_Impl>()->setPerturbationScript(script,
                                                                         inputFileType,
                                                                         outputFileType,
                                                                         isUserScript);
}

void RubyPerturbation::setIsUserScript(bool isUserScript) {
  getImpl<detail::RubyPerturbation_Impl>()->setIsUserScript(isUserScript);
}

void RubyPerturbation::addArgument(const std::string& name) {
  return getImpl<detail::RubyPerturbation_Impl>()->addArgument(name);
}

void RubyPerturbation::addArgument(const std::string& name, const std::string& value) {
  return getImpl<detail::RubyPerturbation_Impl>()->addArgument(name,value);
}

bool RubyPerturbation::addArgument(const ruleset::OSArgument &argument) {
  return getImpl<detail::RubyPerturbation_Impl>()->addArgument(argument);
}

void RubyPerturbation::setArgument(const ruleset::OSArgument& argument) {
  getImpl<detail::RubyPerturbation_Impl>()->setArgument(argument);
}

void RubyPerturbation::setArguments(const std::vector<ruleset::OSArgument>& arguments) {
  getImpl<detail::RubyPerturbation_Impl>()->setArguments(arguments);
}

bool RubyPerturbation::removeArgument(const std::string& argumentName) {
  return getImpl<detail::RubyPerturbation_Impl>()->removeArgument(argumentName);
}

void RubyPerturbation::clearArguments() {
  getImpl<detail::RubyPerturbation_Impl>()->clearArguments();
}

/// @cond
RubyPerturbation::RubyPerturbation(boost::shared_ptr<detail::RubyPerturbation_Impl> impl)
  : DiscretePerturbation(impl)
{}
/// @endcond

} // analysis
} // openstudio

