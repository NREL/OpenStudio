/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_REGEX_HPP
#define UTILITIES_REGEX_HPP

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <boost/algorithm/string/case_conv.hpp>

namespace openstudio {
  struct Regex {
    enum struct Ignore_Case {
      True,
      False
    };

    using Results = std::optional <std::array<std::string_view, 5>>;
    using Results_Func = Results (*)(const std::string_view);

    Results_Func m_match;
    Results_Func m_search;
    Ignore_Case m_ignoreCase = Ignore_Case::False;

    // prevent use by temporary string, since this is returning a string_view
    void match(std::string &&) = delete;
    void search(std::string &&) = delete;



    [[nodiscard]] auto match(const std::string_view sv) const {
      if (m_ignoreCase == Ignore_Case::True) {
        return case_insensitive(m_match, sv);
      } else {
        return m_match(sv);
      }
    }

    [[nodiscard]] auto search(const std::string_view sv) const {
      if (m_ignoreCase == Ignore_Case::True) {
        return case_insensitive(m_search, sv);
      } else {
        return m_search(sv);
      }
    }

  private:
    [[nodiscard]] static Results case_insensitive(Results_Func op, const std::string_view sv) {
      std::string str{sv};
      boost::algorithm::to_lower(str);
      const auto results = op(str);

      if (results) {
        // we need to create a new results object that is a set of string_view into the same positions of
        // the one that was performed on the to_lower string. With only changing the case
        // the positions inside of the string should stay constant
        Results::value_type translated_results;

        for (std::size_t index = 0; index < translated_results.size(); ++index) {
          if (!(*results)[index].empty()) {
            translated_results[index] = sv.substr(std::distance(str.c_str(), (*results)[index].data()), (*results)[index].size());
          }
        }

        return translated_results;
      } else {
        return {};
      }
    }
  };
}

#endif
