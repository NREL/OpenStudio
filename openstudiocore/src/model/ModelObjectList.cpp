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

#include <model/ModelObjectList.hpp>
#include <model/ModelObjectList_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/OS_ModelObjectList_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>

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
      for( std::vector<ModelObject>::iterator it = modelObjects.begin();
       it != modelObjects.end();
       ++it )
      {
        std::vector<IdfObject> removedModelObject = it->remove();
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
      for( std::vector<ModelObject>::iterator it = modelObjects.begin();
       it != modelObjects.end();
       ++it )
      {
        ModelObject modelObjectClone = it->clone(model);
        modelObjectListClone.addModelObject(modelObjectClone);
      }
    }

    return modelObjectListClone;
  }

  std::vector<ModelObject> ModelObjectList_Impl::modelObjects() const {
    std::vector<ModelObject> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
         it != groups.end();
         ++it )
    {
      boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

      if( wo )
      {
        result.push_back(wo->cast<ModelObject>());
      }
    }

    return result;
  }

  bool ModelObjectList_Impl::addModelObject(const ModelObject& modelObject ) {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool ok = eg.setPointer(OS_ModelObjectListExtensibleFields::ModelObject,modelObject.handle());

    if( !ok ) {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
    }
    return ok;
  }

  void ModelObjectList_Impl::removeModelObject(const ModelObject& modelObject ) {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(modelObject);
    getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
  }

  void ModelObjectList_Impl::removeAllModelObjects() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  WorkspaceExtensibleGroup ModelObjectList_Impl::getGroupForModelObject(const ModelObject& modelObject)
  {
    boost::optional<WorkspaceExtensibleGroup> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
         it != groups.end();
         ++it )
    {
      boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ModelObjectListExtensibleFields::ModelObject);

      OS_ASSERT(wo);

      if( wo->handle() == modelObject.handle() )
      {
        result = it->cast<WorkspaceExtensibleGroup>();

        break;
      }
    }

    OS_ASSERT(result);

    return result.get();
  }

} // detail

ModelObjectList::ModelObjectList(const Model& model)
  : ModelObject(ModelObjectList::iddObjectType(),model)
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

bool ModelObjectList::addModelObject(const ModelObject& modelObject ) {
  return getImpl<detail::ModelObjectList_Impl>()->addModelObject(modelObject);
}

void ModelObjectList::removeModelObject(const ModelObject& modelObject ) {
  getImpl<detail::ModelObjectList_Impl>()->removeModelObject(modelObject);
}

void ModelObjectList::removeAllModelObjects() {
  getImpl<detail::ModelObjectList_Impl>()->removeAllModelObjects();
}

/// @cond
ModelObjectList::ModelObjectList(boost::shared_ptr<detail::ModelObjectList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

