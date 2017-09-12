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
  : ResourceObject(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

