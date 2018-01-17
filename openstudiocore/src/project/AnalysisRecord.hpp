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

#ifndef PROJECT_ANALYSISRECORD_HPP
#define PROJECT_ANALYSISRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {
namespace analysis {
  class Analysis;
} // analysis
namespace project {

class ProblemRecord;
class AlgorithmRecord;
class FileReferenceRecord;
class DataPointRecord;

namespace detail {

  class AnalysisRecord_Impl;

} // detail

/** \class AnalysisRecordColumns
 *  \brief Column definitions for the AnalysisRecords table.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( AnalysisRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordId)(INTEGER)(8))
  ((algorithmRecordId)(INTEGER)(9))
  ((seedFileReferenceRecordId)(INTEGER)(10))
  ((weatherFileReferenceRecordId)(INTEGER)(11))
  ((resultsAreInvalid)(BOOLEAN)(12))
  ((dataPointsAreInvalid)(BOOLEAN)(13))
);
 *  \endcode */
OPENSTUDIO_ENUM( AnalysisRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordId)(INTEGER)(8))
  ((algorithmRecordId)(INTEGER)(9))
  ((seedFileReferenceRecordId)(INTEGER)(10))
  ((weatherFileReferenceRecordId)(INTEGER)(11))
  ((resultsAreInvalid)(BOOLEAN)(12))
  ((dataPointsAreInvalid)(BOOLEAN)(13))
);

/** AnalysisRecord is a ObjectRecord*/
class PROJECT_API AnalysisRecord : public ObjectRecord {
 public:

  typedef detail::AnalysisRecord_Impl ImplType;
  typedef AnalysisRecordColumns ColumnsType;
  typedef AnalysisRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  AnalysisRecord(const analysis::Analysis& analysis, ProjectDatabase& database);

  AnalysisRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~AnalysisRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<AnalysisRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  static std::vector<AnalysisRecord> getAnalysisRecords(ProjectDatabase& database);

  static boost::optional<AnalysisRecord> getAnalysisRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns the ProblemRecord (required resource) associated with this AnalysisRecord. */
  ProblemRecord problemRecord() const;

  /** Returns the AlgorithmRecord (optional resource) associated with this AnalysisRecord. */
  boost::optional<AlgorithmRecord> algorithmRecord() const;

  /** Returns the seed FileReferenceRecord (required child) of this AnalysisRecord. */
  FileReferenceRecord seedFileReferenceRecord() const;

  /** Returns the weather FileReferenceRecord (optional child) of this AnalysisRecord. */
  boost::optional<FileReferenceRecord> weatherFileReferenceRecord() const;

  /** Returns the \link DataPointRecord DataPointRecords \endlink (children) of this
   *  AnalysisRecord. */
  std::vector<DataPointRecord> dataPointRecords() const;

  /** Returns the DataPointRecords with complete == false. */
  std::vector<DataPointRecord> incompleteDataPointRecords() const;

  /** Returns the DataPointRecords with complete == true. */
  std::vector<DataPointRecord> completeDataPointRecords() const;

  /** Returns the completeDataPointRecords with failed == false. */
  std::vector<DataPointRecord> successfulDataPointRecords() const;

  /** Returns the completeDataPointRecords with failed == true. */
  std::vector<DataPointRecord> failedDataPointRecords() const;

  std::vector<DataPointRecord> getDataPointRecords(
      const std::vector<QVariant>& variableValues) const;

  std::vector<DataPointRecord> getDataPointRecords(
      const std::vector<int>& discretePerturbationRecordIds) const;

  std::vector<DataPointRecord> getDataPointRecords(const std::string& tag) const;

  /** Returns true if this AnalysisRecord holds DataPointRecords with invalid results still 
   *  attached. */
  bool resultsAreInvalid() const;

  /** Returns true if this AnalysisRecord holds DataPointRecords that are invalid in light of 
   *  changes to the problem formulation. */
  bool dataPointsAreInvalid() const;

  /** Deserializes this record and all its children and resources to analysis::Analysis. */
  analysis::Analysis analysis() const;

  //@}
  /** @name Setters */
  //@{

  /** Provided for callers operating directly on the database, not holding a copy of this
   *  analysis in memory. Use with caution. */
  void setResultsAreInvalid(bool value);

  /** Provided for callers operating directly on the database, not holding a copy of this
   *  analysis in memory. Use with caution. */
  void setDataPointsAreInvalid(bool value);

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::AnalysisRecord_Impl;

  /** Construct from impl. */
  AnalysisRecord(std::shared_ptr<detail::AnalysisRecord_Impl> impl,
                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit AnalysisRecord(std::shared_ptr<detail::AnalysisRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.AnalysisRecord");

  void removeDataPointRecords(const std::vector<UUID>& uuidsToKeep,
                              ProjectDatabase& database);
};

/** \relates AnalysisRecord*/
typedef boost::optional<AnalysisRecord> OptionalAnalysisRecord;

/** \relates AnalysisRecord*/
typedef std::vector<AnalysisRecord> AnalysisRecordVector;

} // project
} // openstudio

#endif // PROJECT_ANALYSISRECORD_HPP
