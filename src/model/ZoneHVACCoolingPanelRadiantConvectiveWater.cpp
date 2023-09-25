/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACCoolingPanelRadiantConvectiveWater.hpp"
#include "ZoneHVACCoolingPanelRadiantConvectiveWater_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilCoolingWaterPanelRadiant.hpp"
#include "CoilCoolingWaterPanelRadiant_Impl.hpp"
#include "Model.hpp"
#include "Space.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_CoolingPanel_RadiantConvective_Water_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                     bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACCoolingPanelRadiantConvectiveWater::iddObjectType());
    }

    ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACCoolingPanelRadiantConvectiveWater::iddObjectType());
    }

    ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl(
      const ZoneHVACCoolingPanelRadiantConvectiveWater_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Cooling Panel Total Cooling Rate",        "Cooling Panel Total System Cooling Rate",
                                                   "Cooling Panel Convective Cooling Rate",   "Cooling Panel Radiant Cooling Rate",
                                                   "Cooling Panel Total Cooling Energy",      "Cooling Panel Total System Cooling Energy",
                                                   "Cooling Panel Convective Cooling Energy", "Cooling Panel Radiant Cooling Energy"};
      return result;
    }

    IddObjectType ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::iddObjectType() const {
      return ZoneHVACCoolingPanelRadiantConvectiveWater::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACCoolingPanelRadiantConvectiveWater", "Availability"));
      }
      return result;
    }

    unsigned ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    unsigned ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    boost::optional<ThermalZone> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::thermalZone() const {
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

    bool ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m || thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::removeFromThermalZone() {
      if (auto thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    std::vector<Surface> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::surfaces() const {

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

    std::vector<ModelObject> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalCoolingCoil()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    ModelObject ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::clone(Model model) const {
      auto panelRadConvWaterClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACCoolingPanelRadiantConvectiveWater>();

      auto t_coolingCoil = coolingCoil();
      auto coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

      panelRadConvWaterClone.setCoolingCoil(coolingCoilClone);

      if (model == this->model()) {
        if (auto plant = t_coolingCoil.plantLoop()) {
          plant->addDemandBranchForComponent(coolingCoilClone);
        }
      }

      return std::move(panelRadConvWaterClone);
    }

    std::vector<IdfObject> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::remove() {
      if (auto waterCoolingCoil = coolingCoil().optionalCast<CoilCoolingWaterPanelRadiant>()) {
        if (boost::optional<PlantLoop> plantLoop = waterCoolingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent(waterCoolingCoil.get());
        }
      }
      return ZoneHVACComponent_Impl::remove();
    }

    Schedule ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    double ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::fractionRadiant() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionRadiant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::fractionofRadiantEnergyIncidentonPeople() const {
      boost::optional<double> value =
        getDouble(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople, true);
      OS_ASSERT(value);
      return value.get();
    }

    HVACComponent ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::coolingCoil() const {
      boost::optional<HVACComponent> value = optionalCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
      }
      return value.get();
    }

    bool ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::AvailabilityScheduleName,
                                "ZoneHVACCoolingPanelRadiantConvectiveWater", "Availability", schedule);
      return result;
    }

    bool ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::setFractionRadiant(double fractionRadiant) {
      bool result = setDouble(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionRadiant, fractionRadiant);
      return result;
    }

    bool ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
      bool result = setDouble(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople,
                              fractionofRadiantEnergyIncidentonPeople);
      return result;
    }

    bool ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::setCoolingCoil(const HVACComponent& radPanelCoolingCoil) {
      bool result = setPointer(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingCoilName, radPanelCoolingCoil.handle());
      return result;
    }

    boost::optional<Schedule> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::AvailabilityScheduleName);
    }

    boost::optional<HVACComponent> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingCoilName);
    }

    std::vector<EMSActuatorNames> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneCoolingPanel:OutdoorTemperatureControlled", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Simple Zone CoolingPanel Capacity At Low Temperature", "Simple Zone CoolingPanel Capacity At High Temperature"};
      return types;
    }

    ComponentType ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::coolingFuelTypes() const {
      return coolingCoil().coolingFuelTypes();
    }

    std::vector<FuelType> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ZoneHVACCoolingPanelRadiantConvectiveWater_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  ZoneHVACCoolingPanelRadiantConvectiveWater::ZoneHVACCoolingPanelRadiantConvectiveWater(const Model& model)
    : ZoneHVACComponent(ZoneHVACCoolingPanelRadiantConvectiveWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>());

    bool ok = true;
    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);
    ok = setFractionRadiant(0.65);
    OS_ASSERT(ok);
    ok = setFractionofRadiantEnergyIncidentonPeople(0.2);
    OS_ASSERT(ok);
    CoilCoolingWaterPanelRadiant coil(model);
    ok = setCoolingCoil(coil);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACCoolingPanelRadiantConvectiveWater::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_CoolingPanel_RadiantConvective_Water};
  }

  Schedule ZoneHVACCoolingPanelRadiantConvectiveWater::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->availabilitySchedule();
  }

  double ZoneHVACCoolingPanelRadiantConvectiveWater::fractionRadiant() const {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->fractionRadiant();
  }

  double ZoneHVACCoolingPanelRadiantConvectiveWater::fractionofRadiantEnergyIncidentonPeople() const {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->fractionofRadiantEnergyIncidentonPeople();
  }

  HVACComponent ZoneHVACCoolingPanelRadiantConvectiveWater::coolingCoil() const {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->coolingCoil();
  }

  bool ZoneHVACCoolingPanelRadiantConvectiveWater::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACCoolingPanelRadiantConvectiveWater::setFractionRadiant(double fractionRadiant) {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->setFractionRadiant(fractionRadiant);
  }

  bool ZoneHVACCoolingPanelRadiantConvectiveWater::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->setFractionofRadiantEnergyIncidentonPeople(
      fractionofRadiantEnergyIncidentonPeople);
  }

  bool ZoneHVACCoolingPanelRadiantConvectiveWater::setCoolingCoil(const HVACComponent& radPanelCoolingCoil) {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->setCoolingCoil(radPanelCoolingCoil);
  }

  boost::optional<ThermalZone> ZoneHVACCoolingPanelRadiantConvectiveWater::thermalZone() const {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->thermalZone();
  }

  bool ZoneHVACCoolingPanelRadiantConvectiveWater::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACCoolingPanelRadiantConvectiveWater::removeFromThermalZone() {
    return getImpl<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->removeFromThermalZone();
  }

  /// @cond
  ZoneHVACCoolingPanelRadiantConvectiveWater::ZoneHVACCoolingPanelRadiantConvectiveWater(
    std::shared_ptr<detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
