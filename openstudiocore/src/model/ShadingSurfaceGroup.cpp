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

#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ShadingSurfaceGroup_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/BoundingBox.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ShadingSurfaceGroup_Impl::ShadingSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : PlanarSurfaceGroup_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ShadingSurfaceGroup::iddObjectType());
  }

  ShadingSurfaceGroup_Impl::ShadingSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ShadingSurfaceGroup::iddObjectType());
  }

  ShadingSurfaceGroup_Impl::ShadingSurfaceGroup_Impl(const ShadingSurfaceGroup_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> ShadingSurfaceGroup_Impl::parent() const
  {
    boost::optional<ParentObject> result;

    std::string shadingSurfaceType = this->shadingSurfaceType();
    if (istringEqual("Site", shadingSurfaceType)){
      result = this->model().getOptionalUniqueModelObject<Site>();
    }else if (istringEqual("Building", shadingSurfaceType)){
      result = this->model().building();
    }else if (istringEqual("Space", shadingSurfaceType)){
      result = this->space();
    }
    return result;
  }

  bool ShadingSurfaceGroup_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Site>()){
      result = this->setShadingSurfaceType("Site");
    }else if (newParent.optionalCast<Building>()){
      result = this->setShadingSurfaceType("Building");
    }else if (newParent.optionalCast<Space>()){
      result = this->setSpace(newParent.cast<Space>());
    }
    return result;
  }

  std::vector<ModelObject> ShadingSurfaceGroup_Impl::children() const
  {
    std::vector<ModelObject> result;

    std::vector<ShadingSurface> shadingSurfaces = this->shadingSurfaces();
    result.insert(result.end(), shadingSurfaces.begin(), shadingSurfaces.end());

    return result;
  }

  std::vector<IddObjectType> ShadingSurfaceGroup_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_ShadingSurface);
    return result;
  }

  const std::vector<std::string>& ShadingSurfaceGroup_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ShadingSurfaceGroup_Impl::iddObjectType() const {
    return ShadingSurfaceGroup::iddObjectType();
  }

  openstudio::Transformation ShadingSurfaceGroup_Impl::buildingTransformation() const
  {
    Transformation transformation;

    if (istringEqual("Space", this->shadingSurfaceType())){

      boost::optional<Space> space = this->space();
      if (space){
        transformation = space->transformation() * this->transformation();
      }else{
        transformation = this->transformation();
      }

    }else if (istringEqual("Building", this->shadingSurfaceType())){

      transformation = this->transformation();

    }else if (istringEqual("Site", this->shadingSurfaceType())){

      boost::optional<Building> building = this->model().building();
      if (building){
        transformation = building->transformation().inverse() * this->transformation();
      }else{
        transformation = this->transformation();
      }

    }

    return transformation;
  }

  bool ShadingSurfaceGroup_Impl::changeTransformation(const openstudio::Transformation& transformation)
  {
    Transformation oldTransformation = this->transformation();
    if (!setTransformation(transformation)){
      return false;
    }

    // go through and update all child points and transformations so that:
    // (Tnew^-1 * Told) * xold = childTransformation * xold = xnew

    Transformation childTransformation = transformation.inverse() * oldTransformation;

    BOOST_FOREACH(ShadingSurface shadingSurface, this->shadingSurfaces()){
      bool test = shadingSurface.setVertices(childTransformation * shadingSurface.vertices());
      if(!test){
        LOG(Error, "Could not transform vertices for ShadingSurface '" << shadingSurface.name().get() << "'.");
      }
    }

    return true;
  }

  openstudio::BoundingBox ShadingSurfaceGroup_Impl::boundingBox() const
  {
    BoundingBox result;
    BOOST_FOREACH(ShadingSurface shadingSurface, this->shadingSurfaces()){
      result.addPoints(shadingSurface.vertices());
    }
    return result;
  }

  std::string ShadingSurfaceGroup_Impl::shadingSurfaceType() const {
    boost::optional<std::string> value = getString(OS_ShadingSurfaceGroupFields::ShadingSurfaceType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ShadingSurfaceGroup_Impl::directionofRelativeNorth() const {
    boost::optional<double> value = getDouble(OS_ShadingSurfaceGroupFields::DirectionofRelativeNorth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadingSurfaceGroup_Impl::isDirectionofRelativeNorthDefaulted() const {
    return isEmpty(OS_ShadingSurfaceGroupFields::DirectionofRelativeNorth);
  }

  double ShadingSurfaceGroup_Impl::xOrigin() const {
    boost::optional<double> value = getDouble(OS_ShadingSurfaceGroupFields::XOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadingSurfaceGroup_Impl::isXOriginDefaulted() const {
    return isEmpty(OS_ShadingSurfaceGroupFields::XOrigin);
  }

  double ShadingSurfaceGroup_Impl::yOrigin() const {
    boost::optional<double> value = getDouble(OS_ShadingSurfaceGroupFields::YOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadingSurfaceGroup_Impl::isYOriginDefaulted() const {
    return isEmpty(OS_ShadingSurfaceGroupFields::YOrigin);
  }

  double ShadingSurfaceGroup_Impl::zOrigin() const {
    boost::optional<double> value = getDouble(OS_ShadingSurfaceGroupFields::ZOrigin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ShadingSurfaceGroup_Impl::isZOriginDefaulted() const {
    return isEmpty(OS_ShadingSurfaceGroupFields::ZOrigin);
  }

  bool ShadingSurfaceGroup_Impl::setShadingSurfaceType(std::string shadingSurfaceType) {
    bool result = false;

    if (istringEqual("Site", shadingSurfaceType)){
      result = setString(OS_ShadingSurfaceGroupFields::ShadingSurfaceType, shadingSurfaceType);
      bool test = setString(OS_ShadingSurfaceGroupFields::SpaceName, "");
      OS_ASSERT(test);
    }else if (istringEqual("Building", shadingSurfaceType)){
      result = setString(OS_ShadingSurfaceGroupFields::ShadingSurfaceType, shadingSurfaceType);
      bool test = setString(OS_ShadingSurfaceGroupFields::SpaceName, "");
      OS_ASSERT(test);
    }else if (istringEqual("Space", shadingSurfaceType)){
      result = this->space();
      if (result){
        bool test = setString(OS_ShadingSurfaceGroupFields::ShadingSurfaceType, shadingSurfaceType);
        OS_ASSERT(test);
      }
    }

    return result;
  }

  void ShadingSurfaceGroup_Impl::setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_ShadingSurfaceGroupFields::DirectionofRelativeNorth, directionofRelativeNorth, driverMethod);
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::resetDirectionofRelativeNorth() {
    bool result = setString(OS_ShadingSurfaceGroupFields::DirectionofRelativeNorth, "");
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::setXOrigin(double xOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_ShadingSurfaceGroupFields::XOrigin, xOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::resetXOrigin() {
    bool result = setString(OS_ShadingSurfaceGroupFields::XOrigin, "");
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::setYOrigin(double yOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_ShadingSurfaceGroupFields::YOrigin, yOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::resetYOrigin() {
    bool result = setString(OS_ShadingSurfaceGroupFields::YOrigin, "");
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::setZOrigin(double zOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_ShadingSurfaceGroupFields::ZOrigin, zOrigin, driverMethod);
    OS_ASSERT(result);
  }

  void ShadingSurfaceGroup_Impl::resetZOrigin() {
    bool result = setString(OS_ShadingSurfaceGroupFields::ZOrigin, "");
    OS_ASSERT(result);
  }

  boost::optional<Space> ShadingSurfaceGroup_Impl::space() const
  {
    boost::optional<Space> result;
    OptionalWorkspaceObject space = getTarget(OS_ShadingSurfaceGroupFields::SpaceName);
    if (space){
      result = space->optionalCast<Space>();
    }
    return result;
  }

  bool ShadingSurfaceGroup_Impl::setSpace(const Space& space)
  {
    bool result = setPointer(OS_ShadingSurfaceGroupFields::SpaceName, space.handle());
    if (result){
      bool test = setString(OS_ShadingSurfaceGroupFields::ShadingSurfaceType, "Space");
      OS_ASSERT(test);
    }
    return result;
  }

  void ShadingSurfaceGroup_Impl::resetSpace() {
    if (space()) {
      bool ok = setShadingSurfaceType("Building");
      OS_ASSERT(ok);
    }
  }

  ShadingSurfaceVector ShadingSurfaceGroup_Impl::shadingSurfaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<ShadingSurface>(ShadingSurface::iddObjectType());
  }

  boost::optional<ModelObject> ShadingSurfaceGroup_Impl::spaceAsModelObject() const {
    OptionalModelObject result;
    OptionalSpace intermediate = space();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> ShadingSurfaceGroup_Impl::shadingSurfacesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(shadingSurfaces());
    return result;
  }

  bool ShadingSurfaceGroup_Impl::setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

ShadingSurfaceGroup::ShadingSurfaceGroup(const Model& model)
  : PlanarSurfaceGroup(ShadingSurfaceGroup::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ShadingSurfaceGroup_Impl>());
  bool ok = this->setShadingSurfaceType("Building");
  OS_ASSERT(ok);
}

IddObjectType ShadingSurfaceGroup::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ShadingSurfaceGroup);
  return result;
}

std::vector<std::string> ShadingSurfaceGroup::validShadingSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadingSurfaceGroupFields::ShadingSurfaceType);
}

std::string ShadingSurfaceGroup::shadingSurfaceType() const {
  return getImpl<detail::ShadingSurfaceGroup_Impl>()->shadingSurfaceType();
}

bool ShadingSurfaceGroup::setShadingSurfaceType(std::string shadingSurfaceType) {
  return getImpl<detail::ShadingSurfaceGroup_Impl>()->setShadingSurfaceType(shadingSurfaceType);
}

boost::optional<Space> ShadingSurfaceGroup::space() const {
  return getImpl<detail::ShadingSurfaceGroup_Impl>()->space();
}

bool ShadingSurfaceGroup::setSpace(const Space& space) {
  return getImpl<detail::ShadingSurfaceGroup_Impl>()->setSpace(space);
}

void ShadingSurfaceGroup::resetSpace() {
  getImpl<detail::ShadingSurfaceGroup_Impl>()->resetSpace();
}

ShadingSurfaceVector ShadingSurfaceGroup::shadingSurfaces() const
{
  return getImpl<detail::ShadingSurfaceGroup_Impl>()->shadingSurfaces();
}

/// @cond
ShadingSurfaceGroup::ShadingSurfaceGroup(boost::shared_ptr<detail::ShadingSurfaceGroup_Impl> impl)
  : PlanarSurfaceGroup(impl)
{}
/// @endcond


} // model
} // openstudio

