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

#ifndef MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP
#define MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

  class InteriorPartitionSurfaceGroup;
  class InteriorPartitionSurface;
  class DaylightingDeviceShelf;

  namespace detail {

    /** InteriorPartitionSurface_Impl is a PlanarSurface_Impl that is the implementation class for InteriorPartitionSurface.*/
    class MODEL_API InteriorPartitionSurface_Impl : public PlanarSurface_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InteriorPartitionSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InteriorPartitionSurface_Impl(const InteriorPartitionSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InteriorPartitionSurface_Impl() = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// set the parent, child may have to call methods on the parent
      bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /// should subtract this surface from parent's gross area for net area
      virtual bool subtractFromGrossArea() const override;

      /// get the construction object and search distance
      virtual boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance() const override;

      /// Returns true if the construction is not directly referenced by this surface .
      virtual bool isConstructionDefaulted() const override;

      /// set the construction object
      virtual bool setConstruction(const ConstructionBase& construction) override;

      /// Resets the construction object.
      virtual void resetConstruction() override;

      /// Returns the containing PlanarSurfaceGroup if available.
      virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const override;

      /// Returns the containing Space if available.
      virtual boost::optional<Space> space() const override;

      /** @name Getters */
      //@{

      bool converttoInternalMass() const;

      bool isConverttoInternalMassDefaulted() const;

      boost::optional<double> surfaceArea() const;

      boost::optional<double> numberofVertices() const;

      bool isNumberofVerticesDefaulted() const;

      bool isNumberofVerticesAutocalculated() const;

      // TODO: Handle this object's extensible fields.

      //@}
      /** @name Setters */
      //@{

      bool setConverttoInternalMass(bool converttoInternalMass);

      void resetConverttoInternalMass();

      bool setSurfaceArea(boost::optional<double> surfaceArea);

      bool setSurfaceArea(double surfaceArea);

      void resetSurfaceArea();

      bool setNumberofVertices(boost::optional<double> numberofVertices);

      bool setNumberofVertices(double numberofVertices);

      void resetNumberofVertices();

      void autocalculateNumberofVertices();

      // TODO: Handle this object's extensible fields.

      //@}

      /// get the interior partition surface group
      boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup() const;

      /// set the interior partition surface group
      bool setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup);

      void resetInteriorPartitionSurfaceGroup();

      boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.InteriorPartitionSurface");

      boost::optional<ModelObject> interiorPartitionSurfaceGroupAsModelObject() const;

      bool setInteriorPartitionSurfaceGroupAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACE_IMPL_HPP
