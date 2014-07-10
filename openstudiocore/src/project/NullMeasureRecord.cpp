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

#include "NullMeasureRecord.hpp"
#include "NullMeasureRecord_Impl.hpp"
#include "MeasureGroupRecord.hpp"
#include "MeasureGroupRecord_Impl.hpp"
#include "JoinRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../analysis/Measure.hpp"
#include "../analysis/Measure_Impl.hpp"
#include "../analysis/NullMeasure.hpp"

#include "../utilities/core/Assert.hpp"

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail{

  NullMeasureRecord_Impl::NullMeasureRecord_Impl(const analysis::NullMeasure& nullMeasure,
                                                 MeasureGroupRecord& measureGroupRecord,
                                                 int measureVectorIndex)
    : MeasureRecord_Impl(nullMeasure,
                         MeasureRecordType::NullMeasureRecord,
                         measureGroupRecord,
                         measureVectorIndex)
  {}

  NullMeasureRecord_Impl::NullMeasureRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : MeasureRecord_Impl(query, database)
  {}

  void NullMeasureRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<NullMeasureRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Measure NullMeasureRecord_Impl::measure() const {
    return nullMeasure().cast<analysis::Measure>();
  }

  analysis::NullMeasure NullMeasureRecord_Impl::nullMeasure() const {
    return analysis::NullMeasure(handle(),
                                 uuidLast(),
                                 name(),
                                 displayName(),
                                 description(),
                                 isSelected());
  }

  void NullMeasureRecord_Impl::bindValues(QSqlQuery& query) const
  {
    MeasureRecord_Impl::bindValues(query);
  }

  void NullMeasureRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    MeasureRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool NullMeasureRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    return MeasureRecord_Impl::compareValues(query);
  }

  void NullMeasureRecord_Impl::saveLastValues()
  {
    MeasureRecord_Impl::saveLastValues();
  }

  void NullMeasureRecord_Impl::revertToLastValues()
  {
    MeasureRecord_Impl::revertToLastValues();
  }

} // detail

NullMeasureRecord::NullMeasureRecord(const analysis::NullMeasure& nullMeasure,
                                     MeasureGroupRecord& measureGroupRecord,
                                     int measureVectorIndex)
  : MeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl>(
        new detail::NullMeasureRecord_Impl(nullMeasure,
                                           measureGroupRecord,
                                           measureVectorIndex)),
        measureGroupRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::NullMeasureRecord_Impl>());
}

NullMeasureRecord::NullMeasureRecord(const QSqlQuery& query, ProjectDatabase& database)
  : MeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl>(
        new detail::NullMeasureRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::NullMeasureRecord_Impl>());
}

NullMeasureRecord::NullMeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl> impl, ProjectDatabase projectDatabase)
  : MeasureRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::NullMeasureRecord_Impl>());
}

boost::optional<NullMeasureRecord> NullMeasureRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalNullMeasureRecord result;
  try {
    result = NullMeasureRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct NullMeasureRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<NullMeasureRecord> NullMeasureRecord::getNullMeasureRecords(ProjectDatabase& database) {
  std::vector<NullMeasureRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() +
      " WHERE measureRecordType=:measureRecordType"));
  query.bindValue(":measureRecordType", MeasureRecordType::NullMeasureRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(NullMeasureRecord(query, database));
  }

  return result;
}

boost::optional<NullMeasureRecord> NullMeasureRecord::getNullMeasureRecord(int id, ProjectDatabase& database) {
  boost::optional<NullMeasureRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() +
      " WHERE measureRecordType=:measureRecordType AND id=:id"));
  query.bindValue(":measureRecordType", MeasureRecordType::NullMeasureRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = NullMeasureRecord(query, database);
  }

  return result;
}

analysis::NullMeasure NullMeasureRecord::nullMeasure() const {
  return getImpl<detail::NullMeasureRecord_Impl>()->nullMeasure();
}

/// @cond
NullMeasureRecord::NullMeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl> impl)
  : MeasureRecord(impl)
{
  OS_ASSERT(getImpl<detail::NullMeasureRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

