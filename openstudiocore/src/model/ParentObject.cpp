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

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "ResourceObject.hpp"
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
    ModelObjectVector subTree = getRecursiveChildren(getObject<ParentObject>(), true);
    for (const ModelObject& object : subTree) {
      result.push_back(object.idfObject());
    }
      
    bool ok = model().removeObjects(getHandles<ModelObject>(subTree));
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
  : ModelObject(p)
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
