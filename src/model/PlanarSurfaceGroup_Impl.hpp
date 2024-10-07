/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANARSURFACEGROUP_IMPL_HPP
#define MODEL_PLANARSURFACEGROUP_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"

namespace openstudio {

class Point3d;
class BoundingBox;

namespace model {
  namespace detail {

    class MODEL_API PlanarSurfaceGroup_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      PlanarSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~PlanarSurfaceGroup_Impl() override = default;

      virtual double directionofRelativeNorth() const = 0;

      virtual bool isDirectionofRelativeNorthDefaulted() const = 0;

      virtual double xOrigin() const = 0;

      virtual bool isXOriginDefaulted() const = 0;

      virtual double yOrigin() const = 0;

      virtual bool isYOriginDefaulted() const = 0;

      virtual double zOrigin() const = 0;

      virtual bool isZOriginDefaulted() const = 0;

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) = 0;

      virtual void resetDirectionofRelativeNorth() = 0;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) = 0;

      virtual void resetXOrigin() = 0;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) = 0;

      virtual void resetYOrigin() = 0;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) = 0;

      virtual void resetZOrigin() = 0;

      /** Returns the transformation from local coordinates to parent coordinates. */
      openstudio::Transformation transformation() const;

      /** Returns the transformation from local coordinates to site coordinates. */
      openstudio::Transformation siteTransformation() const;

      /** Returns the transformation from local coordinates to building coordinates. */
      virtual openstudio::Transformation buildingTransformation() const = 0;

      /** Sets the transformation from local coordinates to parent coordinates,
     *  this method can be used to move the group. */
      bool setTransformation(const openstudio::Transformation& transformation);

      /** Changes the transformation from local coordinates to parent coordinates,
     *  this method alter geometry of children relative to the
     *  group so that it stays in the same place with the new transformation. */
      virtual bool changeTransformation(const openstudio::Transformation& transformation) = 0;

      /** Get the BoundingBox in local coordinates. */
      virtual openstudio::BoundingBox boundingBox() const = 0;

      /** Get the BoundingBox in parent coordinates. */
      openstudio::BoundingBox boundingBoxParentCoordinates() const;

      /** Get the BoundingBox in building coordinates. */
      openstudio::BoundingBox boundingBoxBuildingCoordinates() const;

      /** Get the BoundingBox in site coordinates. */
      openstudio::BoundingBox boundingBoxSiteCoordinates() const;

      //@}
      //private slots:
     private:
      void clearCachedVariables();

     private:
      REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");

      mutable boost::optional<openstudio::Transformation> m_cachedTransformation;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANARSURFACEGROUP_IMPL_HPP
