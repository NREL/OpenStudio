/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_ModelObjectList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ModelObjectList_Impl::ModelObjectList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ModelObjectList::iddObjectType());
    }

    ModelObjectList_Impl::ModelObjectList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ModelObjectList::iddObjectType());
    }

    ModelObjectList_Impl::ModelObjectList_Impl(const ModelObjectList_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ModelObjectList_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ModelObjectList_Impl::iddObjectType() const {
      return ModelObjectList::iddObjectType();
    }

    std::vector<IdfObject> ModelObjectList_Impl::remove() {
      std::vector<IdfObject> result;

      std::vector<ModelObject> modelObjects = this->modelObjects();

      if (!modelObjects.empty()) {
        for (auto& modelObject : modelObjects) {
          std::vector<IdfObject> removedModelObject = modelObject.remove();
          result.insert(result.end(), removedModelObject.begin(), removedModelObject.end());
        }
      }

      std::vector<IdfObject> removedModelObjectList = ModelObject_Impl::remove();
      result.insert(result.end(), removedModelObjectList.begin(), removedModelObjectList.end());

      return result;
    }

    ModelObject ModelObjectList_Impl::clone(Model model) const {
      auto modelObjectListClone = ModelObject_Impl::clone(model).cast<ModelObjectList>();

      std::vector<ModelObject> modelObjects = this->modelObjects();

      if (!modelObjects.empty()) {
        modelObjectListClone.removeAllModelObjects();
        for (const auto& modelObject : modelObjects) {
          ModelObject modelObjectClone = modelObject.clone(model);
          modelObjectListClone.addModelObject(modelObjectClone);
        }
      }

      return std::move(modelObjectListClone);
    }

    std::vector<ModelObject> ModelObjectList_Impl::modelObjects() const {
      std::vector<ModelObject> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

        if (wo) {
          result.push_back(wo->cast<ModelObject>());
        }
      }

      return result;
    }

    unsigned ModelObjectList_Impl::size() const {
      return extensibleGroups().size();
    }

    bool ModelObjectList_Impl::addModelObject(const ModelObject& modelObject) {

      // DLM: should this class prevent duplicates in the list?

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

      bool ok = eg.setPointer(OS_ModelObjectListExtensibleFields::ModelObject, modelObject.handle());

      if (!ok) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }
      return ok;
    }

    bool ModelObjectList_Impl::hasModelObject(const ModelObject& modelObject) const {
      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        // DLM: if an object was pointed to by this list but that object was deleted from the model, this list may have an empty field
        // in that case getTarget may fail to return an object
        boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

        OS_ASSERT(wo);

        if (wo->handle() == modelObject.handle()) {
          return true;
        }
      }

      return false;
    }

    void ModelObjectList_Impl::removeModelObject(const ModelObject& modelObject) {
      if (hasModelObject(modelObject)) {
        WorkspaceExtensibleGroup eg = getGroupForModelObject(modelObject);
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }
    }

    void ModelObjectList_Impl::removeAllModelObjects() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    WorkspaceExtensibleGroup ModelObjectList_Impl::getGroupForModelObject(const ModelObject& modelObject) {
      boost::optional<WorkspaceExtensibleGroup> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

        OS_ASSERT(wo);

        if (wo->handle() == modelObject.handle()) {
          result = group.cast<WorkspaceExtensibleGroup>();

          break;
        }
      }

      OS_ASSERT(result);

      return result.get();
    }

  }  // namespace detail

  ModelObjectList::ModelObjectList(const Model& model) : ModelObject(ModelObjectList::iddObjectType(), model, true) {
    OS_ASSERT(getImpl<detail::ModelObjectList_Impl>());
  }

  IddObjectType ModelObjectList::iddObjectType() {
    return {IddObjectType::OS_ModelObjectList};
  }

  std::vector<IdfObject> ModelObjectList::remove() {
    return getImpl<detail::ModelObjectList_Impl>()->remove();
  }

  ModelObject ModelObjectList::clone(Model model) const {
    return getImpl<detail::ModelObjectList_Impl>()->clone(model);
  }

  std::vector<ModelObject> ModelObjectList::modelObjects() const {
    return getImpl<detail::ModelObjectList_Impl>()->modelObjects();
  }

  unsigned ModelObjectList::size() const {
    return getImpl<detail::ModelObjectList_Impl>()->size();
  }

  bool ModelObjectList::addModelObject(const ModelObject& modelObject) {
    return getImpl<detail::ModelObjectList_Impl>()->addModelObject(modelObject);
  }

  bool ModelObjectList::hasModelObject(const ModelObject& modelObject) const {
    return getImpl<detail::ModelObjectList_Impl>()->hasModelObject(modelObject);
  }

  void ModelObjectList::removeModelObject(const ModelObject& modelObject) {
    getImpl<detail::ModelObjectList_Impl>()->removeModelObject(modelObject);
  }

  void ModelObjectList::removeAllModelObjects() {
    getImpl<detail::ModelObjectList_Impl>()->removeAllModelObjects();
  }

  /// @cond
  ModelObjectList::ModelObjectList(std::shared_ptr<detail::ModelObjectList_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
