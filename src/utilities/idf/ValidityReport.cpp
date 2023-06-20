/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      os << "'. There are no errors to report." << '\n';
    } else {
      os << "The collection is INVALID at strictness level '" << report.level().valueName();
      os << "', because of the errors:" << '\n';
    }
  } else {
    OS_ASSERT(report.scope() == Scope::Object);
    OS_ASSERT(report.objectType());
    if (report.numErrors() == 0) {
      os << "The object of type '" << report.objectType()->valueDescription() << "', named '";
      os << report.objectName() << "', is VALID at strictness level '";
      os << report.level().valueName() << "'. There are no errors to report." << '\n';
    } else {
      os << "The object of type '" << report.objectType()->valueDescription() << "', named '";
      os << report.objectName() << "', is INVALID at strictness level '";
      os << report.level().valueName() << "', because of the errors:" << '\n';
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
