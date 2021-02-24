/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Storage_LiIonNMCBattery_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ElectricalStorage_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType());
    }

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : ElectricalStorage_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType());
    }

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(const ElectricLoadCenterStorageLiIonNMCBattery_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : ElectricalStorage_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricLoadCenterStorageLiIonNMCBattery_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{}; // TODO
      return result;
    }

    IddObjectType ElectricLoadCenterStorageLiIonNMCBattery_Impl::iddObjectType() const {
      return ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricLoadCenterStorageLiIonNMCBattery_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ElectricLoadCenterStorageLiIonNMCBattery", "Availability"));
      }
      return result;
    }

    //@}
    /** @name Getters */
    //@{

    // Included in parent class
    // boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageLiIonNMCBattery_Impl::electricLoadCenterDistribution() const
    // {
    // boost::optional<ElectricLoadCenterDistribution> result;
    // for (auto list : getObject<ModelObject>().getModelObjectSources<ModelObjectList>(ModelObjectList::iddObjectType())){
    // auto elcds = list.getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
    // if (elcds.empty()){
    // error
    // } else if (elcds.size() == 1u){
    // return elcds[0];
    // }else{
    // error
    // }
    // }
    // return boost::none;
    // }

    //boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageLiIonNMCBattery_Impl::electricLoadCenterDistribution const {

    //  boost::optional<ElectricLoadCenterDistribution> value;
    //  for ( const ElectricLoadCenterDistribution& elcd : this->model().getConcreteModelObjects<ElectricLoadCenterDistribution>() )
    //  {
    //    if ( boost::optional<ElectricalStorage> elecStor = mchp.electricalStorage() )
    //    {
    //      if (elecStor->handle() == this->handle())
    //      {
    //        value = elcd;
    //      }
    //    }
    //  }
    //  return value;

    //}

    boost::optional<Schedule> ElectricLoadCenterStorageLiIonNMCBattery_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName);
    }

    Schedule ElectricLoadCenterStorageLiIonNMCBattery_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        //LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
        // I'm choosing to just return alwaysOnDiscreteSchedule
        return this->model().alwaysOnDiscreteSchedule();
      }
      return value.get();
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::isAvailabilityScheduleDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName);
    }

    // Optional thermal Zone for heat gains
    boost::optional<ThermalZone> ElectricLoadCenterStorageLiIonNMCBattery_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName);
    }

    // Radiative fraction for zone heat gains, defaults to 0, and not used if thermalZone above isn't used
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::radiativeFractionforZoneHeatGains() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFractionforZoneHeatGains, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::isRadiativeFractionforZoneHeatGainsDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFractionforZoneHeatGains);
    }

    // Nominal Efficiency for Charging for zone heat gains, has a default in IDD
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::nominalEnergeticEfficiencyforCharging() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalEnergeticEfficiencyforCharging, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::isNominalEnergeticEfficiencyforChargingDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalEnergeticEfficiencyforCharging);
    }

    // Nominal Efficiency for Discharging for zone heat gains, has a default in IDD
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::nominalDischargingEnergeticEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalDischargingEnergeticEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::isNominalDischargingEnergeticEfficiencyDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalDischargingEnergeticEfficiency);
    }

    // Maximum Storage Capacity, required, no default
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::maximumStorageCapacity() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumStorageCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    // Maximum Rate of Discharge, required, no default
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::maximumPowerforDischarging() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumPowerforDischarging, true);
      OS_ASSERT(value);
      return value.get();
    }

    // Maximum Rate of Charge, required, no default
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::maximumPowerforCharging() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumPowerforCharging, true);
      OS_ASSERT(value);
      return value.get();
    }

    // Initial state of charge, if empty it will default to half of the maximum capacity
    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::initialStateofCharge() const {
      boost::optional<double> optIniStateCharge = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialStateofCharge, true);
      // if it's set
      if (optIniStateCharge) {
        // Get it and return
        return optIniStateCharge.get();
      }
      // We Default it to half the maximum capacity
      double maxCap = maximumStorageCapacity();
      return maxCap / 2.0;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::isInitialStateofChargeDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialStateofCharge);
    }

    //@}
    /** @name Setters */
    //@{

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName, "ElectricLoadCenterStorageLiIonNMCBattery",
                                "Availability", schedule);
      return result;
    }

    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    // Set the thermal Zone for heat gains
    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setThermalZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName, thermalZone.handle());
      return result;
    }

    // Reset the zone (leave blank)
    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetThermalZone() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFractionforZoneHeatGains, radiativeFractionforZoneHeatGains);
      return result;
    }

    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetRadiativeFractionforZoneHeatGains() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFractionforZoneHeatGains, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging) {
      bool result =
        setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalEnergeticEfficiencyforCharging, nominalEnergeticEfficiencyforCharging);
      return result;
    }

    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetNominalEnergeticEfficiencyforCharging() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalEnergeticEfficiencyforCharging, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
      bool result =
        setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalDischargingEnergeticEfficiency, nominalDischargingEnergeticEfficiency);
      return result;
    }

    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetNominalDischargingEnergeticEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NominalDischargingEnergeticEfficiency, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setMaximumStorageCapacity(double maximumStorageCapacity) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumStorageCapacity, maximumStorageCapacity);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setMaximumPowerforDischarging(double maximumPowerforDischarging) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumPowerforDischarging, maximumPowerforDischarging);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setMaximumPowerforCharging(double maximumPowerforCharging) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::MaximumPowerforCharging, maximumPowerforCharging);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setInitialStateofCharge(double value) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialStateofCharge, value);
      return result;
    }

    void ElectricLoadCenterStorageLiIonNMCBattery_Impl::resetInitialStateofCharge() {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialStateofCharge, "");
      OS_ASSERT(result);
    }

    std::vector<EMSActuatorNames> ElectricLoadCenterStorageLiIonNMCBattery_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Electrical Storage", "Power Draw Rate"}, {"Electrical Storage", "Power Charge Rate"}};
      return actuators;
    }

    std::vector<std::string> ElectricLoadCenterStorageLiIonNMCBattery_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{}; // TODO
      return types;
    }
  }  // namespace detail

  /* Constructor. Defaults the availabilitySchedule to alwaysOnDiscreteSchedule,
 * Defaults efficiencies for charge and discharge to 0.8
 * maximum storage capacity to 1.0E13
 * Maximum Power for Charge/Discharge to 1.0E6
 * Reference: EnergyPlus example files (v8.5, LrgOff_GridStorageDemandLeveling */
  ElectricLoadCenterStorageLiIonNMCBattery::ElectricLoadCenterStorageLiIonNMCBattery(const Model& model)
    : ElectricalStorage(ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>());

    // already defaults to alwaysOn
    //Schedule schedule = model.alwaysOnDiscreteSchedule();
    //setAvailabilitySchedule(schedule);

    setNominalEnergeticEfficiencyforCharging(0.8);
    setNominalDischargingEnergeticEfficiency(0.8);
    setMaximumStorageCapacity(1.0E13);
    setMaximumPowerforDischarging(1.0E6);
    setMaximumPowerforCharging(1.0E6);
  }

  IddObjectType ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Storage_LiIonNMCBattery);
  }

  // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
  // Included in parent
  // boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageLiIonNMCBattery::electricLoadCenterDistribution() const
  // {
  // return getImpl<detail::Generator_Impl>()->electricLoadCenterDistribution();
  // }

  Schedule ElectricLoadCenterStorageLiIonNMCBattery::availabilitySchedule() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->availabilitySchedule();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::isAvailabilityScheduleDefaulted() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->isAvailabilityScheduleDefaulted();
  }

  // TODO: Included in parent shouldn't need that
  //boost::optional<ThermalZone> ElectricLoadCenterStorageLiIonNMCBattery::thermalZone() const {
  //  return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->thermalZone();
  //}

  double ElectricLoadCenterStorageLiIonNMCBattery::radiativeFractionforZoneHeatGains() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->radiativeFractionforZoneHeatGains();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::isRadiativeFractionforZoneHeatGainsDefaulted() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->isRadiativeFractionforZoneHeatGainsDefaulted();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::nominalEnergeticEfficiencyforCharging() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->nominalEnergeticEfficiencyforCharging();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::isNominalEnergeticEfficiencyforChargingDefaulted() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->isNominalEnergeticEfficiencyforChargingDefaulted();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::nominalDischargingEnergeticEfficiency() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->nominalDischargingEnergeticEfficiency();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::isNominalDischargingEnergeticEfficiencyDefaulted() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->isNominalDischargingEnergeticEfficiencyDefaulted();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::maximumStorageCapacity() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->maximumStorageCapacity();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::maximumPowerforDischarging() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->maximumPowerforDischarging();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::maximumPowerforCharging() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->maximumPowerforCharging();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::initialStateofCharge() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->initialStateofCharge();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::isInitialStateofChargeDefaulted() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->isInitialStateofChargeDefaulted();
  }

  // Setters
  bool ElectricLoadCenterStorageLiIonNMCBattery::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setAvailabilitySchedule(schedule);
  }

  void ElectricLoadCenterStorageLiIonNMCBattery::resetAvailabilitySchedule() {
    getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetAvailabilitySchedule();
  }

  // TODO: Included in parent shouldn't need that
  //bool ElectricLoadCenterStorageLiIonNMCBattery::setThermalZone(ThermalZone& zone) {
  //  return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setThermalZone(zone);
  //}
  //void ElectricLoadCenterStorageLiIonNMCBattery::resetThermalZone() {
  //  getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetThermalZone();
  //}

  bool ElectricLoadCenterStorageLiIonNMCBattery::setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setRadiativeFractionforZoneHeatGains(radiativeFractionforZoneHeatGains);
  }

  void ElectricLoadCenterStorageLiIonNMCBattery::resetRadiativeFractionforZoneHeatGains() {
    getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetRadiativeFractionforZoneHeatGains();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setNominalEnergeticEfficiencyforCharging(nominalEnergeticEfficiencyforCharging);
  }

  void ElectricLoadCenterStorageLiIonNMCBattery::resetNominalEnergeticEfficiencyforCharging() {
    getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetNominalEnergeticEfficiencyforCharging();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setNominalDischargingEnergeticEfficiency(nominalDischargingEnergeticEfficiency);
  }

  void ElectricLoadCenterStorageLiIonNMCBattery::resetNominalDischargingEnergeticEfficiency() {
    getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetNominalDischargingEnergeticEfficiency();
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setMaximumStorageCapacity(double maximumStorageCapacity) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setMaximumStorageCapacity(maximumStorageCapacity);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setMaximumPowerforDischarging(double maximumPowerforDischarging) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setMaximumPowerforDischarging(maximumPowerforDischarging);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setMaximumPowerforCharging(double maximumPowerforCharging) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setMaximumPowerforCharging(maximumPowerforCharging);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setInitialStateofCharge(double initialStateofCharge) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setInitialStateofCharge(initialStateofCharge);
  }

  void ElectricLoadCenterStorageLiIonNMCBattery::resetInitialStateofCharge() {
    getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetInitialStateofCharge();
  }

  /// @cond
  ElectricLoadCenterStorageLiIonNMCBattery::ElectricLoadCenterStorageLiIonNMCBattery(std::shared_ptr<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl> impl)
    : ElectricalStorage(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
