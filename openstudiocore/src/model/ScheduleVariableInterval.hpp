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

#ifndef MODEL_SCHEDULEVARIABLEINTERVAL_HPP
#define MODEL_SCHEDULEVARIABLEINTERVAL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ScheduleVariableInterval_Impl;

} // detail

/** ScheduleVariableInterval is a ScheduleInterval that wraps the OpenStudio IDD object 
 *  'OS_Schedule_VariableInterval'. */
class MODEL_API ScheduleVariableInterval : public ScheduleInterval {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ScheduleVariableInterval(const Model& model);

  virtual ~ScheduleVariableInterval() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  bool interpolatetoTimestep() const;

  bool isInterpolatetoTimestepDefaulted() const;

  double outOfRangeValue() const;

  bool isOutOfRangeValueDefaulted() const;

  int startMonth() const;

  int startDay() const;

  //@}
  /** @name Setters */
  //@{

  void setInterpolatetoTimestep(bool interpolatetoTimestep);

  void resetInterpolatetoTimestep();

  void setOutOfRangeValue(double outOfRangeValue);

  void resetOutOfRangeValue();

  bool setStartMonth(int startMonth);

  bool setStartDay(int startDay);

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleVariableInterval_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ScheduleVariableInterval_Impl;

  explicit ScheduleVariableInterval(std::shared_ptr<detail::ScheduleVariableInterval_Impl> impl);
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ScheduleVariableInterval");
};

/** \relates ScheduleVariableInterval*/
typedef boost::optional<ScheduleVariableInterval> OptionalScheduleVariableInterval;

/** \relates ScheduleVariableInterval*/
typedef std::vector<ScheduleVariableInterval> ScheduleVariableIntervalVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEVARIABLEINTERVAL_HPP

