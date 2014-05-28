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

#ifndef PROJECT_DATAPOINT_MEASURE_JOINRECORD_IMPL_HPP
#define PROJECT_DATAPOINT_MEASURE_JOINRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "JoinRecord_Impl.hpp"

namespace openstudio {
namespace project {

class DataPointRecord;
class MeasureRecord;

namespace detail {

  /** DataPoint_Measure_JoinRecord_Impl is a JoinRecord_Impl that is the
   *  implementation class for DataPoint_Measure_JoinRecord. */
  class PROJECT_API DataPoint_Measure_JoinRecord_Impl : public JoinRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DataPoint_Measure_JoinRecord_Impl(
        const DataPointRecord& dataPointRecord, 
        const MeasureRecord& measureRecord);

    /** Constructor from query. Throws if bad query. */
    DataPoint_Measure_JoinRecord_Impl(const QSqlQuery& query,ProjectDatabase& database);

    virtual ~DataPoint_Measure_JoinRecord_Impl() {}

    //@}

    /** Get the name of the database table for this record. Should be implemented by the base 
     *  class for the table. */
    virtual std::string databaseTableName() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual ObjectRecord leftObject() const;

    virtual ObjectRecord rightObject() const;

    DataPointRecord dataPointRecord() const;

    MeasureRecord measureRecord() const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.project.DataPoint_Measure_JoinRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_DATAPOINT_MEASURE_JOINRECORD_IMPL_HPP

