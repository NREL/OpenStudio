/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
