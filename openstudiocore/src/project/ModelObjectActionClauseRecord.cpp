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

#include <project/ModelObjectActionClauseRecord.hpp>
#include <project/ModelObjectActionClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelObjectActionSetAttributeRecord.hpp>
#include <project/ModelObjectActionSetRelationshipRecord.hpp>

#include <ruleset/ModelObjectActionClause.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetAttribute_Impl.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>
#include <ruleset/ModelObjectActionSetRelationship_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    ModelObjectActionClauseRecord_Impl::ModelObjectActionClauseRecord_Impl(const openstudio::ruleset::ModelObjectActionClause& modelObjectActionClause, const ModelObjectActionClauseRecordType& ModelObjectActionClauseRecordType, const ProjectDatabase& projectDatabase)
      : ActionClauseRecord_Impl(modelObjectActionClause, ActionClauseRecordType::ModelObjectActionClauseRecord, projectDatabase), m_modelObjectActionClauseRecordType(ModelObjectActionClauseRecordType)
    {
    }

    ModelObjectActionClauseRecord_Impl::ModelObjectActionClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : ActionClauseRecord_Impl(query, projectDatabase)
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      QVariant value;
      value = query.value(ClauseRecordColumns::modelObjectActionClauseRecordType);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_modelObjectActionClauseRecordType = ModelObjectActionClauseRecordType(value.toInt());
    }

    void ModelObjectActionClauseRecord_Impl::bindValues(QSqlQuery& query) const
    {
      ActionClauseRecord_Impl::bindValues(query);

      query.bindValue(ClauseRecordColumns::modelObjectActionClauseRecordType, m_modelObjectActionClauseRecordType.value());
    }

    void ModelObjectActionClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      OS_ASSERT(query.isValid());
      OS_ASSERT(query.isActive());
      OS_ASSERT(query.isSelect());

      ActionClauseRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(ClauseRecordColumns::modelObjectActionClauseRecordType);
      OS_ASSERT(value.isValid() && !value.isNull());
      m_lastModelObjectActionClauseRecordType = ModelObjectActionClauseRecordType(value.toInt());
    }

    bool ModelObjectActionClauseRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && ActionClauseRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(ClauseRecordColumns::modelObjectActionClauseRecordType);
      OS_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_modelObjectActionClauseRecordType == ModelObjectActionClauseRecordType(value.toInt()));

      return result;
    }

    void ModelObjectActionClauseRecord_Impl::saveLastValues()
    {
      ActionClauseRecord_Impl::saveLastValues();

      m_lastModelObjectActionClauseRecordType = m_modelObjectActionClauseRecordType;  
    }

    void ModelObjectActionClauseRecord_Impl::revertToLastValues()
    {
      ActionClauseRecord_Impl::revertToLastValues();

      m_modelObjectActionClauseRecordType = m_lastModelObjectActionClauseRecordType;
    }

  } // detail

  ModelObjectActionClauseRecord::ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
    : ActionClauseRecord(impl, projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionClauseRecord_Impl>());
  }

  ModelObjectActionClauseRecord::ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionClauseRecord_Impl> impl)
    : ActionClauseRecord(impl)
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionClauseRecord_Impl>());
  }

  boost::optional<ModelObjectActionClauseRecord> ModelObjectActionClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<ModelObjectActionClauseRecord> result;

    int modelObjectActionClauseRecordType = query.value(ClauseRecordColumns::modelObjectActionClauseRecordType).toInt();

    switch (modelObjectActionClauseRecordType){
      case ModelObjectActionClauseRecordType::ModelObjectActionSetAttributeRecord:
        result = ModelObjectActionSetAttributeRecord(query, database);
        break;
      case ModelObjectActionClauseRecordType::ModelObjectActionSetRelationshipRecord:
        result = ModelObjectActionSetRelationshipRecord(query, database);
        break;
      default:
        LOG(Error, "Unknown modelObjectActionClauseRecordType " << modelObjectActionClauseRecordType);
    }

    return result;
  }
  
  ModelObjectActionClauseRecord ModelObjectActionClauseRecord::factoryFromAction(const openstudio::ruleset::ModelObjectActionClause& action, ProjectDatabase& projectDatabase)
  {
    if (action.optionalCast<ruleset::ModelObjectActionSetAttribute>()){
      return ModelObjectActionSetAttributeRecord(action.cast<ruleset::ModelObjectActionSetAttribute>(), projectDatabase);
    }else if (action.optionalCast<ruleset::ModelObjectActionSetRelationship>()){
      return ModelObjectActionSetRelationshipRecord(action.cast<ruleset::ModelObjectActionSetRelationship>(), projectDatabase);
    }

    OS_ASSERT(false);
    return ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionClauseRecord_Impl>());
  }
      
} // project
} // openstudio
