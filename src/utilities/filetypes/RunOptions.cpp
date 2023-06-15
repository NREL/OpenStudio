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

#include "RunOptions.hpp"
#include "RunOptions_Impl.hpp"
#include "ForwardTranslatorOptions.hpp"

#include "../core/Assert.hpp"

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

    // TODO: Why is this only writing if not default? I find it weird
    if (m_debug) {
      root["debug"] = m_debug;
    }

    if (m_epjson) {
      root["epjson"] = m_epjson;
    }

    if (m_fast) {
      root["fast"] = m_fast;
    }

    if (m_preserveRunDir) {
      root["preserve_run_dir"] = m_preserveRunDir;
    }

    if (m_skipExpandObjects) {
      root["skip_expand_objects"] = m_skipExpandObjects;
    }

    if (m_skipEnergyPlusPreprocess) {
      root["skip_energyplus_preprocess"] = m_skipEnergyPlusPreprocess;
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

  bool RunOptions_Impl::setDebug(bool debug) {
    m_debug = debug;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetDebug() {
    m_debug = false;
    onUpdate();
  }

  bool RunOptions_Impl::epjson() const {
    return m_epjson;
  }

  bool RunOptions_Impl::setEpjson(bool epjson) {
    m_epjson = epjson;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetEpjson() {
    m_epjson = false;
    onUpdate();
  }

  bool RunOptions_Impl::fast() const {
    return m_fast;
  }

  bool RunOptions_Impl::setFast(bool fast) {
    m_fast = fast;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetFast() {
    m_fast = false;
    onUpdate();
  }

  bool RunOptions_Impl::preserveRunDir() const {
    return m_preserveRunDir;
  }

  bool RunOptions_Impl::setPreserveRunDir(bool preserve) {
    m_preserveRunDir = preserve;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetPreserveRunDir() {
    m_preserveRunDir = false;
    onUpdate();
  }

  bool RunOptions_Impl::skipExpandObjects() const {
    return m_skipExpandObjects;
  }

  bool RunOptions_Impl::setSkipExpandObjects(bool skip) {
    m_skipExpandObjects = skip;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetSkipExpandObjects() {
    m_skipExpandObjects = false;
    onUpdate();
  }

  bool RunOptions_Impl::skipEnergyPlusPreprocess() const {
    return m_skipEnergyPlusPreprocess;
  }

  bool RunOptions_Impl::setSkipEnergyPlusPreprocess(bool skip) {
    m_skipEnergyPlusPreprocess = skip;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetSkipEnergyPlusPreprocess() {
    m_skipEnergyPlusPreprocess = false;
    onUpdate();
  }

  bool RunOptions_Impl::cleanup() const {
    return m_cleanup;
  }

  bool RunOptions_Impl::setCleanup(bool cleanup) {
    m_cleanup = cleanup;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetCleanup() {
    m_cleanup = true;
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

bool RunOptions::setDebug(bool debug) {
  return getImpl<detail::RunOptions_Impl>()->setDebug(debug);
}

void RunOptions::resetDebug() {
  getImpl<detail::RunOptions_Impl>()->resetDebug();
}

bool RunOptions::epjson() const {
  return getImpl<detail::RunOptions_Impl>()->epjson();
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

bool RunOptions::setFast(bool fast) {
  return getImpl<detail::RunOptions_Impl>()->setFast(fast);
}
void RunOptions::resetFast() {
  getImpl<detail::RunOptions_Impl>()->resetFast();
}

bool RunOptions::preserveRunDir() const {
  return getImpl<detail::RunOptions_Impl>()->preserveRunDir();
}

bool RunOptions::setPreserveRunDir(bool preserve) {
  return getImpl<detail::RunOptions_Impl>()->setPreserveRunDir(preserve);
}

void RunOptions::resetPreserveRunDir() {
  getImpl<detail::RunOptions_Impl>()->resetPreserveRunDir();
}

bool RunOptions::skipExpandObjects() const {
  return getImpl<detail::RunOptions_Impl>()->skipExpandObjects();
}

bool RunOptions::setSkipExpandObjects(bool skip) {
  return getImpl<detail::RunOptions_Impl>()->setSkipExpandObjects(skip);
}

void RunOptions::resetSkipExpandObjects() {
  getImpl<detail::RunOptions_Impl>()->resetSkipExpandObjects();
}

bool RunOptions::skipEnergyPlusPreprocess() const {
  return getImpl<detail::RunOptions_Impl>()->skipEnergyPlusPreprocess();
}

bool RunOptions::setSkipEnergyPlusPreprocess(bool skip) {
  return getImpl<detail::RunOptions_Impl>()->setSkipEnergyPlusPreprocess(skip);
}

void RunOptions::resetSkipEnergyPlusPreprocess() {
  getImpl<detail::RunOptions_Impl>()->resetSkipEnergyPlusPreprocess();
}

bool RunOptions::cleanup() const {
  return getImpl<detail::RunOptions_Impl>()->cleanup();
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

/// @cond

// DEPRECATED
std::string RunOptions::forwardTranslateOptions() const {
  LOG(Warn, "As of 3.5.1, (std::string) forwardTranslateOptions is deprecated. Use (ForwardTranslatorOptions) forwardTranslatorOptions instead. "
            "It will be removed within three releases.");
  return getImpl<detail::RunOptions_Impl>()->forwardTranslatorOptions().string();
}

bool RunOptions::setForwardTranslateOptions(const std::string& options) {
  LOG(Warn, "As of 3.5.1, setForwardTranslateOptions(std::string) is deprecated. Use setForwardTranslatorOptions(ForwardTranslatorOptions) instead. "
            "It will be removed within three releases.");
  if (auto ftOptions_ = ForwardTranslatorOptions::fromString(options)) {
    return getImpl<detail::RunOptions_Impl>()->setForwardTranslatorOptions(ftOptions_.get());
  }
  return false;
}

void RunOptions::resetForwardTranslateOptions() {
  LOG(Warn, "As of 3.5.1, resetForwardTranslateOptions is deprecated. Use resetForwardTranslateOptions instead. "
            "It will be removed within three releases.");
  getImpl<detail::RunOptions_Impl>()->resetForwardTranslatorOptions();
}

/// @endcond

std::ostream& operator<<(std::ostream& os, const RunOptions& runOptions) {
  os << runOptions.string();
  return os;
}

}  // namespace openstudio
