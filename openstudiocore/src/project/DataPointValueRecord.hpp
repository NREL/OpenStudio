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

#ifndef PROJECT_DATAPOINTVALUERECORD_HPP
#define PROJECT_DATAPOINTVALUERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {
namespace project {

class DataPointRecord;
class FunctionRecord;
class ContinuousVariableRecord;

namespace detail {

  class DataPointValueRecord_Impl;

} // detail

/** \class DataPointValueRecordColumns
 *  \brief Column definitions for the DataPointValueRecords table.
 *
 *  \relates DataPointValueRecord */
OPENSTUDIO_ENUM(DataPointValueRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((dataPointRecordId)(INTEGER)(8))
  ((functionRecordId)(INTEGER)(9))
  ((continuousVariableRecordId)(INTEGER)(10))
  ((dataPointValue)(REAL)(11))
);

/** DataPointValueRecord is a ObjectRecord*/
class PROJECT_API DataPointValueRecord : public ObjectRecord {
 public:

  typedef detail::DataPointValueRecord_Impl ImplType;
  typedef DataPointValueRecordColumns ColumnsType;
  typedef DataPointValueRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  DataPointValueRecord(double dataPointValue, 
                       DataPointRecord& dataPointRecord,
                       FunctionRecord& functionRecord);

  DataPointValueRecord(double dataPointValue, 
                       DataPointRecord& dataPointRecord,
                       ContinuousVariableRecord& continuousVariableRecord);

  DataPointValueRecord(const QSqlQuery& query, ProjectDatabase& database);


  virtual ~DataPointValueRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<DataPointValueRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                ProjectDatabase& database);

  static std::vector<DataPointValueRecord> getDataPointValueRecords(ProjectDatabase& database);

  static boost::optional<DataPointValueRecord> getDataPointValueRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns the DataPointRecord that parents this value. */
  DataPointRecord dataPointRecord() const;

  /** Returns the FunctionRecord to which this value corresponds. */
  boost::optional<FunctionRecord> functionRecord() const;

  boost::optional<int> continuousVariableRecordId() const;

  /** Returns the ContinuousVariableRecord to which this value corresponds. */
  boost::optional<ContinuousVariableRecord> continuousVariableRecord() const;

  double dataPointValue() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::DataPointValueRecord_Impl;

  /** Construct from impl. */
  DataPointValueRecord(std::shared_ptr<detail::DataPointValueRecord_Impl> impl,
                       ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DataPointValueRecord(std::shared_ptr<detail::DataPointValueRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DataPointValueRecord");
};

/** \relates DataPointValueRecord*/
typedef boost::optional<DataPointValueRecord> OptionalDataPointValueRecord;

/** \relates DataPointValueRecord*/
typedef std::vector<DataPointValueRecord> DataPointValueRecordVector;

inline PROJECT_API bool continuousVariableRecordIdEquals(const DataPointValueRecord& record, int id){
  if (boost::optional<int> val = record.continuousVariableRecordId()) {
    return (*val == id);
  }
  return false;
};

} // project
} // openstudio

#endif // PROJECT_DATAPOINTVALUERECORD_HPP

