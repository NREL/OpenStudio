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

#ifndef PROJECT_RUBYCONTINUOUSVARIABLERECORD_IMPL_HPP
#define PROJECT_RUBYCONTINUOUSVARIABLERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ContinuousVariableRecord_Impl.hpp"

namespace openstudio {
namespace analysis {
  class RubyContinuousVariable;
}

namespace project {

class OSArgumentRecord;
class RubyMeasureRecord;

namespace detail {

  /** RubyContinuousVariableRecord_Impl is a ContinuousVariableRecord_Impl that is the implementation class for RubyContinuousVariableRecord.*/
  class PROJECT_API RubyContinuousVariableRecord_Impl : public ContinuousVariableRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RubyContinuousVariableRecord_Impl(const analysis::RubyContinuousVariable& rubyContinuousVariable,
                                      ProblemRecord& problemRecord,
                                      int variableVectorIndex);

    RubyContinuousVariableRecord_Impl(const analysis::RubyContinuousVariable& rubyContinuousVariable,
                                      FunctionRecord& functionRecord,
                                      int variableVectorIndex,
                                      boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    RubyContinuousVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~RubyContinuousVariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    //@}
    /** @name Getters */
    //@{

    OSArgumentRecord osArgumentRecord() const;

    RubyMeasureRecord rubyMeasureRecord() const;

    virtual analysis::Variable variable() const;

    virtual analysis::InputVariable inputVariable() const;

    virtual analysis::ContinuousVariable continuousVariable() const;

    analysis::RubyContinuousVariable rubyContinuousVariable() const;

    //@}
    /** @name Setters */
    //@{

    /** Reverts record id values back to last state. For use in RubyContinuousVariableRecord
     *   constructor so can access old related records for comparison and possible removal. */
    void revertToLastRecordIds();

    void setRubyMeasureRecordId(int id);

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
    REGISTER_LOGGER("openstudio.project.RubyContinuousVariableRecord");

    int m_rubyMeasureRecordId;

    int m_lastRubyMeasureRecordId;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_RUBYCONTINUOUSVARIABLERECORD_IMPL_HPP

