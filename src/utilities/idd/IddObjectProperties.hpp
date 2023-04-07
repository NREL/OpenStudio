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

#ifndef UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP
#define UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Optional.hpp"

#include <string>

namespace openstudio {

/** IddObjectProperties describes the properties of an IddObject, as specified by
 *  object-level IDD markup. */
struct UTILITIES_API IddObjectProperties
{
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

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDOBJECTPROPERTIES_HPP
