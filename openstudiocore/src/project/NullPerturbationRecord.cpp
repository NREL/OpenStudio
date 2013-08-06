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

#include <project/NullPerturbationRecord.hpp>
#include <project/NullPerturbationRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/DiscreteVariableRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>
#include <analysis/NullPerturbation.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail{

  NullPerturbationRecord_Impl::NullPerturbationRecord_Impl(const analysis::NullPerturbation& nullPerturbation,
                                                           DiscreteVariableRecord& discreteVariableRecord,
                                                           int perturbationVectorIndex)
    : DiscretePerturbationRecord_Impl(nullPerturbation,
                                      DiscretePerturbationRecordType::NullPerturbationRecord,
                                      discreteVariableRecord,
                                      perturbationVectorIndex)
  {}

  NullPerturbationRecord_Impl::NullPerturbationRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : DiscretePerturbationRecord_Impl(query, database)
  {}

  void NullPerturbationRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<NullPerturbationRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::DiscretePerturbation NullPerturbationRecord_Impl::discretePerturbation() const {
    return nullPerturbation().cast<analysis::DiscretePerturbation>();
  }

  analysis::NullPerturbation NullPerturbationRecord_Impl::nullPerturbation() const {
    return analysis::NullPerturbation(handle(),
                                      uuidLast(),
                                      name(),
                                      displayName(),
                                      description(),
                                      isSelected());
  }

  void NullPerturbationRecord_Impl::bindValues(QSqlQuery& query) const
  {
    DiscretePerturbationRecord_Impl::bindValues(query);
  }

  void NullPerturbationRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    DiscretePerturbationRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool NullPerturbationRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    return DiscretePerturbationRecord_Impl::compareValues(query);
  }

  void NullPerturbationRecord_Impl::saveLastValues()
  {
    DiscretePerturbationRecord_Impl::saveLastValues();
  }

  void NullPerturbationRecord_Impl::revertToLastValues()
  {
    DiscretePerturbationRecord_Impl::revertToLastValues();
  }

} // detail

NullPerturbationRecord::NullPerturbationRecord(const analysis::NullPerturbation& nullPerturbation,
                       DiscreteVariableRecord& discreteVariableRecord,
                       int perturbationVectorIndex)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl>(
        new detail::NullPerturbationRecord_Impl(nullPerturbation,
                                                discreteVariableRecord,
                                                perturbationVectorIndex)),
        discreteVariableRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::NullPerturbationRecord_Impl>());
}

NullPerturbationRecord::NullPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl>(
        new detail::NullPerturbationRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::NullPerturbationRecord_Impl>());
}

NullPerturbationRecord::NullPerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl> impl, ProjectDatabase projectDatabase)
  : DiscretePerturbationRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::NullPerturbationRecord_Impl>());
}

boost::optional<NullPerturbationRecord> NullPerturbationRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalNullPerturbationRecord result;
  try {
    result = NullPerturbationRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct NullPerturbationRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<NullPerturbationRecord> NullPerturbationRecord::getNullPerturbationRecords(ProjectDatabase& database) {
  std::vector<NullPerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
      " WHERE discretePerturbationRecordType=:discretePerturbationRecordType"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::NullPerturbationRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(NullPerturbationRecord(query, database));
  }

  return result;
}

boost::optional<NullPerturbationRecord> NullPerturbationRecord::getNullPerturbationRecord(int id, ProjectDatabase& database) {
  boost::optional<NullPerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
      " WHERE discretePerturbationRecordType=:discretePerturbationRecordType AND id=:id"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::NullPerturbationRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = NullPerturbationRecord(query, database);
  }

  return result;
}

analysis::NullPerturbation NullPerturbationRecord::nullPerturbation() const {
  return getImpl<detail::NullPerturbationRecord_Impl>()->nullPerturbation();
}

/// @cond
NullPerturbationRecord::NullPerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl> impl)
  : DiscretePerturbationRecord(impl)
{
  OS_ASSERT(getImpl<detail::NullPerturbationRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

