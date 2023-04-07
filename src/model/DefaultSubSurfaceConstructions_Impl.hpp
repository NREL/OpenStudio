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

#ifndef MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP
#define MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;
  class DefaultSubSurfaceConstructions;

  namespace detail {

    /** DefaultSubSurfaceConstructions_Impl is a ResourceObject_Impl that is the implementation class for DefaultSubSurfaceConstructions.*/
    class MODEL_API DefaultSubSurfaceConstructions_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      DefaultSubSurfaceConstructions_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DefaultSubSurfaceConstructions_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DefaultSubSurfaceConstructions_Impl(const DefaultSubSurfaceConstructions_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DefaultSubSurfaceConstructions_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<ConstructionBase> fixedWindowConstruction() const;

      boost::optional<ConstructionBase> operableWindowConstruction() const;

      boost::optional<ConstructionBase> doorConstruction() const;

      boost::optional<ConstructionBase> glassDoorConstruction() const;

      boost::optional<ConstructionBase> overheadDoorConstruction() const;

      boost::optional<ConstructionBase> skylightConstruction() const;

      boost::optional<ConstructionBase> tubularDaylightDomeConstruction() const;

      boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction() const;

      //@}
      /** @name Setters */
      //@{

      bool setFixedWindowConstruction(const ConstructionBase& construction);

      void resetFixedWindowConstruction();

      bool setOperableWindowConstruction(const ConstructionBase& construction);

      void resetOperableWindowConstruction();

      bool setDoorConstruction(const ConstructionBase& construction);

      void resetDoorConstruction();

      bool setGlassDoorConstruction(const ConstructionBase& construction);

      void resetGlassDoorConstruction();

      bool setOverheadDoorConstruction(const ConstructionBase& construction);

      void resetOverheadDoorConstruction();

      bool setSkylightConstruction(const ConstructionBase& construction);

      void resetSkylightConstruction();

      bool setTubularDaylightDomeConstruction(const ConstructionBase& construction);

      void resetTubularDaylightDomeConstruction();

      bool setTubularDaylightDiffuserConstruction(const ConstructionBase& construction);

      void resetTubularDaylightDiffuserConstruction();

      //@}

      /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
      void merge(const DefaultSubSurfaceConstructions& other);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DefaultSubSurfaceConstructions");

      boost::optional<ModelObject> fixedWindowConstructionAsModelObject() const;
      boost::optional<ModelObject> operableWindowConstructionAsModelObject() const;
      boost::optional<ModelObject> doorConstructionAsModelObject() const;
      boost::optional<ModelObject> glassDoorConstructionAsModelObject() const;
      boost::optional<ModelObject> overheadDoorConstructionAsModelObject() const;
      boost::optional<ModelObject> skylightConstructionAsModelObject() const;
      boost::optional<ModelObject> tubularDaylightDomeConstructionAsModelObject() const;
      boost::optional<ModelObject> tubularDaylightDiffuserConstructionAsModelObject() const;

      bool setFixedWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setOperableWindowConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setGlassDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setOverheadDoorConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setSkylightConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setTubularDaylightDomeConstructionAsModelObject(boost::optional<ModelObject> modelObject);
      bool setTubularDaylightDiffuserConstructionAsModelObject(boost::optional<ModelObject> modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_IMPL_HPP
