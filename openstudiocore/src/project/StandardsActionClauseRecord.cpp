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

#include <project/StandardsActionClauseRecord.hpp>
#include <project/StandardsActionClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsActionCloneModelRecord.hpp>
#include <project/StandardsActionRuleResultRecord.hpp>

#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionCloneModel.hpp>
#include <ruleset/StandardsActionCloneModel_Impl.hpp>
#include <ruleset/StandardsActionRuleResult.hpp>
#include <ruleset/StandardsActionRuleResult_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsActionClauseRecord_Impl::StandardsActionClauseRecord_Impl(const openstudio::ruleset::StandardsActionClause& standardsActionClause, const StandardsActionClauseRecordType& StandardsActionClauseRecordType, const ProjectDatabase& projectDatabase)
      : ActionClauseRecord_Impl(standardsActionClause, ActionClauseRecordType::StandardsActionClauseRecord, projectDatabase), m_standardsActionClauseRecordType(StandardsActionClauseRecordType)
    {
    }

    StandardsActionClauseRecord_Impl::StandardsActionClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : ActionClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsActionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsActionClauseRecordType = StandardsActionClauseRecordType(value.toInt());
    }

    void StandardsActionClauseRecord_Impl::bindValues(QSqlQuery& query) const
    {
      ActionClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::standardsActionClauseRecordType, m_standardsActionClauseRecordType.value());
    }

    void StandardsActionClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      ActionClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsActionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsActionClauseRecordType = StandardsActionClauseRecordType(value.toInt());
    }

    bool StandardsActionClauseRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && ActionClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsActionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsActionClauseRecordType == StandardsActionClauseRecordType(value.toInt()));

      return result;
    }

    void StandardsActionClauseRecord_Impl::saveLastValues()
    {
      ActionClauseRecord_Impl::saveLastValues();

      m_lastStandardsActionClauseRecordType = m_standardsActionClauseRecordType;
    }

    void StandardsActionClauseRecord_Impl::revertToLastValues()
    {
      ActionClauseRecord_Impl::revertToLastValues();

      m_standardsActionClauseRecordType = m_lastStandardsActionClauseRecordType;
    }

  } // detail

  StandardsActionClauseRecord::StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
    : ActionClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsActionClauseRecord_Impl>());
  }

  StandardsActionClauseRecord::StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionClauseRecord_Impl> impl)
    : ActionClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsActionClauseRecord_Impl>());
  }

  StandardsActionClauseRecord StandardsActionClauseRecord::factoryFromAction(const ruleset::StandardsActionClause& action, ProjectDatabase& projectDatabase)
  {
    if (action.optionalCast<ruleset::StandardsActionCloneModel>()){
      return StandardsActionCloneModelRecord(action.cast<ruleset::StandardsActionCloneModel>(), projectDatabase);
    }else if(action.optionalCast<ruleset::StandardsActionRuleResult>()){
      return StandardsActionRuleResultRecord::factoryFromAction(action.cast<ruleset::StandardsActionRuleResult>(), projectDatabase);
    }

    BOOST_ASSERT(false);
    return StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionClauseRecord_Impl>());
  }

  boost::optional<StandardsActionClauseRecord> StandardsActionClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<StandardsActionClauseRecord> result;

    int standardsActionClauseRecordType = query.value(ClauseRecordColumns::standardsActionClauseRecordType).toInt();

    switch (standardsActionClauseRecordType){
      case StandardsActionClauseRecordType::StandardsActionCloneModelRecord:
        result = StandardsActionCloneModelRecord(query, database);
        break;
      case StandardsActionClauseRecordType::StandardsActionRuleResultRecord:
        result = StandardsActionRuleResultRecord::factoryFromQuery(query, database);
        break;
      default:
        LOG(Error, "Unknown standardsActionClauseRecordType " << standardsActionClauseRecordType);
    }

    return result;
  }

} // project
} // openstudio
