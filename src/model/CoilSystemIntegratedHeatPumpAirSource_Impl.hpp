/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP
#define MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class HVACComponent;
  class Curve;

  namespace detail {

    /** CoilSystemIntegratedHeatPumpAirSource_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemIntegratedHeatPumpAirSource.*/
    class MODEL_API CoilSystemIntegratedHeatPumpAirSource_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilSystemIntegratedHeatPumpAirSource_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilSystemIntegratedHeatPumpAirSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilSystemIntegratedHeatPumpAirSource_Impl(const CoilSystemIntegratedHeatPumpAirSource_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilSystemIntegratedHeatPumpAirSource_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      // Override to disallow adding to node. It can only be placed inside another component
      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      // Will clone all children coils
      virtual ModelObject clone(Model model) const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      //@}
      /** @name Getters */
      //@{

      StraightComponent spaceCoolingCoil() const;

      StraightComponent spaceHeatingCoil() const;

      boost::optional<HVACComponent> dedicatedWaterHeatingCoil() const;

      boost::optional<HVACComponent> scwhCoil() const;

      boost::optional<StraightComponent> scdwhCoolingCoil() const;

      boost::optional<HVACComponent> scdwhWaterHeatingCoil() const;

      boost::optional<StraightComponent> shdwhHeatingCoil() const;

      boost::optional<HVACComponent> shdwhWaterHeatingCoil() const;

      boost::optional<StraightComponent> enhancedDehumidificationCoolingCoil() const;

      boost::optional<StraightComponent> gridResponseCoolingCoil() const;

      boost::optional<StraightComponent> gridResponseHeatingCoil() const;

      double indoorTemperatureLimitForSCWHMode() const;

      double ambientTemperatureLimitForSCWHMode() const;

      double indoorTemperatureAboveWhichWHHasHigherPriority() const;

      double ambientTemperatureAboveWhichWHHasHigherPriority() const;

      int flagtoIndicateLoadControlInSCWHMode() const;

      int minimumSpeedLevelForSCWHMode() const;

      double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const;

      int minimumSpeedLevelForSCDWHMode() const;

      double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const;

      int minimumSpeedLevelForSHDWHMode() const;

      double sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil() const;

      double sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil() const;

      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil() const;

      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil() const;

      double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const;

      double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil() const;

      double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil() const;

      double sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil() const;

      double sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil() const;

      double sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil() const;

      boost::optional<StraightComponent> chillerCoil() const;

      std::string chillerCoilBelongstoaSingleorSeparateUnit() const;

      int chillerCoilCompressorRunSpeed() const;

      double sizingRatioofChillerCoiltoSpaceCoolingCoil() const;

      boost::optional<HVACComponent> supplementalChillerCoil() const;

      double airFlowRatioofWaterCoiltotheSpaceCoolingCoil() const;

      double waterFlowRatioofWaterCoiltotheChillerCoil() const;

      boost::optional<StraightComponent> storageTank() const;

      double iceFractionBelowWhichChargingStarts() const;

      double chillerEnteringTemperatureatZeroTankFraction() const;

      boost::optional<Curve> temperatureDeviationCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil);

      bool setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil);

      bool setDedicatedWaterHeatingCoil(const boost::optional<HVACComponent>& dedicatedWaterHeatingCoil);

      void resetDedicatedWaterHeatingCoil();

      bool setSCWHCoil(const boost::optional<HVACComponent>& scwhCoil);

      void resetSCWHCoil();

      bool setSCDWHCoolingCoil(const boost::optional<StraightComponent>& scdwhCoolingCoil);

      void resetSCDWHCoolingCoil();

      bool setSCDWHWaterHeatingCoil(const boost::optional<HVACComponent>& scdwhWaterHeatingCoil);

      void resetSCDWHWaterHeatingCoil();

      bool setSHDWHHeatingCoil(const boost::optional<StraightComponent>& shdwhHeatingCoil);

      void resetSHDWHHeatingCoil();

      bool setSHDWHWaterHeatingCoil(const boost::optional<HVACComponent>& shdwhWaterHeatingCoil);

      void resetSHDWHWaterHeatingCoil();

      bool setEnhancedDehumidificationCoolingCoil(const boost::optional<StraightComponent>& enhancedDehumidificationCoolingCoil);

      void resetEnhancedDehumidificationCoolingCoil();

      bool setGridResponseCoolingCoil(const boost::optional<StraightComponent>& gridResponseCoolingCoil);

      void resetGridResponseCoolingCoil();

      bool setGridResponseHeatingCoil(const boost::optional<StraightComponent>& gridResponseHeatingCoil);

      void resetGridResponseHeatingCoil();

      bool setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode);

      bool setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode);

      bool setIndoorTemperatureAboveWhichWHHasHigherPriority(double indoorTemperatureAboveWhichWHHasHigherPriority);

      bool setAmbientTemperatureAboveWhichWHHasHigherPriority(double ambientTemperatureAboveWhichWHHasHigherPriority);

      bool setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode);

      bool setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode);

      bool setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);

      bool setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode);

      bool setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
        double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);

      bool setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode);

      bool setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(double sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil);

      bool setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(double sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil);

      bool setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil);

      bool setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil);

      bool setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);

      bool setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil);

      bool setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(
        double sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil);

      bool setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(double sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil);

      bool setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(double sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil);

      bool setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(double sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil);

      bool setChillerCoil(const boost::optional<StraightComponent>& chillerCoil);

      void resetChillerCoil();

      bool setChillerCoilBelongstoaSingleorSeparateUnit(std::string chillerCoilBelongstoaSingleorSeparateUnit);

      bool setChillerCoilCompressorRunSpeed(int chillerCoilCompressorRunSpeed);

      bool setSizingRatioofChillerCoiltoSpaceCoolingCoil(double sizingRatioofChillerCoiltoSpaceCoolingCoil);

      bool setSupplementalChillerCoil(const boost::optional<HVACComponent>& supplementalChillerCoil);

      void resetSupplementalChillerCoil();

      bool setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(double airFlowRatioofWaterCoiltotheSpaceCoolingCoil);

      bool setWaterFlowRatioofWaterCoiltotheChillerCoil(double waterFlowRatioofWaterCoiltotheChillerCoil);

      bool setStorageTank(const boost::optional<StraightComponent>& storageTank);

      void resetStorageTank();

      bool setIceFractionBelowWhichChargingStarts(double iceFractionBelowWhichChargingStarts);

      bool setChillerEnteringTemperatureatZeroTankFraction(double chillerEnteringTemperatureatZeroTankFraction);

      bool setTemperatureDeviationCurve(const boost::optional<Curve>& temperatureDeviationCurve);

      void resetTemperatureDeviationCurve();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilSystemIntegratedHeatPumpAirSource");

      boost::optional<StraightComponent> optionalSpaceCoolingCoil() const;
      boost::optional<StraightComponent> optionalSpaceHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP
