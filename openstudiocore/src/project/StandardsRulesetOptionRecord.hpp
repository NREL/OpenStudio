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

#ifndef PROJECT_STANDARDSRULESETOPTIONRECORD_HPP
#define PROJECT_STANDARDSRULESETOPTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/RulesetOptionRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsRulesetOption;
}
namespace project {

  namespace detail {
    class StandardsRulesetOptionRecord_Impl;
  }

  class StandardsRulesetRecord;

  OPENSTUDIO_ENUM( StandardsRulesetOptionRecordType,
      ((StandardsEnumRulesetOptionRecord)(StandardsEnumRulesetOptionRecord)(0))
      );

  /** StandardsRulesetOptionRecord
   **/
  class PROJECT_API StandardsRulesetOptionRecord : public RulesetOptionRecord {
    public:

      typedef detail::StandardsRulesetOptionRecord_Impl ImplType;
      typedef RulesetOptionRecordColumns ColumnsType;
      typedef RulesetOptionRecord ObjectRecordType;

      virtual ~StandardsRulesetOptionRecord() {}

      /// get all standards ruleset options in database
      static std::vector<StandardsRulesetOptionRecord> getStandardsRulesetOptionRecords(ProjectDatabase& database);

      /// get standards ruleset option by id
      static boost::optional<StandardsRulesetOptionRecord> getStandardsRulesetOptionRecord(int id, ProjectDatabase& database);

      /// try to find the standards ruleset option in the database by UUID, if not found try to create a new record
      static StandardsRulesetOptionRecord factoryFromOption(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, const StandardsRulesetRecord& standardsRulesetRecord);

      /// try to find the standards ruleset option in the database by UUID, if not found try to create a new record
      static StandardsRulesetOptionRecord factoryFromOption(const openstudio::ruleset::StandardsRulesetOption& standardsRulesetOption, ProjectDatabase& database);

      /// get standards ruleset option from a query
      static boost::optional<StandardsRulesetOptionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

      /// get the ruleset option from the record
      openstudio::ruleset::StandardsRulesetOption standardsRulesetOption() const; 

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsRulesetOptionRecord(boost::shared_ptr<detail::StandardsRulesetOptionRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsRulesetOptionRecord");
  };

} // project
} // openstudio

#endif // PROJECT_STANDARDSRULESETOPTIONRECORD_HPP
