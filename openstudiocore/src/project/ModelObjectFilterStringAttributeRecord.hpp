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

#ifndef PROJECT_MODELOBJECTFILTERSTRINGATTRIBUTERECORD_HPP
#define PROJECT_MODELOBJECTFILTERSTRINGATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectFilterAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterStringAttribute;
}
namespace project {

namespace detail {
  class ModelObjectFilterStringAttributeRecord_Impl;
}

/** ModelObjectFilterStringAttributeRecord
**/
class PROJECT_API ModelObjectFilterStringAttributeRecord : public ModelObjectFilterAttributeRecord {
 public:

  typedef detail::ModelObjectFilterStringAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectFilterStringAttributeRecord(
      const openstudio::ruleset::ModelObjectFilterStringAttribute& modelObjectFilterStringAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectFilterStringAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectFilterStringAttributeRecord() {}

  static boost::optional<ModelObjectFilterStringAttributeRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// get all records in database
  static std::vector<ModelObjectFilterStringAttributeRecord> 
  getModelObjectFilterStringAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<ModelObjectFilterStringAttributeRecord> 
  getModelObjectFilterStringAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::ModelObjectFilterStringAttribute modelObjectFilterStringAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectFilterStringAttributeRecord_Impl;

  // from impl
  ModelObjectFilterStringAttributeRecord(
      boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectFilterStringAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterStringAttributeRecord");
};

/** \relates ModelObjectFilterStringAttributeRecord*/
typedef boost::optional<ModelObjectFilterStringAttributeRecord> OptionalModelObjectFilterStringAttributeRecord;

/** \relates ModelObjectFilterStringAttributeRecord*/
typedef std::vector<ModelObjectFilterStringAttributeRecord> ModelObjectFilterStringAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERSTRINGATTRIBUTERECORD_HPP
