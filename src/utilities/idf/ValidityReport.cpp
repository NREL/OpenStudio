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

#include "ValidityReport.hpp"
#include "IdfObject.hpp"

#include "../idd/IddObject.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

ValidityReport::ValidityReport(StrictnessLevel level) : m_level(level), m_scope(Scope::Collection) {
  m_currentError = m_errors.end();
}

ValidityReport::ValidityReport(StrictnessLevel level, const IdfObject& object)
  : m_level(level), m_scope(Scope::Object), m_objectType(object.iddObject().type()) {
  m_currentError = m_errors.end();
  OptionalString oName = object.name();
  if (oName) {
    m_objectName = *oName;
  }
}

ValidityReport::ValidityReport(const ValidityReport& other)
  : m_level(other.m_level), m_scope(other.m_scope), m_objectType(other.m_objectType), m_objectName(other.m_objectName), m_errors(other.m_errors) {
  // reset the currentError counter
  m_currentError = m_errors.end();
}

void ValidityReport::insertError(const DataError& error) {
  m_errors.insert(error);
  m_currentError = m_errors.end();
}

StrictnessLevel ValidityReport::level() const {
  return m_level;
}

Scope ValidityReport::scope() const {
  return m_scope;
}

std::string ValidityReport::objectName() const {
  return m_objectName;
}

boost::optional<IddObjectType> ValidityReport::objectType() const {
  return m_objectType;
}

unsigned ValidityReport::numErrors() const {
  return m_errors.size();
}

boost::optional<DataError> ValidityReport::nextError() {
  // cycle through errors
  if (m_currentError == m_errors.end()) {
    m_currentError = m_errors.begin();
  } else {
    ++m_currentError;
  }

  // if not at end, report error
  OptionalDataError result;
  if (m_currentError != m_errors.end()) {
    result = *m_currentError;
  }

  return result;
}

std::ostream& operator<<(std::ostream& os, const ValidityReport& report) {
  if (report.scope() == Scope::Collection) {
    if (report.numErrors() == 0) {
      os << "The collection is VALID at strictness level '" << report.level().valueName();
      os << "'. There are no errors to report." << std::endl;
    } else {
      os << "The collection is INVALID at strictness level '" << report.level().valueName();
      os << "', because of the errors:" << std::endl;
    }
  } else {
    OS_ASSERT(report.scope() == Scope::Object);
    OS_ASSERT(report.objectType());
    if (report.numErrors() == 0) {
      os << "The object of type '" << report.objectType()->valueDescription() << "', named '";
      os << report.objectName() << "', is VALID at strictness level '";
      os << report.level().valueName() << "'. There are no errors to report." << std::endl;
    } else {
      os << "The object of type '" << report.objectType()->valueDescription() << "', named '";
      os << report.objectName() << "', is INVALID at strictness level '";
      os << report.level().valueName() << "', because of the errors:" << std::endl;
    }
  }

  // print errors
  if (report.numErrors() > 0) {
    ValidityReport reportCopy(report);
    OptionalDataError error = reportCopy.nextError();
    while (error) {
      os << *error;
      error = reportCopy.nextError();
    }
  }

  return os;
}

}  // namespace openstudio
