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

#include <project/AWSSettingsRecord.hpp>
#include <project/AWSSettingsRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/AWSSettings.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  AWSSettingsRecord_Impl::AWSSettingsRecord_Impl(const NAMESPACE::AWSSettings& aWSSettings,
                                                 const AWSSettingsRecordType& aWSSettingsRecordType,
                                                 ProjectDatabase& database)
    : CloudSettingsRecord_Impl(aWSSettings, database),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_aWSSettingsRecordType(aWSSettingsRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  AWSSettingsRecord_Impl::AWSSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSettingsRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(AWSSettingsRecord::ColumnsType::aWSSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_aWSSettingsRecordType = AWSSettingsRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  boost::optional<ObjectRecord> AWSSettingsRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> AWSSettingsRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> AWSSettingsRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> AWSSettingsRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void AWSSettingsRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase>& database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AWSSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::AWSSettings AWSSettingsRecord::aWSSettings() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void AWSSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSettingsRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(AWSSettingsRecord::ColumnsType::aWSSettingsRecordType,m_aWSSettingsRecordType.value());
    // Template for required data.
    // query.bindValue(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void AWSSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSettingsRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(AWSSettingsRecord::ColumnsType::aWSSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAWSSettingsRecordType = AWSSettingsRecordType(value.toInt());

    // Template for required data.
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool AWSSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSettingsRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(AWSSettingsRecord::ColumnsType::aWSSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_aWSSettingsRecordType == AWSSettingsRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(AWSSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void AWSSettingsRecord_Impl::saveLastValues() {
    CloudSettingsRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastAWSSettingsRecordType = m_aWSSettingsRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void AWSSettingsRecord_Impl::revertToLastValues() {
    CloudSettingsRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_aWSSettingsRecordType = m_lastAWSSettingsRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

AWSSettingsRecord::AWSSettingsRecord(const NAMESPACE::AWSSettings& aWSSettings, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::AWSSettingsRecord_Impl>(
        new detail::AWSSettingsRecord_Impl(aWSSettings, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

AWSSettingsRecord::AWSSettingsRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::AWSSettingsRecord_Impl>(
        new detail::AWSSettingsRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());
}

boost::optional<AWSSettingsRecord> AWSSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAWSSettingsRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int aWSSettingsRecordType = query.value(AWSSettingsRecordColumns::aWSSettingsRecordType).toInt();

  // switch (aWSSettingsRecordType) {
  //   case AWSSettingsRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<AWSSettingsRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown AWSSettingsRecordType " << aWSSettingsRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = AWSSettingsRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct AWSSettingsRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

AWSSettingsRecord AWSSettingsRecord::factoryFromAWSSettings(const NAMESPACE::AWSSettings& aWSSettings, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (aWSSettings.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(aWSSettings.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return AWSSettingsRecord(boost::shared_ptr<detail::AWSSettingsRecord_Impl>());
}

std::vector<AWSSettingsRecord> AWSSettingsRecord::getAWSSettingsRecords(ProjectDatabase& database) {
  std::vector<AWSSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::AWSSettingsRecord);
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalAWSSettingsRecord aWSSettingsRecord = AWSSettingsRecord::factoryFromQuery(query, database);
    // if (aWSSettingsRecord) {
    //   result.push_back(*aWSSettingsRecord);
    // }

    // result.push_back(AWSSettingsRecord(query, database));
  }

  return result;
}

boost::optional<AWSSettingsRecord> AWSSettingsRecord::getAWSSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<AWSSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType AND id=:id"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::AWSSettingsRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = AWSSettingsRecord::factoryFromQuery(query, database);

    // result = AWSSettingsRecord(query, database);
  }

  return result;
}

NAMESPACE::AWSSettings AWSSettingsRecord::aWSSettings() const {
  return getImpl<detail::AWSSettingsRecord_Impl>()->aWSSettings();
}

/// @cond
AWSSettingsRecord::AWSSettingsRecord(boost::shared_ptr<detail::AWSSettingsRecord_Impl> impl)
  : CloudSettingsRecord(impl)
{}

AWSSettingsRecord::AWSSettingsRecord(boost::shared_ptr<detail::AWSSettingsRecord_Impl> impl,
                                     ProjectDatabase database)
  : CloudSettingsRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

