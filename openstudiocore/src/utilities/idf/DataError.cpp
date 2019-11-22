/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <iomanip>
#include "DataError.hpp"
#include "IdfObject.hpp"

#include "../idd/IddObject.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

DataError::DataError(unsigned fieldIndex, const IdfObject& object, DataErrorType errorType)
  : m_scope(Scope::Field),
    m_type(errorType),
    m_fieldIndex(fieldIndex),
    m_objectHandle(object.handle()),
    m_objectType(object.iddObject().type())
{
  // construct field-level error
  OptionalString oName = object.name();
  if (oName) { m_objectName = *oName; }
}

DataError::DataError(const IdfObject& object, DataErrorType errorType)
  : m_scope(Scope::Object),
    m_type(errorType),
    m_fieldIndex(0),
    m_objectHandle(object.handle()),
    m_objectType(object.iddObject().type())
{
  // construct object-level error
  OptionalString oName = object.name();
  if (oName) { m_objectName = *oName; }
}

DataError::DataError(DataErrorType errorType)
  : m_scope(Scope::Collection),
    m_type(errorType),
    m_fieldIndex(0)
{
  // construct collection-level error
}

DataError::DataError(DataErrorType errorType, IddObjectType objectType)
  : m_scope(Scope::Collection),
    m_type(errorType),
    m_fieldIndex(0),
    m_objectType(objectType)
{
  // construct collection-level error
}

Scope DataError::scope() const {
  return m_scope;
}

DataErrorType DataError::type() const {
  return m_type;
}

unsigned DataError::fieldIdentifier() const {
  if (scope() != Scope::Field) {
    LOG_AND_THROW("There is no field identifier for this DataError, which has scope "
                  << scope().valueDescription() << ".");
  }
  return m_fieldIndex;
}

Handle DataError::objectIdentifier() const {
  return m_objectHandle;
}

std::string DataError::objectName() const {
  return m_objectName;
}

boost::optional<IddObjectType> DataError::objectType() const {
  return m_objectType;
}

bool DataError::operator==(const DataError& otherError) const {
  return ( (scope() == otherError.scope()) &&
           (type() == otherError.type()) &&
           (m_fieldIndex == otherError.m_fieldIndex) &&
           (objectIdentifier() == otherError.objectIdentifier()) &&
           (objectName() == otherError.objectName()) &&
           (objectType() == otherError.objectType()) );
}

bool DataError::operator!=(const DataError& otherError) const {
  return !(*this == otherError);
}

bool DataErrorLess::operator()(const DataError& left, const DataError& right) const {
  // order first by error type, since listed in order of severity
  if (left.type() != right.type()) {
    return left.type() < right.type();
  }
  // within type order by collection, then object, then field-level, since
  // collection-level is generally the most severe (IddFile missing, for instance)
  if (left.scope() != right.scope()) {
    return left.scope() > right.scope();
  }
  // if object-level name conflict, order by name, then type
  if ((left.scope() == Scope::Object) && (left.type() == DataErrorType::NameConflict)) {
    if (!istringEqual(left.objectName(),right.objectName())) {
      return istringLess(left.objectName(),right.objectName());
    }
  }
  // if objectType exists, order by that next, since Idd Files are in a
  // semi-logical order
  if ((left.objectType() != boost::none) && (right.objectType() != boost::none)) {
    return left.objectType().get() < right.objectType().get();
  }
  // now order by objectName in remaining cases
  if (!istringEqual(left.objectName(),right.objectName())) {
    return istringLess(left.objectName(),right.objectName());
  }
  // now by objectHandle (for non-named objects)
  if (left.objectIdentifier() != right.objectIdentifier()) {
    return left.objectIdentifier() < right.objectIdentifier();
  }
  // now by fieldIndex, if applicable
  if (left.scope() == Scope::Field) {
    return left.fieldIdentifier() < right.fieldIdentifier();
  }

  // exactly equal
  return false;
}

std::ostream& operator<<(std::ostream& os,const DataError& error) {

  os << std::setw(11) << std::left << error.scope().valueName() << "level data error of type ";
  os << std::setw(18) << std::left << error.type().valueName() << "." << std::endl;

  if (error.scope() == Scope::Field) {
    OS_ASSERT(error.objectType());
    os << "Error is in an object of type '" << error.objectType()->valueDescription();
    os << "', named '" << error.objectName() << "', in field " << error.fieldIdentifier() << ".";
    os << std::endl;
  }

  if (error.scope() == Scope::Object) {
    OS_ASSERT(error.objectType());
    os << "Error pertains to an object of type '" << error.objectType()->valueDescription();
    os << "', named '" << error.objectName() << "'." << std::endl;
  }

  if ((error.scope() == Scope::Collection) && (error.objectType())) {
    os << "Error pertains to objects of type '" << error.objectType()->valueDescription() << "'.";
    os << std::endl;
  }

  os << "Additional information about the error type: " << error.type().valueDescription() << ".";
  os << std::endl;

  return os;
}

} // openstudio
