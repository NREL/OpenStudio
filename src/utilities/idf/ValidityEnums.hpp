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

#ifndef UTILITIES_IDF_VALIDITYENUMS_HPP
#define UTILITIES_IDF_VALIDITYENUMS_HPP

#include "../core/Enum.hpp"

namespace openstudio {

// clang-format off

/** \class StrictnessLevel
 *  \brief Specifies the desired amount of validity.
 *
 *  \li None - Anything goes.
 *  \li Minimal - (Almost) anything goes.
 *  \li Draft - Non-null data is valid.
 *  \li Final - Required and unique data has correct cardinality.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(StrictnessLevel,
  ((None))
  ((Minimal))
  ((Draft))
  ((Final))
);
 *  \endcode */
OPENSTUDIO_ENUM(StrictnessLevel,
  ((None))
  ((Minimal))
  ((Draft))
  ((Final))
);

/** \class Scope
 *  \brief Indicates whether a ValidityReport or DataError pertains to a single field, a
 *  single object, or the entire collection.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(Scope,
  ((Field))
  ((Object))
  ((Collection))
);
 *  \endcode */
OPENSTUDIO_ENUM(Scope,
  ((Field))
  ((Object))
  ((Collection))
);

/** \class DataErrorType
 *  \brief Lists the ways in which an Idf field, object, or collection of objects can be invalid
 *  as compared to its Idd.
 *
 *  The error types are checked that the following levels. (Each level includes all checks listed
 *  at the lower levels as well.)
 *
 *  Strictness::None
 *
 *  Strictness::Minimal
 *
 *  \li NoIdd (field-level, collection-level) - The input data definition is missing or empty.
 *  \li NotInitialized - Data is not ready for use or has been removed from the collection.
 *
 *  Strictness::Draft
 *
 *  \li NoIdd (object-level) - The input data definition is missing or empty.
 *  \li DataType - A field or object contains data of an unexpected type.
 *  \li NumericBound - A numeric field violates a field-level min or max bound.
 *  \li NameConflict - Two objects of the same IddObjectType, or with overlapping reference
 *                     lists, have the same name.
 *
 *  Strictness::Final
 *
 *  \li NumericBound - A numeric field violates an (inter-)object-level min or max bound.
 *  \li NullAndRequired - A required field or object is null or missing.
 *  \li NumberOfFields - A data object has too few or too many fields.
 *  \li Duplicate - There are multiple data objects of a type that is supposed to be unique.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(DataErrorType,
  ((NoIdd)(input data definition is missing or empty))
  ((NotInitialized)(data is not ready for use or has been removed from collection))
  ((DataType)(field-level data is of an incorrect type))
  ((NumericBound)(numeric data violates a min or max bound))
  ((NameConflict)(two objects of the same type or with overlapping references also have the same name))
  ((NullAndRequired)(required data is missing))
  ((NumberOfFields)(object has too few or too many fields))
  ((Duplicate)(unique data is duplicated))
);
 *  \endcode */
// cppcheck-suppress syntaxError
OPENSTUDIO_ENUM(DataErrorType,
  ((NoIdd)(input data definition is missing or empty))
  ((NotInitialized)(data is not ready for use or has been removed from collection))
  ((DataType)(field-level data is of an incorrect type))
  ((NumericBound)(numeric data violates a min or max bound))
  ((NameConflict)(two objects of the same type or with overlapping references also have the same name))
  ((NullAndRequired)(required data is missing))
  ((NumberOfFields)(object has too few or too many fields))
  ((Duplicate)(unique data is duplicated))
);

// clang-format on

}  // namespace openstudio

#endif  // UTILITIES_IDF_VALIDITYENUMS_HPP
