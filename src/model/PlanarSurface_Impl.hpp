/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_PLANARSURFACE_IMPL_HPP
#define MODEL_PLANARSURFACE_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

#include <nano/nano_signal_slot.hpp>  // Signal-Slot replacement

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

      virtual ~PlanarSurface_Impl() = default;

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
