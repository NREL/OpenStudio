/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

    SpaceItem_Impl::SpaceItem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ModelObject_Impl(idfObject, model, keepHandle) {}

    SpaceItem_Impl::SpaceItem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    SpaceItem_Impl::SpaceItem_Impl(const SpaceItem_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SpaceItem_Impl::parent() const {
      return boost::optional<ParentObject>(this->space());
    }

    bool SpaceItem_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<Space>()) {
        result = this->setSpace(newParent.cast<Space>());
      }
      return result;
    }

    boost::optional<Space> SpaceItem_Impl::space() const {
      boost::optional<Space> result;
      OptionalWorkspaceObject space = getTarget(this->spaceIndex());
      if (space) {
        result = space->optionalCast<Space>();
      }
      return result;
    }

    bool SpaceItem_Impl::setSpace(const Space& space) {
      return setPointer(this->spaceIndex(), space.handle());
    }

    void SpaceItem_Impl::resetSpace() {
      bool ok = setString(this->spaceIndex(), "");
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
        } else {
          return false;
        }
      } else {
        resetSpace();
      }
      return true;
    }

  }  // namespace detail

  SpaceItem::SpaceItem(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::SpaceItem_Impl>());
  }

  SpaceItem::SpaceItem(std::shared_ptr<detail::SpaceItem_Impl> impl) : ModelObject(std::move(impl)) {}

  boost::optional<Space> SpaceItem::space() const {
    return getImpl<detail::SpaceItem_Impl>()->space();
  }

  bool SpaceItem::setSpace(const Space& space) {
    return getImpl<detail::SpaceItem_Impl>()->setSpace(space);
  }

  void SpaceItem::resetSpace() {
    return getImpl<detail::SpaceItem_Impl>()->resetSpace();
  }

}  // namespace model
}  // namespace openstudio
