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

#include "RubyUserScriptInfoGetter.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include <sstream>

namespace openstudio {
namespace ruleset {

  RubyUserScriptInfo::RubyUserScriptInfo(const std::string& error)
    : m_error(error)
  {}

  RubyUserScriptInfo::RubyUserScriptInfo(const MeasureType& measureType,
                                         const std::string& className,
                                         const std::string& name,
                                         const std::string& description,
                                         const std::string& modelerDescription,
                                         const std::vector<OSArgument>& arguments)
    : m_measureType(measureType), m_className(className), m_name(name), m_description(description), m_modelerDescription(modelerDescription), m_arguments(arguments)
  {}

  boost::optional<std::string> RubyUserScriptInfo::error() const
  {
    return m_error;
  }

  MeasureType RubyUserScriptInfo::measureType() const
  {
    return m_measureType;
  }

  std::string RubyUserScriptInfo::className() const
  {
    return m_className;
  }

  std::string RubyUserScriptInfo::name() const
  {
    return m_name;
  }

  std::string RubyUserScriptInfo::description() const
  {
    return m_description;
  }

  std::string RubyUserScriptInfo::modelerDescription() const
  {
    return m_modelerDescription;
  }

  std::vector<OSArgument> RubyUserScriptInfo::arguments() const
  {
    return m_arguments;
  }

  bool RubyUserScriptInfo::update(BCLMeasure& measure) const
  {
    bool result = false;

    if (m_error){
      // have error
      if (!measure.error() || measure.error().get() != *m_error){
        result = true;
        measure.setError(*m_error);
      }
      // return here so we don't blow away name or other information from last successful run
      return result;
    } else {
      // no error
      if (measure.error()){
        result = true;
        measure.resetError();
      }
    }

    // map snake cased class name to bcl measure name
    std::string lowerClassName = toUnderscoreCase(m_className);
    if (!lowerClassName.empty() && measure.name() != lowerClassName){
      result = true;
      measure.setName(lowerClassName);
    }

    // map name from measure to bcl measure display name
    if (!m_name.empty() && measure.displayName() != m_name){
      result = true;
      measure.setDisplayName(m_name);
    }

    if (!m_className.empty() && measure.className() != m_className){
      result = true;
      measure.setClassName(m_className);
    }

    if (!m_description.empty() && measure.description() != m_description){
      result = true;
      measure.setDescription(m_description);
    }

    if (!m_modelerDescription.empty() && measure.modelerDescription() != m_modelerDescription){
      result = true;
      measure.setModelerDescription(m_modelerDescription);
    }

    unsigned n = m_arguments.size();
    std::vector<BCLMeasureArgument> bclArguments;
    bclArguments.reserve(n);
    for (const OSArgument& argument : m_arguments){

      std::string bclMeasureType = argument.type().valueName();
      if (argument.type() == OSArgumentType::Quantity){
        LOG(Warn, "Mapping deprecated OSArgumentType::Quantity to Double");
        bclMeasureType = "Double";
      }

      boost::optional<std::string> defaultValue;
      if (argument.hasDefaultValue()){
        defaultValue = argument.defaultValueAsString();
      }

      boost::optional<std::string> minValue;
      boost::optional<std::string> maxValue;
      if (argument.hasDomain()){
        // TODO
      }

      BCLMeasureArgument bclArgument(argument.name(), argument.displayName(),
                                     argument.description(), bclMeasureType, 
                                     argument.units(), argument.required(),
                                     argument.modelDependent(), defaultValue,
                                     argument.choiceValues(), argument.choiceValueDisplayNames(),
                                     minValue,  maxValue);

      bclArguments.push_back(bclArgument);
    }

    std::vector<BCLMeasureArgument> otherArguments = measure.arguments();
    if (otherArguments.size() != n){
      result = true;
      measure.setArguments(bclArguments);
    } else {
      for (unsigned i = 0; i < n; ++i){
        if (!(bclArguments[i] == otherArguments[i])){
          result = true;
          measure.setArguments(bclArguments);
          break;
        }
      }
    }

    return result;
  }

  RubyUserScriptInfo RubyUserScriptInfoGetter::getInfo(
    const BCLMeasure& measure,
    const boost::optional<model::Model>& model,
    const boost::optional<Workspace>& workspace)
{
  if (model) {
    if (workspace) {
      return this->getInfo(measure, *model, *workspace);
    }
    else {
      return this->getInfo(measure, *model);
    }
  }
  else if (workspace) {
    return this->getInfo(measure, *workspace);
  }
  else {
    return this->getInfo(measure);
  }
}

std::string infoExtractorRubyFunction() {
  std::stringstream ss;
  ss << "def infoExtractor(bclMeasure, optionalModel, optionalWorkspace)" << std::endl;
  ss << "  # GET THE USER SCRIPT" << std::endl;
  ss << "  scriptPath = bclMeasure.primaryRubyScriptPath()" << std::endl;
  ss << "  if scriptPath.empty?" << std::endl;
  ss << "    raise \"Unable to locate primary Ruby script path for BCLMeasure '\" + " << std::endl;
  ss << "        bclMeasure.name + \"' located at \" + bclMeasure.directory.to_s + \".\"" << std::endl;
  ss << "  end" << std::endl << std::endl;
  // Check list of objects in memory before loading the script
  ss << "  currentObjects = Hash.new" << std::endl;
  ss << "  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) { |obj| currentObjects[obj] = true }" << std::endl << std::endl; 
  ss << "  ObjectSpace.garbage_collect" << std::endl;
  // This line is REQUIRED or the ObjectSpace order will change when garbage collection runs automatically
  // If ~12 measures are added and garbage collection runs, the following loop to grab the first userscript
  //   will get the wrong one and return incorrect arguments
  ss << "  ObjectSpace.garbage_collect" << std::endl;
  ss << "  load scriptPath.get.to_s # need load in case have seen this script before" << std::endl;
  ss << std::endl;
  ss << "  userScript = nil" << std::endl;
  ss << "  type = String.new" << std::endl;
  ss << "  ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) do |obj|" << std::endl;
  ss << "    if not currentObjects[obj]" << std::endl;
  ss << "      if obj.is_a? OpenStudio::Ruleset::UtilityUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"utility\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::ModelUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"model\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::WorkspaceUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"workspace\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::TranslationUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"translation\"" << std::endl;
  ss << "      elsif obj.is_a? OpenStudio::Ruleset::ReportingUserScript" << std::endl;
  ss << "        userScript = obj" << std::endl;
  ss << "        type = \"report\"" << std::endl;
  ss << "      end" << std::endl;
  ss << "    end" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
  ss << "  if not userScript" << std::endl;
  ss << "    raise \"Unable to extract OpenStudio::Ruleset::UserScript object from \" + " << std::endl;
  ss << "        scriptPath.get.to_s + \". The script should contain a class that derives \" + " << std::endl;
  ss << "        \"from OpenStudio::Ruleset::UserScript and should close with a line stating \" + " << std::endl;
  ss << "        \"the class name followed by .new.registerWithApplication.\"" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
// The following lines may throw
  ss << "  measureType = nil" << std::endl;
  ss << "  className = userScript.class.to_s" << std::endl;
  ss << "  name = userScript.name" << std::endl;
  ss << "  name = className if name.empty?" << std::endl;
  ss << "  description = userScript.description" << std::endl;
  ss << "  modelerDescription = userScript.modeler_description" << std::endl;
  ss << "  args = OpenStudio::Ruleset::OSArgumentVector.new" << std::endl;
  ss << "  model = OpenStudio::Model::Model.new" << std::endl;
  ss << "  workspace = OpenStudio::Workspace.new(\"Draft\".to_StrictnessLevel," << std::endl;
  ss << "                                        \"EnergyPlus\".to_IddFileType)" << std::endl;
  ss << "  model = optionalModel.get if not optionalModel.empty?" << std::endl;
  ss << "  workspace = optionalWorkspace.get if not optionalWorkspace.empty?" << std::endl;
  ss << "  if type == \"utility\"" << std::endl;
  ss << "    measureType = OpenStudio::MeasureType.new(\"UtilityMeasure\")" << std::endl;
  ss << "    args = userScript.arguments" << std::endl;
  ss << "  elsif type == \"report\"" << std::endl;
  ss << "    measureType = OpenStudio::MeasureType.new(\"ReportingMeasure\")" << std::endl;
  ss << "    args = userScript.arguments()" << std::endl;
  ss << "  elsif type == \"model\"" << std::endl;
  ss << "    measureType = OpenStudio::MeasureType.new(\"ModelMeasure\")" << std::endl;
  ss << "    args = userScript.arguments(model)" << std::endl;
  ss << "  elsif type == \"workspace\"" << std::endl;
  ss << "    measureType = OpenStudio::MeasureType.new(\"EnergyPlusMeasure\")" << std::endl;
  ss << "    args = userScript.arguments(workspace)" << std::endl;
  ss << "  elsif type == \"translation\"" << std::endl;
  ss << "    measureType = OpenStudio::MeasureType.new(\"TranslationMeasure\")" << std::endl; // DLM: don't have this in BCLMeasure yet?
  ss << "    args = userScript.arguments(model)" << std::endl;
  ss << "    userScript.arguments(workspace).each { |arg|" << std::endl;
  ss << "      args << arg" << std::endl;
  ss << "    }" << std::endl;
  ss << "  end" << std::endl;
  ss << std::endl;
  ss << "  return OpenStudio::Ruleset::RubyUserScriptInfo.new(measureType, className, name, description, modelerDescription, args)" << std::endl;
  ss << "end" << std::endl;
  return ss.str();
}

} // ruleset
} // openstudio
