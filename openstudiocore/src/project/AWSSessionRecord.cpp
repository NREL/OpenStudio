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

#include <project/AWSSessionRecord.hpp>
#include <project/AWSSessionRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/AWSSession.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  AWSSessionRecord_Impl::AWSSessionRecord_Impl(const NAMESPACE::AWSSession& aWSSession,
                                               const AWSSessionRecordType& aWSSessionRecordType,
                                               ProjectDatabase& database)
    : CloudSessionRecord_Impl(aWSSession, database),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_aWSSessionRecordType(aWSSessionRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  AWSSessionRecord_Impl::AWSSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSessionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(AWSSessionRecord::ColumnsType::aWSSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_aWSSessionRecordType = AWSSessionRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  boost::optional<ObjectRecord> AWSSessionRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> AWSSessionRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> AWSSessionRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> AWSSessionRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void AWSSessionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase>& database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AWSSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::AWSSession AWSSessionRecord::aWSSession() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void AWSSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSessionRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(AWSSessionRecord::ColumnsType::aWSSessionRecordType,m_aWSSessionRecordType.value());
    // Template for required data.
    // query.bindValue(AWSSessionRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(AWSSessionRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(AWSSessionRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void AWSSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSessionRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(AWSSessionRecord::ColumnsType::aWSSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAWSSessionRecordType = AWSSessionRecordType(value.toInt());

    // Template for required data.
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool AWSSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSessionRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(AWSSessionRecord::ColumnsType::aWSSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_aWSSessionRecordType == AWSSessionRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(AWSSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void AWSSessionRecord_Impl::saveLastValues() {
    CloudSessionRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastAWSSessionRecordType = m_aWSSessionRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void AWSSessionRecord_Impl::revertToLastValues() {
    CloudSessionRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_aWSSessionRecordType = m_lastAWSSessionRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

AWSSessionRecord::AWSSessionRecord(const NAMESPACE::AWSSession& aWSSession, ProjectDatabase& database)
  : CloudSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl>(
        new detail::AWSSessionRecord_Impl(aWSSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

AWSSessionRecord::AWSSessionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl>(
        new detail::AWSSessionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());
}

boost::optional<AWSSessionRecord> AWSSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAWSSessionRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int aWSSessionRecordType = query.value(AWSSessionRecordColumns::aWSSessionRecordType).toInt();

  // switch (aWSSessionRecordType) {
  //   case AWSSessionRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<AWSSessionRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown AWSSessionRecordType " << aWSSessionRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = AWSSessionRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct AWSSessionRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

AWSSessionRecord AWSSessionRecord::factoryFromAWSSession(const NAMESPACE::AWSSession& aWSSession, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (aWSSession.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(aWSSession.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return AWSSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl>());
}

std::vector<AWSSessionRecord> AWSSessionRecord::getAWSSessionRecords(ProjectDatabase& database) {
  std::vector<AWSSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::AWSSessionRecord);
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalAWSSessionRecord aWSSessionRecord = AWSSessionRecord::factoryFromQuery(query, database);
    // if (aWSSessionRecord) {
    //   result.push_back(*aWSSessionRecord);
    // }

    // result.push_back(AWSSessionRecord(query, database));
  }

  return result;
}

boost::optional<AWSSessionRecord> AWSSessionRecord::getAWSSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<AWSSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType AND id=:id"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::AWSSessionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = AWSSessionRecord::factoryFromQuery(query, database);

    // result = AWSSessionRecord(query, database);
  }

  return result;
}

NAMESPACE::AWSSession AWSSessionRecord::aWSSession() const {
  return getImpl<detail::AWSSessionRecord_Impl>()->aWSSession();
}

/// @cond
AWSSessionRecord::AWSSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl> impl)
  : CloudSessionRecord(impl)
{}

AWSSessionRecord::AWSSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl> impl,
                                   ProjectDatabase database)
  : CloudSessionRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

