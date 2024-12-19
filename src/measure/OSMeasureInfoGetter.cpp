/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  std::vector<BCLMeasureArgument> OSMeasureInfo::bclMeasureArguments() const {
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

      bclArguments.emplace_back(argument.name(), argument.displayName(), argument.description(), bclMeasureType, argument.units(),
                                argument.required(), argument.modelDependent(), defaultValue, argument.choiceValues(),
                                argument.choiceValueDisplayNames(), minValue, maxValue);
    }

    return bclArguments;
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

    std::vector<BCLMeasureArgument> bclArguments = bclMeasureArguments();

    std::vector<BCLMeasureArgument> otherArguments = measure.arguments();
    if (otherArguments.size() != bclArguments.size()) {
      result = true;
      measure.setArguments(bclArguments);
    } else {
      for (size_t i = 0; i < bclArguments.size(); ++i) {
        if (!(bclArguments[i] == otherArguments[i])) {
          result = true;
          measure.setArguments(bclArguments);
          break;
        }
      }
    }

    // handle outputs
    auto n = m_outputs.size();
    std::vector<BCLMeasureOutput> bclOutputs;
    bclOutputs.reserve(n);
    for (const OSOutput& output : m_outputs) {
      std::string bclOutputType = output.type().valueName();

      bclOutputs.emplace_back(output.name(), output.displayName(), output.shortName(), output.description(), bclOutputType, output.units(),
                              output.modelDependent());
    }

    std::vector<BCLMeasureOutput> otherOutputs = measure.outputs();
    if (otherOutputs.size() != n) {
      result = true;
      measure.setOutputs(bclOutputs);
    } else {
      for (size_t i = 0; i < n; ++i) {
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

  void MeasureInfoBinding::setMeasureInfo(OSMeasureInfo& info) {};
  bool MeasureInfoBinding::renderFile(const std::string& readmeInPath) {
    return false;
  };

}  // namespace measure
}  // namespace openstudio
