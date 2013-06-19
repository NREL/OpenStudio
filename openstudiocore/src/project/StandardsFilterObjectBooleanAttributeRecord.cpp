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

#include <project/StandardsFilterObjectBooleanAttributeRecord.hpp>
#include <project/StandardsFilterObjectBooleanAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>

#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  StandardsFilterObjectBooleanAttributeRecord_Impl::StandardsFilterObjectBooleanAttributeRecord_Impl(const openstudio::ruleset::StandardsFilterObjectBooleanAttribute& standardsFilterObjectBooleanAttribute, const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(standardsFilterObjectBooleanAttribute, StandardsFilterObjectAttributeRecordType::StandardsFilterObjectBooleanAttributeRecord, projectDatabase), 
      m_attributeName(standardsFilterObjectBooleanAttribute.attribute()), m_attributeValue(standardsFilterObjectBooleanAttribute.value())
  {
  }

  StandardsFilterObjectBooleanAttributeRecord_Impl::StandardsFilterObjectBooleanAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = standardsinterface::BooleanAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toBool();
  }

  openstudio::ruleset::StandardsFilterObjectBooleanAttribute StandardsFilterObjectBooleanAttributeRecord_Impl::standardsFilterObjectBooleanAttribute() const
  {
    StandardsFilterAvailableObjectRecord standardsFilterAvailableObjectRecord = this->standardsFilterAvailableObjectRecord();
    openstudio::ruleset::StandardsFilterObjectBooleanAttribute result(standardsFilterAvailableObjectRecord.standardsFilterAvailableObject(), m_attributeName, m_attributeValue, this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::StandardsFilterObjectAttribute StandardsFilterObjectBooleanAttributeRecord_Impl::standardsFilterObjectAttribute() const
  {
    return this->standardsFilterObjectBooleanAttribute();
  }

  openstudio::ruleset::FilterClause StandardsFilterObjectBooleanAttributeRecord_Impl::filterClause() const
  {
    return this->standardsFilterObjectBooleanAttribute();
  }

  boost::optional<ObjectRecord> StandardsFilterObjectBooleanAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsFilterObjectBooleanAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsFilterObjectBooleanAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> StandardsFilterObjectBooleanAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsFilterObjectBooleanAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsFilterObjectBooleanAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsFilterObjectBooleanAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsFilterObjectAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName.valueName()));
    query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue);
  }

  void StandardsFilterObjectBooleanAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsFilterObjectAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = standardsinterface::BooleanAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toBool();
  }

  bool StandardsFilterObjectBooleanAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsFilterObjectAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == standardsinterface::BooleanAttribute(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toBool());

    return result;
  }

  void StandardsFilterObjectBooleanAttributeRecord_Impl::saveLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastAttributeValue = m_attributeValue;
  }

  void StandardsFilterObjectBooleanAttributeRecord_Impl::revertToLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_attributeValue = m_lastAttributeValue;
  }

} // detail

StandardsFilterObjectBooleanAttributeRecord::StandardsFilterObjectBooleanAttributeRecord(const openstudio::ruleset::StandardsFilterObjectBooleanAttribute& standardsFilterObjectBooleanAttribute, ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>(new detail::StandardsFilterObjectBooleanAttributeRecord_Impl(standardsFilterObjectBooleanAttribute, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>());
}

StandardsFilterObjectBooleanAttributeRecord::StandardsFilterObjectBooleanAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>(new detail::StandardsFilterObjectBooleanAttributeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>());
}

StandardsFilterObjectBooleanAttributeRecord::StandardsFilterObjectBooleanAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : StandardsFilterObjectAttributeRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>());
}

/// @cond
StandardsFilterObjectBooleanAttributeRecord::StandardsFilterObjectBooleanAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl> impl)
  : StandardsFilterObjectAttributeRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>());
}
/// @endcond

boost::optional<StandardsFilterObjectBooleanAttributeRecord> StandardsFilterObjectBooleanAttributeRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsFilterObjectBooleanAttributeRecord result;
  try {
    result = StandardsFilterObjectBooleanAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsFilterObjectBooleanAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsFilterObjectBooleanAttributeRecord> StandardsFilterObjectBooleanAttributeRecord::getStandardsFilterObjectBooleanAttributeRecords(ProjectDatabase& database) {
  std::vector<StandardsFilterObjectBooleanAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectBooleanAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsFilterObjectBooleanAttributeRecord(query, database));
  }

  return result;
}

boost::optional<StandardsFilterObjectBooleanAttributeRecord> StandardsFilterObjectBooleanAttributeRecord::getStandardsFilterObjectBooleanAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsFilterObjectBooleanAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectBooleanAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsFilterObjectBooleanAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsFilterObjectBooleanAttribute StandardsFilterObjectBooleanAttributeRecord::standardsFilterObjectBooleanAttribute() const
{
  return this->getImpl<detail::StandardsFilterObjectBooleanAttributeRecord_Impl>()->standardsFilterObjectBooleanAttribute();
}

} // project
} // openstudio
