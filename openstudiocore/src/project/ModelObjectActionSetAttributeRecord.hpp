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

#ifndef PROJECT_MODELOBJECTACTIONSETATTRIBUTERECORD_HPP
#define PROJECT_MODELOBJECTACTIONSETATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectActionSetAttribute;
}
namespace project {

namespace detail {
  class ModelObjectActionSetAttributeRecord_Impl;
}

/** ModelObjectActionSetAttributeRecord
**/
class PROJECT_API ModelObjectActionSetAttributeRecord : public ModelObjectActionClauseRecord {
 public:

  typedef detail::ModelObjectActionSetAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectActionSetAttributeRecord(
      const openstudio::ruleset::ModelObjectActionSetAttribute& modelObjectActionSetAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectActionSetAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectActionSetAttributeRecord() {}

  static boost::optional<ModelObjectActionSetAttributeRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// get all model records in database
  static std::vector<ModelObjectActionSetAttributeRecord> getModelObjectActionSetAttributeRecords(
      ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<ModelObjectActionSetAttributeRecord> 
  getModelObjectActionSetAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the action from the record
  openstudio::ruleset::ModelObjectActionSetAttribute modelObjectActionSetAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectActionSetAttributeRecord_Impl;

  // from impl
  ModelObjectActionSetAttributeRecord(
      boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);
  
  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectActionSetAttributeRecord(boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectActionSetAttributeRecord");
};

/** \relates ModelObjectActionSetAttributeRecord*/
typedef boost::optional<ModelObjectActionSetAttributeRecord> OptionalModelObjectActionSetAttributeRecord;

/** \relates ModelObjectActionSetAttributeRecord*/
typedef std::vector<ModelObjectActionSetAttributeRecord> ModelObjectActionSetAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTACTIONSETATTRIBUTERECORD_HPP
