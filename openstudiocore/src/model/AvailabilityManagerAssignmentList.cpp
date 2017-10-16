/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"

#include "AvailabilityManager.hpp"
#include "Loop.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>

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
    if (result.empty())
    {
      // Not appropriate: no specific output
    }
    return result;
  }

  IddObjectType AvailabilityManagerAssignmentList_Impl::iddObjectType() const {
    return AvailabilityManagerAssignmentList::iddObjectType();
  }

  /*
   * TODO: Clone all the AVMs
   * The Ctor gets a loop though... Perhaps should have a clone(Loop loop) method instead?
   * It's meant to be called in the Loop.clone method anwyays...
   */
  ModelObject AvailabilityManagerAssignmentList_Impl::clone(Model model) const
  {
    boost::optional<Loop> thisLoop;
    for(const Loop& loop: model.getLoops()) {
      if (loop.availabilityManagerAssignmentList().handle() == this->handle()) {
        thisLoop = loop;
      }
    }
    OS_ASSERT(thisLoop);

    // Create a new, blank, one
    AvailabilityManagerAssignmentList avmListClone(*thisLoop);

    // Clone all AVMs and populate the clone
    std::vector<AvailabilityManager> avmVector = availabilityManagers();
    for (const AvailabilityManager& avm : avmVector) {
      avmClone = avm.clone(model);
      bool ok = avmListClone.addAvailabilityManager(avmClone);
      OS_ASSERT(ok);
    }
    return avmListClone;
  }


  unsigned AvailabilityManagerAssignmentList_Impl::priority(const AvailabilityManager & avm) {

    // TODO: perhaps I chould explicitly check that the avm is
    // 1. in the same model
    // 2. in the avm list
    // In order to LOG a helpful message to the user.
    boost::optional<unsigned> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    int i = 1;

    for( const auto & group : groups )
    {
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_AvailabilityManagerAssignmentList_Fields::AvailabilityManagerName);

      OS_ASSERT(wo);

      if( wo->handle() == equipment.handle() )
      {
        result = i
        break;
      }
      i++;
    }

    OS_ASSERT(result);

    return result.get();
  }


  std::vector<AvailabilityManager> AvailabilityManagerAssignmentList_Impl::availabilityManagers() const
  {
    std::vector<AvailabilityManager> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( const auto & group : groups )
    {
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_AvailabilityManagerAssignmentList_Fields::AvailabilityManagerName);

      if( wo )
      {
        result.push_back(wo->cast<AvailabilityManager>());
      }
    }

    return result;
  }


  bool AvailabilityManagerAssignmentList_Impl::addAvailabilityManager(const AvailabilityManager & avm)
  {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool ok = eg.setPointer(OS_AvailabilityManagerAssignmentList_Fields::AvailabilityManagerName, avm.handle());

    return ok;
  }


  bool AvailabilityManagerAssignmentList_Impl::setPriority(const AvailabilityManager & avm, unsigned priority)
  {
    std::vector<AvailabilityManager> avmVector = availabilityManagers();

    OS_ASSERT( std::find(avmVector.begin(),avmVector.end(),equipment) != avmVector.end() );

    if( priority > avmVector.size() ) priority = avmVector.size();
    if( priority < 1 ) priority = 1;

    avmVector.erase(std::find(avmVector.begin(),avmVector.end(),avm));

    avmVector.insert(avmVector.begin() + (priority - 1),avm);

    // Clear the extensible groups, and redo them
    clearExtensibleGroups();
    for (const AvailabilityManager& a : avmVector) {
      bool ok = addAvailabilityManager(a);
      OS_ASSERT(ok);
    }
    return true;
    }
  }

  bool AvailabilityManagerAssignmentList_Impl::addAvailabilityManager(const AvailabilityManager & avm, unsigned priority)
  {
    bool ok = addAvailabilityManager(avm);
    OS_ASSERT(ok);
    ok = setPriority(avm, priority);
    return ok;
  }


} // detail


/*
 * Ctor, accepts Loop as an input parameter
 */
AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(const Loop& loop)
  : ModelObject(AvailabilityManagerAssignmentList::iddObjectType(),loop.model())
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerAssignmentList_Impl>());

  this->setName(loop.name() + " AvailabilityManagerAssignmentList");
}

IddObjectType AvailabilityManagerAssignmentList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManagerAssignmentList);
}

unsigned AvailabilityManagerAssignmentList::priority(const AvailabilityManager & avm)
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->priority(avm);
}

std::vector<AvailabilityManager> AvailabilityManagerAssignmentList::availabilityManagers() const
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->availabilityManagers();
}

bool AvailabilityManagerAssignmentList::addAvailabilityManager(const AvailabilityManager & avm)
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->addAvailabilityManager(avm);
}

bool AvailabilityManagerAssignmentList::setPriority(const AvailabilityManager & avm, unsigned priority);
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->setPriority(avm, priority);
}

bool AvailabilityManagerAssignmentList::addAvailabilityManager(const AvailabilityManager & avm, unsigned priority)
{
  return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->addAvailabilityManager(avm, priority);
}




/// @cond
AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(std::shared_ptr<detail::AvailabilityManagerAssignmentList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

