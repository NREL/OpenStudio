/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGSYSTEM_IMPL_HPP
#define MODEL_SIZINGSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVAC;

  namespace detail {

    /** SizingSystem_Impl is a ModelObject_Impl that is the implementation class for SizingSystem.*/
    class MODEL_API SizingSystem_Impl : public ModelObject_Impl
    {

     public:
      SizingSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SizingSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SizingSystem_Impl(const SizingSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SizingSystem_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool setParent(ParentObject& newParent) override;

      std::string typeofLoadtoSizeOn() const;

      bool isTypeofLoadtoSizeOnDefaulted() const;

      boost::optional<double> designOutdoorAirFlowRate() const;

      bool isDesignOutdoorAirFlowRateDefaulted() const;

      bool isDesignOutdoorAirFlowRateAutosized() const;

      boost::optional<double> centralHeatingMaximumSystemAirFlowRatio() const;

      bool isCentralHeatingMaximumSystemAirFlowRatioDefaulted() const;

      bool isCentralHeatingMaximumSystemAirFlowRatioAutosized() const;

      double preheatDesignTemperature() const;

      double preheatDesignHumidityRatio() const;

      double precoolDesignTemperature() const;

      double precoolDesignHumidityRatio() const;

      double centralCoolingDesignSupplyAirTemperature() const;

      double centralHeatingDesignSupplyAirTemperature() const;

      std::string sizingOption() const;

      bool isSizingOptionDefaulted() const;

      bool allOutdoorAirinCooling() const;

      bool isAllOutdoorAirinCoolingDefaulted() const;

      bool allOutdoorAirinHeating() const;

      bool isAllOutdoorAirinHeatingDefaulted() const;

      double centralCoolingDesignSupplyAirHumidityRatio() const;

      bool isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const;

      double centralHeatingDesignSupplyAirHumidityRatio() const;

      bool isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const;

      std::string coolingDesignAirFlowMethod() const;

      bool isCoolingDesignAirFlowMethodDefaulted() const;

      double coolingDesignAirFlowRate() const;

      bool isCoolingDesignAirFlowRateDefaulted() const;

      std::string heatingDesignAirFlowMethod() const;

      bool isHeatingDesignAirFlowMethodDefaulted() const;

      double heatingDesignAirFlowRate() const;

      bool isHeatingDesignAirFlowRateDefaulted() const;

      std::string systemOutdoorAirMethod() const;

      bool isSystemOutdoorAirMethodDefaulted() const;

      double zoneMaximumOutdoorAirFraction() const;

      double coolingSupplyAirFlowRatePerFloorArea() const;

      double coolingFractionofAutosizedCoolingSupplyAirFlowRate() const;

      double coolingSupplyAirFlowRatePerUnitCoolingCapacity() const;

      double heatingSupplyAirFlowRatePerFloorArea() const;

      double heatingFractionofAutosizedHeatingSupplyAirFlowRate() const;

      double heatingFractionofAutosizedCoolingSupplyAirFlowRate() const;

      double heatingSupplyAirFlowRatePerUnitHeatingCapacity() const;

      std::string coolingDesignCapacityMethod() const;

      boost::optional<double> coolingDesignCapacity() const;

      bool isCoolingDesignCapacityAutosized() const;

      double coolingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedCoolingDesignCapacity() const;

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      std::string centralCoolingCapacityControlMethod() const;

      boost::optional<double> occupantDiversity() const;

      bool isOccupantDiversityAutosized() const;

      bool setTypeofLoadtoSizeOn(const std::string& typeofLoadtoSizeOn);

      void resetTypeofLoadtoSizeOn();

      bool setDesignOutdoorAirFlowRate(boost::optional<double> designOutdoorAirFlowRate);

      void resetDesignOutdoorAirFlowRate();

      void autosizeDesignOutdoorAirFlowRate();

      bool setCentralHeatingMaximumSystemAirFlowRatio(boost::optional<double> centralHeatingMaximumSystemAirFlowRatio);

      void resetCentralHeatingMaximumSystemAirFlowRatio();

      void autosizeCentralHeatingMaximumSystemAirFlowRatio();

      bool setPreheatDesignTemperature(double preheatDesignTemperature);

      bool setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

      bool setPrecoolDesignTemperature(double precoolDesignTemperature);

      bool setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

      bool setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature);

      bool setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature);

      bool setSizingOption(const std::string& sizingOption);

      void resetSizingOption();

      bool setAllOutdoorAirinCooling(bool allOutdoorAirinCooling);

      void resetAllOutdoorAirinCooling();

      bool setAllOutdoorAirinHeating(bool allOutdoorAirinHeating);

      void resetAllOutdoorAirinHeating();

      bool setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio);

      void resetCentralCoolingDesignSupplyAirHumidityRatio();

      bool setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio);

      void resetCentralHeatingDesignSupplyAirHumidityRatio();

      bool setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod);

      void resetCoolingDesignAirFlowMethod();

      bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

      void resetCoolingDesignAirFlowRate();

      bool setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod);

      void resetHeatingDesignAirFlowMethod();

      bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

      void resetHeatingDesignAirFlowRate();

      bool setSystemOutdoorAirMethod(const std::string& systemOutdoorAirMethod);

      void resetSystemOutdoorAirMethod();

      bool setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction);

      bool setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea);

      bool setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate);

      bool setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity);

      bool setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea);

      bool setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate);

      bool setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate);

      bool setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity);

      bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

      bool setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity);

      void autosizeCoolingDesignCapacity();

      bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setCentralCoolingCapacityControlMethod(const std::string& centralCoolingCapacityControlMethod);

      bool setOccupantDiversity(double occupantDiversity);

      void autosizeOccupantDiversity();

      AirLoopHVAC airLoopHVAC() const;

      bool setAirLoopHVAC(const AirLoopHVAC& airLoopHVAC);

      boost::optional<double> autosizedDesignOutdoorAirFlowRate() const;

      boost::optional<double> autosizedCentralHeatingMaximumSystemAirFlowRatio() const;

      boost::optional<double> autosizedCoolingDesignCapacity() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      boost::optional<double> autosizedOccupantDiversity() const;

      void autosize();

      void applySizingValues();

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SizingSystem");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SIZINGSYSTEM_IMPL_HPP
