/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

    SpaceLoad_Impl::SpaceLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : SpaceItem_Impl(idfObject, model, keepHandle) {}

    SpaceLoad_Impl::SpaceLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceItem_Impl(other, model, keepHandle) {}

    SpaceLoad_Impl::SpaceLoad_Impl(const SpaceLoad_Impl& other, Model_Impl* model, bool keepHandle) : SpaceItem_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SpaceLoad_Impl::parent() const {
      boost::optional<ParentObject> result(this->space());
      if (!result) {
        result = boost::optional<ParentObject>(this->spaceType());
      }
      return result;
    }

    bool SpaceLoad_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<Space>()) {
        result = this->setSpace(newParent.cast<Space>());
      } else if (newParent.optionalCast<SpaceType>()) {
        result = this->setSpaceType(newParent.cast<SpaceType>());
      }
      return result;
    }

    boost::optional<SpaceType> SpaceLoad_Impl::spaceType() const {
      boost::optional<SpaceType> result;
      OptionalWorkspaceObject spaceType = getTarget(this->spaceIndex());
      if (spaceType) {
        result = spaceType->optionalCast<SpaceType>();
      }
      return result;
    }

    bool SpaceLoad_Impl::setSpaceType(const SpaceType& spaceType) {
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
        } else {
          return false;
        }
      } else {
        resetSpaceType();
      }
      return true;
    }

  }  // namespace detail

  SpaceLoad::SpaceLoad(IddObjectType type, const Model& model) : SpaceItem(type, model) {
    OS_ASSERT(getImpl<detail::SpaceLoad_Impl>());
  }

  SpaceLoad::SpaceLoad(std::shared_ptr<detail::SpaceLoad_Impl> impl) : SpaceItem(std::move(impl)) {}

  boost::optional<SpaceType> SpaceLoad::spaceType() const {
    return getImpl<detail::SpaceLoad_Impl>()->spaceType();
  }

  bool SpaceLoad::setSpaceType(const SpaceType& spaceType) {
    return getImpl<detail::SpaceLoad_Impl>()->setSpaceType(spaceType);
  }

  void SpaceLoad::resetSpaceType() {
    getImpl<detail::SpaceLoad_Impl>()->resetSpaceType();
  }

  bool SpaceLoad::hardSize() {
    return getImpl<detail::SpaceLoad_Impl>()->hardSize();
  }

  bool SpaceLoad::hardApplySchedules() {
    return getImpl<detail::SpaceLoad_Impl>()->hardApplySchedules();
  }

  bool SpaceLoad::isAbsolute() const {
    return getImpl<detail::SpaceLoad_Impl>()->isAbsolute();
  }

}  // namespace model
}  // namespace openstudio
