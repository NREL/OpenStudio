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

#ifndef PROJECT_MODELOBJECTFILTERATTRIBUTERECORD_HPP
#define PROJECT_MODELOBJECTFILTERATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterAttribute;
}
namespace project {

namespace detail {
  class ModelObjectFilterAttributeRecord_Impl;
}

/** \class ModelObjectFilterAttributeRecordType
 *
 *  \relates ModelObjectFilterAttributeRecord */
OPENSTUDIO_ENUM( ModelObjectFilterAttributeRecordType,
  ((ModelObjectFilterBooleanAttributeRecord)(ModelObjectFilterBooleanAttributeRecord)(0))
  ((ModelObjectFilterNumericAttributeRecord)(ModelObjectFilterNumericAttributeRecord)(1))
  ((ModelObjectFilterStringAttributeRecord)(ModelObjectFilterStringAttributeRecord)(2))
);

/** ModelObjectFilterAttributeRecord
**/
class PROJECT_API ModelObjectFilterAttributeRecord : public ModelObjectFilterClauseRecord {
 public:

  typedef detail::ModelObjectFilterAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  virtual ~ModelObjectFilterAttributeRecord() {}


  /// get all model object filter clauses 
  static std::vector<ModelObjectFilterAttributeRecord> getModelObjectFilterAttributeRecords(
      ProjectDatabase& database);
        
  /// get model object filter clause by id
  static boost::optional<ModelObjectFilterAttributeRecord> getModelObjectFilterAttributeRecord(
      int id, ProjectDatabase& database);

  /// get model object filter clause from a query
  static boost::optional<ModelObjectFilterAttributeRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// try to find the filter in the database by UUID, if not found try to create a new record
  static ModelObjectFilterAttributeRecord factoryFromFilter(
      const openstudio::ruleset::ModelObjectFilterAttribute& filter, 
      ProjectDatabase& projectDatabase);

  /// get the attribute name
  std::string attributeName() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectFilterAttributeRecord_Impl;

  // from impl
  ModelObjectFilterAttributeRecord(
      boost::shared_ptr<detail::ModelObjectFilterAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterAttributeRecord");
};

/** \relates ModelObjectFilterAttributeRecord*/
typedef boost::optional<ModelObjectFilterAttributeRecord> OptionalModelObjectFilterAttributeRecord;

/** \relates ModelObjectFilterAttributeRecord*/
typedef std::vector<ModelObjectFilterAttributeRecord> ModelObjectFilterAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERATTRIBUTERECORD_HPP
