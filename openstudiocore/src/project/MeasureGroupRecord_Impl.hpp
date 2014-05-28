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

#ifndef PROJECT_MEASUREGROUPRECORD_IMPL_HPP
#define PROJECT_MEASUREGROUPRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "DiscreteVariableRecord_Impl.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace analysis {
  class MeasureGroup;
}

namespace project {

class MeasureRecord;

namespace detail{

  /** MeasureGroupRecord_Impl is an DiscreteVariableRecord_Impl that is the
   *  implementation class for MeasureGroupRecord.*/
  class PROJECT_API MeasureGroupRecord_Impl : public DiscreteVariableRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    MeasureGroupRecord_Impl(const analysis::MeasureGroup& measureGroup,
                            ProblemRecord& problemRecord,
                            int workflowIndex);

    MeasureGroupRecord_Impl(const analysis::MeasureGroup& measureGroup,
                            FunctionRecord& functionRecord,
                            int variableVectorIndex,
                            boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    MeasureGroupRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~MeasureGroupRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /// get resource objects
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Variable variable() const;

    virtual analysis::InputVariable inputVariable() const;

    virtual analysis::DiscreteVariable discreteVariable() const;

    //@}
    /** @name Getters and Queries */
    //@{

    analysis::MeasureGroup measureGroup() const;

    /// number of measures
    unsigned numMeasures(bool selectedMeasuresOnly) const;

    /// all measure ids
    std::vector<int> measureRecordIds(bool selectedMeasuresOnly) const;

    /// all measures
    std::vector<MeasureRecord> measureRecords(bool selectedMeasuresOnly) const;

    /** Get the MeasureRecord at index. Throws if index >= numMeasures(false). */
    MeasureRecord getMeasureRecord(int vectorIndex) const;

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
    REGISTER_LOGGER("openstudio.project.MeasureGroupRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_MEASUREGROUPRECORD_IMPL_HPP

