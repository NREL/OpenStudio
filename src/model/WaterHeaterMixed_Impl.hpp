/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERHEATERMIXED_IMPL_HPP
#define MODEL_WATERHEATERMIXED_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

class FuelType;

namespace model {

  class Schedule;
  class CurveCubic;
  class ThermalZone;
  class WaterHeaterSizing;
  class Node;
  class PlantLoop;

  namespace detail {

    class MODEL_API WaterHeaterMixed_Impl : public WaterToWaterComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      WaterHeaterMixed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterHeaterMixed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterHeaterMixed_Impl(const WaterHeaterMixed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterHeaterMixed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // Correspond to the Use Side Nodes
      virtual unsigned supplyInletPort() const override;
      virtual unsigned supplyOutletPort() const override;

      // Correspond to the Source Side Nodes
      virtual unsigned demandInletPort() const override;
      virtual unsigned demandOutletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ModelObject clone(Model model) const override;

      // We have to override to avoid returning the source side plant loop if it is also on the supply side of this one
      virtual boost::optional<PlantLoop> plantLoop() const override;

      // Override to return the PlantLoop connected to the Source Side Inlet/Outlet Nodes whether that is on the demand or supply side
      virtual boost::optional<PlantLoop> secondaryPlantLoop() const override;

      virtual bool removeFromSecondaryPlantLoop() override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> tankVolume() const;

      bool isTankVolumeDefaulted() const;

      bool isTankVolumeAutosized() const;

      boost::optional<Schedule> setpointTemperatureSchedule() const;

      double deadbandTemperatureDifference() const;

      bool isDeadbandTemperatureDifferenceDefaulted() const;

      boost::optional<double> maximumTemperatureLimit() const;

      std::string heaterControlType() const;

      bool isHeaterControlTypeDefaulted() const;

      boost::optional<double> heaterMaximumCapacity() const;

      bool isHeaterMaximumCapacityAutosized() const;

      boost::optional<double> heaterMinimumCapacity() const;

      double heaterIgnitionMinimumFlowRate() const;

      bool isHeaterIgnitionMinimumFlowRateDefaulted() const;

      double heaterIgnitionDelay() const;

      bool isHeaterIgnitionDelayDefaulted() const;

      std::string heaterFuelType() const;

      boost::optional<double> heaterThermalEfficiency() const;

      boost::optional<CurveCubic> partLoadFactorCurve() const;

      double offCycleParasiticFuelConsumptionRate() const;

      bool isOffCycleParasiticFuelConsumptionRateDefaulted() const;

      boost::optional<std::string> offCycleParasiticFuelType() const;

      double offCycleParasiticHeatFractiontoTank() const;

      bool isOffCycleParasiticHeatFractiontoTankDefaulted() const;

      double onCycleParasiticFuelConsumptionRate() const;

      bool isOnCycleParasiticFuelConsumptionRateDefaulted() const;

      boost::optional<std::string> onCycleParasiticFuelType() const;

      double onCycleParasiticHeatFractiontoTank() const;

      bool isOnCycleParasiticHeatFractiontoTankDefaulted() const;

      std::string ambientTemperatureIndicator() const;

      boost::optional<Schedule> ambientTemperatureSchedule() const;

      boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

      boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

      boost::optional<double> offCycleLossCoefficienttoAmbientTemperature() const;

      double offCycleLossFractiontoThermalZone() const;

      bool isOffCycleLossFractiontoThermalZoneDefaulted() const;

      boost::optional<double> onCycleLossCoefficienttoAmbientTemperature() const;

      double onCycleLossFractiontoThermalZone() const;

      bool isOnCycleLossFractiontoThermalZoneDefaulted() const;

      boost::optional<double> peakUseFlowRate() const;

      boost::optional<Schedule> useFlowRateFractionSchedule() const;

      boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

      double useSideEffectiveness() const;

      bool isUseSideEffectivenessDefaulted() const;

      double sourceSideEffectiveness() const;

      bool isSourceSideEffectivenessDefaulted() const;

      boost::optional<double> useSideDesignFlowRate() const;

      bool isUseSideDesignFlowRateDefaulted() const;

      bool isUseSideDesignFlowRateAutosized() const;

      boost::optional<double> sourceSideDesignFlowRate() const;

      bool isSourceSideDesignFlowRateDefaulted() const;

      bool isSourceSideDesignFlowRateAutosized() const;

      double indirectWaterHeatingRecoveryTime() const;

      bool isIndirectWaterHeatingRecoveryTimeDefaulted() const;

      boost::optional<double> autosizedTankVolume() const;

      boost::optional<double> autosizedHeaterMaximumCapacity() const;

      boost::optional<double> autosizedUseSideDesignFlowRate() const;

      boost::optional<double> autosizedSourceSideDesignFlowRate() const;

      std::string sourceSideFlowControlMode() const;

      boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setTankVolume(boost::optional<double> tankVolume);

      void resetTankVolume();

      void autosizeTankVolume();

      bool setSetpointTemperatureSchedule(Schedule& setpointTemperatureSchedule);

      void resetSetpointTemperatureSchedule();

      bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

      void resetDeadbandTemperatureDifference();

      bool setMaximumTemperatureLimit(boost::optional<double> maximumTemperatureLimit);

      void resetMaximumTemperatureLimit();

      bool setHeaterControlType(const std::string& heaterControlType);

      void resetHeaterControlType();

      bool setHeaterMaximumCapacity(boost::optional<double> heaterMaximumCapacity);

      void resetHeaterMaximumCapacity();

      void autosizeHeaterMaximumCapacity();

      bool setHeaterMinimumCapacity(boost::optional<double> heaterMinimumCapacity);

      void resetHeaterMinimumCapacity();

      bool setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate);

      void resetHeaterIgnitionMinimumFlowRate();

      bool setHeaterIgnitionDelay(double heaterIgnitionDelay);

      void resetHeaterIgnitionDelay();

      bool setHeaterFuelType(const FuelType& heaterFuelType);

      bool setHeaterThermalEfficiency(boost::optional<double> heaterThermalEfficiency);

      void resetHeaterThermalEfficiency();

      bool setPartLoadFactorCurve(const boost::optional<CurveCubic>& partLoadFactorCurve);

      void resetPartLoadFactorCurve();

      bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

      void resetOffCycleParasiticFuelConsumptionRate();

      bool setOffCycleParasiticFuelType(const FuelType& offCycleParasiticFuelType);

      void resetOffCycleParasiticFuelType();

      bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

      void resetOffCycleParasiticHeatFractiontoTank();

      bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

      void resetOnCycleParasiticFuelConsumptionRate();

      bool setOnCycleParasiticFuelType(const FuelType& onCycleParasiticFuelType);

      void resetOnCycleParasiticFuelType();

      bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

      void resetOnCycleParasiticHeatFractiontoTank();

      bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

      bool setAmbientTemperatureSchedule(Schedule& ambientTemperatureSchedule);

      void resetAmbientTemperatureSchedule();

      bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& ambientTemperatureThermalZone);

      void resetAmbientTemperatureThermalZone();

      bool setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

      void resetAmbientTemperatureOutdoorAirNodeName();

      bool setOffCycleLossCoefficienttoAmbientTemperature(boost::optional<double> offCycleLossCoefficienttoAmbientTemperature);

      void resetOffCycleLossCoefficienttoAmbientTemperature();

      bool setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone);

      void resetOffCycleLossFractiontoThermalZone();

      bool setOnCycleLossCoefficienttoAmbientTemperature(boost::optional<double> onCycleLossCoefficienttoAmbientTemperature);

      void resetOnCycleLossCoefficienttoAmbientTemperature();

      bool setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone);

      void resetOnCycleLossFractiontoThermalZone();

      bool setPeakUseFlowRate(boost::optional<double> peakUseFlowRate);

      void resetPeakUseFlowRate();

      bool setUseFlowRateFractionSchedule(Schedule& useFlowRateFractionSchedule);

      void resetUseFlowRateFractionSchedule();

      bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

      void resetColdWaterSupplyTemperatureSchedule();

      bool setUseSideEffectiveness(double useSideEffectiveness);

      void resetUseSideEffectiveness();

      bool setSourceSideEffectiveness(double sourceSideEffectiveness);

      void resetSourceSideEffectiveness();

      bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

      void resetUseSideDesignFlowRate();

      void autosizeUseSideDesignFlowRate();

      bool setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate);

      void resetSourceSideDesignFlowRate();

      void autosizeSourceSideDesignFlowRate();

      bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

      void resetIndirectWaterHeatingRecoveryTime();

      bool setSourceSideFlowControlMode(const std::string& sourceSideFlowControlMode);

      bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& indirectAlternateSetpointTemperatureSchedule);
      void resetIndirectAlternateSetpointTemperatureSchedule();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      WaterHeaterSizing waterHeaterSizing() const;

      bool addToSourceSideNode(Node& node);

      // Name aliases
      boost::optional<ModelObject> useSideInletModelObject() const;
      boost::optional<ModelObject> useSideOutletModelObject() const;
      boost::optional<ModelObject> sourceSideInletModelObject() const;
      boost::optional<ModelObject> sourceSideOutletModelObject() const;
      boost::optional<PlantLoop> sourceSidePlantLoop() const;
      boost::optional<PlantLoop> useSidePlantLoop() const;
      bool removeFromSourceSidePlantLoop();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WaterHeaterMixed");

      std::vector<std::string> heaterControlTypeValues() const;
      std::vector<std::string> heaterFuelTypeValues() const;
      std::vector<std::string> offCycleParasiticFuelTypeValues() const;
      std::vector<std::string> onCycleParasiticFuelTypeValues() const;
      std::vector<std::string> ambientTemperatureIndicatorValues() const;
      std::vector<std::string> sourceSideFlowControlModeValues() const;

      boost::optional<ModelObject> setpointTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> partLoadFactorCurveAsModelObject() const;
      boost::optional<ModelObject> ambientTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> ambientTemperatureThermalZoneAsModelObject() const;
      boost::optional<ModelObject> useFlowRateFractionScheduleAsModelObject() const;
      boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

      boost::optional<ModelObject> indirectAlternateSetpointTemperatureScheduleAsModelObject() const;

      bool setSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPartLoadFactorCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAmbientTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAmbientTemperatureThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setUseFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      bool setIndirectAlternateSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERMIXED_IMPL_HPP
