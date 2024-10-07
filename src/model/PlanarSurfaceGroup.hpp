/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANARSURFACEGROUP_HPP
#define MODEL_PLANARSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Transformation;
class Point3d;
class BoundingBox;

namespace model {

  namespace detail {
    class PlanarSurfaceGroup_Impl;
  }

  /** PlanarSurfaceGroup is an abstract class representing a group that holds many PlanarSurfaces.
 */
  class MODEL_API PlanarSurfaceGroup : public ParentObject
  {
   public:
    virtual ~PlanarSurfaceGroup() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlanarSurfaceGroup(const PlanarSurfaceGroup& other) = default;
    PlanarSurfaceGroup(PlanarSurfaceGroup&& other) = default;
    PlanarSurfaceGroup& operator=(const PlanarSurfaceGroup&) = default;
    PlanarSurfaceGroup& operator=(PlanarSurfaceGroup&&) = default;

    /** @name Getters */
    //@{

    double directionofRelativeNorth() const;

    bool isDirectionofRelativeNorthDefaulted() const;

    double xOrigin() const;

    bool isXOriginDefaulted() const;

    double yOrigin() const;

    bool isYOriginDefaulted() const;

    double zOrigin() const;

    bool isZOriginDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDirectionofRelativeNorth(double directionofRelativeNorth);

    void resetDirectionofRelativeNorth();

    bool setXOrigin(double xOrigin);

    void resetXOrigin();

    bool setYOrigin(double yOrigin);

    void resetYOrigin();

    bool setZOrigin(double zOrigin);

    void resetZOrigin();

    //@}

    /** Returns the transformation from local coordinates to parent coordinates. */
    openstudio::Transformation transformation() const;

    /** Returns the transformation from local coordinates to building coordinates. */
    openstudio::Transformation buildingTransformation() const;

    /** Returns the transformation from local coordinates to site coordinates. */
    openstudio::Transformation siteTransformation() const;

    /** Sets the transformation from local coordinates to parent coordinates,
   *  this method can be used to move the group. */
    bool setTransformation(const openstudio::Transformation& transformation);

    /** Changes the transformation from local coordinates to parent coordinates,
   *  this method alter geometry of children relative to the
   *  group so that it stays in the same place with the new transformation. */
    bool changeTransformation(const openstudio::Transformation& transformation);

    /** Get the BoundingBox in local coordinates. */
    openstudio::BoundingBox boundingBox() const;

    /** Get the BoundingBox in parent coordinates. */
    openstudio::BoundingBox boundingBoxParentCoordinates() const;

    /** Get the BoundingBox in building coordinates. */
    openstudio::BoundingBox boundingBoxBuildingCoordinates() const;

    /** Get the BoundingBox in site coordinates. */
    openstudio::BoundingBox boundingBoxSiteCoordinates() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    explicit PlanarSurfaceGroup(IddObjectType iddObjectType, const Model& model);

    //@}

    /// @cond
    using ImplType = detail::PlanarSurfaceGroup_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::PlanarSurfaceGroup_Impl;

    explicit PlanarSurfaceGroup(std::shared_ptr<detail::PlanarSurfaceGroup_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");
  };

  /** \relates PlanarSurfaceGroup */
  using OptionalPlanarSurfaceGroup = boost::optional<PlanarSurfaceGroup>;

  /** \relates PlanarSurfaceGroup */
  using PlanarSurfaceGroupVector = std::vector<PlanarSurfaceGroup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANARSURFACEGROUP_HPP
