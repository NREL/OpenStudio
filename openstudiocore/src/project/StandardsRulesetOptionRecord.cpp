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

#include <project/StandardsRulesetOptionRecord.hpp>
#include <project/StandardsRulesetOptionRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsRulesetRecord.hpp>
#include <project/StandardsEnumRulesetOptionRecord.hpp>

#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsRulesetOptionRecord_Impl::StandardsRulesetOptionRecord_Impl(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, const StandardsRulesetOptionRecordType& StandardsRulesetOptionRecordType, const StandardsRulesetRecord& standardsRulesetRecord)
      : RulesetOptionRecord_Impl(standardsRulesetOption, RulesetOptionRecordType::StandardsRulesetOptionRecord, standardsRulesetRecord), m_standardsRulesetOptionRecordType(StandardsRulesetOptionRecordType)
    {
    }

    StandardsRulesetOptionRecord_Impl::StandardsRulesetOptionRecord_Impl(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, const StandardsRulesetOptionRecordType& StandardsRulesetOptionRecordType, const ProjectDatabase& projectDatabase)
      : RulesetOptionRecord_Impl(standardsRulesetOption, RulesetOptionRecordType::StandardsRulesetOptionRecord, projectDatabase), m_standardsRulesetOptionRecordType(StandardsRulesetOptionRecordType)
    {
    }

    StandardsRulesetOptionRecord_Impl::StandardsRulesetOptionRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : RulesetOptionRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::standardsRulesetOptionRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsRulesetOptionRecordType = StandardsRulesetOptionRecordType(value.toInt());
    }

    void StandardsRulesetOptionRecord_Impl::bindValues(QSqlQuery& query) const
    {
      RulesetOptionRecord_Impl::bindValues(query);

      query.bindValue(RulesetOptionRecordColumns::standardsRulesetOptionRecordType, m_standardsRulesetOptionRecordType.value());
    }

    void StandardsRulesetOptionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      RulesetOptionRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::standardsRulesetOptionRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsRulesetOptionRecordType = StandardsRulesetOptionRecordType(value.toInt());
    }

    bool StandardsRulesetOptionRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && RulesetOptionRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::standardsRulesetOptionRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsRulesetOptionRecordType == StandardsRulesetOptionRecordType(value.toInt()));

      return result;
    }

    void StandardsRulesetOptionRecord_Impl::saveLastValues()
    {
      RulesetOptionRecord_Impl::saveLastValues();

      m_lastStandardsRulesetOptionRecordType = m_standardsRulesetOptionRecordType; 
    }

    void StandardsRulesetOptionRecord_Impl::revertToLastValues()
    {
      RulesetOptionRecord_Impl::revertToLastValues();

      m_standardsRulesetOptionRecordType = m_lastStandardsRulesetOptionRecordType;
    }

  } // detail

  StandardsRulesetOptionRecord::StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RulesetOptionRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetOptionRecord_Impl>());
  }

  StandardsRulesetOptionRecord::StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl> impl)
    : RulesetOptionRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetOptionRecord_Impl>());
  }

  std::vector<StandardsRulesetOptionRecord> StandardsRulesetOptionRecord::getStandardsRulesetOptionRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<StandardsRulesetOptionRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE rulesetOptionRecordType=:rulesetOptionRecordType"));
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    assertExec(query);
    while (query.next()) {
      boost::optional<StandardsRulesetOptionRecord> standardsRulesetOptionRecord = factoryFromQuery(query, projectDatabase);
      BOOST_ASSERT(standardsRulesetOptionRecord);
      result.push_back(*standardsRulesetOptionRecord);
    }

    return result;
  }
  
  boost::optional<StandardsRulesetOptionRecord> StandardsRulesetOptionRecord::getStandardsRulesetOptionRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<StandardsRulesetOptionRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE id=:id AND \
                             rulesetOptionRecordType=:rulesetOptionRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    assertExec(query);
    if (query.first()) {
      result = StandardsRulesetOptionRecord::factoryFromQuery(query, projectDatabase);
    }

    return result;
  }

  StandardsRulesetOptionRecord StandardsRulesetOptionRecord::factoryFromOption(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, const StandardsRulesetRecord& standardsRulesetRecord)
  {
    ProjectDatabase projectDatabase = standardsRulesetRecord.projectDatabase();

    // first try a query
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE handle=:handle AND\
                             rulesetOptionRecordType=:rulesetOptionRecordType"));
    query.bindValue(":handle", toQString(toString(standardsRulesetOption.uuid())));
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    assertExec(query);
    if (query.first()) {
      boost::optional<StandardsRulesetOptionRecord> result = StandardsRulesetOptionRecord::factoryFromQuery(query, projectDatabase);
      BOOST_ASSERT(result);
      return *result;
    }

    // if not found then create new one
    if (standardsRulesetOption.optionalCast<ruleset::StandardsEnumRulesetOption>()){
      return StandardsEnumRulesetOptionRecord(standardsRulesetOption.cast<ruleset::StandardsEnumRulesetOption>(), standardsRulesetRecord);
    }

    BOOST_ASSERT(false);
    return StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl>());
  }

  StandardsRulesetOptionRecord StandardsRulesetOptionRecord::factoryFromOption(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, ProjectDatabase& database)
  {
    // first try a query
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE handle=:handle AND\
                             rulesetOptionRecordType=:rulesetOptionRecordType"));
    query.bindValue(":handle", toQString(toString(standardsRulesetOption.uuid())));
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    assertExec(query);
    if (query.first()) {
      boost::optional<StandardsRulesetOptionRecord> result = StandardsRulesetOptionRecord::factoryFromQuery(query, database);
      BOOST_ASSERT(result);
      return *result;
    }

    // if not found then create new one
    if (standardsRulesetOption.optionalCast<ruleset::StandardsEnumRulesetOption>()){
      return StandardsEnumRulesetOptionRecord(standardsRulesetOption.cast<ruleset::StandardsEnumRulesetOption>(), database);
    }

    BOOST_ASSERT(false);
    return StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl>());
  }

  boost::optional<StandardsRulesetOptionRecord> StandardsRulesetOptionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<StandardsRulesetOptionRecord> result;

    int standardsRulesetOptionRecordType = query.value(RulesetOptionRecordColumns::standardsRulesetOptionRecordType).toInt();

    switch (standardsRulesetOptionRecordType){
      case StandardsRulesetOptionRecordType::StandardsEnumRulesetOptionRecord:
        result = StandardsEnumRulesetOptionRecord(query, database);
        break;     
      default:
        LOG(Error, "Unknown standardsRulesetOptionRecordType " << standardsRulesetOptionRecordType);
    }

    return result;
  }

  openstudio::ruleset::StandardsRulesetOption StandardsRulesetOptionRecord::standardsRulesetOption() const
  {
    return this->getImpl<detail::StandardsRulesetOptionRecord_Impl>()->standardsRulesetOption();
  }


} // project
} // openstudio
