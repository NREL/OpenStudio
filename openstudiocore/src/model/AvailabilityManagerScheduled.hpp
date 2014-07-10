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

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULED_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULED_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;

namespace detail {

  class AvailabilityManagerScheduled_Impl;

} // detail

/** AvailabilityManagerScheduled is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:Scheduled'. */
class MODEL_API AvailabilityManagerScheduled : public ModelObject {
 public:

  explicit AvailabilityManagerScheduled(const Model& model);

  virtual ~AvailabilityManagerScheduled() {}

  static IddObjectType iddObjectType();

  Schedule schedule() const;

  bool setSchedule(Schedule& schedule);

 protected:

  /// @cond

  typedef detail::AvailabilityManagerScheduled_Impl ImplType;

  explicit AvailabilityManagerScheduled(std::shared_ptr<detail::AvailabilityManagerScheduled_Impl> impl);

  friend class detail::AvailabilityManagerScheduled_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
  
 private:

  REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduled");
};

/** \relates AvailabilityManagerScheduled*/
typedef boost::optional<AvailabilityManagerScheduled> OptionalAvailabilityManagerScheduled;

/** \relates AvailabilityManagerScheduled*/
typedef std::vector<AvailabilityManagerScheduled> AvailabilityManagerScheduledVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERSCHEDULED_HPP

