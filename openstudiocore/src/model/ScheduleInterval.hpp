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

#ifndef MODEL_SCHEDULEINTERVAL_HPP
#define MODEL_SCHEDULEINTERVAL_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

namespace detail {
  class ScheduleInterval_Impl;
} // detail

// ETH@20120919 Perhaps all Schedules should be able to return their data as TimeSeries (and TimeSeries should be 
// convertible to and/or implemented using OSQuantityVector). 

/** ScheduleInterval is a Schedule that serves as an abstract base class for ScheduleFixedInterval
 *  and ScheduleVariableInterval. ScheduleIntervals may be constructed from TimeSeries, and can return 
 *  their values as TimeSeries. */
class MODEL_API ScheduleInterval : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ScheduleInterval() {}

  //@}
  
  static boost::optional<ScheduleInterval> fromTimeSeries(const openstudio::TimeSeries& timeSeries, Model& model);
  
  /** @name Getters */
  //@{

  openstudio::TimeSeries timeSeries() const;
  
  //@}
  /** @name Setters */
  //@{

  bool setTimeSeries(const openstudio::TimeSeries& timeSeries);
  
  //@}
 protected:

  typedef detail::ScheduleInterval_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  ScheduleInterval(IddObjectType type,const Model& model);

  // constructor
  explicit ScheduleInterval(std::shared_ptr<detail::ScheduleInterval_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ScheduleInterval");
};

/** \relates ScheduleInterval */
typedef boost::optional<ScheduleInterval> OptionalScheduleInterval;

/** \relates ScheduleInterval */
typedef std::vector<ScheduleInterval> ScheduleIntervalVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEINTERVAL_HPP

