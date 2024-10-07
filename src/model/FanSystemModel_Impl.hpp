/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANSYSTEMMODEL_IMPL_HPP
#define MODEL_FANSYSTEMMODEL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class FanSystemModelSpeed;
  class AirflowNetworkFan;

  namespace detail {

    /** FanSystemModel_Impl is a StraightComponent_Impl that is the implementation class for FanSystemModel.*/
    class MODEL_API FanSystemModel_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanSystemModel_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanSystemModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanSystemModel_Impl(const FanSystemModel_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanSystemModel_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

      boost::optional<double> designMaximumAirFlowRate() const;

      bool isDesignMaximumAirFlowRateAutosized() const;

      boost::optional<double> autosizedDesignMaximumAirFlowRate();

      std::string speedControlMethod() const;

      double electricPowerMinimumFlowRateFraction() const;

      double designPressureRise() const;

      double motorEfficiency() const;

      double motorInAirStreamFraction() const;

      boost::optional<double> designElectricPowerConsumption() const;

      bool isDesignElectricPowerConsumptionAutosized() const;

      boost::optional<double> autosizedDesignElectricPowerConsumption();

      std::string designPowerSizingMethod() const;

      double electricPowerPerUnitFlowRate() const;

      double electricPowerPerUnitFlowRatePerUnitPressure() const;

      double fanTotalEfficiency() const;

      boost::optional<Curve> electricPowerFunctionofFlowFractionCurve() const;

      boost::optional<double> nightVentilationModePressureRise() const;

      boost::optional<double> nightVentilationModeFlowFraction() const;

      boost::optional<ThermalZone> motorLossZone() const;

      double motorLossRadiativeFraction() const;

      std::string endUseSubcategory() const;

      unsigned numberofSpeeds() const;
      std::vector<FanSystemModelSpeed> speeds() const;

      /** Find a given speed by the flowFraction (key) */
      boost::optional<unsigned> speedIndex(const FanSystemModelSpeed& speed) const;

      boost::optional<FanSystemModelSpeed> getSpeed(unsigned speedIndex) const;

      //@}
      /** @name Setters */
      //@{

      /** If a speed group is already present (= the flowFraction already exists) (cf `speedIndex()`), it will Warn and override the electricPowerFraction value */
      bool addSpeed(const FanSystemModelSpeed& speed);

      // Overloads, it creates a FanSystemModelSpeed wrapper, then call `addSpeed(const FanSystemModelSpeed&)`
      bool addSpeed(double flowFraction);
      bool addSpeed(double flowFraction, double electricPowerFraction);

      bool removeSpeed(unsigned speedIndex);

      void removeAllSpeeds();

      // Directly set the speeds from a vector, will delete any existing speeds
      bool setSpeeds(const std::vector<FanSystemModelSpeed>& speeds);

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setDesignMaximumAirFlowRate(double designMaximumAirFlowRate);

      void autosizeDesignMaximumAirFlowRate();

      bool setSpeedControlMethod(const std::string& speedControlMethod);

      bool setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction);

      bool setDesignPressureRise(double designPressureRise);

      bool setMotorEfficiency(double motorEfficiency);

      bool setMotorInAirStreamFraction(double motorInAirStreamFraction);

      bool setDesignElectricPowerConsumption(double designElectricPowerConsumption);

      void autosizeDesignElectricPowerConsumption();

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      bool setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate);

      bool setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure);

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      bool setElectricPowerFunctionofFlowFractionCurve(const Curve& univariateFunctions);

      void resetElectricPowerFunctionofFlowFractionCurve();

      bool setNightVentilationModePressureRise(double nightVentilationModePressureRise);

      void resetNightVentilationModePressureRise();

      bool setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction);

      void resetNightVentilationModeFlowFraction();

      bool setMotorLossZone(const ThermalZone& thermalZone);

      void resetMotorLossZone();

      bool setMotorLossRadiativeFraction(double motorLossRadiativeFraction);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      AirflowNetworkFan getAirflowNetworkFan();
      boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanSystemModel");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      bool addSpeedPrivate(double flowFraction, boost::optional<double> electricPowerFraction);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANSYSTEMMODEL_IMPL_HPP
