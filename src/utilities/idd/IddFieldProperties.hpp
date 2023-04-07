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

#ifndef UTILITIES_IDD_IDDFIELDPROPERTIES_HPP
#define UTILITIES_IDD_IDDFIELDPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace openstudio {

// Disable clang-format for OPENSTUDIO_ENUM call, since clang-format will mess it up
// (eg: 'object-list' becomes 'object - list')
// clang-format off

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

// clang-format on

/** IddFieldProperties describes the properties of an IddField object, as specified by
 *  IDD markup. */
struct UTILITIES_API IddFieldProperties
{
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
  enum BoundTypes
  {
    Unbounded,
    InclusiveBound,
    ExclusiveBound
  };

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

  /** This field specifies that the containing IdfObject belongs to the following reference-class-name lists.
  **/
  std::vector<std::string> referenceClassNames;

  /** This field points to a name registered in another file (typically the RDD file). OpenStudio
   *  treats these fields as plain (unparsed) text. */
  std::vector<std::string> externalLists;

  /** Prints these field properties to os, in standard IDD format. */
  std::ostream& print(std::ostream& os) const;
};

/** Returns the intersection of list1 and list2, as determined by IStringSet. */
UTILITIES_API std::vector<std::string> intersectReferenceLists(const std::vector<std::string>& list1, const std::vector<std::string>& list2);

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDFIELDPROPERTIES_HPP
