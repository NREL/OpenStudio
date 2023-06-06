/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
#define UTILITIES_BCL_BCLMEASUREOUTPUT_HPP

#include "../core/Optional.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

namespace pugi {
class xml_node;
}

namespace openstudio {

/** BCLMeasureOutput is a class representing an output of a measure.  This class does not hold the particular
  *  value of any output, it simply declares that the BCLMeasure has this output.**/
class UTILITIES_API BCLMeasureOutput
{
 public:
  // constructor from xml, throws if required arguments are missing
  BCLMeasureOutput(const pugi::xml_node& element);

  // constructor from xml, throws if required arguments are missing
  BCLMeasureOutput(const std::string& name, const std::string& displayName, const boost::optional<std::string>& shortName,
                   const boost::optional<std::string>& description, const std::string& type, const boost::optional<std::string>& units,
                   bool modelDependent);

  std::string name() const;
  std::string displayName() const;
  boost::optional<std::string> shortName() const;
  boost::optional<std::string> description() const;
  std::string type() const;
  boost::optional<std::string> units() const;
  bool modelDependent() const;

  void writeValues(pugi::xml_node& element) const;

  bool operator==(const BCLMeasureOutput& other) const;

 private:
  std::string m_name;
  std::string m_displayName;
  boost::optional<std::string> m_shortName;
  boost::optional<std::string> m_description;
  std::string m_type;
  boost::optional<std::string> m_units;
  bool m_modelDependent;
};

/** Prints BCLMeasureOutput to os. \relates BCLMeasureOutput */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const BCLMeasureOutput& output);

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
