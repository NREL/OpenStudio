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

#ifndef PROJECT_STANDARDSACTIONRULEVIOLATEDRECORD_HPP
#define PROJECT_STANDARDSACTIONRULEVIOLATEDRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsActionRuleResultRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsActionRuleViolated;
}
namespace project {

namespace detail {
  class StandardsActionRuleViolatedRecord_Impl;
}

/** StandardsActionRuleViolatedRecord
**/
class PROJECT_API StandardsActionRuleViolatedRecord : public StandardsActionRuleResultRecord {
 public:

  typedef detail::StandardsActionRuleViolatedRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsActionRuleViolatedRecord(
      const openstudio::ruleset::StandardsActionRuleViolated& standardsActionRuleViolated, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsActionRuleViolatedRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsActionRuleViolatedRecord() {}

  static boost::optional<StandardsActionRuleViolatedRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// get all model records in database
  static std::vector<StandardsActionRuleViolatedRecord> getStandardsActionRuleViolatedRecords(
      ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<StandardsActionRuleViolatedRecord> getStandardsActionRuleViolatedRecord(
      int id, ProjectDatabase& projectDatabase);

  /// get the action from the record
  openstudio::ruleset::StandardsActionRuleViolated standardsActionRuleViolated() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsActionRuleViolatedRecord_Impl;

  // from impl
  StandardsActionRuleViolatedRecord(
      boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsActionRuleViolatedRecord(boost::shared_ptr<detail::StandardsActionRuleViolatedRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsActionRuleViolatedRecord");
};

/** \relates StandardsActionRuleViolatedRecord*/
typedef boost::optional<StandardsActionRuleViolatedRecord> OptionalStandardsActionRuleViolatedRecord;

/** \relates StandardsActionRuleViolatedRecord*/
typedef std::vector<StandardsActionRuleViolatedRecord> StandardsActionRuleViolatedRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSACTIONRULEVIOLATEDRECORD_HPP
