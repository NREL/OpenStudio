/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Information;
  class Schedule;

  namespace detail {

    // derive CoilCoolingDXTwoSpeed_Impl from ModelObject_Impl to override virtual methods
    class MODEL_API CoilCoolingDXTwoSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      CoilCoolingDXTwoSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CoilCoolingDXTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      CoilCoolingDXTwoSpeed_Impl(const CoilCoolingDXTwoSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXTwoSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> ratedHighSpeedTotalCoolingCapacity() const;

      boost::optional<double> ratedHighSpeedSensibleHeatRatio() const;

      double ratedHighSpeedCOP() const;

      boost::optional<double> ratedHighSpeedAirFlowRate() const;

      double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017() const;

      double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023() const;

      Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

      Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

      Curve energyInputRatioFunctionOfTemperatureCurve() const;

      Curve energyInputRatioFunctionOfFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      boost::optional<double> ratedLowSpeedTotalCoolingCapacity() const;

      boost::optional<double> ratedLowSpeedSensibleHeatRatio() const;

      double ratedLowSpeedCOP() const;

      boost::optional<double> ratedLowSpeedAirFlowRate() const;

      double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017() const;

      double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023() const;

      Curve lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve() const;

      Curve lowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

      boost::optional<std::string> condenserAirInletNodeName() const;

      std::string condenserType() const;

      double highSpeedEvaporativeCondenserEffectiveness() const;

      boost::optional<double> highSpeedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> highSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

      double lowSpeedEvaporativeCondenserEffectiveness() const;

      boost::optional<double> lowSpeedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> lowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      double unitInternalStaticAirPressure() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedHighSpeedTotalCoolingCapacity(boost::optional<double> value);

      bool setRatedHighSpeedTotalCoolingCapacity(double value);

      bool setRatedHighSpeedSensibleHeatRatio(boost::optional<double> value);

      bool setRatedHighSpeedSensibleHeatRatio(double value);

      bool setRatedHighSpeedCOP(double value);

      bool setRatedHighSpeedAirFlowRate(boost::optional<double> value);

      bool setRatedHighSpeedAirFlowRate(double value);

      bool setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017);

      bool setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023);

      bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

      bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setRatedLowSpeedTotalCoolingCapacity(boost::optional<double> value);

      bool setRatedLowSpeedTotalCoolingCapacity(double value);

      bool setRatedLowSpeedSensibleHeatRatio(boost::optional<double> value);

      bool setRatedLowSpeedSensibleHeatRatio(double value);

      bool setRatedLowSpeedCOP(double value);

      bool setRatedLowSpeedAirFlowRate(boost::optional<double> value);

      bool setRatedLowSpeedAirFlowRate(double value);

      bool setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017);

      bool setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023);

      bool setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

      bool setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

      bool setCondenserAirInletNodeName(const std::string& value);

      bool setCondenserType(const std::string& value);

      bool setHighSpeedEvaporativeCondenserEffectiveness(double value);

      bool setHighSpeedEvaporativeCondenserAirFlowRate(boost::optional<double> value);

      bool setHighSpeedEvaporativeCondenserAirFlowRate(double value);

      bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

      bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(double value);

      bool setLowSpeedEvaporativeCondenserEffectiveness(double value);

      bool setLowSpeedEvaporativeCondenserAirFlowRate(boost::optional<double> value);

      bool setLowSpeedEvaporativeCondenserAirFlowRate(double value);

      bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

      bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(double value);

      //TODO
      // A14, \field Supply Water Storage Tank Name
      // supplyWaterStorageTankName
      // setSupplyWaterStorageTankName

      //TODO
      // A15, \field Condensate Collection Water Storage Tank Name
      // getCondensateCollectionWaterStorageTankName
      // setCondensateCollectionWaterStorageTankName

      bool setBasinHeaterCapacity(double value);

      bool setBasinHeaterSetpointTemperature(double value);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      bool setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure);

      //@}
      /** @name Other */
      //@{

      // Autosize methods

      boost::optional<double> autosizedRatedHighSpeedTotalCoolingCapacity() const;

      boost::optional<double> autosizedRatedHighSpeedSensibleHeatRatio() const;

      boost::optional<double> autosizedRatedHighSpeedAirFlowRate() const;

      boost::optional<double> autosizedRatedLowSpeedTotalCoolingCapacity() const;

      boost::optional<double> autosizedRatedLowSpeedSensibleHeatRatio() const;

      boost::optional<double> autosizedRatedLowSpeedAirFlowRate() const;

      boost::optional<double> autosizedHighSpeedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

      boost::optional<double> autosizedLowSpeedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoSpeed");

      // optional getters for children so can remove() if constructor fails
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionOfFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
      boost::optional<Curve> optionalLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalLowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXTWOSPEED_IMPL_HPP
