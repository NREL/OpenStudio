/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_VALIDITYREPORT_HPP
#define UTILITIES_IDF_VALIDITYREPORT_HPP

#include "../UtilitiesAPI.hpp"

#include "DataError.hpp"
#include "ValidityEnums.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class UTILITIES_API ValidityReport
{
 public:
  /** @name Construction */
  //@{

  /** Collection-level constructor. */
  ValidityReport(StrictnessLevel level);

  /** Object-level constructor. */
  ValidityReport(StrictnessLevel level, const IdfObject& object);

  /** Copy constructor. Resets .nextError() in the cloned report. */
  ValidityReport(const ValidityReport& other);

  /** Insert error into the report. Should only be called by a class that is constructing the
   *  report. */
  void insertError(const DataError& error);

  //@}
  /** @name Inspection */
  //@{

  /** The strictness level of the report. StrictnessLevel::Minimal is for opening files,
   *  StricnessLevel::Draft is for active modeling, and StrictnessLevel::Final is for models
   *  ready to simulate. */
  StrictnessLevel level() const;

  /** Scope of the report. Will be collection or object level. */
  Scope scope() const;

  /** Returns the name of the object being reported on, or an empty string. */
  std::string objectName() const;

  /** Returns the type of the object being reported on if this is a report on an individual
   *  object; if the report is for an entire collection, the return value will evaluate to
   *  false. */
  boost::optional<IddObjectType> objectType() const;

  /** Returns the number of errors listed in the report. */
  unsigned numErrors() const;

  /** Get the next DataError in the report. To be used in data clean-up/debugging. Return value
   *  of false indicates that the last error has already been reported. (If the report is
   *  non-empty, then the next call to nextError() will start cycling back through the errors.) */
  boost::optional<DataError> nextError();

  //@}

 private:
  StrictnessLevel m_level;
  Scope m_scope;
  boost::optional<IddObjectType> m_objectType;
  std::string m_objectName;
  std::set<DataError, DataErrorLess> m_errors;
  std::set<DataError, DataErrorLess>::const_iterator m_currentError;
};

/** Ostream operator for ValidityReport. \relates ValidityReport */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const ValidityReport& report);

}  // namespace openstudio

#endif  // UTILITIES_IDF_VALIDITYREPORT_HPP
