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

#ifndef PROJECT_STANDARDSATTRIBUTECHECKRULERECORD_HPP
#define PROJECT_STANDARDSATTRIBUTECHECKRULERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsRuleRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {

namespace ruleset {
  class StandardsAttributeCheckRule;
}

namespace project {

namespace detail {
  class StandardsAttributeCheckRuleRecord_Impl;
}

class PROJECT_API StandardsAttributeCheckRuleRecord : public StandardsRuleRecord {
  public:

    typedef detail::StandardsAttributeCheckRuleRecord_Impl ImplType;
    typedef RuleRecordColumns ColumnsType;
    typedef RuleRecord ObjectRecordType;

    /// constructors
    StandardsAttributeCheckRuleRecord(
        const openstudio::ruleset::StandardsAttributeCheckRule& standardsAttributeCheckRule, 
        ProjectDatabase& projectDatabase);

    /// constructor from query, throws if bad query
    StandardsAttributeCheckRuleRecord(const QSqlQuery& query, 
        ProjectDatabase& projectDatabase);

    virtual ~StandardsAttributeCheckRuleRecord() {}

    static boost::optional<StandardsAttributeCheckRuleRecord> factoryFromQuery(
        const QSqlQuery& query, 
        ProjectDatabase& database);

    /// get all standards records in database
    static std::vector<StandardsAttributeCheckRuleRecord> getStandardsAttributeCheckRuleRecords(
        ProjectDatabase& projectDatabase);

    /// get standards record in database
    static boost::optional<StandardsAttributeCheckRuleRecord> getStandardsAttributeCheckRuleRecord(
        int id, ProjectDatabase& projectDatabase);

  protected:

    friend class Record;
    friend class ProjectDatabase;

    // from impl
    StandardsAttributeCheckRuleRecord(
        boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl> impl, 
        ProjectDatabase projectDatabase);

    // from impl, does not register in database, use with caution
    StandardsAttributeCheckRuleRecord(
        boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl> impl);

  private:

    REGISTER_LOGGER("openstudio.project.StandardsAttributeCheckRuleRecord");
};

/** \relates StandardsAttributeCheckRuleRecord */
typedef boost::optional<StandardsAttributeCheckRuleRecord> OptionalStandardsAttributeCheckRuleRecord;

/** \relates StandardsAttributeCheckRuleRecord */
typedef std::vector<StandardsAttributeCheckRuleRecord> StandardsAttributeCheckRuleRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSATTRIBUTECHECKRULERECORD_HPP
