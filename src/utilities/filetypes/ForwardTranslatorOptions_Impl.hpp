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
    Json::Value json() const;

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
