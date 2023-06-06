/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ResourceObject_Impl::ResourceObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {}

    ResourceObject_Impl::ResourceObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    ResourceObject_Impl::ResourceObject_Impl(const ResourceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    unsigned ResourceObject_Impl::directUseCount(bool excludeChildren) const {

      unsigned result = 0;

      ModelObjectVector children;
      if (excludeChildren) {
        children = this->children();
      }

      WorkspaceObjectVector sources = this->sources();
      for (const WorkspaceObject& source : sources) {
        if (excludeChildren) {
          // check if this is a child
          auto it = std::find_if(children.cbegin(), children.cend(),
                                 [h = source.handle()](const ModelObject& mo) { return handleEquals<ModelObject, Handle>(mo, h); });
          if (it == children.end()) {
            // non-Child--count the use
            ++result;
          }
        } else {
          // count the use
          ++result;
        }
      }

      return result;
    }

    unsigned ResourceObject_Impl::nonResourceObjectUseCount(bool excludeChildren) const {

      unsigned result = 0;

      ModelObjectVector children;
      if (excludeChildren) {
        children = this->children();
      }

      WorkspaceObjectVector sources = this->sources();
      for (const WorkspaceObject& source : sources) {
        OptionalResourceObject oro = source.optionalCast<ResourceObject>();
        if (oro) {
          // another ResourceObject, pass the request through
          result += oro->nonResourceObjectUseCount(excludeChildren);
        } else {

          if (excludeChildren) {
            // check if this is a child
            auto it = std::find_if(children.cbegin(), children.cend(),
                                   [h = source.handle()](const ModelObject& mo) { return handleEquals<ModelObject, Handle>(mo, h); });
            if (it == children.end()) {
              // non-ResourceObject and non-Child--count the use
              ++result;
            }
          } else {
            // non-ResourceObject--count the use
            ++result;
          }
        }
      }

      return result;
    }

  }  // namespace detail

  ResourceObject::ResourceObject(IddObjectType type, const Model& model) : ParentObject(type, model) {
    OS_ASSERT(getImpl<detail::ResourceObject_Impl>());
  }

  ResourceObject::ResourceObject(std::shared_ptr<detail::ResourceObject_Impl> impl) : ParentObject(std::move(impl)) {}

  unsigned ResourceObject::directUseCount(bool excludeChildren) const {
    return getImpl<detail::ResourceObject_Impl>()->directUseCount(excludeChildren);
  }

  unsigned ResourceObject::nonResourceObjectUseCount(bool excludeChildren) const {
    return getImpl<detail::ResourceObject_Impl>()->nonResourceObjectUseCount(excludeChildren);
  }

  std::vector<ResourceObject> getRecursiveResources(const ModelObject& object) {
    std::set<Handle> resultSet;
    std::pair<HandleSet::const_iterator, bool> insertResult;
    std::vector<ResourceObject> result;

    std::deque<ModelObject> objectQueue;
    objectQueue.push_back(object);

    while (!objectQueue.empty()) {
      ModelObject currentObject(objectQueue[0]);
      objectQueue.pop_front();
      for (const ResourceObject& resource : currentObject.resources()) {
        insertResult = resultSet.insert(resource.handle());
        if (insertResult.second) {
          result.push_back(resource);
          objectQueue.push_back(resource.cast<ModelObject>());
        }
      }
    }

    return result;
  }

  std::vector<std::vector<ModelObject>> getRecursiveResourceSubTrees(const ModelObject& object, bool includeComponentCostLineItems) {
    std::set<Handle> resultSet;
    std::pair<HandleSet::const_iterator, bool> insertResult;
    std::vector<std::vector<ModelObject>> result;

    std::deque<ModelObject> objectQueue;
    objectQueue.push_back(object);

    while (!objectQueue.empty()) {
      ModelObject currentObject(objectQueue[0]);
      objectQueue.pop_front();
      for (const ResourceObject& resource : currentObject.resources()) {
        insertResult = resultSet.insert(resource.handle());
        if (insertResult.second) {
          std::vector<ModelObject> subTree = getRecursiveChildren(resource, includeComponentCostLineItems);
          objectQueue.insert(objectQueue.end(), subTree.begin(), subTree.end());
          result.push_back(subTree);
        }
      }
    }

    return result;
  }

}  // namespace model
}  // namespace openstudio
