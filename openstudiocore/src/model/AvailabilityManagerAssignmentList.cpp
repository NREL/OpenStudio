/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"
#include "AvailabilityManagerScheduled.hpp"
#include "AvailabilityManagerScheduled_Impl.hpp"
#include "AvailabilityManagerNightCycle.hpp"
#include "AvailabilityManagerNightCycle_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerAssignmentList::iddObjectType());
  }

  AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerAssignmentList::iddObjectType());
  }

  AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const AvailabilityManagerAssignmentList_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerAssignmentList_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerAssignmentList_Impl::iddObjectType() const {
    return AvailabilityManagerAssignmentList::iddObjectType();
  }

  AvailabilityManagerScheduled AvailabilityManagerAssignmentList_Impl::availabilityManagerScheduled() const
  {
    WorkspaceExtensibleGroup group = getExtensibleGroup(0).cast<WorkspaceExtensibleGroup>();

    OS_ASSERT( ! group.empty() );

    return group.getTarget(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName)->cast<AvailabilityManagerScheduled>();
  }
  
  AvailabilityManagerNightCycle AvailabilityManagerAssignmentList_Impl::availabilityManagerNightCycle() const
  {
    WorkspaceExtensibleGroup group = getExtensibleGroup(1).cast<WorkspaceExtensibleGroup>();

    OS_ASSERT( ! group.empty() );

    return group.getTarget(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName)->cast<AvailabilityManagerNightCycle>();
  }

  std::vector<openstudio::IdfObject> AvailabilityManagerAssignmentList_Impl::remove()
  {
    availabilityManagerNightCycle().remove();

    availabilityManagerScheduled().remove();

    return ModelObject_Impl::remove();
  }

  ModelObject AvailabilityManagerAssignmentList_Impl::clone(Model model) const
  {
    AvailabilityManagerAssignmentList avmlClone = ModelObject_Impl::clone(model).cast<AvailabilityManagerAssignmentList>();

    AvailabilityManagerNightCycle nightCycleClone = availabilityManagerNightCycle().clone(model).cast<AvailabilityManagerNightCycle>();

    AvailabilityManagerScheduled avmScheduledClone = availabilityManagerScheduled().clone(model).cast<AvailabilityManagerScheduled>();

    WorkspaceExtensibleGroup eg = avmlClone.insertExtensibleGroup(0).cast<WorkspaceExtensibleGroup>();

    eg.setPointer(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,avmScheduledClone.handle());

    eg = avmlClone.insertExtensibleGroup(1).cast<WorkspaceExtensibleGroup>();

    eg.setPointer(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,nightCycleClone.handle());

    return avmlClone;
  }

} // detail

AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(const Model& model)
  : ModelObject(AvailabilityManagerAssignmentList::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerAssignmentList_Impl>());

  AvailabilityManagerScheduled avm(model);

  WorkspaceExtensibleGroup eg = insertExtensibleGroup(0).cast<WorkspaceExtensibleGroup>();

  eg.setPointer(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,avm.handle());

  AvailabilityManagerNightCycle avnc(model);

  eg = insertExtensibleGroup(1).cast<WorkspaceExtensibleGroup>();

  eg.setPointer(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,avnc.handle());
}

AvailabilityManagerScheduled AvailabilityManagerAssignmentList::availabilityManagerScheduled() const
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->availabilityManagerScheduled();
}

AvailabilityManagerNightCycle AvailabilityManagerAssignmentList::availabilityManagerNightCycle() const
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->availabilityManagerNightCycle();
}

IddObjectType AvailabilityManagerAssignmentList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManagerAssignmentList);
}

/// @cond
AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(std::shared_ptr<detail::AvailabilityManagerAssignmentList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

