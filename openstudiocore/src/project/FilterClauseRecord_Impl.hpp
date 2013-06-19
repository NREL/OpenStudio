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

#ifndef PROJECT_FILTERCLAUSERECORD_IMPL_HPP
#define PROJECT_FILTERCLAUSERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ClauseRecord_Impl.hpp>
#include <project/FilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class FilterClause;
}
namespace project {

namespace detail {

  class PROJECT_API FilterClauseRecord_Impl : public ClauseRecord_Impl
  {

    Q_OBJECT;

    public:

    /// constructors
    FilterClauseRecord_Impl(const openstudio::ruleset::FilterClause& filterClause, const FilterClauseRecordType& filterClauseRecordType, const ProjectDatabase& projectDatabase);

    /// constructor from query, throws if bad query
    FilterClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

    virtual ~FilterClauseRecord_Impl() {}

    /// get the corresponding filter clause
    virtual openstudio::ruleset::FilterClause filterClause() const = 0;

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

    REGISTER_LOGGER("openstudio.project.FilterClauseRecord");

    FilterClauseRecordType m_filterClauseRecordType;

    FilterClauseRecordType m_lastFilterClauseRecordType;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_FILTERCLAUSERECORD_IMPL_HPP
