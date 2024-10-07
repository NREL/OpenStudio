/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  // class WaterStorageTank;
  class Schedule;
  class CoilCoolingDXVariableSpeedSpeedData;
  class ModelObjectList;

  namespace detail {

    /** CoilCoolingDXVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXVariableSpeed.*/
    class MODEL_API CoilCoolingDXVariableSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXVariableSpeed_Impl(const CoilCoolingDXVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXVariableSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<IdfObject> remove() override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      int nominalSpeedLevel() const;

      boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

      bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

      boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

      bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

      double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

      double maximumCyclingRate() const;

      double latentCapacityTimeConstant() const;

      double fanDelayTime() const;

      Curve energyPartLoadFractionCurve() const;

      // boost::optional<std::string> condenserAirInletNodeName() const;

      std::string condenserType() const;

      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

      bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

      // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalSpeedLevel(int nominalSpeedLevel);

      bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(
        boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

      void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

      bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

      void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

      bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

      bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
        double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

      bool setMaximumCyclingRate(double maximumCyclingRate);

      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

      bool setFanDelayTime(double fanDelayTime);

      bool setEnergyPartLoadFractionCurve(const Curve& curve);

      // bool setCondenserAirInletNodeName(boost::optional<std::string> condenserAirInletNodeName);

      // void resetCondenserAirInletNodeName();

      bool setCondenserType(const std::string& condenserType);

      bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption);

      void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      // bool setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetSupplyWaterStorageTank();

      // bool setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetCondensateCollectionWaterStorageTank();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      //@}
      /** @name Other */
      //@{

      bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetSpeedDataList();

      boost::optional<ModelObjectList> speedDataList() const;

      std::vector<CoilCoolingDXVariableSpeedSpeedData> speeds() const;

      bool addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

      void removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

      void removeAllSpeeds();

      // Autosize methods
      boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeed");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLESPEED_IMPL_HPP
