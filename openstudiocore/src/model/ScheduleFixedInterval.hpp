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

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ScheduleFixedInterval_Impl;

} // detail

/** ScheduleFixedInterval is a ScheduleInterval that wraps the OpenStudio IDD object 
 *  'OS_Schedule_FixedInterval'. */
class MODEL_API ScheduleFixedInterval : public ScheduleInterval {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ScheduleFixedInterval(const Model& model);

  virtual ~ScheduleFixedInterval() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  bool interpolatetoTimestep() const;

  bool isInterpolatetoTimestepDefaulted() const;

  double intervalLength() const;

  double outOfRangeValue() const;

  bool isOutOfRangeValueDefaulted() const;

  int startMonth() const;

  int startDay() const;

  //@}
  /** @name Setters */
  //@{

  void setInterpolatetoTimestep(bool interpolatetoTimestep);

  void resetInterpolatetoTimestep();

  bool setIntervalLength(double intervalLength);

  void setOutOfRangeValue(double outOfRangeValue);

  void resetOutOfRangeValue();

  bool setStartMonth(int startMonth);

  bool setStartDay(int startDay);

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleFixedInterval_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ScheduleFixedInterval_Impl;

  explicit ScheduleFixedInterval(std::shared_ptr<detail::ScheduleFixedInterval_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
};

/** \relates ScheduleFixedInterval*/
typedef boost::optional<ScheduleFixedInterval> OptionalScheduleFixedInterval;

/** \relates ScheduleFixedInterval*/
typedef std::vector<ScheduleFixedInterval> ScheduleFixedIntervalVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEFIXEDINTERVAL_HPP

