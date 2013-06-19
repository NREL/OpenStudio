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

#ifndef PROJECT_MODELOBJECTFILTERRELATIONSHIPRECORD_HPP
#define PROJECT_MODELOBJECTFILTERRELATIONSHIPRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/ModelObjectFilterClauseRecord.hpp>

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterRelationship;
}
namespace project {

namespace detail {

  class ModelObjectFilterRelationshipRecord_Impl;

} // detail

/** ModelObjectFilterRelationshipRecord is a ModelObjectFilterClauseRecord. */
class PROJECT_API ModelObjectFilterRelationshipRecord : public ModelObjectFilterClauseRecord {
 public:
  typedef detail::ModelObjectFilterRelationshipRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  ModelObjectFilterRelationshipRecord(
      const ruleset::ModelObjectFilterRelationship& modelObjectFilterRelationship, 
      ProjectDatabase& database);

  ModelObjectFilterRelationshipRecord(const QSqlQuery& query, ProjectDatabase& database);

  /** Construct from impl. */
  ModelObjectFilterRelationshipRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl> impl,
                                      ProjectDatabase database);

  virtual ~ModelObjectFilterRelationshipRecord() {}

  //@}

  static boost::optional<ModelObjectFilterRelationshipRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<ModelObjectFilterRelationshipRecord> getModelObjectFilterRelationshipRecords(ProjectDatabase& database);

  static boost::optional<ModelObjectFilterRelationshipRecord> getModelObjectFilterRelationshipRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  std::string relationshipName() const;

  ruleset::ModelObjectFilterRelationship modelObjectFilterRelationship() const;

  //@}
 protected:
  /// @cond
  explicit ModelObjectFilterRelationshipRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl> impl);

  friend class detail::ModelObjectFilterRelationshipRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.ModelObjectFilterRelationshipRecord");
};

/** \relates ModelObjectFilterRelationshipRecord*/
typedef boost::optional<ModelObjectFilterRelationshipRecord> OptionalModelObjectFilterRelationshipRecord;

/** \relates ModelObjectFilterRelationshipRecord*/
typedef std::vector<ModelObjectFilterRelationshipRecord> ModelObjectFilterRelationshipRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERRELATIONSHIPRECORD_HPP

