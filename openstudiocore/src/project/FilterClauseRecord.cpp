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

#include <project/FilterClauseRecord.hpp>
#include <project/FilterClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelObjectFilterClauseRecord.hpp>

#include <ruleset/FilterClause.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    FilterClauseRecord_Impl::FilterClauseRecord_Impl(const openstudio::ruleset::FilterClause& filterClause, const FilterClauseRecordType& filterClauseRecordType, const ProjectDatabase& projectDatabase)
      : ClauseRecord_Impl(filterClause, ClauseRecordType::FilterClauseRecord, projectDatabase), m_filterClauseRecordType(filterClauseRecordType)
    {
    }

    FilterClauseRecord_Impl::FilterClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : ClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::filterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_filterClauseRecordType = FilterClauseRecordType(value.toInt());
    }

    void FilterClauseRecord_Impl::bindValues(QSqlQuery& query) const
    {
      ClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::filterClauseRecordType, m_filterClauseRecordType.value());
    }

    void FilterClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      ClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::filterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastFilterClauseRecordType = FilterClauseRecordType(value.toInt());
    }

    bool FilterClauseRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && ClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::filterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_filterClauseRecordType == FilterClauseRecordType(value.toInt()));

      return result;
    }

    void FilterClauseRecord_Impl::saveLastValues()
    {
      ClauseRecord_Impl::saveLastValues();

      m_lastFilterClauseRecordType = m_filterClauseRecordType;
    }

    void FilterClauseRecord_Impl::revertToLastValues()
    {
      ClauseRecord_Impl::revertToLastValues();

      m_filterClauseRecordType = m_lastFilterClauseRecordType;
    }

  } // detail

  FilterClauseRecord::FilterClauseRecord(boost::shared_ptr<detail::FilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
    : ClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::FilterClauseRecord_Impl>());
  }

  FilterClauseRecord::FilterClauseRecord(boost::shared_ptr<detail::FilterClauseRecord_Impl> impl)
    : ClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::FilterClauseRecord_Impl>());
  }

  std::vector<FilterClauseRecord> FilterClauseRecord::getFilterClauseRecords(ProjectDatabase& database)
  {
    std::vector<FilterClauseRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType"));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    assertExec(query);
    while (query.next()) {
      boost::optional<FilterClauseRecord> filterClauseRecord = factoryFromQuery(query, database);
      BOOST_ASSERT(filterClauseRecord);
      result.push_back(*filterClauseRecord);
    }

    return result;

  }

  boost::optional<FilterClauseRecord> FilterClauseRecord::getFilterClauseRecord(int id, ProjectDatabase& database)
  {
    boost::optional<FilterClauseRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE id=:id AND clauseRecordType=:clauseRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    assertExec(query);
    if (query.first()) {
      result = factoryFromQuery(query, database);
    }

    return result;
  }

  boost::optional<FilterClauseRecord> FilterClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<FilterClauseRecord> result;

    int filterClauseRecordType = query.value(ClauseRecordColumns::filterClauseRecordType).toInt();

    switch (filterClauseRecordType){
      case FilterClauseRecordType::ModelFilterClauseRecord:
        //result = ModelFilterClauseRecord::factoryFromQuery(query, database);
        break;
      case FilterClauseRecordType::ModelObjectFilterClauseRecord:
        result = ModelObjectFilterClauseRecord::factoryFromQuery(query, database);
        break;
      default:
        LOG(Error, "Unknown filterClauseRecordType " << filterClauseRecordType);
    }

    return result;
  }

  FilterClauseRecord FilterClauseRecord::factoryFromFilter(const openstudio::ruleset::FilterClause& filter, ProjectDatabase& projectDatabase)
  {
    if (filter.optionalCast<ruleset::ModelObjectFilterClause>()){
      return ModelObjectFilterClauseRecord::factoryFromFilter(filter.cast<ruleset::ModelObjectFilterClause>(), projectDatabase);
    }

    BOOST_ASSERT(false);
    return FilterClauseRecord(boost::shared_ptr<detail::FilterClauseRecord_Impl>());
  }

  openstudio::ruleset::FilterClause FilterClauseRecord::filterClause() const
  {
    return this->getImpl<detail::FilterClauseRecord_Impl>()->filterClause();
  }

} // project
} // openstudio
