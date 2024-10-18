/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGZONE_IMPL_HPP
#define MODEL_SIZINGZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    /** SizingZone_Impl is a ModelObject_Impl that is the implementation class for SizingZone.*/
    class MODEL_API SizingZone_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SizingZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SizingZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SizingZone_Impl(const SizingZone_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SizingZone_Impl() = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Getters */
      //@{

      ThermalZone thermalZone() const;

      std::string zoneCoolingDesignSupplyAirTemperatureInputMethod() const;

      double zoneCoolingDesignSupplyAirTemperature() const;

      double zoneCoolingDesignSupplyAirTemperatureDifference() const;

      std::string zoneHeatingDesignSupplyAirTemperatureInputMethod() const;

      double zoneHeatingDesignSupplyAirTemperature() const;

      double zoneHeatingDesignSupplyAirTemperatureDifference() const;

      double zoneCoolingDesignSupplyAirHumidityRatio() const;

      double zoneHeatingDesignSupplyAirHumidityRatio() const;

      boost::optional<double> zoneHeatingSizingFactor() const;

      boost::optional<double> zoneCoolingSizingFactor() const;

      std::string coolingDesignAirFlowMethod() const;

      bool isCoolingDesignAirFlowMethodDefaulted() const;

      double coolingDesignAirFlowRate() const;

      bool isCoolingDesignAirFlowRateDefaulted() const;

      double coolingMinimumAirFlowperZoneFloorArea() const;

      bool isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const;

      double coolingMinimumAirFlow() const;

      bool isCoolingMinimumAirFlowDefaulted() const;

      double coolingMinimumAirFlowFraction() const;

      bool isCoolingMinimumAirFlowFractionDefaulted() const;

      std::string heatingDesignAirFlowMethod() const;

      bool isHeatingDesignAirFlowMethodDefaulted() const;

      double heatingDesignAirFlowRate() const;

      bool isHeatingDesignAirFlowRateDefaulted() const;

      double heatingMaximumAirFlowperZoneFloorArea() const;

      bool isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const;

      double heatingMaximumAirFlow() const;

      bool isHeatingMaximumAirFlowDefaulted() const;

      double heatingMaximumAirFlowFraction() const;

      bool isHeatingMaximumAirFlowFractionDefaulted() const;

      bool accountforDedicatedOutdoorAirSystem() const;

      std::string dedicatedOutdoorAirSystemControlStrategy() const;

      boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

      bool isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const;

      boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

      bool isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const;

      std::string zoneLoadSizingMethod() const;

      std::string zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod() const;

      boost::optional<double> zoneDehumidificationDesignSupplyAirHumidityRatio() const;

      double zoneCoolingDesignSupplyAirHumidityRatioDifference() const;

      std::string zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod() const;

      boost::optional<double> zoneHumidificationDesignSupplyAirHumidityRatio() const;

      double zoneHumidificationDesignSupplyAirHumidityRatioDifference() const;

      boost::optional<Schedule> zoneHumidistatDehumidificationSetPointSchedule() const;

      boost::optional<Schedule> zoneHumidistatHumidificationSetPointSchedule() const;

      // Fields from DesignSpecification:ZoneAirDistribution

      double designZoneAirDistributionEffectivenessinCoolingMode() const;

      bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

      double designZoneAirDistributionEffectivenessinHeatingMode() const;

      bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

      double designZoneSecondaryRecirculationFraction() const;

      bool isDesignZoneSecondaryRecirculationFractionDefaulted() const;

      double designMinimumZoneVentilationEfficiency() const;

      bool isDesignMinimumZoneVentilationEfficiencyDefaulted() const;

      std::string sizingOption() const;

      //@}
      /** @name Setters */
      //@{

      bool setThermalZone(const ThermalZone& thermalZone);

      bool setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string& value);

      bool setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

      bool setZoneCoolingDesignSupplyAirTemperatureDifference(double value);

      bool setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string& value);

      bool setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

      bool setZoneHeatingDesignSupplyAirTemperatureDifference(double value);

      bool setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio);

      bool setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio);

      bool setZoneHeatingSizingFactor(boost::optional<double> zoneHeatingSizingFactor);

      void resetZoneHeatingSizingFactor();

      bool setZoneCoolingSizingFactor(boost::optional<double> zoneCoolingSizingFactor);

      void resetZoneCoolingSizingFactor();

      bool setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod);

      void resetCoolingDesignAirFlowMethod();

      bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

      void resetCoolingDesignAirFlowRate();

      bool setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea);

      void resetCoolingMinimumAirFlowperZoneFloorArea();

      bool setCoolingMinimumAirFlow(double coolingMinimumAirFlow);

      void resetCoolingMinimumAirFlow();

      bool setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction);

      void resetCoolingMinimumAirFlowFraction();

      bool setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod);

      void resetHeatingDesignAirFlowMethod();

      bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

      void resetHeatingDesignAirFlowRate();

      bool setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea);

      void resetHeatingMaximumAirFlowperZoneFloorArea();

      bool setHeatingMaximumAirFlow(double heatingMaximumAirFlow);

      void resetHeatingMaximumAirFlow();

      bool setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction);

      void resetHeatingMaximumAirFlowFraction();

      bool setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem);

      bool setDedicatedOutdoorAirSystemControlStrategy(const std::string& dedicatedOutdoorAirSystemControlStrategy);

      bool setDedicatedOutdoorAirLowSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign);

      void autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();

      bool setDedicatedOutdoorAirHighSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign);

      void autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();

      bool setZoneLoadSizingMethod(const std::string& zoneLoadSizingMethod);

      bool setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(const std::string& zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);

      bool setZoneDehumidificationDesignSupplyAirHumidityRatio(double zoneDehumidificationDesignSupplyAirHumidityRatio);
      void resetZoneDehumidificationDesignSupplyAirHumidityRatio();

      bool setZoneCoolingDesignSupplyAirHumidityRatioDifference(double zoneCoolingDesignSupplyAirHumidityRatioDifference);

      bool setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(const std::string& zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);

      bool setZoneHumidificationDesignSupplyAirHumidityRatio(double zoneHumidificationDesignSupplyAirHumidityRatio);
      void resetZoneHumidificationDesignSupplyAirHumidityRatio();

      bool setZoneHumidificationDesignSupplyAirHumidityRatioDifference(double zoneHumidificationDesignSupplyAirHumidityRatioDifference);

      bool setZoneHumidistatDehumidificationSetPointSchedule(Schedule& schedule);
      void resetZoneHumidistatDehumidificationSetPointSchedule();

      bool setZoneHumidistatHumidificationSetPointSchedule(Schedule& schedule);
      void resetZoneHumidistatHumidificationSetPointSchedule();

      // Fields from DesignSpecification:ZoneAirDistribution

      bool setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode);

      void resetDesignZoneAirDistributionEffectivenessinCoolingMode();

      bool setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode);

      void resetDesignZoneAirDistributionEffectivenessinHeatingMode();

      bool setDesignZoneSecondaryRecirculationFraction(double designZoneSecondaryRecirculationFraction);

      void resetDesignZoneSecondaryRecirculationFraction();

      bool setDesignMinimumZoneVentilationEfficiency(double designMinimumZoneVentilationEfficiency);

      void resetDesignMinimumZoneVentilationEfficiency();

      bool setSizingOption(const std::string& sizingOption);

      //@}
      /** @name Other */
      //@{

      void autosize();

      void applySizingValues();

      boost::optional<double> autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

      boost::optional<double> autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SizingZone");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      boost::optional<ThermalZone> optionalThermalZone() const;

      std::vector<std::string> coolingDesignAirFlowMethodValues() const;
      std::vector<std::string> heatingDesignAirFlowMethodValues() const;
      boost::optional<ModelObject> thermalZoneAsModelObject() const;

      bool setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGZONE_IMPL_HPP
