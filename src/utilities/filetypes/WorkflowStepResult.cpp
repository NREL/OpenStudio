/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkflowStepResult.hpp"
#include "WorkflowStepResult_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <json/json.h>

#include <utility>

namespace openstudio {
namespace detail {

  WorkflowStepValue_Impl::WorkflowStepValue_Impl(const std::string& name, const Variant& value) : m_name(name), m_value(value) {}

  Json::Value WorkflowStepValue_Impl::toJSON() const {
    Json::Value root(Json::objectValue);
    root["name"] = m_name;

    if (m_displayName) {
      root["display_name"] = *m_displayName;
    }

    if (m_units) {
      root["units"] = *m_units;
    }

    root["value"] = m_value.valueAsJSON();

    return root;
  }

  std::string WorkflowStepValue_Impl::string() const {
    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, toJSON());

    return result;
  }

  std::string WorkflowStepValue_Impl::name() const {
    return m_name;
  }

  std::string WorkflowStepValue_Impl::displayName() const {
    if (m_displayName) {
      return *m_displayName;
    }
    return m_name;
  }

  boost::optional<std::string> WorkflowStepValue_Impl::units() const {
    return m_units;
  }

  VariantType WorkflowStepValue_Impl::variantType() const {
    return m_value.variantType();
  }

  Variant WorkflowStepValue_Impl::valueAsVariant() const {
    return m_value;
  }

  bool WorkflowStepValue_Impl::valueAsBoolean() const {
    return m_value.valueAsBoolean();
  }

  int WorkflowStepValue_Impl::valueAsInteger() const {
    return m_value.valueAsInteger();
  }

  double WorkflowStepValue_Impl::valueAsDouble() const {
    return m_value.valueAsDouble();
  }

  std::string WorkflowStepValue_Impl::valueAsString() const {
    return m_value.valueAsString();
  }

  Json::Value WorkflowStepValue_Impl::valueAsJSON() const {
    return m_value.valueAsJSON();
  }

  void WorkflowStepValue_Impl::setName(const std::string& name) {
    m_name = name;
  }

  void WorkflowStepValue_Impl::setDisplayName(const std::string& displayName) {
    m_displayName = displayName;
  }

  void WorkflowStepValue_Impl::resetDisplayName() {
    m_displayName.reset();
  }

  void WorkflowStepValue_Impl::setUnits(const std::string& units) {
    m_units = units;
  }

  void WorkflowStepValue_Impl::resetUnits() {
    m_units.reset();
  }

  WorkflowStepResult_Impl::WorkflowStepResult_Impl() = default;

  Json::Value WorkflowStepResult_Impl::toJSON() const {

    Json::Value root(Json::objectValue);
    bool complete = false;

    if (startedAt()) {
      root["started_at"] = startedAt()->toISO8601();
    }

    if (completedAt()) {
      complete = true;
      root["completed_at"] = completedAt()->toISO8601();
    }

    if (m_measureType) {
      root["measure_type"] = m_measureType->valueName();
    }

    if (m_measureName) {
      root["measure_name"] = m_measureName.get();
    }

    if (m_measureId) {
      root["measure_uid"] = m_measureId.get();
    }

    if (m_measureVersionId) {
      root["measure_version_id"] = m_measureVersionId.get();
    }

    if (m_measureVersionModified) {
      root["measure_version_modified"] = m_measureVersionModified.get();
    }

    if (m_measureXmlChecksum) {
      root["measure_xml_checksum"] = m_measureXmlChecksum.get();
    }

    if (m_measureClassName) {
      root["measure_class_name"] = m_measureClassName.get();
    }

    if (m_measureDisplayName) {
      root["measure_display_name"] = m_measureDisplayName.get();
    }

    if (m_measureTaxonomy) {
      root["measure_taxonomy"] = m_measureTaxonomy.get();
    }

    if (complete) {
      if (stepResult()) {
        root["step_result"] = stepResult()->valueName();
      } else {
        // error
      }
    }

    if (stepInitialCondition()) {
      root["step_initial_condition"] = stepInitialCondition().get();
    }

    if (stepFinalCondition()) {
      root["step_final_condition"] = stepFinalCondition().get();
    }

    if (complete || (!stepErrors().empty())) {
      Json::Value errors(Json::arrayValue);
      for (const auto& stepError : stepErrors()) {
        errors.append(stepError);
      }
      root["step_errors"] = errors;
    }

    if (complete || (!stepWarnings().empty())) {
      Json::Value warnings(Json::arrayValue);
      for (const auto& stepWarning : stepWarnings()) {
        warnings.append(stepWarning);
      }
      root["step_warnings"] = warnings;
    }

    if (complete || (!stepInfo().empty())) {
      Json::Value info(Json::arrayValue);
      for (const auto& stepI : stepInfo()) {
        info.append(stepI);
      }
      root["step_info"] = info;
    }

    if (complete || (!stepValues().empty())) {
      Json::Value values(Json::arrayValue);
      for (const auto& stepValue : stepValues()) {
        values.append(stepValue.toJSON());
      }
      root["step_values"] = values;
    }

    if (complete || (!stepFiles().empty())) {
      Json::Value files(Json::arrayValue);
      for (const auto& stepFile : stepFiles()) {
        files.append(toString(stepFile));
      }
      root["step_files"] = files;
    }

    if (stdOut()) {
      root["stdout"] = stdOut().get();
    }

    if (stdErr()) {
      root["stderr"] = stdErr().get();
    }

    return root;
  }

  std::string WorkflowStepResult_Impl::string() const {
    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, toJSON());

    return result;
  }

  boost::optional<DateTime> WorkflowStepResult_Impl::startedAt() const {
    return m_startedAt;
  }

  boost::optional<DateTime> WorkflowStepResult_Impl::completedAt() const {
    return m_completedAt;
  }

  boost::optional<MeasureType> WorkflowStepResult_Impl::measureType() const {
    return m_measureType;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureName() const {
    return m_measureName;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureId() const {
    return m_measureId;
  }

  boost::optional<UUID> WorkflowStepResult_Impl::measureUUID() const {
    if (!m_measureId) {
      return boost::none;
    }
    return toUUID("{" + m_measureId.get() + "}");
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureVersionId() const {
    return m_measureVersionId;
  }

  boost::optional<UUID> WorkflowStepResult_Impl::measureVersionUUID() const {
    if (!m_measureVersionId) {
      return boost::none;
    }
    return toUUID("{" + m_measureVersionId.get() + "}");
  }

  boost::optional<DateTime> WorkflowStepResult_Impl::measureVersionModified() const {
    if (m_measureVersionModified) {
      return DateTime::fromISO8601(*m_measureVersionModified);
    }
    return boost::none;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureXmlChecksum() const {
    return m_measureXmlChecksum;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureClassName() const {
    return m_measureClassName;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureDisplayName() const {
    return m_measureDisplayName;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::measureTaxonomy() const {
    return m_measureTaxonomy;
  }

  boost::optional<StepResult> WorkflowStepResult_Impl::stepResult() const {
    return m_stepResult;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stepInitialCondition() const {
    return m_stepInitialCondition;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stepFinalCondition() const {
    return m_stepFinalCondition;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepErrors() const {
    return m_stepErrors;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepWarnings() const {
    return m_stepWarnings;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepInfo() const {
    return m_stepInfo;
  }

  std::vector<WorkflowStepValue> WorkflowStepResult_Impl::stepValues() const {
    return m_stepValues;
  }

  std::vector<openstudio::path> WorkflowStepResult_Impl::stepFiles() const {
    return m_stepFiles;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stdOut() const {
    return m_stdOut;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::stdErr() const {
    return m_stdErr;
  }

  StepResult WorkflowStepResult_Impl::value() const {
    LOG(Debug, "WorkflowStepResult::value is deprecated, use stepResult instead");
    if (m_stepResult) {
      return m_stepResult.get();
    }
    LOG(Warn, "WorkflowStepResult value called with undefined stepResult, returning 'Success'");
    return StepResult::Success;
  }

  std::vector<LogMessage> WorkflowStepResult_Impl::errors() const {
    LOG(Debug, "WorkflowStepResult::errors is deprecated, use stepErrors instead");
    std::vector<LogMessage> result;
    for (const auto& stepError : stepErrors()) {
      result.emplace_back(Error, "", stepError);
    }
    return result;
  }

  std::vector<LogMessage> WorkflowStepResult_Impl::warnings() const {
    LOG(Debug, "WorkflowStepResult::warnings is deprecated, use stepWarnings instead");
    std::vector<LogMessage> result;
    for (const auto& stepWarning : stepWarnings()) {
      result.emplace_back(Warn, "", stepWarning);
    }
    return result;
  }

  std::vector<LogMessage> WorkflowStepResult_Impl::info() const {
    LOG(Debug, "WorkflowStepResult::info is deprecated, use stepInfo instead");
    std::vector<LogMessage> result;
    for (const auto& info : stepInfo()) {
      result.emplace_back(Info, "", info);
    }
    return result;
  }

  boost::optional<LogMessage> WorkflowStepResult_Impl::initialCondition() const {
    LOG(Debug, "WorkflowStepResult::initialCondition is deprecated, use stepInitialCondition instead");
    boost::optional<LogMessage> result;
    if (m_stepInitialCondition) {
      result = LogMessage(Info, "", *m_stepInitialCondition);
    }
    return result;
  }

  boost::optional<LogMessage> WorkflowStepResult_Impl::finalCondition() const {
    LOG(Debug, "WorkflowStepResult::finalCondition is deprecated, use stepFinalCondition instead");
    boost::optional<LogMessage> result;
    if (m_stepFinalCondition) {
      result = LogMessage(Info, "", *m_stepFinalCondition);
    }
    return result;
  }

  std::vector<Attribute> WorkflowStepResult_Impl::attributes() const {
    LOG(Debug, "WorkflowStepResult::attributes is deprecated, use stepValues instead");
    std::vector<Attribute> result;
    for (const auto& stepValue : stepValues()) {
      boost::optional<Attribute> attribute;

      const VariantType variantType = stepValue.variantType();
      switch (variantType.value()) {
        case VariantType::Boolean:
          attribute = Attribute(stepValue.name(), stepValue.valueAsBoolean());
          break;
        case VariantType::Double:
          attribute = Attribute(stepValue.name(), stepValue.valueAsDouble());
          break;
        case VariantType::Integer:
          attribute = Attribute(stepValue.name(), stepValue.valueAsInteger());
          break;
        case VariantType::String:
          attribute = Attribute(stepValue.name(), stepValue.valueAsString());
          break;
        default:
          LOG(Warn, "Unknown Variant Type " << variantType.valueName());
      }

      if (attribute) {
        attribute->setDisplayName(stepValue.displayName());
        if (stepValue.units()) {
          attribute->setUnits(stepValue.units().get());
        }
        result.push_back(*attribute);
      }
    }
    return result;
  }

  void WorkflowStepResult_Impl::setStartedAt(const DateTime& dateTime) {
    m_startedAt = dateTime;
  }

  void WorkflowStepResult_Impl::resetStartedAt() {
    m_startedAt.reset();
  }

  void WorkflowStepResult_Impl::setCompletedAt(const DateTime& dateTime) {
    m_completedAt = dateTime;
  }

  void WorkflowStepResult_Impl::resetCompletedAt() {
    m_completedAt.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureType(const MeasureType& measureType) {
    m_measureType = measureType;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureType() {
    m_measureType.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureName(const std::string& name) {
    m_measureName = name;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureName() {
    m_measureName.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureId(const std::string& id) {
    m_measureId = id;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureId() {
    m_measureId.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureUUID(const UUID& uuid) {
    m_measureId = removeBraces(uuid);
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureUUID() {
    m_measureId.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureVersionId(const std::string& id) {
    m_measureVersionId = id;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureVersionId() {
    m_measureVersionId.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureVersionUUID(const UUID& uuid) {
    m_measureVersionId = removeBraces(uuid);
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureVersionUUID() {
    m_measureVersionId.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureVersionModified(const DateTime& versionModified) {
    m_measureVersionModified = versionModified.toISO8601();
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureVersionModified() {
    m_measureVersionModified.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureXmlChecksum(const std::string& checksum) {
    m_measureXmlChecksum = checksum;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureXmlChecksum() {
    m_measureXmlChecksum.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureClassName(const std::string& className) {
    m_measureClassName = className;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureClassName() {
    m_measureClassName.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureDisplayName(const std::string& displayName) {
    m_measureDisplayName = displayName;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureDisplayName() {
    m_measureDisplayName.reset();
  }

  bool WorkflowStepResult_Impl::setMeasureTaxonomy(const std::string& taxonomy) {
    m_measureTaxonomy = taxonomy;
    return true;
  }

  void WorkflowStepResult_Impl::resetMeasureTaxonomy() {
    m_measureTaxonomy.reset();
  }

  void WorkflowStepResult_Impl::setStepResult(const StepResult& result) {
    m_stepResult = result;
  }

  void WorkflowStepResult_Impl::resetStepResult() {
    m_stepResult.reset();
  }

  void WorkflowStepResult_Impl::setStepInitialCondition(const std::string& initialCondition) {
    m_stepInitialCondition = initialCondition;
  }

  void WorkflowStepResult_Impl::resetStepInitialCondition() {
    m_stepInitialCondition.reset();
  }

  void WorkflowStepResult_Impl::setStepFinalCondition(const std::string& finalCondition) {
    m_stepFinalCondition = finalCondition;
  }

  void WorkflowStepResult_Impl::resetStepFinalCondition() {
    m_stepFinalCondition.reset();
  }

  void WorkflowStepResult_Impl::addStepError(const std::string& error) {
    m_stepErrors.push_back(error);
  }

  void WorkflowStepResult_Impl::resetStepErrors() {
    m_stepErrors.clear();
  }

  void WorkflowStepResult_Impl::addStepWarning(const std::string& warning) {
    m_stepWarnings.push_back(warning);
  }

  void WorkflowStepResult_Impl::resetStepWarnings() {
    m_stepWarnings.clear();
  }

  void WorkflowStepResult_Impl::addStepInfo(const std::string& info) {
    m_stepInfo.push_back(info);
  }

  void WorkflowStepResult_Impl::resetStepInfo() {
    m_stepInfo.clear();
  }

  void WorkflowStepResult_Impl::addStepValue(const WorkflowStepValue& value) {
    // remove any values with the same name
    m_stepValues.erase(
      std::remove_if(m_stepValues.begin(), m_stepValues.end(), [&value](const WorkflowStepValue& x) { return x.name() == value.name(); }),
      m_stepValues.end());

    m_stepValues.push_back(value);
  }

  void WorkflowStepResult_Impl::resetStepValues() {
    m_stepValues.clear();
  }

  void WorkflowStepResult_Impl::addStepFile(const openstudio::path& path) {
    // remove any values with the same path
    m_stepFiles.erase(std::remove_if(m_stepFiles.begin(), m_stepFiles.end(), [&path](const openstudio::path& x) { return x == path; }),
                      m_stepFiles.end());

    m_stepFiles.push_back(path);
  }

  void WorkflowStepResult_Impl::resetStepFiles() {
    m_stepFiles.clear();
  }

  void WorkflowStepResult_Impl::setStdOut(const std::string& stdOut) {
    m_stdOut = stdOut;
  }

  void WorkflowStepResult_Impl::resetStdOut() {
    m_stdOut.reset();
  }

  void WorkflowStepResult_Impl::setStdErr(const std::string& stdErr) {
    m_stdErr = stdErr;
  }

  void WorkflowStepResult_Impl::resetStdErr() {
    m_stdErr.reset();
  }

}  // namespace detail

WorkflowStepValue::WorkflowStepValue(const std::string& name, const Variant& value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, value))) {}

WorkflowStepValue::WorkflowStepValue(const std::string& name, const std::string& value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, Variant(value)))) {}

WorkflowStepValue::WorkflowStepValue(const std::string& name, const char* value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, Variant(std::string(value))))) {}

WorkflowStepValue::WorkflowStepValue(const std::string& name, double value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, Variant(value)))) {}

WorkflowStepValue::WorkflowStepValue(const std::string& name, int value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, Variant(value)))) {}

WorkflowStepValue::WorkflowStepValue(const std::string& name, bool value)
  : m_impl(std::shared_ptr<detail::WorkflowStepValue_Impl>(new detail::WorkflowStepValue_Impl(name, Variant(value)))) {}

boost::optional<WorkflowStepValue> WorkflowStepValue::fromString(const std::string& s) {
  // We let it fail with a warning message
  const Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  Json::Value value;
  const bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
  if (!parsingSuccessful) {
    LOG(Warn, "Couldn't parse WorkflowStepValue from string s='" << s << "'. Error: '" << formattedErrors << "'.");
    return boost::none;
  }

  boost::optional<WorkflowStepValue> result;

  try {
    const std::string name = value["name"].asString();
    const Json::Value v = value["value"];
    if (v.isString()) {
      result = WorkflowStepValue(name, v.asString());
    } else if (v.isIntegral()) {
      result = WorkflowStepValue(name, v.asInt());
    } else if (v.isDouble()) {
      result = WorkflowStepValue(name, v.asDouble());
    } else if (v.isBool()) {
      result = WorkflowStepValue(name, v.asBool());
    } else {
      //error
    }

  } catch (const std::exception&) {
    return boost::none;
  }

  return result;
}

std::string WorkflowStepValue::string() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->string();
}

Json::Value WorkflowStepValue::toJSON() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->toJSON();
}

std::string WorkflowStepValue::name() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->name();
}

std::string WorkflowStepValue::displayName() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->displayName();
}

boost::optional<std::string> WorkflowStepValue::units() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->units();
}

VariantType WorkflowStepValue::variantType() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->variantType();
}

Variant WorkflowStepValue::valueAsVariant() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsVariant();
}

bool WorkflowStepValue::valueAsBoolean() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsBoolean();
}

int WorkflowStepValue::valueAsInteger() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsInteger();
}

double WorkflowStepValue::valueAsDouble() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsDouble();
}

std::string WorkflowStepValue::valueAsString() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsString();
}

Json::Value WorkflowStepValue::valueAsJSON() const {
  return getImpl<detail::WorkflowStepValue_Impl>()->valueAsJSON();
}

void WorkflowStepValue::setName(const std::string& name) {
  getImpl<detail::WorkflowStepValue_Impl>()->setName(name);
}

void WorkflowStepValue::setDisplayName(const std::string& displayName) {
  getImpl<detail::WorkflowStepValue_Impl>()->setDisplayName(displayName);
}

void WorkflowStepValue::resetDisplayName() {
  getImpl<detail::WorkflowStepValue_Impl>()->resetDisplayName();
}

void WorkflowStepValue::setUnits(const std::string& units) {
  getImpl<detail::WorkflowStepValue_Impl>()->setUnits(units);
}

void WorkflowStepValue::resetUnits() {
  getImpl<detail::WorkflowStepValue_Impl>()->resetUnits();
}

WorkflowStepResult::WorkflowStepResult() : m_impl(std::shared_ptr<detail::WorkflowStepResult_Impl>(new detail::WorkflowStepResult_Impl)) {
  OS_ASSERT(getImpl<detail::WorkflowStepResult_Impl>());
}

WorkflowStepResult::WorkflowStepResult(const WorkflowStepResult& other) = default;

WorkflowStepResult::WorkflowStepResult(std::shared_ptr<detail::WorkflowStepResult_Impl> impl) : m_impl(std::move(impl)) {}

boost::optional<WorkflowStepResult> WorkflowStepResult::fromString(const std::string& s) {
  // We let it fail with a warning message
  const Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  Json::Value value;
  const bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
  if (!parsingSuccessful) {
    LOG(Warn, "Couldn't parse WorkflowStepResult from string s='" << s << "'. Error: '" << formattedErrors << "'.");
    return boost::none;
  }

  WorkflowStepResult result;

  try {

    if (value.isMember("started_at")) {
      boost::optional<DateTime> dateTime = DateTime::fromISO8601(value["started_at"].asString());
      if (dateTime) {
        result.setStartedAt(*dateTime);
      }
    }

    if (value.isMember("completed_at")) {
      boost::optional<DateTime> dateTime = DateTime::fromISO8601(value["completed_at"].asString());
      if (dateTime) {
        result.setCompletedAt(*dateTime);
      }
    }

    if (value.isMember("measure_type")) {
      const Json::Value measureType = value["measure_type"];
      try {
        result.setMeasureType(MeasureType(measureType.asString()));
      } catch (const std::exception&) {
        LOG(Error, measureType.asString() << " is not a valid MeasureType.")
      }
    }

    if (value.isMember("measure_name")) {
      const Json::Value measureName = value["measure_name"];
      result.setMeasureName(measureName.asString());
    }

    if (value.isMember("measure_uid")) {
      const Json::Value measureId = value["measure_uid"];
      result.setMeasureId(measureId.asString());
    }

    if (value.isMember("measure_version_id")) {
      const Json::Value versionId = value["measure_version_id"];
      result.setMeasureVersionId(versionId.asString());
    }

    if (value.isMember("measure_version_modified")) {
      const Json::Value versionModified = value["measure_version_modified"];
      const std::string str = versionModified.asString();
      boost::optional<DateTime> dateTime = DateTime::fromISO8601(versionModified.asString());
      if (dateTime) {
        result.setMeasureVersionModified(dateTime.get());
      }
    }

    if (value.isMember("measure_xml_checksum")) {
      const Json::Value checksum = value["measure_xml_checksum"];
      result.setMeasureXmlChecksum(checksum.asString());
    }

    if (value.isMember("measure_class_name")) {
      const Json::Value className = value["measure_class_name"];
      result.setMeasureClassName(className.asString());
    }

    if (value.isMember("measure_display_name")) {
      const Json::Value displayName = value["measure_display_name"];
      result.setMeasureDisplayName(displayName.asString());
    }

    if (value.isMember("measure_taxonomy")) {
      const Json::Value taxonomy = value["measure_taxonomy"];
      result.setMeasureTaxonomy(taxonomy.asString());
    }

    if (value.isMember("step_result")) {
      const StepResult stepResult(value["step_result"].asString());
      result.setStepResult(stepResult);
    }

    if (value.isMember("step_initial_condition")) {
      result.setStepInitialCondition(value["step_initial_condition"].asString());
    }

    if (value.isMember("step_final_condition")) {
      result.setStepFinalCondition(value["step_final_condition"].asString());
    }

    const Json::Value defaultArrayValue(Json::arrayValue);

    Json::Value errors = value.get("step_errors", defaultArrayValue);
    Json::ArrayIndex n = errors.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.addStepError(errors[i].asString());
    }

    Json::Value warnings = value.get("step_warnings", defaultArrayValue);
    n = warnings.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.addStepWarning(warnings[i].asString());
    }

    Json::Value info = value.get("step_info", defaultArrayValue);
    n = info.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.addStepInfo(info[i].asString());
    }

    Json::Value stepValues = value.get("step_values", defaultArrayValue);
    n = stepValues.size();

    if (n > 0) {
      Json::StreamWriterBuilder wbuilder;
      // mimic the old StyledWriter behavior:
      wbuilder["indentation"] = "   ";
      for (Json::ArrayIndex i = 0; i < n; ++i) {
        const std::string stepval_str = Json::writeString(wbuilder, stepValues[i]);
        boost::optional<WorkflowStepValue> workflowStepValue = WorkflowStepValue::fromString(stepval_str);
        if (workflowStepValue) {
          result.addStepValue(*workflowStepValue);
        }
      }
    }

    Json::Value files = value.get("step_files", defaultArrayValue);
    n = files.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      result.addStepFile(toPath(files[i].asString()));
    }

    if (value.isMember("stdout")) {
      result.setStdOut(value["stdout"].asString());
    }

    if (value.isMember("stderr")) {
      result.setStdErr(value["stderr"].asString());
    }

  } catch (const std::exception&) {
    return boost::none;
  }

  return result;
}

std::string WorkflowStepResult::string() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->string();
}

Json::Value WorkflowStepResult::toJSON() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->toJSON();
}

boost::optional<DateTime> WorkflowStepResult::startedAt() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->startedAt();
}

boost::optional<DateTime> WorkflowStepResult::completedAt() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->completedAt();
}

boost::optional<MeasureType> WorkflowStepResult::measureType() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureType();
}

boost::optional<std::string> WorkflowStepResult::measureName() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureName();
}

boost::optional<std::string> WorkflowStepResult::measureId() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureId();
}

boost::optional<UUID> WorkflowStepResult::measureUUID() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureUUID();
}

boost::optional<std::string> WorkflowStepResult::measureVersionId() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureVersionId();
}

boost::optional<UUID> WorkflowStepResult::measureVersionUUID() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureVersionUUID();
}

boost::optional<DateTime> WorkflowStepResult::measureVersionModified() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureVersionModified();
}

boost::optional<std::string> WorkflowStepResult::measureXmlChecksum() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureXmlChecksum();
}

boost::optional<std::string> WorkflowStepResult::measureClassName() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureClassName();
}

boost::optional<std::string> WorkflowStepResult::measureDisplayName() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureDisplayName();
}

boost::optional<std::string> WorkflowStepResult::measureTaxonomy() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->measureTaxonomy();
}

boost::optional<StepResult> WorkflowStepResult::stepResult() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepResult();
}

boost::optional<std::string> WorkflowStepResult::stepInitialCondition() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepInitialCondition();
}

boost::optional<std::string> WorkflowStepResult::stepFinalCondition() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepFinalCondition();
}

std::vector<std::string> WorkflowStepResult::stepErrors() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepErrors();
}

std::vector<std::string> WorkflowStepResult::stepWarnings() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepWarnings();
}

std::vector<std::string> WorkflowStepResult::stepInfo() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepInfo();
}

std::vector<WorkflowStepValue> WorkflowStepResult::stepValues() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepValues();
}

std::vector<openstudio::path> WorkflowStepResult::stepFiles() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stepFiles();
}

boost::optional<std::string> WorkflowStepResult::stdOut() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stdOut();
}

boost::optional<std::string> WorkflowStepResult::stdErr() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->stdErr();
}

StepResult WorkflowStepResult::value() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->value();
}

std::vector<LogMessage> WorkflowStepResult::errors() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->errors();
}

std::vector<LogMessage> WorkflowStepResult::warnings() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->warnings();
}

std::vector<LogMessage> WorkflowStepResult::info() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->info();
}

boost::optional<LogMessage> WorkflowStepResult::initialCondition() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->initialCondition();
}

boost::optional<LogMessage> WorkflowStepResult::finalCondition() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->finalCondition();
}

std::vector<Attribute> WorkflowStepResult::attributes() const {
  return getImpl<detail::WorkflowStepResult_Impl>()->attributes();
}

void WorkflowStepResult::setStartedAt(const DateTime& dateTime) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStartedAt(dateTime);
}

void WorkflowStepResult::resetStartedAt() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStartedAt();
}

void WorkflowStepResult::setCompletedAt(const DateTime& dateTime) {
  getImpl<detail::WorkflowStepResult_Impl>()->setCompletedAt(dateTime);
}

void WorkflowStepResult::resetCompletedAt() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetCompletedAt();
}

bool WorkflowStepResult::setMeasureType(const MeasureType& measureType) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureType(measureType);
}

void WorkflowStepResult::resetMeasureType() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureType();
}

bool WorkflowStepResult::setMeasureName(const std::string& name) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureName(name);
}

void WorkflowStepResult::resetMeasureName() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureName();
}

bool WorkflowStepResult::setMeasureId(const std::string& id) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureId(id);
}

void WorkflowStepResult::resetMeasureId() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureId();
}

bool WorkflowStepResult::setMeasureUUID(const UUID& uuid) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureUUID(uuid);
}

void WorkflowStepResult::resetMeasureUUID() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureUUID();
}

bool WorkflowStepResult::setMeasureVersionId(const std::string& id) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureVersionId(id);
}

void WorkflowStepResult::resetMeasureVersionId() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureVersionId();
}

bool WorkflowStepResult::setMeasureVersionUUID(const UUID& uuid) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureVersionUUID(uuid);
}

void WorkflowStepResult::resetMeasureVersionUUID() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureVersionUUID();
}

bool WorkflowStepResult::setMeasureVersionModified(const DateTime& versionModified) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureVersionModified(versionModified);
}

void WorkflowStepResult::resetMeasureVersionModified() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureVersionModified();
}

bool WorkflowStepResult::setMeasureXmlChecksum(const std::string& checksum) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureXmlChecksum(checksum);
}

void WorkflowStepResult::resetMeasureXmlChecksum() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureXmlChecksum();
}

bool WorkflowStepResult::setMeasureClassName(const std::string& className) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureClassName(className);
}

void WorkflowStepResult::resetMeasureClassName() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureClassName();
}

bool WorkflowStepResult::setMeasureDisplayName(const std::string& displayName) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureDisplayName(displayName);
}

void WorkflowStepResult::resetMeasureDisplayName() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureDisplayName();
}

bool WorkflowStepResult::setMeasureTaxonomy(const std::string& taxonomy) {
  return getImpl<detail::WorkflowStepResult_Impl>()->setMeasureTaxonomy(taxonomy);
}

void WorkflowStepResult::resetMeasureTaxonomy() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetMeasureTaxonomy();
}

void WorkflowStepResult::setStepResult(const StepResult& result) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStepResult(result);
}

void WorkflowStepResult::resetStepResult() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepResult();
}

void WorkflowStepResult::setStepInitialCondition(const std::string& initialCondition) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStepInitialCondition(initialCondition);
}

void WorkflowStepResult::resetStepInitialCondition() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepInitialCondition();
}

void WorkflowStepResult::setStepFinalCondition(const std::string& finalCondition) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStepFinalCondition(finalCondition);
}

void WorkflowStepResult::resetStepFinalCondition() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepFinalCondition();
}

void WorkflowStepResult::addStepError(const std::string& error) {
  getImpl<detail::WorkflowStepResult_Impl>()->addStepError(error);
}

void WorkflowStepResult::resetStepErrors() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepErrors();
}

void WorkflowStepResult::addStepWarning(const std::string& warning) {
  getImpl<detail::WorkflowStepResult_Impl>()->addStepWarning(warning);
}

void WorkflowStepResult::resetStepWarnings() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepWarnings();
}

void WorkflowStepResult::addStepInfo(const std::string& info) {
  getImpl<detail::WorkflowStepResult_Impl>()->addStepInfo(info);
}

void WorkflowStepResult::resetStepInfo() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepInfo();
}

void WorkflowStepResult::addStepValue(const WorkflowStepValue& value) {
  getImpl<detail::WorkflowStepResult_Impl>()->addStepValue(value);
}

void WorkflowStepResult::resetStepValues() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepValues();
}

void WorkflowStepResult::addStepFile(const openstudio::path& path) {
  getImpl<detail::WorkflowStepResult_Impl>()->addStepFile(path);
}

void WorkflowStepResult::resetStepFiles() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStepFiles();
}

void WorkflowStepResult::setStdOut(const std::string& stdOut) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStdOut(stdOut);
}

void WorkflowStepResult::resetStdOut() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStdOut();
}

void WorkflowStepResult::setStdErr(const std::string& stdErr) {
  getImpl<detail::WorkflowStepResult_Impl>()->setStdErr(stdErr);
}

void WorkflowStepResult::resetStdErr() {
  getImpl<detail::WorkflowStepResult_Impl>()->resetStdErr();
}

std::ostream& operator<<(std::ostream& os, const WorkflowStepResult& workflowStepResult) {
  os << workflowStepResult.string();
  return os;
}

}  // namespace openstudio
