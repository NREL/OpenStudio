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

#ifndef MODEL_SHADINGSURFACEGROUP_IMPL_HPP
#define MODEL_SHADINGSURFACEGROUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class ShadingSurface;
  class ShadingSurfaceGroup;
  class Surface;
  class SubSurface;

  namespace detail {

    /** ShadingSurfaceGroup_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for ShadingSurfaceGroup.*/
    class MODEL_API ShadingSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ShadingSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ShadingSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ShadingSurfaceGroup_Impl(const ShadingSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ShadingSurfaceGroup_Impl() = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual openstudio::Transformation buildingTransformation() const override;

      virtual bool changeTransformation(const openstudio::Transformation& transformation) override;

      virtual openstudio::BoundingBox boundingBox() const override;

      /** @name Getters */
      //@{

      std::string shadingSurfaceType() const;

      virtual double directionofRelativeNorth() const override;

      virtual bool isDirectionofRelativeNorthDefaulted() const override;

      virtual double xOrigin() const override;

      virtual bool isXOriginDefaulted() const override;

      virtual double yOrigin() const override;

      virtual bool isYOriginDefaulted() const override;

      virtual double zOrigin() const override;

      virtual bool isZOriginDefaulted() const override;

      //@}
      /** @name Setters */
      //@{

      bool setShadingSurfaceType(const std::string& shadingSurfaceType);

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) override;

      virtual void resetDirectionofRelativeNorth() override;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) override;

      virtual void resetXOrigin() override;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) override;

      virtual void resetYOrigin() override;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) override;

      virtual void resetZOrigin() override;

      //@}

      /// Returns the parent Space.
      boost::optional<Space> space() const;

      /// Sets the parent Space.
      bool setSpace(const Space& space);

      /** If (space()), calls setShadingSurfaceType("Building") to remove the relationship. */
      void resetSpace();

      /// Returns child shading surfaces.
      std::vector<ShadingSurface> shadingSurfaces() const;

      /** Returns the specific sub surface shaded by this group if specified. */
      boost::optional<SubSurface> shadedSubSurface() const;

      /** Returns the surface shaded by this group if specified. */
      boost::optional<Surface> shadedSurface() const;

      /** Sets the specifc sub surface shaded by this group, sub surface must be in the same parent space. */
      bool setShadedSubSurface(const SubSurface& subSurface);

      /** Sets the surface shaded by this group, surface must be in the same parent space. */
      bool setShadedSurface(const Surface& surface);

      /** Resets the sub surface shaded by this group. */
      void resetShadedObject();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ShadingSurfaceGroup");

      boost::optional<ModelObject> spaceAsModelObject() const;
      std::vector<ModelObject> shadingSurfacesAsModelObjects() const;

      bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACEGROUP_IMPL_HPP
