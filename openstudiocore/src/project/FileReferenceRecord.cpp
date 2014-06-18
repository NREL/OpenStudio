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

#include "FileReferenceRecord.hpp"
#include "FileReferenceRecord_Impl.hpp"

#include "ProjectDatabase.hpp"
#include "JoinRecord.hpp"
#include "AttributeRecord.hpp"
#include "TagRecord.hpp"
#include "AnalysisRecord.hpp"
#include "DataPointRecord.hpp"
#include "MeasureRecord.hpp"
#include "AlgorithmRecord.hpp"

#include "../utilities/data/Attribute.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace project {

namespace detail {   

  FileReferenceRecord_Impl::FileReferenceRecord_Impl(const FileReference& fileReference, 
                                                     ProjectDatabase& database)
    : ObjectRecord_Impl(database,
                        fileReference.uuid(),
                        fileReference.name(),
                        fileReference.displayName(),
                        fileReference.description(),
                        fileReference.versionUUID()),
      m_fileReferenceType(fileReference.fileType()),
      m_path(completeAndNormalize(fileReference.path())),
      m_checksumCreate(fileReference.checksumCreate()),
      m_checksumLast(fileReference.checksumLast())
  {}

  FileReferenceRecord_Impl::FileReferenceRecord_Impl(const FileReference& fileReference, 
                                                     ObjectRecord& parentRecord)
    : ObjectRecord_Impl(parentRecord.projectDatabase(),
                        fileReference.uuid(),
                        fileReference.name(),
                        fileReference.displayName(),
                        fileReference.description(),
                        fileReference.versionUUID()),
      m_parentDatabaseTableName(parentRecord.databaseTableName()),
      m_parentRecordId(parentRecord.id()),
      m_fileReferenceType(fileReference.fileType()),
      m_path(completeAndNormalize(fileReference.path())),
      m_checksumCreate(fileReference.checksumCreate()),
      m_checksumLast(fileReference.checksumLast())
  {}

  FileReferenceRecord_Impl::FileReferenceRecord_Impl(const QSqlQuery& query, 
                                                     ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(FileReferenceRecordColumns::parentDatabaseTableName);
    if (value.isValid() && !value.isNull()) {
      m_parentDatabaseTableName = value.toString().toStdString();
    }

    value = query.value(FileReferenceRecordColumns::parentRecordId);
    if (value.isValid() && !value.isNull()) {
      m_parentRecordId = value.toInt();
    }

    value = query.value(FileReferenceRecordColumns::fileReferenceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_fileReferenceType = FileReferenceType(value.toInt());

    value = query.value(FileReferenceRecordColumns::path);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_path = toPath(value.toString());

    value = query.value(FileReferenceRecordColumns::checksumCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_checksumCreate = value.toString().toStdString();

    value = query.value(FileReferenceRecordColumns::checksumLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_checksumLast = value.toString().toStdString();
  }

  std::string FileReferenceRecord_Impl::databaseTableName() const {
    return FileReferenceRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> FileReferenceRecord_Impl::parent() const {
    OptionalObjectRecord result;
    ProjectDatabase database = projectDatabase();
    if (m_parentDatabaseTableName) {
      std::string parentDatabaseTableName = *m_parentDatabaseTableName;
      if (parentDatabaseTableName == DataPointRecord::databaseTableName()) {
        result = DataPointRecord::getDataPointRecord(*m_parentRecordId,database);
      }
      else if (parentDatabaseTableName == AnalysisRecord::databaseTableName()) {
        result = AnalysisRecord::getAnalysisRecord(*m_parentRecordId,database);
      }
      else if (parentDatabaseTableName == MeasureRecord::databaseTableName()) {
        result = MeasureRecord::getMeasureRecord(*m_parentRecordId,database);
      }
      else if (parentDatabaseTableName == AlgorithmRecord::databaseTableName()) {
        result = AlgorithmRecord::getAlgorithmRecord(*m_parentRecordId,database);
      }
      else {
        OS_ASSERT(false);
      }
    }
    return result;
  }

  std::vector<ObjectRecord> FileReferenceRecord_Impl::children() const {
    ObjectRecordVector result;
    TagRecordVector trs = tagRecords();
    result.insert(result.end(),trs.begin(),trs.end());
    AttributeRecordVector ars = attributeRecords();
    result.insert(result.end(),ars.begin(),ars.end());
    return result;
  }

  std::vector<ObjectRecord> FileReferenceRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> FileReferenceRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void FileReferenceRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database) 
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<FileReferenceRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void FileReferenceRecord_Impl::updatePathData(const openstudio::path& originalBase,
                                                const openstudio::path& newBase)
  {
    openstudio::path temp = relocatePath(path(),originalBase,newBase);
    if (!temp.empty()) {
      setPath(temp);
    }
  }

  FileReference FileReferenceRecord_Impl::fileReference() const {
    // TODO: These timestamps are not consistent. FileReference and Record are setting their
    // own timestamps completely independent of each other, and completely independent of the 
    // filesystem.
    FileReference result(handle(),
                         uuidLast(),
                         name(),
                         displayName(),
                         description(),
                         path(),
                         fileType(),
                         timestampCreate(),
                         timestampLast(),
                         checksumCreate(),
                         checksumLast());
    return result;
  }

  FileReferenceType FileReferenceRecord_Impl::fileType() const {
    return m_fileReferenceType;
  }
  
  openstudio::path FileReferenceRecord_Impl::path() const {
    return m_path;
  }

  void FileReferenceRecord_Impl::setPath(const openstudio::path& newPath) {
    m_path = completeAndNormalize(newPath);
    onChange();
  }

  std::string FileReferenceRecord_Impl::checksumCreate() const {
    return m_checksumCreate;
  }

  std::string FileReferenceRecord_Impl::checksumLast() const {
    return m_checksumLast;
  }

  std::vector<AttributeRecord> FileReferenceRecord_Impl::attributeRecords() const {
    AttributeRecordVector result;

    ProjectDatabase database = projectDatabase(); 
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + 
        " WHERE fileReferenceRecordId=:fileReferenceRecordId"));
    query.bindValue(":fileReferenceRecordId", id());
    assertExec(query);
    while (query.next()) {
      result.push_back(AttributeRecord(query, database));
    }

    return result;
  }

  boost::optional<AttributeRecord> FileReferenceRecord_Impl::getAttributeRecord(
      const std::string& name) const
  {
    OptionalAttributeRecord result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + 
        " WHERE fileReferenceRecordId=:fileReferenceRecordId AND name=:name"));
    query.bindValue(":fileReferenceRecordId", id());
    query.bindValue(":name",toQString(name));
    assertExec(query);
    if (query.first()) {
      result = AttributeRecord(query, database);
    }

    return result;
  }

  std::vector<TagRecord> FileReferenceRecord_Impl::tagRecords() const {
    TagRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + TagRecord::databaseTableName() + 
        " WHERE fileReferenceRecordId=:fileReferenceRecordId"));
    query.bindValue(":fileReferenceRecordId", id());
    assertExec(query);
    while (query.next()) {
      result.push_back(TagRecord(query, database));
    }

    return result;
  }

  boost::optional<TagRecord> FileReferenceRecord_Impl::getTagRecord(const std::string& name) const
  {
    OptionalTagRecord result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + TagRecord::databaseTableName() + 
        " WHERE fileReferenceRecordId=:fileReferenceRecordId AND name=:name"));
    query.bindValue(":fileReferenceRecordId", id());
    query.bindValue(":name",toQString(name));
    assertExec(query);
    if (query.first()) {
      result = TagRecord(query, database);
    }

    return result;
  }

  void FileReferenceRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    if (m_parentDatabaseTableName) {
      query.bindValue(FileReferenceRecordColumns::parentDatabaseTableName, 
                      toQString(*m_parentDatabaseTableName));
    }
    else {
      query.bindValue(FileReferenceRecordColumns::parentDatabaseTableName,
                      QVariant(QVariant::String));
    }
    if (m_parentRecordId) {
      query.bindValue(FileReferenceRecordColumns::parentRecordId, *m_parentRecordId);
    }
    else {
      query.bindValue(FileReferenceRecordColumns::parentRecordId, QVariant(QVariant::String));
    }
    query.bindValue(FileReferenceRecordColumns::fileReferenceType, m_fileReferenceType.value());
    query.bindValue(FileReferenceRecordColumns::path, toQString(m_path));
    query.bindValue(FileReferenceRecordColumns::checksumCreate, toQString(m_checksumCreate));
    query.bindValue(FileReferenceRecordColumns::checksumLast, toQString(m_checksumLast));
  }

  void FileReferenceRecord_Impl::setLastValues(const QSqlQuery& query, 
                                               ProjectDatabase& projectDatabase) 
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(FileReferenceRecordColumns::parentDatabaseTableName);
    if (value.isValid() && !value.isNull()) {
      m_lastParentDatabaseTableName = value.toString().toStdString();
    }
    else {
      m_lastParentDatabaseTableName.reset();
    }

    value = query.value(FileReferenceRecordColumns::parentRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastParentRecordId = value.toInt();
    }
    else {
      m_lastParentRecordId.reset();
    }

    value = query.value(FileReferenceRecordColumns::fileReferenceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFileReferenceType = FileReferenceType(value.toInt());

    value = query.value(FileReferenceRecordColumns::path);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastPath = toPath(value.toString());

    value = query.value(FileReferenceRecordColumns::checksumCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastChecksumCreate = value.toString().toStdString();

    value = query.value(FileReferenceRecordColumns::checksumLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastChecksumLast = value.toString().toStdString();
  }

  bool FileReferenceRecord_Impl::compareValues(const QSqlQuery& query) const {
    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(FileReferenceRecordColumns::parentDatabaseTableName);
    if (value.isValid() && !value.isNull()) {
      result = result && m_parentDatabaseTableName && (*m_parentDatabaseTableName == value.toString().toStdString());
    }
    else {
      result = result && !m_parentDatabaseTableName;
    }

    value = query.value(FileReferenceRecordColumns::parentRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_parentRecordId && (*m_parentRecordId == value.toInt());
    }else{
      result = result && !m_parentRecordId;
    }

    value = query.value(FileReferenceRecordColumns::fileReferenceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_fileReferenceType == FileReferenceType(value.toInt()));

    value = query.value(FileReferenceRecordColumns::path);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_path == toPath(value.toString()));
 
    value = query.value(FileReferenceRecordColumns::checksumCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_checksumCreate == value.toString().toStdString());

    value = query.value(FileReferenceRecordColumns::checksumLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_checksumLast == value.toString().toStdString());

    return result;
  }

  void FileReferenceRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastParentDatabaseTableName = m_parentDatabaseTableName;
    m_lastParentRecordId = m_parentRecordId;
    m_lastFileReferenceType = m_fileReferenceType;
    m_lastPath = m_path;
    m_lastChecksumCreate = m_checksumCreate;        
    m_lastChecksumLast = m_checksumLast;
  }

  void FileReferenceRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_parentDatabaseTableName = m_lastParentDatabaseTableName;
    m_parentRecordId = m_lastParentRecordId;
    m_fileReferenceType = m_lastFileReferenceType;
    m_path = m_lastPath;             
    m_checksumCreate = m_lastChecksumCreate;        
    m_checksumLast = m_lastChecksumLast;  
  }

} // detail

FileReferenceRecord::FileReferenceRecord(const FileReference& fileReference, 
                                         ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::FileReferenceRecord_Impl>(
        new detail::FileReferenceRecord_Impl(fileReference, database)),
        database)
{
  OS_ASSERT(getImpl<detail::FileReferenceRecord_Impl>());
}

FileReferenceRecord::FileReferenceRecord(const FileReference& fileReference, 
                                         ObjectRecord& parentRecord)
  : ObjectRecord(std::shared_ptr<detail::FileReferenceRecord_Impl>(
        new detail::FileReferenceRecord_Impl(fileReference, parentRecord)),
        parentRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::FileReferenceRecord_Impl>());
}

FileReferenceRecord::FileReferenceRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::FileReferenceRecord_Impl>(
        new detail::FileReferenceRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::FileReferenceRecord_Impl>());
}

FileReferenceRecord::FileReferenceRecord(std::shared_ptr<detail::FileReferenceRecord_Impl> impl,
                                         ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::FileReferenceRecord_Impl>());
}

std::string FileReferenceRecord::databaseTableName() {
  return "FileReferenceRecords";
}

UpdateByIdQueryData FileReferenceRecord::updateByIdQueryData() {
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

void FileReferenceRecord::updatePathData(ProjectDatabase database,
                                         const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
{
  FileReferenceRecordVector records = getFileReferenceRecords(database);
  for (FileReferenceRecord& record : records) {
    record.getImpl<detail::FileReferenceRecord_Impl>()->updatePathData(originalBase,newBase);
  }
}

boost::optional<FileReferenceRecord> FileReferenceRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalFileReferenceRecord result;
  try {
    result = FileReferenceRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct FileReferenceRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<FileReferenceRecord> FileReferenceRecord::getFileReferenceRecords(ProjectDatabase& database) {
  std::vector<FileReferenceRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FileReferenceRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(FileReferenceRecord(query, database));
  }

  return result;
}

boost::optional<FileReferenceRecord> FileReferenceRecord::getFileReferenceRecord(
    int id, ProjectDatabase& database) 
{
  boost::optional<FileReferenceRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FileReferenceRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = FileReferenceRecord(query, database);
  }

  return result;
}

boost::optional<FileReferenceRecord> FileReferenceRecord::getFileReferenceRecord(
    const UUID& handle,ProjectDatabase& database) 
{
  boost::optional<FileReferenceRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FileReferenceRecord::databaseTableName() + 
      " WHERE handle=:handle"));
  query.bindValue(":handle",handle.toString());
  assertExec(query);
  if (query.first()) {
    result = FileReferenceRecord(query, database);
  }

  return result;
}

boost::optional<FileReferenceRecord> FileReferenceRecord::getFileReferenceRecord(
      const openstudio::path& path, ProjectDatabase& database)
{
  OptionalFileReferenceRecord result;

  // first try to look for the path exactly
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FileReferenceRecord::databaseTableName() + 
      " WHERE path=:path"));
  query.bindValue(":path",toQString(path));
  assertExec(query);
  if (query.first()) {
    result = FileReferenceRecord(query, database);
    return result;
  }

  // if that fails, get all and check for equivalents
  FileReferenceRecordVector allFileReferences = getFileReferenceRecords(database);
  for (const FileReferenceRecord& record : allFileReferences) {
    if (boost::filesystem::equivalent(record.path(),path)) {
      result = record;
      break;
    }
  }

  return result;
}

FileReference FileReferenceRecord::fileReference() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->fileReference();
}

FileReferenceType FileReferenceRecord::fileType() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->fileType();
}

openstudio::path FileReferenceRecord::path() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->path();
}         

std::string FileReferenceRecord::checksumCreate() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->checksumCreate();
}     

std::string FileReferenceRecord::checksumLast() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->checksumLast();
}

std::vector<AttributeRecord> FileReferenceRecord::attributeRecords() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->attributeRecords();
}

boost::optional<AttributeRecord> FileReferenceRecord::getAttributeRecord(
    const std::string& name) const 
{
  return getImpl<detail::FileReferenceRecord_Impl>()->getAttributeRecord(name);
}

std::vector<TagRecord> FileReferenceRecord::tagRecords() const {
  return getImpl<detail::FileReferenceRecord_Impl>()->tagRecords();
}

boost::optional<TagRecord> FileReferenceRecord::getTagRecord(const std::string& name) const {
  return getImpl<detail::FileReferenceRecord_Impl>()->getTagRecord(name);
}

/// @cond
FileReferenceRecord::FileReferenceRecord(std::shared_ptr<detail::FileReferenceRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

} // project
} // openstudio

