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

#ifndef PROJECT_MODELOBJECTFILTERNUMERICATTRIBUTERECORD_HPP
#define PROJECT_MODELOBJECTFILTERNUMERICATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectFilterAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterNumericAttribute;
}
namespace project {

namespace detail {
  class ModelObjectFilterNumericAttributeRecord_Impl;
}

/** ModelObjectFilterNumericAttributeRecord
**/
class PROJECT_API ModelObjectFilterNumericAttributeRecord : public ModelObjectFilterAttributeRecord {
 public:

  typedef detail::ModelObjectFilterNumericAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectFilterNumericAttributeRecord(
      const openstudio::ruleset::ModelObjectFilterNumericAttribute& modelObjectFilterNumericAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectFilterNumericAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectFilterNumericAttributeRecord() {}

  static boost::optional<ModelObjectFilterNumericAttributeRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// get all records in database
  static std::vector<ModelObjectFilterNumericAttributeRecord> 
  getModelObjectFilterNumericAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<ModelObjectFilterNumericAttributeRecord> 
  getModelObjectFilterNumericAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::ModelObjectFilterNumericAttribute modelObjectFilterNumericAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectFilterNumericAttributeRecord_Impl;

  // from impl
  ModelObjectFilterNumericAttributeRecord(
      boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectFilterNumericAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterNumericAttributeRecord");
};

/** \relates ModelObjectFilterNumericAttributeRecord*/
typedef boost::optional<ModelObjectFilterNumericAttributeRecord> OptionalModelObjectFilterNumericAttributeRecord;

/** \relates ModelObjectFilterNumericAttributeRecord*/
typedef std::vector<ModelObjectFilterNumericAttributeRecord> ModelObjectFilterNumericAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERNUMERICATTRIBUTERECORD_HPP
