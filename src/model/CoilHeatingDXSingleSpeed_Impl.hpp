/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP
#define MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;
  class ZoneHVACComponent;

  namespace detail {

    /** CoilHeatingDXSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXSingleSpeed.*/
    class MODEL_API CoilHeatingDXSingleSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXSingleSpeed_Impl(const CoilHeatingDXSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXSingleSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> ratedTotalHeatingCapacity() const;

      bool isRatedTotalHeatingCapacityAutosized() const;

      double ratedCOP() const;

      boost::optional<double> ratedAirFlowRate() const;

      double ratedSupplyFanPowerPerVolumeFlowRate2017() const;

      double ratedSupplyFanPowerPerVolumeFlowRate2023() const;

      bool isRatedAirFlowRateAutosized() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

      double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const;

      double crankcaseHeaterCapacity() const;

      bool isCrankcaseHeaterCapacityDefaulted() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const;

      std::string defrostStrategy() const;

      bool isDefrostStrategyDefaulted() const;

      std::string defrostControl() const;

      bool isDefrostControlDefaulted() const;

      double defrostTimePeriodFraction() const;

      bool isDefrostTimePeriodFractionDefaulted() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityDefaulted() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

      void autosizeRatedTotalHeatingCapacity();

      bool setRatedCOP(double ratedCOP);

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setRatedSupplyFanPowerPerVolumeFlowRate2017(double ratedSupplyFanPowerPerVolumeFlowRate2017);

      bool setRatedSupplyFanPowerPerVolumeFlowRate2023(double ratedSupplyFanPowerPerVolumeFlowRate2023);

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

      bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

      void resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      void resetCrankcaseHeaterCapacity();

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      void resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();

      bool setDefrostStrategy(const std::string& defrostStrategy);

      void resetDefrostStrategy();

      bool setDefrostControl(const std::string& defrostControl);

      void resetDefrostControl();

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      void resetDefrostTimePeriodFraction();

      bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

      void resetResistiveDefrostHeaterCapacity();

      void autosizeResistiveDefrostHeaterCapacity();

      //@}
      /** @name Curves */
      //@{

      // A5 , \field Total Heating Capacity Function of Temperature Curve Name
      // \object-list BiquadraticQuadraticCubicCurves
      Curve totalHeatingCapacityFunctionofTemperatureCurve() const;
      bool setTotalHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
      // \object-list QuadraticCubicCurves
      Curve totalHeatingCapacityFunctionofFlowFractionCurve() const;
      bool setTotalHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

      // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
      // \object-list BiquadraticQuadraticCubicCurves
      Curve energyInputRatioFunctionofTemperatureCurve() const;
      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
      // \object-list QuadraticCubicCurves
      Curve energyInputRatioFunctionofFlowFractionCurve() const;
      bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

      // A9 , \field Part Load Fraction Correlation Curve Name
      // \object-list QuadraticCubicCurves
      Curve partLoadFractionCorrelationCurve() const;
      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
      // \object-list BiquadraticCurves
      // not a required curve so it needs a reset
      boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;
      bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve> curve);

      bool addToNode(Node& node) override;

      //@}

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXSingleSpeed");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP
