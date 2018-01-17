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

