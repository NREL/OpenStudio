/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECONTROLHUMIDISTAT_HPP
#define MODEL_ZONECONTROLHUMIDISTAT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneControlHumidistat_Impl;

  }  // namespace detail

  /** ZoneControlHumidistat is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:Humidistat'. */
  class MODEL_API ZoneControlHumidistat : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneControlHumidistat(const Model& model);

    virtual ~ZoneControlHumidistat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneControlHumidistat(const ZoneControlHumidistat& other) = default;
    ZoneControlHumidistat(ZoneControlHumidistat&& other) = default;
    ZoneControlHumidistat& operator=(const ZoneControlHumidistat&) = default;
    ZoneControlHumidistat& operator=(ZoneControlHumidistat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ThermalZone> controlledZone() const;

    boost::optional<Schedule> humidifyingRelativeHumiditySetpointSchedule() const;

    boost::optional<Schedule> dehumidifyingRelativeHumiditySetpointSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

    bool setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

    void resetHumidifyingRelativeHumiditySetpointSchedule();

    void resetDehumidifyingRelativeHumiditySetpointSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneControlHumidistat_Impl;

    explicit ZoneControlHumidistat(std::shared_ptr<detail::ZoneControlHumidistat_Impl> impl);

    friend class detail::ZoneControlHumidistat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlHumidistat");
  };

  /** \relates ZoneControlHumidistat*/
  using OptionalZoneControlHumidistat = boost::optional<ZoneControlHumidistat>;

  /** \relates ZoneControlHumidistat*/
  using ZoneControlHumidistatVector = std::vector<ZoneControlHumidistat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLHUMIDISTAT_HPP
