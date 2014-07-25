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

#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ResourceObject_Impl::ResourceObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {}

  ResourceObject_Impl::ResourceObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                           Model_Impl* model, 
                                           bool keepHandle)
    : ParentObject_Impl(other, model, keepHandle)
  {}
  
  ResourceObject_Impl::ResourceObject_Impl(const ResourceObject_Impl& other, 
                                           Model_Impl* model,
                                           bool keepHandle)
    : ParentObject_Impl(other, model, keepHandle)
  {}

  unsigned ResourceObject_Impl::directUseCount() const {
    return numSources();
  }

  unsigned ResourceObject_Impl::nonResourceObjectUseCount() const {
   
    unsigned result = 0;

    WorkspaceObjectVector sources = this->sources();
    for (const WorkspaceObject& source : sources) {
      OptionalResourceObject oro = source.optionalCast<ResourceObject>();
      if (oro) {
        // another ResourceObject, pass the request through
        result += oro->nonResourceObjectUseCount();
      }
      else {
        // non-ResourceObject--count the use
        ++result;
      }
    }

    return result;
  }

} // detail

ResourceObject::ResourceObject(IddObjectType type,const Model& model)
  : ParentObject(type,model) 
{
  OS_ASSERT(getImpl<detail::ResourceObject_Impl>());
}

ResourceObject::ResourceObject(std::shared_ptr<detail::ResourceObject_Impl> impl)
  : ParentObject(impl)
{}

unsigned ResourceObject::directUseCount() const {
  return getImpl<detail::ResourceObject_Impl>()->directUseCount();
}

unsigned ResourceObject::nonResourceObjectUseCount() const {
  return getImpl<detail::ResourceObject_Impl>()->nonResourceObjectUseCount();
}

std::vector<ResourceObject> getRecursiveResources(const ModelObject& object) {
  std::set<Handle> resultSet;
  std::pair<HandleSet::const_iterator,bool> insertResult;
  std::vector<ResourceObject> result;

  std::deque<ModelObject> objectQueue;
  objectQueue.push_back(object);

  while (objectQueue.size() > 0) {
    ModelObject currentObject(objectQueue[0]);
    objectQueue.pop_front();
    for (const ResourceObject resource : currentObject.resources()) {
      insertResult = resultSet.insert(resource.handle());
      if (insertResult.second) {
        result.push_back(resource);
        objectQueue.push_back(resource.cast<ModelObject>());
      }
    }
  }

  return result;
}

std::vector< std::vector<ModelObject> > getRecursiveResourceSubTrees(const ModelObject& object, bool includeComponentCostLineItems) {
  std::set<Handle> resultSet;
  std::pair<HandleSet::const_iterator,bool> insertResult;
  std::vector< std::vector<ModelObject> > result;

  std::deque<ModelObject> objectQueue;
  objectQueue.push_back(object);

  while (objectQueue.size() > 0) {
    ModelObject currentObject(objectQueue[0]);
    objectQueue.pop_front();
    for (const ResourceObject resource : currentObject.resources()) {
      insertResult = resultSet.insert(resource.handle());
      if (insertResult.second) {
        std::vector<ModelObject> subTree = getRecursiveChildren(resource, includeComponentCostLineItems);
        objectQueue.insert(objectQueue.end(),subTree.begin(),subTree.end());
        result.push_back(subTree);
      }
    }
  }

  return result;
}

} // model
} // openstudio
