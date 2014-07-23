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

#include "JoinRecord.hpp"
#include "JoinRecord_Impl.hpp"
#include "ProjectDatabase.hpp"
#include "ProjectDatabase_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/String.hpp"

#include <QSqlQuery>

namespace openstudio {
namespace project {
  namespace detail {

    JoinRecord_Impl::JoinRecord_Impl(int leftId, const UUID& leftHandle, int rightId, const UUID& rightHandle, const ProjectDatabase& database)
      : Record_Impl(database), m_leftId(leftId), m_leftHandle(leftHandle), m_rightId(rightId), m_rightHandle(rightHandle)
    {}

    JoinRecord_Impl::JoinRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
      : Record_Impl(database, query)
    {
        OS_ASSERT(query.isValid());
        OS_ASSERT(query.isActive());
        OS_ASSERT(query.isSelect());

        QVariant value;
        value = query.value(JoinRecordColumns::leftId);
        OS_ASSERT(value.isValid() && !value.isNull());
        m_leftId = value.toInt();

        value = query.value(JoinRecordColumns::leftHandle);
        OS_ASSERT(value.isValid() && !value.isNull());
        m_leftHandle = toUUID(value.toString().toStdString());

        value = query.value(JoinRecordColumns::rightId);
        OS_ASSERT(value.isValid() && !value.isNull());
        m_rightId = value.toInt();

        value = query.value(JoinRecordColumns::rightHandle);
        OS_ASSERT(value.isValid() && !value.isNull());
        m_rightHandle = toUUID(value.toString().toStdString());
    }

    int JoinRecord_Impl::leftId() const
    {
      return m_leftId;
    }

    UUID JoinRecord_Impl::leftHandle() const
    {
      return m_leftHandle;
    }

    int JoinRecord_Impl::rightId() const
    {
      return m_rightId;
    }

    UUID JoinRecord_Impl::rightHandle() const
    {
      return m_rightHandle;
    }

    boost::optional<ObjectRecord> JoinRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> JoinRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> JoinRecord_Impl::resources() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<JoinRecord> JoinRecord_Impl::joinRecords() const
    {
      return std::vector<JoinRecord>();
    }

    void JoinRecord_Impl::bindValues(QSqlQuery& query) const
    {
      Record_Impl::bindValues(query);

      query.bindValue(JoinRecordColumns::leftId, m_leftId);
      query.bindValue(JoinRecordColumns::leftHandle, toQString(toString(m_leftHandle)));
      query.bindValue(JoinRecordColumns::rightId, m_rightId);
      query.bindValue(JoinRecordColumns::rightHandle, toQString(toString(m_rightHandle)));
    }

    void JoinRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      Record_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(JoinRecordColumns::leftId);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_lastLeftId = value.toInt();

      value = query.value(JoinRecordColumns::leftHandle);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_lastLeftHandle = toUUID(value.toString().toStdString());

      value = query.value(JoinRecordColumns::rightId);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_lastRightId = value.toInt();

      value = query.value(JoinRecordColumns::rightHandle);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_lastRightHandle = toUUID(value.toString().toStdString());
    }

    bool JoinRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      bool result = true;

      result = result && Record_Impl::compareValues(query);

      QVariant value;

      value = query.value(JoinRecordColumns::leftId);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_leftId == value.toInt());

      value = query.value(JoinRecordColumns::leftHandle);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_leftHandle == toUUID(value.toString().toStdString()));

      value = query.value(JoinRecordColumns::rightId);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_rightId == value.toInt());

      value = query.value(JoinRecordColumns::rightHandle);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_rightHandle == toUUID(value.toString().toStdString()));

      return result;
    }

    void JoinRecord_Impl::saveLastValues()
    {
      Record_Impl::saveLastValues();

      m_lastLeftId = m_leftId;
      m_lastLeftHandle = m_leftHandle;
      m_lastRightId = m_rightId;
      m_lastRightHandle = m_rightHandle;
    }

    void JoinRecord_Impl::revertToLastValues()
    {
      Record_Impl::revertToLastValues();

      m_leftId = m_lastLeftId;
      m_leftHandle = m_lastLeftHandle;
      m_rightId = m_lastRightId;
      m_rightHandle = m_lastRightHandle;
    }

  } // detail

  JoinRecord::JoinRecord(std::shared_ptr<detail::JoinRecord_Impl> impl, ProjectDatabase projectDatabase)
    : Record(impl, projectDatabase)
  {
    OS_ASSERT(getImpl<detail::JoinRecord_Impl>());
  }

  JoinRecord::JoinRecord(std::shared_ptr<detail::JoinRecord_Impl> impl)
    : Record(impl)
  {
    OS_ASSERT(getImpl<detail::JoinRecord_Impl>());
  }

  void JoinRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
  {
    // id is already indexed due to primary key modifier

    QSqlQuery query(qSqlDatabase);
    query.prepare(toQString("CREATE INDEX " + databaseTableName + "handleindex ON " + databaseTableName + " (handle)"));
    assertExec(query);

    query.prepare(toQString("CREATE INDEX " + databaseTableName + "leftidindex ON " + databaseTableName + " (leftId)"));
    assertExec(query);

    query.prepare(toQString("CREATE INDEX " + databaseTableName + "lefthandleindex ON " + databaseTableName + " (leftHandle)"));
    assertExec(query);

    query.prepare(toQString("CREATE INDEX " + databaseTableName + "rightidindex ON " + databaseTableName + " (rightId)"));
    assertExec(query);

    query.prepare(toQString("CREATE INDEX " + databaseTableName + "righthandleindex ON " + databaseTableName + " (rightHandle)"));
    assertExec(query);
  }

  int JoinRecord::leftId() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->leftId();
  }

  UUID JoinRecord::leftHandle() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->leftHandle();
  }

  ObjectRecord JoinRecord::leftObject() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->leftObject();
  }

  int JoinRecord::rightId() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->rightId();
  }

  UUID JoinRecord::rightHandle() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->rightHandle();
  }

  ObjectRecord JoinRecord::rightObject() const
  {
    return this->getImpl<detail::JoinRecord_Impl>()->rightObject();
  }

} // project
} // openstudio
