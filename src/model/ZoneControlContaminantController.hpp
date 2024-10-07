/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP
#define MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    class ZoneControlContaminantController_Impl;

  }  // namespace detail

  /** ZoneControlContaminantController is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:ContaminantController'. */
  class MODEL_API ZoneControlContaminantController : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneControlContaminantController(const Model& model);

    virtual ~ZoneControlContaminantController() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneControlContaminantController(const ZoneControlContaminantController& other) = default;
    ZoneControlContaminantController(ZoneControlContaminantController&& other) = default;
    ZoneControlContaminantController& operator=(const ZoneControlContaminantController&) = default;
    ZoneControlContaminantController& operator=(ZoneControlContaminantController&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ThermalZone> controlledZone() const;

    boost::optional<Schedule> carbonDioxideControlAvailabilitySchedule() const;

    boost::optional<Schedule> carbonDioxideSetpointSchedule() const;

    boost::optional<Schedule> minimumCarbonDioxideConcentrationSchedule() const;

    boost::optional<Schedule> genericContaminantControlAvailabilitySchedule() const;

    boost::optional<Schedule> genericContaminantSetpointSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule);

    void resetCarbonDioxideControlAvailabilitySchedule();

    bool setCarbonDioxideSetpointSchedule(Schedule& schedule);

    void resetCarbonDioxideSetpointSchedule();

    bool setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule);

    void resetMinimumCarbonDioxideConcentrationSchedule();

    bool setGenericContaminantControlAvailabilitySchedule(Schedule& schedule);

    void resetGenericContaminantControlAvailabilitySchedule();

    bool setGenericContaminantSetpointSchedule(Schedule& schedule);

    void resetGenericContaminantSetpointSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneControlContaminantController_Impl;

    explicit ZoneControlContaminantController(std::shared_ptr<detail::ZoneControlContaminantController_Impl> impl);

    friend class detail::ZoneControlContaminantController_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlContaminantController");
  };

  /** \relates ZoneControlContaminantController*/
  using OptionalZoneControlContaminantController = boost::optional<ZoneControlContaminantController>;

  /** \relates ZoneControlContaminantController*/
  using ZoneControlContaminantControllerVector = std::vector<ZoneControlContaminantController>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLCONTAMINANTCONTROLLER_HPP
