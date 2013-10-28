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

#ifndef MODEL_SURFACE_IMPL_HPP
#define MODEL_SURFACE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/PlanarSurface_Impl.hpp>

namespace openstudio {
namespace model {

class Space;
class SubSurface;
class Surface;
class SurfaceIntersection;

namespace detail {

  /** Surface_Impl is a PlanarSurface_Impl that is the implementation class for Surface.*/
  class MODEL_API Surface_Impl : public PlanarSurface_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string constructionType READ constructionType WRITE setConstructionType);
    Q_PROPERTY(std::string surfaceType READ surfaceType WRITE setSurfaceType);
    Q_PROPERTY(std::vector<std::string> surfaceTypeValues READ surfaceTypeValues);
    Q_PROPERTY(std::string outsideBoundaryCondition READ outsideBoundaryCondition WRITE setOutsideBoundaryCondition);
    Q_PROPERTY(std::vector<std::string> outsideBoundaryConditionValues READ outsideBoundaryConditionValues);
    Q_PROPERTY(std::string sunExposure READ sunExposure WRITE setSunExposure RESET resetSunExposure);
    Q_PROPERTY(std::vector<std::string> sunExposureValues READ sunExposureValues);
    Q_PROPERTY(bool isSunExposureDefaulted READ isSunExposureDefaulted);
    Q_PROPERTY(std::string windExposure READ windExposure WRITE setWindExposure RESET resetWindExposure);
    Q_PROPERTY(std::vector<std::string> windExposureValues READ windExposureValues);
    Q_PROPERTY(bool isWindExposureDefaulted READ isWindExposureDefaulted);
    Q_PROPERTY(boost::optional<double> viewFactortoGround READ viewFactortoGround WRITE setViewFactortoGround RESET resetViewFactortoGround);
    Q_PROPERTY(bool isViewFactortoGroundDefaulted READ isViewFactortoGroundDefaulted);
    Q_PROPERTY(bool isViewFactortoGroundAutocalculated READ isViewFactortoGroundAutocalculated);
    Q_PROPERTY(boost::optional<double> numberofVertices READ numberofVertices WRITE setNumberofVertices RESET resetNumberofVertices);
    Q_PROPERTY(bool isNumberofVerticesDefaulted READ isNumberofVerticesDefaulted);
    Q_PROPERTY(bool isNumberofVerticesAutocalculated READ isNumberofVerticesAutocalculated);
    Q_PROPERTY(bool isPartOfEnvelope READ isPartOfEnvelope);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> space READ spaceAsModelObject WRITE setSpaceAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> subSurfaces READ subSurfacesAsModelObjects);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> adjacentSurface READ adjacentSurfaceAsModelObject WRITE setAdjacentSurfaceAsModelObject RESET resetAdjacentSurface);
   public:
    /** @name Constructors and Destructors */
    //@{

    Surface_Impl(const IdfObject& idfObject, 
                 Model_Impl* model,
                 bool keepHandle);

    Surface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

    Surface_Impl(const Surface_Impl& other,
                 Model_Impl* model,
                 bool keepHandle);

    virtual ~Surface_Impl();

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// remove self and all children objects recursively
    virtual std::vector<IdfObject> remove();

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    /// should subtract this surface from parent's gross area for net area
    virtual bool subtractFromGrossArea() const;

    /// get the construction object
    /// if the planar surface is paired with an adjacent planar surface, attempts to resolve any surface matching conflict
    virtual boost::optional<ConstructionBase> construction() const;

    /// get the construction object and the search distance that was needed to find the construction
    /// does not consider adjacent planar surfaces
    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const;

    /// Returns true if the construction is not directly referenced by this surface .
    virtual bool isConstructionDefaulted() const;

    virtual bool setVertices(const std::vector<Point3d>& vertices);

    /// set the construction object
    virtual bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    virtual void resetConstruction();

    /// Returns the containing PlanarSurfaceGroup if available.
    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const;

    /// Returns the containing Space if available.
    virtual boost::optional<Space> space() const;

    /** Get the u-factor of this surface. Includes film coefficients. */
    virtual boost::optional<double> uFactor() const;

    /** Get the conductance of this surface. Does not include film coefficients. */
    virtual boost::optional<double> thermalConductance() const;

    /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include appropriate
     *  film coefficients. By default, assumes still air indoors and 15 mph outdoor air speed. */
    virtual bool setUFactor(double value);

    /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
    virtual bool setThermalConductance(double value);

    /** @name Getters */
    //@{

    std::string surfaceType() const;

    std::vector<std::string> surfaceTypeValues() const;

    std::string outsideBoundaryCondition() const;

    std::vector<std::string> outsideBoundaryConditionValues() const;

    bool isGroundSurface() const;

    std::string sunExposure() const;

    std::vector<std::string> sunExposureValues() const;

    bool isSunExposureDefaulted() const;

    std::string windExposure() const;

    std::vector<std::string> windExposureValues() const;

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
    bool setSurfaceType(std::string surfaceType, bool driverMethod);

    bool setOutsideBoundaryCondition(std::string outsideBoundaryCondition);
    bool setOutsideBoundaryCondition(std::string outsideBoundaryCondition, bool driverMethod);

    bool setSunExposure(std::string sunExposure);
    bool setSunExposure(std::string sunExposure, bool driverMethod);

    void resetSunExposure();

    bool setWindExposure(std::string windExposure);
    bool setWindExposure(std::string windExposure, bool driverMethod);

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

    /** Sets the adjacent Surface. */
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
    void assignDefaultSurfaceType(bool driverMethod);

    /** Assign default boundary condition. */
    void assignDefaultBoundaryCondition();
    void assignDefaultBoundaryCondition(bool driverMethod);

    /** Assign default sun exposure. */
    void assignDefaultSunExposure();
    void assignDefaultSunExposure(bool driverMethod);

    /** Assign default wind exposure. */
    void assignDefaultWindExposure();
    void assignDefaultWindExposure(bool driverMethod);

    /** Returns the ConstructionBase.standardsInformation().constructionType(). Method provided 
     *  so constructionType can be Attribute on the Surface. */
    std::string constructionType() const;

    /** Set the ConstructionBase.standardsInformation().constructionType(). Method provided so 
     *  constructionType can be Attribute on the Surface. */
    bool setConstructionType(const std::string& type);

    /** Get the default film thermal resistance (m^2*K/W) for this surface. Assumes still indoor
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
   private:
    friend class openstudio::model::Surface;

    REGISTER_LOGGER("openstudio.model.Surface");

    std::vector<ModelObject> subSurfacesAsModelObjects() const;
    boost::optional<ModelObject> adjacentSurfaceAsModelObject() const;

    bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAdjacentSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_SURFACE_IMPL_HPP

