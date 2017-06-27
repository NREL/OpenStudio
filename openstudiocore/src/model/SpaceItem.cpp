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

#include "SpaceItem.hpp"
#include "SpaceItem_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SpaceItem_Impl::SpaceItem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {}

  SpaceItem_Impl::SpaceItem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  SpaceItem_Impl::SpaceItem_Impl(const SpaceItem_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  boost::optional<ParentObject> SpaceItem_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->space());
  }

  bool SpaceItem_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Space>()){
      result = this->setSpace(newParent.cast<Space>());
    }
    return result;
  }

  boost::optional<Space> SpaceItem_Impl::space() const
  {
    boost::optional<Space> result;
    OptionalWorkspaceObject space = getTarget(this->spaceIndex());
    if (space){
      result = space->optionalCast<Space>();
    }
    return result;
  }

  bool SpaceItem_Impl::setSpace(const Space& space)
  {
    return setPointer(this->spaceIndex(), space.handle());
  }

  void SpaceItem_Impl::resetSpace() {
    bool ok = setString(this->spaceIndex(),"");
    OS_ASSERT(ok);
  }

  boost::optional<ModelObject> SpaceItem_Impl::spaceAsModelObject() const {
    OptionalModelObject result;
    OptionalSpace intermediate = space();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SpaceItem_Impl::setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSpace intermediate = modelObject->optionalCast<Space>();
      if (intermediate) {
        return setSpace(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSpace();
    }
    return true;
  }

} // detail

SpaceItem::SpaceItem(IddObjectType type,const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::SpaceItem_Impl>());
}

SpaceItem::SpaceItem(std::shared_ptr<detail::SpaceItem_Impl> impl)
  : ModelObject(std::move(impl))
{}

boost::optional<Space> SpaceItem::space() const
{
  return getImpl<detail::SpaceItem_Impl>()->space();
}

bool SpaceItem::setSpace(const Space& space)
{
  return getImpl<detail::SpaceItem_Impl>()->setSpace(space);
}

void SpaceItem::resetSpace() {
  return getImpl<detail::SpaceItem_Impl>()->resetSpace();
}

} // model
} // openstudio
