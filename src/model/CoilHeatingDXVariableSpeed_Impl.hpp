/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilHeatingDXVariableSpeedSpeedData;
  class Curve;
  class ModelObjectList;

  namespace detail {

    /** CoilHeatingDXVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXVariableSpeed.*/
    class MODEL_API CoilHeatingDXVariableSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXVariableSpeed_Impl(const CoilHeatingDXVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXVariableSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      int nominalSpeedLevel() const;

      boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

      bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

      boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

      bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

      Curve energyPartLoadFractionCurve() const;

      boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

      double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      std::string defrostStrategy() const;

      std::string defrostControl() const;

      double defrostTimePeriodFraction() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      boost::optional<double> autosizedRatedHeatingCapacityAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setNominalSpeedLevel(int nominalSpeedLevel);

      bool setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel);

      void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

      bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel);

      void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

      bool setEnergyPartLoadFractionCurve(const Curve& curve);

      bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

      void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      bool setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor);

      void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

      bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setDefrostStrategy(const std::string& defrostStrategy);

      bool setDefrostControl(const std::string& defrostControl);

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

      void autosizeResistiveDefrostHeaterCapacity();

      //@}
      /** @name Other */
      //@{

      bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetSpeedDataList();

      boost::optional<ModelObjectList> speedDataList() const;

      std::vector<CoilHeatingDXVariableSpeedSpeedData> speeds() const;

      bool addSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

      void removeSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

      void removeAllSpeeds();

      std::vector<IdfObject> remove() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeed");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalEnergyPartLoadFractionCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLESPEED_IMPL_HPP
