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

#ifndef PROJECT_STANDARDSACTIONRULESATISFIEDRECORD_HPP
#define PROJECT_STANDARDSACTIONRULESATISFIEDRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsActionRuleResultRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsActionRuleSatisfied;
}
namespace project {

namespace detail {
  class StandardsActionRuleSatisfiedRecord_Impl;
}

/** StandardsActionRuleSatisfiedRecord 
**/
class PROJECT_API StandardsActionRuleSatisfiedRecord : public StandardsActionRuleResultRecord {
 public:

  typedef detail::StandardsActionRuleSatisfiedRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsActionRuleSatisfiedRecord(
      const openstudio::ruleset::StandardsActionRuleSatisfied& standardsActionRuleSatisfied, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsActionRuleSatisfiedRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsActionRuleSatisfiedRecord() {}

  static boost::optional<StandardsActionRuleSatisfiedRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);
  
  /// get all model records in database
  static std::vector<StandardsActionRuleSatisfiedRecord> getStandardsActionRuleSatisfiedRecords(
      ProjectDatabase& projectDatabase);
  
  /// get model record in database
  static boost::optional<StandardsActionRuleSatisfiedRecord> getStandardsActionRuleSatisfiedRecord(
      int id, ProjectDatabase& projectDatabase);
  
  /// get the action from the record
  openstudio::ruleset::StandardsActionRuleSatisfied standardsActionRuleSatisfied() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsActionRuleSatisfiedRecord_Impl;

  // from impl
  StandardsActionRuleSatisfiedRecord(
      boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsActionRuleSatisfiedRecord(boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsActionRuleSatisfiedRecord");
};

/** \relates StandardsActionRuleSatisfiedRecord*/
typedef boost::optional<StandardsActionRuleSatisfiedRecord> OptionalStandardsActionRuleSatisfiedRecord;

/** \relates StandardsActionRuleSatisfiedRecord*/
typedef std::vector<StandardsActionRuleSatisfiedRecord> StandardsActionRuleSatisfiedRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSACTIONRULESATISFIEDRECORD_HPP
