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

#ifndef PROJECT_RULE_CLAUSE_JOINRECORD_HPP
#define PROJECT_RULE_CLAUSE_JOINRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/JoinRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

  namespace detail {
    class Rule_Clause_JoinRecord_Impl;
  }

  class RuleRecord;
  class ClauseRecord;
  class FilterClauseRecord;
  class ActionClauseRecord;

  class PROJECT_API Rule_Clause_JoinRecord : public JoinRecord {
    public:

      typedef detail::Rule_Clause_JoinRecord_Impl ImplType;
      typedef JoinRecordColumns ColumnsType;

      /// get name of the database table
      static std::string databaseTableName();

      static UpdateByIdQueryData updateByIdQueryData();

      /// constructors
      Rule_Clause_JoinRecord(const RuleRecord& ruleRecord, const ClauseRecord& clauseRecord);

      virtual ~Rule_Clause_JoinRecord() {}

      /// get clause records for rule record
      static std::vector<ClauseRecord> getClauseRecords(const RuleRecord& ruleRecord);

      /// get rule records for clause record
      static std::vector<RuleRecord> getRuleRecords(const ClauseRecord& clauseRecord);

      /// get clause record
      ClauseRecord clauseRecord() const;

      /// get rule record
      RuleRecord ruleRecord() const;

    protected:

      friend class Record;
      friend class JoinRecord;

      // from impl
      Rule_Clause_JoinRecord(boost::shared_ptr<detail::Rule_Clause_JoinRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl
      Rule_Clause_JoinRecord(boost::shared_ptr<detail::Rule_Clause_JoinRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.Rule_Clause_JoinRecord");
  };

} // project
} // openstudio

#endif // PROJECT_RULE_CLAUSE_JOINRECORD_HPP
