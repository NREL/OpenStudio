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
