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

#include <project/StandardsFilterClauseRecord.hpp>
#include <project/StandardsFilterClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsFilterAvailableModelRecord.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>
#include <project/StandardsFilterRulesetOptionRecord.hpp>
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <ruleset/StandardsFilterClause.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsFilterClauseRecord_Impl::StandardsFilterClauseRecord_Impl(const openstudio::ruleset::StandardsFilterClause& standardsFilterClause, const StandardsFilterClauseRecordType& StandardsFilterClauseRecordType, const ProjectDatabase& projectDatabase)
      : FilterClauseRecord_Impl(standardsFilterClause, FilterClauseRecordType::StandardsFilterClauseRecord, projectDatabase), m_standardsFilterClauseRecordType(StandardsFilterClauseRecordType)
    {
    }

    StandardsFilterClauseRecord_Impl::StandardsFilterClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : FilterClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsFilterClauseRecordType = StandardsFilterClauseRecordType(value.toInt());
    }

    void StandardsFilterClauseRecord_Impl::bindValues(QSqlQuery& query) const
    {
      FilterClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::standardsFilterClauseRecordType, m_standardsFilterClauseRecordType.value());
    }

    void StandardsFilterClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      FilterClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsFilterClauseRecordType = StandardsFilterClauseRecordType(value.toInt());
    }

    bool StandardsFilterClauseRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && FilterClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterClauseRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsFilterClauseRecordType == StandardsFilterClauseRecordType(value.toInt()));

      return result;
    }

    void StandardsFilterClauseRecord_Impl::saveLastValues()
    {
      FilterClauseRecord_Impl::saveLastValues();

      m_lastStandardsFilterClauseRecordType = m_standardsFilterClauseRecordType; 
    }

    void StandardsFilterClauseRecord_Impl::revertToLastValues()
    {
      FilterClauseRecord_Impl::revertToLastValues();

      m_standardsFilterClauseRecordType = m_lastStandardsFilterClauseRecordType;
    }

  } // detail

  StandardsFilterClauseRecord::StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
    : FilterClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterClauseRecord_Impl>());
  }

  StandardsFilterClauseRecord::StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterClauseRecord_Impl> impl)
    : FilterClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterClauseRecord_Impl>());
  }

  StandardsFilterClauseRecord StandardsFilterClauseRecord::factoryFromFilter(const openstudio::ruleset::StandardsFilterClause& filter, ProjectDatabase& projectDatabase)
  {
    if (filter.optionalCast<ruleset::StandardsFilterObjectAttribute>()){
      return StandardsFilterObjectAttributeRecord::factoryFromFilter(filter.cast<ruleset::StandardsFilterObjectAttribute>(), projectDatabase);
    }else if (filter.optionalCast<ruleset::StandardsFilterAvailableObject>()){
      return StandardsFilterAvailableObjectRecord(filter.cast<ruleset::StandardsFilterAvailableObject>(), projectDatabase);
    }else if (filter.optionalCast<ruleset::StandardsFilterAvailableModel>()){
      return StandardsFilterAvailableModelRecord(filter.cast<ruleset::StandardsFilterAvailableModel>(), projectDatabase);
    }else if (filter.optionalCast<ruleset::StandardsFilterRulesetOption>()){
      return StandardsFilterRulesetOptionRecord(filter.cast<ruleset::StandardsFilterRulesetOption>(), projectDatabase);
    }

    BOOST_ASSERT(false);
    return StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterClauseRecord_Impl>());
  }

  boost::optional<StandardsFilterClauseRecord> StandardsFilterClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<StandardsFilterClauseRecord> result;

    int standardsFilterClauseRecordType = query.value(ClauseRecordColumns::standardsFilterClauseRecordType).toInt();

    switch (standardsFilterClauseRecordType){
      case StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord:
        result = StandardsFilterObjectAttributeRecord::factoryFromQuery(query, database);
        break;  
      case StandardsFilterClauseRecordType::StandardsFilterAvailableObjectRecord:
        result = StandardsFilterAvailableObjectRecord(query, database);
        break;  
      case StandardsFilterClauseRecordType::StandardsFilterAvailableModelRecord:
        result = StandardsFilterAvailableModelRecord(query, database);
        break;     
      case StandardsFilterClauseRecordType::StandardsFilterRulesetOptionRecord:
        result = StandardsFilterRulesetOptionRecord(query, database);
        break;  
      default:
        LOG(Error, "Unknown standardsFilterClauseRecordType " << standardsFilterClauseRecordType);
    }

    return result;
  }

} // project
} // openstudio
