/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP
#define PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "OutputVariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class OutputAttributeVariable;
}
namespace project {

namespace detail {

  class OutputAttributeVariableRecord_Impl;

} // detail

/** OutputAttributeVariableRecord is an OutputVariableRecord*/
class PROJECT_API OutputAttributeVariableRecord : public OutputVariableRecord {
 public:

  typedef detail::OutputAttributeVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OutputAttributeVariableRecord(
      const analysis::OutputAttributeVariable& outputAttributeVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient);

  OutputAttributeVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~OutputAttributeVariableRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<OutputAttributeVariableRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<OutputAttributeVariableRecord>
  getOutputAttributeVariableRecords(ProjectDatabase& database);

  static boost::optional<OutputAttributeVariableRecord>
  getOutputAttributeVariableRecord(int id, ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  analysis::OutputAttributeVariable outputAttributeVariable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::OutputAttributeVariableRecord_Impl;

  /** Construct from impl. */
  OutputAttributeVariableRecord(
      std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit OutputAttributeVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.OutputAttributeVariableRecord");
};

/** \relates OutputAttributeVariableRecord*/
typedef boost::optional<OutputAttributeVariableRecord> OptionalOutputAttributeVariableRecord;

/** \relates OutputAttributeVariableRecord*/
typedef std::vector<OutputAttributeVariableRecord> OutputAttributeVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP
