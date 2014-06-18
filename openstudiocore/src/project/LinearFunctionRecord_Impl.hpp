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

#ifndef PROJECT_LINEARFUNCTIONRECORD_IMPL_HPP
#define PROJECT_LINEARFUNCTIONRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "FunctionRecord_Impl.hpp"

namespace openstudio {
namespace analysis {
  class LinearFunction;
}
namespace project {

namespace detail {

  /** LinearFunctionRecord_Impl is a FunctionRecord_Impl that is the implementation class for LinearFunctionRecord.*/
  class PROJECT_API LinearFunctionRecord_Impl : public FunctionRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    LinearFunctionRecord_Impl(const analysis::LinearFunction& linearFunction, 
                              ProblemRecord& problemRecord,
                              const FunctionType& functionType,
                              int functionVectorIndex);

    /** Constructor from query. Throws if bad query. */
    LinearFunctionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~LinearFunctionRecord_Impl() {}

    //@}

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Function function() const;
  
    analysis::LinearFunction linearFunction() const;

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
    REGISTER_LOGGER("openstudio.project.LinearFunctionRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_LINEARFUNCTIONRECORD_IMPL_HPP

