/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef PROJECT_DATAPOINT_DISCRETEPERTURBATION_JOINRECORD_HPP
#define PROJECT_DATAPOINT_DISCRETEPERTURBATION_JOINRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/JoinRecord.hpp>

namespace openstudio {
namespace project {

class DataPointRecord;
class DiscretePerturbationRecord;

namespace detail {

  class DataPoint_DiscretePerturbation_JoinRecord_Impl;

} // detail

/** DataPoint_DiscretePerturbation_JoinRecord is a JoinRecord*/
class PROJECT_API DataPoint_DiscretePerturbation_JoinRecord : public JoinRecord {
 public:

  typedef detail::DataPoint_DiscretePerturbation_JoinRecord_Impl ImplType;
  typedef JoinRecordColumns ColumnsType;

  /** @name Constructors and Destructors */
  //@{

  DataPoint_DiscretePerturbation_JoinRecord(
      const DataPointRecord& dataPointRecord, 
      const DiscretePerturbationRecord& discretePerturbationRecord);


  virtual ~DataPoint_DiscretePerturbation_JoinRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static std::vector<DiscretePerturbationRecord> getDiscretePerturbationRecords(
      const DataPointRecord& dataPointRecord);

  static std::vector<DataPointRecord> getDataPointRecords(
      const DiscretePerturbationRecord& discretePerturbationRecord);

  //@}

  DataPointRecord dataPointRecord() const;

  DiscretePerturbationRecord discretePerturbationRecord() const;

 protected:
  /// @cond
  friend class Record;
  friend class JoinRecord;
  friend class ProjectDatabase;
  friend class detail::DataPoint_DiscretePerturbation_JoinRecord_Impl;

  /** Construct from impl. */
  DataPoint_DiscretePerturbation_JoinRecord(
      boost::shared_ptr<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DataPoint_DiscretePerturbation_JoinRecord(
      boost::shared_ptr<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DataPoint_DiscretePerturbation_JoinRecord");
};

/** \relates DataPoint_DiscretePerturbation_JoinRecord*/
typedef boost::optional<DataPoint_DiscretePerturbation_JoinRecord> 
    OptionalDataPoint_DiscretePerturbation_JoinRecord;

/** \relates DataPoint_DiscretePerturbation_JoinRecord*/
typedef std::vector<DataPoint_DiscretePerturbation_JoinRecord> 
    DataPoint_DiscretePerturbation_JoinRecordVector;

} // project
} // openstudio

#endif // PROJECT_DATAPOINT_DISCRETEPERTURBATION_JOINRECORD_HPP

