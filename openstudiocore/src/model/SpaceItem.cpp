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
  : ModelObject(impl)
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
