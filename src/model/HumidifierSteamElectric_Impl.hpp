/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** HumidifierSteamElectric_Impl is a StraightComponent_Impl that is the implementation class for HumidifierSteamElectric.*/
    class MODEL_API HumidifierSteamElectric_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HumidifierSteamElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HumidifierSteamElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HumidifierSteamElectric_Impl(const HumidifierSteamElectric_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HumidifierSteamElectric_Impl() override = default;

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

      boost::optional<double> ratedPower() const;

      bool isRatedPowerAutosized() const;

      boost::optional<double> ratedFanPower() const;

      boost::optional<double> standbyPower() const;

      // boost::optional<WaterStorageTank> waterStorageTank() const;

      boost::optional<double> autosizedRatedCapacity() const;

      boost::optional<double> autosizedRatedPower() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setRatedCapacity(boost::optional<double> ratedCapacity);

      void autosizeRatedCapacity();

      bool setRatedPower(boost::optional<double> ratedPower);

      void resetRatedPower();

      void autosizeRatedPower();

      bool setRatedFanPower(boost::optional<double> ratedFanPower);

      void resetRatedFanPower();

      bool setStandbyPower(boost::optional<double> standbyPower);

      void resetStandbyPower();

      // bool setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetWaterStorageTank();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP
