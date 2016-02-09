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

#ifndef UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
#define UTILITIES_BCL_BCLMEASUREOUTPUT_HPP

#include "../core/Optional.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

class QDomDocument;
class QDomElement;

namespace openstudio{

  /** BCLMeasureOutput is a class representing an output of a measure.  This class does not hold the particular
  *  value of any output, it simply declares that the BCLMeasure has this output.**/
  class UTILITIES_API BCLMeasureOutput {
  public:
    
    // constructor from xml, throws if required arguments are missing
    BCLMeasureOutput(const QDomElement& element);

    // constructor from xml, throws if required arguments are missing
    BCLMeasureOutput(const std::string& name, const std::string& displayName,
                     const boost::optional<std::string>& shortName,
                     const boost::optional<std::string>& description,   
                     const std::string& type, const boost::optional<std::string>& units,
                     bool modelDependent);

    std::string name() const;
    std::string displayName() const;
    boost::optional<std::string> shortName() const;
    boost::optional<std::string> description() const;
    std::string type() const;
    boost::optional<std::string> units() const;
    bool modelDependent() const;

    void writeValues(QDomDocument& doc, QDomElement& element) const;

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

} // openstudio

#endif // UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
