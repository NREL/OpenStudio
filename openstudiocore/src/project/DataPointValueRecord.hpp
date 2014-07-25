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

