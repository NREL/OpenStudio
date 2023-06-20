/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
      : ParentObject_Impl(idfObject, model, keepHandle) {
      // connect signals
      this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
    }

    PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      // connect signals
      this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
    }

    PlanarSurfaceGroup_Impl::PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      // connect signals
      this->PlanarSurfaceGroup_Impl::onChange.connect<PlanarSurfaceGroup_Impl, &PlanarSurfaceGroup_Impl::clearCachedVariables>(this);
    }

    openstudio::Transformation PlanarSurfaceGroup_Impl::transformation() const {
      if (!m_cachedTransformation) {
        double x = this->xOrigin();
        double y = this->yOrigin();
        double z = this->zOrigin();

        // translation
        Transformation translation = Transformation::translation(Vector3d(x, y, z));

        double dORN = this->directionofRelativeNorth();

        // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
        Transformation rotation = Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(dORN));

        m_cachedTransformation = translation * rotation;
      }

      return m_cachedTransformation.get();
    }

    bool PlanarSurfaceGroup_Impl::setTransformation(const openstudio::Transformation& transformation) {

      EulerAngles eulerAngles = transformation.eulerAngles();
      if ((eulerAngles.psi() != 0) || (eulerAngles.theta() != 0)) {
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

    openstudio::Transformation PlanarSurfaceGroup_Impl::siteTransformation() const {
      Transformation transformation;

      boost::optional<Building> building = this->model().building();
      if (building) {
        transformation = building->transformation() * this->buildingTransformation();
      } else {
        transformation = this->buildingTransformation();
      }
      return transformation;
    }

    openstudio::BoundingBox PlanarSurfaceGroup_Impl::boundingBoxParentCoordinates() const {
      return transformation() * boundingBox();
    }

    openstudio::BoundingBox PlanarSurfaceGroup_Impl::boundingBoxBuildingCoordinates() const {
      return buildingTransformation() * boundingBox();
    }

    openstudio::BoundingBox PlanarSurfaceGroup_Impl::boundingBoxSiteCoordinates() const {
      return siteTransformation() * boundingBox();
    }

    void PlanarSurfaceGroup_Impl::clearCachedVariables() {
      m_cachedTransformation.reset();
    }

  }  // namespace detail

  PlanarSurfaceGroup::PlanarSurfaceGroup(IddObjectType iddObjectType, const Model& model) : ParentObject(iddObjectType, model) {
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

  bool PlanarSurfaceGroup::setDirectionofRelativeNorth(double directionofRelativeNorth) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->setDirectionofRelativeNorth(directionofRelativeNorth);
  }

  void PlanarSurfaceGroup::resetDirectionofRelativeNorth() {
    getImpl<detail::PlanarSurfaceGroup_Impl>()->resetDirectionofRelativeNorth();
  }

  bool PlanarSurfaceGroup::setXOrigin(double xOrigin) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->setXOrigin(xOrigin);
  }

  void PlanarSurfaceGroup::resetXOrigin() {
    getImpl<detail::PlanarSurfaceGroup_Impl>()->resetXOrigin();
  }

  bool PlanarSurfaceGroup::setYOrigin(double yOrigin) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->setYOrigin(yOrigin);
  }

  void PlanarSurfaceGroup::resetYOrigin() {
    getImpl<detail::PlanarSurfaceGroup_Impl>()->resetYOrigin();
  }

  bool PlanarSurfaceGroup::setZOrigin(double zOrigin) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->setZOrigin(zOrigin);
  }

  void PlanarSurfaceGroup::resetZOrigin() {
    getImpl<detail::PlanarSurfaceGroup_Impl>()->resetZOrigin();
  }

  openstudio::Transformation PlanarSurfaceGroup::transformation() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->transformation();
  }

  openstudio::Transformation PlanarSurfaceGroup::buildingTransformation() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->buildingTransformation();
  }

  openstudio::Transformation PlanarSurfaceGroup::siteTransformation() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->siteTransformation();
  }

  bool PlanarSurfaceGroup::setTransformation(const openstudio::Transformation& transformation) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->setTransformation(transformation);
  }

  bool PlanarSurfaceGroup::changeTransformation(const openstudio::Transformation& transformation) {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->changeTransformation(transformation);
  }

  openstudio::BoundingBox PlanarSurfaceGroup::boundingBox() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->boundingBox();
  }

  openstudio::BoundingBox PlanarSurfaceGroup::boundingBoxParentCoordinates() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->boundingBoxParentCoordinates();
  }

  openstudio::BoundingBox PlanarSurfaceGroup::boundingBoxBuildingCoordinates() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->boundingBoxBuildingCoordinates();
  }

  openstudio::BoundingBox PlanarSurfaceGroup::boundingBoxSiteCoordinates() const {
    return getImpl<detail::PlanarSurfaceGroup_Impl>()->boundingBoxSiteCoordinates();
  }

  /// @cond
  PlanarSurfaceGroup::PlanarSurfaceGroup(std::shared_ptr<detail::PlanarSurfaceGroup_Impl> impl) : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
