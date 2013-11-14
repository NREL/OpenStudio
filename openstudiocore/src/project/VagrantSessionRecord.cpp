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

#include <project/VagrantSessionRecord.hpp>
#include <project/VagrantSessionRecord_Impl.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/UrlRecord.hpp>

#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace project {

namespace detail {

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const VagrantSession& vagrantSession,
                                                       ProjectDatabase& database)
    : CloudSessionRecord_Impl(vagrantSession, 
                              CloudSessionRecordType::VagrantSessionRecord, 
                              database)
  {}

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSessionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  void VagrantSessionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<VagrantSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  CloudSession VagrantSessionRecord_Impl::cloudSession() const {
    return vagrantSession().cast<CloudSession>();
  }

  VagrantSession VagrantSessionRecord_Impl::vagrantSession() const {
    OptionalUrl serverUrl;
    if (OptionalUrlRecord sur = serverUrlRecord()) {
      serverUrl = sur->url();
    }
    UrlVector workerUrls;
    BOOST_FOREACH(const UrlRecord& wur,workerUrlRecords()) {
      workerUrls.push_back(wur.url());
    }
    return VagrantSession(handle(),
                          uuidLast(),
                          sessionId(),
                          serverUrl,
                          workerUrls);
  }

  void VagrantSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSessionRecord_Impl::bindValues(query);
  }

  void VagrantSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSessionRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool VagrantSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSessionRecord_Impl::compareValues(query);

    return result;
  }

  void VagrantSessionRecord_Impl::saveLastValues() {
    CloudSessionRecord_Impl::saveLastValues();
  }

  void VagrantSessionRecord_Impl::revertToLastValues() {
    CloudSessionRecord_Impl::revertToLastValues();
  }

} // detail

VagrantSessionRecord::VagrantSessionRecord(const VagrantSession& vagrantSession, 
                                           ProjectDatabase& database)
  : CloudSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl>(
        new detail::VagrantSessionRecord_Impl(vagrantSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
  constructRelatedRecords(vagrantSession,database);
}

VagrantSessionRecord::VagrantSessionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl>(
        new detail::VagrantSessionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
}

boost::optional<VagrantSessionRecord> VagrantSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalVagrantSessionRecord result;

  try {
    result = VagrantSessionRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct VagrantSessionRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecords(ProjectDatabase& database) {
  std::vector<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(VagrantSessionRecord(query, database));
  }

  return result;
}

boost::optional<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType AND id=:id"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = VagrantSessionRecord(query, database);
  }

  return result;
}

VagrantSession VagrantSessionRecord::vagrantSession() const {
  return getImpl<detail::VagrantSessionRecord_Impl>()->vagrantSession();
}

/// @cond
VagrantSessionRecord::VagrantSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl> impl)
  : CloudSessionRecord(impl)
{}

VagrantSessionRecord::VagrantSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl> impl,
                                           ProjectDatabase database)
  : CloudSessionRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

