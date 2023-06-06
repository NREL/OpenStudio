/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  WorkflowStep_Impl::WorkflowStep_Impl() = default;

  boost::optional<WorkflowStepResult> WorkflowStep_Impl::result() const {
    return m_result;
  }

  void WorkflowStep_Impl::setResult(const WorkflowStepResult& result) {
    m_result = result;
    onUpdate();
  }

  void WorkflowStep_Impl::resetResult() {
    m_result.reset();
    onUpdate();
  }

  void WorkflowStep_Impl::onUpdate() {
    this->onChange.nano_emit();
  }

  MeasureStep_Impl::MeasureStep_Impl(const std::string& measureDirName) : m_measureDirName(measureDirName) {}

  std::string MeasureStep_Impl::string() const {
    Json::Value result;
    result["measure_dir_name"] = m_measureDirName;

    if (m_name) {
      result["name"] = m_name.get();
    }

    if (m_description) {
      result["description"] = m_description.get();
    }

    if (m_modelerDescription) {
      result["modeler_description"] = m_modelerDescription.get();
    }

    Json::Value arguments(Json::objectValue);
    for (const auto& argument : m_arguments) {
      std::string name = argument.first;
      Variant value = argument.second;

      if (value.variantType() == VariantType::String) {
        arguments[name] = value.valueAsString();
      } else if (value.variantType() == VariantType::Double) {
        arguments[name] = value.valueAsDouble();
      } else if (value.variantType() == VariantType::Integer) {
        arguments[name] = value.valueAsInteger();
      } else if (value.variantType() == VariantType::Boolean) {
        arguments[name] = value.valueAsBoolean();
      }
    }
    result["arguments"] = arguments;

    boost::optional<WorkflowStepResult> workflowStepResult = this->result();
    if (workflowStepResult) {

      // We let it fail but with a warning (this shouldn't ever happen really)
      Json::CharReaderBuilder rbuilder;
      std::istringstream ss(workflowStepResult->string());
      std::string formattedErrors;
      Json::Value value;
      bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
      if (parsingSuccessful) {
        result["result"] = value;
      } else {
        LOG(Warn, "Couldn't parse workflowStepResult s='" << workflowStepResult->string() << "'. Error: '" << formattedErrors << "'.");
      }
    }

    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string resultString = Json::writeString(wbuilder, result);

    return resultString;
  }

  std::string MeasureStep_Impl::measureDirName() const {
    return m_measureDirName;
  }

  bool MeasureStep_Impl::setMeasureDirName(const std::string& measureDirName) {
    m_measureDirName = measureDirName;
    return true;
  }

  boost::optional<std::string> MeasureStep_Impl::name() const {
    return m_name;
  }

  bool MeasureStep_Impl::setName(const std::string& name) {
    m_name = name;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetName() {
    m_name.reset();
    onUpdate();
  }

  boost::optional<std::string> MeasureStep_Impl::description() const {
    return m_description;
  }

  bool MeasureStep_Impl::setDescription(const std::string& description) {
    m_description = description;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetDescription() {
    m_description.reset();
    onUpdate();
  }

  boost::optional<std::string> MeasureStep_Impl::modelerDescription() const {
    return m_modelerDescription;
  }

  bool MeasureStep_Impl::setModelerDescription(const std::string& modelerDescription) {
    m_modelerDescription = modelerDescription;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetModelerDescription() {
    m_modelerDescription.reset();
    onUpdate();
  }

  std::map<std::string, Variant> MeasureStep_Impl::arguments() const {
    return m_arguments;
  }

  boost::optional<Variant> MeasureStep_Impl::getArgument(const std::string& name) const {
    auto it = m_arguments.find(name);
    if (it != m_arguments.end()) {
      return it->second;
    }
    return boost::none;
  }

  void MeasureStep_Impl::setArgument(const std::string& name, const Variant& value) {
    m_arguments[name] = value;
    onUpdate();
  }

  void MeasureStep_Impl::setArgument(const std::string& name, bool value) {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, double value) {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, int value) {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, const std::string& value) {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::removeArgument(const std::string& name) {
    m_arguments.erase(name);
    onUpdate();
  }

  void MeasureStep_Impl::clearArguments() {
    m_arguments.clear();
    onUpdate();
  }

}  // namespace detail

WorkflowStep::WorkflowStep(std::shared_ptr<detail::WorkflowStep_Impl> impl) : m_impl(impl) {
  OS_ASSERT(getImpl<detail::WorkflowStep_Impl>());
}

boost::optional<WorkflowStep> WorkflowStep::fromString(const std::string& s) {
  boost::optional<WorkflowStep> result;

  // We let it fail with a warning message
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  Json::Value value;
  bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
  if (!parsingSuccessful) {
    LOG(Warn, "Couldn't parse WorkflowStep from string s='" << s << "'. Error: '" << formattedErrors << "'.");
    return result;
  }

  if (value.isMember("measure_dir_name")) {
    Json::Value measureDirName = value["measure_dir_name"];

    MeasureStep measureStep(measureDirName.asString());
    result = measureStep;

    if (value.isMember("name")) {
      Json::Value name = value["name"];
      measureStep.setName(name.asString());
    }

    if (value.isMember("description")) {
      Json::Value description = value["description"];
      measureStep.setDescription(description.asString());
    }

    if (value.isMember("modeler_description")) {
      Json::Value modelerDescription = value["modeler_description"];
      measureStep.setModelerDescription(modelerDescription.asString());
    }

    Json::Value arguments = value["arguments"];
    for (const auto& name : arguments.getMemberNames()) {
      Json::Value arg_val = arguments[name];

      if (arg_val.isBool()) {
        measureStep.setArgument(name, arg_val.asBool());
      } else if (arg_val.isIntegral()) {
        measureStep.setArgument(name, arg_val.asInt());
      } else if (arg_val.isDouble()) {
        measureStep.setArgument(name, arg_val.asDouble());
      } else {
        measureStep.setArgument(name, arg_val.asString());
      }
    }
  }

  if (value.isMember("result")) {

    Json::Value v = value["result"];

    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result_str = Json::writeString(wbuilder, v);

    boost::optional<WorkflowStepResult> workflowStepResult = WorkflowStepResult::fromString(result_str);
    if (workflowStepResult) {
      result->setResult(*workflowStepResult);
    }
  }

  return result;
}

std::string WorkflowStep::string() const {
  return getImpl<detail::WorkflowStep_Impl>()->string();
}

boost::optional<WorkflowStepResult> WorkflowStep::result() const {
  return getImpl<detail::WorkflowStep_Impl>()->result();
}

void WorkflowStep::setResult(const WorkflowStepResult& result) {
  getImpl<detail::WorkflowStep_Impl>()->setResult(result);
}

void WorkflowStep::resetResult() {
  getImpl<detail::WorkflowStep_Impl>()->resetResult();
}

bool WorkflowStep::operator==(const WorkflowStep& other) const {
  return m_impl == other.m_impl;
}

MeasureStep::MeasureStep(const std::string& measureDirName)
  : WorkflowStep(std::shared_ptr<detail::MeasureStep_Impl>(new detail::MeasureStep_Impl(measureDirName))) {
  OS_ASSERT(getImpl<detail::MeasureStep_Impl>());
}

MeasureStep::MeasureStep(std::shared_ptr<detail::MeasureStep_Impl> impl) : WorkflowStep(impl) {
  OS_ASSERT(getImpl<detail::MeasureStep_Impl>());
}

std::string MeasureStep::measureDirName() const {
  return getImpl<detail::MeasureStep_Impl>()->measureDirName();
}

bool MeasureStep::setMeasureDirName(const std::string& measureDirName) {
  return getImpl<detail::MeasureStep_Impl>()->setMeasureDirName(measureDirName);
}

boost::optional<std::string> MeasureStep::name() const {
  return getImpl<detail::MeasureStep_Impl>()->name();
}

bool MeasureStep::setName(const std::string& name) {
  return getImpl<detail::MeasureStep_Impl>()->setName(name);
}

void MeasureStep::resetName() {
  getImpl<detail::MeasureStep_Impl>()->resetName();
}

boost::optional<std::string> MeasureStep::description() const {
  return getImpl<detail::MeasureStep_Impl>()->description();
}

bool MeasureStep::setDescription(const std::string& description) {
  return getImpl<detail::MeasureStep_Impl>()->setDescription(description);
}

void MeasureStep::resetDescription() {
  getImpl<detail::MeasureStep_Impl>()->resetDescription();
}

boost::optional<std::string> MeasureStep::modelerDescription() const {
  return getImpl<detail::MeasureStep_Impl>()->modelerDescription();
}

bool MeasureStep::setModelerDescription(const std::string& modelerDescription) {
  return getImpl<detail::MeasureStep_Impl>()->setModelerDescription(modelerDescription);
}

void MeasureStep::resetModelerDescription() {
  getImpl<detail::MeasureStep_Impl>()->resetModelerDescription();
}

std::map<std::string, Variant> MeasureStep::arguments() const {
  return getImpl<detail::MeasureStep_Impl>()->arguments();
}

boost::optional<Variant> MeasureStep::getArgument(const std::string& name) const {
  return getImpl<detail::MeasureStep_Impl>()->getArgument(name);
}

void MeasureStep::setArgument(const std::string& name, const Variant& value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, bool value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, double value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, int value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, const char* value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, std::string(value));
}

void MeasureStep::setArgument(const std::string& name, const std::string& value) {
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::removeArgument(const std::string& name) {
  getImpl<detail::MeasureStep_Impl>()->removeArgument(name);
}

void MeasureStep::clearArguments() {
  getImpl<detail::MeasureStep_Impl>()->clearArguments();
}

std::ostream& operator<<(std::ostream& os, const WorkflowStep& workflowStep) {
  os << workflowStep.string();
  return os;
}

}  // namespace openstudio
