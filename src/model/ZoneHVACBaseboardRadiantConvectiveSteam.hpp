/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardRadiantConvectiveSteam_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardRadiantConvectiveSteam is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Steam'. */
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveSteam : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACBaseboardRadiantConvectiveSteam(const Model& model);

    virtual ~ZoneHVACBaseboardRadiantConvectiveSteam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardRadiantConvectiveSteam(const ZoneHVACBaseboardRadiantConvectiveSteam& other) = default;
    ZoneHVACBaseboardRadiantConvectiveSteam(ZoneHVACBaseboardRadiantConvectiveSteam&& other) = default;
    ZoneHVACBaseboardRadiantConvectiveSteam& operator=(const ZoneHVACBaseboardRadiantConvectiveSteam&) = default;
    ZoneHVACBaseboardRadiantConvectiveSteam& operator=(ZoneHVACBaseboardRadiantConvectiveSteam&&) = default;

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

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveSteam is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardRadiantConvectiveSteam to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardRadiantConvectiveSteam from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl;

    explicit ZoneHVACBaseboardRadiantConvectiveSteam(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl> impl);

    friend class detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveSteam");
  };

  /** \relates ZoneHVACBaseboardRadiantConvectiveSteam*/
  using OptionalZoneHVACBaseboardRadiantConvectiveSteam = boost::optional<ZoneHVACBaseboardRadiantConvectiveSteam>;

  /** \relates ZoneHVACBaseboardRadiantConvectiveSteam*/
  using ZoneHVACBaseboardRadiantConvectiveSteamVector = std::vector<ZoneHVACBaseboardRadiantConvectiveSteam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVESTEAM_HPP
