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

#include <project/RubyPerturbationRecord.hpp>
#include <project/RubyPerturbationRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/OSArgumentRecord.hpp>

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>
#include <analysis/RubyPerturbation.hpp>

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

  RubyPerturbationRecord_Impl::RubyPerturbationRecord_Impl(const analysis::RubyPerturbation& rubyPerturbation,
                                                           DiscreteVariableRecord& discreteVariableRecord,
                                                           int perturbationVectorIndex)
    : DiscretePerturbationRecord_Impl(rubyPerturbation,
                                      DiscretePerturbationRecordType::RubyPerturbationRecord,
                                      discreteVariableRecord,
                                      perturbationVectorIndex),
      m_isUserScript(rubyPerturbation.isUserScript()),
      m_usesBCLMeasure(rubyPerturbation.usesBCLMeasure())
  {
    boost::optional<FileReferenceType> inputFileType = rubyPerturbation.inputFileType();
    BOOST_ASSERT(inputFileType);
    m_inputFileType = *inputFileType;

    boost::optional<FileReferenceType> outputFileType = rubyPerturbation.outputFileType();
    BOOST_ASSERT(outputFileType);
    m_outputFileType = *outputFileType;
  }

  RubyPerturbationRecord_Impl::RubyPerturbationRecord_Impl(
      const analysis::RubyPerturbation& rubyPerturbation,
      ProjectDatabase& database)
    : DiscretePerturbationRecord_Impl(rubyPerturbation,
                                      DiscretePerturbationRecordType::RubyPerturbationRecord,
                                      database),
      m_isUserScript(rubyPerturbation.isUserScript()),
      m_usesBCLMeasure(rubyPerturbation.usesBCLMeasure())
  {
    boost::optional<FileReferenceType> inputFileType = rubyPerturbation.inputFileType();
    BOOST_ASSERT(inputFileType);
    m_inputFileType = *inputFileType;

    boost::optional<FileReferenceType> outputFileType = rubyPerturbation.outputFileType();
    BOOST_ASSERT(outputFileType);
    m_outputFileType = *outputFileType;
  }

  RubyPerturbationRecord_Impl::RubyPerturbationRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : DiscretePerturbationRecord_Impl(query, database)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_scriptOrBCLMeasureRecordId = value.toInt();

    value = query.value(DiscretePerturbationRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_inputFileType = FileReferenceType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_outputFileType = FileReferenceType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_isUserScript = value.toBool();

    value = query.value(DiscretePerturbationRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_usesBCLMeasure = value.toBool();
  }

  std::vector<ObjectRecord> RubyPerturbationRecord_Impl::children() const
  {
    std::vector<ObjectRecord> result;
    OSArgumentRecordVector arguments = osArgumentRecords();
    result.insert(result.end(),arguments.begin(),arguments.end());
    return result;
  }

  std::vector<ObjectRecord> RubyPerturbationRecord_Impl::resources() const {
    std::vector<ObjectRecord> result;
    result.push_back(this->scriptOrBCLMeasureRecord());
    return result;
  }

  void RubyPerturbationRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<RubyPerturbationRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  bool RubyPerturbationRecord_Impl::usesBCLMeasure() const {
    return m_usesBCLMeasure;
  }

  FileReferenceRecord RubyPerturbationRecord_Impl::scriptOrBCLMeasureRecord() const {
    ProjectDatabase database = projectDatabase();
    return FileReferenceRecord::getFileReferenceRecord(m_scriptOrBCLMeasureRecordId,database).get();
  }

  FileReferenceType RubyPerturbationRecord_Impl::inputFileType() const {
    return m_inputFileType;
  }

  FileReferenceType RubyPerturbationRecord_Impl::outputFileType() const {
    return m_outputFileType;
  }

  std::vector<OSArgumentRecord> RubyPerturbationRecord_Impl::osArgumentRecords() const
  {
    OSArgumentRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + OSArgumentRecord::databaseTableName() +
                            " WHERE rubyPerturbationRecordId=:rubyPerturbationId"));
    query.bindValue(":rubyPerturbationRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(OSArgumentRecord(query,database));
    }

    return result;
  }

  analysis::DiscretePerturbation RubyPerturbationRecord_Impl::discretePerturbation() const {
    return rubyPerturbation().cast<analysis::DiscretePerturbation>();
  }

  analysis::RubyPerturbation RubyPerturbationRecord_Impl::rubyPerturbation() const {
    FileReferenceRecord scriptOrBCLMeasureRecord = this->scriptOrBCLMeasureRecord();
    OSArgumentRecordVector argumentRecords = this->osArgumentRecords();
    ruleset::OSArgumentVector arguments;
    BOOST_FOREACH(const OSArgumentRecord& argumentRecord,argumentRecords) {
      arguments.push_back(argumentRecord.osArgument());
    }

    return analysis::RubyPerturbation(handle(),
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

  void RubyPerturbationRecord_Impl::revertToLastRecordIds() {
    m_scriptOrBCLMeasureRecordId = m_lastScriptOrBCLMeasureRecordId;
  }

  void RubyPerturbationRecord_Impl::setScriptOrBCLMeasureRecordId(int id) {
    m_scriptOrBCLMeasureRecordId = id;
    this->onChange(false);
  }

  void RubyPerturbationRecord_Impl::bindValues(QSqlQuery& query) const
  {
    DiscretePerturbationRecord_Impl::bindValues(query);

    query.bindValue(DiscretePerturbationRecordColumns::rubyScriptRecordId, m_scriptOrBCLMeasureRecordId);
    query.bindValue(DiscretePerturbationRecordColumns::inputFileType, m_inputFileType.value());
    query.bindValue(DiscretePerturbationRecordColumns::outputFileType, m_outputFileType.value());
    query.bindValue(DiscretePerturbationRecordColumns::isUserScript, m_isUserScript);
    query.bindValue(DiscretePerturbationRecordColumns::usesBCLMeasure, m_usesBCLMeasure);
  }

  void RubyPerturbationRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    DiscretePerturbationRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastScriptOrBCLMeasureRecordId = value.toInt();

    value = query.value(DiscretePerturbationRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastInputFileType = FileReferenceType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastOutputFileType = FileReferenceType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastIsUserScript = value.toBool();

    value = query.value(DiscretePerturbationRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastUsesBCLMeasure = value.toBool();
  }

  bool RubyPerturbationRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && DiscretePerturbationRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::rubyScriptRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_scriptOrBCLMeasureRecordId == value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::inputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_inputFileType == FileReferenceType(value.toInt()));

    value = query.value(DiscretePerturbationRecordColumns::outputFileType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_outputFileType == FileReferenceType(value.toInt()));

    value = query.value(DiscretePerturbationRecordColumns::isUserScript);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_isUserScript == value.toBool());

    value = query.value(DiscretePerturbationRecordColumns::usesBCLMeasure);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_usesBCLMeasure == value.toBool());

    return result;
  }

  void RubyPerturbationRecord_Impl::saveLastValues()
  {
    DiscretePerturbationRecord_Impl::saveLastValues();

    m_lastScriptOrBCLMeasureRecordId = m_scriptOrBCLMeasureRecordId;
    m_lastInputFileType = m_inputFileType;
    m_lastOutputFileType = m_outputFileType;
    m_lastIsUserScript = m_isUserScript;
    m_lastUsesBCLMeasure = m_usesBCLMeasure;
  }

  void RubyPerturbationRecord_Impl::revertToLastValues()
  {
    DiscretePerturbationRecord_Impl::revertToLastValues();

    m_scriptOrBCLMeasureRecordId = m_lastScriptOrBCLMeasureRecordId;
    m_inputFileType = m_lastInputFileType;
    m_outputFileType = m_lastOutputFileType;
    m_isUserScript = m_lastIsUserScript;
    m_usesBCLMeasure = m_lastUsesBCLMeasure;
  }

} // detail

RubyPerturbationRecord::RubyPerturbationRecord(const analysis::RubyPerturbation& rubyPerturbation,
                                               DiscreteVariableRecord& discreteVariableRecord,
                                               int perturbationVectorIndex)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl>(
        new detail::RubyPerturbationRecord_Impl(rubyPerturbation,
                                                discreteVariableRecord,
                                                perturbationVectorIndex)),
        discreteVariableRecord.projectDatabase())
{
  BOOST_ASSERT(getImpl<detail::RubyPerturbationRecord_Impl>());

  constructRelatedRecords(rubyPerturbation);
}

RubyPerturbationRecord::RubyPerturbationRecord(const analysis::RubyPerturbation& rubyPerturbation,
                                               ProjectDatabase& database)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl>(
        new detail::RubyPerturbationRecord_Impl(rubyPerturbation,
                                                database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::RubyPerturbationRecord_Impl>());

  constructRelatedRecords(rubyPerturbation);
}

RubyPerturbationRecord::RubyPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl>(
        new detail::RubyPerturbationRecord_Impl(query, database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::RubyPerturbationRecord_Impl>());
}

RubyPerturbationRecord::RubyPerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl> impl, ProjectDatabase database)
  : DiscretePerturbationRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::RubyPerturbationRecord_Impl>());
}

RubyPerturbationRecord::RubyPerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl> impl)
  : DiscretePerturbationRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::RubyPerturbationRecord_Impl>());
}

boost::optional<RubyPerturbationRecord> RubyPerturbationRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalRubyPerturbationRecord result;
  try {
    result = RubyPerturbationRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct RubyPerturbationRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<RubyPerturbationRecord> RubyPerturbationRecord::getRubyPerturbationRecords(ProjectDatabase& database)
{
  std::vector<RubyPerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() + " WHERE discretePerturbationRecordType=:discretePerturbationRecordType"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::RubyPerturbationRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(RubyPerturbationRecord(query,database));
  }

  return result;
}

boost::optional<RubyPerturbationRecord> RubyPerturbationRecord::getRubyPerturbationRecord(int id, ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RubyPerturbationRecord::databaseTableName() + " WHERE discretePerturbationRecordType=:discretePerturbationRecordType AND id=:id"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::RubyPerturbationRecord);
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    return RubyPerturbationRecord(query,database);
  }

  return boost::none;
}

bool RubyPerturbationRecord::usesBCLMeasure() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->usesBCLMeasure();
}

FileReferenceRecord RubyPerturbationRecord::scriptOrBCLMeasureRecord() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->scriptOrBCLMeasureRecord();
}

FileReferenceType RubyPerturbationRecord::inputFileType() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->inputFileType();
}

FileReferenceType RubyPerturbationRecord::outputFileType() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->outputFileType();
}

std::vector<OSArgumentRecord> RubyPerturbationRecord::osArgumentRecords() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->osArgumentRecords();
}

analysis::RubyPerturbation RubyPerturbationRecord::rubyPerturbation() const {
  return getImpl<detail::RubyPerturbationRecord_Impl>()->rubyPerturbation();
}

void RubyPerturbationRecord::constructRelatedRecords(const analysis::RubyPerturbation& rubyPerturbation) {
  RubyPerturbationRecord copyOfThis(getImpl<detail::RubyPerturbationRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::RubyPerturbationRecord_Impl>()->revertToLastRecordIds();
  }

  // Save child FileReference
  OptionalFileReference oChildFileReference;
  if (rubyPerturbation.usesBCLMeasure()) {
    // co-opt FileReference to store necessary data
    oChildFileReference = FileReference(rubyPerturbation.measureUUID(),
                                        rubyPerturbation.measureVersionUUID(),
                                        "",
                                        "",
                                        "",
                                        rubyPerturbation.measureDirectory(),
                                        FileReferenceType::Unknown,
                                        DateTime::now(),
                                        DateTime::now(),
                                        "",
                                        "");
  }
  else {
    oChildFileReference = rubyPerturbation.perturbationScript();
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
    getImpl<detail::RubyPerturbationRecord_Impl>()->setScriptOrBCLMeasureRecordId(scriptOrBCLMeasureRecord.id());
  }

  // Save child OSArguments
  OSArgumentVector arguments = rubyPerturbation.arguments();
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

void RubyPerturbationRecord::removeOSArgumentRecords(const std::vector<UUID>& uuidsToKeep,
                                                     ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + OSArgumentRecord::databaseTableName() +
        " WHERE (rubyPerturbationRecordId=:rubyPerturbationRecordId) AND (handle NOT IN (";
  std::string sep("");
  BOOST_FOREACH(const UUID& handle,uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":rubyPerturbationRecordId",id());
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

