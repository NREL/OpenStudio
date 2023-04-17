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

#ifndef MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP
#define MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;
  class ConstructionBase;
  class DefaultSubSurfaceConstructions;
  class DefaultSurfaceConstructions;
  class DefaultConstructionSet;

  namespace detail {

    /** DefaultConstructionSet_Impl is a ResourceObject_Impl that is the implementation class for DefaultConstructionSet.*/
    class MODEL_API DefaultConstructionSet_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultConstructionSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultConstructionSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultConstructionSet_Impl(const DefaultConstructionSet_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultConstructionSet_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<DefaultSurfaceConstructions> defaultExteriorSurfaceConstructions() const;

      boost::optional<DefaultSurfaceConstructions> defaultInteriorSurfaceConstructions() const;

      boost::optional<DefaultSurfaceConstructions> defaultGroundContactSurfaceConstructions() const;

      boost::optional<DefaultSubSurfaceConstructions> defaultExteriorSubSurfaceConstructions() const;

      boost::optional<DefaultSubSurfaceConstructions> defaultInteriorSubSurfaceConstructions() const;

      boost::optional<ConstructionBase> interiorPartitionConstruction() const;

      boost::optional<ConstructionBase> spaceShadingConstruction() const;

      boost::optional<ConstructionBase> buildingShadingConstruction() const;

      boost::optional<ConstructionBase> siteShadingConstruction() const;

      boost::optional<ConstructionBase> adiabaticSurfaceConstruction() const;

      //@}
      /** @name Setters */
      //@{

      bool setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

      void resetDefaultExteriorSurfaceConstructions();

      bool setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

      void resetDefaultInteriorSurfaceConstructions();

      bool setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

      void resetDefaultGroundContactSurfaceConstructions();

      bool setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

      void resetDefaultExteriorSubSurfaceConstructions();

      bool setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

      void resetDefaultInteriorSubSurfaceConstructions();

      bool setInteriorPartitionConstruction(const ConstructionBase& construction);

      void resetInteriorPartitionConstruction();

      bool setSpaceShadingConstruction(const ConstructionBase& construction);

      void resetSpaceShadingConstruction();

      bool setBuildingShadingConstruction(const ConstructionBase& construction);

      void resetBuildingShadingConstruction();

      bool setSiteShadingConstruction(const ConstructionBase& construction);

      void resetSiteShadingConstruction();

      bool setAdiabaticSurfaceConstruction(const ConstructionBase& construction);

      void resetAdiabaticSurfaceConstruction();

      //@}

      /// Returns the default construction for this planar surface if available.
      boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;

      /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
      void merge(const DefaultConstructionSet& other);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DefaultConstructionSet");

      boost::optional<ModelObject> defaultExteriorSurfaceConstructionsAsModelObject() const;
      boost::optional<ModelObject> defaultInteriorSurfaceConstructionsAsModelObject() const;
      boost::optional<ModelObject> defaultGroundContactSurfaceConstructionsAsModelObject() const;
      boost::optional<ModelObject> defaultExteriorSubSurfaceConstructionsAsModelObject() const;
      boost::optional<ModelObject> defaultInteriorSubSurfaceConstructionsAsModelObject() const;
      boost::optional<ModelObject> interiorPartitionConstructionAsModelObject() const;
      boost::optional<ModelObject> spaceShadingConstructionAsModelObject() const;
      boost::optional<ModelObject> buildingShadingConstructionAsModelObject() const;
      boost::optional<ModelObject> siteShadingConstructionAsModelObject() const;
      boost::optional<ModelObject> adiabaticSurfaceConstructionAsModelObject() const;

      bool setDefaultExteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultInteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultGroundContactSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultExteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultInteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setInteriorPartitionConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSpaceShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBuildingShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSiteShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAdiabaticSurfaceConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTCONSTRUCTIONSET_IMPL_HPP
