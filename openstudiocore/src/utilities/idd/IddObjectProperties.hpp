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

#ifndef UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP
#define UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Optional.hpp"

#include <string>

namespace openstudio{

/** IddObjectProperties describes the properties of an IddObject, as specified by 
 *  object-level IDD markup. */
struct UTILITIES_API IddObjectProperties{
 public:

  /** Default constructor. */
  IddObjectProperties();

  /** Returns true if all of the underlying data is exactly equal. */
  bool operator==(const IddObjectProperties& other) const;

  /** Negation of operator==. */
  bool operator!=(const IddObjectProperties& other) const;

  /** Returns the memo associated with this object. */
  std::string memo;

  /** Returns true if this object must be unique within an individual IdfFile. */
  bool unique;

  /** Returns true if this object is required for an IdfFile to be valid. */
  bool required;

  /** Returns true if this object is obsolete. */
  bool obsolete;

  /** Returns true if this object contains an IddField of type url. */
  bool hasURL;

  /** Returns true if this object is extensible. */
  bool extensible;

  /** Returns the number of fields in this object's extensible group (zero if !extensible). */
  unsigned numExtensible;

  /** Returns the minimum number of extensible groups an IdfObject following this schema must 
   *  have to be valid. */
  unsigned numExtensibleGroupsRequired;

  /** Formatting markup for this object. Often empty. Contains suggestions for printing certain
   *  types of IdfObjects. For instance, many geometric objects have \\format vertices. */
  std::string format;

  /** Minimum number of fields required for an IdfObject to be valid according to this schema. */
  unsigned minFields;

  /** Maximum number of fields allowed in IdfObjects following this schema. Limits the number of 
   *  allowable extensible groups. */
  boost::optional<unsigned> maxFields;

  /** Print object-level IDD flags to os. Includes indentation for IDD file readability. */
  std::ostream& print(std::ostream& os) const;
};

} // openstudio

#endif // UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP
