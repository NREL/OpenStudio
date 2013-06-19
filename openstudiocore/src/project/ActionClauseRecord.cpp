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

#include <project/ActionClauseRecord.hpp>
#include <project/ActionClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelObjectActionClauseRecord.hpp>
#include <project/StandardsActionClauseRecord.hpp>

#include <ruleset/ActionClause.hpp>
#include <ruleset/ModelObjectActionClause.hpp>
#include <ruleset/ModelObjectActionClause_Impl.hpp>
#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    ActionClauseRecord_Impl::ActionClauseRecord_Impl(const openstudio::ruleset::ActionClause& actionClause, const ActionClauseRecordType& actionClauseRecordType, const ProjectDatabase& projectDatabase)
      : ClauseRecord_Impl(actionClause, ClauseRecordType::ActionClauseRecord, projectDatabase), m_actionClauseRecordType(actionClauseRecordType)
    {
    }

    ActionClauseRecord_Impl::ActionClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : ClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::actionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_actionClauseRecordType = ActionClauseRecordType(value.toInt());
    }

    void ActionClauseRecord_Impl::bindValues(QSqlQuery& query) const
    {
      ClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::actionClauseRecordType, m_actionClauseRecordType.value());
    }

    void ActionClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      ClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::actionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastActionClauseRecordType = ActionClauseRecordType(value.toInt());
    }

    bool ActionClauseRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && ClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::actionClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_actionClauseRecordType == ActionClauseRecordType(value.toInt()));

      return result;
    }

    void ActionClauseRecord_Impl::saveLastValues()
    {
      ClauseRecord_Impl::saveLastValues();

      m_lastActionClauseRecordType = m_actionClauseRecordType;  
    }

    void ActionClauseRecord_Impl::revertToLastValues()
    {
      ClauseRecord_Impl::revertToLastValues();

      m_actionClauseRecordType = m_lastActionClauseRecordType;
    }

  } // detail

  ActionClauseRecord::ActionClauseRecord(boost::shared_ptr<detail::ActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
    : ClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::ActionClauseRecord_Impl>());
  }

  ActionClauseRecord::ActionClauseRecord(boost::shared_ptr<detail::ActionClauseRecord_Impl> impl)
    : ClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::ActionClauseRecord_Impl>());
  }
  
  std::vector<ActionClauseRecord> ActionClauseRecord::getActionClauseRecords(ProjectDatabase& database)
  {
    std::vector<ActionClauseRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType"));
    query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
    assertExec(query);
    while (query.next()) {
      boost::optional<ActionClauseRecord> actionClauseRecord = factoryFromQuery(query, database);
      BOOST_ASSERT(actionClauseRecord);
      result.push_back(*actionClauseRecord);
    }

    return result;

  }

  boost::optional<ActionClauseRecord> ActionClauseRecord::getActionClauseRecord(int id, ProjectDatabase& database)
  {
    boost::optional<ActionClauseRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE id=:id AND clauseRecordType=:clauseRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
    assertExec(query);
    if (query.first()) {
      result = factoryFromQuery(query, database);
    }

    return result;
  }

  boost::optional<ActionClauseRecord> ActionClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<ActionClauseRecord> result;

    int actionClauseRecordType = query.value(ClauseRecordColumns::actionClauseRecordType).toInt();

    switch (actionClauseRecordType){
      case ActionClauseRecordType::ModelObjectActionClauseRecord:
        result = ModelObjectActionClauseRecord::factoryFromQuery(query, database);
        break;
      case ActionClauseRecordType::StandardsActionClauseRecord:
        result = StandardsActionClauseRecord::factoryFromQuery(query, database);
        break;
      default:
        LOG(Error, "Unknown actionClauseRecordType " << actionClauseRecordType);
    }

    return result;
  }

  ActionClauseRecord ActionClauseRecord::factoryFromAction(const openstudio::ruleset::ActionClause& action, ProjectDatabase& projectDatabase)
  {
    if (action.optionalCast<ruleset::ModelObjectActionClause>()){
      return ModelObjectActionClauseRecord::factoryFromAction(action.cast<ruleset::ModelObjectActionClause>(), projectDatabase);
    }else if (action.optionalCast<ruleset::StandardsActionClause>()){
      return StandardsActionClauseRecord::factoryFromAction(action.cast<ruleset::StandardsActionClause>(), projectDatabase);
    }

    BOOST_ASSERT(false);
    return ActionClauseRecord(boost::shared_ptr<detail::ActionClauseRecord_Impl>());
  }
      
  openstudio::ruleset::ActionClause ActionClauseRecord::actionClause() const
  {
    return this->getImpl<detail::ActionClauseRecord_Impl>()->actionClause();
  }

} // project
} // openstudio
