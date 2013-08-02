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

#include <project/RubyMeasureRecord.hpp>
#include <project/RubyMeasureRecord_Impl.hpp>
#include <project/MeasureGroupRecord.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/OSArgumentRecord.hpp>

#include <analysis/Measure.hpp>
#include <analysis/Measure_Impl.hpp>
#include <analysis/RubyMeasure.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/core/Compare.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

using namespace openstudio::ruleset;

namespace openstudio {
namespace project {

namespace detail {

  RubyMeasureRecord_Impl::RubyMeasureRecord_Impl(const analysis::RubyMeasure& rubyMeasure,
                                                 MeasureGroupRecord& measureGroupRecord,
                                                 int measureVectorIndex)
    : MeasureRecord_Impl(rubyMeasure,
                         MeasureRecordType::RubyMeasureRecord,
                         measureGroupRecord,
                         measureVectorIndex),
      m_isUserScript(rubyMeasure.isUserScript()),
      m_usesBCLMeasure(rubyMeasure.usesBCLMeasure())
  {
    boost::optional<FileReferenceType> inputFileType = rubyMeasure.inputFileType();
    BOOST_ASSERT(inputFileType);
    m_inputFileType = *inputFileType;

    boost::optional<FileReferenceType> outputFileType = rubyMeasure.outputFileType();
    BOOST_ASSERT(outputFileType);
    m_outputFileType = *outputFileType;
  }

  RubyMeasureRecord_Impl::RubyMeasureRecord_Impl(
      const analysis::RubyMeasure& rubyMeasure,
      ProjectDatabase& database)
    : MeasureRecord_Impl(rubyMeasure,
                         MeasureRecordType::RubyMeasureRecord,
                         database),
      m_isUserScript(rubyMeasure.isUserScript()),
      m_usesBCLMeasure(rubyMeasure.usesBCLMeasure())
  {
    boost::optional<FileReferenceType> inputFileType = rubyMeasure.inputFileType();
    BOOST_ASSERT(inputFileType);
    m_inputFileType = *inputFileType;

    boost::optional<FileReferenceType> outputFileType = rubyMeasure.outputFileType();
    BOOST_ASSERT(outputFileType);
    m_outputFileType = *outputFileType;
  }

  RubyMeasureRecord_Impl::RubyMeasureRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : MeasureRecord_Impl(query, database)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(MeasureRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_scriptOrBCLMeasureRecordId = value.toInt();

    value = query.value(MeasureRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_inputFileType = FileReferenceType(value.toInt());

    value = query.value(MeasureRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_outputFileType = FileReferenceType(value.toInt());

    value = query.value(MeasureRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_isUserScript = value.toBool();

    value = query.value(MeasureRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_usesBCLMeasure = value.toBool();
  }

  std::vector<ObjectRecord> RubyMeasureRecord_Impl::children() const
  {
    std::vector<ObjectRecord> result;
    OSArgumentRecordVector arguments = osArgumentRecords();
    result.insert(result.end(),arguments.begin(),arguments.end());
    return result;
  }

  std::vector<ObjectRecord> RubyMeasureRecord_Impl::resources() const {
    std::vector<ObjectRecord> result;
    result.push_back(this->scriptOrBCLMeasureRecord());
    return result;
  }

  void RubyMeasureRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<RubyMeasureRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  bool RubyMeasureRecord_Impl::usesBCLMeasure() const {
    return m_usesBCLMeasure;
  }

  FileReferenceRecord RubyMeasureRecord_Impl::scriptOrBCLMeasureRecord() const {
    ProjectDatabase database = projectDatabase();
    return FileReferenceRecord::getFileReferenceRecord(m_scriptOrBCLMeasureRecordId,database).get();
  }

  FileReferenceType RubyMeasureRecord_Impl::inputFileType() const {
    return m_inputFileType;
  }

  FileReferenceType RubyMeasureRecord_Impl::outputFileType() const {
    return m_outputFileType;
  }

  std::vector<OSArgumentRecord> RubyMeasureRecord_Impl::osArgumentRecords() const
  {
    OSArgumentRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + OSArgumentRecord::databaseTableName() +
                            " WHERE rubyMeasureRecordId=:rubyMeasureId"));
    query.bindValue(":rubyMeasureRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(OSArgumentRecord(query,database));
    }

    return result;
  }

  analysis::Measure RubyMeasureRecord_Impl::measure() const {
    return rubyMeasure().cast<analysis::Measure>();
  }

  analysis::RubyMeasure RubyMeasureRecord_Impl::rubyMeasure() const {
    FileReferenceRecord scriptOrBCLMeasureRecord = this->scriptOrBCLMeasureRecord();
    OSArgumentRecordVector argumentRecords = this->osArgumentRecords();
    ruleset::OSArgumentVector arguments;
    BOOST_FOREACH(const OSArgumentRecord& argumentRecord,argumentRecords) {
      arguments.push_back(argumentRecord.osArgument());
    }

    return analysis::RubyMeasure(handle(),
                                 uuidLast(),
                                 name(),
                                 displayName(),
                                 description(),
                                 isSelected(),
                                 scriptOrBCLMeasureRecord.fileReference(),
                                 inputFileType(),
                                 outputFileType(),
                                 m_isUserScript,
                                 arguments,
                                 m_usesBCLMeasure);
  }

  void RubyMeasureRecord_Impl::revertToLastRecordIds() {
    m_scriptOrBCLMeasureRecordId = m_lastScriptOrBCLMeasureRecordId;
  }

  void RubyMeasureRecord_Impl::setScriptOrBCLMeasureRecordId(int id) {
    m_scriptOrBCLMeasureRecordId = id;
    this->onChange(false);
  }

  void RubyMeasureRecord_Impl::bindValues(QSqlQuery& query) const
  {
    MeasureRecord_Impl::bindValues(query);

    query.bindValue(MeasureRecordColumns::rubyScriptRecordId, m_scriptOrBCLMeasureRecordId);
    query.bindValue(MeasureRecordColumns::inputFileType, m_inputFileType.value());
    query.bindValue(MeasureRecordColumns::outputFileType, m_outputFileType.value());
    query.bindValue(MeasureRecordColumns::isUserScript, m_isUserScript);
    query.bindValue(MeasureRecordColumns::usesBCLMeasure, m_usesBCLMeasure);
  }

  void RubyMeasureRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    MeasureRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(MeasureRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastScriptOrBCLMeasureRecordId = value.toInt();

    value = query.value(MeasureRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastInputFileType = FileReferenceType(value.toInt());

    value = query.value(MeasureRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastOutputFileType = FileReferenceType(value.toInt());

    value = query.value(MeasureRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastIsUserScript = value.toBool();

    value = query.value(MeasureRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastUsesBCLMeasure = value.toBool();
  }

  bool RubyMeasureRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && MeasureRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(MeasureRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_scriptOrBCLMeasureRecordId == value.toInt());

    value = query.value(MeasureRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_inputFileType == FileReferenceType(value.toInt()));

    value = query.value(MeasureRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_outputFileType == FileReferenceType(value.toInt()));

    value = query.value(MeasureRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_isUserScript == value.toBool());

    value = query.value(MeasureRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_usesBCLMeasure == value.toBool());

    return result;
  }

  void RubyMeasureRecord_Impl::saveLastValues()
  {
    MeasureRecord_Impl::saveLastValues();

    m_lastScriptOrBCLMeasureRecordId = m_scriptOrBCLMeasureRecordId;
    m_lastInputFileType = m_inputFileType;
    m_lastOutputFileType = m_outputFileType;
    m_lastIsUserScript = m_isUserScript;
    m_lastUsesBCLMeasure = m_usesBCLMeasure;
  }

  void RubyMeasureRecord_Impl::revertToLastValues()
  {
    MeasureRecord_Impl::revertToLastValues();

    m_scriptOrBCLMeasureRecordId = m_lastScriptOrBCLMeasureRecordId;
    m_inputFileType = m_lastInputFileType;
    m_outputFileType = m_lastOutputFileType;
    m_isUserScript = m_lastIsUserScript;
    m_usesBCLMeasure = m_lastUsesBCLMeasure;
  }

} // detail

RubyMeasureRecord::RubyMeasureRecord(const analysis::RubyMeasure& rubyMeasure,
                                               MeasureGroupRecord& measureGroupRecord,
                                               int measureVectorIndex)
  : MeasureRecord(boost::shared_ptr<detail::RubyMeasureRecord_Impl>(
        new detail::RubyMeasureRecord_Impl(rubyMeasure,
                                                measureGroupRecord,
                                                measureVectorIndex)),
        measureGroupRecord.projectDatabase())
{
  BOOST_ASSERT(getImpl<detail::RubyMeasureRecord_Impl>());

  constructRelatedRecords(rubyMeasure);
}

RubyMeasureRecord::RubyMeasureRecord(const analysis::RubyMeasure& rubyMeasure,
                                               ProjectDatabase& database)
  : MeasureRecord(boost::shared_ptr<detail::RubyMeasureRecord_Impl>(
        new detail::RubyMeasureRecord_Impl(rubyMeasure,
                                                database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::RubyMeasureRecord_Impl>());

  constructRelatedRecords(rubyMeasure);
}

RubyMeasureRecord::RubyMeasureRecord(const QSqlQuery& query, ProjectDatabase& database)
  : MeasureRecord(boost::shared_ptr<detail::RubyMeasureRecord_Impl>(
        new detail::RubyMeasureRecord_Impl(query, database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::RubyMeasureRecord_Impl>());
}

RubyMeasureRecord::RubyMeasureRecord(boost::shared_ptr<detail::RubyMeasureRecord_Impl> impl, ProjectDatabase database)
  : MeasureRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::RubyMeasureRecord_Impl>());
}

RubyMeasureRecord::RubyMeasureRecord(boost::shared_ptr<detail::RubyMeasureRecord_Impl> impl)
  : MeasureRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::RubyMeasureRecord_Impl>());
}

boost::optional<RubyMeasureRecord> RubyMeasureRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalRubyMeasureRecord result;
  try {
    result = RubyMeasureRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct RubyMeasureRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<RubyMeasureRecord> RubyMeasureRecord::getRubyMeasureRecords(ProjectDatabase& database)
{
  std::vector<RubyMeasureRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() + " WHERE measureRecordType=:measureRecordType"));
  query.bindValue(":measureRecordType", MeasureRecordType::RubyMeasureRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(RubyMeasureRecord(query,database));
  }

  return result;
}

boost::optional<RubyMeasureRecord> RubyMeasureRecord::getRubyMeasureRecord(int id, ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RubyMeasureRecord::databaseTableName() + " WHERE measureRecordType=:measureRecordType AND id=:id"));
  query.bindValue(":measureRecordType", MeasureRecordType::RubyMeasureRecord);
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    return RubyMeasureRecord(query,database);
  }

  return boost::none;
}

bool RubyMeasureRecord::usesBCLMeasure() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->usesBCLMeasure();
}

FileReferenceRecord RubyMeasureRecord::scriptOrBCLMeasureRecord() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->scriptOrBCLMeasureRecord();
}

FileReferenceType RubyMeasureRecord::inputFileType() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->inputFileType();
}

FileReferenceType RubyMeasureRecord::outputFileType() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->outputFileType();
}

std::vector<OSArgumentRecord> RubyMeasureRecord::osArgumentRecords() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->osArgumentRecords();
}

analysis::RubyMeasure RubyMeasureRecord::rubyMeasure() const {
  return getImpl<detail::RubyMeasureRecord_Impl>()->rubyMeasure();
}

void RubyMeasureRecord::constructRelatedRecords(const analysis::RubyMeasure& rubyMeasure) {
  RubyMeasureRecord copyOfThis(getImpl<detail::RubyMeasureRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::RubyMeasureRecord_Impl>()->revertToLastRecordIds();
  }

  // Save child FileReference
  OptionalFileReference oChildFileReference;
  if (rubyMeasure.usesBCLMeasure()) {
    // co-opt FileReference to store necessary data
    oChildFileReference = FileReference(rubyMeasure.measureUUID(),
                                        rubyMeasure.measureVersionUUID(),
                                        "",
                                        "",
                                        "",
                                        rubyMeasure.measureDirectory(),
                                        FileReferenceType::Unknown,
                                        DateTime::now(),
                                        DateTime::now(),
                                        "",
                                        "");
  }
  else {
    oChildFileReference = rubyMeasure.measureScript();
  }
  FileReference scriptOrBCLMeasureReference = *oChildFileReference;
  bool saveReference = false;
  if (!isNew) {
    {
      // see if old script reference record should be removed
      FileReferenceRecord oldReferenceRecord = scriptOrBCLMeasureRecord();
      if (oldReferenceRecord.handle() != scriptOrBCLMeasureReference.uuid()) {
        database.removeRecord(oldReferenceRecord);
        saveReference = true;
      }
      else if (oldReferenceRecord.uuidLast() != scriptOrBCLMeasureReference.versionUUID()) {
        saveReference = true;
      }
    }
    database.unloadUnusedCleanRecords();
  }
  if (saveReference || isNew) {
    FileReferenceRecord scriptOrBCLMeasureRecord(scriptOrBCLMeasureReference, copyOfThis);
    getImpl<detail::RubyMeasureRecord_Impl>()->setScriptOrBCLMeasureRecordId(scriptOrBCLMeasureRecord.id());
  }

  // Save child OSArguments
  OSArgumentVector arguments = rubyMeasure.arguments();
  std::vector<UUID> argumentUUIDs;
  BOOST_FOREACH(const OSArgument& argument,arguments) {
    // no dirty flag, so construct them all
    argumentUUIDs.push_back(argument.uuid());
    OSArgumentRecord newArgumentRecord = OSArgumentRecord(argument, copyOfThis);
  }

  // Remove orphaned OSArguments
  if (!isNew) {
    removeOSArgumentRecords(argumentUUIDs,database);
  }
}

void RubyMeasureRecord::removeOSArgumentRecords(const std::vector<UUID>& uuidsToKeep,
                                                     ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + OSArgumentRecord::databaseTableName() +
        " WHERE (rubyMeasureRecordId=:rubyMeasureRecordId) AND (handle NOT IN (";
  std::string sep("");
  BOOST_FOREACH(const UUID& handle,uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":rubyMeasureRecordId",id());
  assertExec(query);
  while (query.next()) {
    OptionalOSArgumentRecord argumentRecord = OSArgumentRecord::factoryFromQuery(query, database);
    if (argumentRecord) {
      database.removeRecord(*argumentRecord);
    }
  }
}

} // project
} // openstudio

