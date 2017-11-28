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

  ModelObjectList_Impl::ModelObjectList_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ModelObjectList::iddObjectType());
  }

  ModelObjectList_Impl::ModelObjectList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ModelObjectList::iddObjectType());
  }

  ModelObjectList_Impl::ModelObjectList_Impl(const ModelObjectList_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ModelObjectList_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ModelObjectList_Impl::iddObjectType() const {
    return ModelObjectList::iddObjectType();
  }

  std::vector<IdfObject> ModelObjectList_Impl::remove()
  {
    std::vector<IdfObject> result;

    std::vector<ModelObject> modelObjects = this->modelObjects();

    if ( !modelObjects.empty() ){
      for( auto & modelObject : modelObjects )
      {
        std::vector<IdfObject> removedModelObject = modelObject.remove();
        result.insert(result.end(), removedModelObject.begin(), removedModelObject.end());
      }
    }

    std::vector<IdfObject> removedModelObjectList = ModelObject_Impl::remove();
    result.insert(result.end(), removedModelObjectList.begin(), removedModelObjectList.end());

    return result;
  }

  ModelObject ModelObjectList_Impl::clone(Model model) const
  {
    ModelObjectList modelObjectListClone = ModelObject_Impl::clone(model).cast<ModelObjectList>();

    std::vector<ModelObject> modelObjects = this->modelObjects();

    if ( !modelObjects.empty() ){
      modelObjectListClone.removeAllModelObjects();
      for( const auto & modelObject : modelObjects )
      {
        ModelObject modelObjectClone = modelObject.clone(model);
        modelObjectListClone.addModelObject(modelObjectClone);
      }
    }

    return modelObjectListClone;
  }

  std::vector<ModelObject> ModelObjectList_Impl::modelObjects() const {
    std::vector<ModelObject> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( const auto & group : groups )
    {
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

      if( wo )
      {
        result.push_back(wo->cast<ModelObject>());
      }
    }

    return result;
  }

  unsigned ModelObjectList_Impl::size() const
  {
    return extensibleGroups().size();
  }

  bool ModelObjectList_Impl::addModelObject(const ModelObject& modelObject ) {

    // DLM: should this class prevent duplicates in the list?

    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool ok = eg.setPointer(OS_ModelObjectListExtensibleFields::ModelObject,modelObject.handle());

    if( !ok ) {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
    }
    return ok;
  }

  bool ModelObjectList_Impl::hasModelObject(const ModelObject & modelObject) const
  {
    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups)
    {
      // DLM: if an object was pointed to by this list but that object was deleted from the model, this list may have an empty field
      // in that case getTarget may fail to return an object
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

      OS_ASSERT(wo);

      if (wo->handle() == modelObject.handle())
      {
        return true;
      }
    }

    return false;
  }

  void ModelObjectList_Impl::removeModelObject(const ModelObject& modelObject ) {
    if (hasModelObject(modelObject)){
      WorkspaceExtensibleGroup eg = getGroupForModelObject(modelObject);
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
    }
  }

  void ModelObjectList_Impl::removeAllModelObjects() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  WorkspaceExtensibleGroup ModelObjectList_Impl::getGroupForModelObject(const ModelObject& modelObject)
  {
    boost::optional<WorkspaceExtensibleGroup> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( const auto & group : groups )
    {
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

      OS_ASSERT(wo);

      if( wo->handle() == modelObject.handle() )
      {
        result = group.cast<WorkspaceExtensibleGroup>();

        break;
      }
    }

    OS_ASSERT(result);

    return result.get();
  }

} // detail

ModelObjectList::ModelObjectList(const Model& model)
  : ModelObject(ModelObjectList::iddObjectType(),model,true)
{
  OS_ASSERT(getImpl<detail::ModelObjectList_Impl>());
}

IddObjectType ModelObjectList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ModelObjectList);
}

std::vector<IdfObject> ModelObjectList::remove() {
  return getImpl<detail::ModelObjectList_Impl>()->remove();
}

ModelObject ModelObjectList::clone(Model model) const {
  return getImpl<detail::ModelObjectList_Impl>()->clone(model);
}

std::vector<ModelObject> ModelObjectList::modelObjects() const
{
  return getImpl<detail::ModelObjectList_Impl>()->modelObjects();
}

unsigned ModelObjectList::size() const {
  return getImpl<detail::ModelObjectList_Impl>()->size();
}

bool ModelObjectList::addModelObject(const ModelObject& modelObject ) {
  return getImpl<detail::ModelObjectList_Impl>()->addModelObject(modelObject);
}

bool ModelObjectList::hasModelObject(const ModelObject & modelObject) const{
  return getImpl<detail::ModelObjectList_Impl>()->hasModelObject(modelObject);
}

void ModelObjectList::removeModelObject(const ModelObject& modelObject ) {
  getImpl<detail::ModelObjectList_Impl>()->removeModelObject(modelObject);
}

void ModelObjectList::removeAllModelObjects() {
  getImpl<detail::ModelObjectList_Impl>()->removeAllModelObjects();
}

/// @cond
ModelObjectList::ModelObjectList(std::shared_ptr<detail::ModelObjectList_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

