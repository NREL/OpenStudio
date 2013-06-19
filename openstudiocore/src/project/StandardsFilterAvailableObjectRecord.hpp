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

#ifndef PROJECT_STANDARDSFILTERAVAILABLEOBJECTRECORD_HPP
#define PROJECT_STANDARDSFILTERAVAILABLEOBJECTRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterAvailableObject;
}
namespace project {

namespace detail {
  class StandardsFilterAvailableObjectRecord_Impl;
}

class StandardsFilterAvailableModelRecord;

/** StandardsFilterAvailableObjectRecord
**/
class PROJECT_API StandardsFilterAvailableObjectRecord : public StandardsFilterClauseRecord {
 public:

  typedef detail::StandardsFilterAvailableObjectRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsFilterAvailableObjectRecord(
      const openstudio::ruleset::StandardsFilterAvailableObject& standardsFilterAvailableObject,
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsFilterAvailableObjectRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsFilterAvailableObjectRecord() {}

  static boost::optional<StandardsFilterAvailableObjectRecord> factoryFromQuery(
     const QSqlQuery& query,ProjectDatabase& database);

  /// get all model records in database
  static std::vector<StandardsFilterAvailableObjectRecord>
  getStandardsFilterAvailableObjectRecords(ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<StandardsFilterAvailableObjectRecord>
  getStandardsFilterAvailableObjectRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::StandardsFilterAvailableObject standardsFilterAvailableObject() const;

  /// get the available model filter if set
  boost::optional<StandardsFilterAvailableModelRecord> standardsFilterAvailableModelRecord() const;

 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  StandardsFilterAvailableObjectRecord(
      boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl> impl,
      ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  StandardsFilterAvailableObjectRecord(boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.StandardsFilterAvailableObjectRecord");
};

/** \relates StandardsFilterAvailableObjectRecord */
typedef boost::optional<StandardsFilterAvailableObjectRecord> OptionalStandardsFilterAvailableObjectRecord;

/** \relates StandardsFilterAvailableObjectRecord */
typedef std::vector<StandardsFilterAvailableObjectRecord> StandardsFilterAvailableObjectRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTERAVAILABLEOBJECTRECORD_HPP
