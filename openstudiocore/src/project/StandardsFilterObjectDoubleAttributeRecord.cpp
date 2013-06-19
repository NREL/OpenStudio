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

#include <project/StandardsFilterObjectDoubleAttributeRecord.hpp>
#include <project/StandardsFilterObjectDoubleAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>

#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
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

  StandardsFilterObjectDoubleAttributeRecord_Impl::StandardsFilterObjectDoubleAttributeRecord_Impl(
      const openstudio::ruleset::StandardsFilterObjectDoubleAttribute& standardsFilterObjectDoubleAttribute, 
      const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(standardsFilterObjectDoubleAttribute, StandardsFilterObjectAttributeRecordType::StandardsFilterObjectDoubleAttributeRecord, projectDatabase), 
      m_attributeName(standardsFilterObjectDoubleAttribute.attribute()), 
      m_attributeValue(standardsFilterObjectDoubleAttribute.value()),
      m_predicate(standardsFilterObjectDoubleAttribute.predicate()),
      m_attributeUnits(standardsFilterObjectDoubleAttribute.units())
  {
  }

  StandardsFilterObjectDoubleAttributeRecord_Impl::StandardsFilterObjectDoubleAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = standardsinterface::DoubleAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toDouble();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_predicate = ruleset::RulesetNumericalPredicate(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeUnits = value.toString().toStdString();
  }

  openstudio::ruleset::StandardsFilterObjectDoubleAttribute StandardsFilterObjectDoubleAttributeRecord_Impl::standardsFilterObjectDoubleAttribute() const
  {
    StandardsFilterAvailableObjectRecord standardsFilterAvailableObjectRecord = this->standardsFilterAvailableObjectRecord();
    openstudio::ruleset::StandardsFilterObjectDoubleAttribute result(standardsFilterAvailableObjectRecord.standardsFilterAvailableObject(), 
                                                                     m_attributeName, 
                                                                     m_predicate, 
                                                                     m_attributeValue, 
                                                                     m_attributeUnits,
                                                                     this->handle(), 
                                                                     this->uuidLast());
    return result;
  }

  openstudio::ruleset::StandardsFilterObjectAttribute StandardsFilterObjectDoubleAttributeRecord_Impl::standardsFilterObjectAttribute() const
  {
    return this->standardsFilterObjectDoubleAttribute();
  }

  openstudio::ruleset::FilterClause StandardsFilterObjectDoubleAttributeRecord_Impl::filterClause() const
  {
    return this->standardsFilterObjectDoubleAttribute();
  }

  boost::optional<ObjectRecord> StandardsFilterObjectDoubleAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsFilterObjectDoubleAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsFilterObjectDoubleAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> StandardsFilterObjectDoubleAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsFilterObjectDoubleAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsFilterObjectDoubleAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsFilterObjectDoubleAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsFilterObjectAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName.valueName()));
    query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue);
    query.bindValue(ClauseRecordColumns::predicate, toQString(m_predicate.valueName()));
    query.bindValue(ClauseRecordColumns::attributeUnits, toQString(m_attributeUnits));
  }

  void StandardsFilterObjectDoubleAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsFilterObjectAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = standardsinterface::DoubleAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toString().toDouble();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastPredicate = ruleset::RulesetNumericalPredicate(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeUnits = value.toString().toStdString();
  }

  bool StandardsFilterObjectDoubleAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsFilterObjectAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == standardsinterface::DoubleAttribute(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toString().toDouble());

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_predicate == ruleset::RulesetNumericalPredicate(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeUnits == value.toString().toStdString());

    return result;
  }

  void StandardsFilterObjectDoubleAttributeRecord_Impl::saveLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastAttributeValue = m_attributeValue;
    m_lastPredicate = m_predicate;
    m_lastAttributeUnits = m_attributeUnits;
  }

  void StandardsFilterObjectDoubleAttributeRecord_Impl::revertToLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_attributeValue = m_lastAttributeValue;
    m_predicate = m_lastPredicate;
    m_attributeUnits = m_lastAttributeUnits;
  }

} // detail

StandardsFilterObjectDoubleAttributeRecord::StandardsFilterObjectDoubleAttributeRecord(
    const openstudio::ruleset::StandardsFilterObjectDoubleAttribute& standardsFilterObjectDoubleAttribute, 
    ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>(
            new detail::StandardsFilterObjectDoubleAttributeRecord_Impl(standardsFilterObjectDoubleAttribute, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>());
}

StandardsFilterObjectDoubleAttributeRecord::StandardsFilterObjectDoubleAttributeRecord(
    const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>(
            new detail::StandardsFilterObjectDoubleAttributeRecord_Impl(query, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>());
}

StandardsFilterObjectDoubleAttributeRecord::StandardsFilterObjectDoubleAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl> impl, 
    ProjectDatabase projectDatabase)
  : StandardsFilterObjectAttributeRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>());
}

/// @cond
StandardsFilterObjectDoubleAttributeRecord::StandardsFilterObjectDoubleAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl> impl)
  : StandardsFilterObjectAttributeRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>());
}
/// @endcond

boost::optional<StandardsFilterObjectDoubleAttributeRecord> StandardsFilterObjectDoubleAttributeRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsFilterObjectDoubleAttributeRecord result;
  try {
    result = StandardsFilterObjectDoubleAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsFilterObjectDoubleAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsFilterObjectDoubleAttributeRecord> StandardsFilterObjectDoubleAttributeRecord::getStandardsFilterObjectDoubleAttributeRecords(ProjectDatabase& database) {
  std::vector<StandardsFilterObjectDoubleAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectDoubleAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsFilterObjectDoubleAttributeRecord(query, database));
  }

  return result;
}

boost::optional<StandardsFilterObjectDoubleAttributeRecord> StandardsFilterObjectDoubleAttributeRecord::getStandardsFilterObjectDoubleAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsFilterObjectDoubleAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectDoubleAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsFilterObjectDoubleAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsFilterObjectDoubleAttribute StandardsFilterObjectDoubleAttributeRecord::standardsFilterObjectDoubleAttribute() const
{
  return this->getImpl<detail::StandardsFilterObjectDoubleAttributeRecord_Impl>()->standardsFilterObjectDoubleAttribute();
}

} // project
} // openstudio
