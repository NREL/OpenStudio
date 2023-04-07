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

#ifndef MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP
#define MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP

#include "LayeredConstruction_Impl.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API ConstructionWithInternalSource_Impl : public LayeredConstruction_Impl
    {

     public:
      // constructor
      ConstructionWithInternalSource_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ConstructionWithInternalSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ConstructionWithInternalSource_Impl(const ConstructionWithInternalSource_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ConstructionWithInternalSource_Impl() = default;

      /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int renderingColorIndex() const override;

      virtual bool eraseLayer(unsigned layerIndex) override;

      virtual bool setLayers(const std::vector<Material>& materials) override;

      virtual bool setLayer(const ModelPartitionMaterial& modelPartitionMaterial) override;

      /// Returns sourcePresentAfterLayerNumber.
      int sourcePresentAfterLayerNumber() const;
      bool setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber);

      /// Returns temperatureCalculationRequestedAfterLayerNumber.
      int temperatureCalculationRequestedAfterLayerNumber() const;
      bool setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber);

      /// Returns dimensionsForTheCTFCalculation.
      int dimensionsForTheCTFCalculation() const;
      bool setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation);

      /// Returns tubeSpacing.
      double tubeSpacing() const;
      bool setTubeSpacing(double tubeSpacing);

      /// Returns twoDimensionalTemperatureCalculationPosition.
      double twoDimensionalTemperatureCalculationPosition() const;
      bool setTwoDimensionalTemperatureCalculationPosition(double twoDimensionalTemperatureCalculationPosition);

      ConstructionWithInternalSource reverseConstructionWithInternalSource() const;

     private:
      REGISTER_LOGGER("openstudio.model.ConstructionWithInternalSource");

      void onNumLayersChanged();
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP
