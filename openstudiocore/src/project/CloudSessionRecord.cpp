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

#include <project/CloudSessionRecord.hpp>
#include <project/CloudSessionRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/CloudSession.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  CloudSessionRecord_Impl::CloudSessionRecord_Impl(const NAMESPACE::CloudSession& cloudSession,
                                                   const CloudSessionRecordType& cloudSessionRecordType,
                                                   ProjectDatabase& database)
    : ObjectRecord_Impl(database,cloudSession.uuid(),cloudSession.name(),cloudSession.name(),"",cloudSession.versionUUID()),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_cloudSessionRecordType(cloudSessionRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  CloudSessionRecord_Impl::CloudSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_cloudSessionRecordType = CloudSessionRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  std::string CloudSessionRecord_Impl::databaseTableName() const {
    return CloudSessionRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> CloudSessionRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> CloudSessionRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> CloudSessionRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> CloudSessionRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void CloudSessionRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<CloudSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::CloudSession CloudSessionRecord::cloudSession() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void CloudSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(CloudSessionRecord::ColumnsType::cloudSessionRecordType,m_cloudSessionRecordType.value());
    // Template for required data.
    // query.bindValue(CloudSessionRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(CloudSessionRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(CloudSessionRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void CloudSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastCloudSessionRecordType = CloudSessionRecordType(value.toInt());

    // Template for required data.
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool CloudSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_cloudSessionRecordType == CloudSessionRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(CloudSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void CloudSessionRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastCloudSessionRecordType = m_cloudSessionRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void CloudSessionRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_cloudSessionRecordType = m_lastCloudSessionRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

CloudSessionRecord::CloudSessionRecord(const NAMESPACE::CloudSession& cloudSession, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl>(
        new detail::CloudSessionRecord_Impl(cloudSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::CloudSessionRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

CloudSessionRecord::CloudSessionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl>(
        new detail::CloudSessionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::CloudSessionRecord_Impl>());
}

std::string CloudSessionRecord::databaseTableName() {
  return "CloudSessionRecords";
}

void CloudSessionRecord::updatePathData(ProjectDatabase database,
                                        const openstudio::path& originalBase,
                                        const openstudio::path& newBase)
{
  // TODO: Delete implementation if no classes in this hierarchy contain path data.
  // TODO: Override virtual ObjectRecord_Impl method updatePathData for any classes in this
  // hierarchy that do contain path data. See ProjectDatabaseRecord_Impl for an example. 
  CloudSessionRecordVector records = getCloudSessionRecords(database);
  BOOST_FOREACH(CloudSessionRecord& record,records) {
    record.getImpl<detail::CloudSessionRecord_Impl>()->updatePathData(originalBase,newBase);
  }
}

boost::optional<CloudSessionRecord> CloudSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalCloudSessionRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int cloudSessionRecordType = query.value(CloudSessionRecordColumns::cloudSessionRecordType).toInt();

  // switch (cloudSessionRecordType) {
  //   case CloudSessionRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<CloudSessionRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown CloudSessionRecordType " << cloudSessionRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = CloudSessionRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct CloudSessionRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

CloudSessionRecord CloudSessionRecord::factoryFromCloudSession(const NAMESPACE::CloudSession& cloudSession, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (cloudSession.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(cloudSession.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl>());
}

std::vector<CloudSessionRecord> CloudSessionRecord::getCloudSessionRecords(ProjectDatabase& database) {
  std::vector<CloudSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalCloudSessionRecord cloudSessionRecord = CloudSessionRecord::factoryFromQuery(query, database);
    // if (cloudSessionRecord) {
    //   result.push_back(*cloudSessionRecord);
    // }

    // result.push_back(CloudSessionRecord(query, database));
  }

  return result;
}

boost::optional<CloudSessionRecord> CloudSessionRecord::getCloudSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<CloudSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = CloudSessionRecord::factoryFromQuery(query, database);

    // result = CloudSessionRecord(query, database);
  }

  return result;
}

NAMESPACE::CloudSession CloudSessionRecord::cloudSession() const {
  return getImpl<detail::CloudSessionRecord_Impl>()->cloudSession();
}

/// @cond
CloudSessionRecord::CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl)
  : ObjectRecord(impl)
{}

CloudSessionRecord::CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl,
                                       ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERTgetImpl<detail::CloudSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

