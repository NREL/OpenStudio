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

#include "DakotaAlgorithmRecord.hpp"
#include "DakotaAlgorithmRecord_Impl.hpp"
#include "ProjectDatabase.hpp"
#include "AnalysisRecord.hpp"
#include "FileReferenceRecord.hpp"
#include "DDACEAlgorithmRecord.hpp"
#include "FSUDaceAlgorithmRecord.hpp"
#include "PSUADEDaceAlgorithmRecord.hpp"
#include "ParameterStudyAlgorithmRecord.hpp"
#include "SamplingAlgorithmRecord.hpp"

#include "../analysis/DDACEAlgorithm.hpp"
#include "../analysis/DDACEAlgorithm_Impl.hpp"
#include "../analysis/FSUDaceAlgorithm.hpp"
#include "../analysis/FSUDaceAlgorithm_Impl.hpp"
#include "../analysis/PSUADEDaceAlgorithm.hpp"
#include "../analysis/PSUADEDaceAlgorithm_Impl.hpp"
#include "../analysis/ParameterStudyAlgorithm.hpp"
#include "../analysis/ParameterStudyAlgorithm_Impl.hpp"
#include "../analysis/SamplingAlgorithm.hpp"
#include "../analysis/SamplingAlgorithm_Impl.hpp"

#include "JoinRecord.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DakotaAlgorithmRecord_Impl::DakotaAlgorithmRecord_Impl(
      const analysis::DakotaAlgorithm& dakotaAlgorithm,
      const DakotaAlgorithmRecordType& dakotaAlgorithmRecordType,
      AnalysisRecord& analysisRecord)
    : AlgorithmRecord_Impl(dakotaAlgorithm,
                           AlgorithmRecordType::DakotaAlgorithmRecord,
                           analysisRecord),
      m_dakotaAlgorithmRecordType(dakotaAlgorithmRecordType)
  {
    if (dakotaAlgorithm.job()) {
      m_jobUUID = dakotaAlgorithm.job()->uuid();
    }
  }

  DakotaAlgorithmRecord_Impl::DakotaAlgorithmRecord_Impl(const QSqlQuery& query,
                                                         ProjectDatabase& database)
    : AlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_dakotaAlgorithmRecordType = DakotaAlgorithmRecordType(value.toInt());

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaRestartFileRecordId);
    if (value.isValid() && !value.isNull()) {
      m_dakotaRestartFileRecordId = value.toInt();
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaOutFileRecordId);
    if (value.isValid() && !value.isNull()) {
      m_dakotaOutFileRecordId = value.toInt();
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::jobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      m_jobUUID = openstudio::UUID(value.toString());
    }
  }

  std::vector<ObjectRecord> DakotaAlgorithmRecord_Impl::children() const {
    ObjectRecordVector result = AlgorithmRecord_Impl::children();
    if (OptionalFileReferenceRecord restartFile = restartFileReferenceRecord()) {
      result.push_back(*restartFile);
    }
    if (OptionalFileReferenceRecord outFile = outFileReferenceRecord()) {
      result.push_back(*outFile);
    }
    return result;
  }

  void DakotaAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DakotaAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void DakotaAlgorithmRecord_Impl::reset() {
    if (m_dakotaRestartFileRecordId) {
      FileReferenceRecord restartFile = restartFileReferenceRecord().get();
      m_dakotaRestartFileRecordId.reset();
      projectDatabase().removeRecord(restartFile);
    }
    if (m_dakotaOutFileRecordId) {
      FileReferenceRecord outFile = outFileReferenceRecord().get();
      m_dakotaOutFileRecordId.reset();
      projectDatabase().removeRecord(outFile);
    }
    m_jobUUID.reset();
    AlgorithmRecord_Impl::reset(); // calls onChange
  }

  boost::optional<FileReferenceRecord> DakotaAlgorithmRecord_Impl::restartFileReferenceRecord() const
  {
    OptionalFileReferenceRecord result;
    if (m_dakotaRestartFileRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_dakotaRestartFileRecordId,database);
    }
    return result;
  }

  boost::optional<FileReferenceRecord> DakotaAlgorithmRecord_Impl::outFileReferenceRecord() const
  {
    OptionalFileReferenceRecord result;
    if (m_dakotaOutFileRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_dakotaOutFileRecordId,database);
    }
    return result;
  }

  boost::optional<openstudio::UUID> DakotaAlgorithmRecord_Impl::jobUUID() const
  {
    return m_jobUUID;
  }

  void DakotaAlgorithmRecord_Impl::setRestartFileReferenceRecordId(int id) {
    m_dakotaRestartFileRecordId = id;
    this->onChange(false);
  }

  void DakotaAlgorithmRecord_Impl::setOutFileReferenceRecordId(int id) {
    m_dakotaOutFileRecordId = id;
    this->onChange(false);
  }

  void DakotaAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    AlgorithmRecord_Impl::bindValues(query);

    query.bindValue(DakotaAlgorithmRecord::ColumnsType::dakotaAlgorithmRecordType,
                    m_dakotaAlgorithmRecordType.value());
    if (m_dakotaRestartFileRecordId) {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::dakotaRestartFileRecordId,
                      *m_dakotaRestartFileRecordId);
    }
    else {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::dakotaRestartFileRecordId,
                      QVariant(QVariant::Int));
    }

    if (m_dakotaOutFileRecordId) {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::dakotaOutFileRecordId,
                      *m_dakotaOutFileRecordId);
    }
    else {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::dakotaOutFileRecordId,
                      QVariant(QVariant::Int));
    }

    if (m_jobUUID) {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::jobUUID, m_jobUUID->toString());
    }
    else {
      query.bindValue(DakotaAlgorithmRecord::ColumnsType::jobUUID, QVariant(QVariant::String));
    }
  }

  void DakotaAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    AlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDakotaAlgorithmRecordType = DakotaAlgorithmRecordType(value.toInt());

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaRestartFileRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastDakotaRestartFileRecordId = value.toInt();
    }
    else {
      m_lastDakotaRestartFileRecordId.reset();
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaOutFileRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastDakotaOutFileRecordId = value.toInt();
    }
    else {
      m_lastDakotaOutFileRecordId.reset();
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::jobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      m_lastJobUUID = openstudio::UUID(value.toString());
    }else{
      m_lastJobUUID.reset();
    }
  }

  bool DakotaAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = AlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_dakotaAlgorithmRecordType == DakotaAlgorithmRecordType(value.toInt()));

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaRestartFileRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_dakotaRestartFileRecordId && (*m_dakotaRestartFileRecordId == value.toInt());
    }
    else {
      result = result && !m_dakotaRestartFileRecordId;
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::dakotaOutFileRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_dakotaOutFileRecordId && (*m_dakotaOutFileRecordId == value.toInt());
    }
    else {
      result = result && !m_dakotaOutFileRecordId;
    }

    value = query.value(DakotaAlgorithmRecord::ColumnsType::jobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      openstudio::UUID testUUID(value.toString());
      result = result && m_lastJobUUID && (m_lastJobUUID.get() == testUUID);
    }else{
      result = result && !m_lastJobUUID;
    }

    return result;
  }

  void DakotaAlgorithmRecord_Impl::saveLastValues() {
    AlgorithmRecord_Impl::saveLastValues();

    m_lastDakotaAlgorithmRecordType = m_dakotaAlgorithmRecordType;
    m_lastDakotaRestartFileRecordId = m_dakotaRestartFileRecordId;
    m_lastDakotaOutFileRecordId = m_dakotaOutFileRecordId;
    m_lastJobUUID = m_jobUUID;
  }

  void DakotaAlgorithmRecord_Impl::revertToLastValues() {
    AlgorithmRecord_Impl::revertToLastValues();

    m_dakotaAlgorithmRecordType = m_lastDakotaAlgorithmRecordType;
    m_dakotaRestartFileRecordId = m_lastDakotaRestartFileRecordId;
    m_dakotaOutFileRecordId = m_lastDakotaOutFileRecordId;
    m_jobUUID = m_lastJobUUID;
  }

} // detail

DakotaAlgorithmRecord::DakotaAlgorithmRecord(std::shared_ptr<detail::DakotaAlgorithmRecord_Impl> impl,
                                             ProjectDatabase database,
                                             const boost::optional<analysis::DakotaAlgorithm>& algorithm)
  : AlgorithmRecord(impl, database, boost::optional<analysis::Algorithm>(algorithm))
{
  OS_ASSERT(getImpl<detail::DakotaAlgorithmRecord_Impl>());
}

boost::optional<DakotaAlgorithmRecord> DakotaAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDakotaAlgorithmRecord result;

  int dakotaAlgorithmRecordType = query.value(AlgorithmRecordColumns::dakotaAlgorithmRecordType).toInt();

  switch (dakotaAlgorithmRecordType) {
    case DakotaAlgorithmRecordType::DDACEAlgorithmRecord :
      result = DDACEAlgorithmRecord(query, database).cast<DakotaAlgorithmRecord>();
     break;
     case DakotaAlgorithmRecordType::FSUDaceAlgorithmRecord :
      result = FSUDaceAlgorithmRecord(query, database).cast<DakotaAlgorithmRecord>();
     break;
     case DakotaAlgorithmRecordType::PSUADEDaceAlgorithmRecord :
      result = PSUADEDaceAlgorithmRecord(query, database).cast<DakotaAlgorithmRecord>();
     break;
     case DakotaAlgorithmRecordType::ParameterStudyAlgorithmRecord :
      result = ParameterStudyAlgorithmRecord(query, database).cast<DakotaAlgorithmRecord>();
     break;
     case DakotaAlgorithmRecordType::SamplingAlgorithmRecord :
      result = SamplingAlgorithmRecord(query, database).cast<DakotaAlgorithmRecord>();
     break;
    default :
      LOG(Error,"Unknown DakotaAlgorithmRecordType " << dakotaAlgorithmRecordType);
      return boost::none;
  }

  return result;
}

DakotaAlgorithmRecord DakotaAlgorithmRecord::factoryFromDakotaAlgorithm(
    const analysis::DakotaAlgorithm& dakotaAlgorithm, AnalysisRecord& analysisRecord)
{
  if (dakotaAlgorithm.optionalCast<analysis::DDACEAlgorithm>()) {
    return DDACEAlgorithmRecord(dakotaAlgorithm.cast<analysis::DDACEAlgorithm>(),
                                analysisRecord);
  }
  if (dakotaAlgorithm.optionalCast<analysis::FSUDaceAlgorithm>()) {
    return FSUDaceAlgorithmRecord(dakotaAlgorithm.cast<analysis::FSUDaceAlgorithm>(),
                                analysisRecord);
  }
  if (dakotaAlgorithm.optionalCast<analysis::PSUADEDaceAlgorithm>()) {
    return PSUADEDaceAlgorithmRecord(dakotaAlgorithm.cast<analysis::PSUADEDaceAlgorithm>(),
                                analysisRecord);
  }
  if (dakotaAlgorithm.optionalCast<analysis::ParameterStudyAlgorithm>()) {
    return ParameterStudyAlgorithmRecord(dakotaAlgorithm.cast<analysis::ParameterStudyAlgorithm>(),
                                analysisRecord);
  }
  if (dakotaAlgorithm.optionalCast<analysis::SamplingAlgorithm>()) {
    return SamplingAlgorithmRecord(dakotaAlgorithm.cast<analysis::SamplingAlgorithm>(),
                                analysisRecord);
  }

  OS_ASSERT(false);
  return DakotaAlgorithmRecord(std::shared_ptr<detail::DakotaAlgorithmRecord_Impl>());
}

std::vector<DakotaAlgorithmRecord> DakotaAlgorithmRecord::getDakotaAlgorithmRecords(ProjectDatabase& database) {
  std::vector<DakotaAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::DakotaAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    OptionalDakotaAlgorithmRecord dakotaAlgorithmRecord = DakotaAlgorithmRecord::factoryFromQuery(query, database);
    if (dakotaAlgorithmRecord) {
      result.push_back(*dakotaAlgorithmRecord);
    }
  }

  return result;
}

boost::optional<DakotaAlgorithmRecord> DakotaAlgorithmRecord::getDakotaAlgorithmRecord(int id, ProjectDatabase& database) {
  boost::optional<DakotaAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND id=:id"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::DakotaAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DakotaAlgorithmRecord::factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<FileReferenceRecord> DakotaAlgorithmRecord::restartFileReferenceRecord() const {
  return getImpl<detail::DakotaAlgorithmRecord_Impl>()->restartFileReferenceRecord();
}

boost::optional<FileReferenceRecord> DakotaAlgorithmRecord::outFileReferenceRecord() const {
  return getImpl<detail::DakotaAlgorithmRecord_Impl>()->outFileReferenceRecord();
}

boost::optional<openstudio::UUID> DakotaAlgorithmRecord::jobUUID() const {
  return getImpl<detail::DakotaAlgorithmRecord_Impl>()->jobUUID();
}

analysis::DakotaAlgorithm DakotaAlgorithmRecord::dakotaAlgorithm() const {
  return getImpl<detail::DakotaAlgorithmRecord_Impl>()->dakotaAlgorithm();
}

/// @cond
DakotaAlgorithmRecord::DakotaAlgorithmRecord(std::shared_ptr<detail::DakotaAlgorithmRecord_Impl> impl)
  : AlgorithmRecord(impl)
{}
/// @endcond

void DakotaAlgorithmRecord::constructRelatedRecords(const analysis::DakotaAlgorithm& algorithm) {
  DakotaAlgorithmRecord copyOfThis(getImpl<detail::DakotaAlgorithmRecord_Impl>());
  if (OptionalFileReference restartFile = algorithm.restartFileReference()) {
    FileReferenceRecord restartFileRecord(*restartFile,copyOfThis);
    setRestartFileReferenceRecordId(restartFileRecord.id());
  }
  if (OptionalFileReference outFile = algorithm.outFileReference()) {
    FileReferenceRecord outFileRecord(*outFile,copyOfThis);
    setOutFileReferenceRecordId(outFileRecord.id());
  }
}

void DakotaAlgorithmRecord::setRestartFileReferenceRecordId(int id) {
  getImpl<detail::DakotaAlgorithmRecord_Impl>()->setRestartFileReferenceRecordId(id);
}

void DakotaAlgorithmRecord::setOutFileReferenceRecordId(int id) {
  getImpl<detail::DakotaAlgorithmRecord_Impl>()->setOutFileReferenceRecordId(id);
}

} // project
} // openstudio
