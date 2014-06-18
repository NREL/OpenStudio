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

#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"
#include "../utilities/geometry/EulerAngles.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    // connect signals
    bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
    OS_ASSERT(connected);
  }

  PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    // connect signals
    bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
    OS_ASSERT(connected);
  }

  PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    // connect signals
    bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
    OS_ASSERT(connected);
  }

  openstudio::Transformation PlanarSurfaceGroup_Impl::transformation() const
  {
    if (!m_cachedTransformation){
      double x = this->xOrigin();
      double y = this->yOrigin();
      double z = this->zOrigin();

      // translation
      Transformation translation = Transformation::translation(Vector3d(x, y, z));

      double dORN = this->directionofRelativeNorth();

      // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
      Transformation rotation = Transformation::rotation(Vector3d(0,0,1), -openstudio::degToRad(dORN));

      m_cachedTransformation = translation*rotation;
    }

    return m_cachedTransformation.get();
  }

  bool PlanarSurfaceGroup_Impl::setTransformation(const openstudio::Transformation& transformation) {

    EulerAngles eulerAngles = transformation.eulerAngles();
    if ((eulerAngles.psi() != 0) || (eulerAngles.theta() != 0)){
      return false;
    }

    double dORN = -radToDeg(eulerAngles.phi());
    this->setDirectionofRelativeNorth(dORN, false);

    Vector3d translation = transformation.translation();
    this->setXOrigin(translation.x(), false);
    this->setYOrigin(translation.y(), false);
    this->setZOrigin(translation.z(), false);
    this->emitChangeSignals();

    return true;
  }

  openstudio::Transformation PlanarSurfaceGroup_Impl::siteTransformation() const
  {
    Transformation transformation;

    boost::optional<Building> building = this->model().building();
    if (building){
      transformation = building->transformation() * this->buildingTransformation();
    }else{
      transformation = this->buildingTransformation();
    }
    return transformation;
  }

  void PlanarSurfaceGroup_Impl::clearCachedVariables()
  {
    m_cachedTransformation.reset();
  }

} // detail

PlanarSurfaceGroup::PlanarSurfaceGroup(IddObjectType iddObjectType, const Model& model)
  : ParentObject(iddObjectType,model)
{
  OS_ASSERT(getImpl<detail::PlanarSurfaceGroup_Impl>());
}


double PlanarSurfaceGroup::directionofRelativeNorth() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->directionofRelativeNorth();
}

bool PlanarSurfaceGroup::isDirectionofRelativeNorthDefaulted() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->isDirectionofRelativeNorthDefaulted();
}

double PlanarSurfaceGroup::xOrigin() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->xOrigin();
}

bool PlanarSurfaceGroup::isXOriginDefaulted() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->isXOriginDefaulted();
}

double PlanarSurfaceGroup::yOrigin() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->yOrigin();
}

bool PlanarSurfaceGroup::isYOriginDefaulted() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->isYOriginDefaulted();
}

double PlanarSurfaceGroup::zOrigin() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->zOrigin();
}

bool PlanarSurfaceGroup::isZOriginDefaulted() const {
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->isZOriginDefaulted();
}

void PlanarSurfaceGroup::setDirectionofRelativeNorth(double directionofRelativeNorth) {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->setDirectionofRelativeNorth(directionofRelativeNorth);
}

void PlanarSurfaceGroup::resetDirectionofRelativeNorth() {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->resetDirectionofRelativeNorth();
}

void PlanarSurfaceGroup::setXOrigin(double xOrigin) {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->setXOrigin(xOrigin);
}

void PlanarSurfaceGroup::resetXOrigin() {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->resetXOrigin();
}

void PlanarSurfaceGroup::setYOrigin(double yOrigin) {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->setYOrigin(yOrigin);
}

void PlanarSurfaceGroup::resetYOrigin() {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->resetYOrigin();
}

void PlanarSurfaceGroup::setZOrigin(double zOrigin) {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->setZOrigin(zOrigin);
}

void PlanarSurfaceGroup::resetZOrigin() {
  getImpl<detail::PlanarSurfaceGroup_Impl>()->resetZOrigin();
}

openstudio::Transformation PlanarSurfaceGroup::transformation() const
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->transformation();
}

openstudio::Transformation PlanarSurfaceGroup::buildingTransformation() const
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->buildingTransformation();
}

openstudio::Transformation PlanarSurfaceGroup::siteTransformation() const
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->siteTransformation();
}

bool PlanarSurfaceGroup::setTransformation(const openstudio::Transformation& transformation)
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->setTransformation(transformation);
}

bool PlanarSurfaceGroup::changeTransformation(const openstudio::Transformation& transformation)
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->changeTransformation(transformation);
}

openstudio::BoundingBox PlanarSurfaceGroup::boundingBox() const
{
  return getImpl<detail::PlanarSurfaceGroup_Impl>()->boundingBox();
}


/// @cond
PlanarSurfaceGroup::PlanarSurfaceGroup(std::shared_ptr<detail::PlanarSurfaceGroup_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio
