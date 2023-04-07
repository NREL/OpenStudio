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

#ifndef MODEL_PLANARSURFACEGROUP_IMPL_HPP
#define MODEL_PLANARSURFACEGROUP_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"

namespace openstudio {

class Point3d;
class BoundingBox;

namespace model {
  namespace detail {

    class MODEL_API PlanarSurfaceGroup_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      PlanarSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      PlanarSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlanarSurfaceGroup_Impl(const PlanarSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~PlanarSurfaceGroup_Impl() = default;

      virtual double directionofRelativeNorth() const = 0;

      virtual bool isDirectionofRelativeNorthDefaulted() const = 0;

      virtual double xOrigin() const = 0;

      virtual bool isXOriginDefaulted() const = 0;

      virtual double yOrigin() const = 0;

      virtual bool isYOriginDefaulted() const = 0;

      virtual double zOrigin() const = 0;

      virtual bool isZOriginDefaulted() const = 0;

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) = 0;

      virtual void resetDirectionofRelativeNorth() = 0;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) = 0;

      virtual void resetXOrigin() = 0;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) = 0;

      virtual void resetYOrigin() = 0;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) = 0;

      virtual void resetZOrigin() = 0;

      /** Returns the transformation from local coordinates to parent coordinates. */
      openstudio::Transformation transformation() const;

      /** Returns the transformation from local coordinates to site coordinates. */
      openstudio::Transformation siteTransformation() const;

      /** Returns the transformation from local coordinates to building coordinates. */
      virtual openstudio::Transformation buildingTransformation() const = 0;

      /** Sets the transformation from local coordinates to parent coordinates,
     *  this method can be used to move the group. */
      bool setTransformation(const openstudio::Transformation& transformation);

      /** Changes the transformation from local coordinates to parent coordinates,
     *  this method alter geometry of children relative to the
     *  group so that it stays in the same place with the new transformation. */
      virtual bool changeTransformation(const openstudio::Transformation& transformation) = 0;

      /** Get the BoundingBox in local coordinates. */
      virtual openstudio::BoundingBox boundingBox() const = 0;

      /** Get the BoundingBox in parent coordinates. */
      openstudio::BoundingBox boundingBoxParentCoordinates() const;

      /** Get the BoundingBox in building coordinates. */
      openstudio::BoundingBox boundingBoxBuildingCoordinates() const;

      /** Get the BoundingBox in site coordinates. */
      openstudio::BoundingBox boundingBoxSiteCoordinates() const;

      //@}
      //private slots:
     private:
      void clearCachedVariables();

     private:
      REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");

      mutable boost::optional<openstudio::Transformation> m_cachedTransformation;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANARSURFACEGROUP_IMPL_HPP
