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

#ifndef PROJECT_STANDARDSRULERECORD_HPP
#define PROJECT_STANDARDSRULERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/RuleRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset{
  class StandardsRule;
}

namespace project {

  namespace detail {
    class StandardsRuleRecord_Impl;
  }


  /** StandardsRuleRecord 
  **/
  class PROJECT_API StandardsRuleRecord : public RuleRecord {
    public:

      typedef detail::StandardsRuleRecord_Impl ImplType;
      typedef RuleRecordColumns ColumnsType;
      typedef RuleRecord ObjectRecordType;

      /// constructors
      StandardsRuleRecord(const openstudio::ruleset::StandardsRule& standardsRule, ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      StandardsRuleRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~StandardsRuleRecord() {}

      static boost::optional<StandardsRuleRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database);

      /// get all standards records in database
      static std::vector<StandardsRuleRecord> getStandardsRuleRecords(ProjectDatabase& projectDatabase);

      /// get standards record in database
      static boost::optional<StandardsRuleRecord> getStandardsRuleRecord(int id, ProjectDatabase& projectDatabase);

      /// get the standards rule
      openstudio::ruleset::StandardsRule standardsRule() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsRuleRecord(const ruleset::StandardsRule& standardsRule,
          boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl, 
          ProjectDatabase projectDatabase);

      // from impl
      StandardsRuleRecord(boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsRuleRecord(boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsRuleRecord");
  };

  /** \relates StandardsRuleRecord */
  typedef boost::optional<StandardsRuleRecord> OptionalStandardsRuleRecord;

  /** \relates StandardsRuleRecord */
  typedef std::vector<StandardsRuleRecord> StandardsRuleRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSRULERECORD_HPP
