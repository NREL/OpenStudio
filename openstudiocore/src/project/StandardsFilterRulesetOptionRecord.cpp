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

#include <project/StandardsFilterRulesetOptionRecord.hpp>
#include <project/StandardsFilterRulesetOptionRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>

#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsRulesetOption.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsFilterRulesetOptionRecord_Impl::StandardsFilterRulesetOptionRecord_Impl(
        const openstudio::ruleset::StandardsFilterRulesetOption& standardsFilterRulesetOption, 
        const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(
            standardsFilterRulesetOption, 
            StandardsFilterClauseRecordType::StandardsFilterRulesetOptionRecord, 
            projectDatabase)
    {
      ProjectDatabase other = projectDatabase;
      StandardsRulesetOptionRecord standardsRulesetOptionRecord = 
          StandardsRulesetOptionRecord::factoryFromOption(standardsFilterRulesetOption.option(), other);
      m_standardsRulesetOptionId = standardsRulesetOptionRecord.id();
    } 

    StandardsFilterRulesetOptionRecord_Impl::StandardsFilterRulesetOptionRecord_Impl(
        const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsRulesetOptionId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsRulesetOptionId = value.toInt();
    }

    openstudio::ruleset::StandardsFilterRulesetOption StandardsFilterRulesetOptionRecord_Impl::standardsFilterRulesetOption() const
    {
      StandardsRulesetOptionRecord standardsRulesetOptionRecord = this->standardsRulesetOptionRecord();
      openstudio::ruleset::StandardsFilterRulesetOption result(standardsRulesetOptionRecord.standardsRulesetOption(), this->handle(), this->uuidLast());
      return result;
    }

    openstudio::ruleset::FilterClause StandardsFilterRulesetOptionRecord_Impl::filterClause() const
    {
      return this->standardsFilterRulesetOption();
    }

    StandardsRulesetOptionRecord StandardsFilterRulesetOptionRecord_Impl::standardsRulesetOptionRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<StandardsRulesetOptionRecord> result = StandardsRulesetOptionRecord::getStandardsRulesetOptionRecord(m_standardsRulesetOptionId, projectDatabase);
      if (!result) {
        std::cout << "My standardsRulesetOptionId = " << m_standardsRulesetOptionId << ".";
      }
      BOOST_ASSERT(result);
      return *result;
    }

    boost::optional<ObjectRecord> StandardsFilterRulesetOptionRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> StandardsFilterRulesetOptionRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> StandardsFilterRulesetOptionRecord_Impl::resources() const
    {
      std::vector<ObjectRecord> result;
      result.push_back(this->standardsRulesetOptionRecord());
      return result;
    }

    std::vector<JoinRecord> StandardsFilterRulesetOptionRecord_Impl::joinRecords() const
    {
      std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
      return std::vector<JoinRecord>(result.begin(), result.end());
    }

    void StandardsFilterRulesetOptionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<StandardsFilterRulesetOptionRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void StandardsFilterRulesetOptionRecord_Impl::bindValues(QSqlQuery& query) const
    {
      StandardsFilterClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::standardsRulesetOptionId, m_standardsRulesetOptionId);
    }

    void StandardsFilterRulesetOptionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      StandardsFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsRulesetOptionId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsRulesetOptionId = value.toInt();
    }

    bool StandardsFilterRulesetOptionRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && StandardsFilterClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsRulesetOptionId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsRulesetOptionId == value.toInt());

      return result;
    }

    void StandardsFilterRulesetOptionRecord_Impl::saveLastValues()
    {
      StandardsFilterClauseRecord_Impl::saveLastValues();

      m_lastStandardsRulesetOptionId = m_standardsRulesetOptionId;  
    }

    void StandardsFilterRulesetOptionRecord_Impl::revertToLastValues()
    {
      StandardsFilterClauseRecord_Impl::revertToLastValues();

      m_standardsRulesetOptionId = m_lastStandardsRulesetOptionId; 
    }

  } // detail

  StandardsFilterRulesetOptionRecord::StandardsFilterRulesetOptionRecord(const openstudio::ruleset::StandardsFilterRulesetOption& standardsFilterRulesetOption, ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl>(new detail::StandardsFilterRulesetOptionRecord_Impl(standardsFilterRulesetOption, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>());
  }

  StandardsFilterRulesetOptionRecord::StandardsFilterRulesetOptionRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl>(new detail::StandardsFilterRulesetOptionRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>());
  }

  StandardsFilterRulesetOptionRecord::StandardsFilterRulesetOptionRecord(boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase)
    : StandardsFilterClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>());
  }

  StandardsFilterRulesetOptionRecord::StandardsFilterRulesetOptionRecord(boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl> impl)
    : StandardsFilterClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>());
  }

  boost::optional<StandardsFilterRulesetOptionRecord> StandardsFilterRulesetOptionRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database)
  {
    OptionalStandardsFilterRulesetOptionRecord result;
    try {
      result = StandardsFilterRulesetOptionRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct StandardsFilterRulesetOptionRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  openstudio::ruleset::StandardsFilterRulesetOption StandardsFilterRulesetOptionRecord::standardsFilterRulesetOption() const
  {
    return this->getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>()->standardsFilterRulesetOption();
  }

  StandardsRulesetOptionRecord StandardsFilterRulesetOptionRecord::standardsRulesetOptionRecord() const
  {
    return this->getImpl<detail::StandardsFilterRulesetOptionRecord_Impl>()->standardsRulesetOptionRecord();
  }

} // project
} // openstudio
