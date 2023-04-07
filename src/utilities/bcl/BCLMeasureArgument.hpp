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

#ifndef UTILITIES_BCL_BCLMEASUREARGUMENT_HPP
#define UTILITIES_BCL_BCLMEASUREARGUMENT_HPP

#include "../core/Optional.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

namespace pugi {
class xml_node;
}

namespace openstudio {

/** BCLMeasureArgument is a class representing an argument of a measure.  This class does not hold the particular
  *  value of any argument, it simply declares that the BCLMeasure has this argument.**/
class UTILITIES_API BCLMeasureArgument
{
 public:
  // constructor from xml, throws if required arguments are missing
  BCLMeasureArgument(const pugi::xml_node& element);

  // constructor from xml, throws if required arguments are missing
  BCLMeasureArgument(const std::string& name, const std::string& displayName, const boost::optional<std::string>& description,
                     const std::string& type, const boost::optional<std::string>& units, bool required, bool modelDependent,
                     const boost::optional<std::string>& defaultValue, const std::vector<std::string>& choiceValues,
                     const std::vector<std::string>& choiceDisplayNames, const boost::optional<std::string>& minValue,
                     const boost::optional<std::string>& maxValue);

  std::string name() const;
  std::string displayName() const;
  boost::optional<std::string> description() const;
  std::string type() const;
  boost::optional<std::string> units() const;
  bool required() const;
  bool modelDependent() const;
  boost::optional<std::string> defaultValue() const;
  std::vector<std::string> choiceValues() const;
  std::vector<std::string> choiceDisplayNames() const;
  boost::optional<std::string> minValue() const;
  boost::optional<std::string> maxValue() const;

  void writeValues(pugi::xml_node& element) const;

  bool operator==(const BCLMeasureArgument& other) const;

 private:
  std::string m_name;
  std::string m_displayName;
  boost::optional<std::string> m_description;
  std::string m_type;
  boost::optional<std::string> m_units;
  bool m_required;
  bool m_modelDependent;
  boost::optional<std::string> m_defaultValue;
  std::vector<std::string> m_choiceValues;
  std::vector<std::string> m_choiceDisplayNames;
  boost::optional<std::string> m_minValue;
  boost::optional<std::string> m_maxValue;
};

/** Prints BCLMeasureArgument to os. \relates BCLMeasureArgument */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const BCLMeasureArgument& argument);

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLMEASUREARGUMENT_HPP
