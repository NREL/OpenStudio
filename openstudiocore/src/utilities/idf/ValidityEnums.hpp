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

#ifndef UTILITIES_IDF_VALIDITYENUMS_HPP
#define UTILITIES_IDF_VALIDITYENUMS_HPP

#include "../core/Enum.hpp"

namespace openstudio {

/** \class StrictnessLevel
 *  \brief Specifies the desired amount of validity.
 *  
 *  \li None - (Almost) anything goes.
 *  \li Draft - Non-null data is valid.
 *  \li Final - Required and unique data has correct cardinality. 
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is: 
 *  \code
OPENSTUDIO_ENUM(StrictnessLevel, 
                ((None)) 
                ((Draft)) 
                ((Final)) );
 *  \endcode */
OPENSTUDIO_ENUM(StrictnessLevel, 
                ((None)) 
                ((Draft)) 
                ((Final)) );

/** \class Scope
 *  \brief Indicates whether a ValidityReport or DataError pertains to a single field, a 
 *  single object, or the entire collection. 
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is: 
 *  \code
OPENSTUDIO_ENUM(Scope,
                ((Field))
                ((Object))
                ((Collection)) );
 *  \endcode */
OPENSTUDIO_ENUM(Scope,
                ((Field))
                ((Object))
                ((Collection)) );

/** \class DataErrorType
 *  \brief Lists the ways in which an Idf field, object, or collection of objects can be invalid
 *  as compared to its Idd.
 *
 *  The error types are checked that the following levels. (Each level includes all checks listed
 *  at the lower levels as well.)
 *
 *  Strictness::None
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
    ((Duplicate)(unique data is duplicated)) );
 *  \endcode */
OPENSTUDIO_ENUM(DataErrorType,
    ((NoIdd)(input data definition is missing or empty))
    ((NotInitialized)(data is not ready for use or has been removed from collection))
    ((DataType)(field-level data is of an incorrect type))
    ((NumericBound)(numeric data violates a min or max bound))
    ((NameConflict)(two objects of the same type or with overlapping references also have the same name))
    ((NullAndRequired)(required data is missing))
    ((NumberOfFields)(object has too few or too many fields))
    ((Duplicate)(unique data is duplicated)) );

} // openstudio

#endif // UTILITIES_IDF_VALIDITYENUMS_HPP
