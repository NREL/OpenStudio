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

#ifndef PROJECT_ACTIONCLAUSERECORD_IMPL_HPP
#define PROJECT_ACTIONCLAUSERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ClauseRecord_Impl.hpp>
#include <project/ActionClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ActionClause;
}
namespace project {

  class ActionClauseRecord;

namespace detail {

  class PROJECT_API ActionClauseRecord_Impl : public ClauseRecord_Impl
  {

    Q_OBJECT;

    public:

    /// constructors
    ActionClauseRecord_Impl(const openstudio::ruleset::ActionClause& actionClause, const ActionClauseRecordType& actionClauseRecordType, const ProjectDatabase& projectDatabase);

    /// constructor from query, throws if bad query
    ActionClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

    virtual ~ActionClauseRecord_Impl() {}

    /// get the corresponding action clause
    virtual openstudio::ruleset::ActionClause actionClause() const = 0;

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

    private:

    REGISTER_LOGGER("openstudio.project.ActionClauseRecord");

    ActionClauseRecordType m_actionClauseRecordType;

    ActionClauseRecordType m_lastActionClauseRecordType;

  };

} // detail
} // project
} // openstudio

#endif // PROJECT_ACTIONCLAUSERECORD_IMPL_HPP
