/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../core/Assert.hpp"

#include <json/json.h>

#include <memory>
#include <string>

namespace openstudio {
namespace detail {

  void ForwardTranslatorOptions_Impl::reset() {
    m_runcontrolspecialdays = true;
    m_ip_tabular_output = false;
    m_no_lifecyclecosts = false;
    m_no_sqlite_output = false;
    m_no_html_output = false;
    m_no_variable_dictionary = false;
    m_no_space_translation = false;
  }

  bool ForwardTranslatorOptions_Impl::keepRunControlSpecialDays() const {
    return m_runcontrolspecialdays;
  }

  void ForwardTranslatorOptions_Impl::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays) {
    m_runcontrolspecialdays = keepRunControlSpecialDays;
  }

  bool ForwardTranslatorOptions_Impl::iPTabularOutput() const {
    return m_ip_tabular_output;
  }

  void ForwardTranslatorOptions_Impl::setIPTabularOutput(bool iPTabularOutput) {
    m_ip_tabular_output = iPTabularOutput;
  }

  bool ForwardTranslatorOptions_Impl::excludeLCCObjects() const {
    return m_no_lifecyclecosts;
  }

  void ForwardTranslatorOptions_Impl::setExcludeLCCObjects(bool excludeLCCObjects) {
    m_no_lifecyclecosts = excludeLCCObjects;
  }

  bool ForwardTranslatorOptions_Impl::excludeSQliteOutputReport() const {
    return m_no_sqlite_output;
  }

  void ForwardTranslatorOptions_Impl::setExcludeSQliteOutputReport(bool excludeSQliteOutputReport) {
    m_no_sqlite_output = excludeSQliteOutputReport;
  }

  bool ForwardTranslatorOptions_Impl::excludeHTMLOutputReport() const {
    return m_no_html_output;
  }

  void ForwardTranslatorOptions_Impl::setExcludeHTMLOutputReport(bool excludeHTMLOutputReport) {
    m_no_html_output = excludeHTMLOutputReport;
  }

  bool ForwardTranslatorOptions_Impl::excludeVariableDictionary() const {
    return m_no_variable_dictionary;
  }

  void ForwardTranslatorOptions_Impl::setExcludeVariableDictionary(bool excludeVariableDictionary) {
    m_no_variable_dictionary = excludeVariableDictionary;
  }

  bool ForwardTranslatorOptions_Impl::excludeSpaceTranslation() const {
    return m_no_space_translation;
  }

  void ForwardTranslatorOptions_Impl::setExcludeSpaceTranslation(bool excludeSpaceTranslation) {
    m_no_space_translation = excludeSpaceTranslation;
  }

  Json::Value ForwardTranslatorOptions_Impl::json() const {
    Json::Value value;

    value["runcontrolspecialdays"] = m_runcontrolspecialdays;
    value["ip_tabular_output"] = m_ip_tabular_output;
    value["no_lifecyclecosts"] = m_no_lifecyclecosts;
    value["no_sqlite_output"] = m_no_sqlite_output;
    value["no_html_output"] = m_no_html_output;
    value["no_variable_dictionary"] = m_no_variable_dictionary;
    value["no_space_translation"] = m_no_space_translation;

    return value;
  }

  std::string ForwardTranslatorOptions_Impl::string() const {

    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, json());

    return result;
  }

  void RunOptions_Impl::onUpdate() {
    this->onChange.nano_emit();
  }

  std::string RunOptions_Impl::string() const {
    Json::Value value;

    // TODO: Why is this only writing if not default? I find it weird
    if (m_debug) {
      value["debug"] = m_debug;
    }

    if (m_epjson) {
      value["epjson"] = m_epjson;
    }

    if (m_fast) {
      value["fast"] = m_fast;
    }

    if (m_preserveRunDir) {
      value["preserve_run_dir"] = m_preserveRunDir;
    }

    if (m_skipExpandObjects) {
      value["skip_expand_objects"] = m_skipExpandObjects;
    }

    if (m_skipEnergyPlusPreprocess) {
      value["skip_energyplus_preprocess"] = m_skipEnergyPlusPreprocess;
    }

    if (m_customOutputAdapter) {
      Json::Value outputAdapter;
      outputAdapter["custom_file_name"] = m_customOutputAdapter->customFileName();
      outputAdapter["class_name"] = m_customOutputAdapter->className();

      Json::CharReaderBuilder rbuilder;
      std::istringstream ss(m_customOutputAdapter->options());
      std::string formattedErrors;
      Json::Value options;
      bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &options, &formattedErrors);
      if (parsingSuccessful) {
        outputAdapter["options"] = options;
      } else {
        LOG(Warn, "Couldn't parse CustomOutputAdapter's options='" << m_customOutputAdapter->options() << "'. Error: '" << formattedErrors << "'.");
      }

      value["output_adapter"] = outputAdapter;
    }

    value["ft_options"] = m_forwardTranslatorOptions.json();

    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, value);

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

CustomOutputAdapter::CustomOutputAdapter(const std::string& customFileName, const std::string& className, const std::string& options)
  : m_customFileName(customFileName), m_className(className), m_options(options) {}

std::string CustomOutputAdapter::customFileName() const {
  return m_customFileName;
}

std::string CustomOutputAdapter::className() const {
  return m_className;
}

std::string CustomOutputAdapter::options() const {
  return m_options;
}

ForwardTranslatorOptions::ForwardTranslatorOptions(std::shared_ptr<detail::ForwardTranslatorOptions_Impl> impl) : m_impl(std::move(impl)) {}
ForwardTranslatorOptions::ForwardTranslatorOptions() : m_impl(std::make_shared<detail::ForwardTranslatorOptions_Impl>()) {}

void ForwardTranslatorOptions::reset() {
  m_impl->reset();
}

boost::optional<ForwardTranslatorOptions> ForwardTranslatorOptions::fromString(const std::string& s) {

  // We let it fail with a warning message
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  Json::Value value;
  bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &value, &formattedErrors);
  if (!parsingSuccessful) {
    LOG(Warn, "Couldn't parse ForwardTranslatorOptions from string s='" << s << "'. Error: '" << formattedErrors << "'.");
    return boost::none;
  }

  return ForwardTranslatorOptions::fromJSON(value);
}

ForwardTranslatorOptions ForwardTranslatorOptions::fromJSON(const Json::Value& value) {

  ForwardTranslatorOptions result;

  if (value.isMember("runcontrolspecialdays") && value["runcontrolspecialdays"].isBool()) {
    result.setKeepRunControlSpecialDays(value["runcontrolspecialdays"].asBool());
  }
  if (value.isMember("ip_tabular_output") && value["ip_tabular_output"].isBool()) {
    result.setIPTabularOutput(value["ip_tabular_output"].asBool());
  }
  if (value.isMember("no_lifecyclecosts") && value["no_lifecyclecosts"].isBool()) {
    result.setExcludeLCCObjects(value["no_lifecyclecosts"].asBool());
  }
  if (value.isMember("no_sqlite_output") && value["no_sqlite_output"].isBool()) {
    result.setExcludeSQliteOutputReport(value["no_sqlite_output"].asBool());
  }
  if (value.isMember("no_html_output") && value["no_html_output"].isBool()) {
    result.setExcludeHTMLOutputReport(value["no_html_output"].asBool());
  }
  if (value.isMember("no_variable_dictionary") && value["no_variable_dictionary"].isBool()) {
    result.setExcludeVariableDictionary(value["no_variable_dictionary"].asBool());
  }
  if (value.isMember("no_space_translation") && value["no_space_translation"].isBool()) {
    result.setExcludeSpaceTranslation(value["no_space_translation"].asBool());
  }

  return result;
}

Json::Value ForwardTranslatorOptions::json() const {
  return m_impl->json();
}

std::string ForwardTranslatorOptions::string() const {
  return m_impl->string();
}

bool ForwardTranslatorOptions::keepRunControlSpecialDays() const {
  return m_impl->keepRunControlSpecialDays();
}

void ForwardTranslatorOptions::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays) {
  m_impl->setKeepRunControlSpecialDays(keepRunControlSpecialDays);
}

bool ForwardTranslatorOptions::iPTabularOutput() const {
  return m_impl->iPTabularOutput();
}

void ForwardTranslatorOptions::setIPTabularOutput(bool iPTabularOutput) {
  m_impl->setIPTabularOutput(iPTabularOutput);
}

bool ForwardTranslatorOptions::excludeLCCObjects() const {
  return m_impl->excludeLCCObjects();
}

void ForwardTranslatorOptions::setExcludeLCCObjects(bool excludeLCCObjects) {
  m_impl->setExcludeLCCObjects(excludeLCCObjects);
}

bool ForwardTranslatorOptions::excludeSQliteOutputReport() const {
  return m_impl->excludeSQliteOutputReport();
}

void ForwardTranslatorOptions::setExcludeSQliteOutputReport(bool excludeSQliteOutputReport) {
  m_impl->setExcludeSQliteOutputReport(excludeSQliteOutputReport);
}

bool ForwardTranslatorOptions::excludeHTMLOutputReport() const {
  return m_impl->excludeHTMLOutputReport();
}

void ForwardTranslatorOptions::setExcludeHTMLOutputReport(bool excludeHTMLOutputReport) {
  m_impl->setExcludeHTMLOutputReport(excludeHTMLOutputReport);
}

bool ForwardTranslatorOptions::excludeVariableDictionary() const {
  return m_impl->excludeVariableDictionary();
}

void ForwardTranslatorOptions::setExcludeVariableDictionary(bool excludeVariableDictionary) {
  m_impl->setExcludeVariableDictionary(excludeVariableDictionary);
}

bool ForwardTranslatorOptions::excludeSpaceTranslation() const {
  return m_impl->excludeSpaceTranslation();
}

void ForwardTranslatorOptions::setExcludeSpaceTranslation(bool excludeSpaceTranslation) {
  m_impl->setExcludeSpaceTranslation(excludeSpaceTranslation);
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

      Json::StreamWriterBuilder wbuilder;
      // mimic the old StyledWriter behavior:
      wbuilder["indentation"] = "   ";
      std::string optionString = Json::writeString(wbuilder, options);

      CustomOutputAdapter coa(customFileName, className, optionString);
      result.setCustomOutputAdapter(coa);
    }
  }

  if (value.isMember("ft_options") && value["ft_options"].isObject()) {
    result.setForwardTranslatorOptions(ForwardTranslatorOptions::fromJSON(value["ft_options"]));
  }

  return result;
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
