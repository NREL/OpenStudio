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

#include <project/StandardsEnumRulesetOptionRecord.hpp>
#include <project/StandardsEnumRulesetOptionRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsRulesetRecord.hpp>

#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsEnumRulesetOptionRecord_Impl::StandardsEnumRulesetOptionRecord_Impl(
        const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, 
        const StandardsRulesetRecord& standardsRulesetRecord)
      : StandardsRulesetOptionRecord_Impl(standardsEnumRulesetOption, 
                                          StandardsRulesetOptionRecordType::StandardsEnumRulesetOptionRecord, 
                                          standardsRulesetRecord),
        m_optionEnum(standardsEnumRulesetOption.enumOption()), 
        m_defaultOptionValue(standardsEnumRulesetOption.defaultValue()), 
        m_optionValue(standardsEnumRulesetOption.value())
    {
    }

    StandardsEnumRulesetOptionRecord_Impl::StandardsEnumRulesetOptionRecord_Impl(const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, const ProjectDatabase& projectDatabase)
      : StandardsRulesetOptionRecord_Impl(standardsEnumRulesetOption, StandardsRulesetOptionRecordType::StandardsEnumRulesetOptionRecord, projectDatabase),
        m_optionEnum(standardsEnumRulesetOption.enumOption()), m_defaultOptionValue(standardsEnumRulesetOption.defaultValue()), m_optionValue(standardsEnumRulesetOption.value())
    {
    }

    StandardsEnumRulesetOptionRecord_Impl::StandardsEnumRulesetOptionRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : StandardsRulesetOptionRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::optionEnum);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_optionEnum = standardsinterface::EnumAttribute(value.toInt());

      value = query.value(RulesetOptionRecordColumns::defaultOptionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_defaultOptionValue = value.toString().toStdString();

      value = query.value(RulesetOptionRecordColumns::optionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_optionValue = value.toString().toStdString();
    }

    openstudio::ruleset::StandardsEnumRulesetOption StandardsEnumRulesetOptionRecord_Impl::standardsEnumRulesetOption() const
    {
      openstudio::ruleset::StandardsEnumRulesetOption result(m_optionEnum, m_defaultOptionValue, m_optionValue, this->handle(), this->uuidLast());
      return result;
    }

    openstudio::ruleset::StandardsRulesetOption StandardsEnumRulesetOptionRecord_Impl::standardsRulesetOption() const
    {
      return this->standardsEnumRulesetOption();
    }

    boost::optional<ObjectRecord> StandardsEnumRulesetOptionRecord_Impl::parent() const
    {
      boost::optional<ObjectRecord> result;
      boost::optional<RulesetRecord> rulesetRecord = this->rulesetRecord();
      if (rulesetRecord){
        result = *rulesetRecord;
      }
      return result;
    }

    std::vector<ObjectRecord> StandardsEnumRulesetOptionRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> StandardsEnumRulesetOptionRecord_Impl::resources() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<JoinRecord> StandardsEnumRulesetOptionRecord_Impl::joinRecords() const
    {
      return std::vector<JoinRecord>();
    }

    void StandardsEnumRulesetOptionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<StandardsEnumRulesetOptionRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void StandardsEnumRulesetOptionRecord_Impl::bindValues(QSqlQuery& query) const
    {
      StandardsRulesetOptionRecord_Impl::bindValues(query);

      query.bindValue(RulesetOptionRecordColumns::optionEnum, m_optionEnum.value());
      query.bindValue(RulesetOptionRecordColumns::defaultOptionValue, toQString(m_defaultOptionValue));
      query.bindValue(RulesetOptionRecordColumns::optionValue, toQString(m_optionValue));
    }

    void StandardsEnumRulesetOptionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      StandardsRulesetOptionRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::optionEnum);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastOptionEnum = standardsinterface::EnumAttribute(value.toInt());

      value = query.value(RulesetOptionRecordColumns::defaultOptionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastDefaultOptionValue = value.toString().toStdString();

      value = query.value(RulesetOptionRecordColumns::optionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastOptionValue = value.toString().toStdString();
    }

    bool StandardsEnumRulesetOptionRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && StandardsRulesetOptionRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(RulesetOptionRecordColumns::optionEnum);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_optionEnum == standardsinterface::EnumAttribute(value.toInt()));

      value = query.value(RulesetOptionRecordColumns::defaultOptionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_defaultOptionValue == value.toString().toStdString());

      value = query.value(RulesetOptionRecordColumns::optionValue);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_optionValue == value.toString().toStdString());

      return result;
    }

    void StandardsEnumRulesetOptionRecord_Impl::saveLastValues()
    {
      StandardsRulesetOptionRecord_Impl::saveLastValues();

      m_lastOptionEnum = m_optionEnum;  
      m_lastDefaultOptionValue = m_defaultOptionValue;
      m_lastOptionValue = m_optionValue;
    }

    void StandardsEnumRulesetOptionRecord_Impl::revertToLastValues()
    {
      StandardsRulesetOptionRecord_Impl::revertToLastValues();

      m_optionEnum = m_lastOptionEnum; 
      m_defaultOptionValue = m_lastDefaultOptionValue;
      m_optionValue = m_lastOptionValue;
    }

  } // detail

  StandardsEnumRulesetOptionRecord::StandardsEnumRulesetOptionRecord(const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, const StandardsRulesetRecord& standardsRulesetRecord)
    : StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl>(new detail::StandardsEnumRulesetOptionRecord_Impl(standardsEnumRulesetOption, standardsRulesetRecord)), standardsRulesetRecord.projectDatabase())
  {
    BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>());
  }

  StandardsEnumRulesetOptionRecord::StandardsEnumRulesetOptionRecord(const openstudio::ruleset::StandardsEnumRulesetOption& standardsEnumRulesetOption, ProjectDatabase& projectDatabase)
    : StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl>(new detail::StandardsEnumRulesetOptionRecord_Impl(standardsEnumRulesetOption, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>());
  }

  StandardsEnumRulesetOptionRecord::StandardsEnumRulesetOptionRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl>(new detail::StandardsEnumRulesetOptionRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>());
  }

  StandardsEnumRulesetOptionRecord::StandardsEnumRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase)
    : StandardsRulesetOptionRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>());
  }

  StandardsEnumRulesetOptionRecord::StandardsEnumRulesetOptionRecord(boost::shared_ptr<detail::StandardsEnumRulesetOptionRecord_Impl> impl)
    : StandardsRulesetOptionRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>());
  }

  boost::optional<StandardsEnumRulesetOptionRecord> StandardsEnumRulesetOptionRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database)
  {
    OptionalStandardsEnumRulesetOptionRecord result;
    try {
      result = StandardsEnumRulesetOptionRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct StandardsEnumRulesetOptionRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<StandardsEnumRulesetOptionRecord> StandardsEnumRulesetOptionRecord::getStandardsEnumRulesetOptionRecords(ProjectDatabase& database)
  {
    std::vector<StandardsEnumRulesetOptionRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE \
                             rulesetOptionRecordType=:rulesetOptionRecordType AND \
                             standardsRulesetOptionRecordType=:standardsRulesetOptionRecordType"));
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    query.bindValue(":standardsRulesetOptionRecordType", StandardsRulesetOptionRecordType::StandardsEnumRulesetOptionRecord);
    assertExec(query);
    while (query.next()) {
      StandardsEnumRulesetOptionRecord standardsEnumRulesetOptionRecord(query, database);
      result.push_back(standardsEnumRulesetOptionRecord);
    }

    return result;

  }

  boost::optional<StandardsEnumRulesetOptionRecord> StandardsEnumRulesetOptionRecord::getStandardsEnumRulesetOptionRecord(int id, ProjectDatabase& database)
  {
    boost::optional<StandardsEnumRulesetOptionRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE id=:id AND \
                             rulesetOptionRecordType=:rulesetOptionRecordType AND \
                             standardsRulesetOptionRecordType=:standardsRulesetOptionRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
    query.bindValue(":standardsRulesetOptionRecordType", StandardsRulesetOptionRecordType::StandardsEnumRulesetOptionRecord);
    assertExec(query);
    if (query.first()) {
      result = StandardsEnumRulesetOptionRecord(query, database);
    }

    return result;
  }

  openstudio::ruleset::StandardsEnumRulesetOption StandardsEnumRulesetOptionRecord::standardsEnumRulesetOption() const
  {
    return this->getImpl<detail::StandardsEnumRulesetOptionRecord_Impl>()->standardsEnumRulesetOption();
  }

} // project
} // openstudio
