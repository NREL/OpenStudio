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

#ifndef PROJECT_STANDARDSACTIONCLAUSERECORD_HPP
#define PROJECT_STANDARDSACTIONCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsActionClause;
}
namespace project {

  namespace detail {
    class StandardsActionClauseRecord_Impl;
  }

  OPENSTUDIO_ENUM( StandardsActionClauseRecordType,
    ((StandardsActionCloneModelRecord)(StandardsActionCloneModelRecord)(0))
    ((StandardsActionRuleResultRecord)(StandardsActionRuleResultRecord)(1))
  );

  /** StandardsActionClauseRecord
  **/
  class PROJECT_API StandardsActionClauseRecord : public ActionClauseRecord {
    public:

      typedef detail::StandardsActionClauseRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      virtual ~StandardsActionClauseRecord() {} 

      /// get from action
      static StandardsActionClauseRecord factoryFromAction(const openstudio::ruleset::StandardsActionClause& action, ProjectDatabase& projectDatabase);

      // get from query
      static boost::optional<StandardsActionClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsActionClauseRecord(boost::shared_ptr<openstudio::project::detail::StandardsActionClauseRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsActionClauseRecord");
  };

} // project
} // openstudio

#endif // PROJECT_STANDARDSACTIONCLAUSERECORD_HPP
