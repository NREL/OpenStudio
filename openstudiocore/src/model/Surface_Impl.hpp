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

#ifndef MODEL_SURFACE_IMPL_HPP
#define MODEL_SURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

class Space;
class SubSurface;
class Surface;
class ShadingSurfaceGroup;
class SurfaceIntersection;
class ConstructionBase;

namespace detail {

  /** Surface_Impl is a PlanarSurface_Impl that is the implementation class for Surface.*/
  class MODEL_API Surface_Impl : public PlanarSurface_Impl {
    Q_OBJECT;
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

    virtual boost::optional<ParentObject> parent() const;

    virtual bool setParent(ParentObject& newParent);

    virtual std::vector<ModelObject> children() const;

    virtual std::vector<IdfObject> remove();

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    virtual bool subtractFromGrossArea() const;

    virtual boost::optional<ConstructionBase> construction() const;

    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const;

    virtual bool isConstructionDefaulted() const;

    virtual bool setVertices(const std::vector<Point3d>& vertices);

    virtual bool setConstruction(const ConstructionBase& construction);

    virtual void resetConstruction();

    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const;

    virtual boost::optional<Space> space() const;

    virtual boost::optional<double> uFactor() const;

    virtual boost::optional<double> thermalConductance() const;

    virtual bool setUFactor(double value);

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

    std::vector<SubSurface> subSurfaces() const;

    bool setSpace(const Space& space);

    boost::optional<Surface> adjacentSurface() const;

    bool setAdjacentSurface(Surface& surface);

    void resetAdjacentSurface();

    bool intersect(Surface& otherSurface);
    boost::optional<SurfaceIntersection> computeIntersection(Surface& otherSurface);

    boost::optional<Surface> createAdjacentSurface(const Space& otherSpace);

    bool isPartOfEnvelope() const;

    void assignDefaultSurfaceType();
    void assignDefaultSurfaceType(bool driverMethod);

    void assignDefaultBoundaryCondition();
    void assignDefaultBoundaryCondition(bool driverMethod);

    void assignDefaultSunExposure();
    void assignDefaultSunExposure(bool driverMethod);

    void assignDefaultWindExposure();
    void assignDefaultWindExposure(bool driverMethod);

    double filmResistance() const;

    double windowToWallRatio() const;

    double skylightToRoofRatio() const;

    double skylightToProjectedFloorRatio() const;

    boost::optional<SubSurface> setWindowToWallRatio(double wwr);
    
    boost::optional<SubSurface> setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor);

    std::vector<SubSurface> applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio, 
                                                               double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                               double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor, 
                                                               const boost::optional<ConstructionBase>& viewGlassConstruction, 
                                                               const boost::optional<ConstructionBase>& daylightingGlassConstruction);

    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    std::vector<Surface> splitSurfaceForSubSurfaces();

    std::vector<SubSurface> createSubSurfaces(const std::vector<std::vector<Point3d> >& faces, double inset, const boost::optional<ConstructionBase>& construction);

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

