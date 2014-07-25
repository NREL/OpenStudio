/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include "IddObjectProperties.hpp"

#include <sstream>

namespace openstudio {

  /// default constructor
  IddObjectProperties::IddObjectProperties(): 
    unique(false), 
    required(false), 
    obsolete(false), 
    hasURL(false),
    extensible(false), 
    numExtensible(0),
    numExtensibleGroupsRequired(0), 
    minFields(0)
  {}

  // ETH@20100329 Is the purpose to identify exactly equal properties, or objects that behave
  // the same? If the latter, might ignore memo, and even numExtensibleGroupsRequired, and 
  // minFields. Keeping it simple for now.
  /// equality operator
  bool IddObjectProperties::operator==(const IddObjectProperties& other) const 
  {
 
    return ((this == &other) || 
            ((memo == other.memo) &&
             (unique == other.unique) &&
             (required == other.required) &&
             (obsolete == other.obsolete) &&
             (hasURL == other.hasURL) &&
             (extensible == other.extensible) &&
             (numExtensible == other.numExtensible) &&
             (numExtensibleGroupsRequired == other.numExtensibleGroupsRequired) &&
             (format == other.format) &&
             (minFields == other.minFields) &&
             (maxFields == other.maxFields)));
  }

  bool IddObjectProperties::operator!=(const IddObjectProperties& other) const {
    return !(*this == other);
  }

  /// print
  std::ostream& IddObjectProperties::print(std::ostream& os) const
  {
    if (!memo.empty()){
      std::stringstream ss(memo);
      std::string line;
      while (! ss.eof() ) {
        std::getline(ss,line);
        os << "       \\memo " << line << std::endl;
      }
    }

    if (unique){
      os << "       \\unique-object" << std::endl;
    }

    if (required){
      os << "       \\required-object" << std::endl;
    }

    if (obsolete){
      os << "       \\obsolete" << std::endl;
    }

    if (hasURL)
    {
      os << "       \\url-object" << std::endl;
    }
    if (extensible){
      os << "       \\extensible:" << numExtensible << std::endl;
    }

    if (!format.empty()){
      os << "       \\format " << format << std::endl; 
    }

    if (minFields > 0){
      os << "       \\min-fields " << minFields << std::endl; 
    } 

    if (maxFields) {
      os << "       \\max-fields " << *maxFields << std::endl; 
    }

    return os;
  }

} // openstudio
