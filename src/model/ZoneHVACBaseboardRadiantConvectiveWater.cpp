/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilHeatingWaterBaseboardRadiant.hpp"
#include "CoilHeatingWaterBaseboardRadiant_Impl.hpp"
#include "Model.hpp"
#include "Space.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_Baseboard_RadiantConvective_Water_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType());
    }

    ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType());
    }

    ZoneHVACBaseboardRadiantConvectiveWater_Impl::ZoneHVACBaseboardRadiantConvectiveWater_Impl(
      const ZoneHVACBaseboardRadiantConvectiveWater_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACBaseboardRadiantConvectiveWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Baseboard Total Heating Rate",        "Baseboard Convective Heating Rate",
                                                   "Baseboard Radiant Heating Rate",      "Baseboard Total Heating Energy",
                                                   "Baseboard Convective Heating Energy", "Baseboard Radiant Heating Energy"};
      return result;
    }

    IddObjectType ZoneHVACBaseboardRadiantConvectiveWater_Impl::iddObjectType() const {
      return ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACBaseboardRadiantConvectiveWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACBaseboardRadiantConvectiveWater", "Availability"));
      }
      return result;
    }

    unsigned ZoneHVACBaseboardRadiantConvectiveWater_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    unsigned ZoneHVACBaseboardRadiantConvectiveWater_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveWater_Impl::thermalZone() const {
      auto thisObject = this->getObject<ModelObject>();
      auto const thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
      for (auto const& thermalZone : thermalZones) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          return thermalZone;
        }
      }
      return boost::none;
    }

    bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m || thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneHVACBaseboardRadiantConvectiveWater_Impl::removeFromThermalZone() {
      if (auto thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    std::vector<Surface> ZoneHVACBaseboardRadiantConvectiveWater_Impl::surfaces() const {

      //vector to hold all of the surfaces that this radiant system is attached to
      std::vector<Surface> surfaces;

      //get the thermal zone this equipment belongs to
      if (auto const thermalZone = this->thermalZone()) {

        //loop through all the spaces in this zone
        for (auto const& space : thermalZone->spaces()) {

          //loop through all the surfaces in this space
          for (auto const& surface : space.surfaces()) {
            surfaces.push_back(surface);
          }
        }
      }

      return surfaces;
    }

    std::vector<ModelObject> ZoneHVACBaseboardRadiantConvectiveWater_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    ModelObject ZoneHVACBaseboardRadiantConvectiveWater_Impl::clone(Model model) const {
      auto baseboardRadConvWaterClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACBaseboardRadiantConvectiveWater>();

      auto t_heatingCoil = heatingCoil();
      auto heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

      baseboardRadConvWaterClone.setHeatingCoil(heatingCoilClone);

      if (model == this->model()) {
        if (auto plant = t_heatingCoil.plantLoop()) {
          plant->addDemandBranchForComponent(heatingCoilClone);
        }
      }

      return std::move(baseboardRadConvWaterClone);
    }

    std::vector<IdfObject> ZoneHVACBaseboardRadiantConvectiveWater_Impl::remove() {
      if (auto waterHeatingCoil = heatingCoil().optionalCast<CoilHeatingWaterBaseboardRadiant>()) {
        if (boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent(waterHeatingCoil.get());
        }
      }
      return ZoneHVACComponent_Impl::remove();
    }

    Schedule ZoneHVACBaseboardRadiantConvectiveWater_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    double ZoneHVACBaseboardRadiantConvectiveWater_Impl::fractionRadiant() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionRadiant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACBaseboardRadiantConvectiveWater_Impl::fractionofRadiantEnergyIncidentonPeople() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople, true);
      OS_ASSERT(value);
      return value.get();
    }

    HVACComponent ZoneHVACBaseboardRadiantConvectiveWater_Impl::heatingCoil() const {
      boost::optional<HVACComponent> value = optionalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
      }
      return value.get();
    }

    bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName,
                                "ZoneHVACBaseboardRadiantConvectiveWater", "Availability", schedule);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setFractionRadiant(double fractionRadiant) {
      bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionRadiant, fractionRadiant);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
      bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople,
                              fractionofRadiantEnergyIncidentonPeople);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveWater_Impl::setHeatingCoil(const HVACComponent& radBaseboardHeatingCoil) {
      bool result = setPointer(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingCoilName, radBaseboardHeatingCoil.handle());
      return result;
    }

    boost::optional<Schedule> ZoneHVACBaseboardRadiantConvectiveWater_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName);
    }

    boost::optional<HVACComponent> ZoneHVACBaseboardRadiantConvectiveWater_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingCoilName);
    }

    std::vector<EMSActuatorNames> ZoneHVACBaseboardRadiantConvectiveWater_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneBaseboard:OutdoorTemperatureControlled", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACBaseboardRadiantConvectiveWater_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Simple Zone Baseboard Capacity At Low Temperature", "Simple Zone Baseboard Capacity At High Temperature"};
      return types;
    }

    ComponentType ZoneHVACBaseboardRadiantConvectiveWater_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> ZoneHVACBaseboardRadiantConvectiveWater_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ZoneHVACBaseboardRadiantConvectiveWater_Impl::heatingFuelTypes() const {
      return heatingCoil().heatingFuelTypes();
    }

    std::vector<AppGFuelType> ZoneHVACBaseboardRadiantConvectiveWater_Impl::appGHeatingFuelTypes() const {
      return heatingCoil().appGHeatingFuelTypes();
    }

  }  // namespace detail

  ZoneHVACBaseboardRadiantConvectiveWater::ZoneHVACBaseboardRadiantConvectiveWater(const Model& model)
    : ZoneHVACComponent(ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>());

    bool ok = true;
    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);
    ok = setFractionRadiant(0.3);
    OS_ASSERT(ok);
    ok = setFractionofRadiantEnergyIncidentonPeople(0.3);
    OS_ASSERT(ok);
    CoilHeatingWaterBaseboardRadiant coil(model);
    ok = setHeatingCoil(coil);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACBaseboardRadiantConvectiveWater::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water};
  }

  Schedule ZoneHVACBaseboardRadiantConvectiveWater::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->availabilitySchedule();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater::fractionRadiant() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->fractionRadiant();
  }

  double ZoneHVACBaseboardRadiantConvectiveWater::fractionofRadiantEnergyIncidentonPeople() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->fractionofRadiantEnergyIncidentonPeople();
  }

  HVACComponent ZoneHVACBaseboardRadiantConvectiveWater::heatingCoil() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->heatingCoil();
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater::setFractionRadiant(double fractionRadiant) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setFractionRadiant(fractionRadiant);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setFractionofRadiantEnergyIncidentonPeople(
      fractionofRadiantEnergyIncidentonPeople);
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater::setHeatingCoil(const HVACComponent& radBaseboardHeatingCoil) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->setHeatingCoil(radBaseboardHeatingCoil);
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveWater::thermalZone() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->thermalZone();
  }

  bool ZoneHVACBaseboardRadiantConvectiveWater::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACBaseboardRadiantConvectiveWater::removeFromThermalZone() {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->removeFromThermalZone();
  }

  /// @cond
  ZoneHVACBaseboardRadiantConvectiveWater::ZoneHVACBaseboardRadiantConvectiveWater(
    std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
