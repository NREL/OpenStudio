/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "DefaultSurfaceConstructions.hpp"
#include "DefaultSurfaceConstructions_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"

#include <utilities/idd/OS_DefaultSurfaceConstructions_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DefaultSurfaceConstructions_Impl::DefaultSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DefaultSurfaceConstructions::iddObjectType());
  }

  DefaultSurfaceConstructions_Impl::DefaultSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DefaultSurfaceConstructions::iddObjectType());
  }

  DefaultSurfaceConstructions_Impl::DefaultSurfaceConstructions_Impl(const DefaultSurfaceConstructions_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DefaultSurfaceConstructions_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DefaultSurfaceConstructions_Impl::iddObjectType() const {
    return DefaultSurfaceConstructions::iddObjectType();
  }

  boost::optional<ConstructionBase> DefaultSurfaceConstructions_Impl::floorConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSurfaceConstructionsFields::FloorConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSurfaceConstructions_Impl::wallConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSurfaceConstructionsFields::WallConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSurfaceConstructions_Impl::roofCeilingConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSurfaceConstructionsFields::RoofCeilingConstructionName);
  }

  bool DefaultSurfaceConstructions_Impl::setFloorConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_DefaultSurfaceConstructionsFields::FloorConstructionName, construction.handle());
  }

  void DefaultSurfaceConstructions_Impl::resetFloorConstruction()
  {
    setString(OS_DefaultSurfaceConstructionsFields::FloorConstructionName, "");
  }

  bool DefaultSurfaceConstructions_Impl::setWallConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_DefaultSurfaceConstructionsFields::WallConstructionName, construction.handle());
  }

  void DefaultSurfaceConstructions_Impl::resetWallConstruction()
  {
    setString(OS_DefaultSurfaceConstructionsFields::WallConstructionName, "");
  }

  bool DefaultSurfaceConstructions_Impl::setRoofCeilingConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_DefaultSurfaceConstructionsFields::RoofCeilingConstructionName, construction.handle());
  }

  void DefaultSurfaceConstructions_Impl::resetRoofCeilingConstruction()
  {
    setString(OS_DefaultSurfaceConstructionsFields::RoofCeilingConstructionName, "");
  }

  void DefaultSurfaceConstructions_Impl::merge(const DefaultSurfaceConstructions& other)
  {
    boost::optional<ConstructionBase> construction;

    if (!(this->floorConstruction())){
      construction = other.floorConstruction();
      if (construction){
        this->setFloorConstruction(*construction);
      }
    }

    if (!(this->wallConstruction())){
      construction = other.wallConstruction();
      if (construction){
        this->setWallConstruction(*construction);
      }
    }

    if (!(this->roofCeilingConstruction())){
      construction = other.roofCeilingConstruction();
      if (construction){
        this->setRoofCeilingConstruction(*construction);
      }
    }
  }

  boost::optional<ModelObject> DefaultSurfaceConstructions_Impl::floorConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = floorConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSurfaceConstructions_Impl::wallConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = wallConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSurfaceConstructions_Impl::roofCeilingConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = roofCeilingConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool DefaultSurfaceConstructions_Impl::setFloorConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setFloorConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetFloorConstruction();
    }
    return true;
  }

  bool DefaultSurfaceConstructions_Impl::setWallConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setWallConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetWallConstruction();
    }
    return true;
  }

  bool DefaultSurfaceConstructions_Impl::setRoofCeilingConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setRoofCeilingConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetRoofCeilingConstruction();
    }
    return true;
  }

} // detail

DefaultSurfaceConstructions::DefaultSurfaceConstructions(const Model& model)
  : ResourceObject(DefaultSurfaceConstructions::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DefaultSurfaceConstructions_Impl>());
}

IddObjectType DefaultSurfaceConstructions::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DefaultSurfaceConstructions);
  return result;
}

boost::optional<ConstructionBase> DefaultSurfaceConstructions::floorConstruction() const{
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->floorConstruction();
}

boost::optional<ConstructionBase> DefaultSurfaceConstructions::wallConstruction() const{
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->wallConstruction();
}

boost::optional<ConstructionBase> DefaultSurfaceConstructions::roofCeilingConstruction() const{
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->roofCeilingConstruction();
}

bool DefaultSurfaceConstructions::setFloorConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->setFloorConstruction(construction);
}

void DefaultSurfaceConstructions::resetFloorConstruction(){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->resetFloorConstruction();
}

bool DefaultSurfaceConstructions::setWallConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->setWallConstruction(construction);
}

void DefaultSurfaceConstructions::resetWallConstruction(){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->resetWallConstruction();
}

bool DefaultSurfaceConstructions::setRoofCeilingConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->setRoofCeilingConstruction(construction);
}

void DefaultSurfaceConstructions::resetRoofCeilingConstruction(){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->resetRoofCeilingConstruction();
}

void DefaultSurfaceConstructions::merge(const DefaultSurfaceConstructions& other){
  return getImpl<detail::DefaultSurfaceConstructions_Impl>()->merge(other);
}

/// @cond
DefaultSurfaceConstructions::DefaultSurfaceConstructions(std::shared_ptr<detail::DefaultSurfaceConstructions_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond


} // model
} // openstudio

