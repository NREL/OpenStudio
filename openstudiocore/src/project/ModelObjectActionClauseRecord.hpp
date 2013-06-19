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

#ifndef PROJECT_MODELOBJECTACTIONCLAUSERECORD_HPP
#define PROJECT_MODELOBJECTACTIONCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectActionClause;
}
namespace project {

  namespace detail {
    class ModelObjectActionClauseRecord_Impl;
  }

  /** \class ModelObjectActionClauseRecordType
   *
   *  \relates ModelObjectActionClauseRecord */
  OPENSTUDIO_ENUM( ModelObjectActionClauseRecordType,
      ((ModelObjectActionSetAttributeRecord)(ModelObjectActionSetAttributeRecord)(0))
      ((ModelObjectActionSetRelationshipRecord)(ModelObjectActionSetRelationshipRecord)(1))
      );

  /** ModelObjectActionClauseRecord
   **/
  class PROJECT_API ModelObjectActionClauseRecord : public ActionClauseRecord {
    public:
      virtual ~ModelObjectActionClauseRecord() {}

      typedef detail::ModelObjectActionClauseRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      // get from query
      static boost::optional<ModelObjectActionClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

      // get from action
      static ModelObjectActionClauseRecord factoryFromAction(const openstudio::ruleset::ModelObjectActionClause& action, ProjectDatabase& projectDatabase);

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      ModelObjectActionClauseRecord(boost::shared_ptr<openstudio::project::detail::ModelObjectActionClauseRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.ModelObjectActionClauseRecord");
  };

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTACTIONCLAUSERECORD_HPP
