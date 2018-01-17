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

#ifndef MODEL_SUBSURFACE_IMPL_HPP
#define MODEL_SUBSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

class Surface;
class SubSurface;
class ShadingSurface;
class ShadingControl;
class ShadingSurfaceGroup;
class DaylightingDeviceShelf;
class WindowPropertyFrameAndDivider;
class SurfacePropertyOtherSideCoefficients;
class SurfacePropertyOtherSideConditionsModel;
class SurfacePropertyConfectionCoefficients;

namespace detail {

  /** SubSurface_Impl is a PlanarSurface_Impl that is the implementation class for SubSurface.*/
  class MODEL_API SubSurface_Impl : public PlanarSurface_Impl {
















   public:
    /** @name Constructors and Destructors */
    //@{

    SubSurface_Impl(const IdfObject& idfObject,
                    Model_Impl* model,
                    bool keepHandle);

    SubSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    SubSurface_Impl(const SubSurface_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    virtual ~SubSurface_Impl();

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const override;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent) override;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const override;

    /// remove self and all children objects recursively
    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /// should subtract this surface from parent's gross area for net area
    virtual bool subtractFromGrossArea() const override;

    /// get the construction object
    /// if the planar surface is paired with an adjacent planar surface, attempts to resolve any surface matching conflict
    virtual boost::optional<ConstructionBase> construction() const override;

    /// get the construction object and the search distance that was needed to find the construction
    /// does not consider adjacent planar surfaces
    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const override;

    /// Returns true if the construction is not directly referenced by this surface .
    virtual bool isConstructionDefaulted() const override;

    virtual bool setVertices(const std::vector<Point3d>& vertices) override;

    /// set the construction object
    virtual bool setConstruction(const ConstructionBase& construction) override;

    /// Resets the construction object.
    virtual void resetConstruction() override;

    /// Returns the containing PlanarSurfaceGroup if available.
    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const override;

    /// Returns the containing Space if available.
    virtual boost::optional<Space> space() const override;

    /** Get the u-factor of this surface. Includes film coefficients. */
    virtual boost::optional<double> uFactor() const override;

    /** Get the conductance of this surface. Does not include film coefficients. */
    virtual boost::optional<double> thermalConductance() const override;

    /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include appropriate
     *  film coefficients. By default, assumes still air indoors and 15 mph outdoor air speed. */
    virtual bool setUFactor(double value) override;

    /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
    virtual bool setThermalConductance(double value) override;

    /** @name Getters */
    //@{

    std::string subSurfaceType() const;

    std::vector<std::string> subSurfaceTypeValues() const;

    boost::optional<double> viewFactortoGround() const;

    bool isViewFactortoGroundDefaulted() const;

    bool isViewFactortoGroundAutocalculated() const;

    bool allowShadingControl() const;

    boost::optional<ShadingControl> shadingControl() const;

    bool allowWindowPropertyFrameAndDivider() const;

    boost::optional<WindowPropertyFrameAndDivider> windowPropertyFrameAndDivider() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    //@}
    /** @name Setters */
    //@{

    bool setSubSurfaceType(std::string subSurfaceType);

    bool setViewFactortoGround(boost::optional<double> viewFactortoGround);

    bool setViewFactortoGround(double viewFactortoGround);

    void resetViewFactortoGround();

    void autocalculateViewFactortoGround();

    bool setShadingControl(const ShadingControl& shadingControl);

    void resetShadingControl();

    bool setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider);

    void resetWindowPropertyFrameAndDivider();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    //@}

    /// get the surface
    boost::optional<Surface> surface() const;

    /// set the surface
    bool setSurface(const Surface& surface);

    /// get the adjacent subsurface
    boost::optional<SubSurface> adjacentSubSurface() const;

    /// set the adjacent subsurface, will fail unless both sub surfaces are parented by surfaces
    /// which are adjacent
    bool setAdjacentSubSurface(SubSurface& subSurface);

    /// reset the adjacent subsurface, will clear references on both this and adjacent sub surface
    void resetAdjacentSubSurface();

    /** Returns the surface property convection coefficients */
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

    /** Default sub surface type based on vertices. */
    std::string defaultSubSurfaceType() const;

    /** Assign default sub surface type based on vertices. */
    void assignDefaultSubSurfaceType();

    /** Return the surface()'s outsideBoundaryCondition, or an empty string. */
    std::string outsideBoundaryCondition() const;

    /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors. */
    boost::optional<ShadingSurface> addOverhang(double depth, double offset);

    /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors.
     *  Offset is a fraction of the total window height, projection factor is based on height and offset. */
    boost::optional<ShadingSurface> addOverhangByProjectionFactor(double projectionFactor, double offsetFraction);

    /** Returns any shading surface groups associated with this sub surface. */
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    /** Returns true if this sub surface allows the addition of a daylighting light shelf. */
    bool allowDaylightingDeviceShelf() const;

    /** Get the daylighting light shelf associated with this sub surface if there is one. */
    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

    /** Add a daylighting light shelf associated with this sub surface.  Only succeeds if this is a fixed window,
     * operable window, or glass door. Will return existing daylighting light shelf if there already is one. */
    boost::optional<DaylightingDeviceShelf> addDaylightingDeviceShelf() const;

   protected:

   private:
    friend class openstudio::model::SubSurface;

    REGISTER_LOGGER("openstudio.model.SubSurface");

    boost::optional<ModelObject> surfaceAsModelObject() const;
    boost::optional<ModelObject> adjacentSubSurfaceAsModelObject() const;

    bool setSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAdjacentSubSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SUBSURFACE_IMPL_HPP

