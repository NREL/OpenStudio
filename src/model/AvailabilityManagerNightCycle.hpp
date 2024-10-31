/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;
  class AirLoopHVAC;

  namespace detail {

    class AvailabilityManagerNightCycle_Impl;

  }  // namespace detail

  /** AvailabilityManagerNightCycle is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightCycle'. */
  class MODEL_API AvailabilityManagerNightCycle : public AvailabilityManager
  {
   public:
    explicit AvailabilityManagerNightCycle(const Model& model);

    virtual ~AvailabilityManagerNightCycle() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerNightCycle(const AvailabilityManagerNightCycle& other) = default;
    AvailabilityManagerNightCycle(AvailabilityManagerNightCycle&& other) = default;
    AvailabilityManagerNightCycle& operator=(const AvailabilityManagerNightCycle&) = default;
    AvailabilityManagerNightCycle& operator=(AvailabilityManagerNightCycle&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlTypeValues();

    boost::optional<AirLoopHVAC> airLoopHVAC() const;

    Schedule applicabilitySchedule() const;
    bool setApplicabilitySchedule(Schedule& schedule);

    /** Helper getter which will fetch the AirLoopHVAC::AvailabilitySchedule */
    boost::optional<Schedule> fanSchedule() const;

    std::string controlType() const;
    bool setControlType(const std::string& controlType);
    void resetControlType();
    bool isControlTypeDefaulted() const;

    double thermostatTolerance() const;
    bool setThermostatTolerance(double thermostatTolerance);
    bool isThermostatToleranceDefaulted() const;
    void resetThermostatTolerance();

    double cyclingRunTime() const;
    bool setCyclingRunTime(double cyclingRunTime);
    void resetCyclingRunTime();
    bool isCyclingRunTimeDefaulted() const;

    static std::vector<std::string> cyclingRunTimeControlTypeValues();
    std::string cyclingRunTimeControlType() const;
    bool isCyclingRunTimeControlTypeDefaulted() const;
    bool setCyclingRunTimeControlType(const std::string& cyclingRunTimeControlType);
    void resetCyclingRunTimeControlType();

    std::vector<ThermalZone> controlThermalZones() const;
    bool setControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetControlThermalZones();

    std::vector<ThermalZone> coolingControlThermalZones() const;
    bool setCoolingControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetCoolingControlThermalZones();

    std::vector<ThermalZone> heatingControlThermalZones() const;
    bool setHeatingControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetHeatingControlThermalZones();

    std::vector<ThermalZone> heatingZoneFansOnlyThermalZones() const;
    bool setHeatingZoneFansOnlyThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetHeatingZoneFansOnlyThermalZones();

   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerNightCycle_Impl;

    explicit AvailabilityManagerNightCycle(std::shared_ptr<detail::AvailabilityManagerNightCycle_Impl> impl);

    friend class detail::AvailabilityManagerNightCycle_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");
  };

  /** \relates AvailabilityManagerNightCycle*/
  using OptionalAvailabilityManagerNightCycle = boost::optional<AvailabilityManagerNightCycle>;

  /** \relates AvailabilityManagerNightCycle*/
  using AvailabilityManagerNightCycleVector = std::vector<AvailabilityManagerNightCycle>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP
