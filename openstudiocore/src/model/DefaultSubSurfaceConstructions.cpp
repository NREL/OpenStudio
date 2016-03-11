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

#include "DefaultSubSurfaceConstructions.hpp"
#include "DefaultSubSurfaceConstructions_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"

#include <utilities/idd/OS_DefaultSubSurfaceConstructions_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DefaultSubSurfaceConstructions_Impl::DefaultSubSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DefaultSubSurfaceConstructions::iddObjectType());
  }

  DefaultSubSurfaceConstructions_Impl::DefaultSubSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DefaultSubSurfaceConstructions::iddObjectType());
  }

  DefaultSubSurfaceConstructions_Impl::DefaultSubSurfaceConstructions_Impl(const DefaultSubSurfaceConstructions_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DefaultSubSurfaceConstructions_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DefaultSubSurfaceConstructions_Impl::iddObjectType() const {
    return DefaultSubSurfaceConstructions::iddObjectType();
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::fixedWindowConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::FixedWindowConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::operableWindowConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::OperableWindowConstructionName);
  }
  
  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::doorConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::DoorConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::glassDoorConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::GlassDoorConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::overheadDoorConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::OverheadDoorConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::skylightConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::SkylightConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::tubularDaylightDomeConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDomeConstructionName);
  }

  boost::optional<ConstructionBase> DefaultSubSurfaceConstructions_Impl::tubularDaylightDiffuserConstruction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDiffuserConstructionName);
  }

  bool DefaultSubSurfaceConstructions_Impl::setFixedWindowConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::FixedWindowConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetFixedWindowConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::FixedWindowConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setOperableWindowConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::OperableWindowConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetOperableWindowConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::OperableWindowConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setDoorConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::DoorConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetDoorConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::DoorConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setGlassDoorConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::GlassDoorConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetGlassDoorConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::GlassDoorConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setOverheadDoorConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::OverheadDoorConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetOverheadDoorConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::OverheadDoorConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setSkylightConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::SkylightConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetSkylightConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::SkylightConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setTubularDaylightDomeConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDomeConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetTubularDaylightDomeConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDomeConstructionName, "");
  }

  bool DefaultSubSurfaceConstructions_Impl::setTubularDaylightDiffuserConstruction(const ConstructionBase& construction)  
  {
    return setPointer(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDiffuserConstructionName, construction.handle());
  }

  void DefaultSubSurfaceConstructions_Impl::resetTubularDaylightDiffuserConstruction()
  {
    setString(OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDiffuserConstructionName, "");
  }
    
  void DefaultSubSurfaceConstructions_Impl::merge(const DefaultSubSurfaceConstructions& other)
  {
    boost::optional<ConstructionBase> construction;
    
    if (!(this->fixedWindowConstruction())){
      construction = other.fixedWindowConstruction();
      if (construction){
        this->setFixedWindowConstruction(*construction);
      }
    }

    if (!(this->operableWindowConstruction())){
      construction = other.operableWindowConstruction();
      if (construction){
        this->setOperableWindowConstruction(*construction);
      }
    }

    if (!(this->doorConstruction())){
      construction = other.doorConstruction();
      if (construction){
        this->setDoorConstruction(*construction);
      }
    }

    if (!(this->glassDoorConstruction())){
      construction = other.glassDoorConstruction();
      if (construction){
        this->setGlassDoorConstruction(*construction);
      }
    }

    if (!(this->overheadDoorConstruction())){
      construction = other.overheadDoorConstruction();
      if (construction){
        this->setOverheadDoorConstruction(*construction);
      }
    }

    if (!(this->skylightConstruction())){
      construction = other.skylightConstruction();
      if (construction){
        this->setSkylightConstruction(*construction);
      }
    }

    if (!(this->tubularDaylightDomeConstruction())){
      construction = other.tubularDaylightDomeConstruction();
      if (construction){
        this->setTubularDaylightDomeConstruction(*construction);
      }
    }
        
    if (!(this->tubularDaylightDiffuserConstruction())){
      construction = other.tubularDaylightDiffuserConstruction();
      if (construction){
        this->setTubularDaylightDiffuserConstruction(*construction);
      }
    }
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::fixedWindowConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = fixedWindowConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::operableWindowConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = operableWindowConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::doorConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = doorConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::glassDoorConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = glassDoorConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::overheadDoorConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = overheadDoorConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::skylightConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = skylightConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::tubularDaylightDomeConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = tubularDaylightDomeConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultSubSurfaceConstructions_Impl::tubularDaylightDiffuserConstructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = tubularDaylightDiffuserConstruction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool DefaultSubSurfaceConstructions_Impl::setFixedWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setFixedWindowConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetFixedWindowConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setOperableWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setOperableWindowConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetOperableWindowConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setDoorConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetDoorConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setGlassDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setGlassDoorConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetGlassDoorConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setOverheadDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setOverheadDoorConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetOverheadDoorConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setSkylightConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setSkylightConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSkylightConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setTubularDaylightDomeConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setTubularDaylightDomeConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetTubularDaylightDomeConstruction();
    }
    return true;
  }

  bool DefaultSubSurfaceConstructions_Impl::setTubularDaylightDiffuserConstructionAsModelObject(boost::optional<ModelObject> modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setTubularDaylightDiffuserConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetTubularDaylightDiffuserConstruction();
    }
    return true;
  }

} // detail

DefaultSubSurfaceConstructions::DefaultSubSurfaceConstructions(const Model& model)
  : ResourceObject(DefaultSubSurfaceConstructions::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DefaultSubSurfaceConstructions_Impl>());
}

IddObjectType DefaultSubSurfaceConstructions::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DefaultSubSurfaceConstructions);
  return result;
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::fixedWindowConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->fixedWindowConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::operableWindowConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->operableWindowConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::doorConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->doorConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::glassDoorConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->glassDoorConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::overheadDoorConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->overheadDoorConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::skylightConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->skylightConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::tubularDaylightDomeConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->tubularDaylightDomeConstruction();
}

boost::optional<ConstructionBase> DefaultSubSurfaceConstructions::tubularDaylightDiffuserConstruction() const{
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->tubularDaylightDiffuserConstruction();
}

bool DefaultSubSurfaceConstructions::setFixedWindowConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setFixedWindowConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetFixedWindowConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetFixedWindowConstruction();
}

bool DefaultSubSurfaceConstructions::setOperableWindowConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setOperableWindowConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetOperableWindowConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetOperableWindowConstruction();
}

bool DefaultSubSurfaceConstructions::setDoorConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setDoorConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetDoorConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetDoorConstruction();
}

bool DefaultSubSurfaceConstructions::setGlassDoorConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setGlassDoorConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetGlassDoorConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetGlassDoorConstruction();
}

bool DefaultSubSurfaceConstructions::setOverheadDoorConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setOverheadDoorConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetOverheadDoorConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetOverheadDoorConstruction();
}

bool DefaultSubSurfaceConstructions::setSkylightConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setSkylightConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetSkylightConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetSkylightConstruction();
}

bool DefaultSubSurfaceConstructions::setTubularDaylightDomeConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setTubularDaylightDomeConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetTubularDaylightDomeConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetTubularDaylightDomeConstruction();
}

bool DefaultSubSurfaceConstructions::setTubularDaylightDiffuserConstruction(const ConstructionBase& construction){
  return getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->setTubularDaylightDiffuserConstruction(construction);
}

void DefaultSubSurfaceConstructions::resetTubularDaylightDiffuserConstruction(){
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->resetTubularDaylightDiffuserConstruction();
}
    
void DefaultSubSurfaceConstructions::merge(const DefaultSubSurfaceConstructions& other)
{
  getImpl<detail::DefaultSubSurfaceConstructions_Impl>()->merge(other);
}

/// @cond
DefaultSubSurfaceConstructions::DefaultSubSurfaceConstructions(std::shared_ptr<detail::DefaultSubSurfaceConstructions_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond


} // model
} // openstudio

