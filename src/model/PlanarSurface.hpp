/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANARSURFACE_HPP
#define MODEL_PLANARSURFACE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Plane;
class Point3d;
class Vector3d;

namespace model {

  class PlanarSurfaceGroup;
  class Space;
  class ConstructionBase;
  class GeneratorPhotovoltaic;
  class SurfacePropertyConvectionCoefficients;

  namespace detail {
    class PlanarSurface_Impl;
  }  // namespace detail

  // clang-format off

  /** \class FilmResistanceType
   *
   *  \relates PlanarSurface */
  OPENSTUDIO_ENUM(FilmResistanceType,
    ((StillAir_HorizontalSurface_HeatFlowsUpward))
    ((StillAir_45DegreeSurface_HeatFlowsUpward))
    ((StillAir_VerticalSurface))
    ((StillAir_45DegreeSurface_HeatFlowsDownward))
    ((StillAir_HorizontalSurface_HeatFlowsDownward))
    ((MovingAir_15mph))
    ((MovingAir_7p5mph))
  );

  // clang-format on

  /** PlanarSurface is an abstract class derived from ParentObject.
 *
 *  PlanarSurface is a pure virtual class to consolodate functions
 *  dealing with surface geometry.
 *
 *  \todo There is too much functionality in this class that is not common to
 *  all derived classes.  These members need to be factored out of PlanarSurface.
 */
  class MODEL_API PlanarSurface : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~PlanarSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlanarSurface(const PlanarSurface& other) = default;
    PlanarSurface(PlanarSurface&& other) = default;
    PlanarSurface& operator=(const PlanarSurface&) = default;
    PlanarSurface& operator=(PlanarSurface&&) = default;

    //@}
    /** @name Getters */
    //@{

    /// Returns the construction object.
    /// If the planar surface is paired with an adjacent planar surface, attempts to resolve any surface matching conflict.
    boost::optional<ConstructionBase> construction() const;

    /// Returns the construction object and the search distance that was needed to find the construction.
    /// Does not consider adjacent planar surfaces.
    boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance() const;

    /// Returns true if the construction is not directly referenced by this surface .
    bool isConstructionDefaulted() const;

    /// Returns the containing PlanarSurfaceGroup if available.
    boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const;

    /// Returns the containing Space if available.
    boost::optional<Space> space() const;

    /// Returns the vertices.
    std::vector<Point3d> vertices() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the vertices.
    bool setVertices(const std::vector<Point3d>& vertices);

    /// Sets the construction object.
    bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    void resetConstruction();

    //@}

    /** Find PlanarSurfaces within angular range, specified in degrees in the site coordinate system, an unset optional means no limit.
      Values for degrees from North are between 0 and 360 and for degrees tilt they are between 0 and 180.
      Note that maxDegreesFromNorth may be less than minDegreesFromNorth,
      e.g. to search between NorthWest and NorthEast set minDegreesFromNorth = 315 and maxDegreesFromNorth = 45.
      The value of maxDegreesTilt must be greater than or equal to minDegreesTilt.  A tolerance of one degree is applied by default.
  */
    static std::vector<PlanarSurface> findPlanarSurfaces(const std::vector<PlanarSurface>& planarSurfaces,
                                                         boost::optional<double> minDegreesFromNorth, boost::optional<double> maxDegreesFromNorth,
                                                         boost::optional<double> minDegreesTilt, boost::optional<double> maxDegreesTilt,
                                                         double tol = 1);

    /** Film resistances from ASHRAE Fundamentals, Chapter 25, Table 1 for non-reflective surfaces.
   *  Units of m^2*K/W. */
    static double filmResistance(const FilmResistanceType& type);

    /** Film resistance of still air on surface with tilt (rad). Horizontal roof has tilt of 0 rad;
   *  horizontal floor/slab has tilt of pi rad. */
    static double stillAirFilmResistance(double tilt);

    /// Checks if this surface is an air wall, returns true if it's a ConstructionAirBoundary
    bool isAirWall() const;

    /// Computes the surface's gross area (m^2).
    double grossArea() const;

    /// Reports if user should subtract this surface's net area from parent's gross area
    /// to compute parent's net area (e.g. true for a window).
    bool subtractFromGrossArea() const;

    /// Computes the surface's net area (m^2).
    double netArea() const;

    /// Returns the outward normal vector in local coordinates.
    /// Throws openstudio::Exception if cannot compute outward normal for this surface.
    Vector3d outwardNormal() const;

    /// Returns the surface's tilt measured as angle between outward normal and local up (radians).
    /// Throws openstudio::Exception if cannot compute outward normal for this surface.
    double tilt() const;

    /// Returns the surface's azimuth measured clockwise as angle between outward normal and local North (radians).
    /// Throws openstudio::Exception if cannot compute outward normal for this surface.
    double azimuth() const;

    // Returns the u-factor of this surface. Includes standard film coefficients, does not consider any custom SurfacePropertyConvectionCoefficients.
    boost::optional<double> uFactor() const;

    /** Sets the u-factor of this surface in W/m^2*K, if possible. Value should already include appropriate
      *  standard film coefficients, assuming still air indoors and 15 mph outdoor air speed.
      *  Does not consider any custom SurfacePropertyConvectionCoefficients */
    bool setUFactor(double value);

    // Returns the thermal conductance of this surface in W/m^2*K. Does not include film coefficients.
    boost::optional<double> thermalConductance() const;

    // Sets the conductance of this surface in W/m^2*K, if possible. Value should not include any film coefficients.
    bool setThermalConductance(double value);

    // Returns the heat capacity of this surface in J/m^2*K, if available.
    boost::optional<double> heatCapacity() const;

    // Returns interior visible absorptance (unitless), if available.
    boost::optional<double> interiorVisibleAbsorptance() const;

    /// Returns exterior visible absorptance (unitless), if available.
    boost::optional<double> exteriorVisibleAbsorptance() const;

    /// Returns visible transmittance (unitless), if available.
    /// May require EnergyPlus simulation output.
    boost::optional<double> visibleTransmittance() const;

    /// Check if this planar surface's vertices are equal to the other's in the building coordinate system
    bool equalVertices(const PlanarSurface& other) const;

    /// Check if this planar surface's vertices are reverse equal to the other's in the building coordinate system
    bool reverseEqualVertices(const PlanarSurface& other) const;

    /// Get the plane, throws openstudio::Exception if cannot compute plane for this surface.
    Plane plane() const;

    /// Get a triangulation of this surface, subsurfaces will be replaced by holes in the triangulation
    virtual std::vector<std::vector<Point3d>> triangulation() const;

    /// Return the centroid of this planar surface's vertices
    Point3d centroid() const;

    /// Returns any solar hot water collectors associated with this surface.
    std::vector<ModelObject> solarCollectors() const;

    /// Returns any generator photovoltaics associated with this surface.
    std::vector<GeneratorPhotovoltaic> generatorPhotovoltaics() const;

    /// Returns any SurfacePropertyConvectionCoefficients associated with this surface, does not return SurfacePropertyConvectionCoefficientsMultipleSurface.
    std::vector<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

    bool isConvex() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new PlanarSurface object in the model.
    PlanarSurface(IddObjectType type, const std::vector<Point3d>& vertices, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::PlanarSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    // constructor
    explicit PlanarSurface(std::shared_ptr<detail::PlanarSurface_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.PlanarSurface");

    /// @endcond
  };

  /** \relates PlanarSurface */
  using OptionalPlanarSurface = boost::optional<PlanarSurface>;

  /** \relates PlanarSurface */
  using PlanarSurfaceVector = std::vector<PlanarSurface>;

}  // namespace model
}  // namespace openstudio

#endif  //MODEL_PLANARSURFACE_HPP
