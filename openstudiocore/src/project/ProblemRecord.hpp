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

#ifndef PROJECT_PROBLEMRECORD_HPP
#define PROJECT_PROBLEMRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace analysis {
  class Problem;
}

namespace project {

class ProblemRecord;
class InputVariableRecord;
class MeasureRecord;
class FunctionRecord;
class WorkflowRecord;

namespace detail {

  class ProblemRecord_Impl;

} // detail

/** \class ProblemRecordType
 *  \brief Enumeration of ProblemRecord and immediately derived types. 
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(ProblemRecordType,
  ((ProblemRecord))
  ((OptimizationProblemRecord))
);
 * \endcode */
OPENSTUDIO_ENUM(ProblemRecordType,
  ((ProblemRecord))
  ((OptimizationProblemRecord))
);

// ETH@20130221 - workflowRecordId is deprecated

/** \class ProblemRecordColumns
 *  \brief Column definitions for the ProblemRecords table.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(ProblemRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordType)(INTEGER)(8))
  ((workflowRecordId)(INTEGER)(9)) // deprecated
);
 * \endcode */
OPENSTUDIO_ENUM(ProblemRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordType)(INTEGER)(8))
  ((workflowRecordId)(INTEGER)(9))
);

/** ProblemRecord is an ObjectRecord that serializes objects of type analysis::Problem. 
 *  \link ProblemRecord ProblemRecords\endlink are comprised of child \link InputVariableRecord 
 *  InputVariableRecords\endlink and \link WorkflowRecord WorkflowRecords\endlink that are
 *  merged together to form the Problem's workflow. Optionally, there may also be a vector of 
 *  response \link FunctionRecord FunctionRecords\endlink. */
class PROJECT_API ProblemRecord : public ObjectRecord {
 public:

  typedef detail::ProblemRecord_Impl ImplType;
  typedef ProblemRecordColumns ColumnsType;
  typedef ProblemRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~ProblemRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<ProblemRecord> factoryFromQuery(const QSqlQuery& query,
                                                         ProjectDatabase& database);

  static ProblemRecord factoryFromProblem(const analysis::Problem& problem,
                                          ProjectDatabase& database);

  /// get all parameter space records in database
  static std::vector<ProblemRecord> getProblemRecords(ProjectDatabase& database);

  /// get all parameter space record by id
  static boost::optional<ProblemRecord> getProblemRecord(int id, ProjectDatabase& database);

  /** @name Getters and Queries */
  //@{

  /// get variables
  std::vector<InputVariableRecord> inputVariableRecords() const;

  std::vector<WorkflowRecord> workflowRecords() const;

  std::vector<FunctionRecord> responseRecords() const;

  analysis::Problem problem() const;

  /** Returns the total number of discrete combinations, if all variables are discrete. */
  boost::optional<int> combinatorialSize(bool selectedMeasuresOnly) const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class detail::Record_Impl;
  friend class ProjectDatabase;

  /** Construct from impl. */
  ProblemRecord(std::shared_ptr<detail::ProblemRecord_Impl> impl,
                ProjectDatabase database);

  /// from impl
  ProblemRecord(std::shared_ptr<detail::ProblemRecord_Impl> impl);
  /// @endcond

  void constructRelatedRecords(const analysis::Problem& problem);

 private:
  REGISTER_LOGGER("openstudio.project.ProblemRecord");

  ProblemRecord(const analysis::Problem& problem, ProjectDatabase& database);

  ProblemRecord(const QSqlQuery& query, ProjectDatabase& database);

  void removeInputVariableRecords(const std::vector<UUID>& uuidsToKeep,
                                  ProjectDatabase& database);

  void removeResponseRecords(const std::vector<UUID>& uuidsToKeep,
                             ProjectDatabase& database);
};

/** \relates ProblemRecord*/
typedef boost::optional<ProblemRecord> OptionalProblemRecord;

/** \relates ProblemRecord*/
typedef std::vector<ProblemRecord> ProblemRecordVector;

} // project
} // openstudio

#endif // PROJECT_PROBLEMRECORD_HPP
