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

#ifndef PROJECT_STANDARDSFILTERCLAUSERECORD_HPP
#define PROJECT_STANDARDSFILTERCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/FilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterClause;
}
namespace project {

  namespace detail {
    class StandardsFilterClauseRecord_Impl;
  }

  OPENSTUDIO_ENUM( StandardsFilterClauseRecordType,
      ((StandardsFilterObjectAttributeRecord)(StandardsFilterObjectAttributeRecord)(0))
      ((StandardsFilterAvailableObjectRecord)(StandardsFilterAvailableObjectRecord)(1))
      ((StandardsFilterAvailableModelRecord)(StandardsFilterAvailableModelRecord)(2))
      ((StandardsFilterRulesetOptionRecord)(StandardsFilterRulesetOptionRecord)(3))
      );

  /** StandardsFilterClauseRecord
   **/
  class PROJECT_API StandardsFilterClauseRecord : public FilterClauseRecord {
    public:

      typedef detail::StandardsFilterClauseRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      virtual ~StandardsFilterClauseRecord() {}

      /// get standards object filter clause from a filter
      static StandardsFilterClauseRecord factoryFromFilter(const openstudio::ruleset::StandardsFilterClause& filter, ProjectDatabase& projectDatabase);

      /// get standards object filter clause from a query
      static boost::optional<StandardsFilterClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsFilterClauseRecord(boost::shared_ptr<detail::StandardsFilterClauseRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsFilterClauseRecord");
  };

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTERCLAUSERECORD_HPP
