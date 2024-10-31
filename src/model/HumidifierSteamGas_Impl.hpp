/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HUMIDIFIERSTEAMGAS_IMPL_HPP
#define MODEL_HUMIDIFIERSTEAMGAS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** HumidifierSteamGas_Impl is a StraightComponent_Impl that is the implementation class for HumidifierSteamGas.*/
    class MODEL_API HumidifierSteamGas_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HumidifierSteamGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HumidifierSteamGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HumidifierSteamGas_Impl(const HumidifierSteamGas_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HumidifierSteamGas_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> ratedCapacity() const;

      bool isRatedCapacityAutosized() const;

      boost::optional<double> ratedGasUseRate() const;

      bool isRatedGasUseRateAutosized() const;

      double thermalEfficiency() const;

      bool isThermalEfficiencyDefaulted() const;

      boost::optional<Curve> thermalEfficiencyModifierCurve() const;

      boost::optional<double> ratedFanPower() const;

      double auxiliaryElectricPower() const;

      bool isAuxiliaryElectricPowerDefaulted() const;

      // boost::optional<WaterStorageTank> waterStorageTank() const;

      std::string inletWaterTemperatureOption() const;

      bool isInletWaterTemperatureOptionDefaulted() const;

      boost::optional<double> autosizedRatedCapacity() const;

      boost::optional<double> autosizedRatedGasUseRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setRatedCapacity(boost::optional<double> ratedCapacity);

      void autosizeRatedCapacity();

      bool setRatedGasUseRate(boost::optional<double> ratedGasUseRate);

      void resetRatedGasUseRate();

      void autosizeRatedGasUseRate();

      bool setThermalEfficiency(boost::optional<double> thermalEfficiency);

      void resetThermalEfficiency();

      bool setThermalEfficiencyModifierCurve(const boost::optional<Curve>& curve);

      void resetThermalEfficiencyModifierCurve();

      bool setRatedFanPower(boost::optional<double> ratedFanPower);

      void resetRatedFanPower();

      bool setAuxiliaryElectricPower(boost::optional<double> auxiliaryElectricPower);

      void resetAuxiliaryElectricPower();

      // bool setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetWaterStorageTank();

      bool setInletWaterTemperatureOption(boost::optional<std::string> inletWaterTemperatureOption);

      void resetInletWaterTemperatureOption();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HumidifierSteamGas");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HUMIDIFIERSTEAMGAS_IMPL_HPP
