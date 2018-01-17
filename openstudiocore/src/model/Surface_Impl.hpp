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
class SurfacePropertyOtherSideCoefficients;
class SurfacePropertyOtherSideConditionsModel;
class SurfacePropertyConfectionCoefficients;
class FoundationKiva;

namespace detail {

  /** Surface_Impl is a PlanarSurface_Impl that is the implementation class for Surface.*/
  class MODEL_API Surface_Impl : public PlanarSurface_Impl {






















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

    virtual boost::optional<ParentObject> parent() const override;

    virtual bool setParent(ParentObject& newParent) override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IdfObject> remove() override;

    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual bool subtractFromGrossArea() const override;

    virtual boost::optional<ConstructionBase> construction() const override;

    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const override;

    virtual bool isConstructionDefaulted() const override;

    virtual bool setVertices(const std::vector<Point3d>& vertices) override;

    virtual bool setConstruction(const ConstructionBase& construction) override;

    virtual void resetConstruction() override;

    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const override;

    virtual boost::optional<Space> space() const override;

    virtual boost::optional<double> uFactor() const override;

    virtual boost::optional<double> thermalConductance() const override;

    virtual bool setUFactor(double value) override;

    virtual bool setThermalConductance(double value) override;

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

    boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

    boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients() const;

    bool setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients);

    void resetSurfacePropertyOtherSideCoefficients();

    boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel() const;

    bool setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel);

    void resetSurfacePropertyOtherSideConditionsModel();

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

