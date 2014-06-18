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

#ifndef UTILITIES_IDF_DATAERROR_HPP
#define UTILITIES_IDF_DATAERROR_HPP

#include "../UtilitiesAPI.hpp"

#include "ValidityEnums.hpp"
#include "Handle.hpp"

#include <utilities/idd/IddEnums.hxx>

#include "../core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class IdfObject;

class UTILITIES_API DataError {
 public:
  /** @name Construction */
  //@{

  /** Constructor for field-level error. */
  DataError(unsigned fieldIndex, const IdfObject& object, DataErrorType errorType);

  /** Constructor for object-level error. */
  DataError(const IdfObject& object, DataErrorType errorType);

  /** Constructor for collection-level error without a related IddObjectType. */
  DataError(DataErrorType errorType);

  /** Constructor for collection-level error with a related IddObjectType. */
  DataError(DataErrorType errorType, IddObjectType objectType);

  //@}
  /** @name Inspection */
  //@{

  /** Returns the scope of the error. */
  Scope scope() const;

  /** Returns the enum that describes the error. The .valueDescription() contains a brief 
   *  description. */
  DataErrorType type() const;

  /** Returns the index of the erroneous field, if scope() == Scope::Field, otherwise throws. */
  unsigned fieldIdentifier() const;

  /** Returns the handle of the erroneous object, if applicable. Otherwise, returned 
   *  Handle.isNull(). */
  Handle objectIdentifier() const;

  /** Returns the name of the object identified by objectIdentifier(), or an empty string. */
  std::string objectName() const;

  /** Returns the type of the object identified by objectIdentifier() if the error is field- or 
   *  object-level; if the error is collection level, the return value will either evaluate to 
   *  false or be related to a cardinality constraint. */
  boost::optional<IddObjectType> objectType() const;

  /** Returns true if all data exactly equal. */
  bool operator==(const DataError& otherError) const;

  /** Negation of operator==. */
  bool operator!=(const DataError& otherError) const;

  //@}
 private:
  Scope m_scope;
  DataErrorType m_type;
  unsigned m_fieldIndex;
  Handle m_objectHandle;
  std::string m_objectName;
  boost::optional<IddObjectType> m_objectType;

  REGISTER_LOGGER("utilities.idf.DataError");
};

/** Set comparator for DataError. \relates DataError */
struct UTILITIES_API DataErrorLess {
  bool operator()(const DataError& left, const DataError& right) const;
};

/** Set container for DataError. \relates DataError */
typedef std::set<DataError,DataErrorLess> DataErrorSet;

/** \relates DataError */
typedef boost::optional<DataError> OptionalDataError;

/** \relates DataError */
typedef std::vector<DataError> DataErrorVector;

/** Ostream operator for DataError. \relates DataError */
UTILITIES_API std::ostream& operator<<(std::ostream& os,const DataError& error);

} // openstudio

#endif // UTILITIES_IDF_DATAERROR_HPP
