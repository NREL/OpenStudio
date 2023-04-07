/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "OSMeasureInfoGetter.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include <sstream>

namespace openstudio {
namespace measure {

  OSMeasureInfo::OSMeasureInfo(const std::string& error) : m_error(error) {}

  OSMeasureInfo::OSMeasureInfo(const MeasureType& measureType, const std::string& className, const std::string& name, const std::string& description,
                               const std::string& taxonomy, const std::string& modelerDescription, const std::vector<OSArgument>& arguments,
                               const std::vector<OSOutput>& outputs)
    : m_measureType(measureType),
      m_className(className),
      m_name(name),
      m_description(description),
      m_taxonomy(taxonomy),
      m_modelerDescription(modelerDescription),
      m_arguments(arguments),
      m_outputs(outputs) {}

  boost::optional<std::string> OSMeasureInfo::error() const {
    return m_error;
  }

  MeasureType OSMeasureInfo::measureType() const {
    return m_measureType;
  }

  std::string OSMeasureInfo::className() const {
    return m_className;
  }

  std::string OSMeasureInfo::name() const {
    return m_name;
  }

  std::string OSMeasureInfo::description() const {
    return m_description;
  }

  std::string OSMeasureInfo::taxonomy() const {
    return m_taxonomy;
  }

  std::string OSMeasureInfo::modelerDescription() const {
    return m_modelerDescription;
  }

  std::vector<OSArgument> OSMeasureInfo::arguments() const {
    return m_arguments;
  }

  std::vector<OSOutput> OSMeasureInfo::outputs() const {
    return m_outputs;
  }

  bool OSMeasureInfo::update(BCLMeasure& measure) const {
    bool result = false;

    if (m_error) {
      // have error
      if (!measure.error() || measure.error().get() != *m_error) {
        result = true;
        measure.setError(*m_error);
      }
      // return here so we don't blow away name or other information from last successful run
      return result;
    } else {
      // no error
      if (measure.error()) {
        result = true;
        measure.resetError();
      }
    }

    // map snake cased class name to bcl measure name
    std::string lowerClassName = toUnderscoreCase(m_className);
    if (!lowerClassName.empty() && measure.name() != lowerClassName) {
      result = true;
      measure.setName(lowerClassName);
    }

    // map name from measure to bcl measure display name
    if (!m_name.empty() && measure.displayName() != m_name) {
      result = true;
      measure.setDisplayName(m_name);
    }

    if (!m_className.empty() && measure.className() != m_className) {
      result = true;
      measure.setClassName(m_className);
    }

    if (!m_description.empty() && measure.description() != m_description) {
      result = true;
      measure.setDescription(m_description);
    }

    if (!m_taxonomy.empty() && measure.taxonomyTag() != m_taxonomy) {
      result = true;
      measure.setTaxonomyTag(m_taxonomy);
    }

    if (!m_modelerDescription.empty() && measure.modelerDescription() != m_modelerDescription) {
      result = true;
      measure.setModelerDescription(m_modelerDescription);
    }

    unsigned n = m_arguments.size();
    std::vector<BCLMeasureArgument> bclArguments;
    bclArguments.reserve(n);
    for (const OSArgument& argument : m_arguments) {

      std::string bclMeasureType = argument.type().valueName();
      if (argument.type() == OSArgumentType::Quantity) {
        LOG(Warn, "Mapping deprecated OSArgumentType::Quantity to Double");
        bclMeasureType = "Double";
      }

      boost::optional<std::string> defaultValue;
      if (argument.hasDefaultValue()) {
        defaultValue = argument.defaultValueAsString();
      }

      boost::optional<std::string> minValue;
      boost::optional<std::string> maxValue;
      if (argument.hasDomain()) {
        if (argument.type() == OSArgumentType::Integer) {
          auto domain = argument.domainAsInteger();
          auto& low = domain.front();
          auto& high = domain.back();
          if (low > std::numeric_limits<int>::lowest()) {
            minValue = std::to_string(low);
          }
          if (high < std::numeric_limits<int>::max()) {
            maxValue = std::to_string(high);
          }
        } else if (argument.type() == OSArgumentType::Double) {
          auto domain = argument.domainAsDouble();
          auto& low = domain.front();
          auto& high = domain.back();
          if (low > std::numeric_limits<double>::lowest()) {
            minValue = std::to_string(low);
          }
          if (high < std::numeric_limits<double>::max()) {
            maxValue = std::to_string(high);
          }
        }
      }

      BCLMeasureArgument bclArgument(argument.name(), argument.displayName(), argument.description(), bclMeasureType, argument.units(),
                                     argument.required(), argument.modelDependent(), defaultValue, argument.choiceValues(),
                                     argument.choiceValueDisplayNames(), minValue, maxValue);

      bclArguments.push_back(bclArgument);
    }

    std::vector<BCLMeasureArgument> otherArguments = measure.arguments();
    if (otherArguments.size() != n) {
      result = true;
      measure.setArguments(bclArguments);
    } else {
      for (unsigned i = 0; i < n; ++i) {
        if (!(bclArguments[i] == otherArguments[i])) {
          result = true;
          measure.setArguments(bclArguments);
          break;
        }
      }
    }

    // handle outputs
    n = m_outputs.size();
    std::vector<BCLMeasureOutput> bclOutputs;
    bclOutputs.reserve(n);
    for (const OSOutput& output : m_outputs) {
      std::string bclOutputType = output.type().valueName();

      BCLMeasureOutput bclOutput(output.name(), output.displayName(), output.shortName(), output.description(), bclOutputType, output.units(),
                                 output.modelDependent());
      bclOutputs.push_back(bclOutput);
    }

    std::vector<BCLMeasureOutput> otherOutputs = measure.outputs();
    if (otherOutputs.size() != n) {
      result = true;
      measure.setOutputs(bclOutputs);
    } else {
      for (unsigned i = 0; i < n; ++i) {
        if (!(bclOutputs[i] == otherOutputs[i])) {
          result = true;
          measure.setOutputs(bclOutputs);
          break;
        }
      }
    }

    return result;
  }

  OSMeasureInfo OSMeasureInfoGetter::getInfo(const BCLMeasure& measure, const boost::optional<model::Model>& model,
                                             const boost::optional<Workspace>& workspace) {
    if (model) {
      if (workspace) {
        return this->getInfo(measure, *model, *workspace);
      } else {
        return this->getInfo(measure, *model);
      }
    } else if (workspace) {
      return this->getInfo(measure, *workspace);
    } else {
      return this->getInfo(measure);
    }
  }

  std::string infoExtractorRubyFunction() {
    std::stringstream ss;
    ss << "def infoExtractor(bclMeasure, optionalModel, optionalWorkspace)" << '\n';
    ss << "  # GET THE USER SCRIPT" << '\n';
    ss << "  scriptPath = bclMeasure.primaryRubyScriptPath()" << '\n';
    ss << "  if scriptPath.empty?" << '\n';
    ss << "    raise \"Unable to locate primary Ruby script path for BCLMeasure '\" + " << '\n';
    ss << "        bclMeasure.name + \"' located at \" + bclMeasure.directory.to_s + \".\"" << '\n';
    ss << "  end" << '\n' << '\n';
    // Check list of objects in memory before loading the script
    ss << "  currentObjects = Hash.new" << '\n';
    ss << "  ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) { |obj| currentObjects[obj] = true }" << '\n' << '\n';
    ss << "  ObjectSpace.garbage_collect" << '\n';
    // This line is REQUIRED or the ObjectSpace order will change when garbage collection runs automatically
    // If ~12 measures are added and garbage collection runs, the following loop to grab the first measure
    //   will get the wrong one and return incorrect arguments
    ss << "  ObjectSpace.garbage_collect" << '\n';
    ss << "  load scriptPath.get.to_s # need load in case have seen this script before" << '\n';
    ss << '\n';
    ss << "  measure = nil" << '\n';
    ss << "  type = String.new" << '\n';
    ss << "  ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) do |obj|" << '\n';
    ss << "    if not currentObjects[obj]" << '\n';
    ss << "      if obj.is_a? OpenStudio::Measure::ModelMeasure" << '\n';
    ss << "        measure = obj" << '\n';
    ss << "        type = \"model\"" << '\n';
    ss << "      elsif obj.is_a? OpenStudio::Measure::EnergyPlusMeasure" << '\n';
    ss << "        measure = obj" << '\n';
    ss << "        type = \"energyplus\"" << '\n';
    ss << "      elsif obj.is_a? OpenStudio::Measure::ReportingMeasure" << '\n';
    ss << "        measure = obj" << '\n';
    ss << "        type = \"report\"" << '\n';
    ss << "      end" << '\n';
    ss << "    end" << '\n';
    ss << "  end" << '\n';
    ss << '\n';
    ss << "  if not measure" << '\n';
    ss << "    raise \"Unable to extract OpenStudio::Measure::OSMeasure object from \" + " << '\n';
    ss << "        scriptPath.get.to_s + \". The script should contain a class that derives \" + " << '\n';
    ss << "        \"from OpenStudio::Measure::OSMeasure and should close with a line stating \" + " << '\n';
    ss << "        \"the class name followed by .new.registerWithApplication.\"" << '\n';
    ss << "  end" << '\n';
    ss << '\n';
    // The following lines may throw
    ss << "  measureType = nil" << '\n';
    ss << "  className = measure.class.to_s" << '\n';
    ss << "  name = measure.name" << '\n';
    ss << "  name = className if name.empty?" << '\n';
    ss << "  description = measure.description" << '\n';
    ss << "  taxonomy = measure.taxonomy" << '\n';
    ss << "  modelerDescription = measure.modeler_description" << '\n';
    ss << "  args = OpenStudio::Measure::OSArgumentVector.new" << '\n';
    ss << "  outputs = OpenStudio::Measure::OSOutputVector.new" << '\n';
    ss << "  model = OpenStudio::Model::Model.new" << '\n';
    ss << "  workspace = OpenStudio::Workspace.new(\"Draft\".to_StrictnessLevel," << '\n';
    ss << "                                        \"EnergyPlus\".to_IddFileType)" << '\n';
    ss << "  model = optionalModel.get if not optionalModel.empty?" << '\n';
    ss << "  workspace = optionalWorkspace.get if not optionalWorkspace.empty?" << '\n';
    ss << "  if type == \"report\"" << '\n';
    ss << "    measureType = OpenStudio::MeasureType.new(\"ReportingMeasure\")" << '\n';
    ss << "    args = measure.arguments(model)" << '\n';
    ss << "    outputs = measure.outputs()" << '\n';
    ss << "  elsif type == \"model\"" << '\n';
    ss << "    measureType = OpenStudio::MeasureType.new(\"ModelMeasure\")" << '\n';
    ss << "    args = measure.arguments(model)" << '\n';
    ss << "    outputs = measure.outputs()" << '\n';
    ss << "  elsif type == \"energyplus\"" << '\n';
    ss << "    measureType = OpenStudio::MeasureType.new(\"EnergyPlusMeasure\")" << '\n';
    ss << "    args = measure.arguments(workspace)" << '\n';
    ss << "    outputs = measure.outputs()" << '\n';
    ss << "  end" << '\n';
    ss << '\n';
    ss << "  return OpenStudio::Measure::OSMeasureInfo.new(measureType, className, name, description, taxonomy, modelerDescription, args, outputs)"
       << '\n';
    ss << "end" << '\n';
    return ss.str();
  }

}  // namespace measure
}  // namespace openstudio
