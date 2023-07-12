/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACBaseboardConvectiveElectric.hpp"
#include "ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_ZoneHVAC_Baseboard_Convective_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardConvectiveElectric::iddObjectType());
    }

    ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardConvectiveElectric::iddObjectType());
    }

    ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const ZoneHVACBaseboardConvectiveElectric_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACBaseboardConvectiveElectric_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Baseboard Total Heating Rate", "Baseboard Total Heating Energy", "Baseboard Electricity Energy",
                                                   "Baseboard Electricity Rate"};
      return result;
    }

    IddObjectType ZoneHVACBaseboardConvectiveElectric_Impl::iddObjectType() const {
      return ZoneHVACBaseboardConvectiveElectric::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACBaseboardConvectiveElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACBaseboardConvectiveElectric", "Availability"));
      }
      return result;
    }

    Schedule ZoneHVACBaseboardConvectiveElectric_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<double> ZoneHVACBaseboardConvectiveElectric_Impl::nominalCapacity() const {
      return getDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, true);
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double ZoneHVACBaseboardConvectiveElectric_Impl::efficiency() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::isEfficiencyDefaulted() const {
      return isEmpty(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency);
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule, "ZoneHVACBaseboardConvectiveElectric",
                                "Availability", schedule);
      return result;
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
      bool result(false);
      if (nominalCapacity) {
        result = setDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, nominalCapacity.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void ZoneHVACBaseboardConvectiveElectric_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::setEfficiency(double efficiency) {
      bool result = setDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, efficiency);
      return result;
    }

    void ZoneHVACBaseboardConvectiveElectric_Impl::resetEfficiency() {
      bool result = setString(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ZoneHVACBaseboardConvectiveElectric_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule);
    }

    boost::optional<ModelObject> ZoneHVACBaseboardConvectiveElectric_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool ZoneHVACBaseboardConvectiveElectric_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    unsigned ZoneHVACBaseboardConvectiveElectric_Impl::inletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    unsigned ZoneHVACBaseboardConvectiveElectric_Impl::outletPort() const {
      return 0;  // this object has no inlet or outlet node
    }

    boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveElectric_Impl::thermalZone() const {
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

    bool ZoneHVACBaseboardConvectiveElectric_Impl::addToThermalZone(ThermalZone& thermalZone) {
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

    void ZoneHVACBaseboardConvectiveElectric_Impl::removeFromThermalZone() {
      if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {
        thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
      }
    }

    boost::optional<double> ZoneHVACBaseboardConvectiveElectric_Impl::autosizedNominalCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Heating Design Capacity", "W", "ZONEHVAC:BASEBOARD:CONVECTIVE:ELECTRIC");
    }

    void ZoneHVACBaseboardConvectiveElectric_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void ZoneHVACBaseboardConvectiveElectric_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    std::vector<EMSActuatorNames> ZoneHVACBaseboardConvectiveElectric_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ZoneBaseboard:OutdoorTemperatureControlled", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> ZoneHVACBaseboardConvectiveElectric_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Simple Zone Baseboard Capacity At Low Temperature", "Simple Zone Baseboard Capacity At High Temperature"};
      return types;
    }

    ComponentType ZoneHVACBaseboardConvectiveElectric_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> ZoneHVACBaseboardConvectiveElectric_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ZoneHVACBaseboardConvectiveElectric_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> ZoneHVACBaseboardConvectiveElectric_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::Electric};
    }

  }  // namespace detail

  ZoneHVACBaseboardConvectiveElectric::ZoneHVACBaseboardConvectiveElectric(const Model& model)
    : ZoneHVACComponent(ZoneHVACBaseboardConvectiveElectric::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>());

    autosizeNominalCapacity();

    Schedule schedule = model.alwaysOnDiscreteSchedule();

    setAvailabilitySchedule(schedule);

    setEfficiency(1.0);
  }

  IddObjectType ZoneHVACBaseboardConvectiveElectric::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric};
  }

  Schedule ZoneHVACBaseboardConvectiveElectric::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->availabilitySchedule();
  }

  boost::optional<double> ZoneHVACBaseboardConvectiveElectric::nominalCapacity() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->nominalCapacity();
  }

  bool ZoneHVACBaseboardConvectiveElectric::isNominalCapacityAutosized() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->isNominalCapacityAutosized();
  }

  double ZoneHVACBaseboardConvectiveElectric::efficiency() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->efficiency();
  }

  bool ZoneHVACBaseboardConvectiveElectric::isEfficiencyDefaulted() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->isEfficiencyDefaulted();
  }

  bool ZoneHVACBaseboardConvectiveElectric::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACBaseboardConvectiveElectric::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void ZoneHVACBaseboardConvectiveElectric::autosizeNominalCapacity() {
    getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->autosizeNominalCapacity();
  }

  bool ZoneHVACBaseboardConvectiveElectric::setEfficiency(double efficiency) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setEfficiency(efficiency);
  }

  void ZoneHVACBaseboardConvectiveElectric::resetEfficiency() {
    getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->resetEfficiency();
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveElectric::thermalZone() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->thermalZone();
  }

  bool ZoneHVACBaseboardConvectiveElectric::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACBaseboardConvectiveElectric::removeFromThermalZone() {
    getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->removeFromThermalZone();
  }

  /// @cond
  ZoneHVACBaseboardConvectiveElectric::ZoneHVACBaseboardConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveElectric_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> ZoneHVACBaseboardConvectiveElectric::autosizedNominalCapacity() const {
    return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model
}  // namespace openstudio
