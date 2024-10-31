/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"
#include "ThermalZone.hpp"
#include "Model.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class StraightComponent;

  namespace detail {

    class ZoneHVACBaseboardConvectiveWater_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:Convective:Water'. */
  class MODEL_API ZoneHVACBaseboardConvectiveWater : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACBaseboardConvectiveWater(const Model& model, Schedule& availabilitySchedule, StraightComponent& heatingCoilBaseboard);

    virtual ~ZoneHVACBaseboardConvectiveWater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardConvectiveWater(const ZoneHVACBaseboardConvectiveWater& other) = default;
    ZoneHVACBaseboardConvectiveWater(ZoneHVACBaseboardConvectiveWater&& other) = default;
    ZoneHVACBaseboardConvectiveWater& operator=(const ZoneHVACBaseboardConvectiveWater&) = default;
    ZoneHVACBaseboardConvectiveWater& operator=(ZoneHVACBaseboardConvectiveWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    StraightComponent heatingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingCoil(const StraightComponent& heatingCoilBaseboard);

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardConvectiveWater is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardConvectiveWater_Impl;

    explicit ZoneHVACBaseboardConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl);

    friend class detail::ZoneHVACBaseboardConvectiveWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveWater");
  };

  /** \relates ZoneHVACBaseboardConvectiveWater*/
  using OptionalZoneHVACBaseboardConvectiveWater = boost::optional<ZoneHVACBaseboardConvectiveWater>;

  /** \relates ZoneHVACBaseboardConvectiveWater*/
  using ZoneHVACBaseboardConvectiveWaterVector = std::vector<ZoneHVACBaseboardConvectiveWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP
