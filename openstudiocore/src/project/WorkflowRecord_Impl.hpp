/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    virtual std::string databaseTableName() const override;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const override;

    /// get child objects
    virtual std::vector<ObjectRecord> children() const override;

    /// get resource objects
    virtual std::vector<ObjectRecord> resources() const override;

    /// get join record objects
    virtual std::vector<JoinRecord> joinRecords() const override;

    /// save the row for just this object in the database
    /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database) override;

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
    virtual void bindValues(QSqlQuery& query) const override;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const override;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues() override;

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues() override;

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
