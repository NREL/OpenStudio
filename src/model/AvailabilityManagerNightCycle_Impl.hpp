/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "ModelObjectList.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class AirLoopHVAC;

  namespace detail {

    /** AvailabilityManagerNightCycle_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerNightCycle.*/
    class MODEL_API AvailabilityManagerNightCycle_Impl : public AvailabilityManager_Impl
    {

     public:
      AvailabilityManagerNightCycle_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightCycle_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightCycle_Impl(const AvailabilityManagerNightCycle_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerNightCycle_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // Cloning should ensure we do have modelObjectLists for control zones, but they should be empty
      // as it is not connected to an AirLoopHVAC (yet) and it wouldn't make sense to have zones there
      virtual ModelObject clone(Model model) const override;

      // Clears the ModelObjectLists, then remove them
      virtual std::vector<IdfObject> remove() override;

      boost::optional<AirLoopHVAC> airLoopHVAC() const;

      Schedule applicabilitySchedule() const;
      bool setApplicabilitySchedule(Schedule& schedule);

      boost::optional<Schedule> fanSchedule() const;
      // Not implemented, it uses AirLoopHVAC::availabilitySchedule
      // bool setFanSchedule(Schedule& schedule);

      std::string controlType() const;
      bool isControlTypeDefaulted() const;
      bool setControlType(const std::string& controlType);
      void resetControlType();

      double thermostatTolerance() const;
      bool isThermostatToleranceDefaulted() const;
      bool setThermostatTolerance(double thermostatTolerance);
      void resetThermostatTolerance();

      std::string cyclingRunTimeControlType() const;
      bool isCyclingRunTimeControlTypeDefaulted() const;
      bool setCyclingRunTimeControlType(const std::string& cyclingRunTimeControlType);
      void resetCyclingRunTimeControlType();

      double cyclingRunTime() const;
      bool isCyclingRunTimeDefaulted() const;
      bool setCyclingRunTime(double cyclingRunTime);
      void resetCyclingRunTime();

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

     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");

      boost::optional<Schedule> optionalApplicabilitySchedule() const;

      std::vector<std::string> controlTypeValues() const;
      std::vector<std::string> cyclingRunTimeControlTypeValues() const;

      ModelObjectList controlThermalZoneList() const;
      void clearControlThermalZoneList();

      ModelObjectList coolingControlThermalZoneList() const;
      void clearCoolingControlThermalZoneList();

      ModelObjectList heatingControlThermalZoneList() const;
      void clearHeatingControlThermalZoneList();

      ModelObjectList heatingZoneFansOnlyThermalZoneList() const;
      void clearHeatingZoneFansOnlyThermalZoneList();
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
