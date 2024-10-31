/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;
  class ModelObjectList;

  namespace detail {

    /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.*/
    class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle);

      CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                                 Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

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

      boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

      bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

      double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

      double maximumCyclingRate() const;
      double latentCapacityTimeConstant() const;
      double fanDelayTime() const;

      bool useHotGasReheat() const;

      Curve energyPartLoadFractionCurve() const;

      boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedRatedWaterFlowRateAtSelectedNominalSpeedLevel() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setNominalSpeedLevel(int nominalSpeedLevel);

      bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(
        boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

      void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

      bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

      void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

      bool setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel);

      void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

      bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

      bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
        double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

      bool setMaximumCyclingRate(double maximumCyclingRate);
      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);
      bool setFanDelayTime(double fanDelayTime);

      bool setUseHotGasReheat(bool useHotGasReheat);

      bool setEnergyPartLoadFractionCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetSpeedDataList();

      boost::optional<ModelObjectList> speedDataList() const;

      std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

      bool addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

      void removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

      void removeAllSpeeds();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_IMPL_HPP
