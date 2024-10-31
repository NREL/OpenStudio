/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACCOOLINGPANELRADIANTCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACCOOLINGPANELRADIANTCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACCoolingPanelRadiantConvectiveWater_Impl;

  }  // namespace detail

  /** ZoneHVACCoolingPanelRadiantConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:CoolingPanel:RadiantConvective:Water'. */
  class MODEL_API ZoneHVACCoolingPanelRadiantConvectiveWater : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACCoolingPanelRadiantConvectiveWater(const Model& model);

    virtual ~ZoneHVACCoolingPanelRadiantConvectiveWater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACCoolingPanelRadiantConvectiveWater(const ZoneHVACCoolingPanelRadiantConvectiveWater& other) = default;
    ZoneHVACCoolingPanelRadiantConvectiveWater(ZoneHVACCoolingPanelRadiantConvectiveWater&& other) = default;
    ZoneHVACCoolingPanelRadiantConvectiveWater& operator=(const ZoneHVACCoolingPanelRadiantConvectiveWater&) = default;
    ZoneHVACCoolingPanelRadiantConvectiveWater& operator=(ZoneHVACCoolingPanelRadiantConvectiveWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    HVACComponent coolingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    bool setCoolingCoil(const HVACComponent& coolingCoil);

    //@}
    /** @name Other */
    //@{

    /** Returns the optional ThermalZone that this ZoneHVACCoolingPanelRadiantConvectiveWater is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACCoolingPanelRadiantConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACCoolingPanelRadiantConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl;

    explicit ZoneHVACCoolingPanelRadiantConvectiveWater(std::shared_ptr<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl> impl);

    friend class detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACCoolingPanelRadiantConvectiveWater");
  };

  /** \relates ZoneHVACCoolingPanelRadiantConvectiveWater*/
  using OptionalZoneHVACCoolingPanelRadiantConvectiveWater = boost::optional<ZoneHVACCoolingPanelRadiantConvectiveWater>;

  /** \relates ZoneHVACCoolingPanelRadiantConvectiveWater*/
  using ZoneHVACCoolingPanelRadiantConvectiveWaterVector = std::vector<ZoneHVACCoolingPanelRadiantConvectiveWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACCOOLINGPANELRADIANTCONVECTIVEWATER_HPP
