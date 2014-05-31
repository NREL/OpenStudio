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

#ifndef MODEL_PLANARSURFACE_IMPL_HPP
#define MODEL_PLANARSURFACE_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

namespace openstudio {

namespace model {

class PlanarSurfaceGroup;
class PlanarSurface;
class Space;
class ConstructionBase;

namespace detail {

  // derive PlanarSurface_Impl from ParentObject_Impl to override virtual methods
  class MODEL_API PlanarSurface_Impl : public ParentObject_Impl{
    Q_OBJECT;
    Q_PROPERTY(double area READ grossArea);
    Q_PROPERTY(double grossArea READ grossArea);
    Q_PROPERTY(double netArea READ netArea);

    // includes film coefficients. may require simulation results.
    Q_PROPERTY(boost::optional<double> uFactor READ uFactor WRITE setUFactor);
    // does not include film coefficients. may require simulation results.
    Q_PROPERTY(boost::optional<double> thermalConductance READ thermalConductance WRITE setThermalConductance);

    Q_PROPERTY(boost::optional<double> heatCapacity READ heatCapacity);

    // may require simulation results.
    Q_PROPERTY(boost::optional<double> interiorVisibleAbsorptance READ interiorVisibleAbsorptance);
    // may require simulation results.
    Q_PROPERTY(boost::optional<double> exteriorVisibleAbsorptance READ exteriorVisibleAbsorptance);
    // may require simulation results.
    Q_PROPERTY(boost::optional<double> visibleTransmittance READ visibleTransmittance);

    // relationships
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> construction READ constructionAsModelObject WRITE setConstructionAsModelObject );
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> planarSurfaceGroup READ planarSurfaceGroupAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> space READ spaceAsModelObject);
   public:

    /** @name Constructors and Destructors */
    //@{

    // constructor
    PlanarSurface_Impl(IddObjectType type,
                       Model_Impl* model);

    // constructor
    PlanarSurface_Impl(const IdfObject& idfObject,
                       Model_Impl* model,
                       bool keepHandle);

    // construct from workspace
    PlanarSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    // clone copy constructor
    PlanarSurface_Impl(const PlanarSurface_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~PlanarSurface_Impl() {}

    //@}
    /** @name Getters */
    //@{

    /// get the construction object
    /// if the planar surface is paired with an adjacent planar surface, attempts to resolve any surface matching conflict
    virtual boost::optional<ConstructionBase> construction() const;

    /// get the construction object and the search distance that was needed to find the construction
    /// does not consider adjacent planar surfaces
    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const = 0;

    /// Returns true if the construction is not directly referenced by this surface .
    virtual bool isConstructionDefaulted() const = 0;

    /// Returns the containing PlanarSurfaceGroup if available.
    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const = 0;

    /// Returns the containing Space if available.
    virtual boost::optional<Space> space() const = 0;

    /// get the vertices
    std::vector<Point3d> vertices() const;

    //@}
    /** @name Setters */

    /// set the vertices
    virtual bool setVertices(const std::vector<Point3d>& vertices);

    /// set the construction object
    virtual bool setConstruction(const ConstructionBase& construction) = 0;

    /// Resets the construction object.
    virtual void resetConstruction() = 0;

    /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include appropriate
     *  film coefficients. By default, assumes still air indoors and 15 mph outdoor air speed. */
    virtual bool setUFactor(double value);

    bool setUFactor(boost::optional<double> value);

    /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
    virtual bool setThermalConductance(double value);

    bool setThermalConductance(boost::optional<double> value);

    //@}
    /** @name Queries */
    //@{

    /// is this surface an air wall
    bool isAirWall() const;

    /// compute gross area (m^2)
    double grossArea() const;

    /// should subtract this surface from parent's gross area for net area
    virtual bool subtractFromGrossArea() const = 0;

    /// compute net area (m^2)
    double netArea() const;

    /// get the outward normal in local coordinates
    /// throws openstudio::Exception if cannot compute outward normal for this surface.
    Vector3d outwardNormal() const;

    /// get tilt [from local up] (rad)
    /// throws openstudio::Exception if cannot compute outward normal for this surface.
    double tilt() const;

    /// get azimuth [clockwise from local North] (rad)
    /// throws openstudio::Exception if cannot compute outward normal for this surface.
    double azimuth() const;

    /** Get the u-factor of this surface. Includes film coefficients. */
    virtual boost::optional<double> uFactor() const;

    /** Get the conductance of this surface. Does not include film coefficients. */
    virtual boost::optional<double> thermalConductance() const;

    /** Get the heat capacity of this surface in J/m^2*K, if available. */
    boost::optional<double> heatCapacity() const;

    /// get interior visible absorptance (unitless)
    boost::optional<double> interiorVisibleAbsorptance() const;

    /// get exterior visible absorptance (unitless)
    boost::optional<double> exteriorVisibleAbsorptance() const;

    /// get visible transmittance (unitless)
    /// requires 'EnvelopeSummary' summary table
    boost::optional<double> visibleTransmittance() const;

    /// Check if this planar surface's vertices are equal to the other's in the building coordinate system
    bool equalVertices(const PlanarSurface& other) const;

    /// Check if this planar surface's vertices are reverse equal to the other's in the building coordinate system
    bool reverseEqualVertices(const PlanarSurface& other) const;

    /// Get the plane, throws openstudio::Exception if cannot compute plane for this surface.
    Plane plane() const;

    /// Get a triangulation of this surface, subsurfaces will be replaced by holes in the triangulation
    std::vector<std::vector<Point3d> > triangulation() const;

    /// Return the centroid of this planar surface's vertices
    Point3d centroid() const;

    //@}
   protected:

    boost::optional<ModelObject> spaceAsModelObject() const;

   private slots:

    void clearCachedVariables();

   private:

    REGISTER_LOGGER("openstudio.model.PlanarSurface");

    boost::optional<ModelObject> constructionAsModelObject() const;
    boost::optional<ModelObject> planarSurfaceGroupAsModelObject() const;

    bool setConstructionAsModelObject(boost::optional<ModelObject> modelObject);

    mutable boost::optional<std::vector<Point3d> > m_cachedVertices;
    mutable boost::optional<Plane> m_cachedPlane;
    mutable boost::optional<Vector3d> m_cachedOutwardNormal;
    mutable std::vector<std::vector<Point3d> > m_cachedTriangulation;

  };

} // detail
} // model
} // openstudio

#endif
