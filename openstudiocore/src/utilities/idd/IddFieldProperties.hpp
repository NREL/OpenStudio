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

#ifndef UTILITIES_IDD_IDDFIELDPROPERTIES_HPP
#define UTILITIES_IDD_IDDFIELDPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace openstudio {

/** \class IddFieldType 
 *  \brief Enumeration of IDF field types as defined by the possible values for the IDD \\type 
 *  flag. See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro 
 *  call is:
 *  \code
OPENSTUDIO_ENUM(IddFieldType,
  ((UnknownType)(unknown))
  ((IntegerType)(integer))
  ((RealType)(real))
  ((AlphaType)(alpha))
  ((ChoiceType)(choice))
  ((NodeType)(node))
  ((ObjectListType)(object-list))
  ((ExternalListType)(external-list))
  ((URLType)(url))
  ((HandleType)(handle))
);
 *  \endcode */
OPENSTUDIO_ENUM(IddFieldType,
  ((UnknownType)(unknown))
  ((IntegerType)(integer))
  ((RealType)(real))
  ((AlphaType)(alpha))
  ((ChoiceType)(choice))
  ((NodeType)(node))
  ((ObjectListType)(object-list))
  ((ExternalListType)(external-list))
  ((URLType)(url))
  ((HandleType)(handle))
);

/** IddFieldProperties describes the properties of an IddField object, as specified by 
 *  IDD markup. */
struct UTILITIES_API IddFieldProperties{
 public:

  /** Default constructor. */
  IddFieldProperties();

  /** Returns true if no properties other than type are set. */
  bool isGeneric() const;

  /** Returns true if all underlying data is exactly equal. */
  bool operator==(const IddFieldProperties& other) const;

  /** Negation of operator== */
  bool operator!=(const IddFieldProperties& other) const;

  /** Field type (alpha, numeric, various flavors of alpha providing links to other objects 
   *  and files). */
  IddFieldType type;

  /** Field notes collected into one string. */
  std::string note;

  /** True if this field is required (must be filled in for the IdfObject to be valid). */
  bool required;

  /** True if the field data is autosizable. */
  bool autosizable;

  /** True if the field autocalculatable. */
  bool autocalculatable;

  /** True if the field value is case-sensitive. */
  bool retaincase;

  /** True if this field has been deprecated (should not be used, but is okay for legacy files). */
  bool deprecated;

  /** True if this field is the first in an extensible group. */
  bool beginExtensible;

  /** The EnergyPlus-format unit string for this field. All data are assumed to be associated 
   *  with these (typically SI) units. */
  boost::optional<std::string> units;

  /** The EnergyPlus-format unit string indicating this field's IP units. No effect on underlying
   *  data; a suggestion for user-facing displays. */
  boost::optional<std::string> ipUnits;

  /** Enumeration indicating the type of numeric bound to be applied. */
  enum BoundTypes { Unbounded, InclusiveBound, ExclusiveBound };

  /** Minimum bound is non-existent (Unbounded), >= (InclusiveBound), or > (ExclusiveBound). */
  BoundTypes minBoundType;
  boost::optional<double> minBoundValue;
  boost::optional<std::string> minBoundText;

  /** Maximum bound is non-existent (Unbounded), <= (InclusiveBound), or < (ExclusiveBound). */
  BoundTypes maxBoundType;
  boost::optional<double> maxBoundValue;
  boost::optional<std::string> maxBoundText;

  /// The default value as string.
  boost::optional<std::string> stringDefault;

  /// The default value as double.
  boost::optional<double> numericDefault;

  /** This field points to an IdfObject that is registered with one of the reference lists named 
   *  in this vector. The list names are defined by \\reference markup of name fields. There is 
   *  also an OpenStudio-specific universal reference list called 'AllObjects'. */
  std::vector<std::string> objectLists;

  /** This field specifies that the containing IdfObject belongs to the following reference lists. 
   *  This information can be used by other objects to point to (reference) this object using 
   *  \\object-list markup. */
  std::vector<std::string> references;

  /** This field points to a name registered in another file (typically the RDD file). OpenStudio
   *  treats these fields as plain (unparsed) text. */
  std::vector<std::string> externalLists;

  /** Prints these field properties to os, in standard IDD format. */
  std::ostream& print(std::ostream& os) const;
};

/** Returns the intersection of list1 and list2, as determined by IStringSet. */
UTILITIES_API std::vector<std::string> intersectReferenceLists(
    const std::vector<std::string>& list1,
    const std::vector<std::string>& list2);

} // openstudio

#endif // UTILITIES_IDD_IDDFIELDPROPERTIES_HPP
