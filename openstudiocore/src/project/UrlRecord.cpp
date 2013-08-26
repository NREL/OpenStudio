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

#include <project/UrlRecord.hpp>
#include <project/UrlRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
// TODO: Add derived class includes for factory methods if this is a base class.

// TODO: Replace with derived class includes if this is a base class.
#include <NAMESPACE/Url.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  UrlRecord_Impl::UrlRecord_Impl(const NAMESPACE::Url& url,
                                 const UrlRecordType& urlRecordType,
                                 ProjectDatabase& database)
    : ObjectRecord_Impl(database,url.uuid(),url.name(),url.name(),"",url.versionUUID()),
  // TODO: Delete member enum initialization if deleted from _Impl.hpp
      m_urlRecordType(urlRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  UrlRecord_Impl::UrlRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Delete deserialization of enum if deleted from _Impl.hpp
    value = query.value(UrlRecord::ColumnsType::urlRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_urlRecordType = UrlRecordType(value.toInt());

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  std::string UrlRecord_Impl::databaseTableName() const {
    return UrlRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> UrlRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> UrlRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> UrlRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    OS_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> UrlRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    OS_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void UrlRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<UrlRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::Url UrlRecord::url() const {
    // TODO: De-serialize the object here.
    OS_ASSERT(false);
  }

  void UrlRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    // TODO: Delete bind for enum if no derived classes.
    query.bindValue(UrlRecord::ColumnsType::urlRecordType,m_urlRecordType.value());
    // Template for required data.
    // query.bindValue(UrlRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(UrlRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(UrlRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void UrlRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(UrlRecord::ColumnsType::urlRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUrlRecordType = UrlRecordType(value.toInt());

    // Template for required data.
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool UrlRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    // TODO: Delete if no derived classes.
    value = query.value(UrlRecord::ColumnsType::urlRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_urlRecordType == UrlRecordType(value.toInt()));

    // Template for required data.
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // OS_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(UrlRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void UrlRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    // TODO: Delete if no derived types.
    m_lastUrlRecordType = m_urlRecordType;
    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void UrlRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    // TODO: Delete if no derived types.
    m_urlRecordType = m_lastUrlRecordType;
    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

UrlRecord::UrlRecord(const NAMESPACE::Url& url, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::UrlRecord_Impl>(
        new detail::UrlRecord_Impl(url, database)),
        database)
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());

  OS_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

UrlRecord::UrlRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::UrlRecord_Impl>(
        new detail::UrlRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}

std::string UrlRecord::databaseTableName() {
  return "UrlRecords";
}

void UrlRecord::updatePathData(ProjectDatabase database,
                               const openstudio::path& originalBase,
                               const openstudio::path& newBase)
{
  // TODO: Delete implementation if no classes in this hierarchy contain path data.
  // TODO: Override virtual ObjectRecord_Impl method updatePathData for any classes in this
  // hierarchy that do contain path data. See ProjectDatabaseRecord_Impl for an example. 
  UrlRecordVector records = getUrlRecords(database);
  BOOST_FOREACH(UrlRecord& record,records) {
    record.getImpl<detail::UrlRecord_Impl>()->updatePathData(originalBase,newBase);
  }
}

boost::optional<UrlRecord> UrlRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalUrlRecord result;

  // Template for base classes. See, for instance, MeasureRecord::factoryFromQuery.
  // int urlRecordType = query.value(UrlRecordColumns::urlRecordType).toInt();

  // switch (urlRecordType) {
  //   case UrlRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<UrlRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown UrlRecordType " << urlRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = UrlRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct UrlRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

UrlRecord UrlRecord::factoryFromUrl(const NAMESPACE::Url& url, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  OS_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (url.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(url.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  OS_ASSERT(false);
  return UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl>());
}

std::vector<UrlRecord> UrlRecord::getUrlRecords(ProjectDatabase& database) {
  std::vector<UrlRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + UrlRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalUrlRecord urlRecord = UrlRecord::factoryFromQuery(query, database);
    // if (urlRecord) {
    //   result.push_back(*urlRecord);
    // }

    // result.push_back(UrlRecord(query, database));
  }

  return result;
}

boost::optional<UrlRecord> UrlRecord::getUrlRecord(int id, ProjectDatabase& database) {
  boost::optional<UrlRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + UrlRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = UrlRecord::factoryFromQuery(query, database);

    // result = UrlRecord(query, database);
  }

  return result;
}

NAMESPACE::Url UrlRecord::url() const {
  return getImpl<detail::UrlRecord_Impl>()->url();
}

/// @cond
UrlRecord::UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl)
  : ObjectRecord(impl)
{}

UrlRecord::UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl,
                     ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERTgetImpl<detail::UrlRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

