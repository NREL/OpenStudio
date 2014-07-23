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

#ifndef MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class AvailabilityManagerScheduled;
class AvailabilityManagerNightCycle;

namespace detail {

  /** AvailabilityManagerAssignmentList_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerAssignmentList.*/
  class MODEL_API AvailabilityManagerAssignmentList_Impl : public ModelObject_Impl {
   public:

    AvailabilityManagerAssignmentList_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    AvailabilityManagerAssignmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    AvailabilityManagerAssignmentList_Impl(const AvailabilityManagerAssignmentList_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~AvailabilityManagerAssignmentList_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    AvailabilityManagerScheduled availabilityManagerScheduled() const;

    AvailabilityManagerNightCycle availabilityManagerNightCycle() const;

    std::vector<openstudio::IdfObject> remove();

    ModelObject clone(Model model) const;

   protected:

    bool setAvailabilityManagerNightCycle( const AvailabilityManagerNightCycle & availabilityManagerNightCycle );

    bool setAvailabilityManagerScheduled( const AvailabilityManagerScheduled & availabilityManagerScheduled );

   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerAssignmentList");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_IMPL_HPP

