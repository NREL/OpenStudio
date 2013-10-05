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

#include <project/ObjectRecord.hpp>
#include <project/ObjectRecord_Impl.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ProjectDatabase_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/String.hpp>
#include <utilities/time/DateTime.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

namespace openstudio {
namespace project {

namespace detail {

  ObjectRecord_Impl::ObjectRecord_Impl(const std::string& name,
                                       const std::string& displayName,
                                       const std::string& description)
    : Record_Impl(),
      m_name(name),
      m_displayName(displayName),
      m_description(description),
      m_timestampCreate(DateTime::now()),
      m_timestampLast(m_timestampCreate),
      m_uuidLast(this->handle().toString())
  {}

  ObjectRecord_Impl::ObjectRecord_Impl(const ProjectDatabase& projectDatabase,
                                       const std::string& name,
                                       const std::string& displayName,
                                       const std::string& description)
    : Record_Impl(projectDatabase),
      m_name(name),
      m_displayName(displayName),
      m_description(description),
      m_timestampCreate(DateTime::now()),
      m_timestampLast(m_timestampCreate),
      m_uuidLast(this->handle().toString())
  {}

  ObjectRecord_Impl::ObjectRecord_Impl(const ProjectDatabase& projectDatabase,
                                       const UUID& handle,
                                       const std::string& name,
                                       const std::string& displayName,
                                       const std::string& description,
                                       const UUID& uuidLast)
    : Record_Impl(projectDatabase, handle),
      m_name(name),
      m_displayName(displayName),
      m_description(description),
      m_timestampCreate(DateTime::now()),
      m_timestampLast(m_timestampCreate),
      m_uuidLast(uuidLast)
  {}

  ObjectRecord_Impl::ObjectRecord_Impl(const ProjectDatabase& projectDatabase, const QSqlQuery& query)
    : Record_Impl(projectDatabase, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ObjectRecordColumns::name);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_name = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::displayName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_displayName = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::description);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_description = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::timestampCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_timestampCreate = DateTime(value.toString().toStdString());

    value = query.value(ObjectRecordColumns::timestampLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_timestampLast = DateTime(value.toString().toStdString());

    value = query.value(ObjectRecordColumns::uuidLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_uuidLast = toUUID(value.toString().toStdString());
  }

  ObjectRecord_Impl::~ObjectRecord_Impl()
  {
  }

  std::string ObjectRecord_Impl::name() const
  {
    return m_name;
  }

  bool ObjectRecord_Impl::setName(const std::string& name)
  {
    m_name = name;
    this->onChange();
    return true;
  }

  std::string ObjectRecord_Impl::displayName() const
  {
    return m_displayName;
  }

  bool ObjectRecord_Impl::setDisplayName(const std::string& displayName)
  {
    m_displayName = displayName;
    this->onChange();
    return true;
  }

  std::string ObjectRecord_Impl::description() const
  {
    return m_description;
  }

  bool ObjectRecord_Impl::setDescription(const std::string& description)
  {
    m_description = description;
    this->onChange();
    return true;
  }

  DateTime ObjectRecord_Impl::timestampCreate() const
  {
    return m_timestampCreate;
  }

  DateTime ObjectRecord_Impl::timestampLast() const
  {
    return m_timestampLast;
  }

  UUID ObjectRecord_Impl::uuidLast() const
  {
    return m_uuidLast;
  }

  void ObjectRecord_Impl::updatePathData(const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
  {
    // default implementation does nothing.
  }

  UUID ObjectRecord_Impl::lastUuidLast() const {
    return m_lastUUIDLast;
  }

  void ObjectRecord_Impl::bindValues(QSqlQuery& query) const
  {
    Record_Impl::bindValues(query);

    query.bindValue(ObjectRecordColumns::name, toQString(m_name));
    query.bindValue(ObjectRecordColumns::displayName, toQString(m_displayName));
    query.bindValue(ObjectRecordColumns::description, toQString(m_description));
    query.bindValue(ObjectRecordColumns::timestampCreate, toQString(m_timestampCreate.toString()));
    query.bindValue(ObjectRecordColumns::timestampLast, toQString(m_timestampLast.toString()));
    query.bindValue(ObjectRecordColumns::uuidLast, toQString(toString(m_uuidLast)));
  }

  void ObjectRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    Record_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(ObjectRecordColumns::name);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastName = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::displayName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDisplayName = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::description);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDescription = value.toString().toStdString();

    value = query.value(ObjectRecordColumns::timestampCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastTimestampCreate = DateTime(value.toString().toStdString());

    value = query.value(ObjectRecordColumns::timestampLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastTimestampLast = DateTime(value.toString().toStdString());

    value = query.value(ObjectRecordColumns::uuidLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUUIDLast = toUUID(value.toString().toStdString());
  }

  void ObjectRecord_Impl::saveLastValues()
  {
    Record_Impl::saveLastValues();

    m_lastName = m_name;
    m_lastDisplayName = m_displayName;
    m_lastDescription = m_description;
    m_lastTimestampCreate = m_timestampCreate;
    m_lastTimestampLast = m_timestampLast;
    m_lastUUIDLast = m_uuidLast;
  }

  void ObjectRecord_Impl::revertToLastValues()
  {
    Record_Impl::revertToLastValues();

    m_name = m_lastName;
    m_displayName = m_lastDisplayName;
    m_description = m_lastDescription;
    m_timestampCreate = m_lastTimestampCreate;
    m_timestampLast = m_lastTimestampLast;
    m_uuidLast = m_lastUUIDLast;
  }

  bool ObjectRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = true;

    result = result && Record_Impl::compareValues(query);
    if (!result) return result;

    QVariant value;

    value = query.value(ObjectRecordColumns::name);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_name == value.toString().toStdString());
    if (!result) return result;

    value = query.value(ObjectRecordColumns::displayName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_displayName == value.toString().toStdString());
    if (!result) return result;

    value = query.value(ObjectRecordColumns::description);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_description == value.toString().toStdString());
    if (!result) return result;

    value = query.value(ObjectRecordColumns::timestampCreate);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_timestampCreate == DateTime(value.toString().toStdString()));
    if (!result) return result;

    value = query.value(ObjectRecordColumns::timestampLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_timestampLast == DateTime(value.toString().toStdString()));
    if (!result) return result;

    value = query.value(ObjectRecordColumns::uuidLast);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_uuidLast == toUUID(value.toString().toStdString()));

    return result;
  }

  void ObjectRecord_Impl::onChange(bool newVersion)
  {
    if (newVersion) {
      m_lastUUIDLast = m_uuidLast;
      m_lastTimestampLast = m_timestampLast;

      m_uuidLast = createUUID();
      m_timestampLast = DateTime::now();
    }

    Record_Impl::onChange(newVersion);
  }

  bool ObjectRecord_Impl::setUuidLast(const UUID& uuidLast)
  {
    m_uuidLast = uuidLast;

    Record_Impl::onChange();

    return true;
  }

} // detail

ObjectRecord::ObjectRecord(boost::shared_ptr<detail::ObjectRecord_Impl> impl, ProjectDatabase projectDatabase)
  : Record(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::ObjectRecord_Impl>());
}

ObjectRecord::ObjectRecord(boost::shared_ptr<detail::ObjectRecord_Impl> impl)
  : Record(impl)
{
  OS_ASSERT(getImpl<detail::ObjectRecord_Impl>());
}

void ObjectRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  // id is already indexed due to primary key modifier

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "handleindex ON " + databaseTableName + " (handle)"));
  assertExec(query);
}

std::string ObjectRecord::name() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->name();
}

bool ObjectRecord::setName(const std::string& name)
{
  return this->getImpl<detail::ObjectRecord_Impl>()->setName(name);
}

std::string ObjectRecord::displayName() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->displayName();
}

bool ObjectRecord::setDisplayName(const std::string& displayName)
{
  return this->getImpl<detail::ObjectRecord_Impl>()->setDisplayName(displayName);
}

std::string ObjectRecord::description() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->description();
}

bool ObjectRecord::setDescription(const std::string& description)
{
  return this->getImpl<detail::ObjectRecord_Impl>()->setDescription(description);
}

DateTime ObjectRecord::timestampCreate() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->timestampCreate();
}

DateTime ObjectRecord::timestampLast() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->timestampLast();
}

UUID ObjectRecord::uuidLast() const
{
  return this->getImpl<detail::ObjectRecord_Impl>()->uuidLast();
}

void ObjectRecord::updatePathData(const openstudio::path& originalBase,
                                  const openstudio::path& newBase)
{
  getImpl<detail::ObjectRecord_Impl>()->updatePathData(originalBase,newBase);
}

} // project
} // openstudio
