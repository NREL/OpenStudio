/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANARSURFACE_IMPL_HPP
#define MODEL_PLANARSURFACE_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

#include <nano/nano_signal_slot.hpp>  // Signal-Slot replacement

namespace openstudio {

class Surface3d;

namespace model {

  class PlanarSurfaceGroup;
  class PlanarSurface;
  class Space;
  class ConstructionBase;
  class GeneratorPhotovoltaic;
  class SurfacePropertyConvectionCoefficients;

  namespace detail {

    // derive PlanarSurface_Impl from ParentObject_Impl to override virtual methods
    class MODEL_API PlanarSurface_Impl : public ParentObject_Impl
    {

      // includes film coefficients. may require simulation results.

      // does not include film coefficients. may require simulation results.

      // may require simulation results.

      // may require simulation results.

      // may require simulation results.

      // relationships

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      PlanarSurface_Impl(IddObjectType type, Model_Impl* model);

      // constructor
      PlanarSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      PlanarSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      PlanarSurface_Impl(const PlanarSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlanarSurface_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<ConstructionBase> construction() const;

      virtual boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance() const = 0;

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

      Surface3d surface3d() const;

      bool isConvex() const;

      std::vector<std::vector<Point3d>> triangulation() const;

      Point3d centroid() const;

      std::vector<ModelObject> solarCollectors() const;

      std::vector<GeneratorPhotovoltaic> generatorPhotovoltaics() const;

      std::vector<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

      //@}
     protected:
      boost::optional<ModelObject> spaceAsModelObject() const;

      //private slots:
     private:
      void clearCachedVariables();

     private:
      REGISTER_LOGGER("openstudio.model.PlanarSurface");

      boost::optional<ModelObject> constructionAsModelObject() const;
      boost::optional<ModelObject> planarSurfaceGroupAsModelObject() const;

      bool setConstructionAsModelObject(boost::optional<ModelObject> modelObject);

      mutable boost::optional<std::vector<Point3d>> m_cachedVertices;
      mutable boost::optional<Plane> m_cachedPlane;
      mutable boost::optional<Vector3d> m_cachedOutwardNormal;
      mutable std::vector<std::vector<Point3d>> m_cachedTriangulation;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif
