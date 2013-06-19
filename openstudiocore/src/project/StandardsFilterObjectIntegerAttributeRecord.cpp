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

#include <project/StandardsFilterObjectIntegerAttributeRecord.hpp>
#include <project/StandardsFilterObjectIntegerAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>

#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
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

  StandardsFilterObjectIntegerAttributeRecord_Impl::StandardsFilterObjectIntegerAttributeRecord_Impl(
      const openstudio::ruleset::StandardsFilterObjectIntegerAttribute& standardsFilterObjectIntegerAttribute, 
      const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(standardsFilterObjectIntegerAttribute, 
                                                StandardsFilterObjectAttributeRecordType::StandardsFilterObjectIntegerAttributeRecord, 
                                                projectDatabase), 
      m_attributeName(standardsFilterObjectIntegerAttribute.attribute()), 
      m_attributeValue(standardsFilterObjectIntegerAttribute.value()),
      m_predicate(standardsFilterObjectIntegerAttribute.predicate()),
      m_attributeUnits(standardsFilterObjectIntegerAttribute.units())
  {
  }

  StandardsFilterObjectIntegerAttributeRecord_Impl::StandardsFilterObjectIntegerAttributeRecord_Impl(
      const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsFilterObjectAttributeRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = standardsinterface::IntegerAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toInt();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_predicate = ruleset::RulesetNumericalPredicate(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeUnits = value.toString().toStdString();
  }

  openstudio::ruleset::StandardsFilterObjectIntegerAttribute StandardsFilterObjectIntegerAttributeRecord_Impl::standardsFilterObjectIntegerAttribute() const
  {
    StandardsFilterAvailableObjectRecord standardsFilterAvailableObjectRecord = this->standardsFilterAvailableObjectRecord();
    openstudio::ruleset::StandardsFilterObjectIntegerAttribute result(standardsFilterAvailableObjectRecord.standardsFilterAvailableObject(), 
                                                                      m_attributeName, 
                                                                      m_predicate, 
                                                                      m_attributeValue, 
                                                                      m_attributeUnits,
                                                                      this->handle(), 
                                                                      this->uuidLast());
    return result;
  }

  openstudio::ruleset::StandardsFilterObjectAttribute StandardsFilterObjectIntegerAttributeRecord_Impl::standardsFilterObjectAttribute() const
  {
    return this->standardsFilterObjectIntegerAttribute();
  }

  openstudio::ruleset::FilterClause StandardsFilterObjectIntegerAttributeRecord_Impl::filterClause() const
  {
    return this->standardsFilterObjectIntegerAttribute();
  }

  boost::optional<ObjectRecord> StandardsFilterObjectIntegerAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsFilterObjectIntegerAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsFilterObjectIntegerAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> StandardsFilterObjectIntegerAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsFilterObjectIntegerAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsFilterObjectIntegerAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsFilterObjectIntegerAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsFilterObjectAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName.valueName()));
    query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue);
    query.bindValue(ClauseRecordColumns::predicate, toQString(m_predicate.valueName()));
    query.bindValue(ClauseRecordColumns::attributeUnits, toQString(m_attributeUnits));
  }

  void StandardsFilterObjectIntegerAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsFilterObjectAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = standardsinterface::IntegerAttribute(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toString().toInt();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastPredicate = ruleset::RulesetNumericalPredicate(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeUnits = value.toString().toStdString();
  }

  bool StandardsFilterObjectIntegerAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsFilterObjectAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == standardsinterface::IntegerAttribute(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toString().toInt());

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_predicate == ruleset::RulesetNumericalPredicate(value.toString().toStdString()));

    value = query.value(ClauseRecordColumns::attributeUnits);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeUnits == value.toString().toStdString());

    return result;
  }

  void StandardsFilterObjectIntegerAttributeRecord_Impl::saveLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastAttributeValue = m_attributeValue;
    m_lastPredicate = m_predicate;
    m_lastAttributeUnits = m_attributeUnits;
  }

  void StandardsFilterObjectIntegerAttributeRecord_Impl::revertToLastValues()
  {
    StandardsFilterObjectAttributeRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_attributeValue = m_lastAttributeValue;
    m_predicate = m_lastPredicate;
    m_attributeUnits = m_lastAttributeUnits;
  }

} // detail

StandardsFilterObjectIntegerAttributeRecord::StandardsFilterObjectIntegerAttributeRecord(
    const openstudio::ruleset::StandardsFilterObjectIntegerAttribute& standardsFilterObjectIntegerAttribute, 
    ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>(
            new detail::StandardsFilterObjectIntegerAttributeRecord_Impl(standardsFilterObjectIntegerAttribute, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>());
}

StandardsFilterObjectIntegerAttributeRecord::StandardsFilterObjectIntegerAttributeRecord(
    const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsFilterObjectAttributeRecord(
        boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>(
            new detail::StandardsFilterObjectIntegerAttributeRecord_Impl(query, projectDatabase)), 
            projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>());
}

StandardsFilterObjectIntegerAttributeRecord::StandardsFilterObjectIntegerAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl> impl, 
    ProjectDatabase projectDatabase)
  : StandardsFilterObjectAttributeRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>());
}

/// @cond
StandardsFilterObjectIntegerAttributeRecord::StandardsFilterObjectIntegerAttributeRecord(
    boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl> impl)
  : StandardsFilterObjectAttributeRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>());
}
/// @endcond

boost::optional<StandardsFilterObjectIntegerAttributeRecord> StandardsFilterObjectIntegerAttributeRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsFilterObjectIntegerAttributeRecord result;
  try {
    result = StandardsFilterObjectIntegerAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsFilterObjectIntegerAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsFilterObjectIntegerAttributeRecord> StandardsFilterObjectIntegerAttributeRecord::getStandardsFilterObjectIntegerAttributeRecords(ProjectDatabase& database) {
  std::vector<StandardsFilterObjectIntegerAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectIntegerAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsFilterObjectIntegerAttributeRecord(query, database));
  }

  return result;
}

boost::optional<StandardsFilterObjectIntegerAttributeRecord> StandardsFilterObjectIntegerAttributeRecord::getStandardsFilterObjectIntegerAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsFilterObjectIntegerAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"
      " AND standardsFilterObjectAttributeRecordType=:standardsFilterObjectAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType",StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
  query.bindValue(":standardsFilterObjectAttributeRecordType", StandardsFilterObjectAttributeRecordType::StandardsFilterObjectIntegerAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsFilterObjectIntegerAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsFilterObjectIntegerAttribute StandardsFilterObjectIntegerAttributeRecord::standardsFilterObjectIntegerAttribute() const
{
  return this->getImpl<detail::StandardsFilterObjectIntegerAttributeRecord_Impl>()->standardsFilterObjectIntegerAttribute();
}

} // project
} // openstudio
