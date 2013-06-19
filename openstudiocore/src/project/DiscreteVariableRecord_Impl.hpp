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

#ifndef PROJECT_DISCRETEVARIABLERECORD_IMPL_HPP
#define PROJECT_DISCRETEVARIABLERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/InputVariableRecord_Impl.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>

namespace openstudio {
namespace analysis {
  class DiscreteVariable;
}

namespace project {

class DiscretePerturbationRecord;

namespace detail{

  /** DiscreteVariableRecord_Impl is an InputVariableRecord_Impl that is the
   *  implementation class for DiscreteVariableRecord.*/
  class PROJECT_API DiscreteVariableRecord_Impl : public InputVariableRecord_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    DiscreteVariableRecord_Impl(const analysis::DiscreteVariable& discreteVariable,
                                ProblemRecord& problemRecord,
                                int workflowIndex);

    DiscreteVariableRecord_Impl(const analysis::DiscreteVariable& discreteVariable,
                                FunctionRecord& functionRecord,
                                int variableVectorIndex,
                                boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    DiscreteVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~DiscreteVariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /// get resource objects
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Variable variable() const;

    virtual analysis::InputVariable inputVariable() const;

    //@}
    /** @name Getters and Queries */
    //@{

    analysis::DiscreteVariable discreteVariable() const;

    /// number of discrete perturbations
    unsigned numPerturbations(bool selectedPerturbationsOnly) const;

    /// all discrete perturbation ids
    std::vector<int> discretePerturbationRecordIds(bool selectedPerturbationsOnly) const;

    /// all discrete perturbations
    std::vector<DiscretePerturbationRecord> discretePerturbationRecords(
        bool selectedPerturbationsOnly) const;

    /** Get the DiscretePerturbationRecord at index. Throws if index >= numPerturbations(false). */
    DiscretePerturbationRecord getDiscretePerturbationRecord(int perturbationVectorIndex) const;

    //@}
   protected:
    /** Bind data member values to a query for saving. */
    virtual void bindValues(QSqlQuery& query) const;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const;

    /** Save values to last state. */
    virtual void saveLastValues();

    /** Revert values back to last state. */
    virtual void revertToLastValues();

   private:
    REGISTER_LOGGER("openstudio.project.DiscreteVariableRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_DISCRETEVARIABLERECORD_IMPL_HPP

