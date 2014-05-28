/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef PROJECT_WORKFLOWRECORD_IMPL_HPP
#define PROJECT_WORKFLOWRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

#include <QVariant>

class QSqlQuery;

namespace openstudio {
namespace runmanager {
  class Workflow;
}
namespace project {

class ProblemRecord;

namespace detail {

  class PROJECT_API WorkflowRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// constructor from run manager workflow
    WorkflowRecord_Impl(const runmanager::Workflow& workflow,
                        ProblemRecord& problemRecord,
                        int workflowIndex);

    /// constructor from query, throws if bad query
    WorkflowRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

    virtual ~WorkflowRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /// get name of the database table
    virtual std::string databaseTableName() const;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const;

    /// get child objects
    virtual std::vector<ObjectRecord> children() const;

    /// get resource objects
    virtual std::vector<ObjectRecord> resources() const;

    /// get join record objects
    virtual std::vector<JoinRecord> joinRecords() const;

    /// save the row for just this object in the database
    /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    //@}
    /** @name Getters */
    //@{

    /** Return the ProblemRecord (parent) of this WorkflowRecord. */
    ProblemRecord problemRecord() const;

    std::string runManagerWorkflowKey() const;

    /** Index of where in problem's vector of WorkflowSteps first WorkItem in workflow() should
     *  be placed. */
    int workflowIndex() const;

    /// get run manager workflow
    runmanager::Workflow workflow() const;

    //@}
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

    REGISTER_LOGGER("openstudio.project.WorkflowRecord");

    std::string m_runManagerWorkflowKey;
    int m_problemRecordId;
    int m_workflowIndex;

    std::string m_lastRunManagerWorkflowKey;
    int m_lastProblemRecordId;
    int m_lastWorkflowIndex;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_WORKFLOWRECORD_IMPL_HPP
