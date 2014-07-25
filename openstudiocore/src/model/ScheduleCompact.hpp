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

#ifndef MODEL_SCHEDULECOMPACT_HPP
#define MODEL_SCHEDULECOMPACT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class ScheduleCompact_Impl;
} // detail

/** ScheduleCompact is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Compact'. */
class MODEL_API ScheduleCompact : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Creates an empty (no values set for any days) ScheduleCompact. */
  explicit ScheduleCompact(const Model& model);

  /** Creates a ScheduleCompact with constantValue applied to the entire year. */
  ScheduleCompact(const Model& model, double constantValue);

  /** Creates a ScheduleCompact with constantValue applied to the entire year, if 
   *  scheduleTypeLimits() and the units are compatible. Otherwise creates an empty 
   *  ScheduleCompact. */
  ScheduleCompact(const Model& model, const Quantity& constantValue);

  virtual ~ScheduleCompact() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Setters */
  //@{

  /** Clears all existing data and sets the data so that value will be applied to the entire 
   *  year. */
  void setToConstantValue(double value);

  /** If value can be converted to scheduleTypeLimit()'s units, then all existing data is cleared
   *  and value is set (after unit conversion, if necessary). */
  bool setToConstantValue(const Quantity& value);

  //@}
  /** @name Queries */
  //@{

  /** Returns true if this ScheduleCompact has a single value applied to the entire year. 
   *  (Roughly--the text fields are not checked.) */
  bool isConstantValue() const;

  /** Returns this ScheduleCompact's constant value, if this ScheduleCompact conforms to that 
   *  pattern. Otherwise returns boost::none (evaluates to false). */
  boost::optional<double> constantValue() const;

  /** Returns this ScheduleCompact's constant value as a Quantity, if this ScheduleCompact 
   *  conforms to that pattern and scheduleTypeLimits() is set. Otherwise returns boost::none 
   *  (evaluates to false). */
  boost::optional<Quantity> getConstantValue(bool returnIP=false) const;

  //@}
 protected:

  typedef detail::ScheduleCompact_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit ScheduleCompact(std::shared_ptr<detail::ScheduleCompact_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ScheduleCompact");
};

/** \relates ScheduleCompact */
typedef boost::optional<ScheduleCompact> OptionalScheduleCompact;

/** \relates ScheduleCompact */
typedef std::vector<ScheduleCompact> ScheduleCompactVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULECOMPACT_HPP
