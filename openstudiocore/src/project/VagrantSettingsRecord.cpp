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

#include <project/VagrantSettingsRecord.hpp>
#include <project/VagrantSettingsRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/VagrantSettings.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  VagrantSettingsRecord_Impl::VagrantSettingsRecord_Impl(const NAMESPACE::VagrantSettings& vagrantSettings,
                                                         const VagrantSettingsRecordType& vagrantSettingsRecordType,
                                                         ProjectDatabase& database)
    : CloudSettingsRecord_Impl(vagrantSettings, database),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_vagrantSettingsRecordType(vagrantSettingsRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  VagrantSettingsRecord_Impl::VagrantSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSettingsRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(VagrantSettingsRecord::ColumnsType::vagrantSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_vagrantSettingsRecordType = VagrantSettingsRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  boost::optional<ObjectRecord> VagrantSettingsRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> VagrantSettingsRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> VagrantSettingsRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> VagrantSettingsRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void VagrantSettingsRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<VagrantSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::VagrantSettings VagrantSettingsRecord::vagrantSettings() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void VagrantSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSettingsRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(VagrantSettingsRecord::ColumnsType::vagrantSettingsRecordType,m_vagrantSettingsRecordType.value());
    // Template for required data.
    // query.bindValue(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void VagrantSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSettingsRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(VagrantSettingsRecord::ColumnsType::vagrantSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastVagrantSettingsRecordType = VagrantSettingsRecordType(value.toInt());

    // Template for required data.
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool VagrantSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSettingsRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(VagrantSettingsRecord::ColumnsType::vagrantSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_vagrantSettingsRecordType == VagrantSettingsRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(VagrantSettingsRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void VagrantSettingsRecord_Impl::saveLastValues() {
    CloudSettingsRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastVagrantSettingsRecordType = m_vagrantSettingsRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void VagrantSettingsRecord_Impl::revertToLastValues() {
    CloudSettingsRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_vagrantSettingsRecordType = m_lastVagrantSettingsRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

VagrantSettingsRecord::VagrantSettingsRecord(const NAMESPACE::VagrantSettings& vagrantSettings, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl>(
        new detail::VagrantSettingsRecord_Impl(vagrantSettings, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSettingsRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

VagrantSettingsRecord::VagrantSettingsRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl>(
        new detail::VagrantSettingsRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSettingsRecord_Impl>());
}

boost::optional<VagrantSettingsRecord> VagrantSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalVagrantSettingsRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int vagrantSettingsRecordType = query.value(VagrantSettingsRecordColumns::vagrantSettingsRecordType).toInt();

  // switch (vagrantSettingsRecordType) {
  //   case VagrantSettingsRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<VagrantSettingsRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown VagrantSettingsRecordType " << vagrantSettingsRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = VagrantSettingsRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct VagrantSettingsRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

VagrantSettingsRecord VagrantSettingsRecord::factoryFromVagrantSettings(const NAMESPACE::VagrantSettings& vagrantSettings, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (vagrantSettings.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(vagrantSettings.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl>());
}

std::vector<VagrantSettingsRecord> VagrantSettingsRecord::getVagrantSettingsRecords(ProjectDatabase& database) {
  std::vector<VagrantSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::VagrantSettingsRecord);
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalVagrantSettingsRecord vagrantSettingsRecord = VagrantSettingsRecord::factoryFromQuery(query, database);
    // if (vagrantSettingsRecord) {
    //   result.push_back(*vagrantSettingsRecord);
    // }

    // result.push_back(VagrantSettingsRecord(query, database));
  }

  return result;
}

boost::optional<VagrantSettingsRecord> VagrantSettingsRecord::getVagrantSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<VagrantSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType AND id=:id"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::VagrantSettingsRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = VagrantSettingsRecord::factoryFromQuery(query, database);

    // result = VagrantSettingsRecord(query, database);
  }

  return result;
}

NAMESPACE::VagrantSettings VagrantSettingsRecord::vagrantSettings() const {
  return getImpl<detail::VagrantSettingsRecord_Impl>()->vagrantSettings();
}

/// @cond
VagrantSettingsRecord::VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl)
  : CloudSettingsRecord(impl)
{}

VagrantSettingsRecord::VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl,
                                             ProjectDatabase database)
  : CloudSettingsRecord(impl, database)
{
  OS_ASSERTgetImpl<detail::VagrantSettingsRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

