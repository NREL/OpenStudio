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

#ifndef PROJECT_MEASUREGROUPRECORD_HPP
#define PROJECT_MEASUREGROUPRECORD_HPP

#include "ProjectAPI.hpp"
#include "DiscreteVariableRecord.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {

namespace analysis{
  class MeasureGroup;
}

namespace project {

class MeasureRecord;

namespace detail{
  class MeasureGroupRecord_Impl;
}

class PROJECT_API MeasureGroupRecord : public DiscreteVariableRecord {
 public:

  typedef detail::MeasureGroupRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                     ProblemRecord& problemRecord,
                     int workflowIndex);

  MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                     FunctionRecord& functionRecord,
                     int variableVectorIndex,
                     boost::optional<double> functionCoefficient);

  MeasureGroupRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~MeasureGroupRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<MeasureGroupRecord> factoryFromQuery(const QSqlQuery& query,
                                                              ProjectDatabase& database);

  static std::vector<MeasureGroupRecord> getMeasureGroupRecords(ProjectDatabase& database);

  static boost::optional<MeasureGroupRecord> getMeasureGroupRecord(int id,
                                                                   ProjectDatabase& database);

  //@}

  analysis::MeasureGroup measureGroup() const;

  /// number of measures
  unsigned numMeasures(bool selectedMeasuresOnly) const;

  /// all measure ids
  std::vector<int> measureRecordIds(bool selectedMeasuresOnly) const;

  /// all measures
  std::vector<MeasureRecord> measureRecords(bool selectedMeasuresOnly) const;

  /** Get the MeasureRecord at index. Throws if index >= numMeasures(false). */
  MeasureRecord getMeasureRecord(int vectorIndex) const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::MeasureGroupRecord_Impl;

  /** Construct from impl. */
  MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.MeasureGroupRecord");

  void constructMeasureRecords(const analysis::MeasureGroup& measureGroup);

  void removeMeasureRecords(const std::vector<UUID>& uuidsToKeep,
                            ProjectDatabase& database);
};

/** \relates MeasureGroupRecord*/
typedef boost::optional<MeasureGroupRecord> OptionalMeasureGroupRecord;

/** \relates MeasureGroupRecord*/
typedef std::vector<MeasureGroupRecord> MeasureGroupRecordVector;

} // project
} // openstudio

#endif // PROJECT_MEASUREGROUPRECORD_HPP
