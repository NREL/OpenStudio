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

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/VagrantSession.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const NAMESPACE::VagrantSession& vagrantSession,
                                                       const VagrantSessionRecordType& vagrantSessionRecordType,
                                                       ProjectDatabase& database)
    : CloudSessionRecord_Impl(vagrantSession, database),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_vagrantSessionRecordType(vagrantSessionRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSessionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(VagrantSessionRecord::ColumnsType::vagrantSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_vagrantSessionRecordType = VagrantSessionRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  boost::optional<ObjectRecord> VagrantSessionRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> VagrantSessionRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> VagrantSessionRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> VagrantSessionRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void VagrantSessionRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<VagrantSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::VagrantSession VagrantSessionRecord::vagrantSession() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void VagrantSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSessionRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(VagrantSessionRecord::ColumnsType::vagrantSessionRecordType,m_vagrantSessionRecordType.value());
    // Template for required data.
    // query.bindValue(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void VagrantSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSessionRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(VagrantSessionRecord::ColumnsType::vagrantSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastVagrantSessionRecordType = VagrantSessionRecordType(value.toInt());

    // Template for required data.
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool VagrantSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSessionRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(VagrantSessionRecord::ColumnsType::vagrantSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_vagrantSessionRecordType == VagrantSessionRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(VagrantSessionRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void VagrantSessionRecord_Impl::saveLastValues() {
    CloudSessionRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastVagrantSessionRecordType = m_vagrantSessionRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void VagrantSessionRecord_Impl::revertToLastValues() {
    CloudSessionRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_vagrantSessionRecordType = m_lastVagrantSessionRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

VagrantSessionRecord::VagrantSessionRecord(const NAMESPACE::VagrantSession& vagrantSession, ProjectDatabase& database)
  : CloudSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl>(
        new detail::VagrantSessionRecord_Impl(vagrantSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
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

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int vagrantSessionRecordType = query.value(VagrantSessionRecordColumns::vagrantSessionRecordType).toInt();

  // switch (vagrantSessionRecordType) {
  //   case VagrantSessionRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<VagrantSessionRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown VagrantSessionRecordType " << vagrantSessionRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = VagrantSessionRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct VagrantSessionRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

VagrantSessionRecord VagrantSessionRecord::factoryFromVagrantSession(const NAMESPACE::VagrantSession& vagrantSession, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (vagrantSession.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(vagrantSession.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return VagrantSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl>());
}

std::vector<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecords(ProjectDatabase& database) {
  std::vector<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalVagrantSessionRecord vagrantSessionRecord = VagrantSessionRecord::factoryFromQuery(query, database);
    // if (vagrantSessionRecord) {
    //   result.push_back(*vagrantSessionRecord);
    // }

    // result.push_back(VagrantSessionRecord(query, database));
  }

  return result;
}

boost::optional<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType AND id=:id"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = VagrantSessionRecord::factoryFromQuery(query, database);

    // result = VagrantSessionRecord(query, database);
  }

  return result;
}

NAMESPACE::VagrantSession VagrantSessionRecord::vagrantSession() const {
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
  OS_ASSERTgetImpl<detail::VagrantSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

