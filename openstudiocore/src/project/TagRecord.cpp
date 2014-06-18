/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "TagRecord.hpp"
#include "TagRecord_Impl.hpp"
#include "FileReferenceRecord.hpp"
#include "DataPointRecord.hpp"
#include "ProjectDatabase.hpp"
#include "JoinRecord.hpp"

#include "../utilities/data/Tag.hpp"
#include "../utilities/core/Assert.hpp"

#include <QSqlQuery>

namespace openstudio {
namespace project {

namespace detail {

  TagRecord_Impl::TagRecord_Impl(const std::string& tag, const FileReferenceRecord& fileReferenceRecord)
    : ObjectRecord_Impl(fileReferenceRecord.projectDatabase(), tag, tag, ""), 
      m_fileReferenceRecordId(fileReferenceRecord.id())
  {}

  TagRecord_Impl::TagRecord_Impl(const Tag& tag, const DataPointRecord& dataPointRecord)
    : ObjectRecord_Impl(dataPointRecord.projectDatabase(), 
                        tag.uuid(), 
                        tag.name(), 
                        tag.name(), 
                        "", 
                        createUUID()),
      m_dataPointRecordId(dataPointRecord.id())
  {}

  TagRecord_Impl::TagRecord_Impl(const QSqlQuery& query, ProjectDatabase& database) 
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
    
    QVariant value;

    value = query.value(TagRecordColumns::fileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      m_fileReferenceRecordId = value.toInt(); 
    }

    value = query.value(TagRecordColumns::dataPointRecordId);
    if (value.isValid() && !value.isNull()) {
      m_dataPointRecordId = value.toInt();
    }

  }

  std::string TagRecord_Impl::databaseTableName() const {
    return TagRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> TagRecord_Impl::parent() const {
    OptionalFileReferenceRecord fileReferenceRecord = this->fileReferenceRecord();
    if (fileReferenceRecord) { 
      return fileReferenceRecord->cast<ObjectRecord>();
    }
    OptionalDataPointRecord dataPointRecord = this->dataPointRecord();
    if (dataPointRecord) {
      return dataPointRecord->cast<ObjectRecord>();
    }
    OS_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> TagRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> TagRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> TagRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void TagRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<TagRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  boost::optional<FileReferenceRecord> TagRecord_Impl::fileReferenceRecord() const {
    OptionalFileReferenceRecord result;
    if (m_fileReferenceRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_fileReferenceRecordId,database);
    }
    return result;
  }

  boost::optional<DataPointRecord> TagRecord_Impl::dataPointRecord() const {
    OptionalDataPointRecord result;
    if (m_dataPointRecordId) {
      ProjectDatabase database = projectDatabase();
      result = DataPointRecord::getDataPointRecord(*m_dataPointRecordId,database);
    }
    return result;
  }

  Tag TagRecord_Impl::tag() const {
    return Tag(handle(),name());
  }

  void TagRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    if (m_fileReferenceRecordId){
      query.bindValue(TagRecordColumns::fileReferenceRecordId, *m_fileReferenceRecordId);
    }else{
      query.bindValue(TagRecordColumns::fileReferenceRecordId, QVariant(QVariant::Int));
    }

    if (m_dataPointRecordId){
      query.bindValue(TagRecordColumns::dataPointRecordId, *m_dataPointRecordId);
    }else{
      query.bindValue(TagRecordColumns::dataPointRecordId, QVariant(QVariant::Int));
    }

  }

  void TagRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(TagRecordColumns::fileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastFileReferenceRecordId = value.toInt(); 
    }

    value = query.value(TagRecordColumns::dataPointRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastDataPointRecordId = value.toInt(); 
    }
  }

  bool TagRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(TagRecordColumns::fileReferenceRecordId);
    if(value.isValid()){
      result = result && m_fileReferenceRecordId && (*m_fileReferenceRecordId == value.toInt());
    }else{
      result = result && !m_fileReferenceRecordId;
    }

    value = query.value(TagRecordColumns::dataPointRecordId);
    if(value.isValid()){
      result = result && m_dataPointRecordId && (*m_dataPointRecordId == value.toInt());
    }else{
      result = result && !m_dataPointRecordId;
    }

    return result;
  }

  void TagRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastFileReferenceRecordId = m_fileReferenceRecordId;
    m_lastDataPointRecordId = m_dataPointRecordId;
  }

  void TagRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_fileReferenceRecordId = m_lastFileReferenceRecordId;
    m_dataPointRecordId = m_lastDataPointRecordId;
  }

} // detail

TagRecord::TagRecord(const std::string& tag, const FileReferenceRecord& fileReferenceRecord)
  : ObjectRecord(std::shared_ptr<detail::TagRecord_Impl>(
                     new detail::TagRecord_Impl(tag, fileReferenceRecord)), 
                 fileReferenceRecord.projectDatabase())
{
  OS_ASSERT(this->getImpl<detail::TagRecord_Impl>());
}

TagRecord::TagRecord(const Tag& tag, const DataPointRecord& dataPointRecord)
  : ObjectRecord(std::shared_ptr<detail::TagRecord_Impl>(
                     new detail::TagRecord_Impl(tag, dataPointRecord)), 
                 dataPointRecord.projectDatabase())
{
  OS_ASSERT(this->getImpl<detail::TagRecord_Impl>());
}

TagRecord::TagRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::TagRecord_Impl>(
                     new detail::TagRecord_Impl(query,database)), 
                 database)
{
  OS_ASSERT(this->getImpl<detail::TagRecord_Impl>());
}

TagRecord::TagRecord(std::shared_ptr<detail::TagRecord_Impl> impl, 
                     ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  OS_ASSERT(this->getImpl<detail::TagRecord_Impl>());
}

std::string TagRecord::databaseTableName() {
  return "TagRecords";
}

UpdateByIdQueryData TagRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(), 
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      } 
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (const auto & columnValue : result.columnValues)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(columnValue), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void TagRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "fileReferenceRecordIdIndex ON " + databaseTableName + " (fileReferenceRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "dataPointRecordIdIndex ON " + databaseTableName + " (dataPointRecordId)"));
  assertExec(query);
}

void TagRecord::updatePathData(ProjectDatabase database,
                               const openstudio::path& originalBase,
                               const openstudio::path& newBase)
{}

boost::optional<TagRecord> TagRecord::factoryFromQuery(const QSqlQuery& query, 
                                                       ProjectDatabase& database)
{
  OptionalTagRecord result;
  try {
    result = TagRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct TagRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<TagRecord> TagRecord::getTagRecords(ProjectDatabase& database) {
  std::vector<TagRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + TagRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(TagRecord(query, database));
  }

  return result;
}

boost::optional<TagRecord> TagRecord::getTagRecord(int id, ProjectDatabase& database) {
  boost::optional<TagRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + TagRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = TagRecord(query, database);
  }

  return result;
}

boost::optional<FileReferenceRecord> TagRecord::fileReferenceRecord() const {
  return getImpl<detail::TagRecord_Impl>()->fileReferenceRecord();
}

boost::optional<DataPointRecord> TagRecord::dataPointRecord() const {
  return getImpl<detail::TagRecord_Impl>()->dataPointRecord();
}

Tag TagRecord::tag() const {
  return getImpl<detail::TagRecord_Impl>()->tag();
}

/// @cond
TagRecord::TagRecord(std::shared_ptr<detail::TagRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(this->getImpl<detail::TagRecord_Impl>());
}
/// @endcond
} // project
} // openstudio
