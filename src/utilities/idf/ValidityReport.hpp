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
