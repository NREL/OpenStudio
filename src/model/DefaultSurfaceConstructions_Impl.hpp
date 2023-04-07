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

#ifndef MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP
#define MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;
  class DefaultSurfaceConstructions;

  namespace detail {

    /** DefaultSurfaceConstructions_Impl is a ResourceObject_Impl that is the implementation class for DefaultSurfaceConstructions.*/
    class MODEL_API DefaultSurfaceConstructions_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultSurfaceConstructions_Impl(const DefaultSurfaceConstructions_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultSurfaceConstructions_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<ConstructionBase> floorConstruction() const;

      boost::optional<ConstructionBase> wallConstruction() const;

      boost::optional<ConstructionBase> roofCeilingConstruction() const;

      //@}
      /** @name Setters */
      //@{

      bool setFloorConstruction(const ConstructionBase& construction);

      void resetFloorConstruction();

      bool setWallConstruction(const ConstructionBase& construction);

      void resetWallConstruction();

      bool setRoofCeilingConstruction(const ConstructionBase& construction);

      void resetRoofCeilingConstruction();

      //@}

      /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
      void merge(const DefaultSurfaceConstructions& other);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DefaultSurfaceConstructions");

      boost::optional<ModelObject> floorConstructionAsModelObject() const;
      boost::optional<ModelObject> wallConstructionAsModelObject() const;
      boost::optional<ModelObject> roofCeilingConstructionAsModelObject() const;

      bool setFloorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setWallConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setRoofCeilingConstructionAsModelObject(boost::optional<ModelObject> modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSURFACECONSTRUCTIONS_IMPL_HPP
