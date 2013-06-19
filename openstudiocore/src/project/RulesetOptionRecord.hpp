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

#ifndef PROJECT_RULESETOPTIONRECORD_HPP
#define PROJECT_RULESETOPTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

namespace detail {
  class RulesetOptionRecord_Impl;
}

class RulesetRecord;

/** \class RulesetOptionRecordType
 *  \relates RulesetOptionRecord */
OPENSTUDIO_ENUM(RulesetOptionRecordType,
                ((ModelRulesetOptionRecord)(ModelRulesetOptionRecord)(0))
                ((StandardsRulesetOptionRecord)(StandardsRulesetOptionRecord)(1))
                );

/** \class RulesetOptionRecordColumns
 *  \brief Columns of the RulesetOptionRecords table.
 *  \relates RulesetOptionRecord */
OPENSTUDIO_ENUM( RulesetOptionRecordColumns,
                 ((id)(INTEGER PRIMARY KEY)(0))
                 ((handle)(TEXT)(1))
                 ((name)(TEXT)(2))
                 ((displayName)(TEXT)(3))
                 ((description)(TEXT)(4))
                 ((timestampCreate)(TEXT)(5))
                 ((timestampLast)(TEXT)(6))
                 ((uuidLast)(TEXT)(7))
                 ((rulesetOptionRecordType)(INTEGER)(8))
                 ((modelRulesetOptionRecordType)(INTEGER)(9))
                 ((standardsRulesetOptionRecordType)(INTEGER)(10))
                 ((rulesetRecordId)(INTEGER)(11))
                 ((optionEnum)(INTEGER)(12))
                 ((defaultOptionValue)(TEXT)(13))
                 ((optionValue)(TEXT)(14))
                 );

/** RulesetOptionRecord is an abstract ObjectRecord used to serialize ruleset::RulesetOptions. */
class PROJECT_API RulesetOptionRecord : public ObjectRecord {
 public:

  typedef detail::RulesetOptionRecord_Impl ImplType;
  typedef RulesetOptionRecordColumns ColumnsType;
  typedef RulesetOptionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~RulesetOptionRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /// get all clauses
  static std::vector<RulesetOptionRecord> getRulesetOptionRecords(ProjectDatabase& database);

  /// get clause by id
  static boost::optional<RulesetOptionRecord> getRulesetOptionRecord(int id, ProjectDatabase& database);

  /// get clause from a query
  static boost::optional<RulesetOptionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /// get the related ruleset record
  boost::optional<RulesetRecord> rulesetRecord() const;

  //@}
 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  RulesetOptionRecord(boost::shared_ptr<detail::RulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  RulesetOptionRecord(boost::shared_ptr<detail::RulesetOptionRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.RulesetOptionRecord");
};

/** \relates RulesetOptionRecord */
typedef boost::optional<RulesetOptionRecord> OptionalRulesetOptionRecord;

/** \relates RulesetOptionRecord */
typedef std::vector<RulesetOptionRecord> RulesetOptionRecordVector;

} // project
} // openstudio

#endif // PROJECT_RULESETOPTIONRECORD_HPP
