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

#ifndef PROJECT_MODELOBJECTFILTERCLAUSERECORD_HPP
#define PROJECT_MODELOBJECTFILTERCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/FilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterClause;
}
namespace project {

namespace detail {
  class ModelObjectFilterClauseRecord_Impl;
}

/** \class ModelObjectFilterClauseRecordType 
 *
 *  \relates ModelObjectFilterClauseRecord */
OPENSTUDIO_ENUM( ModelObjectFilterClauseRecordType,
  ((ModelObjectFilterAttributeRecord)(ModelObjectFilterAttributeRecord)(0))
  ((ModelObjectFilterTypeRecord)(ModelObjectFilterTypeRecord)(1))
  ((ModelObjectFilterRelationshipRecord)(ModelObjectFilterRelationshipRecord)(2))
);

/** ModelObjectFilterClauseRecord
**/
class PROJECT_API ModelObjectFilterClauseRecord : public FilterClauseRecord {
 public:

  typedef detail::ModelObjectFilterClauseRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  virtual ~ModelObjectFilterClauseRecord() {}

  /// get all model object filter clauses 
  static std::vector<ModelObjectFilterClauseRecord> getModelObjectFilterClauseRecords(ProjectDatabase& database);
        
  /// get model object filter clause by id
  static boost::optional<ModelObjectFilterClauseRecord> getModelObjectFilterClauseRecord(int id, ProjectDatabase& database);

  /// get model object filter clause from a filter
  static ModelObjectFilterClauseRecord factoryFromFilter(const openstudio::ruleset::ModelObjectFilterClause& filter, ProjectDatabase& projectDatabase);

  /// get model object filter clause from a query
  static boost::optional<ModelObjectFilterClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterClauseRecord_Impl> impl);
  
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterClauseRecord");
};

/** \relates ModelObjectFilterClauseRecord */
typedef boost::optional<ModelObjectFilterClauseRecord> OptionalModelObjectFilterClauseRecord;

/** \relates ModelObjectFilterClauseRecord */
typedef std::vector<ModelObjectFilterClauseRecord> ModelObjectFilterClauseRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERCLAUSERECORD_HPP
