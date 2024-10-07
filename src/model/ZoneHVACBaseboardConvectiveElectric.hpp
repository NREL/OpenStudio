/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardConvectiveElectric_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardConvectiveElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:Convective:Electric'. */
  class MODEL_API ZoneHVACBaseboardConvectiveElectric : public ZoneHVACComponent
  {

   public:
    explicit ZoneHVACBaseboardConvectiveElectric(const Model& model);

    virtual ~ZoneHVACBaseboardConvectiveElectric() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardConvectiveElectric(const ZoneHVACBaseboardConvectiveElectric& other) = default;
    ZoneHVACBaseboardConvectiveElectric(ZoneHVACBaseboardConvectiveElectric&& other) = default;
    ZoneHVACBaseboardConvectiveElectric& operator=(const ZoneHVACBaseboardConvectiveElectric&) = default;
    ZoneHVACBaseboardConvectiveElectric& operator=(ZoneHVACBaseboardConvectiveElectric&&) = default;

    static IddObjectType iddObjectType();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    /** In EnergyPlus 8.2.0 and up, this property maps to the EnergyPLus field "Heating Design Capacity"
  **/
    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    bool setNominalCapacity(double nominalCapacity);

    void autosizeNominalCapacity();

    double efficiency() const;

    bool isEfficiencyDefaulted() const;

    bool setEfficiency(double efficiency);

    void resetEfficiency();

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardConvectiveElectric is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardConvectiveElectric to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardConvectiveElectric from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardConvectiveElectric_Impl;

    explicit ZoneHVACBaseboardConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveElectric_Impl> impl);

    friend class detail::ZoneHVACBaseboardConvectiveElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveElectric");
  };

  /** \relates ZoneHVACBaseboardConvectiveElectric*/
  using OptionalZoneHVACBaseboardConvectiveElectric = boost::optional<ZoneHVACBaseboardConvectiveElectric>;

  /** \relates ZoneHVACBaseboardConvectiveElectric*/
  using ZoneHVACBaseboardConvectiveElectricVector = std::vector<ZoneHVACBaseboardConvectiveElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP
