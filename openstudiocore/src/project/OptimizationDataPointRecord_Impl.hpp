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

#ifndef PROJECT_OPTIMIZATIONDATAPOINTRECORD_IMPL_HPP
#define PROJECT_OPTIMIZATIONDATAPOINTRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "DataPointRecord_Impl.hpp"

namespace openstudio {
namespace analysis {
  class OptimizationDataPoint;
}
namespace project {

class DataPointValueRecord;
class OptimizationProblemRecord;

namespace detail {

  /** OptimizationDataPointRecord_Impl is a DataPointRecord_Impl that is the implementation class for OptimizationDataPointRecord.*/
  class PROJECT_API OptimizationDataPointRecord_Impl : public DataPointRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    OptimizationDataPointRecord_Impl(const analysis::OptimizationDataPoint& optimizationDataPoint,
                                     AnalysisRecord& analysisRecord,
                                     const OptimizationProblemRecord& problemRecord);

    /** Constructor from query. Throws if bad query. */
    OptimizationDataPointRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~OptimizationDataPointRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual analysis::DataPoint dataPoint() const;

    /** Provided for callers operating directly on the database, not holding a copy of this
     *  analysis in memory. Use with caution. Does not do file system cleanup. */
    virtual void clearResults();

    //@}
    /** @name Getters */
    //@{

    std::vector<DataPointValueRecord> objectiveValueRecords() const;

    std::vector<double> objectiveValues() const;

    analysis::OptimizationDataPoint optimizationDataPoint() const;

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
    REGISTER_LOGGER("openstudio.project.OptimizationDataPointRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_OPTIMIZATIONDATAPOINTRECORD_IMPL_HPP

