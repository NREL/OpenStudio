/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACBaseboardRadiantConvectiveSteam.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveSteam_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilHeatingSteamBaseboardRadiant.hpp"
#include "CoilHeatingSteamBaseboardRadiant_Impl.hpp"
#include "Model.hpp"
#include "Space.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_Baseboard_RadiantConvective_Steam_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACBaseboardRadiantConvectiveSteam_Impl::ZoneHVACBaseboardRadiantConvectiveSteam_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveSteam::iddObjectType());
    }

    ZoneHVACBaseboardRadiantConvectiveSteam_Impl::ZoneHVACBaseboardRadiantConvectiveSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveSteam::iddObjectType());
    }

    ZoneHVACBaseboardRadiantConvectiveSteam_Impl::ZoneHVACBaseboardRadiantConvectiveSteam_Impl(
      const ZoneHVACBaseboardRadiantConvectiveSteam_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACBaseboardRadiantConvectiveSteam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Baseboard Total Heating Rate",        "Baseboard Convective Heating Rate",
                                                   "Baseboard Radiant Heating Rate",      "Baseboard Total Heating Energy",
                                                   "Baseboard Convective Heating Energy", "Baseboard Radiant Heating Energy"};
      return result;
    }

    IddObjectType ZoneHVACBaseboardRadiantConvectiveSteam_Impl::iddObjectType() const {
      return ZoneHVACBaseboardRadiantConvectiveSteam::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACBaseboardRadiantConvectiveSteam", "Availability"));
      }
      return result;
    }

    unsigned ZoneHVACBaseboardRadiantConvectiveSteam_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    unsigned ZoneHVACBaseboardRadiantConvectiveSteam_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::thermalZone() const {
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

    bool ZoneHVACBaseboardRadiantConvectiveSteam_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m || thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneHVACBaseboardRadiantConvectiveSteam_Impl::removeFromThermalZone() {
      if (auto thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    std::vector<Surface> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::surfaces() const {

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

    std::vector<ModelObject> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    ModelObject ZoneHVACBaseboardRadiantConvectiveSteam_Impl::clone(Model model) const {
      auto baseboardRadConvSteamClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACBaseboardRadiantConvectiveSteam>();

      auto t_heatingCoil = heatingCoil();
      auto heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

      baseboardRadConvSteamClone.setHeatingCoil(heatingCoilClone);

      if (model == this->model()) {
        if (auto plant = t_heatingCoil.plantLoop()) {
          plant->addDemandBranchForComponent(heatingCoilClone);
        }
      }

      return std::move(baseboardRadConvSteamClone);
    }

    std::vector<IdfObject> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::remove() {
      if (auto waterHeatingCoil = heatingCoil().optionalCast<CoilHeatingSteamBaseboardRadiant>()) {
        if (boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent(waterHeatingCoil.get());
        }
      }
      return ZoneHVACComponent_Impl::remove();
    }

    Schedule ZoneHVACBaseboardRadiantConvectiveSteam_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    double ZoneHVACBaseboardRadiantConvectiveSteam_Impl::fractionRadiant() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::FractionRadiant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACBaseboardRadiantConvectiveSteam_Impl::fractionofRadiantEnergyIncidentonPeople() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::FractionofRadiantEnergyIncidentonPeople, true);
      OS_ASSERT(value);
      return value.get();
    }

    HVACComponent ZoneHVACBaseboardRadiantConvectiveSteam_Impl::heatingCoil() const {
      boost::optional<HVACComponent> value = optionalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
      }
      return value.get();
    }

    bool ZoneHVACBaseboardRadiantConvectiveSteam_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::AvailabilityScheduleName,
                                "ZoneHVACBaseboardRadiantConvectiveSteam", "Availability", schedule);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveSteam_Impl::setFractionRadiant(double fractionRadiant) {
      bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::FractionRadiant, fractionRadiant);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveSteam_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
      bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::FractionofRadiantEnergyIncidentonPeople,
                              fractionofRadiantEnergyIncidentonPeople);
      return result;
    }

    bool ZoneHVACBaseboardRadiantConvectiveSteam_Impl::setHeatingCoil(const HVACComponent& radBaseboardHeatingCoil) {
      bool result = setPointer(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::HeatingCoilName, radBaseboardHeatingCoil.handle());
      return result;
    }

    boost::optional<Schedule> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::AvailabilityScheduleName);
    }

    boost::optional<HVACComponent> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_Baseboard_RadiantConvective_SteamFields::HeatingCoilName);
    }

    std::vector<EMSActuatorNames> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneBaseboard:OutdoorTemperatureControlled", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Simple Zone Baseboard Capacity At Low Temperature", "Simple Zone Baseboard Capacity At High Temperature"};
      return types;
    }

    ComponentType ZoneHVACBaseboardRadiantConvectiveSteam_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::heatingFuelTypes() const {
      return heatingCoil().heatingFuelTypes();
    }

    std::vector<AppGFuelType> ZoneHVACBaseboardRadiantConvectiveSteam_Impl::appGHeatingFuelTypes() const {
      return heatingCoil().appGHeatingFuelTypes();
    }

  }  // namespace detail

  ZoneHVACBaseboardRadiantConvectiveSteam::ZoneHVACBaseboardRadiantConvectiveSteam(const Model& model)
    : ZoneHVACComponent(ZoneHVACBaseboardRadiantConvectiveSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>());

    bool ok = true;
    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);
    ok = setFractionRadiant(0.3);
    OS_ASSERT(ok);
    ok = setFractionofRadiantEnergyIncidentonPeople(0.3);
    OS_ASSERT(ok);
    CoilHeatingSteamBaseboardRadiant coil(model);
    ok = setHeatingCoil(coil);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACBaseboardRadiantConvectiveSteam::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Steam};
  }

  Schedule ZoneHVACBaseboardRadiantConvectiveSteam::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->availabilitySchedule();
  }

  double ZoneHVACBaseboardRadiantConvectiveSteam::fractionRadiant() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->fractionRadiant();
  }

  double ZoneHVACBaseboardRadiantConvectiveSteam::fractionofRadiantEnergyIncidentonPeople() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->fractionofRadiantEnergyIncidentonPeople();
  }

  HVACComponent ZoneHVACBaseboardRadiantConvectiveSteam::heatingCoil() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->heatingCoil();
  }

  bool ZoneHVACBaseboardRadiantConvectiveSteam::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACBaseboardRadiantConvectiveSteam::setFractionRadiant(double fractionRadiant) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->setFractionRadiant(fractionRadiant);
  }

  bool ZoneHVACBaseboardRadiantConvectiveSteam::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->setFractionofRadiantEnergyIncidentonPeople(
      fractionofRadiantEnergyIncidentonPeople);
  }

  bool ZoneHVACBaseboardRadiantConvectiveSteam::setHeatingCoil(const HVACComponent& radBaseboardHeatingCoil) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->setHeatingCoil(radBaseboardHeatingCoil);
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveSteam::thermalZone() const {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->thermalZone();
  }

  bool ZoneHVACBaseboardRadiantConvectiveSteam::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACBaseboardRadiantConvectiveSteam::removeFromThermalZone() {
    return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl>()->removeFromThermalZone();
  }

  /// @cond
  ZoneHVACBaseboardRadiantConvectiveSteam::ZoneHVACBaseboardRadiantConvectiveSteam(
    std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveSteam_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
