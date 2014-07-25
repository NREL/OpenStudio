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

#ifndef PROJECT_OUTPUTVARIABLERECORD_IMPL_HPP
#define PROJECT_OUTPUTVARIABLERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "VariableRecord_Impl.hpp"
#include "OutputVariableRecord.hpp"

namespace openstudio {
namespace project {

namespace detail {

  /** OutputVariableRecord_Impl is a VariableRecord_Impl that is the implementation class for OutputVariableRecord.*/
  class PROJECT_API OutputVariableRecord_Impl : public VariableRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OutputVariableRecord_Impl(const analysis::OutputVariable& outputVariable,
                              const OutputVariableRecordType& outputVariableRecordType,
                              FunctionRecord& functionRecord,
                              int variableVectorIndex,
                              boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    OutputVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~OutputVariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(ProjectDatabase& projectDatabase);

    //@}
    /** @name Getters */
    //@{

    virtual analysis::OutputVariable outputVariable() const = 0;

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
    REGISTER_LOGGER("openstudio.project.OutputVariableRecord");

    OutputVariableRecordType m_outputVariableRecordType;

    OutputVariableRecordType m_lastOutputVariableRecordType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_OUTPUTVARIABLERECORD_IMPL_HPP

