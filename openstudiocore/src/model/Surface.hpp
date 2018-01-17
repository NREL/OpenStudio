/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SURFACE_HPP
#define MODEL_SURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

#include "SurfacePropertyExposedFoundationPerimeter.hpp"
#include "SurfacePropertyExposedFoundationPerimeter_Impl.hpp"

namespace openstudio {
namespace model {

class Space;
class SubSurface;
class ShadingSurfaceGroup;
class ConstructionBase;
class SurfacePropertyOtherSideCoefficients;
class SurfacePropertyOtherSideConditionsModel;
class SurfacePropertyConvectionCoefficients;
class FoundationKiva;
class SurfacePropertyExposedFoundationPerimeter;

namespace detail {

  class Surface_Impl;

} // detail

class SurfaceIntersection;

/** Surface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_Surface'. */
class MODEL_API Surface : public PlanarSurface {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Surface(const std::vector<Point3d>& vertices, const Model& model);

  virtual ~Surface() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validSurfaceTypeValues();

  static std::vector<std::string> validOutsideBoundaryConditionValues();

  static std::vector<std::string> validSunExposureValues();

  static std::vector<std::string> validWindExposureValues();

  //@}
  /** @name Getters */
  //@{

  std::string surfaceType() const;

  std::string outsideBoundaryCondition() const;

  /** Returns true if the surface has outsideBoundaryCondition of Ground, GroundFCfactorMethod
   *  GroundSlabPreprocessorAverage, GroundSlabPreprocessorCore, GroundSlabPreprocessorPerimeter,
   *  GroundBasementPreprocessorAverageWall, GroundBasementPreprocessorAverageFloor,
   *  GroundBasementPreprocessorUpperWall, or GroundBasementPreprocessorLowerWall */
  bool isGroundSurface() const;

  std::string sunExposure() const;

  bool isSunExposureDefaulted() const;

  std::string windExposure() const;

  bool isWindExposureDefaulted() const;

  boost::optional<double> viewFactortoGround() const;

  bool isViewFactortoGroundDefaulted() const;

  bool isViewFactortoGroundAutocalculated() const;

  boost::optional<double> numberofVertices() const;

  bool isNumberofVerticesDefaulted() const;

  bool isNumberofVerticesAutocalculated() const;

  //@}
  /** @name Setters */
  //@{

  bool setSurfaceType(std::string surfaceType);

  bool setOutsideBoundaryCondition(std::string outsideBoundaryCondition);

  bool setSunExposure(std::string sunExposure);

  void resetSunExposure();

  bool setWindExposure(std::string windExposure);

  void resetWindExposure();

  bool setViewFactortoGround(boost::optional<double> viewFactortoGround);

  bool setViewFactortoGround(double viewFactortoGround);

  void resetViewFactortoGround();

  void autocalculateViewFactortoGround();

  bool setNumberofVertices(boost::optional<double> numberofVertices);

  bool setNumberofVertices(double numberofVertices);

  void resetNumberofVertices();

  void autocalculateNumberofVertices();

  //@}

  /// Returns all child \link SubSurface SubSurfaces \endlink.
  std::vector<SubSurface> subSurfaces() const;

  /// Sets the parent Space.
  bool setSpace(const Space& space);

  /** Returns the adjacent Surface, if it exists. */
  boost::optional<Surface> adjacentSurface() const;

  /** Sets the adjacent Surface, will clear adjacent surface on other surface. */
  bool setAdjacentSurface(Surface& surface);

  /** Resets the adjacent Surface. */
  void resetAdjacentSurface();

  /** Returns the SurfacePropertyConvectionCoefficients, if it exists. */
  boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

  /** Returns the adjacent SurfaceSurfacePropertyOtherSideCoefficients, if it exists. */
  boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients() const;

  /** Sets the SurfacePropertyOtherSideCoefficients. */
  bool setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients);

  /** Resets the SurfacePropertyOtherSideCoefficients. */
  void resetSurfacePropertyOtherSideCoefficients();

  /** Returns the adjacent SurfacePropertyOtherSideConditionsModel, if it exists. */
  boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel() const;

  /** Sets the SurfacePropertyOtherSideConditionsModel. */
  bool setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel);

  /** Resets the SurfacePropertyOtherSideConditionsModel. */
  void resetSurfacePropertyOtherSideConditionsModel();

  /** Intersect with other Surface in other Space.
   *  Returns false if either surface has child windows.
   *  Returns false if either surface has an adjacent surface.
   *  Returns false if surfaces are not on the same plane with opposing outward normals.
   *  If the surfaces are the same, returns true but no new geometry is created.
   *  Returns true if an intersection occurred. Does not set surface adjacency. */
  bool intersect(Surface& otherSurface);
  boost::optional<SurfaceIntersection> computeIntersection(Surface& otherSurface);

  /** Creates an adjacent Surface in another Space, also create adjacent SubSurface objects if needed.
      Returns the new Surface if created. */
  boost::optional<Surface> createAdjacentSurface(const Space& otherSpace);

  /** Returns true if the Surface is part of the building envelope. */
  bool isPartOfEnvelope() const;

  /** Assign default surface type based on vertices. */
  void assignDefaultSurfaceType();

  /** Assign default boundary condition. */
  void assignDefaultBoundaryCondition();

  /** Assign default sun exposure. */
  void assignDefaultSunExposure();

  /** Assign default wind exposure. */
  void assignDefaultWindExposure();

  /** Returns the default film thermal resistance (m^2*K/W) for this surface. Assumes still indoor
   *  air, and 15 mph wind outside. Does not consider any custom SurfacePropertyConvectionCoefficients. */
  double filmResistance() const;

  /** Get the window to wall ratio for this surface. Calculated as
  sum(surface.windows.netArea)/surface.grossArea if this surface is a wall, returns
  0 if this surface is not a wall. */
  double windowToWallRatio() const;

  /** Get the skylight to roof ratio for this surface. Calculated as
  sum(surface.skylights.netArea)/surface.grossArea if this surface is a roof, returns
  0 if this surface is not a roof. */
  double skylightToRoofRatio() const;

  /** Get the skylight to projected floor ratio for this surface. Calculated as
  sum(surface.skylights.netArea)/project(surface to z=0 plane).grossArea if this surface is a roof, returns
  0 if this surface is not a roof. */
  double skylightToProjectedFloorRatio() const;

  /** Sets the window to wall ratio for this surface using a single banded window.
   *  Uses applyViewAndDaylightingGlassRatios for implementation. */
  boost::optional<SubSurface> setWindowToWallRatio(double wwr);

  /** Same as setWindowToWallRatio but with extra parameters desiredHeightOffset and heightOffsetFromFloor.
   *  If heightOffsetFromFloor is true then desiredHeightOffset is the desired sill height, otherwise it is the
   *  offset from the ceiling. Uses applyViewAndDaylightingGlassRatios for implementation. */
  boost::optional<SubSurface> setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor);

  /** Applies banded view and daylighting windows to the surface with optional exterior shading and interior light shelf.
   *
   *  Assumes that this surface spans the entire height of the space, this method should not be used if the wall is broken
   *  into multiple vertical pieces.
   *
   *  Returns false if the surface is not a wall, if the surface is not rectangular in face coordinates,
   *  if requested ratio is too large (window area ~= surface area), or if surface has any doors.
   *
   *  Otherwise, removes all existing windows and adds new windows to meet requested ratio.
   *
   *  viewGlassToWallRatio - the ratio of view glass to wall area, if 0 no view glass will be created
   *  daylightingGlassToWallRatio - the ratio of daylighting glass to wall area, if 0 no daylighting glass will be created
   *  desiredViewGlassSillHeight - the desired distance from the floor to the bottom of the view glass, this may be reduced if the requested window area is too high
   *  desiredDaylightingGlassHeaderHeight - the distance from the ceiling to the top of the daylighting glass, this may be reduced if the requested window area is too high
   *  exteriorShadingProjectionFactor - projection factor of exterior shading applied to the view window, if 0 no exterior shading will be created
   *  interiorShelfProjectionFactor - projection factor of interior light shelf applied to the daylighting window, if 0 no interior light shelf will be created
   *  viewGlassConstruction - optional construction to use for the view glass
   *  daylightingGlassConstruction - optional construction to use for the daylighting glass
   *
   *  If successful returns a vector of sub surfaces created, view window will be the first in the vector if viewGlassToWallRatio > 0
  */
  std::vector<SubSurface> applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio,
                                                             double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                             double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor,
                                                             const boost::optional<ConstructionBase>& viewGlassConstruction,
                                                             const boost::optional<ConstructionBase>& daylightingGlassConstruction);


  /** Returns any shading surface groups associated with this surface. */
  std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

  /** Splits this surface vertically surrounding any sub surfaces.
   *  This surface must be a wall and must not have an adjacent surface.
   *  Returns any new surfaces created in this routine.
   *  Typically this is called on a surface that has doors but no windows before applying banded windows. */
  std::vector<Surface> splitSurfaceForSubSurfaces();

  /** Moves all of the surfaces vertices towards the centroid by inset then intersects each face with the inset polygon.
   *  Faces are in space coordinates, it is expected that not all faces will intersect the surface.
   *  Returns all new sub surfaces created, sub surface types are defaulted.  Optional construction is applied.
   *  Returns false is this surface has any current sub surfaces or if there is an adjacent surface.*/
  std::vector<SubSurface> createSubSurfaces(const std::vector<std::vector<Point3d> >& faces, double inset, const boost::optional<ConstructionBase>& construction);

  bool setAdjacentFoundation(const FoundationKiva& kiva);
  
  boost::optional<FoundationKiva> adjacentFoundation() const;
  
  void resetAdjacentFoundation();
  
  // if surface property exposed foundation perimeter already exists, do nothing and return nil; creates the surface property exposed foundation perimeter if it does not already exist and return it;
  boost::optional<SurfacePropertyExposedFoundationPerimeter> createSurfacePropertyExposedFoundationPerimeter(std::string exposedPerimeterCalculationMethod, double exposedPerimeter);

  // returns the surface property exposed foundation perimeter if set
  boost::optional<SurfacePropertyExposedFoundationPerimeter> surfacePropertyExposedFoundationPerimeter() const;

  // resets the surface property exposed foundation perimeter
  void resetSurfacePropertyExposedFoundationPerimeter();
  
 protected:
  /// @cond
  typedef detail::Surface_Impl ImplType;

  friend class Model;
  friend class openstudio::detail::IdfObject_Impl;
  friend class openstudio::IdfObject;
  friend class openstudio::model::detail::Surface_Impl;

  explicit Surface(std::shared_ptr<detail::Surface_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Surface");
};

/** \relates Surface*/
typedef boost::optional<Surface> OptionalSurface;

/** \relates Surface*/
typedef std::vector<Surface> SurfaceVector;

/** SurfaceIntersection contains detailed information about a surface intersection. */
class MODEL_API SurfaceIntersection {
public:
  SurfaceIntersection(const Surface& surface1,
                      const Surface& surface2,
                      const std::vector<Surface>& newSurfaces1,
                      const std::vector<Surface>& newSurfaces2);

  // first surface post intersection
  Surface surface1() const;

  // second surface post intersection
  Surface surface2() const;

  // new surfaces generated in the first surface's space
  std::vector<Surface> newSurfaces1() const;

  // new surfaces generated in the second surface's space
  std::vector<Surface> newSurfaces2() const;

private:
  model::Surface m_surface1;
  model::Surface m_surface2;
  std::vector<Surface> m_newSurfaces1;
  std::vector<Surface> m_newSurfaces2;
};

/** \relates SurfaceIntersection */
MODEL_API std::ostream& operator<<(std::ostream& os, const SurfaceIntersection& surfaceIntersection);

} // model
} // openstudio

#endif // MODEL_SURFACE_HPP

