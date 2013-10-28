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

#ifndef MODEL_SURFACE_HPP
#define MODEL_SURFACE_HPP

#include <model/ModelAPI.hpp>
#include <model/PlanarSurface.hpp>

namespace openstudio {
namespace model {

class Space;
class SubSurface;

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

  /** Returns the ConstructionBase.standardsInformation().constructionType(). Method provided
   *  so constructionType can be Attribute on the Surface. */
  std::string constructionType() const;

  /** Set the ConstructionBase.standardsInformation().constructionType(). Method provided so
   *  constructionType can be Attribute on the Surface. */
  bool setConstructionType(const std::string& type);

  /** Returns the default film thermal resistance (m^2*K/W) for this surface. Assumes still indoor
   *  air, and 15 mph wind outside. */
  double filmResistance() const;

  /** Get the window to wall ratio for this surface. Calculated as 
  sum(surface.windows.netArea)/surface.grossArea if this surface is a wall, returns
  0 if this surface is not a wall. */
  double windowToWallRatio() const;

  /** Sets the window to wall ratio for this surface.  Returns false if the
  surface is not a wall, if the surface is not rectangular in face coordinates, if 
  requested ratio is too large (window area ~= surface area) or too small (min dimension
  of window < 1 foot), or if the window clips any remaining sub surfaces. Otherwise, removes
  all existing windows and adds new window to meet requested ratio.*/
  boost::optional<SubSurface> setWindowToWallRatio(double wwr);
  
  /** Same as setWindowToWallRatio but with extra parameters desiredHeightOffset and heightOffsetFromFloor.
  If heightOffsetFromFloor is true then desiredHeightOffset is the desired sill height, otherwise it is the
  offset from the ceiling. */
  boost::optional<SubSurface> setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor);

 protected:
  /// @cond
  typedef detail::Surface_Impl ImplType;

  friend class Model;
  friend class openstudio::detail::IdfObject_Impl;
  friend class openstudio::IdfObject;
  friend class openstudio::model::detail::Surface_Impl;

  explicit Surface(boost::shared_ptr<detail::Surface_Impl> impl);

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

