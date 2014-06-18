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

#ifndef PROJECT_INPUTVARIABLERECORD_IMPL_HPP
#define PROJECT_INPUTVARIABLERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "VariableRecord_Impl.hpp"
#include "InputVariableRecord.hpp"

#include "../analysis/AnalysisEnums.hpp"

namespace openstudio {
namespace project {

namespace detail {

  /** InputVariableRecord_Impl is a VariableRecord_Impl that is the implementation class for InputVariableRecord.*/
  class PROJECT_API InputVariableRecord_Impl : public VariableRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    InputVariableRecord_Impl(const analysis::InputVariable& inputVariable,
                             const InputVariableRecordType& inputVariableRecordType,
                             ProblemRecord& problemRecord,
                             int workflowIndex);

    InputVariableRecord_Impl(const analysis::InputVariable& inputVariable,
                             const InputVariableRecordType& inputVariableRecordType,
                             FunctionRecord& functionRecord,
                             int variableVectorIndex,
                             boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    InputVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~InputVariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(ProjectDatabase& projectDatabase);

    //@}
    /** @name Getters */
    //@{

    /** Returns this InputVariableRecord's parent ProblemRecord. */
    boost::optional<ProblemRecord> problemRecord() const;

    /** Returns this InputVariableRecord's AttributeRecords, which are used to store
     *  uncertainty description information. */
    std::vector<AttributeRecord> attributeRecords() const;

    boost::optional<analysis::UncertaintyDescription> uncertaintyDescription() const;

    virtual analysis::InputVariable inputVariable() const = 0;

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
    REGISTER_LOGGER("openstudio.project.InputVariableRecord");

    boost::optional<int> m_problemRecordId;
    boost::optional<analysis::UncertaintyDescriptionType> m_uncertaintyDescriptionType;
    InputVariableRecordType m_inputVariableRecordType;

    boost::optional<int> m_lastProblemRecordId;
    boost::optional<analysis::UncertaintyDescriptionType> m_lastUncertaintyDescriptionType;
    InputVariableRecordType m_lastInputVariableRecordType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_INPUTVARIABLERECORD_IMPL_HPP

