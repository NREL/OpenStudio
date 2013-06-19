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

#ifndef PROJECT_STANDARDSFILTERRULESETOPTIONRECORD_HPP
#define PROJECT_STANDARDSFILTERRULESETOPTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterRulesetOption;
}
namespace project {

  namespace detail {
    class StandardsFilterRulesetOptionRecord_Impl;
  }

  class StandardsRulesetOptionRecord;

  /** StandardsFilterRulesetOptionRecord 
  **/
  class PROJECT_API StandardsFilterRulesetOptionRecord : public StandardsFilterClauseRecord {
    public:

      typedef detail::StandardsFilterRulesetOptionRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      /// constructors
      StandardsFilterRulesetOptionRecord(
          const openstudio::ruleset::StandardsFilterRulesetOption& standardsFilterRulesetOption, 
          ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      StandardsFilterRulesetOptionRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~StandardsFilterRulesetOptionRecord() {}

      static boost::optional<StandardsFilterRulesetOptionRecord> 
      factoryFromQuery(const QSqlQuery& query,ProjectDatabase& database);

      /// get all model records in database
      static std::vector<StandardsFilterRulesetOptionRecord> 
      getStandardsFilterRulesetOptionRecords(ProjectDatabase& projectDatabase);

      /// get model record in database
      static boost::optional<StandardsFilterRulesetOptionRecord> 
      getStandardsFilterRulesetOptionRecord(int id, ProjectDatabase& projectDatabase);

      /// get the filter from the record
      openstudio::ruleset::StandardsFilterRulesetOption standardsFilterRulesetOption() const;

      /// get option record
      StandardsRulesetOptionRecord standardsRulesetOptionRecord() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsFilterRulesetOptionRecord(
          boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl> impl, 
          ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsFilterRulesetOptionRecord(
          boost::shared_ptr<detail::StandardsFilterRulesetOptionRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsFilterRulesetOptionRecord");
  };

  /** \relates StandardsFilterRulesetOptionRecord */
  typedef boost::optional<StandardsFilterRulesetOptionRecord> OptionalStandardsFilterRulesetOptionRecord;

  /** \relates StandardsFilterRulesetOptionRecord */
  typedef std::vector<StandardsFilterRulesetOptionRecord> StandardsFilterRulesetOptionRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTERRULESETOPTIONRECORD_HPP
