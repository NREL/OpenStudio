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

  unsigned ResourceObject_Impl::directUseCount(bool excludeChildren) const {

    unsigned result = 0;

    ModelObjectVector children;
    if (excludeChildren){
      children = this->children();
    }

    WorkspaceObjectVector sources = this->sources();
    for (const WorkspaceObject& source : sources) {
      if (excludeChildren){
        // check if this is a child
        auto it = std::find_if(children.begin(), children.end(),
                               std::bind(handleEquals<ModelObject, Handle>, std::placeholders::_1, source.handle()));
        if (it == children.end()){
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
    if (excludeChildren){
      children = this->children();
    }

    WorkspaceObjectVector sources = this->sources();
    for (const WorkspaceObject& source : sources) {
      OptionalResourceObject oro = source.optionalCast<ResourceObject>();
      if (oro) {
        // another ResourceObject, pass the request through
        result += oro->nonResourceObjectUseCount(excludeChildren);
      }
      else {

        if (excludeChildren){
          // check if this is a child
          auto it = std::find_if(children.begin(), children.end(),
                                 std::bind(handleEquals<ModelObject, Handle>, std::placeholders::_1, source.handle()));
          if (it == children.end()){
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

} // detail

ResourceObject::ResourceObject(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::ResourceObject_Impl>());
}

ResourceObject::ResourceObject(std::shared_ptr<detail::ResourceObject_Impl> impl)
  : ParentObject(std::move(impl))
{}

unsigned ResourceObject::directUseCount(bool excludeChildren) const {
  return getImpl<detail::ResourceObject_Impl>()->directUseCount(excludeChildren);
}

unsigned ResourceObject::nonResourceObjectUseCount(bool excludeChildren) const {
  return getImpl<detail::ResourceObject_Impl>()->nonResourceObjectUseCount(excludeChildren);
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
