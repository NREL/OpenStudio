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

#ifndef MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP
#define MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class AvailabilityManagerScheduled;
class AvailabilityManagerNightCycle;

namespace detail {

  class AvailabilityManagerAssignmentList_Impl;

} // detail

/** AvailabilityManagerAssignmentList is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManagerAssignmentList'. */
class MODEL_API AvailabilityManagerAssignmentList : public ModelObject {
 public:

  explicit AvailabilityManagerAssignmentList(const Model& model);

  virtual ~AvailabilityManagerAssignmentList() {}

  static IddObjectType iddObjectType();

  AvailabilityManagerScheduled availabilityManagerScheduled() const;

  AvailabilityManagerNightCycle availabilityManagerNightCycle() const;

 protected:

  /// @cond
  typedef detail::AvailabilityManagerAssignmentList_Impl ImplType;

  explicit AvailabilityManagerAssignmentList(std::shared_ptr<detail::AvailabilityManagerAssignmentList_Impl> impl);

  friend class detail::AvailabilityManagerAssignmentList_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.AvailabilityManagerAssignmentList");
};

/** \relates AvailabilityManagerAssignmentList*/
typedef boost::optional<AvailabilityManagerAssignmentList> OptionalAvailabilityManagerAssignmentList;

/** \relates AvailabilityManagerAssignmentList*/
typedef std::vector<AvailabilityManagerAssignmentList> AvailabilityManagerAssignmentListVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP

