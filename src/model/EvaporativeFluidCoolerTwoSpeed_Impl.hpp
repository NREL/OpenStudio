/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  // class WaterStorageTank;

  namespace detail {

    /** EvaporativeFluidCoolerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeFluidCoolerTwoSpeed.*/
    class MODEL_API EvaporativeFluidCoolerTwoSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EvaporativeFluidCoolerTwoSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EvaporativeFluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EvaporativeFluidCoolerTwoSpeed_Impl(const EvaporativeFluidCoolerTwoSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EvaporativeFluidCoolerTwoSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> highFanSpeedAirFlowRate() const;

      bool isHighFanSpeedAirFlowRateAutosized() const;

      boost::optional<double> highFanSpeedFanPower() const;

      bool isHighFanSpeedFanPowerAutosized() const;

      boost::optional<double> lowFanSpeedAirFlowRate() const;

      bool isLowFanSpeedAirFlowRateAutosized() const;

      double lowFanSpeedAirFlowRateSizingFactor() const;

      boost::optional<double> lowFanSpeedFanPower() const;

      bool isLowFanSpeedFanPowerAutosized() const;

      double lowFanSpeedFanPowerSizingFactor() const;

      double designSprayWaterFlowRate() const;

      std::string performanceInputMethod() const;

      boost::optional<std::string> outdoorAirInletNodeName() const;

      double heatRejectionCapacityandNominalCapacitySizingRatio() const;

      boost::optional<double> highSpeedStandardDesignCapacity() const;

      boost::optional<double> lowSpeedStandardDesignCapacity() const;

      bool isLowSpeedStandardDesignCapacityAutosized() const;

      double lowSpeedStandardCapacitySizingFactor() const;

      boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

      bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

      boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

      bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

      double lowFanSpeedUFactorTimesAreaSizingFactor() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> highSpeedUserSpecifiedDesignCapacity() const;

      boost::optional<double> lowSpeedUserSpecifiedDesignCapacity() const;

      bool isLowSpeedUserSpecifiedDesignCapacityAutosized() const;

      double lowSpeedUserSpecifiedDesignCapacitySizingFactor() const;

      boost::optional<double> designEnteringWaterTemperature() const;

      boost::optional<double> designEnteringAirTemperature() const;

      boost::optional<double> designEnteringAirWetbulbTemperature() const;

      double highSpeedSizingFactor() const;

      std::string evaporationLossMode() const;

      boost::optional<double> evaporationLossFactor() const;

      double driftLossPercent() const;

      std::string blowdownCalculationMode() const;

      double blowdownConcentrationRatio() const;

      boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

      // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

      boost::optional<double> autosizedHighFanSpeedAirFlowRate() const;

      boost::optional<double> autosizedHighFanSpeedFanPower() const;

      boost::optional<double> autosizedLowFanSpeedAirFlowRate() const;

      boost::optional<double> autosizedLowFanSpeedFanPower() const;

      boost::optional<double> autosizedLowSpeedStandardDesignCapacity() const;

      boost::optional<double> autosizedHighFanSpeedUfactorTimesAreaValue() const;

      boost::optional<double> autosizedLowFanSpeedUfactorTimesAreaValue() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedLowSpeedUserSpecifiedDesignCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

      void autosizeHighFanSpeedAirFlowRate();

      bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

      void autosizeHighFanSpeedFanPower();

      bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

      void autosizeLowFanSpeedAirFlowRate();

      bool setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

      bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

      void autosizeLowFanSpeedFanPower();

      bool setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

      bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      bool setOutdoorAirInletNodeName(boost::optional<std::string> outdoorAirInletNodeName);

      void resetOutdoorAirInletNodeName();

      bool setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

      bool setHighSpeedStandardDesignCapacity(boost::optional<double> highSpeedStandardDesignCapacity);

      void resetHighSpeedStandardDesignCapacity();

      bool setLowSpeedStandardDesignCapacity(boost::optional<double> lowSpeedStandardDesignCapacity);

      void resetLowSpeedStandardDesignCapacity();

      void autosizeLowSpeedStandardDesignCapacity();

      bool setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor);

      bool setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue);

      void resetHighFanSpeedUfactorTimesAreaValue();

      void autosizeHighFanSpeedUfactorTimesAreaValue();

      bool setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue);

      void resetLowFanSpeedUfactorTimesAreaValue();

      void autosizeLowFanSpeedUfactorTimesAreaValue();

      bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void resetDesignWaterFlowRate();

      void autosizeDesignWaterFlowRate();

      bool setHighSpeedUserSpecifiedDesignCapacity(boost::optional<double> highSpeedUserSpecifiedDesignCapacity);

      void resetHighSpeedUserSpecifiedDesignCapacity();

      bool setLowSpeedUserSpecifiedDesignCapacity(boost::optional<double> lowSpeedUserSpecifiedDesignCapacity);

      void resetLowSpeedUserSpecifiedDesignCapacity();

      void autosizeLowSpeedUserSpecifiedDesignCapacity();

      bool setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor);

      bool setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature);

      void resetDesignEnteringWaterTemperature();

      bool setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature);

      void resetDesignEnteringAirTemperature();

      bool setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature);

      void resetDesignEnteringAirWetbulbTemperature();

      bool setHighSpeedSizingFactor(double highSpeedSizingFactor);

      bool setEvaporationLossMode(const std::string& evaporationLossMode);

      bool setEvaporationLossFactor(boost::optional<double> evaporationLossFactor);

      void resetEvaporationLossFactor();

      bool setDriftLossPercent(double driftLossPercent);

      bool setBlowdownCalculationMode(const std::string& blowdownCalculationMode);

      bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

      bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

      void resetBlowdownMakeupWaterUsageSchedule();

      // bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetSupplyWaterStorageTank();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerTwoSpeed");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_IMPL_HPP
