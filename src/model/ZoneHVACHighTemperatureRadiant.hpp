/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Surface;

  namespace detail {

    class ZoneHVACHighTemperatureRadiant_Impl;

  }  // namespace detail

  /** ZoneHVACHighTemperatureRadiant is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:HighTemperatureRadiant'. */
  class MODEL_API ZoneHVACHighTemperatureRadiant : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACHighTemperatureRadiant(const Model& model);

    virtual ~ZoneHVACHighTemperatureRadiant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACHighTemperatureRadiant(const ZoneHVACHighTemperatureRadiant& other) = default;
    ZoneHVACHighTemperatureRadiant(ZoneHVACHighTemperatureRadiant&& other) = default;
    ZoneHVACHighTemperatureRadiant& operator=(const ZoneHVACHighTemperatureRadiant&) = default;
    ZoneHVACHighTemperatureRadiant& operator=(ZoneHVACHighTemperatureRadiant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    static std::vector<std::string> temperatureControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Heating Design Capacity" **/
    boost::optional<double> maximumPowerInput() const;

    bool isMaximumPowerInputAutosized() const;

    std::string fuelType() const;

    double combustionEfficiency() const;

    double fractionofInputConvertedtoRadiantEnergy() const;

    double fractionofInputConvertedtoLatentEnergy() const;

    double fractionofInputthatIsLost() const;

    std::string temperatureControlType() const;

    double heatingThrottlingRange() const;

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumPowerInput(double maximumPowerInput);

    void autosizeMaximumPowerInput();

    bool setFuelType(const std::string& fuelType);

    bool setCombustionEfficiency(double combustionEfficiency);

    bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

    bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

    bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

    bool setTemperatureControlType(const std::string& temperatureControlType);

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    boost::optional<ThermalZone> thermalZone() const override;

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();
    //@}

    boost::optional<double> autosizedMaximumPowerInput() const;

   protected:
    /// @cond
    using ImplType = detail::ZoneHVACHighTemperatureRadiant_Impl;

    explicit ZoneHVACHighTemperatureRadiant(std::shared_ptr<detail::ZoneHVACHighTemperatureRadiant_Impl> impl);

    friend class detail::ZoneHVACHighTemperatureRadiant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
  };

  /** \relates ZoneHVACHighTemperatureRadiant*/
  using OptionalZoneHVACHighTemperatureRadiant = boost::optional<ZoneHVACHighTemperatureRadiant>;

  /** \relates ZoneHVACHighTemperatureRadiant*/
  using ZoneHVACHighTemperatureRadiantVector = std::vector<ZoneHVACHighTemperatureRadiant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP
