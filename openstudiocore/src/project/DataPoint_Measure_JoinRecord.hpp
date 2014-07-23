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

#ifndef PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP
#define PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP

#include "ProjectAPI.hpp"
#include "JoinRecord.hpp"

namespace openstudio {
namespace project {

class DataPointRecord;
class MeasureRecord;

namespace detail {

  class DataPoint_Measure_JoinRecord_Impl;

} // detail

/** DataPoint_Measure_JoinRecord is a JoinRecord*/
class PROJECT_API DataPoint_Measure_JoinRecord : public JoinRecord {
 public:

  typedef detail::DataPoint_Measure_JoinRecord_Impl ImplType;
  typedef JoinRecordColumns ColumnsType;

  /** @name Constructors and Destructors */
  //@{

  DataPoint_Measure_JoinRecord(const DataPointRecord& dataPointRecord,
                               const MeasureRecord& measureRecord);


  virtual ~DataPoint_Measure_JoinRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static std::vector<MeasureRecord> getMeasureRecords(const DataPointRecord& dataPointRecord);

  static std::vector<DataPointRecord> getDataPointRecords(const MeasureRecord& measureRecord);

  //@}

  DataPointRecord dataPointRecord() const;

  MeasureRecord measureRecord() const;

 protected:
  /// @cond
  friend class Record;
  friend class JoinRecord;
  friend class ProjectDatabase;
  friend class detail::DataPoint_Measure_JoinRecord_Impl;

  /** Construct from impl. */
  DataPoint_Measure_JoinRecord(
      std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DataPoint_Measure_JoinRecord(
      std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DataPoint_Measure_JoinRecord");
};

/** \relates DataPoint_Measure_JoinRecord*/
typedef boost::optional<DataPoint_Measure_JoinRecord>
    OptionalDataPoint_Measure_JoinRecord;

/** \relates DataPoint_Measure_JoinRecord*/
typedef std::vector<DataPoint_Measure_JoinRecord>
    DataPoint_Measure_JoinRecordVector;

} // project
} // openstudio

#endif // PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP

