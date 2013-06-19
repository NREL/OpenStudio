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

#ifndef PROJECT_RULESET_RULE_JOINRECORD_HPP
#define PROJECT_RULESET_RULE_JOINRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/JoinRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

  namespace detail {
    class Ruleset_Rule_JoinRecord_Impl;
  }

  class RulesetRecord;
  class RuleRecord;

  class PROJECT_API Ruleset_Rule_JoinRecord : public JoinRecord {
    public:

      typedef detail::Ruleset_Rule_JoinRecord_Impl ImplType;
      typedef JoinRecordColumns ColumnsType;

      /// get name of the database table
      static std::string databaseTableName();

      static UpdateByIdQueryData updateByIdQueryData();

      /// constructors
      Ruleset_Rule_JoinRecord(const RulesetRecord& rulesetRecord, const RuleRecord& ruleRecord);

      virtual ~Ruleset_Rule_JoinRecord() {}

      /// get rule records for ruleset record
      static std::vector<RuleRecord> getRuleRecords(const RulesetRecord& rulesetRecord);

      /// get ruleset records for baseline ruleset record
      static std::vector<RulesetRecord> getRulesetRecords(const RuleRecord& ruleRecord);

      /// get rule record
      RuleRecord ruleRecord() const;

      /// get ruleset record
      RulesetRecord rulesetRecord() const;

    protected:
      friend class Record;
      friend class JoinRecord;

      // from impl
      Ruleset_Rule_JoinRecord(boost::shared_ptr<detail::Ruleset_Rule_JoinRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl
      Ruleset_Rule_JoinRecord(boost::shared_ptr<detail::Ruleset_Rule_JoinRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.Ruleset_Rule_JoinRecord");
  };

} // project
} // openstudio

#endif // PROJECT_RULESET_RULE_JOINRECORD_HPP
