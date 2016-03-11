/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_BCL_BCLMEASUREARGUMENT_HPP
#define UTILITIES_BCL_BCLMEASUREARGUMENT_HPP

#include "../core/Optional.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

class QDomDocument;
class QDomElement;

namespace openstudio{

  /** BCLMeasureArgument is a class representing an argument of a measure.  This class does not hold the particular
  *  value of any argument, it simply declares that the BCLMeasure has this argument.**/
  class UTILITIES_API BCLMeasureArgument {
  public:
    
    // constructor from xml, throws if required arguments are missing
    BCLMeasureArgument(const QDomElement& element);

    // constructor from xml, throws if required arguments are missing
    BCLMeasureArgument(const std::string& name, const std::string& displayName,
                       const boost::optional<std::string>& description,
                       const std::string& type, const boost::optional<std::string>& units,
                       bool required, bool modelDependent,
                       const boost::optional<std::string>& defaultValue,
                       const std::vector<std::string>& choiceValues,
                       const std::vector<std::string>& choiceDisplayNames,
                       const boost::optional<std::string>& minValue,
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

    void writeValues(QDomDocument& doc, QDomElement& element) const;

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

} // openstudio

#endif // UTILITIES_BCL_BCLMEASUREARGUMENT_HPP
