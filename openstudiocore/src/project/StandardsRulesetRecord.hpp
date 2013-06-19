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

#ifndef PROJECT_STANDARDSRULESETRECORD_HPP
#define PROJECT_STANDARDSRULESETRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/RulesetRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset{
  class StandardsRuleset;
  class StandardsRule;
}

namespace project {

  namespace detail {
    class StandardsRulesetRecord_Impl;
  }

  class StandardsRuleRecord;
  class StandardsRulesetOptionRecord;

  /** StandardsRulesetRecord
  **/
  class PROJECT_API StandardsRulesetRecord : public RulesetRecord {
    public:

      typedef detail::StandardsRulesetRecord_Impl ImplType;
      typedef RulesetRecordColumns ColumnsType;
      typedef RulesetRecord ObjectRecordType;

      /// constructors
      StandardsRulesetRecord(const openstudio::ruleset::StandardsRuleset& standardsRuleset, ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      StandardsRulesetRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~StandardsRulesetRecord() {}

      static boost::optional<StandardsRulesetRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                      ProjectDatabase& database);

      /// get all standards ruleset records in database
      static std::vector<StandardsRulesetRecord> getStandardsRulesetRecords(ProjectDatabase& projectDatabase);

      /// get standards ruleset record in database
      static boost::optional<StandardsRulesetRecord> getStandardsRulesetRecord(int id, ProjectDatabase& projectDatabase);

      /// get the standards ruleset
      openstudio::ruleset::StandardsRuleset standardsRuleset() const;

      /// get the standards ruleset option records
      std::vector<StandardsRulesetOptionRecord> standardsRulesetOptionRecords() const;

      /// get the standards rule records
      std::vector<StandardsRuleRecord> standardsRuleRecords() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsRulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsRulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsRulesetRecord");
  };

  /** \relates StandardsRulesetRecord */
  typedef boost::optional<StandardsRulesetRecord> OptionalStandardsRulesetRecord;

  /** \relates StandardsRulesetRecord */
  typedef std::vector<StandardsRulesetRecord> StandardsRulesetRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSRULESETRECORD_HPP
