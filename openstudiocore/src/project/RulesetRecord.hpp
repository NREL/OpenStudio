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

#ifndef PROJECT_RULESETRECORD_HPP
#define PROJECT_RULESETRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

namespace detail {
  class RulesetRecord_Impl;
}

/** \class RulesetRecordType
 *
 *  \relates RulesetRecord */
OPENSTUDIO_ENUM(RulesetRecordType,
                ((ModelRulesetRecord)(ModelRulesetRecord)(0))
                );

/** \class RulesetRecordColumns
 *  \brief Column definitions for the RulesetRecords table.
 *
 *  \relates RulesetRecord */
OPENSTUDIO_ENUM(RulesetRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((rulesetRecordType)(INTEGER)(8))
                );

/** RulesetRecord is an abstract ObjectRecord used to serialize ruleset::Rulesets. */
class PROJECT_API RulesetRecord : public ObjectRecord {
 public:

  typedef detail::RulesetRecord_Impl ImplType;
  typedef RulesetRecordColumns ColumnsType;
  typedef RulesetRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~RulesetRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /// get all rulesets
  static std::vector<RulesetRecord> getRulesetRecords(ProjectDatabase& database);

  /// get ruleset by id
  static boost::optional<RulesetRecord> getRulesetRecord(int id, ProjectDatabase& database);

  /// get ruleset from a query
  static boost::optional<RulesetRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

 protected:

  friend class Record;
  friend class detail::Record_Impl;
  friend class ProjectDatabase;

  // from impl
  RulesetRecord(boost::shared_ptr<detail::RulesetRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  RulesetRecord(boost::shared_ptr<openstudio::project::detail::RulesetRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.RulesetRecord");
};

/** \relates RulesetRecord */
typedef boost::optional<RulesetRecord> OptionalRulesetRecord;

/** \relates RulesetRecord */
typedef std::vector<RulesetRecord> RulesetRecordVector;

} // project
} // openstudio

#endif // PROJECT_RULESETRECORD_HPP
