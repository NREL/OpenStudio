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

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "ResourceObject.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Component.hpp"

#include "../utilities/idf/Workspace_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <deque>

namespace openstudio {
namespace model {

namespace detail {

  ParentObject_Impl::ParentObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {}

  ParentObject_Impl::ParentObject_Impl(IddObjectType type, Model_Impl* model)
    : ModelObject_Impl(type, model)
  {}

  ParentObject_Impl::ParentObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ModelObject_Impl(other, model,keepHandle)
  {}

  ParentObject_Impl::ParentObject_Impl(const ParentObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  std::vector<ModelObject> ParentObject_Impl::children() const {
    return ModelObjectVector();
  }

  /// remove self and all children objects recursively
  std::vector<IdfObject> ParentObject_Impl::remove()
  {
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
    auto subTree = getRecursiveChildren(getObject<ParentObject>(), true);
    // drop the Curve instances
    // Perhaps this could be done in the getRecursiveChildren, but this way
    // the getRecursiveChildren method might be less surprising
    // This is probably the unique situation where you want to get children minus curves
    auto isCurve = [](const ModelObject & modelObject) {
      return modelObject.optionalCast<Curve>();
    };
    auto end = std::remove_if(subTree.begin(), subTree.end(), isCurve);
    std::vector<ModelObject> noCurvesSubTree(subTree.begin(),end);

    for (const ModelObject& object : noCurvesSubTree) {
      result.push_back(object.idfObject());
    }

    bool ok = model().removeObjects(getHandles<ModelObject>(noCurvesSubTree));
    if (!ok) { result.clear(); }

    return result;
  }

  /// get a vector of allowable children types
  std::vector<IddObjectType> ParentObject_Impl::allowableChildTypes() const
  {
    IddObjectTypeVector result;
    return result;
  }

  ModelObject ParentObject_Impl::clone(Model model) const
  {
    ModelObject newParentAsModelObject = ModelObject_Impl::clone(model);
    ParentObject newParent = newParentAsModelObject.cast<ParentObject>();
    for (ModelObject child : children())
    {
      ModelObject newChild = child.clone(model);
      newChild.setParent(newParent);
    }
    return newParentAsModelObject;
  }

} // detail

ParentObject::ParentObject(IddObjectType type,const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::ParentObject_Impl>());
}

ParentObject::ParentObject(std::shared_ptr<detail::ParentObject_Impl> p)
  : ModelObject(std::move(p))
{  }

/// return any children objects in the hierarchy
ModelObjectVector ParentObject::children() const
{
  return getImpl<detail::ParentObject_Impl>()->children();
}

/// get a vector of allowable children types
std::vector<IddObjectType> ParentObject::allowableChildTypes() const
{
  return getImpl<detail::ParentObject_Impl>()->allowableChildTypes();
}

std::vector<ModelObject> getRecursiveChildren(const ParentObject& object, bool includeLifeCycleCosts) {
  std::set<Handle> resultSet;
  std::pair<HandleSet::const_iterator,bool> insertResult;
  std::vector<ModelObject> result;
  resultSet.insert(object.handle());
  result.push_back(object);

  if (includeLifeCycleCosts){
    for (const LifeCycleCost& lifeCycleCost : object.lifeCycleCosts()){
      result.push_back(lifeCycleCost);
    }
  }

  std::deque<ParentObject> parents;
  parents.push_back(object);

  while (parents.size() > 0) {
    ParentObject currentParent(parents[0]);
    parents.pop_front();

    // parent's costs have already been added

    for (const ModelObject& child : currentParent.children()) {
      insertResult = resultSet.insert(child.handle());
      if (insertResult.second) {
        result.push_back(child);

        if (includeLifeCycleCosts){
          for (const LifeCycleCost& lifeCycleCost : child.lifeCycleCosts()){
            result.push_back(lifeCycleCost);
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
  std::pair<HandleSet::const_iterator,bool> insertResult;
  std::vector<ModelObject> result;
  resultSet.insert(object.handle());
  result.push_back(object);

  std::deque<ModelObject> objectQueue;
  objectQueue.push_back(object);

  while (objectQueue.size() > 0) {
    ModelObject currentObject(objectQueue[0]);
    objectQueue.pop_front();
    // resources
    for (const ResourceObject& resource : currentObject.resources()) {
      insertResult = resultSet.insert(resource.handle());
      if (insertResult.second) {
        // new object
        ModelObject mo = resource.cast<ModelObject>();
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

} // model
} // openstudio
