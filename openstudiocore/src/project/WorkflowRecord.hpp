/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
