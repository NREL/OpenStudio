/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HUMIDIFIERSTEAMGAS_HPP
#define MODEL_HUMIDIFIERSTEAMGAS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class HumidifierSteamGas_Impl;

  }  // namespace detail

  /** HumidifierSteamGas is a StraightComponent that wraps the OpenStudio IDD object 'OS:Humidifier:Steam:Gas'. */
  class MODEL_API HumidifierSteamGas : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HumidifierSteamGas(const Model& model);

    virtual ~HumidifierSteamGas() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HumidifierSteamGas(const HumidifierSteamGas& other) = default;
    HumidifierSteamGas(HumidifierSteamGas&& other) = default;
    HumidifierSteamGas& operator=(const HumidifierSteamGas&) = default;
    HumidifierSteamGas& operator=(HumidifierSteamGas&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRatedCapacity(double ratedCapacity);

    void autosizeRatedCapacity();

    bool setRatedGasUseRate(double ratedGasUseRate);

    void resetRatedGasUseRate();

    void autosizeRatedGasUseRate();

    bool setThermalEfficiency(double thermalEfficiency);

    void resetThermalEfficiency();

    bool setThermalEfficiencyModifierCurve(const Curve& curve);

    void resetThermalEfficiencyModifierCurve();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setAuxiliaryElectricPower(double auxiliaryElectricPower);

    void resetAuxiliaryElectricPower();

    // bool setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetWaterStorageTank();

    bool setInletWaterTemperatureOption(const std::string& inletWaterTemperatureOption);

    void resetInletWaterTemperatureOption();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedCapacity() const;

    boost::optional<double> autosizedRatedGasUseRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HumidifierSteamGas_Impl;

    explicit HumidifierSteamGas(std::shared_ptr<detail::HumidifierSteamGas_Impl> impl);

    friend class detail::HumidifierSteamGas_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HumidifierSteamGas");
  };

  /** \relates HumidifierSteamGas*/
  using OptionalHumidifierSteamGas = boost::optional<HumidifierSteamGas>;

  /** \relates HumidifierSteamGas*/
  using HumidifierSteamGasVector = std::vector<HumidifierSteamGas>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HUMIDIFIERSTEAMGAS_HPP
