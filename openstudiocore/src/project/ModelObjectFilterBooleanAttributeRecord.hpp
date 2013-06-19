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

#ifndef PROJECT_MODELOBJECTFILTERBOOLEANATTRIBUTERECORD_HPP
#define PROJECT_MODELOBJECTFILTERBOOLEANATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectFilterAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterBooleanAttribute;
}
namespace project {

namespace detail {
  class ModelObjectFilterBooleanAttributeRecord_Impl;
}

/** ModelObjectFilterBooleanAttributeRecord 
**/
class PROJECT_API ModelObjectFilterBooleanAttributeRecord : public ModelObjectFilterAttributeRecord {
 public:

  typedef detail::ModelObjectFilterBooleanAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectFilterBooleanAttributeRecord(
      const openstudio::ruleset::ModelObjectFilterBooleanAttribute& modelObjectFilterBooleanAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectFilterBooleanAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectFilterBooleanAttributeRecord() {}

  static boost::optional<ModelObjectFilterBooleanAttributeRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);
  
  /// get all records in database
  static std::vector<ModelObjectFilterBooleanAttributeRecord> 
  getModelObjectFilterBooleanAttributeRecords(ProjectDatabase& projectDatabase);
  
  /// get record in database
  static boost::optional<ModelObjectFilterBooleanAttributeRecord> 
  getModelObjectFilterBooleanAttributeRecord(int id, ProjectDatabase& projectDatabase);
  
  /// get the filter from the record
  openstudio::ruleset::ModelObjectFilterBooleanAttribute modelObjectFilterBooleanAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectFilterBooleanAttributeRecord_Impl;

  // from impl
  ModelObjectFilterBooleanAttributeRecord(
      boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectFilterBooleanAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterBooleanAttributeRecord");
};

/** \relates ModelObjectFilterBooleanAttributeRecord*/
typedef boost::optional<ModelObjectFilterBooleanAttributeRecord> OptionalModelObjectFilterBooleanAttributeRecord;

/** \relates ModelObjectFilterBooleanAttributeRecord*/
typedef std::vector<ModelObjectFilterBooleanAttributeRecord> ModelObjectFilterBooleanAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERBOOLEANATTRIBUTERECORD_HPP
