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

#include <project/CloudSettingsRecord.hpp>
#include <project/CloudSettingsRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/CloudSettings.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  CloudSettingsRecord_Impl::CloudSettingsRecord_Impl(const NAMESPACE::CloudSettings& cloudSettings,
                                                     const CloudSettingsRecordType& cloudSettingsRecordType,
                                                     ProjectDatabase& database)
    : ObjectRecord_Impl(database,cloudSettings.uuid(),cloudSettings.name(),cloudSettings.name(),"",cloudSettings.versionUUID()),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_cloudSettingsRecordType(cloudSettingsRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  CloudSettingsRecord_Impl::CloudSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_cloudSettingsRecordType = CloudSettingsRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  std::string CloudSettingsRecord_Impl::databaseTableName() const {
    return CloudSettingsRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> CloudSettingsRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> CloudSettingsRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> CloudSettingsRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> CloudSettingsRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void CloudSettingsRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<CloudSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::CloudSettings CloudSettingsRecord::cloudSettings() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void CloudSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType,m_cloudSettingsRecordType.value());
    // Template for required data.
    // query.bindValue(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void CloudSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastCloudSettingsRecordType = CloudSettingsRecordType(value.toInt());

    // Template for required data.
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool CloudSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_cloudSettingsRecordType == CloudSettingsRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(CloudSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void CloudSettingsRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastCloudSettingsRecordType = m_cloudSettingsRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void CloudSettingsRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_cloudSettingsRecordType = m_lastCloudSettingsRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

CloudSettingsRecord::CloudSettingsRecord(const NAMESPACE::CloudSettings& cloudSettings, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl>(
        new detail::CloudSettingsRecord_Impl(cloudSettings, database)),
        database)
{
  OS_ASSERT(getImpl<detail::CloudSettingsRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

CloudSettingsRecord::CloudSettingsRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl>(
        new detail::CloudSettingsRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::CloudSettingsRecord_Impl>());
}

std::string CloudSettingsRecord::databaseTableName() {
  return "CloudSettingsRecords";
}

void CloudSettingsRecord::updatePathData(ProjectDatabase database,
                                         const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
{
  // TODO: Delete implementation if no classes in this hierarchy contain path data.
  // TODO: Override virtual ObjectRecord_Impl method updatePathData for any classes in this
  // hierarchy that do contain path data. See ProjectDatabaseRecord_Impl for an example. 
  CloudSettingsRecordVector records = getCloudSettingsRecords(database);
  BOOST_FOREACH(CloudSettingsRecord& record,records) {
    record.getImpl<detail::CloudSettingsRecord_Impl>()->updatePathData(originalBase,newBase);
  }
}

boost::optional<CloudSettingsRecord> CloudSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalCloudSettingsRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int cloudSettingsRecordType = query.value(CloudSettingsRecordColumns::cloudSettingsRecordType).toInt();

  // switch (cloudSettingsRecordType) {
  //   case CloudSettingsRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<CloudSettingsRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown CloudSettingsRecordType " << cloudSettingsRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = CloudSettingsRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct CloudSettingsRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

CloudSettingsRecord CloudSettingsRecord::factoryFromCloudSettings(const NAMESPACE::CloudSettings& cloudSettings, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (cloudSettings.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(cloudSettings.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl>());
}

std::vector<CloudSettingsRecord> CloudSettingsRecord::getCloudSettingsRecords(ProjectDatabase& database) {
  std::vector<CloudSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalCloudSettingsRecord cloudSettingsRecord = CloudSettingsRecord::factoryFromQuery(query, database);
    // if (cloudSettingsRecord) {
    //   result.push_back(*cloudSettingsRecord);
    // }

    // result.push_back(CloudSettingsRecord(query, database));
  }

  return result;
}

boost::optional<CloudSettingsRecord> CloudSettingsRecord::getCloudSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<CloudSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = CloudSettingsRecord::factoryFromQuery(query, database);

    // result = CloudSettingsRecord(query, database);
  }

  return result;
}

NAMESPACE::CloudSettings CloudSettingsRecord::cloudSettings() const {
  return getImpl<detail::CloudSettingsRecord_Impl>()->cloudSettings();
}

/// @cond
CloudSettingsRecord::CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl)
  : ObjectRecord(impl)
{}

CloudSettingsRecord::CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl,
                                         ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERTgetImpl<detail::CloudSettingsRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

