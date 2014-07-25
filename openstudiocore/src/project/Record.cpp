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

#include "Record.hpp"
#include "Record_Impl.hpp"
#include "JoinRecord.hpp"
#include "ObjectRecord.hpp"
#include "ProjectDatabase.hpp"
#include "ProjectDatabase_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/time/DateTime.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QSqlDriver>
#include <QThread>

namespace openstudio {
namespace project {
  namespace detail {

    Record_Impl::Record_Impl()
      : m_projectDatabaseWeakImpl(),
        m_id(std::numeric_limits<int>::min()),
        m_handle(createUUID()),
        m_haveLastValues(false)
    {
    }

    Record_Impl::Record_Impl(const ProjectDatabase& projectDatabase)
      : m_projectDatabaseWeakImpl(projectDatabase.m_impl), m_id(std::numeric_limits<int>::min()),
        m_handle(createUUID()),
        m_haveLastValues(false)
    {}

    Record_Impl::Record_Impl(const ProjectDatabase& projectDatabase,
                             const UUID& handle)
      : m_projectDatabaseWeakImpl(projectDatabase.m_impl),
        m_id(std::numeric_limits<int>::min()),
        m_handle(handle),
        m_haveLastValues(false)
    {}

    Record_Impl::Record_Impl(const ProjectDatabase& projectDatabase, const QSqlQuery& query)
      : m_projectDatabaseWeakImpl(projectDatabase.m_impl),
        m_id(std::numeric_limits<int>::min()),
        m_handle(),
        m_haveLastValues(false)
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(RecordColumns::id);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_id = value.toInt();

      value = query.value(RecordColumns::handle);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_handle = toUUID(value.toString().toStdString());
    }

    Record_Impl::~Record_Impl()
    {
    }

    ProjectDatabase Record_Impl::projectDatabase() const
    {
      std::shared_ptr<detail::ProjectDatabase_Impl> impl = m_projectDatabaseWeakImpl.lock();
      OS_ASSERT(impl);
      return ProjectDatabase(impl);
    }

    int Record_Impl::id() const
    {
      return m_id;
    }

    UUID Record_Impl::handle() const
    {
      return m_handle;
    }

    bool Record_Impl::revertRow(ProjectDatabase& projectDatabase)
    {
      if (!this->haveLastValues()){
        return false;
      }

      this->revertToLastValues();
      this->saveRow(projectDatabase.qSqlDatabase());
      return true;
    }

    bool Record_Impl::connect(const std::string& signal, const QObject* qObject, const std::string& slot, Qt::ConnectionType type) const
    {
      return QObject::connect(this, signal.c_str(), qObject, slot.c_str(), type);
    }

    void Record_Impl::bindValues(QSqlQuery& query) const
    {
      OS_ASSERT(m_id);

      query.bindValue(RecordColumns::id, m_id);
      query.bindValue(RecordColumns::handle, toQString(toString(m_handle)));
    }

    void Record_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ObjectRecordColumns::id);
      OS_ASSERT(value.isValid() && !value.isNull());
      if (m_id == std::numeric_limits<int>::min()){
        m_id = value.toInt();
      }else{
        OS_ASSERT(m_id == value.toInt());
      }

      value = query.value(ObjectRecordColumns::handle);
      OS_ASSERT(value.isValid() && !value.isNull());
      OS_ASSERT(m_handle == toUUID(value.toString().toStdString()));

      m_haveLastValues = true;
    }

    void Record_Impl::saveLastValues()
    {
      m_haveLastValues = true;
    }

    void Record_Impl::revertToLastValues()
    {
      OS_ASSERT(m_haveLastValues);
    }

    bool Record_Impl::compareValues(const QSqlQuery& query) const
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      bool result = true;

      QVariant value;

      // ids may be different
      //value = query.value(ObjectRecordColumns::id);
      //OS_ASSERT(value.isValid() && !value.isNull());
      //result = result && (m_id == value.toInt());

      value = query.value(ObjectRecordColumns::handle);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_handle == toUUID(value.toString().toStdString()));

      return result;
    }

    void Record_Impl::onChange(bool newVersion)
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      this->saveRow(projectDatabase.qSqlDatabase());

      emit onChange(m_handle);
    }

    boost::optional<int> Record_Impl::findIdByHandle() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();

      boost::optional<int> result;

      QSqlQuery query(*(projectDatabase.qSqlDatabase()));
      query.prepare(toQString("SELECT id FROM " + this->databaseTableName() + " WHERE handle=:handle"));
      query.bindValue(":handle", toQString(toString(this->handle())));

      assertExec(query);
      if(query.first()){
        result = query.value(0).toInt();
      }

      return result;
   }

    void Record_Impl::insertRow(const std::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);

      // check there is not already an entry
      query.prepare(QString::fromStdString("SELECT id FROM " + this->databaseTableName() + " WHERE handle=:handle"));
      query.bindValue(":handle", toQString(toString(this->handle())));
      assertExec(query);
      OS_ASSERT(!query.first());

      // do the insert
      query.prepare(QString::fromStdString("INSERT INTO " + this->databaseTableName() + " (id) VALUES (:id)"));
      query.bindValue(":id", QVariant(QVariant::Int));
      assertExec(query);

      // get the new id
      if (database->driver()->hasFeature(QSqlDriver::LastInsertId)){
        QVariant id = query.lastInsertId().toInt();
        OS_ASSERT(id.isValid() && !id.isNull());
        m_id = id.toInt();
      }else{
        query.prepare(QString::fromStdString("SELECT id FROM " + this->databaseTableName()));
        assertExec(query);
        assertLast(query);
        m_id = query.value(0).toInt();
      }
    }

    void Record_Impl::removeRow(const std::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeDeleteByIdQuery(query);
      assertExec(query);
    }

    void Record_Impl::setLastValues(ProjectDatabase& projectDatabase)
    {
      QSqlQuery query(*(projectDatabase.qSqlDatabase()));
      this->makeSelectByHandleQuery(query);
      assertExec(query);
      assertFirst(query);
      this->setLastValues(query, projectDatabase);
    }

    bool Record_Impl::databaseCompare(const ProjectDatabase& projectDatabase) const
    {
      QSqlQuery query(*(projectDatabase.qSqlDatabase()));
      this->makeSelectByHandleQuery(query);
      assertExec(query);
      assertFirst(query);
      return this->compareValues(query);
    }

    void Record_Impl::makeSelectAllQuery(QSqlQuery& query) const
    {
      std::stringstream ss;
      ss << "SELECT * FROM " << this->databaseTableName();

      query.prepare(QString::fromStdString(ss.str()));
    }

    void Record_Impl::makeSelectByIdQuery(QSqlQuery& query) const
    {
      std::stringstream ss;
      ss << "SELECT * FROM " << this->databaseTableName() << " WHERE id=:id";

      query.prepare(QString::fromStdString(ss.str()));
      query.bindValue(":id", this->id());
    }

    void Record_Impl::makeSelectByHandleQuery(QSqlQuery& query) const
    {
      std::stringstream ss;
      ss << "SELECT * FROM " << this->databaseTableName() << " WHERE handle=:handle";

      query.prepare(QString::fromStdString(ss.str()));
      query.bindValue(":handle", toQString(toString(this->handle())));
    }

    void Record_Impl::makeDeleteByIdQuery(QSqlQuery& query) const
    {
      std::stringstream ss;
      ss << "DELETE FROM " << this->databaseTableName() << " WHERE id=:id";

      query.prepare(QString::fromStdString(ss.str()));
      query.bindValue(":id", this->id());
    }

    void Record_Impl::makeDeleteByHandleQuery(QSqlQuery& query) const
    {
      std::stringstream ss;
      ss << "DELETE FROM " << this->databaseTableName() << " WHERE handle=:handle";

      query.prepare(QString::fromStdString(ss.str()));
      query.bindValue(":handle", toQString(toString(this->handle())));
    }

    bool Record_Impl::haveLastValues() const
    {
      return m_haveLastValues;
    }

  } // detail


  Record::Record(std::shared_ptr<detail::Record_Impl> impl, ProjectDatabase projectDatabase)
  {
    // check if this object is already loaded
    boost::optional<Record> record = projectDatabase.findLoadedRecord(impl->handle());
    if (record) {
      m_impl = record->getImpl<detail::Record_Impl>();
      OS_ASSERT(typeid(*impl) == typeid(*m_impl));
    }

    // check if this object is in the database but not yet loaded
    if (!m_impl){
      boost::optional<int> id = impl->findIdByHandle();
      if (id){

        // clean state of this object is from database
        // may be dirty if came from xml
        // ETH@20121107 - Using lower-level functionality instead of setLastValues(projectDatabase)
        // and databaseCompare(projectDatabase) for performance reasons.
        QSqlQuery query(*(projectDatabase.qSqlDatabase()));
        impl->makeSelectByHandleQuery(query);
        assertExec(query);
        assertFirst(query);
        impl->setLastValues(query, projectDatabase);
        if (impl->compareValues(query)) {
          Record record(impl);
          projectDatabase.addCleanRecord(record);
        }else{
          Record record(impl);
          projectDatabase.addDirtyRecord(record);
        }

        m_impl = impl;
      }
    }

    // keep this new object
    if (!m_impl){
      m_impl = impl;
      Record record(impl);
      projectDatabase.addNewRecord(record);
    }

    OS_ASSERT(m_impl);
  }

  Record::Record(std::shared_ptr<detail::Record_Impl> impl)
    : m_impl(impl)
  {
    OS_ASSERT(m_impl);
  }

  ProjectDatabase Record::projectDatabase() const
  {
    return m_impl->projectDatabase();
  }

  int Record::id() const
  {
    return m_impl->id();
  }

  UUID Record::handle() const
  {
    return m_impl->handle();
  }

  std::string Record::databaseTableName() const
  {
    return m_impl->databaseTableName();
  }

  std::vector<ObjectRecord> Record::children() const
  {
    return m_impl->children();
  }

  std::vector<ObjectRecord> Record::resources() const
  {
    return m_impl->resources();
  }

  std::vector<JoinRecord> Record::joinRecords() const
  {
    return m_impl->joinRecords();
  }

  void Record::saveRow(ProjectDatabase &projectDatabase)
  {
    m_impl->saveRow(projectDatabase.qSqlDatabase());
  }

  void Record::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    m_impl->saveRow(database);
  }

  bool Record::revertRow(ProjectDatabase& projectDatabase)
  {
    return m_impl->revertRow(projectDatabase);
  }

  bool Record::connect(const std::string& signal, const QObject* qObject, const std::string& slot, Qt::ConnectionType type) const
  {
    return m_impl->connect(signal, qObject, slot, type);
  }

  unsigned Record::useCount() const
  {
    return m_impl.use_count();
  }

  void Record::bindValues(QSqlQuery& query) const
  {
    return m_impl->bindValues(query);
  }

  void Record::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    return m_impl->setLastValues(query, projectDatabase);
  }

  bool Record::compareValues(const QSqlQuery& query) const
  {
    return m_impl->compareValues(query);
  }

  void Record::saveLastValues()
  {
    return m_impl->saveLastValues();
  }

  void Record::revertToLastValues()
  {
    return m_impl->revertToLastValues();
  }

  boost::optional<int> Record::findIdByHandle() const
  {
    return m_impl->findIdByHandle();
  }

  void Record::insertRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    m_impl->insertRow(database);
  }

  void Record::insertRow(ProjectDatabase& projectDatabase)
  {
    m_impl->insertRow(projectDatabase.qSqlDatabase());
  }

  void Record::removeRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    m_impl->removeRow(database);
  }

  void Record::removeRow(ProjectDatabase& projectDatabase)
  {
    m_impl->removeRow(projectDatabase.qSqlDatabase());
  }

  void Record::setLastValues(ProjectDatabase& projectDatabase)
  {
    m_impl->setLastValues(projectDatabase);
  }

  bool Record::databaseCompare(const ProjectDatabase& projectDatabase) const
  {
    return m_impl->databaseCompare(projectDatabase);
  }

  void Record::onChange()
  {
    m_impl->onChange();
  }

  void Record::makeSelectAllQuery(QSqlQuery& query) const
  {
    m_impl->makeSelectAllQuery(query);
  }

  void Record::makeSelectByIdQuery(QSqlQuery& query) const
  {
    m_impl->makeSelectByIdQuery(query);
  }

  void Record::makeSelectByHandleQuery(QSqlQuery& query) const
  {
    m_impl->makeSelectByHandleQuery(query);
  }

  void Record::makeDeleteByIdQuery(QSqlQuery& query) const
  {
    m_impl->makeDeleteByIdQuery(query);
  }

  void Record::makeDeleteByHandleQuery(QSqlQuery& query) const
  {
    m_impl->makeDeleteByHandleQuery(query);
  }

  bool Record::haveLastValues() const
  {
    return m_impl->haveLastValues();
  }

  void Record::moveToThread( QThread * targetThread )
  {
    m_impl->moveToThread(targetThread);
  }

  bool operator<(const Record& left, const Record& right)
  {
    return left.handle() < right.handle();
  }

  bool assertExec(QSqlQuery& query)
  {
    bool result = query.exec();
    if (!result){

      bool hasBoundValues = false;

      std::stringstream ss;
      QMapIterator<QString, QVariant> i(query.boundValues());
      unsigned count = 0;
      while (i.hasNext()) {
        hasBoundValues = true;
        i.next();
        if (i.key().toStdString().empty()){
          ss << i.key().toStdString() << "=" << i.value().toString().toStdString();
        }else{
          ss << count << "=" << i.value().toString().toStdString();
        }
        if (i.hasNext()){
          ss << ", ";
        }else{
          ss << " ";
        }
        ++count;
      }

      if (hasBoundValues){
        LOG_FREE_AND_THROW("QSqlQuery", "Query: " << query.lastQuery().toStdString() << std::endl <<
                      "BoundValues: " << ss.str() << std::endl <<
                      "Error: " << query.lastError().text().toStdString());
      }else{
        LOG_FREE_AND_THROW("QSqlQuery", "Query: " << query.lastQuery().toStdString() << std::endl <<
                      "Error: " << query.lastError().text().toStdString());
      }
    }
    return result;
  }

  bool assertFirst(QSqlQuery& query)
  {
    bool result = query.first();
    if (!result){
      LOG_FREE_AND_THROW("QSqlQuery", "Query: " << query.lastQuery().toStdString() << std::endl << "Error: " << query.lastError().text().toStdString());
    }
    return result;
  }

  bool assertLast(QSqlQuery& query)
  {
    bool result = query.last();
    if (!result){
      LOG_FREE_AND_THROW("QSqlQuery", "Query: " << query.lastQuery().toStdString() << std::endl << "Error: " << query.lastError().text().toStdString());
    }
    return result;
  }

} // project
} // openstudio
