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

#ifndef PROJECT_STANDARDSACTIONRULERESULTRECORD_HPP
#define PROJECT_STANDARDSACTIONRULERESULTRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsActionRuleResult;
}
namespace project {

namespace detail {
  class StandardsActionRuleResultRecord_Impl;
}

class StandardsFilterObjectAttributeRecord;

OPENSTUDIO_ENUM( StandardsActionRuleResultRecordType,
  ((StandardsActionRuleSatisfiedRecord)(StandardsActionRuleSatisfiedRecord)(0))
  ((StandardsActionRuleViolatedRecord)(StandardsActionRuleViolatedRecord)(1))
);

/** StandardsActionRuleResultRecord
**/
class PROJECT_API StandardsActionRuleResultRecord : public StandardsActionClauseRecord {
 public:

  typedef detail::StandardsActionRuleResultRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  virtual ~StandardsActionRuleResultRecord() {}


  /// get all records
  static std::vector<StandardsActionRuleResultRecord> getStandardsActionRuleResultRecords(ProjectDatabase& database);
        
  /// get record by id
  static boost::optional<StandardsActionRuleResultRecord> getStandardsActionRuleResultRecord(int id, ProjectDatabase& database);

  /// get record from action
  static StandardsActionRuleResultRecord factoryFromAction(const openstudio::ruleset::StandardsActionRuleResult& action, ProjectDatabase& projectDatabase);

  /// get record from a query
  static boost::optional<StandardsActionRuleResultRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  /// get action record
  StandardsFilterObjectAttributeRecord standardsFilterObjectAttributeRecord() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsActionRuleResultRecord_Impl;

  // from impl
  StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleResultRecord_Impl> impl, ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleResultRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsActionRuleResultRecord");
};

/** \relates StandardsActionRuleResultRecord*/
typedef boost::optional<StandardsActionRuleResultRecord> OptionalStandardsActionRuleResultRecord;

/** \relates StandardsActionRuleResultRecord*/
typedef std::vector<StandardsActionRuleResultRecord> StandardsActionRuleResultRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSACTIONRULERESULTRECORD_HPP
