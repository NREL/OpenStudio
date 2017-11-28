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
    this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
  }

  PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    // connect signals
    this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
  }

  PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    // connect signals
    this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
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
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
