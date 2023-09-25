/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACBaseboardConvectiveWater.hpp"
#include "ZoneHVACBaseboardConvectiveWater_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "CoilHeatingWaterBaseboard.hpp"
#include "CoilHeatingWaterBaseboard_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_ZoneHVAC_Baseboard_Convective_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardConvectiveWater::iddObjectType());
    }

    ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardConvectiveWater::iddObjectType());
    }

    ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const ZoneHVACBaseboardConvectiveWater_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    ModelObject ZoneHVACBaseboardConvectiveWater_Impl::clone(Model model) const {
      auto baseboardConvWaterClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACBaseboardConvectiveWater>();

      auto t_heatingCoil = heatingCoil();
      auto heatingCoilClone = t_heatingCoil.clone(model).cast<StraightComponent>();

      baseboardConvWaterClone.setHeatingCoil(heatingCoilClone);

      if (model == this->model()) {
        if (auto plant = t_heatingCoil.plantLoop()) {
          plant->addDemandBranchForComponent(heatingCoilClone);
        }
      }

      return std::move(baseboardConvWaterClone);
    }

    std::vector<IdfObject> ZoneHVACBaseboardConvectiveWater_Impl::remove() {
      if (boost::optional<CoilHeatingWaterBaseboard> waterHeatingCoil = heatingCoil().optionalCast<CoilHeatingWaterBaseboard>()) {
        if (boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent(waterHeatingCoil.get());
        }
      }
      return ZoneHVACComponent_Impl::remove();
    }

    const std::vector<std::string>& ZoneHVACBaseboardConvectiveWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Baseboard Total Heating Rate",       "Baseboard Total Heating Energy",    "Baseboard Hot Water Energy",
        "Baseboard Hot Water Mass Flow Rate", "Baseboard Air Mass Flow Rate",      "Baseboard Air Inlet Temperature",
        "Baseboard Air Outlet Temperature",   "Baseboard Water Inlet Temperature", "Baseboard Water Outlet Temperature"};
      return result;
    }

    IddObjectType ZoneHVACBaseboardConvectiveWater_Impl::iddObjectType() const {
      return ZoneHVACBaseboardConvectiveWater::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACBaseboardConvectiveWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACBaseboardConvectiveWater", "Availability"));
      }
      return result;
    }
    unsigned ZoneHVACBaseboardConvectiveWater_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    std::vector<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalStraightComponent intermediate = optionalHeatingCoil()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    unsigned ZoneHVACBaseboardConvectiveWater_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }
    Schedule ZoneHVACBaseboardConvectiveWater_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    StraightComponent ZoneHVACBaseboardConvectiveWater_Impl::heatingCoil() const {
      boost::optional<StraightComponent> value = optionalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
      }
      return value.get();
    }

    bool ZoneHVACBaseboardConvectiveWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName, "ZoneHVACBaseboardConvectiveWater",
                                "Availability", schedule);
      return result;
    }

    bool ZoneHVACBaseboardConvectiveWater_Impl::setHeatingCoil(const StraightComponent& baseboardHeatingCoil) {
      bool result = setPointer(OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName, baseboardHeatingCoil.handle());
      return result;
    }

    boost::optional<Schedule> ZoneHVACBaseboardConvectiveWater_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName);
    }

    boost::optional<StraightComponent> ZoneHVACBaseboardConvectiveWater_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName);
    }

    boost::optional<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    boost::optional<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::heatingCoilAsModelObject() const {
      OptionalModelObject result = heatingCoil();
      return result;
    }

    bool ZoneHVACBaseboardConvectiveWater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    bool ZoneHVACBaseboardConvectiveWater_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalStraightComponent intermediate = modelObject->optionalCast<StraightComponent>();
        if (intermediate) {
          StraightComponent baseboardHeatingCoil(*intermediate);
          return setHeatingCoil(baseboardHeatingCoil);
        }
      }
      return false;
    }

    boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveWater_Impl::thermalZone() const {
      auto thisObject = this->getObject<ModelObject>();
      std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
      for (const auto& thermalZone : thermalZones) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          return thermalZone;
        }
      }
      return boost::none;
    }

    //reimplemented to override the base-class method in ZoneHVACComponent
    //because this component doesn't get attached to the zone inlet and zone outlet nodes
    bool ZoneHVACBaseboardConvectiveWater_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m) {
        return false;
      }

      if (thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    //reimplemented to override the base-class method in ZoneHVACComponent
    //because this component doesn't get attached to the zone inlet and zone outlet nodes
    //and therefore doesn't need to be removed from them when removed from the zone
    void ZoneHVACBaseboardConvectiveWater_Impl::removeFromThermalZone() {
      if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    std::vector<EMSActuatorNames> ZoneHVACBaseboardConvectiveWater_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneBaseboard:OutdoorTemperatureControlled", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACBaseboardConvectiveWater_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Simple Zone Baseboard Capacity At Low Temperature", "Simple Zone Baseboard Capacity At High Temperature"};
      return types;
    }

    ComponentType ZoneHVACBaseboardConvectiveWater_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> ZoneHVACBaseboardConvectiveWater_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ZoneHVACBaseboardConvectiveWater_Impl::heatingFuelTypes() const {
      return heatingCoil().heatingFuelTypes();
    }

    std::vector<AppGFuelType> ZoneHVACBaseboardConvectiveWater_Impl::appGHeatingFuelTypes() const {
      return heatingCoil().appGHeatingFuelTypes();
    }

  }  // namespace detail

  ZoneHVACBaseboardConvectiveWater::ZoneHVACBaseboardConvectiveWater(const Model& model, Schedule& availabilitySchedule,
                                                                     StraightComponent& heatingCoilBaseboard)
    : ZoneHVACComponent(ZoneHVACBaseboardConvectiveWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>());

    //     OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName
    //     OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName
    bool ok = setAvailabilitySchedule(availabilitySchedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << availabilitySchedule.briefDescription() << ".");
    }
    ok = setHeatingCoil(heatingCoilBaseboard);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACBaseboardConvectiveWater::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water};
  }

  Schedule ZoneHVACBaseboardConvectiveWater::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->availabilitySchedule();
  }

  StraightComponent ZoneHVACBaseboardConvectiveWater::heatingCoil() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->heatingCoil();
  }

  bool ZoneHVACBaseboardConvectiveWater::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACBaseboardConvectiveWater::setHeatingCoil(const StraightComponent& baseboardHeatingCoil) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->setHeatingCoil(baseboardHeatingCoil);
  }
  boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveWater::thermalZone() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->thermalZone();
  }

  bool ZoneHVACBaseboardConvectiveWater::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACBaseboardConvectiveWater::removeFromThermalZone() {
    return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->removeFromThermalZone();
  }
  /// @cond
  ZoneHVACBaseboardConvectiveWater::ZoneHVACBaseboardConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
