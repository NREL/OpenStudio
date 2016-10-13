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

#ifndef PROJECT_DATAPOINTRECORD_HPP
#define PROJECT_DATAPOINTRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {

class Attribute;
class FileReference;

namespace analysis {
  class DataPoint;
  class DataPointRunType;
} // analysis
namespace project {

class AnalysisRecord;
class AttributeRecord;
class ProblemRecord;
class FileReferenceRecord;
class MeasureRecord;
class TagRecord;
class DataPointValueRecord;

namespace detail {

  class DataPointRecord_Impl;

} // detail

// ETH@20110808 Adding Type enum now, because will need it when add optimization problems and
// data points.

/** \class DataPointRecordType
 *
 *  \relates DataPointRecord */
OPENSTUDIO_ENUM(DataPointRecordType,
  ((DataPointRecord)(DataPointRecord)(0))
  ((OptimizationDataPointRecord)(OptimizationDataPointRecord)(1))
);

// ETH@20130612 inputDataRecordId == osmInputDataRecordId. Fix at an appropriate time.

/** \class DataPointRecordColumns
 *  \brief Column definitions for the DataPointRecords table.
 *
 *  \relates DataPointRecord */
OPENSTUDIO_ENUM(DataPointRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((analysisRecordId)(INTEGER)(8))
  ((problemRecordId)(INTEGER)(9))
  ((dataPointRecordType)(INTEGER)(10))
  ((complete)(BOOLEAN)(11))
  ((failed)(BOOLEAN)(12))
  ((directory)(TEXT)(13))
  ((inputDataRecordId)(INTEGER)(14))
  ((sqlOutputDataRecordId)(INTEGER)(15))
  ((xmlOutputDataRecordId)(INTEGER)(16))
  ((topLevelJobUUID)(TEXT)(17))
  ((dakotaParametersFiles)(TEXT)(18))
  ((idfInputDataRecordId)(INTEGER)(19))
  ((selected)(BOOLEAN)(20))
  ((runType)(INTEGER)(21))
);

/** DataPointRecord is a ObjectRecord*/
class PROJECT_API DataPointRecord : public ObjectRecord {
 public:

  typedef detail::DataPointRecord_Impl ImplType;
  typedef DataPointRecordColumns ColumnsType;
  typedef DataPointRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~DataPointRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get DataPointRecord from query. Returned object will be of the correct derived type. */
  static boost::optional<DataPointRecord> factoryFromQuery(const QSqlQuery& query,
                                                           ProjectDatabase& database);

  static DataPointRecord factoryFromDataPoint(const analysis::DataPoint& dataPoint,
                                              AnalysisRecord& analysisRecord,
                                              const ProblemRecord& problemRecord);

  static std::vector<DataPointRecord> getDataPointRecords(ProjectDatabase& database);

  static boost::optional<DataPointRecord> getDataPointRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns the AnalysisRecord to which this DataPointRecord belongs (as a child). */
  AnalysisRecord analysisRecord() const;

  /** Returns the ProblemRecord associated with this DataPointRecord (as a resource). */
  ProblemRecord problemRecord() const;

  /** Returns true if this data point has been run/simulated. Returns true even if failed().
   *  \deprecated */
  bool isComplete() const;

  bool complete() const;

  /** Returns true if this data point has been run, but the simulation failed. Returns false
   *  otherwise. (Always returns false if !complete().) */
  bool failed() const;

  bool selected() const;

  analysis::DataPointRunType runType() const;

  openstudio::path directory() const;

  std::vector<QVariant> variableValues() const;

  /** Returns the measures associated with this DataPoint via MeasureGroup selection. Returns them
   *  in ProblemRecord::inputVariableRecords order. */
  std::vector<MeasureRecord> measureRecords() const;

  /** Returns the continuous variable values associated with this DataPoint. Returns them in 
   *  ProblemRecord::inputVariableRecords order. */
  std::vector<DataPointValueRecord> continuousVariableValueRecords() const;

  std::vector<DataPointValueRecord> responseValueRecords() const;

  std::vector<double> responseValues() const;

  boost::optional<FileReferenceRecord> osmInputDataRecord() const;

  boost::optional<FileReferenceRecord> idfInputDataRecord() const;

  boost::optional<FileReferenceRecord> sqlOutputDataRecord() const;

  /** \deprecated Will always return empty vector; call attributeRecords() instead. */
  std::vector<FileReferenceRecord> xmlOutputDataRecords() const;

  boost::optional<openstudio::UUID> topLevelJobUUID() const;

  std::vector<TagRecord> tagRecords() const;

  /** Returns this DataPointRecord's attributeRecords. */
  std::vector<AttributeRecord> attributeRecords() const;

  analysis::DataPoint dataPoint() const;

  //@}
  /** @name Setters */
  //@{

  void setDirectory(const openstudio::path& directory);

  /** Provided for callers operating directly on the database, not holding a copy of this
   *  analysis in memory. Use with caution. Does not do file system cleanup. */
  void clearResults();

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class detail::Record_Impl;
  friend class ProjectDatabase;
  friend class detail::DataPointRecord_Impl;

  /** Construct from impl. */
  DataPointRecord(std::shared_ptr<detail::DataPointRecord_Impl> impl,
                  ProjectDatabase database);


  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DataPointRecord(std::shared_ptr<detail::DataPointRecord_Impl> impl);

  /// @endcond

  void constructRelatedRecords(const analysis::DataPoint& dataPoint,
                               AnalysisRecord& analysisRecord,
                               const ProblemRecord& problemRecord);

 private:
  REGISTER_LOGGER("openstudio.project.DataPointRecord");

  DataPointRecord(const analysis::DataPoint& dataPoint,
                  AnalysisRecord& analysisRecord,
                  const ProblemRecord& problemRecord);

  DataPointRecord(const QSqlQuery& query, ProjectDatabase& database);

  boost::optional<FileReferenceRecord> saveChildFileReference(
      const boost::optional<FileReference>& childFileReference,
      boost::optional<FileReferenceRecord> oldFileReferenceRecord,
      DataPointRecord& copyOfThis,
      ProjectDatabase& database,
      bool isNew);
};

/** \relates DataPointRecord*/
typedef boost::optional<DataPointRecord> OptionalDataPointRecord;

/** \relates DataPointRecord*/
typedef std::vector<DataPointRecord> DataPointRecordVector;

} // project
} // openstudio

#endif // PROJECT_DATAPOINTRECORD_HPP

