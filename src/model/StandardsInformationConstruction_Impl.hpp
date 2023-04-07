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

#ifndef MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP
#define MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class StandardsInformationConstruction;
  class ConstructionBase;
  class Material;

  namespace detail {

    class MODEL_API StandardsInformationConstruction_Impl : public ModelObject_Impl
    {
     public:
      /** Constructors and Destructors */
      //@{

      // constructor
      StandardsInformationConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      StandardsInformationConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      StandardsInformationConstruction_Impl(const StandardsInformationConstruction_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~StandardsInformationConstruction_Impl() = default;

      //@}
      /** @name Getters */
      //@{

      ConstructionBase construction() const;

      boost::optional<std::string> intendedSurfaceType() const;

      boost::optional<std::string> standardsConstructionType() const;

      std::vector<std::string> suggestedStandardsConstructionTypes() const;

      boost::optional<Material> perturbableLayer() const;

      std::string perturbableLayerType() const;

      bool isPerturbableLayerTypeDefaulted() const;

      boost::optional<std::string> otherPerturbableLayerType() const;

      std::vector<std::string> suggestedConstructionStandards() const;

      boost::optional<std::string> constructionStandard() const;

      std::vector<std::string> suggestedConstructionStandardSources() const;

      boost::optional<std::string> constructionStandardSource() const;

      boost::optional<std::string> fenestrationType() const;

      boost::optional<std::string> fenestrationAssemblyContext() const;

      boost::optional<std::string> fenestrationNumberOfPanes() const;

      boost::optional<std::string> fenestrationFrameType() const;

      boost::optional<std::string> fenestrationDividerType() const;

      boost::optional<std::string> fenestrationTint() const;

      boost::optional<std::string> fenestrationGasFill() const;

      bool fenestrationLowEmissivityCoating() const;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call non-const methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Setters */
      //@{

      bool setIntendedSurfaceType(const std::string& type);
      void resetIntendedSurfaceType();

      bool setStandardsConstructionType(const std::string& type);
      void resetStandardsConstructionType();

      bool setPerturbableLayer(unsigned layerIndex);

      bool setPerturbableLayer(const Material& material);

      /** Clears all PerturbableLayer information, including type. */
      void resetPerturbableLayer();

      bool setPerturbableLayerType(const std::string& type);

      void resetPerturbableLayerType();

      bool setOtherPerturbableLayerType(const std::string& otherPerturbableLayerType);

      void resetOtherPerturbableLayerType();

      bool setConstructionStandard(const std::string& constructionStandard);

      void resetConstructionStandard();

      bool setConstructionStandardSource(const std::string& constructionStandardSource);

      void resetConstructionStandardSource();

      bool setFenestrationType(const std::string& fenestrationType);

      void resetFenestrationType();

      bool setFenestrationAssemblyContext(const std::string& fenestrationAssemblyContext);

      void resetFenestrationAssemblyContext();

      bool setFenestrationNumberOfPanes(const std::string& fenestrationNumberOfPanes);

      void resetFenestrationNumberOfPanes();

      bool setFenestrationFrameType(const std::string& fenestrationFrameType);

      void resetFenestrationFrameType();

      bool setFenestrationDividerType(const std::string& fenestrationDividerType);

      void resetFenestrationDividerType();

      bool setFenestrationTint(const std::string& fenestrationTint);

      void resetFenestrationTint();

      bool setFenestrationGasFill(const std::string& fenestrationGasFill);

      void resetFenestrationGasFill();

      bool setFenestrationLowEmissivityCoating(bool fenestrationLowEmissivityCoating);

      void resetFenestrationLowEmissivityCoating();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.StandardsInformationConstruction");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDSINFORMATIONCONSTRUCTION_IMPL_HPP
