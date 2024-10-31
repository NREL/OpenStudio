/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardRadiantConvectiveWater_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardRadiantConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Water'. */
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveWater : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACBaseboardRadiantConvectiveWater(const Model& model);

    virtual ~ZoneHVACBaseboardRadiantConvectiveWater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardRadiantConvectiveWater(const ZoneHVACBaseboardRadiantConvectiveWater& other) = default;
    ZoneHVACBaseboardRadiantConvectiveWater(ZoneHVACBaseboardRadiantConvectiveWater&& other) = default;
    ZoneHVACBaseboardRadiantConvectiveWater& operator=(const ZoneHVACBaseboardRadiantConvectiveWater&) = default;
    ZoneHVACBaseboardRadiantConvectiveWater& operator=(ZoneHVACBaseboardRadiantConvectiveWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    HVACComponent heatingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    //@}
    /** @name Other */
    //@{

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveWater is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardRadiantConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardRadiantConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl;

    explicit ZoneHVACBaseboardRadiantConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl> impl);

    friend class detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveWater");
  };

  /** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
  using OptionalZoneHVACBaseboardRadiantConvectiveWater = boost::optional<ZoneHVACBaseboardRadiantConvectiveWater>;

  /** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
  using ZoneHVACBaseboardRadiantConvectiveWaterVector = std::vector<ZoneHVACBaseboardRadiantConvectiveWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP
