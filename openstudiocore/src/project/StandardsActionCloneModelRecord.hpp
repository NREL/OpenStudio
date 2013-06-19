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

#ifndef PROJECT_STANDARDSACTIONCLONEMODEL_HPP
#define PROJECT_STANDARDSACTIONCLONEMODEL_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsActionCloneModel;
}
namespace project {

namespace detail {
  class StandardsActionCloneModelRecord_Impl;
}

class StandardsFilterAvailableModelRecord;

/** StandardsActionCloneModelRecord
**/
class PROJECT_API StandardsActionCloneModelRecord : public StandardsActionClauseRecord {
 public:

  typedef detail::StandardsActionCloneModelRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsActionCloneModelRecord(
      const openstudio::ruleset::StandardsActionCloneModel& standardsActionCloneModel,
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsActionCloneModelRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsActionCloneModelRecord() {}

  static boost::optional<StandardsActionCloneModelRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// get all model records in database
  static std::vector<StandardsActionCloneModelRecord> getStandardsActionCloneModelRecords(
      ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<StandardsActionCloneModelRecord> getStandardsActionCloneModelRecord(
      int id, ProjectDatabase& projectDatabase);

  /// get the action from the record
  openstudio::ruleset::StandardsActionCloneModel standardsActionCloneModel() const;

  /// get the child filter record
  StandardsFilterAvailableModelRecord standardsFilterAvailableModelRecord() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsActionCloneModelRecord_Impl;

  // from impl
  StandardsActionCloneModelRecord(
      boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl> impl,
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsActionCloneModelRecord(
      boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsActionCloneModelRecord");
};

/** \relates StandardsActionCloneModelRecord*/
typedef boost::optional<StandardsActionCloneModelRecord> OptionalStandardsActionCloneModelRecord;

/** \relates StandardsActionCloneModelRecord*/
typedef std::vector<StandardsActionCloneModelRecord> StandardsActionCloneModelRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSACTIONCLONEMODEL_HPP
