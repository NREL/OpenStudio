/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Component.hpp"
#include "AdditionalProperties.hpp"
#include "AdditionalProperties_Impl.hpp"

#include "../utilities/idf/Workspace_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"

#include <deque>

namespace openstudio {
namespace model {

  namespace detail {

    ParentObject_Impl::ParentObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    ParentObject_Impl::ParentObject_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    ParentObject_Impl::ParentObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ParentObject_Impl::ParentObject_Impl(const ParentObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    std::vector<ModelObject> ParentObject_Impl::children() const {
      return {};
    }

    /// remove self and all children objects recursively
    std::vector<IdfObject> ParentObject_Impl::remove() {
      std::vector<IdfObject> result;

      // DLM: the following code does not work because subTree includes this object
      // and we don't want to call remove on the same object recursively
      //
      //ModelObjectVector subTree = getRecursiveChildren(getObject<ParentObject>());
      //for (ModelObject& object : subTree) {
      //  std::vector<IdfObject> removed = object.remove();
      //  result.insert(result.end(), removed.begin(), removed.end());
      //}

      // subTree includes this object, make sure to include costs as well
      // drop the ResourceObject instances, if they are used by other objects
      // This is probably the unique situation where you want to get children minus ResourceObjects
      auto subTree = getRecursiveChildren(getObject<ParentObject>(), true, false);
      result.reserve(subTree.size());
      for (const ModelObject& object : subTree) {
        result.emplace_back(object.idfObject());
      }

      bool ok = model().removeObjects(getHandles<ModelObject>(subTree));
      if (!ok) {
        result.clear();
      }

      return result;
    }

    /// get a vector of allowable children types
    std::vector<IddObjectType> ParentObject_Impl::allowableChildTypes() const {
      IddObjectTypeVector result;
      return result;
    }

    ModelObject ParentObject_Impl::clone(Model model) const {
      ModelObject newParentAsModelObject = ModelObject_Impl::clone(model);
      auto newParent = newParentAsModelObject.cast<ParentObject>();
      for (const ModelObject& child : children()) {
        ModelObject newChild = child.clone(model);
        newChild.setParent(newParent);
      }
      return newParentAsModelObject;
    }

  }  // namespace detail

  ParentObject::ParentObject(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::ParentObject_Impl>());
  }

  ParentObject::ParentObject(std::shared_ptr<detail::ParentObject_Impl> impl) : ModelObject(std::move(impl)) {}

  /// return any children objects in the hierarchy
  ModelObjectVector ParentObject::children() const {
    return getImpl<detail::ParentObject_Impl>()->children();
  }

  /// get a vector of allowable children types
  std::vector<IddObjectType> ParentObject::allowableChildTypes() const {
    return getImpl<detail::ParentObject_Impl>()->allowableChildTypes();
  }

  std::vector<ModelObject> getRecursiveChildren(const ParentObject& object, bool includeLifeCycleCostsAndAdditionalProperties,
                                                bool includeUsedResources) {
    std::set<Handle> resultSet;
    std::pair<HandleSet::const_iterator, bool> insertResult;
    resultSet.insert(object.handle());
    std::vector<ModelObject> result;
    result.push_back(object);

    if (includeLifeCycleCostsAndAdditionalProperties) {
      openstudio::detail::concat_helper(result, object.lifeCycleCosts());
      if (object.hasAdditionalProperties()) {
        result.push_back(object.additionalProperties());
      }
    }

    std::deque<ParentObject> parents;
    parents.push_back(object);

    while (!parents.empty()) {
      ParentObject currentParent(parents[0]);
      parents.pop_front();

      // parent's costs have already been added

      for (const ModelObject& child : currentParent.children()) {
        if (!includeUsedResources) {
          auto _ro = child.optionalCast<ResourceObject>();
          if (_ro && _ro->directUseCount() > 1) {
            continue;
          }
        }
        insertResult = resultSet.insert(child.handle());
        if (insertResult.second) {
          result.push_back(child);

          if (includeLifeCycleCostsAndAdditionalProperties) {
            openstudio::detail::concat_helper(result, child.lifeCycleCosts());
            if (child.hasAdditionalProperties()) {
              result.emplace_back(child.additionalProperties());
            }
          }

          OptionalParentObject opo = child.optionalCast<ParentObject>();
          if (opo) {
            parents.push_back(*opo);
          }
        }
      }
    }

    return result;
  }

  std::vector<ModelObject> getRecursiveChildrenAndResources(const ModelObject& object) {
    std::set<Handle> resultSet;
    std::pair<HandleSet::const_iterator, bool> insertResult;
    std::vector<ModelObject> result;
    resultSet.insert(object.handle());
    result.push_back(object);

    std::deque<ModelObject> objectQueue;
    objectQueue.push_back(object);

    while (!objectQueue.empty()) {
      ModelObject currentObject(objectQueue[0]);
      objectQueue.pop_front();
      // resources
      for (const ResourceObject& resource : currentObject.resources()) {
        insertResult = resultSet.insert(resource.handle());
        if (insertResult.second) {
          // new object
          auto mo = resource.cast<ModelObject>();
          result.push_back(mo);
          objectQueue.push_back(mo);
        }
      }
      // children
      OptionalParentObject opo = currentObject.optionalCast<ParentObject>();
      if (opo) {
        ParentObject currentParent(*opo);
        for (const ModelObject& child : currentParent.children()) {
          insertResult = resultSet.insert(child.handle());
          if (insertResult.second) {
            // new object
            result.push_back(child);
            objectQueue.push_back(child);
          }
        }
      }
    }

    return result;
  }

}  // namespace model
}  // namespace openstudio
