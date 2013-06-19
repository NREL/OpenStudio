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

#ifndef PROJECT_RULERECORD_IMPL_HPP
#define PROJECT_RULERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord_Impl.hpp>
#include <project/RuleRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class Rule;
}
namespace project {

namespace detail {

  class PROJECT_API RuleRecord_Impl : public ObjectRecord_Impl
  {

    Q_OBJECT;

    public:

    /// constructors
    RuleRecord_Impl(const openstudio::ruleset::Rule& rule, 
        const RuleRecordType& ruleRecordType, 
        const ProjectDatabase& projectDatabase);

    /// constructor from query, throws if bad query
    RuleRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

    virtual ~RuleRecord_Impl() {}

    /// get name of the database table
    virtual std::string databaseTableName() const;

    /// get whether this rule is active
    bool active() const;

    /// get all clauses
    std::vector<ClauseRecord> clauseRecords() const;

    /// get filter clauses
    std::vector<FilterClauseRecord> filterClauseRecords() const;

    /// get action clauses
    std::vector<ActionClauseRecord> actionClauseRecords() const;

    protected:

    /// bind values to a query for saving
    /// override in derived classes
    virtual void bindValues(QSqlQuery& query) const;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues();

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues();

    // DLM@20110601: temp code
    void setRuleRecordType(const RuleRecordType& ruleRecordType);

    private:

    REGISTER_LOGGER("openstudio.project.RuleRecord");

    bool m_active;

    bool m_lastActive;

    RuleRecordType m_ruleRecordType;

    RuleRecordType m_lastRuleRecordType;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_RULERECORD_IMPL_HPP
