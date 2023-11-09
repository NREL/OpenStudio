/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RunOptions.hpp"
#include "RunOptions_Impl.hpp"
#include "ForwardTranslatorOptions.hpp"

#include "../core/Assert.hpp"
#include "../core/DeprecatedHelpers.hpp"

#include <json/json.h>

#include <json/value.h>
#include <memory>
#include <string>

namespace openstudio {
namespace detail {

  void RunOptions_Impl::onUpdate() {
    this->onChange.nano_emit();
  }

  Json::Value RunOptions_Impl::toJSON() const {
    Json::Value root;

    if (!m_is_debug_defaulted) {
      root["debug"] = m_debug;
    }

    if (!m_is_epjson_defaulted) {
      root["epjson"] = m_epjson;
    }

    if (!m_is_fast_defaulted) {
      root["fast"] = m_fast;
    }

    if (!m_is_preserveRunDir_defaulted) {
      root["preserve_run_dir"] = m_preserveRunDir;
    }

    if (!m_is_skipExpandObjects_defaulted) {
      root["skip_expand_objects"] = m_skipExpandObjects;
    }

    if (!m_is_skipEnergyPlusPreprocess_defaulted) {
      root["skip_energyplus_preprocess"] = m_skipEnergyPlusPreprocess;
    }

    if (!m_is_cleanup_defaulted) {
      root["cleanup"] = m_cleanup;
    }

    if (m_customOutputAdapter) {
      root["output_adapter"] = m_customOutputAdapter->toJSON();
    }

    if (auto ft_opt = m_forwardTranslatorOptions.toJSON()) {
      root["ft_options"] = ft_opt;
    }
    return root;
  }

  std::string RunOptions_Impl::string() const {
    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, toJSON());

    return result;
  }

  bool RunOptions_Impl::debug() const {
    return m_debug;
  }

  bool RunOptions_Impl::isDebugDefaulted() const {
    return m_is_debug_defaulted;
  }

  bool RunOptions_Impl::setDebug(bool debug) {
    m_debug = debug;
    m_is_debug_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetDebug() {
    m_debug = DEFAULT_DEBUG;
    m_is_debug_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::epjson() const {
    return m_epjson;
  }

  bool RunOptions_Impl::isEpjsonDefaulted() const {
    return m_is_epjson_defaulted;
  }

  bool RunOptions_Impl::setEpjson(bool epjson) {
    m_epjson = epjson;
    m_is_epjson_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetEpjson() {
    m_epjson = DEFAULT_EPJSON;
    m_is_epjson_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::fast() const {
    return m_fast;
  }

  bool RunOptions_Impl::isFastDefaulted() const {
    return m_is_fast_defaulted;
  }

  bool RunOptions_Impl::setFast(bool fast) {
    m_fast = fast;
    m_is_fast_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetFast() {
    m_fast = DEFAULT_FAST;
    m_is_fast_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::preserveRunDir() const {
    return m_preserveRunDir;
  }

  bool RunOptions_Impl::isPreserveRunDirDefaulted() const {
    return m_is_preserveRunDir_defaulted;
  }

  bool RunOptions_Impl::setPreserveRunDir(bool preserveRunDir) {
    m_preserveRunDir = preserveRunDir;
    m_is_preserveRunDir_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetPreserveRunDir() {
    m_preserveRunDir = DEFAULT_PRESERVERUNDIR;
    m_is_preserveRunDir_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::skipExpandObjects() const {
    return m_skipExpandObjects;
  }

  bool RunOptions_Impl::isSkipExpandObjectsDefaulted() const {
    return m_is_skipExpandObjects_defaulted;
  }

  bool RunOptions_Impl::setSkipExpandObjects(bool skipExpandObjects) {
    m_skipExpandObjects = skipExpandObjects;
    m_is_skipExpandObjects_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetSkipExpandObjects() {
    m_skipExpandObjects = DEFAULT_SKIPEXPANDOBJECTS;
    m_is_skipExpandObjects_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::skipEnergyPlusPreprocess() const {
    return m_skipEnergyPlusPreprocess;
  }

  bool RunOptions_Impl::isSkipEnergyPlusPreprocessDefaulted() const {
    return m_is_skipEnergyPlusPreprocess_defaulted;
  }

  bool RunOptions_Impl::setSkipEnergyPlusPreprocess(bool skipEnergyPlusPreprocess) {
    m_skipEnergyPlusPreprocess = skipEnergyPlusPreprocess;
    m_is_skipEnergyPlusPreprocess_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetSkipEnergyPlusPreprocess() {
    m_skipEnergyPlusPreprocess = DEFAULT_SKIPENERGYPLUSPREPROCESS;
    m_is_skipEnergyPlusPreprocess_defaulted = true;
    onUpdate();
  }

  bool RunOptions_Impl::cleanup() const {
    return m_cleanup;
  }

  bool RunOptions_Impl::isCleanupDefaulted() const {
    return m_is_cleanup_defaulted;
  }

  bool RunOptions_Impl::setCleanup(bool cleanup) {
    m_cleanup = cleanup;
    m_is_cleanup_defaulted = false;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetCleanup() {
    m_cleanup = DEFAULT_CLEANUP;
    m_is_cleanup_defaulted = true;
    onUpdate();
  }

  boost::optional<CustomOutputAdapter> RunOptions_Impl::customOutputAdapter() const {
    return m_customOutputAdapter;
  }

  bool RunOptions_Impl::setCustomOutputAdapter(const CustomOutputAdapter& adapter) {
    m_customOutputAdapter = adapter;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetCustomOutputAdapter() {
    m_customOutputAdapter.reset();
    onUpdate();
  }

  ForwardTranslatorOptions RunOptions_Impl::forwardTranslatorOptions() const {
    return m_forwardTranslatorOptions;
  }

  bool RunOptions_Impl::setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions) {
    m_forwardTranslatorOptions = ForwardTranslatorOptions(forwardTranslatorOptions.getImpl());
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetForwardTranslatorOptions() {
    m_forwardTranslatorOptions.reset();
    onUpdate();
  }

  void RunOptions_Impl::overrideValuesWith(const RunOptions& other) {

    if (!other.isDebugDefaulted()) {
      setDebug(other.debug());
    }

    if (!other.isEpjsonDefaulted()) {
      setEpjson(other.epjson());
    }

    if (!other.isFastDefaulted()) {
      setFast(other.fast());
    }

    if (!other.isPreserveRunDirDefaulted()) {
      setPreserveRunDir(other.preserveRunDir());
    }

    if (!other.isSkipExpandObjectsDefaulted()) {
      setSkipExpandObjects(other.skipExpandObjects());
    }

    if (!other.isSkipEnergyPlusPreprocessDefaulted()) {
      setSkipEnergyPlusPreprocess(other.skipEnergyPlusPreprocess());
    }

    if (!other.isCleanupDefaulted()) {
      setCleanup(other.cleanup());
    }

    m_forwardTranslatorOptions.overrideValuesWith(other.forwardTranslatorOptions());
  }

}  // namespace detail

CustomOutputAdapter CustomOutputAdapter::fromString(std::string customFileName, std::string className, const std::string& options) {

  if (options.empty()) {
    return CustomOutputAdapter(std::move(customFileName), std::move(className));
  }
  const Json::CharReaderBuilder rbuilder;
  std::istringstream ss(options);
  std::string formattedErrors;
  Json::Value parsedOptions;
  const bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &parsedOptions, &formattedErrors);
  if (!parsingSuccessful) {
    parsedOptions = Json::Value(Json::nullValue);
  }
  return CustomOutputAdapter(std::move(customFileName), std::move(className), std::move(parsedOptions));
}

CustomOutputAdapter::CustomOutputAdapter(std::string customFileName, std::string className, Json::Value options)
  : m_customFileName(std::move(customFileName)), m_className(std::move(className)), m_options(std::move(options)) {}

std::string CustomOutputAdapter::customFileName() const {
  return m_customFileName;
}

std::string CustomOutputAdapter::className() const {
  return m_className;
}

std::string CustomOutputAdapter::options() const {
  if (m_options.isNull()) {
    return "";
  }

  // Write to string
  Json::StreamWriterBuilder wbuilder;
  // mimic the old StyledWriter behavior:
  wbuilder["indentation"] = "   ";
  return Json::writeString(wbuilder, m_options);
}

Json::Value CustomOutputAdapter::optionsJSON() const {
  return m_options;
}

Json::Value CustomOutputAdapter::toJSON() const {
  Json::Value outputAdapter;
  outputAdapter["custom_file_name"] = m_customFileName;
  outputAdapter["class_name"] = m_className;
  outputAdapter["options"] = m_options;
  return outputAdapter;
}

RunOptions::RunOptions() : m_impl(std::make_shared<detail::RunOptions_Impl>()) {
  OS_ASSERT(getImpl<detail::RunOptions_Impl>());
}

RunOptions::~RunOptions() = default;

boost::optional<RunOptions> RunOptions::fromString(const std::string& s) {

  // We let it fail with a warning message
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  Json::Value value;
  bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
  if (!parsingSuccessful) {
    LOG(Warn, "Couldn't parse RunOptions from string s='" << s << "'. Error: '" << formattedErrors << "'.");
    return boost::none;
  }

  RunOptions result;

  if (value.isMember("epjson") && value["epjson"].isBool()) {
    result.setEpjson(value["epjson"].asBool());
  }

  if (value.isMember("debug") && value["debug"].isBool()) {
    result.setDebug(value["debug"].asBool());
  }

  if (value.isMember("fast") && value["fast"].isBool()) {
    result.setFast(value["fast"].asBool());
  }

  if (value.isMember("preserve_run_dir") && value["preserve_run_dir"].isBool()) {
    result.setPreserveRunDir(value["preserve_run_dir"].asBool());
  }

  if (value.isMember("skip_expand_objects") && value["skip_expand_objects"].isBool()) {
    result.setSkipExpandObjects(value["skip_expand_objects"].asBool());
  }

  if (value.isMember("skip_energyplus_preprocess") && value["skip_energyplus_preprocess"].isBool()) {
    result.setSkipEnergyPlusPreprocess(value["skip_energyplus_preprocess"].asBool());
  }

  if (value.isMember("output_adapter")) {
    Json::Value outputAdapter = value["output_adapter"];
    if (outputAdapter.isMember("custom_file_name") && outputAdapter.isMember("class_name")) {
      std::string customFileName = outputAdapter["custom_file_name"].asString();
      std::string className = outputAdapter["class_name"].asString();
      Json::Value options = outputAdapter["options"];
      const CustomOutputAdapter coa(std::move(customFileName), std::move(className), std::move(options));
      result.setCustomOutputAdapter(coa);
    }
  }

  if (value.isMember("ft_options") && value["ft_options"].isObject()) {
    result.setForwardTranslatorOptions(ForwardTranslatorOptions::fromJSON(value["ft_options"]));
  }

  return result;
}

Json::Value RunOptions::toJSON() const {
  return getImpl<detail::RunOptions_Impl>()->toJSON();
}

std::string RunOptions::string() const {
  return getImpl<detail::RunOptions_Impl>()->string();
}

bool RunOptions::debug() const {
  return getImpl<detail::RunOptions_Impl>()->debug();
}

bool RunOptions::isDebugDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isDebugDefaulted();
}

bool RunOptions::setDebug(bool debug) {
  return getImpl<detail::RunOptions_Impl>()->setDebug(debug);
}

void RunOptions::resetDebug() {
  getImpl<detail::RunOptions_Impl>()->resetDebug();
}

bool RunOptions::epjson() const {
  return getImpl<detail::RunOptions_Impl>()->epjson();
}

bool RunOptions::isEpjsonDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isEpjsonDefaulted();
}

bool RunOptions::setEpjson(bool epjson) {
  return getImpl<detail::RunOptions_Impl>()->setEpjson(epjson);
}

void RunOptions::resetEpjson() {
  getImpl<detail::RunOptions_Impl>()->resetEpjson();
}

bool RunOptions::fast() const {
  return getImpl<detail::RunOptions_Impl>()->fast();
}

bool RunOptions::isFastDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isFastDefaulted();
}

bool RunOptions::setFast(bool fast) {
  return getImpl<detail::RunOptions_Impl>()->setFast(fast);
}

void RunOptions::resetFast() {
  getImpl<detail::RunOptions_Impl>()->resetFast();
}

bool RunOptions::preserveRunDir() const {
  return getImpl<detail::RunOptions_Impl>()->preserveRunDir();
}

bool RunOptions::isPreserveRunDirDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isPreserveRunDirDefaulted();
}

bool RunOptions::setPreserveRunDir(bool preserveRunDir) {
  return getImpl<detail::RunOptions_Impl>()->setPreserveRunDir(preserveRunDir);
}

void RunOptions::resetPreserveRunDir() {
  getImpl<detail::RunOptions_Impl>()->resetPreserveRunDir();
}

bool RunOptions::skipExpandObjects() const {
  return getImpl<detail::RunOptions_Impl>()->skipExpandObjects();
}

bool RunOptions::isSkipExpandObjectsDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isSkipExpandObjectsDefaulted();
}

bool RunOptions::setSkipExpandObjects(bool skipExpandObjects) {
  return getImpl<detail::RunOptions_Impl>()->setSkipExpandObjects(skipExpandObjects);
}

void RunOptions::resetSkipExpandObjects() {
  getImpl<detail::RunOptions_Impl>()->resetSkipExpandObjects();
}

bool RunOptions::skipEnergyPlusPreprocess() const {
  return getImpl<detail::RunOptions_Impl>()->skipEnergyPlusPreprocess();
}

bool RunOptions::isSkipEnergyPlusPreprocessDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isSkipEnergyPlusPreprocessDefaulted();
}

bool RunOptions::setSkipEnergyPlusPreprocess(bool skipEnergyPlusPreprocess) {
  return getImpl<detail::RunOptions_Impl>()->setSkipEnergyPlusPreprocess(skipEnergyPlusPreprocess);
}

void RunOptions::resetSkipEnergyPlusPreprocess() {
  getImpl<detail::RunOptions_Impl>()->resetSkipEnergyPlusPreprocess();
}

bool RunOptions::cleanup() const {
  return getImpl<detail::RunOptions_Impl>()->cleanup();
}

bool RunOptions::isCleanupDefaulted() const {
  return getImpl<detail::RunOptions_Impl>()->isCleanupDefaulted();
}

bool RunOptions::setCleanup(bool cleanup) {
  return getImpl<detail::RunOptions_Impl>()->setCleanup(cleanup);
}

void RunOptions::resetCleanup() {
  getImpl<detail::RunOptions_Impl>()->resetCleanup();
}
boost::optional<CustomOutputAdapter> RunOptions::customOutputAdapter() const {
  return getImpl<detail::RunOptions_Impl>()->customOutputAdapter();
}

bool RunOptions::setCustomOutputAdapter(const CustomOutputAdapter& adapter) {
  return getImpl<detail::RunOptions_Impl>()->setCustomOutputAdapter(adapter);
}

void RunOptions::resetCustomOutputAdapter() {
  getImpl<detail::RunOptions_Impl>()->resetCustomOutputAdapter();
}

ForwardTranslatorOptions RunOptions::forwardTranslatorOptions() const {
  return getImpl<detail::RunOptions_Impl>()->forwardTranslatorOptions();
}

bool RunOptions::setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions) {
  return getImpl<detail::RunOptions_Impl>()->setForwardTranslatorOptions(forwardTranslatorOptions);
}

void RunOptions::resetForwardTranslatorOptions() {
  getImpl<detail::RunOptions_Impl>()->resetForwardTranslatorOptions();
}

void RunOptions::overrideValuesWith(const RunOptions& other) {
  getImpl<detail::RunOptions_Impl>()->overrideValuesWith(other);
}

/// @cond

// DEPRECATED
std::string RunOptions::forwardTranslateOptions() const {
  DEPRECATED_AT_MSG(3, 6, 0, "Use `ForwardTranslatorOptions RunOptions::forwardTranslatorOptions` instead.");
  return getImpl<detail::RunOptions_Impl>()->forwardTranslatorOptions().string();
}

bool RunOptions::setForwardTranslateOptions(const std::string& options) {
  DEPRECATED_AT_MSG(
    3, 6, 0,
    "Use `ForwardTranslatorOptions RunOptions::setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions)` instead.");

  if (auto ftOptions_ = ForwardTranslatorOptions::fromString(options)) {
    return getImpl<detail::RunOptions_Impl>()->setForwardTranslatorOptions(ftOptions_.get());
  }
  return false;
}

void RunOptions::resetForwardTranslateOptions() {
  DEPRECATED_AT_MSG(3, 6, 0, "Use resetForwardTranslateOptions instead.");
  getImpl<detail::RunOptions_Impl>()->resetForwardTranslatorOptions();
}

/// @endcond

std::ostream& operator<<(std::ostream& os, const RunOptions& runOptions) {
  os << runOptions.string();
  return os;
}

}  // namespace openstudio
