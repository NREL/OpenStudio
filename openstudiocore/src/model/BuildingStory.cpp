/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/RenderingColor.hpp>
#include <model/RenderingColor_Impl.hpp>

#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  BuildingStory_Impl::BuildingStory_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == BuildingStory::iddObjectType());
  }

  BuildingStory_Impl::BuildingStory_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == BuildingStory::iddObjectType());
  }

  BuildingStory_Impl::BuildingStory_Impl(const BuildingStory_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& BuildingStory_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType BuildingStory_Impl::iddObjectType() const {
    return BuildingStory::iddObjectType();
  }

  boost::optional<double> BuildingStory_Impl::nominalZCoordinate() const {
    return getDouble(OS_BuildingStoryFields::NominalZCoordinate,true);
  }
  
  OSOptionalQuantity BuildingStory_Impl::getNominalZCoordinate(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_BuildingStoryFields::NominalZCoordinate,true,returnIP);
    return value;
  }

  double BuildingStory_Impl::nominalFloortoFloorHeight() const {
    boost::optional<double> value = getDouble(OS_BuildingStoryFields::NominalFloortoFloorHeight,true);
    OS_ASSERT(value);
    return value.get();
  }
  
  Quantity BuildingStory_Impl::getNominalFloortoFloorHeight(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_BuildingStoryFields::NominalFloortoFloorHeight,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  bool BuildingStory_Impl::isNominalFloortoFloorHeightDefaulted() const {
    return isEmpty(OS_BuildingStoryFields::NominalFloortoFloorHeight);
  }

  void BuildingStory_Impl::setNominalZCoordinate(boost::optional<double> nominalZCoordinate) {
    bool result = false;
    if (nominalZCoordinate) {
      result = setDouble(OS_BuildingStoryFields::NominalZCoordinate, nominalZCoordinate.get());
    } else {
      result = setString(OS_BuildingStoryFields::NominalZCoordinate, "");
    }
    OS_ASSERT(result);
  }

  void BuildingStory_Impl::setNominalZCoordinate(double nominalZCoordinate) {
    bool result = false;
    result = setDouble(OS_BuildingStoryFields::NominalZCoordinate, nominalZCoordinate);
    OS_ASSERT(result);
  }
  
  bool BuildingStory_Impl::setNominalZCoordinate(const OSOptionalQuantity& nominalZCoordinate) {
    bool result;
    if (nominalZCoordinate.isSet()) {
      result = setQuantity(OS_BuildingStoryFields::NominalZCoordinate,nominalZCoordinate.get());
    } else {
      result = setString(OS_BuildingStoryFields::NominalZCoordinate, "");
    }
    return result;
  }

  void BuildingStory_Impl::resetNominalZCoordinate() {
    bool result = setString(OS_BuildingStoryFields::NominalZCoordinate, "");
    OS_ASSERT(result);
  }

  bool BuildingStory_Impl::setNominalFloortoFloorHeight(double nominalFloortoFloorHeight) {
    bool result = false;
    result = setDouble(OS_BuildingStoryFields::NominalFloortoFloorHeight, nominalFloortoFloorHeight);
    return result;
  }

  bool BuildingStory_Impl::setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight) {
    return setQuantity(OS_BuildingStoryFields::NominalFloortoFloorHeight,nominalFloortoFloorHeight);
  }

  void BuildingStory_Impl::resetNominalFloortoFloorHeight() {
    bool result = setString(OS_BuildingStoryFields::NominalFloortoFloorHeight, "");
    OS_ASSERT(result);
  }

  boost::optional<DefaultConstructionSet> BuildingStory_Impl::defaultConstructionSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultConstructionSet>(OS_BuildingStoryFields::DefaultConstructionSetName);
  }

  bool BuildingStory_Impl::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
  {
    return setPointer(OS_BuildingStoryFields::DefaultConstructionSetName, defaultConstructionSet.handle());
  }

  void BuildingStory_Impl::resetDefaultConstructionSet()
  {
    setString(OS_BuildingStoryFields::DefaultConstructionSetName, "");
  }

  boost::optional<DefaultScheduleSet> BuildingStory_Impl::defaultScheduleSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultScheduleSet>(OS_BuildingStoryFields::DefaultScheduleSetName);
  }

  bool BuildingStory_Impl::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
  {
    return setPointer(OS_BuildingStoryFields::DefaultScheduleSetName, defaultScheduleSet.handle());
  }

  void BuildingStory_Impl::resetDefaultScheduleSet()
  {
    setString(OS_BuildingStoryFields::DefaultScheduleSetName, "");
  }
 
  boost::optional<RenderingColor> BuildingStory_Impl::renderingColor() const
  {
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_BuildingStoryFields::GroupRenderingName);
  }

  bool BuildingStory_Impl::setRenderingColor(const RenderingColor& renderingColor)
  {
    return setPointer(OS_BuildingStoryFields::GroupRenderingName, renderingColor.handle());
  }

  void BuildingStory_Impl::resetRenderingColor()
  {
    setString(OS_BuildingStoryFields::GroupRenderingName, "");
  }

  std::vector<Space> BuildingStory_Impl::spaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<Space>(
        Space::iddObjectType());
  }
  
  openstudio::OSOptionalQuantity BuildingStory_Impl::nominalZCoordinate_SI() const {
    return getNominalZCoordinate(false);
  }

  openstudio::OSOptionalQuantity BuildingStory_Impl::nominalZCoordinate_IP() const {
    return getNominalZCoordinate(true);
  }

  openstudio::Quantity BuildingStory_Impl::nominalFloortoFloorHeight_SI() const {
    return getNominalFloortoFloorHeight(false);
  }

  openstudio::Quantity BuildingStory_Impl::nominalFloortoFloorHeight_IP() const {
    return getNominalFloortoFloorHeight(true);
  }  

  boost::optional<ModelObject> BuildingStory_Impl::defaultConstructionSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultConstructionSet object = defaultConstructionSet();
    if (object) {
      result = *object;
    }
    return result;
  }

  boost::optional<ModelObject> BuildingStory_Impl::defaultScheduleSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultScheduleSet object = defaultScheduleSet();
    if (object) {
      result = *object;
    }
    return result;
  }

  boost::optional<ModelObject> BuildingStory_Impl::renderingColorAsModelObject() const {
    OptionalModelObject result;
    OptionalRenderingColor object = renderingColor();
    if (object) {
      result = *object;
    }
    return result;
  }

  bool BuildingStory_Impl::setDefaultConstructionSetAsModelObject(
      boost::optional<ModelObject> modelObject)
  {
    if (modelObject) {
      OptionalDefaultConstructionSet object = modelObject->optionalCast<DefaultConstructionSet>();
      if (object) {
        return setDefaultConstructionSet(*object);
      }
      return false;
    }
    else {
      resetDefaultConstructionSet();
    }
    return true;
  }

  bool BuildingStory_Impl::setDefaultScheduleSetAsModelObject(
      boost::optional<ModelObject> modelObject)
  {
    if (modelObject) {
      OptionalDefaultScheduleSet object = modelObject->optionalCast<DefaultScheduleSet>();
      if (object) {
        return setDefaultScheduleSet(*object);
      }
      return false;
    }
    else {
      resetDefaultScheduleSet();
    }
    return true;
  }

  bool BuildingStory_Impl::setRenderingColorAsModelObject(boost::optional<ModelObject> modelObject)
  {
    if (modelObject) {
      OptionalRenderingColor object = modelObject->optionalCast<RenderingColor>();
      if (object) {
        return setRenderingColor(*object);
      }
      return false;
    }
    else {
      resetRenderingColor();
    }
    return true;
  }

} // detail

BuildingStory::BuildingStory(const Model& model)
  : ModelObject(BuildingStory::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::BuildingStory_Impl>());
}

IddObjectType BuildingStory::iddObjectType() {
  IddObjectType result(IddObjectType::OS_BuildingStory);
  return result;
}

boost::optional<double> BuildingStory::nominalZCoordinate() const {
  return getImpl<detail::BuildingStory_Impl>()->nominalZCoordinate();
}

OSOptionalQuantity BuildingStory::getNominalZCoordinate(bool returnIP) const {
  return getImpl<detail::BuildingStory_Impl>()->getNominalZCoordinate(returnIP);
}
double BuildingStory::nominalFloortoFloorHeight() const {
  return getImpl<detail::BuildingStory_Impl>()->nominalFloortoFloorHeight();
}

Quantity BuildingStory::getNominalFloortoFloorHeight(bool returnIP) const {
  return getImpl<detail::BuildingStory_Impl>()->getNominalFloortoFloorHeight(returnIP);
}

bool BuildingStory::isNominalFloortoFloorHeightDefaulted() const {
  return getImpl<detail::BuildingStory_Impl>()->isNominalFloortoFloorHeightDefaulted();
}

void BuildingStory::setNominalZCoordinate(boost::optional<double> nominalZCoordinate) {
  getImpl<detail::BuildingStory_Impl>()->setNominalZCoordinate(nominalZCoordinate);
}

void BuildingStory::setNominalZCoordinate(double nominalZCoordinate) {
  getImpl<detail::BuildingStory_Impl>()->setNominalZCoordinate(nominalZCoordinate);
}

bool BuildingStory::setNominalZCoordinate(const Quantity& nominalZCoordinate) {
  return getImpl<detail::BuildingStory_Impl>()->setNominalZCoordinate(nominalZCoordinate);
}

void BuildingStory::resetNominalZCoordinate() {
  getImpl<detail::BuildingStory_Impl>()->resetNominalZCoordinate();
}

bool BuildingStory::setNominalFloortoFloorHeight(double nominalFloortoFloorHeight) {
  return getImpl<detail::BuildingStory_Impl>()->setNominalFloortoFloorHeight(nominalFloortoFloorHeight);
}

bool BuildingStory::setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight) {
  return getImpl<detail::BuildingStory_Impl>()->setNominalFloortoFloorHeight(nominalFloortoFloorHeight);
}

void BuildingStory::resetNominalFloortoFloorHeight() {
  getImpl<detail::BuildingStory_Impl>()->resetNominalFloortoFloorHeight();
}

boost::optional<DefaultConstructionSet> BuildingStory::defaultConstructionSet() const
{
  return getImpl<detail::BuildingStory_Impl>()->defaultConstructionSet();
}

bool BuildingStory::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
{
  return getImpl<detail::BuildingStory_Impl>()->setDefaultConstructionSet(defaultConstructionSet);
}

void BuildingStory::resetDefaultConstructionSet()
{
  getImpl<detail::BuildingStory_Impl>()->resetDefaultConstructionSet();
}

boost::optional<DefaultScheduleSet> BuildingStory::defaultScheduleSet() const
{
  return getImpl<detail::BuildingStory_Impl>()->defaultScheduleSet();
}

bool BuildingStory::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
{
  return getImpl<detail::BuildingStory_Impl>()->setDefaultScheduleSet(defaultScheduleSet);
}

void BuildingStory::resetDefaultScheduleSet()
{
  getImpl<detail::BuildingStory_Impl>()->resetDefaultScheduleSet();
}

boost::optional<RenderingColor> BuildingStory::renderingColor() const
{
  return getImpl<detail::BuildingStory_Impl>()->renderingColor();
}

bool BuildingStory::setRenderingColor(const RenderingColor& renderingColor)
{
  return getImpl<detail::BuildingStory_Impl>()->setRenderingColor(renderingColor);
}

void BuildingStory::resetRenderingColor()
{
  getImpl<detail::BuildingStory_Impl>()->resetRenderingColor();
}

std::vector<Space> BuildingStory::spaces() const
{
  return getImpl<detail::BuildingStory_Impl>()->spaces();
}

/// @cond
BuildingStory::BuildingStory(boost::shared_ptr<detail::BuildingStory_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond


} // model
} // openstudio

