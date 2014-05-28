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

#ifndef MODEL_SCHEDULE_HPP
#define MODEL_SCHEDULE_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class Schedule_Impl;
}

/** Schedule is a ScheduleBase abstract class that represents the reference list 'ScheduleNames'.
 *  The 'ScheduleNames' reference list includes objects of type:
 *     \li OS:Schedule:Year
 *     \li OS:Schedule:Compact
 *     \li OS:Schedule:Constant
 *     \li OS:Schedule:Ruleset
 *     \li OS:Schedule:FixedInterval
 *     \li OS:Schedule:VariableInterval
 *
 *  For detailed information on ScheduleTypeLimits, please see ScheduleBase and 
 *  \link ScheduleTypeRegistrySingleton ScheduleTypeRegistry \endlink. In general, ModelObject 
 *  setters that take a Schedule will fail if the ScheduleTypeLimits are set and are incompatible 
 *  with the proposed use. This ensures that the Schedule is written using units and bounds 
 *  expected by the user object (e.g. fractional for lighting schedules, W for people activity 
 *  level schedules, C or F for thermostat schedules). */
class MODEL_API Schedule : public ScheduleBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Schedule() {}

  //@}
 protected:
  /// @cond
  typedef detail::Schedule_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  // constructor
  explicit Schedule(std::shared_ptr<detail::Schedule_Impl> impl);

  Schedule(IddObjectType type,const Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Schedule");
};

// optional Schedule
typedef boost::optional<Schedule> OptionalSchedule;

// vector of Schedule
typedef std::vector<Schedule> ScheduleVector;

} // model
} // openstudio

#endif
