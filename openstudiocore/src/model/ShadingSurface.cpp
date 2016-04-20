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

#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "DaylightingDeviceShelf.hpp"
#include "DaylightingDeviceShelf_Impl.hpp"

#include <utilities/idd/OS_ShadingSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ShadingSurface_Impl::ShadingSurface_Impl(const IdfObject& idfObject,
                                           Model_Impl* model, 
                                           bool keepHandle)
    : PlanarSurface_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ShadingSurface::iddObjectType());
  }

  ShadingSurface_Impl::ShadingSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ShadingSurface::iddObjectType());
  }

  ShadingSurface_Impl::ShadingSurface_Impl(const ShadingSurface_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> ShadingSurface_Impl::parent() const
  {
    boost::optional<ParentObject> result;
    result = this->shadingSurfaceGroup();
    return result;
  }

  std::vector<ModelObject> ShadingSurface_Impl::children() const
  {
    std::vector<ModelObject> result;

    // solar collectors?

    return result;
  }

  bool ShadingSurface_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    OptionalShadingSurfaceGroup shadingSurfaceGroup = newParent.optionalCast<ShadingSurfaceGroup>();
    if (shadingSurfaceGroup){
      result = this->setShadingSurfaceGroup(*shadingSurfaceGroup);
    }
    return result;
  }

  const std::vector<std::string>& ShadingSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ShadingSurface_Impl::iddObjectType() const {
    return ShadingSurface::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ShadingSurface_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ShadingSurfaceFields::TransmittanceScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ShadingSurface","Transmittance"));
    }
    return result;
  }

  bool ShadingSurface_Impl::subtractFromGrossArea() const
  {
    return false;
  }

  boost::optional<std::pair<ConstructionBase, int> > ShadingSurface_Impl::constructionWithSearchDistance() const
  {
    boost::optional<std::pair<ConstructionBase, int> > result;

    boost::optional<ConstructionBase> construction = getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_ShadingSurfaceFields::ConstructionName);
    if (construction){
      return std::make_pair(*construction, 0);
    }

    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup = this->shadingSurfaceGroup();
    if (shadingSurfaceGroup){

      // check the space
      boost::optional<Space> space = shadingSurfaceGroup->space();
      if (space){
        result = space->getDefaultConstructionWithSearchDistance(this->getObject<ShadingSurface>());
        if (result){
          return result;
        }
      }

      // check the building
      boost::optional<Building> building = this->model().building();
      if (building){

        boost::optional<DefaultConstructionSet> defaultConstructionSet = building->defaultConstructionSet();
        if (defaultConstructionSet){
          construction = defaultConstructionSet->getDefaultConstruction(this->getObject<ShadingSurface>());
          if (construction){
            return std::make_pair(*construction, 4);
          }
        }

        // check the building's space type
        boost::optional<SpaceType> spaceType = building->spaceType();
        if (spaceType){
          defaultConstructionSet = spaceType->defaultConstructionSet();
          if (defaultConstructionSet){
            construction = defaultConstructionSet->getDefaultConstruction(this->getObject<ShadingSurface>());
            if (construction){
              return std::make_pair(*construction, 5);
            }
          }
        }
      }
    }

    return result;
  }

  bool ShadingSurface_Impl::isConstructionDefaulted() const
  {
    return isEmpty(OS_ShadingSurfaceFields::ConstructionName);
  }

  bool ShadingSurface_Impl::setConstruction(const ConstructionBase& construction)
  {
    return this->setPointer(OS_ShadingSurfaceFields::ConstructionName, construction.handle());
  }

  void ShadingSurface_Impl::resetConstruction()
  {
    setString(OS_ShadingSurfaceFields::ConstructionName, "");
  }

  boost::optional<PlanarSurfaceGroup> ShadingSurface_Impl::planarSurfaceGroup() const
  {
    return boost::optional<PlanarSurfaceGroup>(this->shadingSurfaceGroup());
  }

  boost::optional<Space> ShadingSurface_Impl::space() const
  { 
    boost::optional<Space> result;
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup = this->shadingSurfaceGroup();
    if (shadingSurfaceGroup){
      result = shadingSurfaceGroup->space();
    }
    return result;
  }

  boost::optional<double> ShadingSurface_Impl::numberofVertices() const {
    return getDouble(OS_ShadingSurfaceFields::NumberofVertices,true);
  }

  bool ShadingSurface_Impl::isNumberofVerticesDefaulted() const {
    return isEmpty(OS_ShadingSurfaceFields::NumberofVertices);
  }

  bool ShadingSurface_Impl::isNumberofVerticesAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ShadingSurfaceFields::NumberofVertices, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  bool ShadingSurface_Impl::setNumberofVertices(boost::optional<double> numberofVertices) {
    bool result = false;
    if (numberofVertices) {
      result = setDouble(OS_ShadingSurfaceFields::NumberofVertices, numberofVertices.get());
    } else {
      result = setString(OS_ShadingSurfaceFields::NumberofVertices, "");
    }
    return result;
  }

  bool ShadingSurface_Impl::setNumberofVertices(double numberofVertices) {
    bool result = setDouble(OS_ShadingSurfaceFields::NumberofVertices, numberofVertices);
    return result;
  }

  void ShadingSurface_Impl::resetNumberofVertices() {
    bool result = setString(OS_ShadingSurfaceFields::NumberofVertices, "");
    OS_ASSERT(result);
  }

  void ShadingSurface_Impl::autocalculateNumberofVertices() {
    bool result = setString(OS_ShadingSurfaceFields::NumberofVertices, "Autocalculate");
    OS_ASSERT(result);
  }

  boost::optional<ShadingSurfaceGroup> ShadingSurface_Impl::shadingSurfaceGroup() const
  {
    boost::optional<ShadingSurfaceGroup> result;
    OptionalWorkspaceObject object = this->getTarget(OS_ShadingSurfaceFields::ShadingSurfaceGroupName);
    if (object){
      result = object->optionalCast<ShadingSurfaceGroup>();
    }
    return result;
  }

  bool ShadingSurface_Impl::setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup)
  {
    return  this->setPointer(OS_ShadingSurfaceFields::ShadingSurfaceGroupName, shadingSurfaceGroup.handle());
  }

  void ShadingSurface_Impl::resetShadingSurfaceGroup() {
    bool ok = setString(OS_ShadingSurfaceFields::ShadingSurfaceGroupName, "");
    OS_ASSERT(ok);
  }

  boost::optional<Schedule> ShadingSurface_Impl::transmittanceSchedule() const
  {
    boost::optional<Schedule> result;
    OptionalWorkspaceObject object = this->getTarget(OS_ShadingSurfaceFields::TransmittanceScheduleName);
    if (object){
      result = object->optionalCast<Schedule>();
    }
    return result;
  }

  bool ShadingSurface_Impl::setTransmittanceSchedule(Schedule& transmittanceSchedule)
  {
    bool result = setSchedule(OS_ShadingSurfaceFields::TransmittanceScheduleName,
                              "ShadingSurface",
                              "Transmittance",
                              transmittanceSchedule);
    return result;
  }

  void ShadingSurface_Impl::resetTransmittanceSchedule()
  {
    bool test = this->setString(OS_ShadingSurfaceFields::TransmittanceScheduleName, "");
    OS_ASSERT(test);
  }

  boost::optional<DaylightingDeviceShelf> ShadingSurface_Impl::daylightingDeviceShelf() const
  {
    std::vector<DaylightingDeviceShelf> sources = getObject<ModelObject>().getModelObjectSources<DaylightingDeviceShelf>();
    if (sources.empty()){
      return boost::none;
    }
    if (sources.size() > 1){
      LOG(Error, "ShadingSurface is referenced by more than one DaylightingDeviceShelf, returning first");
    }
    return sources[0];
  }

  boost::optional<ModelObject> ShadingSurface_Impl::shadingSurfaceGroupAsModelObject() const {
    OptionalModelObject result;
    OptionalShadingSurfaceGroup intermediate = shadingSurfaceGroup();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ShadingSurface_Impl::transmittanceScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = transmittanceSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ShadingSurface_Impl::setShadingSurfaceGroupAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalShadingSurfaceGroup intermediate = modelObject->optionalCast<ShadingSurfaceGroup>();
      if (intermediate) {
        return setShadingSurfaceGroup(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetShadingSurfaceGroup();
    }
    return true;
  }

  bool ShadingSurface_Impl::setTransmittanceScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setTransmittanceSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetTransmittanceSchedule();
    }
    return true;
  }

} // detail

ShadingSurface::ShadingSurface(const std::vector<Point3d>& vertices, const Model& model)
  : PlanarSurface(ShadingSurface::iddObjectType(),vertices,model)
{
  OS_ASSERT(getImpl<detail::ShadingSurface_Impl>());
}

IddObjectType ShadingSurface::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ShadingSurface);
  return result;
}

boost::optional<double> ShadingSurface::numberofVertices() const {
  return getImpl<detail::ShadingSurface_Impl>()->numberofVertices();
}

bool ShadingSurface::isNumberofVerticesDefaulted() const {
  return getImpl<detail::ShadingSurface_Impl>()->isNumberofVerticesDefaulted();
}

bool ShadingSurface::isNumberofVerticesAutocalculated() const {
  return getImpl<detail::ShadingSurface_Impl>()->isNumberofVerticesAutocalculated();
}

bool ShadingSurface::setNumberofVertices(boost::optional<double> numberofVertices) {
  return getImpl<detail::ShadingSurface_Impl>()->setNumberofVertices(numberofVertices);
}

bool ShadingSurface::setNumberofVertices(double numberofVertices) {
  return getImpl<detail::ShadingSurface_Impl>()->setNumberofVertices(numberofVertices);
}

void ShadingSurface::resetNumberofVertices() {
  getImpl<detail::ShadingSurface_Impl>()->resetNumberofVertices();
}

void ShadingSurface::autocalculateNumberofVertices() {
  getImpl<detail::ShadingSurface_Impl>()->autocalculateNumberofVertices();
}

boost::optional<ShadingSurfaceGroup> ShadingSurface::shadingSurfaceGroup() const
{
  return getImpl<detail::ShadingSurface_Impl>()->shadingSurfaceGroup();
}

bool ShadingSurface::setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup)
{
  return getImpl<detail::ShadingSurface_Impl>()->setShadingSurfaceGroup(shadingSurfaceGroup);
}

void ShadingSurface::resetShadingSurfaceGroup() {
  getImpl<detail::ShadingSurface_Impl>()->resetShadingSurfaceGroup();
}

boost::optional<Schedule> ShadingSurface::transmittanceSchedule() const
{
  return getImpl<detail::ShadingSurface_Impl>()->transmittanceSchedule();
}

bool ShadingSurface::setTransmittanceSchedule(Schedule& transmittanceSchedule)
{
  return getImpl<detail::ShadingSurface_Impl>()->setTransmittanceSchedule(transmittanceSchedule);
}

void ShadingSurface::resetTransmittanceSchedule()
{
  getImpl<detail::ShadingSurface_Impl>()->resetTransmittanceSchedule();
}

boost::optional<DaylightingDeviceShelf> ShadingSurface::daylightingDeviceShelf() const
{
  return getImpl<detail::ShadingSurface_Impl>()->daylightingDeviceShelf();
}

/// @cond
ShadingSurface::ShadingSurface(std::shared_ptr<detail::ShadingSurface_Impl> impl)
  : PlanarSurface(impl)
{}
/// @endcond


} // model
} // openstudio

