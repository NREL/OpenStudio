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

#ifndef UTILITIES_IDF_DATAERROR_HPP
#define UTILITIES_IDF_DATAERROR_HPP

#include "../UtilitiesAPI.hpp"

#include "ValidityEnums.hpp"
#include "Handle.hpp"

#include "../core/Logger.hpp"

#include "../idd/IddEnums.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class IdfObject;

class UTILITIES_API DataError
{
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
struct UTILITIES_API DataErrorLess
{
  bool operator()(const DataError& left, const DataError& right) const;
};

/** Set container for DataError. \relates DataError */
using DataErrorSet = std::set<DataError, DataErrorLess>;

/** \relates DataError */
using OptionalDataError = boost::optional<DataError>;

/** \relates DataError */
using DataErrorVector = std::vector<DataError>;

/** Ostream operator for DataError. \relates DataError */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const DataError& error);

}  // namespace openstudio

#endif  // UTILITIES_IDF_DATAERROR_HPP
