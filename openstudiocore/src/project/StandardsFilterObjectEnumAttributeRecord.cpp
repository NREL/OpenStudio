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

#include <project/StandardsFilterObjectEnumAttributeRecord.hpp>
#include <project/StandardsFilterObjectEnumAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>

#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
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

  StandardsFilterObjectEnumAttributeRecord_Impl::StandardsFilterObjectEnumAttributeRecord_Impl(
      const openstudio::ruleset::StandardsFilterObjectEnumAttribute& standardsFilterObjectEnumAttribute, 
      const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(standardsFilterObjectEnumAttribute, 
                                                StandardsFilterObjectAttributeRecordType::StandardsFilterObjectEnumAttributeRecord, 
                                                projectDatabase), 
      m_attributeName(standardsFilterObjectEnumAttribute.attribute()), 
      m_attributeValue(standardsFilterObjectEnumAttribute.value()),
      m_predicate(standardsFilterObjectEnumAttribute.predicate())
  {
  }

  StandardsFilterObjectEnumAttributeRecord_Impl::StandardsFilterObjectEnumAttributeRecord_Impl(
      const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = standardsinterface::EnumAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_predicate = ruleset::RulesetStringPredicate(value.toString().toStdString());
  }

  openstudio::ruleset::StandardsFilterObjectEnumAttribute StandardsFilterObjectEnumAttributeRecord_Impl::standardsFilterObjectEnumAttribute() const
  {
    StandardsFilterAvailableObjectRecord standardsFilterAvailableObjectRecord = this->standardsFilterAvailableObjectRecord();
    openstudio::ruleset::StandardsFilterObjectEnumAttribute result(
        standardsFilterAvailableObjectRecord.standardsFilterAvailableObject(), 
        m_attributeName, 
        m_predicate, 
        m_attributeValue, 
        this->handle(), 
        this->uuidLast());
    return result;
  }

  openstudio::ruleset::StandardsFilterObjectAttribute StandardsFilterObjectEnumAttributeRecord_Impl::standardsFilterObjectAttribute() const
  {
    return this->standardsFilterObjectEnumAttribute();
  }

  openstudio::ruleset::FilterClause StandardsFilterObjectEnumAttributeRecord_Impl::filterClause() const
  {
    return this->standardsFilterObjectEnumAttribute();
  }

  boost::optional<ObjectRecord> StandardsFilterObjectEnumAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsFilterObjectEnumAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsFilterObjectEnumAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> StandardsFilterObjectEnumAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsFilterObjectEnumAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsFilterObjectEnumAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsFilterObjectEnumAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsFilterObjectAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName.valueName()));
    query.bindValue(ClauseRecordColumns::attributeValue, toQString(m_attributeValue));
    query.bindValue(ClauseRecordColumns::predicate, toQString(m_predicate.valueName()));
  }

  void StandardsFilterObjectEnumAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsFilterObjectAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = standardsinterface::EnumAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastPredicate = ruleset::RulesetStringPredicate(value.toString().toStdString());
  }

  bool StandardsFilterObjectEnumAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsFilterObjectAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == standardsinterface::EnumAttribute(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toString().toStdString());

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_predicate == ruleset::RulesetStringPredicate(value.toString().toStdString()));

    return result;
  }

  void StandardsFilterObjectEnumAttributeRecord_Impl::saveLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastAttributeValue = m_attributeValue;
    m_lastPredicate = m_predicate;
  }

  void StandardsFilterObjectEnumAttributeRecord_Impl::revertToLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_attributeValue = m_lastAttributeValue;
    m_predicate = m_lastPredicate;
  }

} // detail

StandardsFilterObjectEnumAttributeRecord::StandardsFilterObjectEnumAttributeRecord(
    const openstudio::ruleset::StandardsFilterObjectEnumAttribute& standardsFilterObjectEnumAttribute, 
    ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl>(
            new detail::StandardsFilterObjectEnumAttributeRecord_Impl(standardsFilterObjectEnumAttribute, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectEnumAttributeRecord_Impl>());
}

StandardsFilterObjectEnumAttributeRecord::StandardsFilterObjectEnumAttributeRecord(
    const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl>(
            new detail::StandardsFilterObjectEnumAttributeRecord_Impl(query, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectEnumAttributeRecord_Impl>());
}

StandardsFilterObjectEnumAttributeRecord::StandardsFilterObjectEnumAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl> impl, 
    ProjectDatabase projectDatabase)
  : StandardsFilterObjectAttributeRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectEnumAttributeRecord_Impl>());
}

/// @cond
StandardsFilterObjectEnumAttributeRecord::StandardsFilterObjectEnumAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl> impl)
  : StandardsFilterObjectAttributeRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectEnumAttributeRecord_Impl>());
}
/// @endcond

boost::optional<StandardsFilterObjectEnumAttributeRecord> StandardsFilterObjectEnumAttributeRecord::factoryFromQuery(
    const QSqlQuery& query,ProjectDatabase& database)
{
  OptionalStandardsFilterObjectEnumAttributeRecord result;
  try {
    result = StandardsFilterObjectEnumAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsFilterObjectEnumAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsFilterObjectEnumAttributeRecord> 
StandardsFilterObjectEnumAttributeRecord::getStandardsFilterObjectEnumAttributeRecords(ProjectDatabase& database) {
  std::vector<StandardsFilterObjectEnumAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectEnumAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsFilterObjectEnumAttributeRecord(query, database));
  }

  return result;
}

boost::optional<StandardsFilterObjectEnumAttributeRecord> StandardsFilterObjectEnumAttributeRecord::getStandardsFilterObjectEnumAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsFilterObjectEnumAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectEnumAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsFilterObjectEnumAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsFilterObjectEnumAttribute StandardsFilterObjectEnumAttributeRecord::standardsFilterObjectEnumAttribute() const
{
  return this->getImpl<detail::StandardsFilterObjectEnumAttributeRecord_Impl>()->standardsFilterObjectEnumAttribute();
}

} // project
} // openstudio
