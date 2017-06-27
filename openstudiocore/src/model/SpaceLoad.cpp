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

#include "SpaceLoad.hpp"
#include "SpaceLoad_Impl.hpp"

#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SpaceLoad_Impl::SpaceLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceItem_Impl(idfObject, model, keepHandle)
  {}

  SpaceLoad_Impl::SpaceLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceItem_Impl(other, model, keepHandle)
  {}

  SpaceLoad_Impl::SpaceLoad_Impl(const SpaceLoad_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceItem_Impl(other, model, keepHandle)
  {}

  boost::optional<ParentObject> SpaceLoad_Impl::parent() const
  {
    boost::optional<ParentObject> result(this->space());
    if (!result){
      result = boost::optional<ParentObject>(this->spaceType());
    }
    return result;
  }

  bool SpaceLoad_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Space>()){
      result = this->setSpace(newParent.cast<Space>());
    }else if (newParent.optionalCast<SpaceType>()){
      result = this->setSpaceType(newParent.cast<SpaceType>());
    }
    return result;
  }

  boost::optional<SpaceType> SpaceLoad_Impl::spaceType() const
  {
    boost::optional<SpaceType> result;
    OptionalWorkspaceObject spaceType = getTarget(this->spaceIndex());
    if (spaceType){
      result = spaceType->optionalCast<SpaceType>();
    }
    return result;
  }

  bool SpaceLoad_Impl::setSpaceType(const SpaceType& spaceType)
  {
    return setPointer(this->spaceIndex(), spaceType.handle());
  }

  void SpaceLoad_Impl::resetSpaceType() {
    bool ok = setString(this->spaceIndex(), "");
    OS_ASSERT(ok);
  }

  boost::optional<ModelObject> SpaceLoad_Impl::spaceTypeAsModelObject() const {
    OptionalModelObject result;
    OptionalSpaceType intermediate = spaceType();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SpaceLoad_Impl::setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSpaceType intermediate = modelObject->optionalCast<SpaceType>();
      if (intermediate) {
        return setSpaceType(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSpaceType();
    }
    return true;
  }

} // detail

SpaceLoad::SpaceLoad(IddObjectType type,const Model& model)
  : SpaceItem(type,model)
{
  OS_ASSERT(getImpl<detail::SpaceLoad_Impl>());
}

SpaceLoad::SpaceLoad(std::shared_ptr<detail::SpaceLoad_Impl> impl)
  : SpaceItem(std::move(impl))
{}

boost::optional<SpaceType> SpaceLoad::spaceType() const
{
  return getImpl<detail::SpaceLoad_Impl>()->spaceType();
}

bool SpaceLoad::setSpaceType(const SpaceType& spaceType)
{
  return getImpl<detail::SpaceLoad_Impl>()->setSpaceType(spaceType);
}

void SpaceLoad::resetSpaceType() {
  getImpl<detail::SpaceLoad_Impl>()->resetSpaceType();
}

bool SpaceLoad::hardSize()
{
  return getImpl<detail::SpaceLoad_Impl>()->hardSize();
}

bool SpaceLoad::hardApplySchedules()
{
  return getImpl<detail::SpaceLoad_Impl>()->hardApplySchedules();
}

bool SpaceLoad::isAbsolute() const
{
  return getImpl<detail::SpaceLoad_Impl>()->isAbsolute();
}

} // model
} // openstudio
