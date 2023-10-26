/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneMixing.hpp"
#include "ZoneMixing_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneMixing_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneMixing_Impl::ZoneMixing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneMixing::iddObjectType());
    }

    ZoneMixing_Impl::ZoneMixing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneMixing::iddObjectType());
    }

    ZoneMixing_Impl::ZoneMixing_Impl(const ZoneMixing_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneMixing_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Zone Mixing Volume",
        "Zone Mixing Current Density Air Volume Flow Rate",
        "Zone Mixing Standard Density Air Volume Flow Rate",
        "Zone Mixing Mass",
        "Zone Mixing Mass Flow Rate",
        "Zone Mixing Sensible Heat Loss Energy",
        "Zone Mixing Sensible Heat Gain Energy",
        "Zone Mixing Latent Heat Loss Energy",
        "Zone Mixing Latent Heat Gain Energy",
        "Zone Mixing Total Heat Loss Energy",
        "Zone Mixing Total Heat Gain Energy"
        // DLM: these are not working yet
        // https://github.com/NREL/EnergyPlus/issues/5042
        //"Zone Mixing Receiving Air Mass Flow Rate",
        //"Zone Mixing Source Air Mass Flow Rate",
      };
      return result;
    }

    IddObjectType ZoneMixing_Impl::iddObjectType() const {
      return ZoneMixing::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneMixing_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneMixingFields::ScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Zone Mixing");
      }
      if (std::find(b, e, OS_ZoneMixingFields::DeltaTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Delta Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MinimumReceivingTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Minimum Receiving Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MaximumReceivingTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Maximum Receiving Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MinimumSourceTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Minimum Source Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MaximumSourceTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Maximum Source Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Minimum Outdoor Temperature");
      }
      if (std::find(b, e, OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName) != e) {
        result.emplace_back("ZoneMixing", "Maximum Outdoor Temperature");
      }
      return result;
    }

    bool ZoneMixing_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (auto mo_ = newParent.optionalCast<Space>()) {
        result = this->setSpace(mo_.get());
        // } else if (auto mo_ = newParent.optionalCast<ThermalZone>()) {
        //   result = this->setZone(mo_.get());
      }
      return result;
    }

    ModelObject ZoneMixing_Impl::zoneOrSpace() const {
      boost::optional<ModelObject> value = getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_ZoneMixingFields::ZoneorSpaceName);
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a receiving ThermalZone Or Space.");
      }
      return value.get();
    }

    boost::optional<ThermalZone> ZoneMixing_Impl::zone() const {
      return zoneOrSpace().optionalCast<ThermalZone>();
    }

    boost::optional<Space> ZoneMixing_Impl::space() const {
      return zoneOrSpace().optionalCast<Space>();
    }

    bool ZoneMixing_Impl::setZone(const ThermalZone& zone) {
      return setPointer(OS_ZoneMixingFields::ZoneorSpaceName, zone.handle());
    }

    bool ZoneMixing_Impl::setSpace(const Space& space) {
      return setPointer(OS_ZoneMixingFields::ZoneorSpaceName, space.handle());
    }

    boost::optional<ModelObject> ZoneMixing_Impl::sourceZoneOrSpace() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_ZoneMixingFields::SourceZoneorSpaceName);
    }

    boost::optional<ThermalZone> ZoneMixing_Impl::sourceZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneMixingFields::SourceZoneorSpaceName);
    }

    boost::optional<Space> ZoneMixing_Impl::sourceSpace() const {
      return getObject<ModelObject>().getModelObjectTarget<Space>(OS_ZoneMixingFields::SourceZoneorSpaceName);
    }

    bool ZoneMixing_Impl::setSourceZone(const ThermalZone& zone) {

      auto receiving = this->zoneOrSpace();
      // source zone cannot be the same as this zone
      if (zone.handle() == receiving.handle()) {
        LOG(Warn, "For " << briefDescription() << ", Source Zone cannot be the same as the Receiving Zone '" << zone.nameString() << "'.");
        return false;
      }
      if (receiving.iddObjectType() != IddObjectType::OS_ThermalZone) {
        LOG(Warn, "For " << briefDescription() << ", Receiving is a Space, so you cannot set Source as a Zone.");
        return false;
      }

      return setPointer(OS_ZoneMixingFields::SourceZoneorSpaceName, zone.handle());
    }

    bool ZoneMixing_Impl::setSourceSpace(const Space& space) {
      auto receiving = this->zoneOrSpace();
      // source zone cannot be the same as this zone
      if (space.handle() == receiving.handle()) {
        LOG(Warn, "For " << briefDescription() << ", Source Space cannot be the same as the Receiving Space '" << space.nameString() << "'.");
        return false;
      }
      if (receiving.iddObjectType() != IddObjectType::OS_Space) {
        LOG(Warn, "For " << briefDescription() << ", Receiving is a Zone, so you cannot set Source as a Space.");
        return false;
      }

      return setPointer(OS_ZoneMixingFields::SourceZoneorSpaceName, space.handle());
    }

    void ZoneMixing_Impl::resetSourceZoneOrSpace() {
      bool result = setString(OS_ZoneMixingFields::SourceZoneorSpaceName, "");
      OS_ASSERT(result);
    }

    Schedule ZoneMixing_Impl::schedule() const {
      boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::ScheduleName);
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
      }
      return value.get();
    }

    std::string ZoneMixing_Impl::designFlowRateCalculationMethod() const {
      boost::optional<std::string> value = getString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> ZoneMixing_Impl::designFlowRate() const {
      return getDouble(OS_ZoneMixingFields::DesignFlowRate, true);
    }

    boost::optional<double> ZoneMixing_Impl::flowRateperFloorArea() const {
      return getDouble(OS_ZoneMixingFields::FlowRateperFloorArea, true);
    }

    boost::optional<double> ZoneMixing_Impl::flowRateperPerson() const {
      return getDouble(OS_ZoneMixingFields::FlowRateperPerson, true);
    }

    boost::optional<double> ZoneMixing_Impl::airChangesperHour() const {
      return getDouble(OS_ZoneMixingFields::AirChangesperHour, true);
    }

    boost::optional<double> ZoneMixing_Impl::deltaTemperature() const {
      return getDouble(OS_ZoneMixingFields::DeltaTemperature);
    }

    boost::optional<Schedule> ZoneMixing_Impl::deltaTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::DeltaTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::minimumReceivingTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumReceivingTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::maximumReceivingTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumReceivingTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::minimumSourceTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumSourceTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::maximumSourceTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumSourceTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::minimumOutdoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName);
    }

    boost::optional<Schedule> ZoneMixing_Impl::maximumOutdoorTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName);
    }

    bool ZoneMixing_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::ScheduleName, "ZoneMixing", "Zone Mixing", schedule);
      return result;
    }

    bool ZoneMixing_Impl::setDesignFlowRate(double designFlowRate) {
      bool result(false);
      result = setDouble(OS_ZoneMixingFields::DesignFlowRate, designFlowRate);
      if (result) {
        result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Zone");
        OS_ASSERT(result);
        //result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
        //OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
      return result;
    }

    bool ZoneMixing_Impl::setFlowRateperFloorArea(double flowRateperFloorArea) {
      bool result(false);
      result = setDouble(OS_ZoneMixingFields::FlowRateperFloorArea, flowRateperFloorArea);
      if (result) {
        result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Area");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
        OS_ASSERT(result);
        //result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
        //OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
      return result;
    }

    bool ZoneMixing_Impl::setFlowRateperPerson(double flowRateperPerson) {
      bool result(false);
      result = setDouble(OS_ZoneMixingFields::FlowRateperPerson, flowRateperPerson);
      if (result) {
        result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Person");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperFloorArea, "");
        OS_ASSERT(result);
        //result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
        //OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
      return result;
    }

    bool ZoneMixing_Impl::setAirChangesperHour(double airChangesperHour) {
      bool result(false);
      result = setDouble(OS_ZoneMixingFields::AirChangesperHour, airChangesperHour);
      if (result) {
        result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
        OS_ASSERT(result);
        //result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
        //OS_ASSERT(result);
      }
      return result;
    }

    bool ZoneMixing_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }

      if (this->designFlowRate()) {
        return true;
      }

      if (boost::optional<double> flowRateperFloorArea = this->flowRateperFloorArea()) {
        return this->setDesignFlowRate(*flowRateperFloorArea * space->floorArea());
      }

      if (boost::optional<double> flowRateperPerson = this->flowRateperPerson()) {
        return this->setDesignFlowRate(*flowRateperPerson * space->numberOfPeople());
      }

      if (boost::optional<double> airChangesperHour = this->airChangesperHour()) {
        return this->setDesignFlowRate(*airChangesperHour * space->volume() / 3600.0);
      }

      return false;
    }

    bool ZoneMixing_Impl::setDeltaTemperature(double deltaTemperature) {
      bool result = setDouble(OS_ZoneMixingFields::DeltaTemperature, deltaTemperature);
      OS_ASSERT(result);
      resetDeltaTemperatureSchedule();
      return result;
    }

    void ZoneMixing_Impl::resetDeltaTemperature() {
      bool result = setString(OS_ZoneMixingFields::DeltaTemperature, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setDeltaTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::DeltaTemperatureScheduleName, "ZoneMixing", "Delta Temperature", schedule);
      if (result) {
        resetDeltaTemperature();
      }
      return result;
    }

    void ZoneMixing_Impl::resetDeltaTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::DeltaTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMinimumReceivingTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumReceivingTemperatureScheduleName, "ZoneMixing",
                                                  "Minimum Receiving Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMinimumReceivingTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MinimumReceivingTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMaximumReceivingTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumReceivingTemperatureScheduleName, "ZoneMixing",
                                                  "Maximum Receiving Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMaximumReceivingTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MaximumReceivingTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMinimumSourceTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumSourceTemperatureScheduleName, "ZoneMixing",
                                                  "Minimum Source Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMinimumSourceTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MinimumSourceTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMaximumSourceTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumSourceTemperatureScheduleName, "ZoneMixing",
                                                  "Maximum Source Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMaximumSourceTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MaximumSourceTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, "ZoneMixing",
                                                  "Minimum Outdoor Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMinimumOutdoorTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ZoneMixing_Impl::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, "ZoneMixing",
                                                  "Maximum Outdoor Temperature", schedule);
      return result;
    }

    void ZoneMixing_Impl::resetMaximumOutdoorTemperatureSchedule() {
      bool result = setString(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    std::vector<EMSActuatorNames> ZoneMixing_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneMixing", "Air Exchange Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> ZoneMixing_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  ZoneMixing::ZoneMixing(const ThermalZone& zone) : ModelObject(ZoneMixing::iddObjectType(), zone.model()) {
    OS_ASSERT(getImpl<detail::ZoneMixing_Impl>());

    bool ok = setPointer(OS_ZoneMixingFields::ZoneorSpaceName, zone.handle());
    OS_ASSERT(ok);

    ok = setPointer(OS_ZoneMixingFields::ScheduleName, zone.model().alwaysOnContinuousSchedule().handle());
    OS_ASSERT(ok);

    ok = setDesignFlowRate(0.0);
    OS_ASSERT(ok);
  }

  ZoneMixing::ZoneMixing(const Space& space) : ModelObject(ZoneMixing::iddObjectType(), space.model()) {
    OS_ASSERT(getImpl<detail::ZoneMixing_Impl>());

    bool ok = setPointer(OS_ZoneMixingFields::ZoneorSpaceName, space.handle());
    OS_ASSERT(ok);

    ok = setPointer(OS_ZoneMixingFields::ScheduleName, space.model().alwaysOnContinuousSchedule().handle());
    OS_ASSERT(ok);

    ok = setDesignFlowRate(0.0);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneMixing::iddObjectType() {
    return {IddObjectType::OS_ZoneMixing};
  }

  bool ZoneMixing::hardSize() {
    return getImpl<detail::ZoneMixing_Impl>()->hardSize();
  }

  boost::optional<ThermalZone> ZoneMixing::zone() const {
    return getImpl<detail::ZoneMixing_Impl>()->zone();
  }

  boost::optional<Space> ZoneMixing::space() const {
    return getImpl<detail::ZoneMixing_Impl>()->space();
  }

  ModelObject ZoneMixing::zoneOrSpace() const {
    return getImpl<detail::ZoneMixing_Impl>()->zoneOrSpace();
  }

  boost::optional<ThermalZone> ZoneMixing::sourceZone() const {
    return getImpl<detail::ZoneMixing_Impl>()->sourceZone();
  }

  boost::optional<Space> ZoneMixing::sourceSpace() const {
    return getImpl<detail::ZoneMixing_Impl>()->sourceSpace();
  }

  boost::optional<ModelObject> ZoneMixing::sourceZoneOrSpace() const {
    return getImpl<detail::ZoneMixing_Impl>()->sourceZoneOrSpace();
  }

  bool ZoneMixing::setSourceZone(const ThermalZone& zone) {
    return getImpl<detail::ZoneMixing_Impl>()->setSourceZone(zone);
  }

  bool ZoneMixing::setSourceSpace(const Space& space) {
    return getImpl<detail::ZoneMixing_Impl>()->setSourceSpace(space);
  }

  void ZoneMixing::resetSourceZoneOrSpace() {
    return getImpl<detail::ZoneMixing_Impl>()->resetSourceZoneOrSpace();
  }

  Schedule ZoneMixing::schedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->schedule();
  }

  std::string ZoneMixing::designFlowRateCalculationMethod() const {
    return getImpl<detail::ZoneMixing_Impl>()->designFlowRateCalculationMethod();
  }

  boost::optional<double> ZoneMixing::designFlowRate() const {
    return getImpl<detail::ZoneMixing_Impl>()->designFlowRate();
  }

  boost::optional<double> ZoneMixing::flowRateperFloorArea() const {
    return getImpl<detail::ZoneMixing_Impl>()->flowRateperFloorArea();
  }

  boost::optional<double> ZoneMixing::flowRateperPerson() const {
    return getImpl<detail::ZoneMixing_Impl>()->flowRateperPerson();
  }

  boost::optional<double> ZoneMixing::airChangesperHour() const {
    return getImpl<detail::ZoneMixing_Impl>()->airChangesperHour();
  }

  boost::optional<double> ZoneMixing::deltaTemperature() const {
    return getImpl<detail::ZoneMixing_Impl>()->deltaTemperature();
  }

  boost::optional<Schedule> ZoneMixing::deltaTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->deltaTemperatureSchedule();
  }

  bool ZoneMixing::setSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setSchedule(schedule);
  }

  bool ZoneMixing::setDesignFlowRate(double designFlowRate) {
    return getImpl<detail::ZoneMixing_Impl>()->setDesignFlowRate(designFlowRate);
  }

  bool ZoneMixing::setFlowRateperFloorArea(double flowRateperFloorArea) {
    return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperFloorArea(flowRateperFloorArea);
  }

  bool ZoneMixing::setFlowRateperPerson(double flowRateperPerson) {
    return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperPerson(flowRateperPerson);
  }

  bool ZoneMixing::setAirChangesperHour(double airChangesperHour) {
    return getImpl<detail::ZoneMixing_Impl>()->setAirChangesperHour(airChangesperHour);
  }

  bool ZoneMixing::setDeltaTemperature(double deltaTemperature) {
    return getImpl<detail::ZoneMixing_Impl>()->setDeltaTemperature(deltaTemperature);
  }

  void ZoneMixing::resetDeltaTemperature() {
    getImpl<detail::ZoneMixing_Impl>()->resetDeltaTemperature();
  }

  bool ZoneMixing::setDeltaTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setDeltaTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetDeltaTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetDeltaTemperatureSchedule();
  }

  // ReceivingTemperatureSchedule

  boost::optional<Schedule> ZoneMixing::minimumReceivingTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->minimumReceivingTemperatureSchedule();
  }

  bool ZoneMixing::setMinimumReceivingTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMinimumReceivingTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMinimumReceivingTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMinimumReceivingTemperatureSchedule();
  }

  boost::optional<Schedule> ZoneMixing::maximumReceivingTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->maximumReceivingTemperatureSchedule();
  }

  bool ZoneMixing::setMaximumReceivingTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMaximumReceivingTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMaximumReceivingTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMaximumReceivingTemperatureSchedule();
  }

  // SourceTemperatureSchedule

  boost::optional<Schedule> ZoneMixing::minimumSourceTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->minimumSourceTemperatureSchedule();
  }

  bool ZoneMixing::setMinimumSourceTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMinimumSourceTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMinimumSourceTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMinimumSourceTemperatureSchedule();
  }

  boost::optional<Schedule> ZoneMixing::maximumSourceTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->maximumSourceTemperatureSchedule();
  }

  bool ZoneMixing::setMaximumSourceTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMaximumSourceTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMaximumSourceTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMaximumSourceTemperatureSchedule();
  }

  // Outdoor Temperature Schedule

  boost::optional<Schedule> ZoneMixing::minimumOutdoorTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->minimumOutdoorTemperatureSchedule();
  }

  bool ZoneMixing::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMinimumOutdoorTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMinimumOutdoorTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMinimumOutdoorTemperatureSchedule();
  }

  boost::optional<Schedule> ZoneMixing::maximumOutdoorTemperatureSchedule() const {
    return getImpl<detail::ZoneMixing_Impl>()->maximumOutdoorTemperatureSchedule();
  }

  bool ZoneMixing::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneMixing_Impl>()->setMaximumOutdoorTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMaximumOutdoorTemperatureSchedule() {
    getImpl<detail::ZoneMixing_Impl>()->resetMaximumOutdoorTemperatureSchedule();
  }

  /// @cond
  ZoneMixing::ZoneMixing(std::shared_ptr<detail::ZoneMixing_Impl> impl) : ModelObject(std::move(impl)) {}

  // DEPRECATED

  void ZoneMixing::resetSourceZone() {
    DEPRECATED_AT_MSG(3, 5, 0, "Use resetSourceZoneOrSpace instead.");
    getImpl<detail::ZoneMixing_Impl>()->resetSourceZoneOrSpace();
  }

  boost::optional<double> ZoneMixing::flowRateperZoneFloorArea() const {
    DEPRECATED_AT_MSG(3, 5, 0, "Use flowRateperFloorArea instead.");
    return getImpl<detail::ZoneMixing_Impl>()->flowRateperFloorArea();
  }

  bool ZoneMixing::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
    DEPRECATED_AT_MSG(3, 5, 0, "Use setFlowRateperFloorArea instead.");
    return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperFloorArea(flowRateperZoneFloorArea);
  }

  // ReceivingTemperatureSchedule
  boost::optional<Schedule> ZoneMixing::minimumZoneTemperatureSchedule() const {
    DEPRECATED_AT_MSG(3, 5, 0, "Use minimumReceivingTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->minimumReceivingTemperatureSchedule();
  }

  bool ZoneMixing::setMinimumZoneTemperatureSchedule(Schedule& schedule) {
    DEPRECATED_AT_MSG(3, 5, 0, "Use setMinimumReceivingTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->setMinimumReceivingTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMinimumZoneTemperatureSchedule() {
    DEPRECATED_AT_MSG(3, 5, 0, "Use resetMinimumReceivingTemperatureSchedule instead.");
    getImpl<detail::ZoneMixing_Impl>()->resetMinimumReceivingTemperatureSchedule();
  }

  boost::optional<Schedule> ZoneMixing::maximumZoneTemperatureSchedule() const {
    DEPRECATED_AT_MSG(3, 5, 0, "Use maximumReceivingTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->maximumReceivingTemperatureSchedule();
  }

  bool ZoneMixing::setMaximumZoneTemperatureSchedule(Schedule& schedule) {
    DEPRECATED_AT_MSG(3, 5, 0, "Use setMaximumReceivingTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->setMaximumReceivingTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMaximumZoneTemperatureSchedule() {
    DEPRECATED_AT_MSG(3, 5, 0, "Use resetMaximumReceivingTemperatureSchedule instead.");
    getImpl<detail::ZoneMixing_Impl>()->resetMaximumReceivingTemperatureSchedule();
  }

  // SourceTemperatureSchedule
  boost::optional<Schedule> ZoneMixing::minimumSourceZoneTemperatureSchedule() const {
    DEPRECATED_AT_MSG(3, 5, 0, "Use minimumSourceTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->minimumSourceTemperatureSchedule();
  }

  bool ZoneMixing::setMinimumSourceZoneTemperatureSchedule(Schedule& schedule) {
    DEPRECATED_AT_MSG(3, 5, 0, "Use setMinimumSourceTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->setMinimumSourceTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMinimumSourceZoneTemperatureSchedule() {
    DEPRECATED_AT_MSG(3, 5, 0, "Use resetMinimumSourceTemperatureSchedule instead.");
    getImpl<detail::ZoneMixing_Impl>()->resetMinimumSourceTemperatureSchedule();
  }

  boost::optional<Schedule> ZoneMixing::maximumSourceZoneTemperatureSchedule() const {
    DEPRECATED_AT_MSG(3, 5, 0, "Use maximumSourceTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->maximumSourceTemperatureSchedule();
  }

  bool ZoneMixing::setMaximumSourceZoneTemperatureSchedule(Schedule& schedule) {
    DEPRECATED_AT_MSG(3, 5, 0, "Use setMaximumSourceTemperatureSchedule instead.");
    return getImpl<detail::ZoneMixing_Impl>()->setMaximumSourceTemperatureSchedule(schedule);
  }

  void ZoneMixing::resetMaximumSourceZoneTemperatureSchedule() {
    DEPRECATED_AT_MSG(3, 5, 0, "Use resetMaximumSourceTemperatureSchedule instead.");
    getImpl<detail::ZoneMixing_Impl>()->resetMaximumSourceTemperatureSchedule();
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
