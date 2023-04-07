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

#ifndef MODEL_HEATPUMPPLANTLOOPEIRCOOLING_IMPL_HPP
#define MODEL_HEATPUMPPLANTLOOPEIRCOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class HeatPumpPlantLoopEIRHeating;
  class Curve;

  namespace detail {

    /** HeatPumpPlantLoopEIRCooling_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpPlantLoopEIRCooling.*/
    class MODEL_API HeatPumpPlantLoopEIRCooling_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpPlantLoopEIRCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpPlantLoopEIRCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpPlantLoopEIRCooling_Impl(const HeatPumpPlantLoopEIRCooling_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpPlantLoopEIRCooling_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned supplyInletPort() const override;

      virtual unsigned supplyOutletPort() const override;

      virtual unsigned demandInletPort() const override;

      virtual unsigned demandOutletPort() const override;

      /* This function will call the base class' method WaterToWaterComponent_Impl::addToNode()
       * If this is connecting to the demand side of a loop, will set the condenserType to 'WaterSource'
       */
      virtual bool addToNode(Node& node) override;

      /** Override to switch the condenser type to 'AirSource' **/
      virtual bool removeFromSecondaryPlantLoop() override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string condenserType() const;

      boost::optional<HeatPumpPlantLoopEIRHeating> companionHeatingHeatPump() const;

      boost::optional<double> loadSideReferenceFlowRate() const;

      bool isLoadSideReferenceFlowRateAutosized() const;

      boost::optional<double> sourceSideReferenceFlowRate() const;

      bool isSourceSideReferenceFlowRateAutosized() const;

      boost::optional<double> referenceCapacity() const;

      bool isReferenceCapacityAutosized() const;

      double referenceCoefficientofPerformance() const;

      double sizingFactor() const;

      Curve capacityModifierFunctionofTemperatureCurve() const;

      Curve electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const;

      Curve electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setCondenserType(const std::string& condenserType);

      bool setCompanionHeatingHeatPump(const HeatPumpPlantLoopEIRHeating& companionHP);

      bool setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate);

      void autosizeLoadSideReferenceFlowRate();

      bool setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate);

      void autosizeSourceSideReferenceFlowRate();

      bool setReferenceCapacity(double referenceCapacity);

      void autosizeReferenceCapacity();

      bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

      bool setSizingFactor(double sizingFactor);

      bool setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve);

      bool setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve);

      bool setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
        const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedLoadSideReferenceFlowRate() const;

      boost::optional<double> autosizedSourceSideReferenceFlowRate() const;

      boost::optional<double> autosizedReferenceCapacity() const;

      /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
      boost::optional<PlantLoop> loadSideWaterLoop() const;

      /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
      boost::optional<PlantLoop> sourceSideWaterLoop() const;

      // Convenience function to return the inletNode on the Source Side
      boost::optional<Node> sourceSideWaterInletNode() const;
      // Convenience function to return the outletNode on the Source Side
      boost::optional<Node> sourceSideWaterOutletNode() const;

      // Convenience function to return the inletNode on the Load Side
      boost::optional<Node> loadSideWaterInletNode() const;
      // Convenience function to return the outletNode on the Load Side
      boost::optional<Node> loadSideWaterOutletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpPlantLoopEIRCooling");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPPLANTLOOPEIRCOOLING_IMPL_HPP
