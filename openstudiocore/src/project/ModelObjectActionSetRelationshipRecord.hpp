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

#ifndef PROJECT_MODELOBJECTACTIONSETRELATIONSHIPRECORD_HPP
#define PROJECT_MODELOBJECTACTIONSETRELATIONSHIPRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectActionSetRelationship;
}
namespace project {

class FileReferenceRecord;

namespace detail {
  class ModelObjectActionSetRelationshipRecord_Impl;
}

/** ModelObjectActionSetRelationshipRecord 
**/
class PROJECT_API ModelObjectActionSetRelationshipRecord : public ModelObjectActionClauseRecord {
 public:

  typedef detail::ModelObjectActionSetRelationshipRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectActionSetRelationshipRecord(
      const openstudio::ruleset::ModelObjectActionSetRelationship& modelObjectActionSetRelationship, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectActionSetRelationshipRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectActionSetRelationshipRecord() {}

  static boost::optional<ModelObjectActionSetRelationshipRecord> factoryFromQuery(
      const QSqlQuery& query,
      ProjectDatabase& database);

  /// get all model records in database
  static std::vector<ModelObjectActionSetRelationshipRecord> 
  getModelObjectActionSetRelationshipRecords(ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<ModelObjectActionSetRelationshipRecord> 
  getModelObjectActionSetRelationshipRecord(int id, ProjectDatabase& projectDatabase);

  /// get the component record
  FileReferenceRecord componentRecord() const;

  /// get the action from the record
  openstudio::ruleset::ModelObjectActionSetRelationship modelObjectActionSetRelationship() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectActionSetRelationshipRecord_Impl;

  // from impl
  ModelObjectActionSetRelationshipRecord(
      boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl> impl,
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectActionSetRelationshipRecord(
      boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectActionSetRelationshipRecord");
};

/** \relates ModelObjectActionSetRelationshipRecord*/
typedef boost::optional<ModelObjectActionSetRelationshipRecord> OptionalModelObjectActionSetRelationshipRecord;

/** \relates ModelObjectActionSetRelationshipRecord*/
typedef std::vector<ModelObjectActionSetRelationshipRecord> ModelObjectActionSetRelationshipRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTACTIONSETRELATIONSHIPRECORD_HPP
