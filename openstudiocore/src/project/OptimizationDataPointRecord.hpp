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

#ifndef PROJECT_OPTIMIZATIONDATAPOINTRECORD_HPP
#define PROJECT_OPTIMIZATIONDATAPOINTRECORD_HPP

#include "ProjectAPI.hpp"
#include "DataPointRecord.hpp"

namespace openstudio {
namespace analysis {
  class OptimizationDataPoint;
}
namespace project {

class DataPointValueRecord;
class OptimizationProblemRecord;

namespace detail {

  class OptimizationDataPointRecord_Impl;

} // detail

/** OptimizationDataPointRecord is a DataPointRecord*/
class PROJECT_API OptimizationDataPointRecord : public DataPointRecord {
 public:

  typedef detail::OptimizationDataPointRecord_Impl ImplType;
  typedef DataPointRecordColumns ColumnsType;
  typedef DataPointRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OptimizationDataPointRecord(const analysis::OptimizationDataPoint& optimizationDataPoint, 
                              AnalysisRecord& analysisRecord, 
                              const OptimizationProblemRecord& problemRecord);

  OptimizationDataPointRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~OptimizationDataPointRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<OptimizationDataPointRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                       ProjectDatabase& database);

  static std::vector<OptimizationDataPointRecord> getOptimizationDataPointRecords(
      ProjectDatabase& database);

  static boost::optional<OptimizationDataPointRecord> getOptimizationDataPointRecord(
      int id, ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  std::vector<DataPointValueRecord> objectiveValueRecords() const;

  std::vector<double> objectiveValues() const;

  analysis::OptimizationDataPoint optimizationDataPoint() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::OptimizationDataPointRecord_Impl;

  /** Construct from impl. */
  OptimizationDataPointRecord(std::shared_ptr<detail::OptimizationDataPointRecord_Impl> impl,
                              ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit OptimizationDataPointRecord(std::shared_ptr<detail::OptimizationDataPointRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.OptimizationDataPointRecord");
};

/** \relates OptimizationDataPointRecord*/
typedef boost::optional<OptimizationDataPointRecord> OptionalOptimizationDataPointRecord;

/** \relates OptimizationDataPointRecord*/
typedef std::vector<OptimizationDataPointRecord> OptimizationDataPointRecordVector;

} // project
} // openstudio

#endif // PROJECT_OPTIMIZATIONDATAPOINTRECORD_HPP

