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

#include <project/StandardsFilterAvailableModelRecord.hpp>
#include <project/StandardsFilterAvailableModelRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsFilterAvailableModelRecord_Impl::StandardsFilterAvailableModelRecord_Impl(const openstudio::ruleset::StandardsFilterAvailableModel& standardsFilterAvailableModel, const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(standardsFilterAvailableModel, StandardsFilterClauseRecordType::StandardsFilterAvailableModelRecord, projectDatabase), 
        m_standardsFilterAvailableModelId(standardsFilterAvailableModel.id()), m_standardsRulesetModelType(standardsFilterAvailableModel.modelType())
    {
    }

    StandardsFilterAvailableModelRecord_Impl::StandardsFilterAvailableModelRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
      if(value.isValid() && !value.isNull()){
        m_standardsFilterAvailableModelId = value.toString().toStdString();
      }

      value = query.value(ClauseRecordColumns::standardsRulesetModelType);
      if(value.isValid() && !value.isNull()){
        m_standardsRulesetModelType = ruleset::StandardsRulesetModelType(value.toString().toStdString());
      }
    }

    openstudio::ruleset::StandardsFilterAvailableModel StandardsFilterAvailableModelRecord_Impl::standardsFilterAvailableModel() const
    {
      if (m_standardsRulesetModelType){
        return openstudio::ruleset::StandardsFilterAvailableModel(*m_standardsRulesetModelType, this->handle(), this->uuidLast());
      }

      openstudio::ruleset::StandardsFilterAvailableModel result(m_standardsFilterAvailableModelId, this->handle(), this->uuidLast());
      return result;
    }

    openstudio::ruleset::FilterClause StandardsFilterAvailableModelRecord_Impl::filterClause() const
    {
      return this->standardsFilterAvailableModel();
    }

    boost::optional<ObjectRecord> StandardsFilterAvailableModelRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> StandardsFilterAvailableModelRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> StandardsFilterAvailableModelRecord_Impl::resources() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<JoinRecord> StandardsFilterAvailableModelRecord_Impl::joinRecords() const
    {
      std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
      return std::vector<JoinRecord>(result.begin(), result.end());
    }

    void StandardsFilterAvailableModelRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<StandardsFilterAvailableModelRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void StandardsFilterAvailableModelRecord_Impl::bindValues(QSqlQuery& query) const
    {
      StandardsFilterClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelId, toQString(m_standardsFilterAvailableModelId));

      if(m_standardsRulesetModelType){
        query.bindValue(ClauseRecordColumns::standardsRulesetModelType, toQString(m_standardsRulesetModelType->valueName()));
      }else{
        query.bindValue(ClauseRecordColumns::standardsRulesetModelType, QVariant(QVariant::String));
      }
    }

    void StandardsFilterAvailableModelRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      StandardsFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
      if(value.isValid() && !value.isNull()){
        m_lastStandardsFilterAvailableModelId = value.toString().toStdString();
      }else{
        m_lastStandardsFilterAvailableModelId.clear();
      }

      value = query.value(ClauseRecordColumns::standardsRulesetModelType);
      if(value.isValid() && !value.isNull()){
        m_lastStandardsRulesetModelType = openstudio::ruleset::StandardsRulesetModelType(value.toString().toStdString());
      }else{
        m_lastStandardsRulesetModelType.reset();
      }
    }

    bool StandardsFilterAvailableModelRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && StandardsFilterClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
      if(value.isValid() && !value.isNull()){
        result = result && (m_standardsFilterAvailableModelId == value.toString().toStdString());
      }else{
        result = result && m_standardsFilterAvailableModelId.empty();
      }

      value = query.value(ClauseRecordColumns::standardsRulesetModelType);
      if(value.isValid() && !value.isNull()){
        result = result && m_standardsRulesetModelType && (m_standardsRulesetModelType == openstudio::ruleset::StandardsRulesetModelType(value.toString().toStdString()));
      }else{
        result = result && !m_standardsRulesetModelType;
      }

      return result;
    }

    void StandardsFilterAvailableModelRecord_Impl::saveLastValues()
    {
      StandardsFilterClauseRecord_Impl::saveLastValues();

      m_lastStandardsFilterAvailableModelId = m_standardsFilterAvailableModelId;  
      m_lastStandardsRulesetModelType = m_standardsRulesetModelType;
    }

    void StandardsFilterAvailableModelRecord_Impl::revertToLastValues()
    {
      StandardsFilterClauseRecord_Impl::revertToLastValues();

      m_standardsFilterAvailableModelId = m_lastStandardsFilterAvailableModelId; 
      m_standardsRulesetModelType = m_lastStandardsRulesetModelType;
    }

  } // detail

  StandardsFilterAvailableModelRecord::StandardsFilterAvailableModelRecord(const openstudio::ruleset::StandardsFilterAvailableModel& standardsFilterAvailableModel, ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl>(new detail::StandardsFilterAvailableModelRecord_Impl(standardsFilterAvailableModel, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModelRecord_Impl>());
  }

  StandardsFilterAvailableModelRecord::StandardsFilterAvailableModelRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl>(new detail::StandardsFilterAvailableModelRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModelRecord_Impl>());
  }

  StandardsFilterAvailableModelRecord::StandardsFilterAvailableModelRecord(boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl> impl, ProjectDatabase projectDatabase)
    : StandardsFilterClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModelRecord_Impl>());
  }

  StandardsFilterAvailableModelRecord::StandardsFilterAvailableModelRecord(boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl> impl)
    : StandardsFilterClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModelRecord_Impl>());
  }

  boost::optional<StandardsFilterAvailableModelRecord> StandardsFilterAvailableModelRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database)
  {
    OptionalStandardsFilterAvailableModelRecord result;
    try {
      result = StandardsFilterAvailableModelRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct StandardsFilterAvailableModelRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<StandardsFilterAvailableModelRecord> StandardsFilterAvailableModelRecord::getStandardsFilterAvailableModelRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<StandardsFilterAvailableModelRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND \
                             filterClauseRecordType=:filterClauseRecordType AND \
                             standardsFilterClauseRecordType=:standardsFilterClauseRecordType"));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
    query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterAvailableModelRecord);
    assertExec(query);
    while (query.next()) {
      result.push_back(projectDatabase.getFromQuery<StandardsFilterAvailableModelRecord>(query));
    }

    return result;
  }

  boost::optional<StandardsFilterAvailableModelRecord> StandardsFilterAvailableModelRecord::getStandardsFilterAvailableModelRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<StandardsFilterAvailableModelRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND \
                             filterClauseRecordType=:filterClauseRecordType AND \
                             standardsFilterClauseRecordType=:standardsFilterClauseRecordType AND id=:id"));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
    query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterAvailableModelRecord);
    query.bindValue(":id", id);
    assertExec(query);
    if (query.first()) {
      result = projectDatabase.getFromQuery<StandardsFilterAvailableModelRecord>(query);
    }

    return result;
  }

  openstudio::ruleset::StandardsFilterAvailableModel StandardsFilterAvailableModelRecord::standardsFilterAvailableModel() const
  {
    return this->getImpl<detail::StandardsFilterAvailableModelRecord_Impl>()->standardsFilterAvailableModel();
  }

} // project
} // openstudio
