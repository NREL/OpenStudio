/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP

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

    /** AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl is a HVACComponent_Impl that is the implementation class for AirConditionerVariableRefrigerantFlowFluidTemperatureControl.*/
    class MODEL_API AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                        bool keepHandle);

      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
        const AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl() {}

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

      double minimumOutdoorAirTemperatureinCoolingMode() const;

      double maximumOutdoorAirTemperatureinCoolingMode() const;

      double minimumOutdoorAirTemperatureinHeatingMode() const;

      double maximumOutdoorAirTemperatureinHeatingMode() const;

      double referenceOutdoorUnitSuperheating() const;

      double referenceOutdoorUnitSubcooling() const;

      std::string refrigerantTemperatureControlAlgorithmforIndoorUnit() const;

      double referenceEvaporatingTemperatureforIndoorUnit() const;

      double referenceCondensingTemperatureforIndoorUnit() const;

      double variableEvaporatingTemperatureMinimumforIndoorUnit() const;

      double variableEvaporatingTemperatureMaximumforIndoorUnit() const;

      double variableCondensingTemperatureMinimumforIndoorUnit() const;

      double variableCondensingTemperatureMaximumforIndoorUnit() const;

      double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const;

      double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const;

      Curve outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

      Curve outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

      double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

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

      double compressorMaximumDeltaPressure() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRefrigerantType(const std::string& refrigerantType);

      bool setRatedEvaporativeCapacity(double ratedEvaporativeCapacity);

      void autosizeRatedEvaporativeCapacity();

      bool setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);

      bool setMinimumOutdoorAirTemperatureinCoolingMode(double minimumOutdoorAirTemperatureinCoolingMode);

      bool setMaximumOutdoorAirTemperatureinCoolingMode(double maximumOutdoorAirTemperatureinCoolingMode);

      bool setMinimumOutdoorAirTemperatureinHeatingMode(double minimumOutdoorAirTemperatureinHeatingMode);

      bool setMaximumOutdoorAirTemperatureinHeatingMode(double maximumOutdoorAirTemperatureinHeatingMode);

      bool setReferenceOutdoorUnitSuperheating(double referenceOutdoorUnitSuperheating);

      bool setReferenceOutdoorUnitSubcooling(double referenceOutdoorUnitSubcooling);

      bool setRefrigerantTemperatureControlAlgorithmforIndoorUnit(const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit);

      bool setReferenceEvaporatingTemperatureforIndoorUnit(double referenceEvaporatingTemperatureforIndoorUnit);

      bool setReferenceCondensingTemperatureforIndoorUnit(double referenceCondensingTemperatureforIndoorUnit);

      bool setVariableEvaporatingTemperatureMinimumforIndoorUnit(double variableEvaporatingTemperatureMinimumforIndoorUnit);

      bool setVariableEvaporatingTemperatureMaximumforIndoorUnit(double variableEvaporatingTemperatureMaximumforIndoorUnit);

      bool setVariableCondensingTemperatureMinimumforIndoorUnit(double variableCondensingTemperatureMinimumforIndoorUnit);

      bool setVariableCondensingTemperatureMaximumforIndoorUnit(double variableCondensingTemperatureMaximumforIndoorUnit);

      bool setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);

      bool setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);

      bool setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

      bool setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

      bool setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

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

      bool setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure);

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
      REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlowFluidTemperatureControl");

      boost::optional<ModelObjectList> optionalLoadingIndexList() const;  // For children()
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;
      boost::optional<Curve> optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
