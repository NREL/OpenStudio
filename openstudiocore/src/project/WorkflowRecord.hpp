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

#ifndef PROJECT_WORKFLOWRECORD_HPP
#define PROJECT_WORKFLOWRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"

#include <QVariant>

class QSqlQuery;

namespace openstudio {

namespace runmanager {
  class Workflow;
}

namespace project {

class ProblemRecord;

namespace detail {
  class WorkflowRecord_Impl;
}

/** \class WorkflowRecordColumns
 *  \brief Column definitions for the WorkflowRecords table.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( WorkflowRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((runManagerWorkflowKey)(TEXT)(8))
  ((problemRecordId)(INTEGER)(9))
  ((workflowIndex)(INTEGER)(10))
);
 *  \endcode */
OPENSTUDIO_ENUM( WorkflowRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((runManagerWorkflowKey)(TEXT)(8))
  ((problemRecordId)(INTEGER)(9))
  ((workflowIndex)(INTEGER)(10))
);

/** WorkflowRecord is an ObjectRecord that refers to a runmanager::Workflow stored in the RunManager
 *  database associated with this ProjectDatabase. Workflows and \link WorkflowRecord 
 *  WorkflowRecords\endlink are used to store the non-InputVariable poritions of an 
 *  analysis::Problem's workflow. */
class PROJECT_API WorkflowRecord : public ObjectRecord {
 public:
  typedef detail::WorkflowRecord_Impl ImplType;
  typedef WorkflowRecordColumns ColumnsType;
  typedef WorkflowRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  /// constructor from run manager workflow
  WorkflowRecord(const runmanager::Workflow& workflow,
                 ProblemRecord& problemRecord,
                 int workflowIndex);

  WorkflowRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~WorkflowRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<WorkflowRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  /// static constructor from database
  static std::vector<WorkflowRecord> getWorkflowRecords(ProjectDatabase& projectDatabase);

  /// static constructor from database
  static boost::optional<WorkflowRecord> getWorkflowRecord(int id,
                                                           ProjectDatabase& projectDatabase);

  /** @name Getters */
  //@{

  /** Returns the ProblemRecord (parent) of this WorkflowRecord. */
  ProblemRecord problemRecord() const;

  /** Returns the string key used to locate this runmanager::Workflow in the RunManager database. */
  std::string runManagerWorkflowKey() const;

  /** Index of where in problem's vector of WorkflowSteps the first WorkItem in workflow() should
   *  be placed. */
  int workflowIndex() const;

  /** Returns the runmanager::Workflow associated with this record. */
  runmanager::Workflow workflow() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;

  // from impl, does not register in database, use with caution
  WorkflowRecord(std::shared_ptr<detail::WorkflowRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.WorkflowRecord");
};

/** \relates WorkflowRecord */
typedef boost::optional<WorkflowRecord> OptionalWorkflowRecord;

/** \relates WorkflowRecord */
typedef std::vector<WorkflowRecord> WorkflowRecordVector;

/** Sorts WorkflowRecords by workflowIndex. */
struct PROJECT_API WorkflowRecordWorkflowIndexLess {
  bool operator()(const WorkflowRecord& left, const WorkflowRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_WORKFLOWRECORD_HPP
