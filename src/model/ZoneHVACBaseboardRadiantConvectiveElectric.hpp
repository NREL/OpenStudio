/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardRadiantConvectiveElectric_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardRadiantConvectiveElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Electric'. */
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveElectric : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACBaseboardRadiantConvectiveElectric(const Model& model);

    virtual ~ZoneHVACBaseboardRadiantConvectiveElectric() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardRadiantConvectiveElectric(const ZoneHVACBaseboardRadiantConvectiveElectric& other) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric(ZoneHVACBaseboardRadiantConvectiveElectric&& other) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric& operator=(const ZoneHVACBaseboardRadiantConvectiveElectric&) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric& operator=(ZoneHVACBaseboardRadiantConvectiveElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    double efficiency() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setEfficiency(double efficiency);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveElectric is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardRadiantConvectiveElectric to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardRadiantConvectiveElectric from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl;

    explicit ZoneHVACBaseboardRadiantConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl> impl);

    friend class detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveElectric");
  };

  /** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
  using OptionalZoneHVACBaseboardRadiantConvectiveElectric = boost::optional<ZoneHVACBaseboardRadiantConvectiveElectric>;

  /** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
  using ZoneHVACBaseboardRadiantConvectiveElectricVector = std::vector<ZoneHVACBaseboardRadiantConvectiveElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP
