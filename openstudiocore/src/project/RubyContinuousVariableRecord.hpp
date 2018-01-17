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

#ifndef PROJECT_RUBYCONTINUOUSVARIABLERECORD_HPP
#define PROJECT_RUBYCONTINUOUSVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "ContinuousVariableRecord.hpp"

namespace openstudio {

namespace analysis {
class RubyContinuousVariable;
}

namespace project {

class OSArgumentRecord;
class RubyMeasureRecord;

namespace detail {

  class RubyContinuousVariableRecord_Impl;

} // detail

/** RubyContinuousVariableRecord is a ContinuousVariableRecord. */
class PROJECT_API RubyContinuousVariableRecord : public ContinuousVariableRecord {
 public:

  typedef detail::RubyContinuousVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  RubyContinuousVariableRecord(const analysis::RubyContinuousVariable& rubyContinuousVariable,
                               ProblemRecord& problemRecord,
                               int variableVectorIndex);

  RubyContinuousVariableRecord(const analysis::RubyContinuousVariable& rubyContinuousVariable,
                               FunctionRecord& functionRecord,
                               int variableVectorIndex,
                               boost::optional<double> functionCoefficient);

  RubyContinuousVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  /** Construct from impl. */
  RubyContinuousVariableRecord(std::shared_ptr<detail::RubyContinuousVariableRecord_Impl> impl,
                               ProjectDatabase database);

  virtual ~RubyContinuousVariableRecord() {}

  //@}

  static boost::optional<RubyContinuousVariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                                        ProjectDatabase& database);

  static std::vector<RubyContinuousVariableRecord> getRubyContinuousVariableRecords(
      ProjectDatabase& database);

  static boost::optional<RubyContinuousVariableRecord> getRubyContinuousVariableRecord(
      int id,
      ProjectDatabase& database);

  /** @name Getters */
  //@{

  OSArgumentRecord osArgumentRecord() const;

  RubyMeasureRecord rubyMeasureRecord() const;

  analysis::RubyContinuousVariable rubyContinuousVariable() const;

  //@}
 protected:
  /// @cond
  explicit RubyContinuousVariableRecord(std::shared_ptr<detail::RubyContinuousVariableRecord_Impl> impl);

  friend class detail::RubyContinuousVariableRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.RubyContinuousVariableRecord");

  virtual void constructRelatedRecords(const analysis::Variable& variable);
};

/** \relates RubyContinuousVariableRecord*/
typedef boost::optional<RubyContinuousVariableRecord> OptionalRubyContinuousVariableRecord;

/** \relates RubyContinuousVariableRecord*/
typedef std::vector<RubyContinuousVariableRecord> RubyContinuousVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_RUBYCONTINUOUSVARIABLERECORD_HPP

