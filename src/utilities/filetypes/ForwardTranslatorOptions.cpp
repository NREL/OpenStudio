/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ForwardTranslatorOptions.hpp"
#include "ForwardTranslatorOptions_Impl.hpp"

#include <json/json.h>

#include <memory>
#include <string>

namespace openstudio {
namespace detail {

  void ForwardTranslatorOptions_Impl::reset() {
    m_runcontrolspecialdays = DEFAULT_RUNCONTROLSPECIALDAYS;
    m_is_runcontrolspecialdays_defaulted = true;

    m_ip_tabular_output = DEFAULT_IP_TABULAR_OUTPUT;
    m_is_ip_tabular_output_defaulted = true;

    m_no_lifecyclecosts = DEFAULT_NO_LIFECYCLECOSTS;
    m_is_no_lifecyclecosts_defaulted = true;

    m_no_sqlite_output = DEFAULT_NO_SQLITE_OUTPUT;
    m_is_no_sqlite_output_defaulted = true;

    m_no_html_output = DEFAULT_NO_HTML_OUTPUT;
    m_is_no_html_output_defaulted = true;

    m_no_variable_dictionary = DEFAULT_NO_VARIABLE_DICTIONARY;
    m_is_no_variable_dictionary_defaulted = true;

    m_no_space_translation = DEFAULT_NO_SPACE_TRANSLATION;
    m_is_no_space_translation_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::keepRunControlSpecialDays() const {
    return m_runcontrolspecialdays;
  }

  bool ForwardTranslatorOptions_Impl::isKeepRunControlSpecialDaysDefaulted() const {
    return m_is_runcontrolspecialdays_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays) {
    m_runcontrolspecialdays = keepRunControlSpecialDays;
    m_is_runcontrolspecialdays_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetKeepRunControlSpecialDays() {
    m_runcontrolspecialdays = DEFAULT_RUNCONTROLSPECIALDAYS;
    m_is_runcontrolspecialdays_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::iPTabularOutput() const {
    return m_ip_tabular_output;
  }

  bool ForwardTranslatorOptions_Impl::isIPTabularOutputDefaulted() const {
    return m_is_ip_tabular_output_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setIPTabularOutput(bool iPTabularOutput) {
    m_ip_tabular_output = iPTabularOutput;
    m_is_ip_tabular_output_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetIPTabularOutput() {
    m_ip_tabular_output = DEFAULT_IP_TABULAR_OUTPUT;
    m_is_ip_tabular_output_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::excludeLCCObjects() const {
    return m_no_lifecyclecosts;
  }

  bool ForwardTranslatorOptions_Impl::isExcludeLCCObjectsDefaulted() const {
    return m_is_no_lifecyclecosts_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setExcludeLCCObjects(bool excludeLCCObjects) {
    m_no_lifecyclecosts = excludeLCCObjects;
    m_is_no_lifecyclecosts_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetExcludeLCCObjects() {
    m_no_lifecyclecosts = DEFAULT_NO_LIFECYCLECOSTS;
    m_is_no_lifecyclecosts_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::excludeSQliteOutputReport() const {
    return m_no_sqlite_output;
  }

  bool ForwardTranslatorOptions_Impl::isExcludeSQliteOutputReportDefaulted() const {
    return m_is_no_sqlite_output_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setExcludeSQliteOutputReport(bool excludeSQliteOutputReport) {
    m_no_sqlite_output = excludeSQliteOutputReport;
    m_is_no_sqlite_output_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetExcludeSQliteOutputReport() {
    m_no_sqlite_output = DEFAULT_NO_SQLITE_OUTPUT;
    m_is_no_sqlite_output_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::excludeHTMLOutputReport() const {
    return m_no_html_output;
  }

  bool ForwardTranslatorOptions_Impl::isExcludeHTMLOutputReportDefaulted() const {
    return m_is_no_html_output_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setExcludeHTMLOutputReport(bool excludeHTMLOutputReport) {
    m_no_html_output = excludeHTMLOutputReport;
    m_is_no_html_output_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetExcludeHTMLOutputReport() {
    m_no_html_output = DEFAULT_NO_HTML_OUTPUT;
    m_is_no_html_output_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::excludeVariableDictionary() const {
    return m_no_variable_dictionary;
  }

  bool ForwardTranslatorOptions_Impl::isExcludeVariableDictionaryDefaulted() const {
    return m_is_no_variable_dictionary_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setExcludeVariableDictionary(bool excludeVariableDictionary) {
    m_no_variable_dictionary = excludeVariableDictionary;
    m_is_no_variable_dictionary_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetExcludeVariableDictionary() {
    m_no_variable_dictionary = DEFAULT_NO_VARIABLE_DICTIONARY;
    m_is_no_variable_dictionary_defaulted = true;
  }

  bool ForwardTranslatorOptions_Impl::excludeSpaceTranslation() const {
    return m_no_space_translation;
  }

  bool ForwardTranslatorOptions_Impl::isExcludeSpaceTranslationDefaulted() const {
    return m_is_no_space_translation_defaulted;
  }

  void ForwardTranslatorOptions_Impl::setExcludeSpaceTranslation(bool excludeSpaceTranslation) {
    m_no_space_translation = excludeSpaceTranslation;
    m_is_no_space_translation_defaulted = false;
  }

  void ForwardTranslatorOptions_Impl::resetExcludeSpaceTranslation() {
    m_no_space_translation = DEFAULT_NO_SPACE_TRANSLATION;
    m_is_no_space_translation_defaulted = true;
  }

  void ForwardTranslatorOptions_Impl::overrideValuesWith(const ForwardTranslatorOptions& other) {
    if (!other.isKeepRunControlSpecialDaysDefaulted()) {
      setKeepRunControlSpecialDays(other.keepRunControlSpecialDays());
    }

    if (!other.isIPTabularOutputDefaulted()) {
      setIPTabularOutput(other.iPTabularOutput());
    }

    if (!other.isExcludeLCCObjectsDefaulted()) {
      setExcludeLCCObjects(other.excludeLCCObjects());
    }

    if (!other.isExcludeSQliteOutputReportDefaulted()) {
      setExcludeSQliteOutputReport(other.excludeSQliteOutputReport());
    }

    if (!other.isExcludeHTMLOutputReportDefaulted()) {
      setExcludeHTMLOutputReport(other.excludeHTMLOutputReport());
    }

    if (!other.isExcludeVariableDictionaryDefaulted()) {
      setExcludeVariableDictionary(other.excludeVariableDictionary());
    }

    if (!other.isExcludeSpaceTranslationDefaulted()) {
      setExcludeSpaceTranslation(other.excludeSpaceTranslation());
    }
  }

  Json::Value ForwardTranslatorOptions_Impl::json() const {
    Json::Value value;

    if (!m_is_runcontrolspecialdays_defaulted) {
      value["runcontrolspecialdays"] = m_runcontrolspecialdays;
    }

    if (!m_is_ip_tabular_output_defaulted) {
      value["ip_tabular_output"] = m_ip_tabular_output;
    }

    if (!m_is_no_lifecyclecosts_defaulted) {
      value["no_lifecyclecosts"] = m_no_lifecyclecosts;
    }

    if (!m_is_no_sqlite_output_defaulted) {
      value["no_sqlite_output"] = m_no_sqlite_output;
    }

    if (!m_is_no_html_output_defaulted) {
      value["no_html_output"] = m_no_html_output;
    }

    if (!m_is_no_variable_dictionary_defaulted) {
      value["no_variable_dictionary"] = m_no_variable_dictionary;
    }

    if (!m_is_no_space_translation_defaulted) {
      value["no_space_translation"] = m_no_space_translation;
    }

    return value;
  }

  std::string ForwardTranslatorOptions_Impl::string() const {

    const Json::Value data = this->json();
    if (data.isNull()) {
      return "";
    }
    // Write to string
    Json::StreamWriterBuilder wbuilder;
    // mimic the old StyledWriter behavior:
    wbuilder["indentation"] = "   ";
    std::string result = Json::writeString(wbuilder, data);

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

bool ForwardTranslatorOptions::isKeepRunControlSpecialDaysDefaulted() const {
  return m_impl->isKeepRunControlSpecialDaysDefaulted();
}

bool ForwardTranslatorOptions::keepRunControlSpecialDays() const {
  return m_impl->keepRunControlSpecialDays();
}

void ForwardTranslatorOptions::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays) {
  m_impl->setKeepRunControlSpecialDays(keepRunControlSpecialDays);
}

void ForwardTranslatorOptions::resetKeepRunControlSpecialDays() {
  m_impl->resetKeepRunControlSpecialDays();
}

bool ForwardTranslatorOptions::isIPTabularOutputDefaulted() const {
  return m_impl->isIPTabularOutputDefaulted();
}

bool ForwardTranslatorOptions::iPTabularOutput() const {
  return m_impl->iPTabularOutput();
}

void ForwardTranslatorOptions::setIPTabularOutput(bool iPTabularOutput) {
  m_impl->setIPTabularOutput(iPTabularOutput);
}

void ForwardTranslatorOptions::resetIPTabularOutput() {
  m_impl->resetIPTabularOutput();
}

bool ForwardTranslatorOptions::isExcludeLCCObjectsDefaulted() const {
  return m_impl->isExcludeLCCObjectsDefaulted();
}

bool ForwardTranslatorOptions::excludeLCCObjects() const {
  return m_impl->excludeLCCObjects();
}

void ForwardTranslatorOptions::setExcludeLCCObjects(bool excludeLCCObjects) {
  m_impl->setExcludeLCCObjects(excludeLCCObjects);
}

void ForwardTranslatorOptions::resetExcludeLCCObjects() {
  m_impl->resetExcludeLCCObjects();
}

bool ForwardTranslatorOptions::isExcludeSQliteOutputReportDefaulted() const {
  return m_impl->isExcludeSQliteOutputReportDefaulted();
}

bool ForwardTranslatorOptions::excludeSQliteOutputReport() const {
  return m_impl->excludeSQliteOutputReport();
}

void ForwardTranslatorOptions::setExcludeSQliteOutputReport(bool excludeSQliteOutputReport) {
  m_impl->setExcludeSQliteOutputReport(excludeSQliteOutputReport);
}

void ForwardTranslatorOptions::resetExcludeSQliteOutputReport() {
  m_impl->resetExcludeSQliteOutputReport();
}

bool ForwardTranslatorOptions::isExcludeHTMLOutputReportDefaulted() const {
  return m_impl->isExcludeHTMLOutputReportDefaulted();
}

bool ForwardTranslatorOptions::excludeHTMLOutputReport() const {
  return m_impl->excludeHTMLOutputReport();
}

void ForwardTranslatorOptions::setExcludeHTMLOutputReport(bool excludeHTMLOutputReport) {
  m_impl->setExcludeHTMLOutputReport(excludeHTMLOutputReport);
}

void ForwardTranslatorOptions::resetExcludeHTMLOutputReport() {
  m_impl->resetExcludeHTMLOutputReport();
}

bool ForwardTranslatorOptions::isExcludeVariableDictionaryDefaulted() const {
  return m_impl->isExcludeVariableDictionaryDefaulted();
}

bool ForwardTranslatorOptions::excludeVariableDictionary() const {
  return m_impl->excludeVariableDictionary();
}

void ForwardTranslatorOptions::setExcludeVariableDictionary(bool excludeVariableDictionary) {
  m_impl->setExcludeVariableDictionary(excludeVariableDictionary);
}

void ForwardTranslatorOptions::resetExcludeVariableDictionary() {
  m_impl->resetExcludeVariableDictionary();
}

bool ForwardTranslatorOptions::isExcludeSpaceTranslationDefaulted() const {
  return m_impl->isExcludeSpaceTranslationDefaulted();
}

bool ForwardTranslatorOptions::excludeSpaceTranslation() const {
  return m_impl->excludeSpaceTranslation();
}

void ForwardTranslatorOptions::setExcludeSpaceTranslation(bool excludeSpaceTranslation) {
  m_impl->setExcludeSpaceTranslation(excludeSpaceTranslation);
}

void ForwardTranslatorOptions::resetExcludeSpaceTranslation() {
  m_impl->resetExcludeSpaceTranslation();
}

void ForwardTranslatorOptions::overrideValuesWith(const ForwardTranslatorOptions& other) {
  m_impl->overrideValuesWith(other);
}

std::ostream& operator<<(std::ostream& os, const ForwardTranslatorOptions& forwardTranslatorOptions) {
  os << forwardTranslatorOptions.string();
  return os;
}

std::vector<ForwardTranslatorOptionKeyMethod> ForwardTranslatorOptions::forwardTranslatorOptionKeyMethods() {
  return std::vector<ForwardTranslatorOptionKeyMethod>{{{"runcontrolspecialdays", "setKeepRunControlSpecialDays"},
                                                        {"ip_tabular_output", "setIPTabularOutput"},
                                                        {"no_lifecyclecosts", "setExcludeLCCObjects"},
                                                        {"no_sqlite_output", "setExcludeSQliteOutputReport"},
                                                        {"no_html_output", "setExcludeHTMLOutputReport"},
                                                        {"no_variable_dictionary", "setExcludeVariableDictionary"},
                                                        {"no_space_translation", "setExcludeSpaceTranslation"}}};
}

std::ostream& operator<<(std::ostream& out, const ForwardTranslatorOptionKeyMethod& opt) {
  out << "(" << opt.json_name << ", " << opt.ft_method_name << ")";
  return out;
}

}  // namespace openstudio
