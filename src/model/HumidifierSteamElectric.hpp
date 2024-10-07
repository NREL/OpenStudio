/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class HumidifierSteamElectric_Impl;

  }  // namespace detail

  /** HumidifierSteamElectric is a StraightComponent that wraps the OpenStudio IDD object 'OS:Humidifier:Steam:Electric'. */
  class MODEL_API HumidifierSteamElectric : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HumidifierSteamElectric(const Model& model);

    virtual ~HumidifierSteamElectric() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HumidifierSteamElectric(const HumidifierSteamElectric& other) = default;
    HumidifierSteamElectric(HumidifierSteamElectric&& other) = default;
    HumidifierSteamElectric& operator=(const HumidifierSteamElectric&) = default;
    HumidifierSteamElectric& operator=(HumidifierSteamElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRatedCapacity(double ratedCapacity);

    void autosizeRatedCapacity();

    bool setRatedPower(double ratedPower);

    void resetRatedPower();

    void autosizeRatedPower();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setStandbyPower(double standbyPower);

    void resetStandbyPower();

    // bool setWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedCapacity() const;

    boost::optional<double> autosizedRatedPower() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HumidifierSteamElectric_Impl;

    explicit HumidifierSteamElectric(std::shared_ptr<detail::HumidifierSteamElectric_Impl> impl);

    friend class detail::HumidifierSteamElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
  };

  /** \relates HumidifierSteamElectric*/
  using OptionalHumidifierSteamElectric = boost::optional<HumidifierSteamElectric>;

  /** \relates HumidifierSteamElectric*/
  using HumidifierSteamElectricVector = std::vector<HumidifierSteamElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HUMIDIFIERSTEAMELECTRIC_HPP
