/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
