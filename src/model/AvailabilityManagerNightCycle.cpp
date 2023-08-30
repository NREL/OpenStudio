/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerNightCycle.hpp"
#include "AvailabilityManagerNightCycle_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ModelObject.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Model.hpp"
#include "Loop.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerNightCycle_Impl::AvailabilityManagerNightCycle_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerNightCycle::iddObjectType());
    }

    AvailabilityManagerNightCycle_Impl::AvailabilityManagerNightCycle_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerNightCycle::iddObjectType());
    }

    AvailabilityManagerNightCycle_Impl::AvailabilityManagerNightCycle_Impl(const AvailabilityManagerNightCycle_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerNightCycle_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Night Cycle Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerNightCycle_Impl::iddObjectType() const {
      return AvailabilityManagerNightCycle::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AvailabilityManagerNightCycle_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AvailabilityManager_NightCycleFields::ApplicabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AvailabilityManagerNightCycle", "Applicability Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> AvailabilityManagerNightCycle_Impl::optionalApplicabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_NightCycleFields::ApplicabilitySchedule);
    }

    Schedule AvailabilityManagerNightCycle_Impl::applicabilitySchedule() const {
      boost::optional<Schedule> value = optionalApplicabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Applicability Schedule attached.");
      }
      return value.get();
    }

    bool AvailabilityManagerNightCycle_Impl::setApplicabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AvailabilityManager_NightCycleFields::ApplicabilitySchedule, "AvailabilityManagerNightCycle",
                                "Applicability Schedule", schedule);
      return result;
    }

    ModelObject AvailabilityManagerNightCycle_Impl::clone(Model model) const {
      auto avmClone = ModelObject_Impl::clone(model).cast<AvailabilityManagerNightCycle>();

      // Now recreate the lists
      // Control Zone List
      ModelObjectList controlThermalZoneList = ModelObjectList(model);
      controlThermalZoneList.setName(this->name().get() + " Control Zone List");
      bool ok = avmClone.getImpl<detail::AvailabilityManager_Impl>()->setPointer(OS_AvailabilityManager_NightCycleFields::ControlZoneorZoneListName,
                                                                                 controlThermalZoneList.handle());
      OS_ASSERT(ok);

      // Cooling Control Zone List
      ModelObjectList coolingControlThermalZoneList = ModelObjectList(model);
      coolingControlThermalZoneList.setName(this->name().get() + " Cooling Control Zone List");
      ok = avmClone.getImpl<detail::AvailabilityManager_Impl>()->setPointer(OS_AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName,
                                                                            coolingControlThermalZoneList.handle());
      OS_ASSERT(ok);

      // Heating Control Zone List
      ModelObjectList heatingControlThermalZoneList = ModelObjectList(model);
      heatingControlThermalZoneList.setName(this->name().get() + " Heating Control Zone List");
      ok = avmClone.getImpl<detail::AvailabilityManager_Impl>()->setPointer(OS_AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName,
                                                                            heatingControlThermalZoneList.handle());
      OS_ASSERT(ok);

      // Heating Zone Fans Only Zone List
      ModelObjectList heatingZoneFansOnlyThermalZoneList = ModelObjectList(model);
      heatingZoneFansOnlyThermalZoneList.setName(this->name().get() + " Heating Zone Fans Only Zone List");
      ok = avmClone.getImpl<detail::AvailabilityManager_Impl>()->setPointer(
        OS_AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName, heatingZoneFansOnlyThermalZoneList.handle());
      OS_ASSERT(ok);

      return std::move(avmClone);
    }

    std::vector<IdfObject> AvailabilityManagerNightCycle_Impl::remove() {
      std::vector<IdfObject> result;

      // Remove the ModelObjectLists. You have to clear them first, or it'll also remove the ThermalZones contained within.
      {
        ModelObjectList mo_list = controlThermalZoneList();
        mo_list.removeAllModelObjects();
        std::vector<IdfObject> removedMoList = mo_list.remove();
        result.insert(result.end(), removedMoList.begin(), removedMoList.end());
      }

      {
        ModelObjectList mo_list = coolingControlThermalZoneList();
        mo_list.removeAllModelObjects();
        std::vector<IdfObject> removedMoList = mo_list.remove();
        result.insert(result.end(), removedMoList.begin(), removedMoList.end());
      }

      {
        ModelObjectList mo_list = heatingControlThermalZoneList();
        mo_list.removeAllModelObjects();
        std::vector<IdfObject> removedMoList = mo_list.remove();
        result.insert(result.end(), removedMoList.begin(), removedMoList.end());
      }

      {
        ModelObjectList mo_list = heatingZoneFansOnlyThermalZoneList();
        mo_list.removeAllModelObjects();
        std::vector<IdfObject> removedMoList = mo_list.remove();
        result.insert(result.end(), removedMoList.begin(), removedMoList.end());
      }

      std::vector<IdfObject> AvailabilityManagerNightCycle = ModelObject_Impl::remove();
      result.insert(result.end(), AvailabilityManagerNightCycle.begin(), AvailabilityManagerNightCycle.end());

      return result;
    }

    boost::optional<AirLoopHVAC> AvailabilityManagerNightCycle_Impl::airLoopHVAC() const {
      if (boost::optional<Loop> _loop = loop()) {
        return _loop->optionalCast<AirLoopHVAC>();
      }
      return boost::none;
    }

    boost::optional<Schedule> AvailabilityManagerNightCycle_Impl::fanSchedule() const {
      boost::optional<Schedule> result;
      if (boost::optional<AirLoopHVAC> _airLoop = airLoopHVAC()) {
        result = _airLoop->availabilitySchedule();
      }
      return result;
    }

    std::string AvailabilityManagerNightCycle_Impl::controlType() const {
      boost::optional<std::string> value = getString(OS_AvailabilityManager_NightCycleFields::ControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerNightCycle_Impl::isControlTypeDefaulted() const {
      return isEmpty(OS_AvailabilityManager_NightCycleFields::ControlType);
    }

    double AvailabilityManagerNightCycle_Impl::thermostatTolerance() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_NightCycleFields::ThermostatTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerNightCycle_Impl::isThermostatToleranceDefaulted() const {
      return isEmpty(OS_AvailabilityManager_NightCycleFields::ThermostatTolerance);
    }

    double AvailabilityManagerNightCycle_Impl::cyclingRunTime() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_NightCycleFields::CyclingRunTime, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerNightCycle_Impl::isCyclingRunTimeDefaulted() const {
      return isEmpty(OS_AvailabilityManager_NightCycleFields::CyclingRunTime);
    }

    bool AvailabilityManagerNightCycle_Impl::setControlType(const std::string& controlType) {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::ControlType, controlType);
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetControlType() {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::ControlType, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerNightCycle_Impl::setThermostatTolerance(double thermostatTolerance) {
      bool result = setDouble(OS_AvailabilityManager_NightCycleFields::ThermostatTolerance, thermostatTolerance);
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetThermostatTolerance() {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::ThermostatTolerance, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerNightCycle_Impl::setCyclingRunTime(double cyclingRunTime) {
      bool result = setDouble(OS_AvailabilityManager_NightCycleFields::CyclingRunTime, cyclingRunTime);
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetCyclingRunTime() {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::CyclingRunTime, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> AvailabilityManagerNightCycle_Impl::controlTypeValues() const {
      return AvailabilityManagerNightCycle::controlTypeValues();
    }

    // Cycling Run Time Control Type
    std::vector<std::string> AvailabilityManagerNightCycle_Impl::cyclingRunTimeControlTypeValues() const {
      return AvailabilityManagerNightCycle::cyclingRunTimeControlTypeValues();
    }

    std::string AvailabilityManagerNightCycle_Impl::cyclingRunTimeControlType() const {
      boost::optional<std::string> value = getString(OS_AvailabilityManager_NightCycleFields::CyclingRunTimeControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerNightCycle_Impl::isCyclingRunTimeControlTypeDefaulted() const {
      return isEmpty(OS_AvailabilityManager_NightCycleFields::CyclingRunTimeControlType);
    }

    bool AvailabilityManagerNightCycle_Impl::setCyclingRunTimeControlType(const std::string& cyclingRunTimeControlType) {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::CyclingRunTimeControlType, cyclingRunTimeControlType);
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetCyclingRunTimeControlType() {
      bool result = setString(OS_AvailabilityManager_NightCycleFields::CyclingRunTimeControlType, "");
      OS_ASSERT(result);
    }

    // Control Zone
    ModelObjectList AvailabilityManagerNightCycle_Impl::controlThermalZoneList() const {
      boost::optional<ModelObjectList> mo_list =
        getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_AvailabilityManager_NightCycleFields::ControlZoneorZoneListName);
      if (mo_list) {
        return mo_list.get();
      } else {
        LOG_AND_THROW(briefDescription() << " does not have a controlThermalZoneList attached");
      }
    }
    void AvailabilityManagerNightCycle_Impl::clearControlThermalZoneList() {
      ModelObjectList mo_list = controlThermalZoneList();
      mo_list.removeAllModelObjects();
    }

    std::vector<ThermalZone> AvailabilityManagerNightCycle_Impl::controlThermalZones() const {
      ModelObjectList mo_list = controlThermalZoneList();
      std::vector<ThermalZone> result;
      for (const ModelObject& mo : mo_list.modelObjects()) {
        boost::optional<ThermalZone> thermalZone = mo.optionalCast<ThermalZone>();
        if (thermalZone) {
          result.push_back(thermalZone.get());
        } else {
          LOG_AND_THROW(briefDescription() << " appears to have non ThermalZone objects in the controlThermalZones list");
        }
      }
      return result;
    }

    bool AvailabilityManagerNightCycle_Impl::setControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
      // Clear it first
      clearControlThermalZoneList();
      bool result = false;

      ModelObjectList mo_list = controlThermalZoneList();
      for (const ThermalZone& thermalZone : thermalZones) {
        result = mo_list.addModelObject(thermalZone);
        if (!result) {
          LOG_AND_THROW("Adding " << thermalZone.briefDescription() << " to ControlThermalZones failed for " << briefDescription());
        }
      }
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetControlThermalZones() {
      // Clear it
      clearControlThermalZoneList();
    }

    // Cooling Control Zones
    ModelObjectList AvailabilityManagerNightCycle_Impl::coolingControlThermalZoneList() const {
      boost::optional<ModelObjectList> mo_list =
        getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName);
      if (mo_list) {
        return mo_list.get();
      } else {
        LOG_AND_THROW(briefDescription() << " does not have a coolingControlThermalZoneList attached");
      }
    }

    void AvailabilityManagerNightCycle_Impl::clearCoolingControlThermalZoneList() {
      ModelObjectList mo_list = coolingControlThermalZoneList();
      mo_list.removeAllModelObjects();
    }

    std::vector<ThermalZone> AvailabilityManagerNightCycle_Impl::coolingControlThermalZones() const {
      ModelObjectList mo_list = coolingControlThermalZoneList();
      std::vector<ThermalZone> result;
      for (const ModelObject& mo : mo_list.modelObjects()) {
        boost::optional<ThermalZone> thermalZone = mo.optionalCast<ThermalZone>();
        if (thermalZone) {
          result.push_back(thermalZone.get());
        } else {
          LOG_AND_THROW(briefDescription() << " appears to have non ThermalZone objects in the coolingControlThermalZones list");
        }
      }
      return result;
    }

    bool AvailabilityManagerNightCycle_Impl::setCoolingControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
      // Clear it first
      clearCoolingControlThermalZoneList();
      bool result = false;

      ModelObjectList mo_list = coolingControlThermalZoneList();
      for (const ThermalZone& thermalZone : thermalZones) {
        result = mo_list.addModelObject(thermalZone);
        if (!result) {
          LOG_AND_THROW("Adding " << thermalZone.briefDescription() << " to CoolingControlThermalZones failed for " << briefDescription());
        }
      }
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetCoolingControlThermalZones() {
      // Clear it
      clearCoolingControlThermalZoneList();
    }

    // Heating Control Zones
    ModelObjectList AvailabilityManagerNightCycle_Impl::heatingControlThermalZoneList() const {
      boost::optional<ModelObjectList> mo_list =
        getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName);
      if (mo_list) {
        return mo_list.get();
      } else {
        LOG_AND_THROW(briefDescription() << " does not have a heatingControlThermalZoneList attached");
      }
    }

    void AvailabilityManagerNightCycle_Impl::clearHeatingControlThermalZoneList() {
      ModelObjectList mo_list = heatingControlThermalZoneList();
      mo_list.removeAllModelObjects();
    }

    std::vector<ThermalZone> AvailabilityManagerNightCycle_Impl::heatingControlThermalZones() const {
      ModelObjectList mo_list = heatingControlThermalZoneList();
      std::vector<ThermalZone> result;
      for (const ModelObject& mo : mo_list.modelObjects()) {
        boost::optional<ThermalZone> thermalZone = mo.optionalCast<ThermalZone>();
        if (thermalZone) {
          result.push_back(thermalZone.get());
        } else {
          LOG_AND_THROW(briefDescription() << " appears to have non ThermalZone objects in the heatingControlThermalZones list");
        }
      }
      return result;
    }

    bool AvailabilityManagerNightCycle_Impl::setHeatingControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
      // Clear it first
      clearHeatingControlThermalZoneList();
      bool result = false;

      ModelObjectList mo_list = heatingControlThermalZoneList();
      for (const ThermalZone& thermalZone : thermalZones) {
        result = mo_list.addModelObject(thermalZone);
        if (!result) {
          LOG_AND_THROW("Adding " << thermalZone.briefDescription() << " to HeatingControlThermalZones failed for " << briefDescription());
        }
      }
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetHeatingControlThermalZones() {
      // Clear it
      clearHeatingControlThermalZoneList();
    }

    // Heating Zone Fans Only Zones

    ModelObjectList AvailabilityManagerNightCycle_Impl::heatingZoneFansOnlyThermalZoneList() const {
      boost::optional<ModelObjectList> mo_list = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName);
      if (mo_list) {
        return mo_list.get();
      } else {
        LOG_AND_THROW(briefDescription() << " does not have a heatingZoneFansOnlyThermalZoneList attached");
      }
    }

    void AvailabilityManagerNightCycle_Impl::clearHeatingZoneFansOnlyThermalZoneList() {
      ModelObjectList mo_list = heatingZoneFansOnlyThermalZoneList();
      mo_list.removeAllModelObjects();
    }

    std::vector<ThermalZone> AvailabilityManagerNightCycle_Impl::heatingZoneFansOnlyThermalZones() const {
      ModelObjectList mo_list = heatingZoneFansOnlyThermalZoneList();
      std::vector<ThermalZone> result;
      for (const ModelObject& mo : mo_list.modelObjects()) {
        boost::optional<ThermalZone> thermalZone = mo.optionalCast<ThermalZone>();
        if (thermalZone) {
          result.push_back(thermalZone.get());
        } else {
          LOG_AND_THROW(briefDescription() << " appears to have non ThermalZone objects in the heatingZoneFansOnlyThermalZones list");
        }
      }
      return result;
    }

    bool AvailabilityManagerNightCycle_Impl::setHeatingZoneFansOnlyThermalZones(const std::vector<ThermalZone>& thermalZones) {
      // Clear it first
      clearHeatingZoneFansOnlyThermalZoneList();
      bool result = false;
      ModelObjectList mo_list = heatingZoneFansOnlyThermalZoneList();
      for (const ThermalZone& thermalZone : thermalZones) {
        result = mo_list.addModelObject(thermalZone);
        if (!result) {
          LOG_AND_THROW("Adding " << thermalZone.briefDescription() << " to HeatingZoneFansOnlyThermalZones failed for " << briefDescription());
        }
      }
      return result;
    }

    void AvailabilityManagerNightCycle_Impl::resetHeatingZoneFansOnlyThermalZones() {
      // Clear it
      clearHeatingZoneFansOnlyThermalZoneList();
    }

    /*
 *    bool setControlThermalZones(const std::vector<ThermalZone>& thermalZones);
 *    void resetControlThermalZones();
 *
 *    std::vector<ThermalZone> coolingControlZones() const;
 *    bool setCoolingControlZones(const std::vector<ThermalZone>& thermalZones);
 *    void resetCoolingControlZones();
 *
 *    std::vector<ThermalZone> heatingControlZones() const;
 *    bool setHeatingControlZones(const std::vector<ThermalZone>& thermalZones);
 *    void resetHeatingControlZones();
 *
 *    std::vector<ThermalZone> heatingZoneFansOnlyZones() const;
 *    bool setHeatingZoneFansOnlyZones(const std::vector<ThermalZone>& thermalZones);
 *    void resetHeatingZoneFansOnlyZones();
 */

  }  // namespace detail

  AvailabilityManagerNightCycle::AvailabilityManagerNightCycle(const Model& model)
    : AvailabilityManager(AvailabilityManagerNightCycle::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerNightCycle_Impl>());

    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setApplicabilitySchedule(schedule);
    }

    setThermostatTolerance(1.0);
    setCyclingRunTime(3600);

    // Control Zone List
    ModelObjectList controlThermalZoneList = ModelObjectList(model);
    controlThermalZoneList.setName(this->name().get() + " Control Zone List");
    bool ok = setPointer(OS_AvailabilityManager_NightCycleFields::ControlZoneorZoneListName, controlThermalZoneList.handle());
    OS_ASSERT(ok);

    // Cooling Control Zone List
    ModelObjectList coolingControlThermalZoneList = ModelObjectList(model);
    coolingControlThermalZoneList.setName(this->name().get() + " Cooling Control Zone List");
    ok = setPointer(OS_AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName, coolingControlThermalZoneList.handle());
    OS_ASSERT(ok);

    // Heating Control Zone List
    ModelObjectList heatingControlThermalZoneList = ModelObjectList(model);
    heatingControlThermalZoneList.setName(this->name().get() + " Heating Control Zone List");
    ok = setPointer(OS_AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName, heatingControlThermalZoneList.handle());
    OS_ASSERT(ok);

    // Heating Zone Fans Only Zone List
    ModelObjectList heatingZoneFansOnlyThermalZoneList = ModelObjectList(model);
    heatingZoneFansOnlyThermalZoneList.setName(this->name().get() + " Heating Zone Fans Only Zone List");
    ok = setPointer(OS_AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName, heatingZoneFansOnlyThermalZoneList.handle());
    OS_ASSERT(ok);
  }

  IddObjectType AvailabilityManagerNightCycle::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_NightCycle};
  }

  std::vector<std::string> AvailabilityManagerNightCycle::controlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AvailabilityManager_NightCycleFields::ControlType);
  }

  std::string AvailabilityManagerNightCycle::controlType() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->controlType();
  }

  bool AvailabilityManagerNightCycle::isControlTypeDefaulted() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->isControlTypeDefaulted();
  }

  double AvailabilityManagerNightCycle::thermostatTolerance() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->thermostatTolerance();
  }

  bool AvailabilityManagerNightCycle::isThermostatToleranceDefaulted() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->isThermostatToleranceDefaulted();
  }

  double AvailabilityManagerNightCycle::cyclingRunTime() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->cyclingRunTime();
  }

  bool AvailabilityManagerNightCycle::isCyclingRunTimeDefaulted() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->isCyclingRunTimeDefaulted();
  }

  bool AvailabilityManagerNightCycle::setControlType(const std::string& controlType) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setControlType(controlType);
  }

  void AvailabilityManagerNightCycle::resetControlType() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetControlType();
  }

  bool AvailabilityManagerNightCycle::setThermostatTolerance(double thermostatTolerance) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setThermostatTolerance(thermostatTolerance);
  }

  void AvailabilityManagerNightCycle::resetThermostatTolerance() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetThermostatTolerance();
  }

  bool AvailabilityManagerNightCycle::setCyclingRunTime(double cyclingRunTime) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setCyclingRunTime(cyclingRunTime);
  }

  void AvailabilityManagerNightCycle::resetCyclingRunTime() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetCyclingRunTime();
  }

  std::vector<ThermalZone> AvailabilityManagerNightCycle::controlThermalZones() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->controlThermalZones();
  }
  bool AvailabilityManagerNightCycle::setControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setControlThermalZones(thermalZones);
  }
  void AvailabilityManagerNightCycle::resetControlThermalZones() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetControlThermalZones();
  }

  std::vector<ThermalZone> AvailabilityManagerNightCycle::coolingControlThermalZones() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->coolingControlThermalZones();
  }
  bool AvailabilityManagerNightCycle::setCoolingControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setCoolingControlThermalZones(thermalZones);
  }
  void AvailabilityManagerNightCycle::resetCoolingControlThermalZones() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetCoolingControlThermalZones();
  }

  std::vector<ThermalZone> AvailabilityManagerNightCycle::heatingControlThermalZones() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->heatingControlThermalZones();
  }
  bool AvailabilityManagerNightCycle::setHeatingControlThermalZones(const std::vector<ThermalZone>& thermalZones) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setHeatingControlThermalZones(thermalZones);
  }
  void AvailabilityManagerNightCycle::resetHeatingControlThermalZones() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetHeatingControlThermalZones();
  }

  std::vector<ThermalZone> AvailabilityManagerNightCycle::heatingZoneFansOnlyThermalZones() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->heatingZoneFansOnlyThermalZones();
  }
  bool AvailabilityManagerNightCycle::setHeatingZoneFansOnlyThermalZones(const std::vector<ThermalZone>& thermalZones) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setHeatingZoneFansOnlyThermalZones(thermalZones);
  }
  void AvailabilityManagerNightCycle::resetHeatingZoneFansOnlyThermalZones() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetHeatingZoneFansOnlyThermalZones();
  }

  // Cycling Run Time Control Type
  std::vector<std::string> AvailabilityManagerNightCycle::cyclingRunTimeControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AvailabilityManager_NightCycleFields::CyclingRunTimeControlType);
  }

  std::string AvailabilityManagerNightCycle::cyclingRunTimeControlType() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->cyclingRunTimeControlType();
  }

  bool AvailabilityManagerNightCycle::isCyclingRunTimeControlTypeDefaulted() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->isCyclingRunTimeControlTypeDefaulted();
  }

  bool AvailabilityManagerNightCycle::setCyclingRunTimeControlType(const std::string& cyclingRunTimeControlType) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setCyclingRunTimeControlType(cyclingRunTimeControlType);
  }

  void AvailabilityManagerNightCycle::resetCyclingRunTimeControlType() {
    getImpl<detail::AvailabilityManagerNightCycle_Impl>()->resetCyclingRunTimeControlType();
  }

  boost::optional<AirLoopHVAC> AvailabilityManagerNightCycle::airLoopHVAC() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->airLoopHVAC();
  }

  Schedule AvailabilityManagerNightCycle::applicabilitySchedule() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->applicabilitySchedule();
  }

  bool AvailabilityManagerNightCycle::setApplicabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->setApplicabilitySchedule(schedule);
  }

  boost::optional<Schedule> AvailabilityManagerNightCycle::fanSchedule() const {
    return getImpl<detail::AvailabilityManagerNightCycle_Impl>()->fanSchedule();
  }

  /// @cond
  AvailabilityManagerNightCycle::AvailabilityManagerNightCycle(std::shared_ptr<detail::AvailabilityManagerNightCycle_Impl> impl)
    : AvailabilityManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
