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

#include "ProjectDatabaseRecord.hpp"
#include "ProjectDatabaseRecord_Impl.hpp"
#include "JoinRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/Workspace.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ProjectDatabaseRecord_Impl::ProjectDatabaseRecord_Impl(const std::string& version,
                                                         const openstudio::path& runManagerDBPath,
                                                         const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase,
                        toString(projectDatabase.path()),
                        toString(projectDatabase.path().filename()),
                        ""),
    m_version(version),
    m_runManagerDBPath(completeAndNormalize(runManagerDBPath))
  {
  }

  ProjectDatabaseRecord_Impl::ProjectDatabaseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ProjectDatabaseRecordColumns::version);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_version = value.toString().toStdString();

    value = query.value(ProjectDatabaseRecordColumns::runManagerDBPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_runManagerDBPath = toPath(value.toString());
  }

  std::string ProjectDatabaseRecord_Impl::version() const
  {
    return m_version;
  }

  openstudio::path ProjectDatabaseRecord_Impl::runManagerDBPath() const
  {
    return m_runManagerDBPath;
  }

  std::string ProjectDatabaseRecord_Impl::databaseTableName() const
  {
    return ProjectDatabaseRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> ProjectDatabaseRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ProjectDatabaseRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ProjectDatabaseRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ProjectDatabaseRecord_Impl::joinRecords() const
  {
    return std::vector<JoinRecord>();
  }

  void ProjectDatabaseRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ProjectDatabaseRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ProjectDatabaseRecord_Impl::updatePathData(const openstudio::path& originalBase,
                                                  const openstudio::path& newBase)
  {
    // don't use arguments -- go to the source
    ProjectDatabase database = projectDatabase();
    setName(toString(database.path()));
    setRunManagerDBPath(completeAndNormalize(database.runManager().dbPath()));
  }

  void ProjectDatabaseRecord_Impl::setVersion(const std::string& version) {
    m_version = version;
    this->onChange();
  }

  void ProjectDatabaseRecord_Impl::setRunManagerDBPath(const openstudio::path& newPath) {
    m_runManagerDBPath = completeAndNormalize(newPath);
    this->onChange();
  }

  void ProjectDatabaseRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(ProjectDatabaseRecordColumns::version, toQString(m_version));
    query.bindValue(ProjectDatabaseRecordColumns::runManagerDBPath, toQString(toString(m_runManagerDBPath)));
  }

  void ProjectDatabaseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);
    
    QVariant value;
    value = query.value(ProjectDatabaseRecordColumns::version);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastVersion = value.toString().toStdString();

    value = query.value(ProjectDatabaseRecordColumns::runManagerDBPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRunManagerDBPath = toPath(value.toString());
  }

  bool ProjectDatabaseRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ProjectDatabaseRecordColumns::version);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_version == value.toString().toStdString());

    value = query.value(ProjectDatabaseRecordColumns::runManagerDBPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_runManagerDBPath == toPath(value.toString()));

    return result;
  }

  void ProjectDatabaseRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastVersion = m_version;
    m_lastRunManagerDBPath = m_runManagerDBPath;
  }

  void ProjectDatabaseRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_version = m_lastVersion;
    m_runManagerDBPath = m_lastRunManagerDBPath;
  }

} // detail

std::string ProjectDatabaseRecord::databaseTableName() {
  return "ProjectDatabaseRecords";
}

UpdateByIdQueryData ProjectDatabaseRecord::updateByIdQueryData() {
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

ProjectDatabaseRecord ProjectDatabaseRecord::getProjectDatabaseRecord(
    const ProjectDatabase& database)
{
  return database.projectDatabaseRecord();
}

void ProjectDatabaseRecord::updatePathData(ProjectDatabase database,
                                           const openstudio::path& originalBase,
                                           const openstudio::path& newBase)
{
  bool didStartTransaction = database.startTransaction();

  ProjectDatabaseRecord record = getProjectDatabaseRecord(database);
  record.getImpl<detail::ProjectDatabaseRecord_Impl>()->updatePathData(originalBase,newBase);

  if (didStartTransaction) {
    bool test = database.commitTransaction();
    OS_ASSERT(test);
  }
}

ProjectDatabaseRecord::ProjectDatabaseRecord(const std::string& version,
                                             const openstudio::path& runManagerDBPath,
                                             ProjectDatabase projectDatabase)
  : ObjectRecord(std::shared_ptr<detail::ProjectDatabaseRecord_Impl>(
                     new detail::ProjectDatabaseRecord_Impl(version,
                                                            runManagerDBPath,
                                                            projectDatabase)),
                 projectDatabase)
{
  OS_ASSERT(this->getImpl<detail::ProjectDatabaseRecord_Impl>());
}

ProjectDatabaseRecord::ProjectDatabaseRecord(std::shared_ptr<detail::ProjectDatabaseRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(this->getImpl<detail::ProjectDatabaseRecord_Impl>());
}

std::string ProjectDatabaseRecord::version() const
{
  return this->getImpl<detail::ProjectDatabaseRecord_Impl>()->version();
}

openstudio::path ProjectDatabaseRecord::runManagerDBPath() const
{
  return this->getImpl<detail::ProjectDatabaseRecord_Impl>()->runManagerDBPath();
}

void ProjectDatabaseRecord::setVersion(const std::string& version) {
  getImpl<detail::ProjectDatabaseRecord_Impl>()->setVersion(version);
}

} // project
} // openstudio
