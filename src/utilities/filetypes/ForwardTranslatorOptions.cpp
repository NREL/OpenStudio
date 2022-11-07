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

#include "ForwardTranslatorOptions.hpp"
#include "ForwardTranslatorOptions_Impl.hpp"

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

}  // namespace detail

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

std::ostream& operator<<(std::ostream& os, const ForwardTranslatorOptions& forwardTranslatorOptions) {
  os << forwardTranslatorOptions.string();
  return os;
}

}  // namespace openstudio
