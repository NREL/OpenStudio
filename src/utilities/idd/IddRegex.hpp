/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_IDD_IDDREGEX_HPP
#define UTILITIES_IDD_IDDREGEX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Regex.hpp"

namespace openstudio{
namespace iddRegex{

  /// name of the comment only object automatically added to the idd
  UTILITIES_API const std::string &commentOnlyObjectName();

  /// text of the comment only object automatically added to the idd
  UTILITIES_API const std::string &commentOnlyObjectText();

  /// Search for IDD version in line
  /// matches[1], version identifier
  UTILITIES_API const Regex &version();

  /// Search for IDD build in line
  /// matches[1], build identifier
  UTILITIES_API const Regex &build();

  /// Search for IDD header, each line must start with '!', no preceding whitespace
  /// matches[1], header
  UTILITIES_API const Regex &header();

  /// Match comment only line
  /// matches[1], comment
  UTILITIES_API const Regex &commentOnlyLine();

  /// Match content then comment
  /// matches[1], content
  /// matches[2], comment if any
  UTILITIES_API const Regex &contentAndCommentLine();

  /// Match group identifier
  /// matches[1], group name
  UTILITIES_API const Regex &group();

  /// Match include-file identifier
  /// matches[1], included Idd file name
  UTILITIES_API const Regex &includeFile();

  /// Match remove-object identifier
  /// matches[1], object in included Idd file that should not be included in this file
  UTILITIES_API const Regex &removeObject();

  /// Match line with either a ',' or a ';' that are not preceded by '!'
  /// matches[1], before separator
  /// matches[2], after separator
  UTILITIES_API const Regex &line();

  /// Match an object memo property
  /// matches[1], memo text
  UTILITIES_API const Regex &memoProperty();

  /// Match an object note property
  /// matches[1], note text
  UTILITIES_API const Regex &noteProperty();

  /// Match an object with no fields in the idd
  /// matches[1], before separator
  /// matches[2], after separator
  UTILITIES_API const Regex &objectNoFields();

  /// Match an object with one or more fields in the idd
  /// matches[1], object text
  /// matches[2], field(s) text
  UTILITIES_API const Regex &objectAndFields();

  /// Match an object unique property
  UTILITIES_API const Regex &uniqueProperty();

  /// Match an object required property
  UTILITIES_API const Regex &requiredObjectProperty();

  /// Match an object obsolete property
  /// matches[1], reason for obsolete
  UTILITIES_API const Regex &obsoleteProperty();

  /// Match an object hasURL property
  UTILITIES_API const Regex &hasurlProperty();

  /// Match an object extensible property
  /// matches[1], number of last fields to extend
  UTILITIES_API const Regex &extensibleProperty();

  /// Match an object format property
  /// matches[1], format text
  UTILITIES_API const Regex &formatProperty();

  /// Match an object min fields property
  /// matches[1], min number of fields
  UTILITIES_API const Regex &minFieldsProperty();

  /// Match an object max fields property
  /// matches[1], max number of fields
  UTILITIES_API const Regex &maxFieldsProperty();

  /// Match a field declaration in the idd
  /// A or N, then one or more numbers then white space and then a ',' or ';'
  /// matches[1], alpha or numeric indicator
  /// matches[2], alpha or numeric number
  /// matches[3], after separator
  UTILITIES_API const Regex &field();

  /// Match the closing field in an idd object
  /// matches[1], all previous text
  /// matches[2], the last field
  UTILITIES_API const Regex &closingField();

  /// Match the last field declaration in a string, may or may not be the closing field
  /// matches[1], all previous text
  /// matches[2], the last field
  UTILITIES_API const Regex &lastField();

  /// Match a field name
  /// matches[1], the field name
  UTILITIES_API const Regex &name();

  /// Match a field field name property
  /// matches[1], the field name
  UTILITIES_API const Regex &nameProperty();

  /// Match a field required property
  UTILITIES_API const Regex &requiredFieldProperty();

  /// Match a field autosizable property
  UTILITIES_API const Regex &autosizableProperty();

  /// Match a field autocalculatable property
  UTILITIES_API const Regex &autocalculatableProperty();

  /// Match a field retaincase property
  UTILITIES_API const Regex &retaincaseProperty();

  /// Match a field units property
  /// matches[1], the field units
  UTILITIES_API const Regex &unitsProperty();

  /// Match a field ip units property
  /// matches[1], the field ip units
  UTILITIES_API const Regex &ipUnitsProperty();

  /// Match a field exclusive minimum property
  /// matches[1], the field exclusive minimum
  UTILITIES_API const Regex &minExclusiveProperty();

  /// Match a field inclusive minimum property
  /// matches[1], the field inclusive minimum
  UTILITIES_API const Regex &minInclusiveProperty();

  /// Match a field exclusive maximum property
  /// matches[1], the field exclusive maximum
  UTILITIES_API const Regex &maxExclusiveProperty();

  /// Match a field inclusive maximum property
  /// matches[1], the field inclusive maximum
  UTILITIES_API const Regex &maxInclusiveProperty();

  /// Match a field deprecated property
  /// matches[1], reason for deprecated
  UTILITIES_API const Regex &deprecatedProperty();

  /// Match a field default property
  /// matches[1], default value
  UTILITIES_API const Regex &defaultProperty();

  /// Match a field default property with either autocalculate or autosize
  UTILITIES_API const Regex &automaticDefault();

  /// Match a field type property
  /// matches[1], type
  UTILITIES_API const Regex &typeProperty();

  /// Match a field key property
  /// matches[1], key value
  UTILITIES_API const Regex &keyProperty();

  /// Match a field object-list property
  /// matches[1], object-list value
  UTILITIES_API const Regex &objectListProperty();

  /// Match a field external-list property
  /// matches[1], external-list value
  UTILITIES_API const Regex &externalListProperty();

  /// Match a field reference property
  /// matches[1], reference value
  UTILITIES_API const Regex &referenceProperty();

  /// Match a field reference-class-name property
  /// matches[1], reference-class-name value
  UTILITIES_API const Regex &referenceClassNameProperty();

  /// Match begin extensible
  UTILITIES_API const Regex &beginExtensible();

  /// Match begin extensible
  UTILITIES_API const Regex &beginExtensibleProperty();

  /// Match a field or object level comment
  /// matches[1], after '\' until next '\'
  /// matches[2], after second '\' (may be empty)
  UTILITIES_API const Regex &metaDataComment();

  /// Match IDD names that correspond to a Version object.
  UTILITIES_API const Regex &versionObjectName();

} // iddRegex
} // openstudio

#endif // UTILITIES_IDD_IDDREGEX_HPP
