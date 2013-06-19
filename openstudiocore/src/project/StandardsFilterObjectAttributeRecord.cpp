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

#include <project/StandardsFilterObjectAttributeRecord.hpp>
#include <project/StandardsFilterObjectAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsFilterAvailableObjectRecord.hpp>
#include <project/StandardsFilterObjectBooleanAttributeRecord.hpp>
#include <project/StandardsFilterObjectDoubleAttributeRecord.hpp>
#include <project/StandardsFilterObjectEnumAttributeRecord.hpp>
#include <project/StandardsFilterObjectIntegerAttributeRecord.hpp>

#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    StandardsFilterObjectAttributeRecord_Impl::StandardsFilterObjectAttributeRecord_Impl(
        const openstudio::ruleset::StandardsFilterObjectAttribute& standardsFilterObjectAttribute, 
        const StandardsFilterObjectAttributeRecordType& standardsFilterObjectAttributeRecordType, 
        const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(
            standardsFilterObjectAttribute, 
            StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord, 
            projectDatabase), 
        m_standardsFilterObjectAttributeRecordType(standardsFilterObjectAttributeRecordType)
    {
      ProjectDatabase other = projectDatabase;
      StandardsFilterClauseRecord standardsFilterClauseRecord = StandardsFilterClauseRecord::factoryFromFilter(standardsFilterObjectAttribute.object(), other);
      m_standardsFilterAvailableObjectRecordId = standardsFilterClauseRecord.id();
    }

    StandardsFilterObjectAttributeRecord_Impl::StandardsFilterObjectAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : StandardsFilterClauseRecord_Impl(query, projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsFilterObjectAttributeRecordType = StandardsFilterObjectAttributeRecordType(value.toInt());
      
      value = query.value(ClauseRecordColumns::standardsFilterAvailableObjectId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_standardsFilterAvailableObjectRecordId = value.toInt();
    }

    StandardsFilterAvailableObjectRecord StandardsFilterObjectAttributeRecord_Impl::standardsFilterAvailableObjectRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<StandardsFilterAvailableObjectRecord> result = StandardsFilterAvailableObjectRecord::getStandardsFilterAvailableObjectRecord(m_standardsFilterAvailableObjectRecordId, projectDatabase);
      BOOST_ASSERT(result);
      return *result;
    }

    void StandardsFilterObjectAttributeRecord_Impl::bindValues(QSqlQuery& query) const
    {
      StandardsFilterClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::standardsFilterObjectAttributeRecordType, m_standardsFilterObjectAttributeRecordType.value());
      query.bindValue(ClauseRecordColumns::standardsFilterAvailableObjectId, m_standardsFilterAvailableObjectRecordId);
    }

    void StandardsFilterObjectAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      StandardsFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsFilterObjectAttributeRecordType = StandardsFilterObjectAttributeRecordType(value.toInt());

      value = query.value(ClauseRecordColumns::standardsFilterAvailableObjectId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastStandardsFilterAvailableObjectRecordId = value.toInt();
    }

    bool StandardsFilterObjectAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && StandardsFilterClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsFilterObjectAttributeRecordType == StandardsFilterObjectAttributeRecordType(value.toInt()));
      
      value = query.value(ClauseRecordColumns::standardsFilterAvailableObjectId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_standardsFilterAvailableObjectRecordId == value.toInt());

      return result;
    }

    void StandardsFilterObjectAttributeRecord_Impl::saveLastValues()
    {
      StandardsFilterClauseRecord_Impl::saveLastValues();

      m_lastStandardsFilterObjectAttributeRecordType = m_standardsFilterObjectAttributeRecordType; 
      m_lastStandardsFilterAvailableObjectRecordId = m_standardsFilterAvailableObjectRecordId; 
    }

    void StandardsFilterObjectAttributeRecord_Impl::revertToLastValues()
    {
      StandardsFilterClauseRecord_Impl::revertToLastValues();

      m_standardsFilterObjectAttributeRecordType = m_lastStandardsFilterObjectAttributeRecordType;
      m_standardsFilterAvailableObjectRecordId = m_lastStandardsFilterAvailableObjectRecordId;
    }

  } // detail

  StandardsFilterObjectAttributeRecord::StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
    : StandardsFilterClauseRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterObjectAttributeRecord_Impl>());
  }

  StandardsFilterObjectAttributeRecord::StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectAttributeRecord_Impl> impl)
    : StandardsFilterClauseRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsFilterObjectAttributeRecord_Impl>());
  }

  std::vector<StandardsFilterObjectAttributeRecord> StandardsFilterObjectAttributeRecord::getStandardsFilterObjectAttributeRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<StandardsFilterObjectAttributeRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND \
                             filterClauseRecordType=:filterClauseRecordType AND \
                             standardsFilterClauseRecordType=:standardsFilterClauseRecordType"));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
    query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
    assertExec(query);
    while (query.next()) {
      boost::optional<StandardsFilterObjectAttributeRecord> standardsFilterObjectAttributeRecord = StandardsFilterObjectAttributeRecord::factoryFromQuery(query, projectDatabase);
      BOOST_ASSERT(standardsFilterObjectAttributeRecord);
      result.push_back(*standardsFilterObjectAttributeRecord);
    }

    return result;
  }

  boost::optional<StandardsFilterObjectAttributeRecord> StandardsFilterObjectAttributeRecord::getStandardsFilterObjectAttributeRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<StandardsFilterObjectAttributeRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND \
                             filterClauseRecordType=:filterClauseRecordType AND \
                             standardsFilterClauseRecordType=:standardsFilterClauseRecordType AND id=:id"));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
    query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterObjectAttributeRecord);
    query.bindValue(":id", id);
    assertExec(query);
    if (query.first()) {
      result = StandardsFilterObjectAttributeRecord::factoryFromQuery(query, projectDatabase);
    }

    return result;
  }

  boost::optional<StandardsFilterObjectAttributeRecord> StandardsFilterObjectAttributeRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<StandardsFilterObjectAttributeRecord> result;

    int standardsFilterObjectAttributeRecordType = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordType).toInt();

    switch (standardsFilterObjectAttributeRecordType){
      case StandardsFilterObjectAttributeRecordType::StandardsFilterObjectBooleanAttributeRecord:
        result = StandardsFilterObjectBooleanAttributeRecord(query, database);
        break; 
      case StandardsFilterObjectAttributeRecordType::StandardsFilterObjectDoubleAttributeRecord:
        result = StandardsFilterObjectDoubleAttributeRecord(query, database);
        break;
      case StandardsFilterObjectAttributeRecordType::StandardsFilterObjectEnumAttributeRecord:
        result = StandardsFilterObjectEnumAttributeRecord(query, database);
        break;
      case StandardsFilterObjectAttributeRecordType::StandardsFilterObjectIntegerAttributeRecord:
        result = StandardsFilterObjectIntegerAttributeRecord(query, database);
        break;
      default:
        LOG(Error, "Unknown standardsFilterObjectAttributeRecordType " << standardsFilterObjectAttributeRecordType);
    }

    return result;
  }

  StandardsFilterObjectAttributeRecord StandardsFilterObjectAttributeRecord::factoryFromFilter(const openstudio::ruleset::StandardsFilterObjectAttribute& filter, ProjectDatabase& projectDatabase)
  {

    if (filter.optionalCast<ruleset::StandardsFilterObjectBooleanAttribute>()){
      return StandardsFilterObjectBooleanAttributeRecord(filter.cast<ruleset::StandardsFilterObjectBooleanAttribute>(), projectDatabase);
    }else if(filter.optionalCast<ruleset::StandardsFilterObjectDoubleAttribute>()){
      return StandardsFilterObjectDoubleAttributeRecord(filter.cast<ruleset::StandardsFilterObjectDoubleAttribute>(), projectDatabase);
    }else if(filter.optionalCast<ruleset::StandardsFilterObjectEnumAttribute>()){
      return StandardsFilterObjectEnumAttributeRecord(filter.cast<ruleset::StandardsFilterObjectEnumAttribute>(), projectDatabase);
    }else if(filter.optionalCast<ruleset::StandardsFilterObjectIntegerAttribute>()){
      return StandardsFilterObjectIntegerAttributeRecord(filter.cast<ruleset::StandardsFilterObjectIntegerAttribute>(), projectDatabase);
    }

    BOOST_ASSERT(false);
    return StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectAttributeRecord_Impl>());
  }
 
  openstudio::ruleset::StandardsFilterObjectAttribute StandardsFilterObjectAttributeRecord::standardsFilterObjectAttribute() const
  {
    return this->getImpl<detail::StandardsFilterObjectAttributeRecord_Impl>()->standardsFilterObjectAttribute();
  }

  StandardsFilterAvailableObjectRecord StandardsFilterObjectAttributeRecord::standardsFilterAvailableObjectRecord() const
  {
    return this->getImpl<detail::StandardsFilterObjectAttributeRecord_Impl>()->standardsFilterAvailableObjectRecord();
  }


} // project
} // openstudio
