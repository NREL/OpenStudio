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

#ifndef PROJECT_ACTIONCLAUSERECORD_HPP
#define PROJECT_ACTIONCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset{
  class ActionClause;
}
namespace project {

namespace detail {
  class ActionClauseRecord_Impl;
}

/** \class ActionClauseRecordType 
 *
 *  \relates ActionClauseRecord */
OPENSTUDIO_ENUM( ActionClauseRecordType,
    ((ModelActionClauseRecord)(ModelActionClauseRecord)(0))
    ((ModelObjectActionClauseRecord)(ModelObjectActionClauseRecord)(1))
    ((StandardsActionClauseRecord)(StandardsActionClauseRecord)(2))
);

/** ActionClauseRecord
 **/
class PROJECT_API ActionClauseRecord : public ClauseRecord {
 public:
  virtual ~ActionClauseRecord() {}

  typedef detail::ActionClauseRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// get all action clauses 
  static std::vector<ActionClauseRecord> getActionClauseRecords(ProjectDatabase& database);

  /// get action clause by id
  static boost::optional<ActionClauseRecord> getActionClauseRecord(int id, ProjectDatabase& database);

  /// get action clause from a query
  static boost::optional<ActionClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // try to find the action in the database by UUID, if not found try to create a new record
  static ActionClauseRecord factoryFromAction(const openstudio::ruleset::ActionClause& action, ProjectDatabase& projectDatabase);

  /// get the corresponding action clause
  openstudio::ruleset::ActionClause actionClause() const;

 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  ActionClauseRecord(boost::shared_ptr<detail::ActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  ActionClauseRecord(boost::shared_ptr<detail::ActionClauseRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.ActionClauseRecord");
};

/** \relates ActionClauseRecord */
typedef boost::optional<ActionClauseRecord> OptionalActionClauseRecord;

/** \relates ActionClauseRecord */
typedef std::vector<ActionClauseRecord> ActionClauseRecordVector;

} // project
} // openstudio

#endif // PROJECT_ACTIONCLAUSERECORD_HPP
