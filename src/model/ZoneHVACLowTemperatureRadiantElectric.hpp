/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP
#define MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACLowTemperatureRadiantElectric_Impl;

  }  // namespace detail

  /** ZoneHVACLowTemperatureRadiantElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:Electric'. */
  class MODEL_API ZoneHVACLowTemperatureRadiantElectric : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACLowTemperatureRadiantElectric(const Model& model, Schedule& availabilitySchedule, Schedule& heatingTemperatureSchedule);

    virtual ~ZoneHVACLowTemperatureRadiantElectric() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACLowTemperatureRadiantElectric(const ZoneHVACLowTemperatureRadiantElectric& other) = default;
    ZoneHVACLowTemperatureRadiantElectric(ZoneHVACLowTemperatureRadiantElectric&& other) = default;
    ZoneHVACLowTemperatureRadiantElectric& operator=(const ZoneHVACLowTemperatureRadiantElectric&) = default;
    ZoneHVACLowTemperatureRadiantElectric& operator=(ZoneHVACLowTemperatureRadiantElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> radiantSurfaceTypeValues();

    static std::vector<std::string> temperatureControlTypeValues();

    /** @name Getters */

    Schedule availabilitySchedule() const;

    Schedule heatingSetpointTemperatureSchedule() const;

    boost::optional<std::string> radiantSurfaceType() const;

    std::vector<Surface> surfaces() const;

    /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Heating Design Capacity" **/
    boost::optional<double> maximumElectricalPowertoPanel() const;

    bool isMaximumElectricalPowertoPanelDefaulted() const;

    bool isMaximumElectricalPowertoPanelAutosized() const;

    std::string temperatureControlType() const;

    bool isTemperatureControlTypeDefaulted() const;

    std::string setpointControlType() const;

    bool isSetpointControlTypeDefaulted() const;

    double heatingThrottlingRange() const;

    bool isHeatingThrottlingRangeDefaulted() const;

    /** @name Setters */

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    bool setRadiantSurfaceType(const std::string& radiantSurfaceType);

    void resetRadiantSurfaceType();

    bool setMaximumElectricalPowertoPanel(double maximumElectricalPowertoPanel);

    void resetMaximumElectricalPowertoPanel();

    void autosizeMaximumElectricalPowertoPanel();

    bool setTemperatureControlType(const std::string& temperatureControlType);

    void resetTemperatureControlType();

    bool setSetpointControlType(const std::string& setpointControlType);

    void resetSetpointControlType();

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    void resetHeatingThrottlingRange();

    boost::optional<ThermalZone> thermalZone() const override;

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();

    /** @name Other */

    boost::optional<double> autosizedMaximumElectricalPowertoPanel() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACLowTemperatureRadiantElectric_Impl;

    explicit ZoneHVACLowTemperatureRadiantElectric(std::shared_ptr<detail::ZoneHVACLowTemperatureRadiantElectric_Impl> impl);

    friend class detail::ZoneHVACLowTemperatureRadiantElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACLowTemperatureRadiantElectric");
  };

  /** \relates ZoneHVACLowTemperatureRadiantElectric*/
  using OptionalZoneHVACLowTemperatureRadiantElectric = boost::optional<ZoneHVACLowTemperatureRadiantElectric>;

  /** \relates ZoneHVACLowTemperatureRadiantElectric*/
  using ZoneHVACLowTemperatureRadiantElectricVector = std::vector<ZoneHVACLowTemperatureRadiantElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP
