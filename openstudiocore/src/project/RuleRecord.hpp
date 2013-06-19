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

#ifndef PROJECT_RULERECORD_HPP
#define PROJECT_RULERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class Rule;
}
namespace project {

namespace detail {
  class RuleRecord_Impl;
}

class ClauseRecord;
class FilterClauseRecord;
class ActionClauseRecord;

/** \class RuleRecordType
 *
 *  \relates RuleRecord */
OPENSTUDIO_ENUM(RuleRecordType,
                ((ModelRuleRecord)(ModelRuleRecord)(0))
                ((StandardsRuleRecord)(StandardsRuleRecord)(1))
                ((StandardsAttributeCheckRuleRecord)(StandardsAttributeCheckRuleRecord)(2))
                );

/** \class RuleRecordColumns
 *  \brief Column definitions for the RuleRecords table.
 *
 *  \relates RuleRecord */
OPENSTUDIO_ENUM( RuleRecordColumns,
                 ((id)(INTEGER PRIMARY KEY)(0))
                 ((handle)(TEXT)(1))
                 ((name)(TEXT)(2))
                 ((displayName)(TEXT)(3))
                 ((description)(TEXT)(4))
                 ((timestampCreate)(TEXT)(5))
                 ((timestampLast)(TEXT)(6))
                 ((uuidLast)(TEXT)(7))
                 ((active)(TEXT)(8))
                 ((ruleRecordType)(INTEGER)(9))
                 );

/** RuleRecord is an abstract ObjectRecord that stores ruleset::Rules. */
class PROJECT_API RuleRecord : public ObjectRecord {
 public:

  typedef detail::RuleRecord_Impl ImplType;
  typedef RuleRecordColumns ColumnsType;
  typedef RuleRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~RuleRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /// get all rule records
  static std::vector<RuleRecord> getRuleRecords(ProjectDatabase& database);

  /// get rule record by id
  static boost::optional<RuleRecord> getRuleRecord(int id, ProjectDatabase& database);

  /// get rule record from query
  static boost::optional<RuleRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns true if this rule will be applied during the execution of its parent ruleset. */
  bool active() const;

  /// get filter clauses
  std::vector<FilterClauseRecord> filterClauseRecords() const;

  /// get action clauses
  std::vector<ActionClauseRecord> actionClauseRecords() const;

  //@}
 protected:

  friend class Record;
  friend class detail::Record_Impl;
  friend class ProjectDatabase;

  // from impl
  RuleRecord(const openstudio::ruleset::Rule& rule, boost::shared_ptr<detail::RuleRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl
  RuleRecord(boost::shared_ptr<detail::RuleRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  RuleRecord(boost::shared_ptr<detail::RuleRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.RuleRecord");
};

/** \relates RuleRecord */
typedef boost::optional<RuleRecord> OptionalRuleRecord;

/** \relates RuleRecord */
typedef std::vector<RuleRecord> RuleRecordVector;

} // project
} // openstudio

#endif // PROJECT_RULERECORD_HPP
