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

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_IMPL_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class ModelObjectList;
  class ZoneHVACTerminalUnitVariableRefrigerantFlow;
  class LoadingIndex;

  namespace detail {

    /** AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl is a HVACComponent_Impl that is the implementation class for AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR.*/
    class MODEL_API AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                          bool keepHandle);

      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
        const AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      std::string refrigerantType() const;

      boost::optional<double> ratedEvaporativeCapacity() const;

      bool isRatedEvaporativeCapacityAutosized() const;

      boost::optional<double> autosizedRatedEvaporativeCapacity();

      double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const;

      double minimumOutdoorAirTemperatureinCoolingOnlyMode() const;

      double maximumOutdoorAirTemperatureinCoolingOnlyMode() const;

      double minimumOutdoorAirTemperatureinHeatingOnlyMode() const;

      double maximumOutdoorAirTemperatureinHeatingOnlyMode() const;

      double minimumOutdoorTemperatureinHeatRecoveryMode() const;

      double maximumOutdoorTemperatureinHeatRecoveryMode() const;

      std::string refrigerantTemperatureControlAlgorithmforIndoorUnit() const;

      double referenceEvaporatingTemperatureforIndoorUnit() const;

      double referenceCondensingTemperatureforIndoorUnit() const;

      double variableEvaporatingTemperatureMinimumforIndoorUnit() const;

      double variableEvaporatingTemperatureMaximumforIndoorUnit() const;

      double variableCondensingTemperatureMinimumforIndoorUnit() const;

      double variableCondensingTemperatureMaximumforIndoorUnit() const;

      double outdoorUnitEvaporatorReferenceSuperheating() const;

      double outdoorUnitCondenserReferenceSubcooling() const;

      double outdoorUnitEvaporatorRatedBypassFactor() const;

      double outdoorUnitCondenserRatedBypassFactor() const;

      double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode() const;

      double outdoorUnitHeatExchangerCapacityRatio() const;

      double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const;

      double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const;

      Curve outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

      Curve outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

      double diameterofMainPipeforSuctionGas() const;

      double diameterofMainPipeforDischargeGas() const;

      double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

      double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

      double heightDifferenceBetweenOutdoorUnitandIndoorUnits() const;

      double mainPipeInsulationThickness() const;

      double mainPipeInsulationThermalConductivity() const;

      double crankcaseHeaterPowerperCompressor() const;

      int numberofCompressors() const;

      double ratioofCompressorSizetoTotalCompressorCapacity() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const;

      std::string defrostStrategy() const;

      std::string defrostControl() const;

      boost::optional<Curve> defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const;

      double defrostTimePeriodFraction() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity();

      double maximumOutdoorDrybulbTemperatureforDefrostOperation() const;

      double initialHeatRecoveryCoolingCapacityFraction() const;

      double heatRecoveryCoolingCapacityTimeConstant() const;

      double initialHeatRecoveryCoolingEnergyFraction() const;

      double heatRecoveryCoolingEnergyTimeConstant() const;

      double initialHeatRecoveryHeatingCapacityFraction() const;

      double heatRecoveryHeatingCapacityTimeConstant() const;

      double initialHeatRecoveryHeatingEnergyFraction() const;

      double heatRecoveryHeatingEnergyTimeConstant() const;

      double compressorMaximumDeltaPressure() const;

      double compressorInverterEfficiency() const;

      double compressorEvaporativeCapacityCorrectionFactor() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRefrigerantType(const std::string& refrigerantType);

      bool setRatedEvaporativeCapacity(double ratedEvaporativeCapacity);

      void autosizeRatedEvaporativeCapacity();

      bool setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);

      bool setMinimumOutdoorAirTemperatureinCoolingOnlyMode(double minimumOutdoorAirTemperatureinCoolingOnlyMode);

      bool setMaximumOutdoorAirTemperatureinCoolingOnlyMode(double maximumOutdoorAirTemperatureinCoolingOnlyMode);

      bool setMinimumOutdoorAirTemperatureinHeatingOnlyMode(double minimumOutdoorAirTemperatureinHeatingOnlyMode);

      bool setMaximumOutdoorAirTemperatureinHeatingOnlyMode(double maximumOutdoorAirTemperatureinHeatingOnlyMode);

      bool setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);

      bool setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);

      bool setRefrigerantTemperatureControlAlgorithmforIndoorUnit(const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit);

      bool setReferenceEvaporatingTemperatureforIndoorUnit(double referenceEvaporatingTemperatureforIndoorUnit);

      bool setReferenceCondensingTemperatureforIndoorUnit(double referenceCondensingTemperatureforIndoorUnit);

      bool setVariableEvaporatingTemperatureMinimumforIndoorUnit(double variableEvaporatingTemperatureMinimumforIndoorUnit);

      bool setVariableEvaporatingTemperatureMaximumforIndoorUnit(double variableEvaporatingTemperatureMaximumforIndoorUnit);

      bool setVariableCondensingTemperatureMinimumforIndoorUnit(double variableCondensingTemperatureMinimumforIndoorUnit);

      bool setVariableCondensingTemperatureMaximumforIndoorUnit(double variableCondensingTemperatureMaximumforIndoorUnit);

      bool setOutdoorUnitEvaporatorReferenceSuperheating(double outdoorUnitEvaporatorReferenceSuperheating);

      bool setOutdoorUnitCondenserReferenceSubcooling(double outdoorUnitCondenserReferenceSubcooling);

      bool setOutdoorUnitEvaporatorRatedBypassFactor(double outdoorUnitEvaporatorRatedBypassFactor);

      bool setOutdoorUnitCondenserRatedBypassFactor(double outdoorUnitCondenserRatedBypassFactor);

      bool setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(
        double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode);

      bool setOutdoorUnitHeatExchangerCapacityRatio(double outdoorUnitHeatExchangerCapacityRatio);

      bool setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);

      bool setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);

      bool setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

      bool setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

      bool setDiameterofMainPipeforSuctionGas(double diameterofMainPipeforSuctionGas);

      bool setDiameterofMainPipeforDischargeGas(double diameterofMainPipeforDischargeGas);

      bool setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

      bool setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
        double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

      bool setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(double heightDifferenceBetweenOutdoorUnitandIndoorUnits);

      bool setMainPipeInsulationThickness(double mainPipeInsulationThickness);

      bool setMainPipeInsulationThermalConductivity(double mainPipeInsulationThermalConductivity);

      bool setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);

      bool setNumberofCompressors(int numberofCompressors);

      bool setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(double maximumOutdoorDryBulbTemperatureforCrankcaseHeater);

      bool setDefrostStrategy(const std::string& defrostStrategy);

      bool setDefrostControl(const std::string& defrostControl);

      bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve);

      void resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

      void autosizeResistiveDefrostHeaterCapacity();

      bool setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

      bool setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);

      bool setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);

      bool setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);

      bool setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);

      bool setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);

      bool setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);

      bool setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);

      bool setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

      bool setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure);

      bool setCompressorInverterEfficiency(double compressorInverterEfficiency);

      bool setCompressorEvaporativeCapacityCorrectionFactor(double compressorEvaporativeCapacityCorrectionFactor);

      //@}
      /** @name Other */
      //@{

      ModelObjectList vrfModelObjectList() const;

      bool setVRFModelObjectList(const ModelObjectList& modelObjectList);

      bool addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

      void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

      void removeAllTerminals();

      std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

      ModelObjectList loadingIndexList() const;

      bool setLoadingIndexList(const ModelObjectList& modelObjectList);

      bool addLoadingIndex(LoadingIndex& loadingIndex);

      void removeLoadingIndex(LoadingIndex& loadingIndex);

      void removeAllLoadingIndexes();

      std::vector<LoadingIndex> loadingIndexes() const;

      boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR");

      boost::optional<ModelObjectList> optionalLoadingIndexList() const;  // For children()
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;
      boost::optional<Curve> optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_IMPL_HPP
