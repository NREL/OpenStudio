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

#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"

#include <utilities/idd/OS_InteriorPartitionSurfaceGroup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Vector3d.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/BoundingBox.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  InteriorPartitionSurfaceGroup_Impl::InteriorPartitionSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : PlanarSurfaceGroup_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == InteriorPartitionSurfaceGroup::iddObjectType());
  }

  InteriorPartitionSurfaceGroup_Impl::InteriorPartitionSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == InteriorPartitionSurfaceGroup::iddObjectType());
  }

  InteriorPartitionSurfaceGroup_Impl::InteriorPartitionSurfaceGroup_Impl(const InteriorPartitionSurfaceGroup_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> InteriorPartitionSurfaceGroup_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->space());
  }

  bool InteriorPartitionSurfaceGroup_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Space>()){
      result = this->setSpace(newParent.cast<Space>());
    }
    return result;
  }

  std::vector<ModelObject> InteriorPartitionSurfaceGroup_Impl::children() const
  {
    std::vector<ModelObject> result;

    std::vector<InteriorPartitionSurface> interiorPartitionSurfaces = this->interiorPartitionSurfaces();
    result.insert(result.end(), interiorPartitionSurfaces.begin(), interiorPartitionSurfaces.end());

    return result;
  }

  std::vector<IddObjectType> InteriorPartitionSurfaceGroup_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_InteriorPartitionSurface);
    return result;
  }

  const std::vector<std::string>& InteriorPartitionSurfaceGroup_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType InteriorPartitionSurfaceGroup_Impl::iddObjectType() const {
    return InteriorPartitionSurfaceGroup::iddObjectType();
  }

  Transformation InteriorPartitionSurfaceGroup_Impl::buildingTransformation() const
  {
    Transformation transformation;
    if (this->space()){
      transformation = this->space()->buildingTransformation() * this->transformation();
    }else{
      transformation = this->transformation();
    }
    return transformation;
  }

  bool InteriorPartitionSurfaceGroup_Impl::changeTransformation(const openstudio::Transformation& transformation) {
    Transformation oldTransformation = this->transformation();
    if (!setTransformation(transformation)){
      return false;
    }

    // go through and update all child points and transformations so that:
    // (Tnew^-1 * Told) * xold = childTransformation * xold = xnew

    Transformation childTransformation = transformation.inverse() * oldTransformation;

    for (InteriorPartitionSurface interiorPartitionSurface : this->interiorPartitionSurfaces()){
      bool test = interiorPartitionSurface.setVertices(childTransformation * interiorPartitionSurface.vertices());
      if(!test){
        LOG(Error, "Could not transform vertices for InteriorPartitionSurface '" << interiorPartitionSurface.name().get() << "'.");
      }
    }

    return true;
  }

  BoundingBox InteriorPartitionSurfaceGroup_Impl::boundingBox() const
  {
    BoundingBox result;
    for (InteriorPartitionSurface interiorPartitionSurface : this->interiorPartitionSurfaces()){
      result.addPoints(interiorPartitionSurface.vertices());
    }
    return result;
  }

  double InteriorPartitionSurfaceGroup_Impl::directionofRelativeNorth() const {
    boost::optional<double> value = getDouble(OS_InteriorPartitionSurfaceGroupFields::DirectionofRelativeNorth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InteriorPartitionSurfaceGroup_Impl::isDirectionofRelativeNorthDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceGroupFields::DirectionofRelativeNorth);
  }

  double InteriorPartitionSurfaceGroup_Impl::xOrigin() const {
    boost::optional<double> value = getDouble(OS_InteriorPartitionSurfaceGroupFields::XOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InteriorPartitionSurfaceGroup_Impl::isXOriginDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceGroupFields::XOrigin);
  }

  double InteriorPartitionSurfaceGroup_Impl::yOrigin() const {
    boost::optional<double> value = getDouble(OS_InteriorPartitionSurfaceGroupFields::YOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InteriorPartitionSurfaceGroup_Impl::isYOriginDefaulted() const {
    return (!getString(OS_InteriorPartitionSurfaceGroupFields::YOrigin, false));
  }

  double InteriorPartitionSurfaceGroup_Impl::zOrigin() const {
    boost::optional<double> value = getDouble(OS_InteriorPartitionSurfaceGroupFields::ZOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InteriorPartitionSurfaceGroup_Impl::isZOriginDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceGroupFields::ZOrigin);
  }

  int InteriorPartitionSurfaceGroup_Impl::multiplier() const {
    boost::optional<int> value = getInt(OS_InteriorPartitionSurfaceGroupFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InteriorPartitionSurfaceGroup_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceGroupFields::Multiplier);
  }

  void InteriorPartitionSurfaceGroup_Impl::setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod) {
    bool result = setDouble(OS_InteriorPartitionSurfaceGroupFields::DirectionofRelativeNorth, directionofRelativeNorth, driverMethod);
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::resetDirectionofRelativeNorth() {
    bool result = setString(OS_InteriorPartitionSurfaceGroupFields::DirectionofRelativeNorth, "");
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::setXOrigin(double xOrigin, bool driverMethod) {
    bool result = setDouble(OS_InteriorPartitionSurfaceGroupFields::XOrigin, xOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::resetXOrigin() {
    bool result = setString(OS_InteriorPartitionSurfaceGroupFields::XOrigin, "");
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::setYOrigin(double yOrigin, bool driverMethod) {
    bool result = setDouble(OS_InteriorPartitionSurfaceGroupFields::YOrigin, yOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::resetYOrigin() {
    bool result = setString(OS_InteriorPartitionSurfaceGroupFields::YOrigin, "");
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::setZOrigin(double zOrigin, bool driverMethod) {
    bool result = setDouble(OS_InteriorPartitionSurfaceGroupFields::ZOrigin, zOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void InteriorPartitionSurfaceGroup_Impl::resetZOrigin() {
    bool result = setString(OS_InteriorPartitionSurfaceGroupFields::ZOrigin, "");
    OS_ASSERT(result);
  }

  bool InteriorPartitionSurfaceGroup_Impl::setMultiplier(int multiplier) {
    bool result = setInt(OS_InteriorPartitionSurfaceGroupFields::Multiplier, multiplier);
    return result;
  }

  void InteriorPartitionSurfaceGroup_Impl::resetMultiplier() {
    bool result = setString(OS_InteriorPartitionSurfaceGroupFields::Multiplier, "");
    OS_ASSERT(result);
  }

  boost::optional<Space> InteriorPartitionSurfaceGroup_Impl::space() const {
    boost::optional<Space> result;
    OptionalWorkspaceObject space = getTarget(OS_InteriorPartitionSurfaceGroupFields::SpaceName);
    if (space){
      result = space->optionalCast<Space>();
    }
    return result;
  }

  bool InteriorPartitionSurfaceGroup_Impl::setSpace(const Space& space) {
    return setPointer(OS_InteriorPartitionSurfaceGroupFields::SpaceName, space.handle());
  }

  void InteriorPartitionSurfaceGroup_Impl::resetSpace() {
    bool ok = setString(OS_InteriorPartitionSurfaceGroupFields::SpaceName, "");
    OS_ASSERT(ok);
  }

  InteriorPartitionSurfaceVector InteriorPartitionSurfaceGroup_Impl::interiorPartitionSurfaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<InteriorPartitionSurface>(InteriorPartitionSurface::iddObjectType());
  }

  boost::optional<ModelObject> InteriorPartitionSurfaceGroup_Impl::spaceAsModelObject() const {
    OptionalModelObject result;
    OptionalSpace intermediate = space();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> InteriorPartitionSurfaceGroup_Impl::interiorPartitionSurfacesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(interiorPartitionSurfaces());
    return result;
  }

  bool InteriorPartitionSurfaceGroup_Impl::setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSpace intermediate = modelObject->optionalCast<Space>();
      if (intermediate) {
        return setSpace(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSpace();
    }
    return true;
  }

} // detail

InteriorPartitionSurfaceGroup::InteriorPartitionSurfaceGroup(const Model& model)
  : PlanarSurfaceGroup(InteriorPartitionSurfaceGroup::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::InteriorPartitionSurfaceGroup_Impl>());
}

IddObjectType InteriorPartitionSurfaceGroup::iddObjectType() {
  IddObjectType result(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  return result;
}

int InteriorPartitionSurfaceGroup::multiplier() const {
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->multiplier();
}

bool InteriorPartitionSurfaceGroup::isMultiplierDefaulted() const {
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->isMultiplierDefaulted();
}

bool InteriorPartitionSurfaceGroup::setMultiplier(int multiplier) {
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->setMultiplier(multiplier);
}

void InteriorPartitionSurfaceGroup::resetMultiplier() {
  getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->resetMultiplier();
}

boost::optional<Space> InteriorPartitionSurfaceGroup::space() const {
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->space();
}

bool InteriorPartitionSurfaceGroup::setSpace(const Space& space) {
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->setSpace(space);
}

void InteriorPartitionSurfaceGroup::resetSpace() {
  getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->resetSpace();
}

InteriorPartitionSurfaceVector InteriorPartitionSurfaceGroup::interiorPartitionSurfaces() const
{
  return getImpl<detail::InteriorPartitionSurfaceGroup_Impl>()->interiorPartitionSurfaces();
}

/// @cond
InteriorPartitionSurfaceGroup::InteriorPartitionSurfaceGroup(std::shared_ptr<detail::InteriorPartitionSurfaceGroup_Impl> impl)
  : PlanarSurfaceGroup(impl)
{}
/// @endcond


} // model
} // openstudio

