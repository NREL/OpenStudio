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

#include <project/OptimizationDataPointRecord.hpp>
#include <project/OptimizationDataPointRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
#include <project/AnalysisRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/DataPointValueRecord.hpp>
#include <project/FunctionRecord.hpp>
#include <project/OptimizationProblemRecord.hpp>
#include <project/OptimizationProblemRecord_Impl.hpp>

#include <analysis/Problem.hpp>
#include <analysis/OptimizationProblem.hpp>
#include <analysis/OptimizationProblem_Impl.hpp>
#include <analysis/OptimizationDataPoint.hpp>
#include <analysis/OptimizationDataPoint_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/FileReference.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace project {

namespace detail {

  OptimizationDataPointRecord_Impl::OptimizationDataPointRecord_Impl(
      const analysis::OptimizationDataPoint& optimizationDataPoint,
      AnalysisRecord& analysisRecord,
      const OptimizationProblemRecord& problemRecord)
    : DataPointRecord_Impl(optimizationDataPoint,
                           DataPointRecordType::OptimizationDataPointRecord,
                           analysisRecord,
                           problemRecord)
  {}

  OptimizationDataPointRecord_Impl::OptimizationDataPointRecord_Impl(const QSqlQuery& query,
                                                                     ProjectDatabase& database)
    : DataPointRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  std::vector<ObjectRecord> OptimizationDataPointRecord_Impl::children() const {
    ObjectRecordVector result = DataPointRecord_Impl::children();
    DataPointValueRecordVector objectiveValueRecords = this->objectiveValueRecords();
    result.insert(result.end(),
                  objectiveValueRecords.begin(),
                  objectiveValueRecords.end());
    return result;
  }

  void OptimizationDataPointRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OptimizationDataPointRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::DataPoint OptimizationDataPointRecord_Impl::dataPoint() const {
    return optimizationDataPoint().cast<analysis::DataPoint>();
  }

  void OptimizationDataPointRecord_Impl::clearResults() {
    ProjectDatabase database = projectDatabase();
    DataPointValueRecordVector ovrs = objectiveValueRecords();
    BOOST_FOREACH(DataPointValueRecord& ovr,ovrs) {
      database.removeRecord(ovr);
    }
    DataPointRecord_Impl::clearResults();
  }

  std::vector<DataPointValueRecord>
  OptimizationDataPointRecord_Impl::objectiveValueRecords() const
  {
    DataPointValueRecordVector result;
    ProjectDatabase database = projectDatabase();
    FunctionRecordVector objectives =
        this->problemRecord().cast<OptimizationProblemRecord>().objectiveRecords();
    BOOST_FOREACH(const FunctionRecord& function,objectives) {
      QSqlQuery query(*(database.qSqlDatabase()));
      query.prepare(toQString("SELECT * FROM " + DataPointValueRecord::databaseTableName() +
          " WHERE dataPointRecordId=:dataPointRecordId AND functionRecordId=:functionRecordId "));
      query.bindValue(":dataPointRecordId",id());
      query.bindValue(":functionRecordId",function.id());
      assertExec(query);
      if (query.first()) {
        result.push_back(DataPointValueRecord(query,database));
      }
    }
    return result;
  }

  std::vector<double> OptimizationDataPointRecord_Impl::objectiveValues() const {
    DoubleVector result;
    DataPointValueRecordVector valueRecords = objectiveValueRecords();
    BOOST_FOREACH(const DataPointValueRecord& valueRecord,valueRecords) {
      result.push_back(valueRecord.dataPointValue());
    }
    return result;
  }

  analysis::OptimizationDataPoint OptimizationDataPointRecord_Impl::optimizationDataPoint() const {
    analysis::DataPoint prelim = DataPointRecord_Impl::dataPoint();
    return analysis::OptimizationDataPoint(prelim.uuid(),
                                           prelim.versionUUID(),
                                           prelim.name(),
                                           prelim.displayName(),
                                           prelim.description(),
                                           prelim.problem().cast<analysis::OptimizationProblem>(),
                                           prelim.complete(),
                                           prelim.failed(),
                                           prelim.selected(),
                                           prelim.runType(),
                                           prelim.variableValues(),
                                           responseValues(),
                                           objectiveValues(),
                                           prelim.directory(),
                                           prelim.osmInputData(),
                                           prelim.idfInputData(),
                                           prelim.sqlOutputData(),
                                           prelim.xmlOutputData(),
                                           prelim.topLevelJob(),
                                           prelim.dakotaParametersFiles(),
                                           prelim.tags(),
                                           prelim.outputAttributes());
  }

  void OptimizationDataPointRecord_Impl::bindValues(QSqlQuery& query) const {
    DataPointRecord_Impl::bindValues(query);
  }

  void OptimizationDataPointRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DataPointRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool OptimizationDataPointRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DataPointRecord_Impl::compareValues(query);

    return result;
  }

  void OptimizationDataPointRecord_Impl::saveLastValues() {
    DataPointRecord_Impl::saveLastValues();
  }

  void OptimizationDataPointRecord_Impl::revertToLastValues() {
    DataPointRecord_Impl::revertToLastValues();
  }

} // detail

OptimizationDataPointRecord::OptimizationDataPointRecord(
    const analysis::OptimizationDataPoint& optimizationDataPoint,
    AnalysisRecord& analysisRecord,
    const OptimizationProblemRecord& problemRecord)
  : DataPointRecord(boost::shared_ptr<detail::OptimizationDataPointRecord_Impl>(
        new detail::OptimizationDataPointRecord_Impl(optimizationDataPoint,
                                                     analysisRecord,
                                                     problemRecord)),
        analysisRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::OptimizationDataPointRecord_Impl>());

  // base class related records
  constructRelatedRecords(optimizationDataPoint,analysisRecord,problemRecord);

  OptimizationDataPointRecord copyOfThis(getImpl<detail::OptimizationDataPointRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  // remove old objetive function values
  DataPointValueRecordVector valueRecords = copyOfThis.objectiveValueRecords();
  BOOST_FOREACH(DataPointValueRecord& valueRecord,valueRecords) {
    database.removeRecord(valueRecord);
  }
  // save current objective function values
  FunctionRecordVector objectives = problemRecord.cast<OptimizationProblemRecord>().objectiveRecords();
  DoubleVector values = optimizationDataPoint.objectiveValues();
  if (!values.empty()) {
    int n = objectives.size();
    OS_ASSERT(values.size() == static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
      DataPointValueRecord newValue(values[i],copyOfThis,objectives[i]);
    }
  }
}

OptimizationDataPointRecord::OptimizationDataPointRecord(const QSqlQuery& query,
                                                         ProjectDatabase& database)
  : DataPointRecord(boost::shared_ptr<detail::OptimizationDataPointRecord_Impl>(
        new detail::OptimizationDataPointRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OptimizationDataPointRecord_Impl>());
}

OptimizationDataPointRecord::OptimizationDataPointRecord(
    boost::shared_ptr<detail::OptimizationDataPointRecord_Impl> impl,
    ProjectDatabase database)
  : DataPointRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OptimizationDataPointRecord_Impl>());
}

boost::optional<OptimizationDataPointRecord> OptimizationDataPointRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalOptimizationDataPointRecord result;
  try {
    result = OptimizationDataPointRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct OptimizationDataPointRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<OptimizationDataPointRecord> OptimizationDataPointRecord::getOptimizationDataPointRecords(
    ProjectDatabase& database)
{
  std::vector<OptimizationDataPointRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
      " WHERE dataPointRecordType=:dataPointRecordType"));
  query.bindValue(":dataPointRecordType", DataPointRecordType::OptimizationDataPointRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(OptimizationDataPointRecord(query, database));
  }

  return result;
}

boost::optional<OptimizationDataPointRecord> OptimizationDataPointRecord::getOptimizationDataPointRecord(int id, ProjectDatabase& database) {
  boost::optional<OptimizationDataPointRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
      " WHERE dataPointRecordType=:dataPointRecordType AND id=:id"));
  query.bindValue(":dataPointRecordType", DataPointRecordType::OptimizationDataPointRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OptimizationDataPointRecord(query, database);
  }

  return result;
}

std::vector<DataPointValueRecord> OptimizationDataPointRecord::objectiveValueRecords() const
{
  return getImpl<detail::OptimizationDataPointRecord_Impl>()->objectiveValueRecords();
}

std::vector<double> OptimizationDataPointRecord::objectiveValues() const {
  return getImpl<detail::OptimizationDataPointRecord_Impl>()->objectiveValues();
}

analysis::OptimizationDataPoint OptimizationDataPointRecord::optimizationDataPoint() const {
  return getImpl<detail::OptimizationDataPointRecord_Impl>()->optimizationDataPoint();
}

/// @cond
OptimizationDataPointRecord::OptimizationDataPointRecord(boost::shared_ptr<detail::OptimizationDataPointRecord_Impl> impl)
  : DataPointRecord(impl)
{}
/// @endcond

} // project
} // openstudio

