/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_IMPL_HPP
#define UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"

namespace Json {
class Value;
}

namespace openstudio {
class ForwardTranslatorOptions;

namespace detail {

  // TODO: is the nano onChange signal really needed for RunOptions?! If so, I need to implement one for ForwardTranslatorOptions too
  class UTILITIES_API ForwardTranslatorOptions_Impl
  {
   public:
    ForwardTranslatorOptions_Impl() = default;

    void reset();

    std::string string() const;
    Json::Value toJSON() const;

    bool keepRunControlSpecialDays() const;
    bool isKeepRunControlSpecialDaysDefaulted() const;
    void setKeepRunControlSpecialDays(bool keepRunControlSpecialDays);
    void resetKeepRunControlSpecialDays();

    bool iPTabularOutput() const;
    bool isIPTabularOutputDefaulted() const;
    void setIPTabularOutput(bool iPTabularOutput);
    void resetIPTabularOutput();

    bool excludeLCCObjects() const;
    bool isExcludeLCCObjectsDefaulted() const;
    void setExcludeLCCObjects(bool excludeLCCObjects);
    void resetExcludeLCCObjects();

    bool excludeSQliteOutputReport() const;
    bool isExcludeSQliteOutputReportDefaulted() const;
    void setExcludeSQliteOutputReport(bool excludeSQliteOutputReport);
    void resetExcludeSQliteOutputReport();

    bool excludeHTMLOutputReport() const;
    bool isExcludeHTMLOutputReportDefaulted() const;
    void setExcludeHTMLOutputReport(bool excludeHTMLOutputReport);
    void resetExcludeHTMLOutputReport();

    bool excludeVariableDictionary() const;
    bool isExcludeVariableDictionaryDefaulted() const;
    void setExcludeVariableDictionary(bool excludeVariableDictionary);
    void resetExcludeVariableDictionary();

    bool excludeSpaceTranslation() const;
    bool isExcludeSpaceTranslationDefaulted() const;
    void setExcludeSpaceTranslation(bool excludeSpaceTranslation);
    void resetExcludeSpaceTranslation();

    /* Any non-defaulted value from other is brought over */
    void overrideValuesWith(const ForwardTranslatorOptions& other);

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.ForwardTranslatorOptions");

    static constexpr bool DEFAULT_RUNCONTROLSPECIALDAYS = true;  // At 3.1.0 this was changed to true.
    static constexpr bool DEFAULT_IP_TABULAR_OUTPUT = false;
    static constexpr bool DEFAULT_NO_LIFECYCLECOSTS = false;
    static constexpr bool DEFAULT_NO_SQLITE_OUTPUT = false;
    static constexpr bool DEFAULT_NO_HTML_OUTPUT = false;
    static constexpr bool DEFAULT_NO_VARIABLE_DICTIONARY = false;
    static constexpr bool DEFAULT_NO_SPACE_TRANSLATION = false;  // At 3.4.1, this was changed to false.

    bool m_runcontrolspecialdays = DEFAULT_RUNCONTROLSPECIALDAYS;
    bool m_is_runcontrolspecialdays_defaulted = true;

    bool m_ip_tabular_output = DEFAULT_IP_TABULAR_OUTPUT;
    bool m_is_ip_tabular_output_defaulted = true;

    bool m_no_lifecyclecosts = DEFAULT_NO_LIFECYCLECOSTS;
    bool m_is_no_lifecyclecosts_defaulted = true;

    bool m_no_sqlite_output = DEFAULT_NO_SQLITE_OUTPUT;
    bool m_is_no_sqlite_output_defaulted = true;

    bool m_no_html_output = DEFAULT_NO_HTML_OUTPUT;
    bool m_is_no_html_output_defaulted = true;

    bool m_no_variable_dictionary = DEFAULT_NO_VARIABLE_DICTIONARY;
    bool m_is_no_variable_dictionary_defaulted = true;

    bool m_no_space_translation = DEFAULT_NO_SPACE_TRANSLATION;
    bool m_is_no_space_translation_defaulted = true;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_IMPL_HPP
