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

#ifndef PROJECT_MODELOBJECTFILTERTYPERECORD_HPP
#define PROJECT_MODELOBJECTFILTERTYPERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ModelObjectFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterType;
}
namespace project {

namespace detail {
  class ModelObjectFilterTypeRecord_Impl;
}

/** ModelObjectFilterTypeRecord 
**/
class PROJECT_API ModelObjectFilterTypeRecord : public ModelObjectFilterClauseRecord {
 public:

  typedef detail::ModelObjectFilterTypeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  ModelObjectFilterTypeRecord(const openstudio::ruleset::ModelObjectFilterType& modelObjectFilterType, ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  ModelObjectFilterTypeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~ModelObjectFilterTypeRecord() {}

  static boost::optional<ModelObjectFilterTypeRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                       ProjectDatabase& database);

  /// get all model records in database
  static std::vector<ModelObjectFilterTypeRecord> getModelObjectFilterTypeRecords(ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<ModelObjectFilterTypeRecord> getModelObjectFilterTypeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::ModelObjectFilterType modelObjectFilterType() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelObjectFilterTypeRecord_Impl;

  // from impl
  ModelObjectFilterTypeRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl> impl, ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelObjectFilterTypeRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelObjectFilterTypeRecord");
};

/** \relates ModelObjectFilterTypeRecord*/
typedef boost::optional<ModelObjectFilterTypeRecord> OptionalModelObjectFilterTypeRecord;

/** \relates ModelObjectFilterTypeRecord*/
typedef std::vector<ModelObjectFilterTypeRecord> ModelObjectFilterTypeRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERTYPERECORD_HPP
