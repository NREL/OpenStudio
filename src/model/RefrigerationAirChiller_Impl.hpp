/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP
#define MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;
  class RefrigerationSecondarySystem;
  class RefrigerationCompressorRack;
  class Schedule;
  // class CurveLinear;

  namespace detail {

    /** RefrigerationAirChiller_Impl is a ZoneHVACComponent_Impl that is the implementation class for RefrigerationAirChiller.*/
    class MODEL_API RefrigerationAirChiller_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationAirChiller_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationAirChiller_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationAirChiller_Impl(const RefrigerationAirChiller_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationAirChiller_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // virtual std::vector<ModelObject> children() const;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToThermalZone(ThermalZone& thermalZone) override;

      virtual void removeFromThermalZone() override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      std::string capacityRatingType() const;

      boost::optional<double> ratedUnitLoadFactor() const;

      boost::optional<double> ratedCapacity() const;

      double ratedRelativeHumidity() const;

      bool isRatedRelativeHumidityDefaulted() const;

      double ratedCoolingSourceTemperature() const;

      double ratedTemperatureDifferenceDT1() const;

      boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const;

      double coilMaterialCorrectionFactor() const;

      bool isCoilMaterialCorrectionFactorDefaulted() const;

      double refrigerantCorrectionFactor() const;

      bool isRefrigerantCorrectionFactorDefaulted() const;

      // std::string capacityCorrectionCurveType() const;

      // bool isCapacityCorrectionCurveTypeDefaulted() const;

      // boost::optional<CurveLinear> capacityCorrectionCurve() const;

      double sHR60CorrectionFactor() const;

      bool isSHR60CorrectionFactorDefaulted() const;

      double ratedTotalHeatingPower() const;

      boost::optional<Schedule> heatingPowerSchedule() const;

      std::string fanSpeedControlType() const;

      bool isFanSpeedControlTypeDefaulted() const;

      double ratedFanPower() const;

      bool isRatedFanPowerDefaulted() const;

      double ratedAirFlow() const;

      double minimumFanAirFlowRatio() const;

      bool isMinimumFanAirFlowRatioDefaulted() const;

      std::string defrostType() const;

      bool isDefrostTypeDefaulted() const;

      std::string defrostControlType() const;

      bool isDefrostControlTypeDefaulted() const;

      Schedule defrostSchedule() const;

      boost::optional<Schedule> defrostDripDownSchedule() const;

      boost::optional<double> defrostPower() const;

      boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

      std::string verticalLocation() const;

      bool isVerticalLocationDefaulted() const;

      double averageRefrigerantChargeInventory() const;

      bool isAverageRefrigerantChargeInventoryDefaulted() const;

      boost::optional<RefrigerationSystem> system() const;

      boost::optional<RefrigerationSecondarySystem> secondarySystem() const;

      boost::optional<RefrigerationCompressorRack> compressorRack() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCapacityRatingType(const std::string& capacityRatingType);

      bool setRatedUnitLoadFactor(boost::optional<double> ratedUnitLoadFactor);

      void resetRatedUnitLoadFactor();

      bool setRatedCapacity(boost::optional<double> ratedCapacity);

      void resetRatedCapacity();

      bool setRatedRelativeHumidity(double ratedRelativeHumidity);

      void resetRatedRelativeHumidity();

      bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

      bool setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1);

      bool setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(
        boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature);

      void resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();

      bool setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor);

      void resetCoilMaterialCorrectionFactor();

      bool setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor);

      void resetRefrigerantCorrectionFactor();

      // bool setCapacityCorrectionCurveType(const std::string& capacityCorrectionCurveType);

      // void resetCapacityCorrectionCurveType();

      // bool setCapacityCorrectionCurve(const boost::optional<CurveLinear>& curveLinear);

      // void resetCapacityCorrectionCurve();

      bool setSHR60CorrectionFactor(double sHR60CorrectionFactor);

      void resetSHR60CorrectionFactor();

      bool setRatedTotalHeatingPower(double ratedTotalHeatingPower);

      bool setHeatingPowerSchedule(Schedule& schedule);

      void resetHeatingPowerSchedule();

      bool setFanSpeedControlType(const std::string& fanSpeedControlType);

      void resetFanSpeedControlType();

      bool setRatedFanPower(double ratedFanPower);

      void resetRatedFanPower();

      bool setRatedAirFlow(double ratedAirFlow);

      bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

      void resetMinimumFanAirFlowRatio();

      bool setDefrostType(const std::string& defrostType);

      void resetDefrostType();

      bool setDefrostControlType(const std::string& defrostControlType);

      void resetDefrostControlType();

      bool setDefrostSchedule(Schedule& schedule);

      bool setDefrostDripDownSchedule(Schedule& schedule);

      void resetDefrostDripDownSchedule();

      bool setDefrostPower(boost::optional<double> defrostPower);

      void resetDefrostPower();

      bool setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce);

      void resetTemperatureTerminationDefrostFractiontoIce();

      bool setVerticalLocation(const std::string& verticalLocation);

      void resetVerticalLocation();

      bool setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

      void resetAverageRefrigerantChargeInventory();

      bool addToSystem(RefrigerationSystem& system);

      void removeFromSystem();

      bool addToSecondarySystem(RefrigerationSecondarySystem& secondarySystem);

      void removeFromSecondarySystem();

      bool addToCompressorRack(RefrigerationCompressorRack& compressorRack);

      void removeFromCompressorRack();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationAirChiller");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalDefrostSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP
