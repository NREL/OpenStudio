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

#ifndef MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP
#define MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class InteriorPartitionSurface;
  class InteriorPartitionSurfaceGroup;

  namespace detail {

    /** InteriorPartitionSurfaceGroup_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for InteriorPartitionSurfaceGroup.*/
    class MODEL_API InteriorPartitionSurfaceGroup_Impl : public PlanarSurfaceGroup_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InteriorPartitionSurfaceGroup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurfaceGroup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurfaceGroup_Impl(const InteriorPartitionSurfaceGroup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InteriorPartitionSurfaceGroup_Impl() = default;

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

      // TODO: Handle Non-Extensible IddField Space Name.

      virtual double directionofRelativeNorth() const override;

      virtual bool isDirectionofRelativeNorthDefaulted() const override;

      virtual double xOrigin() const override;

      virtual bool isXOriginDefaulted() const override;

      virtual double yOrigin() const override;

      virtual bool isYOriginDefaulted() const override;

      virtual double zOrigin() const override;

      virtual bool isZOriginDefaulted() const override;

      int multiplier() const;

      bool isMultiplierDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) override;

      virtual void resetDirectionofRelativeNorth() override;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) override;

      virtual void resetXOrigin() override;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) override;

      virtual void resetYOrigin() override;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) override;

      virtual void resetZOrigin() override;

      bool setMultiplier(int multiplier);

      void resetMultiplier();

      //@}

      /// Returns the parent Space.
      boost::optional<Space> space() const;

      /// Sets the parent Space.
      bool setSpace(const Space& space);

      void resetSpace();

      /// Returns all \link InteriorPartitionSurface InteriorPartitionSurfaces \endlink in this group.
      std::vector<InteriorPartitionSurface> interiorPartitionSurfaces() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.InteriorPartitionSurfaceGroup");

      boost::optional<ModelObject> spaceAsModelObject() const;
      std::vector<ModelObject> interiorPartitionSurfacesAsModelObjects() const;

      bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACEGROUP_IMPL_HPP
