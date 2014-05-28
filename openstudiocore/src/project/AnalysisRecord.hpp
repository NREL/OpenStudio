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
