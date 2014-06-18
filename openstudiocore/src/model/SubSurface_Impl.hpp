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

namespace detail {

  /** SubSurface_Impl is a PlanarSurface_Impl that is the implementation class for SubSurface.*/
  class MODEL_API SubSurface_Impl : public PlanarSurface_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string subSurfaceType READ subSurfaceType WRITE setSubSurfaceType);
    Q_PROPERTY(std::vector<std::string> subSurfaceTypeValues READ subSurfaceTypeValues);
    Q_PROPERTY(boost::optional<double> viewFactortoGround READ viewFactortoGround WRITE setViewFactortoGround RESET resetViewFactortoGround);
    Q_PROPERTY(bool isViewFactortoGroundDefaulted READ isViewFactortoGroundDefaulted);
    Q_PROPERTY(bool isViewFactortoGroundAutocalculated READ isViewFactortoGroundAutocalculated);
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);
    Q_PROPERTY(boost::optional<double> numberofVertices READ numberofVertices WRITE setNumberofVertices RESET resetNumberofVertices);
    Q_PROPERTY(bool isNumberofVerticesDefaulted READ isNumberofVerticesDefaulted);
    Q_PROPERTY(bool isNumberofVerticesAutocalculated READ isNumberofVerticesAutocalculated);

    Q_PROPERTY(std::string outsideBoundaryCondition READ outsideBoundaryCondition);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> surface READ surfaceAsModelObject WRITE setSurfaceAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> adjacentSubSurface READ adjacentSubSurfaceAsModelObject WRITE setAdjacentSubSurfaceAsModelObject RESET resetAdjacentSubSurface);
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
    virtual boost::optional<ParentObject> parent() const;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// remove self and all children objects recursively
    virtual std::vector<IdfObject> remove();

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

    std::string subSurfaceType() const;

    std::vector<std::string> subSurfaceTypeValues() const;

    boost::optional<double> viewFactortoGround() const;

    bool isViewFactortoGroundDefaulted() const;

    bool isViewFactortoGroundAutocalculated() const;

    bool allowShadingControl() const;

    boost::optional<ShadingControl> shadingControl() const;

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

