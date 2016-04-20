/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
class GeneratorPhotovoltaic;
class SurfacePropertyConvectionCoefficients;

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

    virtual boost::optional<ConstructionBase> construction() const;

    virtual boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance() const = 0;

    virtual bool isConstructionDefaulted() const = 0;

    virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const = 0;

    virtual boost::optional<Space> space() const = 0;

    std::vector<Point3d> vertices() const;

    //@}
    /** @name Setters */

    virtual bool setVertices(const std::vector<Point3d>& vertices);

    virtual bool setConstruction(const ConstructionBase& construction) = 0;

    virtual void resetConstruction() = 0;

    virtual bool setUFactor(double value);

    bool setUFactor(boost::optional<double> value);

    virtual bool setThermalConductance(double value);

    bool setThermalConductance(boost::optional<double> value);

    //@}
    /** @name Queries */
    //@{

    bool isAirWall() const;

    double grossArea() const;

    virtual bool subtractFromGrossArea() const = 0;

    double netArea() const;

    Vector3d outwardNormal() const;

    double tilt() const;

    double azimuth() const;

    virtual boost::optional<double> uFactor() const;

    virtual boost::optional<double> thermalConductance() const;

    boost::optional<double> heatCapacity() const;

    boost::optional<double> interiorVisibleAbsorptance() const;

    boost::optional<double> exteriorVisibleAbsorptance() const;

    boost::optional<double> visibleTransmittance() const;

    bool equalVertices(const PlanarSurface& other) const;

    bool reverseEqualVertices(const PlanarSurface& other) const;

    Plane plane() const;

    std::vector<std::vector<Point3d> > triangulation() const;

    Point3d centroid() const;

    std::vector<ModelObject> solarCollectors() const;

    std::vector<GeneratorPhotovoltaic> generatorPhotovoltaics() const;
    
    std::vector<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

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
