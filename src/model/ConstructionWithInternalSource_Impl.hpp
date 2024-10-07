/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      virtual ~ConstructionWithInternalSource_Impl() override = default;

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
