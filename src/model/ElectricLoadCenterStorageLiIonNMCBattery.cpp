/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Storage_LiIonNMCBattery_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : ElectricalStorage_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType());
    }

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ElectricalStorage_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType());
    }

    ElectricLoadCenterStorageLiIonNMCBattery_Impl::ElectricLoadCenterStorageLiIonNMCBattery_Impl(
      const ElectricLoadCenterStorageLiIonNMCBattery_Impl& other, Model_Impl* model, bool keepHandle)
      : ElectricalStorage_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricLoadCenterStorageLiIonNMCBattery_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Electric Storage Operating mode Index", "Electric Storage Battery Charge State", "Electric Storage Charge Fraction",
        "Electric Storage Total Current",        "Electric Storage Total Voltage",        "Electric Storage Degradation Fraction",
        "Electric Storage Charge Power",         "Electric Storage Charge Energy",        "Electric Storage Production Decrement Energy",
        "Electric Storage Discharge Power",      "Electric Storage Discharge Energy",     "Electric Storage Thermal Loss Rate",
        "Electric Storage Thermal Loss Energy",  "Electric Storage Battery Temperature"};
      return result;
    }

    IddObjectType ElectricLoadCenterStorageLiIonNMCBattery_Impl::iddObjectType() const {
      return ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricLoadCenterStorageLiIonNMCBattery_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
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

    // Optional thermal Zone for heat gains
    boost::optional<ThermalZone> ElectricLoadCenterStorageLiIonNMCBattery_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName);
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::radiativeFraction() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ElectricLoadCenterStorageLiIonNMCBattery_Impl::lifetimeModel() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ElectricLoadCenterStorageLiIonNMCBattery_Impl::numberofCellsinSeries() const {
      boost::optional<int> value = getInt(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ElectricLoadCenterStorageLiIonNMCBattery_Impl::numberofStringsinParallel() const {
      boost::optional<int> value = getInt(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::initialFractionalStateofCharge() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialFractionalStateofCharge, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::dctoDCChargingEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DCtoDCChargingEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::batteryMass() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::batterySurfaceArea() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::batterySpecificHeatCapacity() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySpecificHeatCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::heatTransferCoefficientBetweenBatteryandAmbient() const {
      boost::optional<double> value =
        getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::HeatTransferCoefficientBetweenBatteryandAmbient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::fullyChargedCellVoltage() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellVoltage, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::cellVoltageatEndofExponentialZone() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofExponentialZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::cellVoltageatEndofNominalZone() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofNominalZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::defaultNominalCellVoltage() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DefaultNominalCellVoltage, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::fullyChargedCellCapacity() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::fractionofCellCapacityRemovedattheEndofExponentialZone() const {
      boost::optional<double> value =
        getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofExponentialZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::fractionofCellCapacityRemovedattheEndofNominalZone() const {
      boost::optional<double> value =
        getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofNominalZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::chargeRateatWhichVoltagevsCapacityCurveWasGenerated() const {
      boost::optional<double> value =
        getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ChargeRateatWhichVoltagevsCapacityCurveWasGenerated, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ElectricLoadCenterStorageLiIonNMCBattery_Impl::batteryCellInternalElectricalResistance() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryCellInternalElectricalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    //@}
    /** @name Setters */
    //@{

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName,
                                "ElectricLoadCenterStorageLiIonNMCBattery", "Availability", schedule);
      return result;
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

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setRadiativeFraction(double radiativeFraction) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFraction, radiativeFraction);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setLifetimeModel(const std::string& lifetimeModel) {
      bool result = setString(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel, lifetimeModel);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setNumberofCellsinSeries(int numberofCellsinSeries) {
      bool result = setInt(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries, numberofCellsinSeries);
      OS_ASSERT(result);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setNumberofStringsinParallel(int numberofStringsinParallel) {
      bool result = setInt(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel, numberofStringsinParallel);
      OS_ASSERT(result);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setInitialFractionalStateofCharge(double initialFractionalStateofCharge) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialFractionalStateofCharge, initialFractionalStateofCharge);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setDCtoDCChargingEfficiency(double dctoDCChargingEfficiency) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DCtoDCChargingEfficiency, dctoDCChargingEfficiency);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setBatteryMass(double batteryMass) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass, batteryMass);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setBatterySurfaceArea(double batterySurfaceArea) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea, batterySurfaceArea);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setBatterySpecificHeatCapacity(double batterySpecificHeatCapacity) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySpecificHeatCapacity, batterySpecificHeatCapacity);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setHeatTransferCoefficientBetweenBatteryandAmbient(
      double heatTransferCoefficientBetweenBatteryandAmbient) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::HeatTransferCoefficientBetweenBatteryandAmbient,
                              heatTransferCoefficientBetweenBatteryandAmbient);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setFullyChargedCellVoltage(double fullyChargedCellVoltage) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellVoltage, fullyChargedCellVoltage);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setCellVoltageatEndofExponentialZone(double cellVoltageatEndofExponentialZone) {
      bool result =
        setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofExponentialZone, cellVoltageatEndofExponentialZone);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setCellVoltageatEndofNominalZone(double cellVoltageatEndofNominalZone) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofNominalZone, cellVoltageatEndofNominalZone);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setDefaultNominalCellVoltage(double defaultNominalCellVoltage) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DefaultNominalCellVoltage, defaultNominalCellVoltage);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setFullyChargedCellCapacity(double fullyChargedCellCapacity) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellCapacity, fullyChargedCellCapacity);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setFractionofCellCapacityRemovedattheEndofExponentialZone(
      double fractionofCellCapacityRemovedattheEndofExponentialZone) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofExponentialZone,
                              fractionofCellCapacityRemovedattheEndofExponentialZone);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setFractionofCellCapacityRemovedattheEndofNominalZone(
      double fractionofCellCapacityRemovedattheEndofNominalZone) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofNominalZone,
                              fractionofCellCapacityRemovedattheEndofNominalZone);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(
      double chargeRateatWhichVoltagevsCapacityCurveWasGenerated) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ChargeRateatWhichVoltagevsCapacityCurveWasGenerated,
                              chargeRateatWhichVoltagevsCapacityCurveWasGenerated);
      return result;
    }

    bool ElectricLoadCenterStorageLiIonNMCBattery_Impl::setBatteryCellInternalElectricalResistance(double batteryCellInternalElectricalResistance) {
      bool result = setDouble(OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryCellInternalElectricalResistance,
                              batteryCellInternalElectricalResistance);
      return result;
    }

    std::vector<EMSActuatorNames> ElectricLoadCenterStorageLiIonNMCBattery_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Electrical Storage", "Power Draw Rate"}, {"Electrical Storage", "Power Charge Rate"}};
      return actuators;
    }

    std::vector<std::string> ElectricLoadCenterStorageLiIonNMCBattery_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{};  // TODO
      return types;
    }
  }  // namespace detail

  /* Default Constructor */
  ElectricLoadCenterStorageLiIonNMCBattery::ElectricLoadCenterStorageLiIonNMCBattery(const Model& model)
    : ElectricalStorage(ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>());

    // Defaults to alwaysOn
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(schedule);
    OS_ASSERT(ok);

    // These from ShopWithPVandLiIonBattery.idf. They should be modified by user!
    setNumberofCellsinSeries(139);
    setNumberofStringsinParallel(25);
    setBatteryMass(342.0);
    setBatterySurfaceArea(4.26);

    setRadiativeFraction(0);
    setLifetimeModel("KandlerSmith");
    setInitialFractionalStateofCharge(0.5);
    setDCtoDCChargingEfficiency(0.95);
    setBatterySpecificHeatCapacity(1500);
    setHeatTransferCoefficientBetweenBatteryandAmbient(7.5);
    setFullyChargedCellVoltage(4.2);
    setCellVoltageatEndofExponentialZone(3.53);
    setCellVoltageatEndofNominalZone(3.342);
    setDefaultNominalCellVoltage(3.342);
    setFullyChargedCellCapacity(3.2);
    setFractionofCellCapacityRemovedattheEndofExponentialZone(0.8075);
    setFractionofCellCapacityRemovedattheEndofNominalZone(0.976875);
    setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(1);
    setBatteryCellInternalElectricalResistance(0.09);
  }

  /* Constructor. Defaults the availabilitySchedule to alwaysOnDiscreteSchedule */
  ElectricLoadCenterStorageLiIonNMCBattery::ElectricLoadCenterStorageLiIonNMCBattery(const Model& model, const int numberofCellsinSeries,
                                                                                     const int numberofStringsinParallel, const double batteryMass,
                                                                                     const double batterySurfaceArea)
    : ElectricalStorage(ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>());

    // Defaults to alwaysOn
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(schedule);
    OS_ASSERT(ok);

    setRadiativeFraction(0);
    setLifetimeModel("KandlerSmith");
    setNumberofCellsinSeries(numberofCellsinSeries);
    setNumberofStringsinParallel(numberofStringsinParallel);
    setInitialFractionalStateofCharge(0.5);
    setDCtoDCChargingEfficiency(0.95);
    setBatteryMass(batteryMass);
    setBatterySurfaceArea(batterySurfaceArea);
    setBatterySpecificHeatCapacity(1500);
    setHeatTransferCoefficientBetweenBatteryandAmbient(7.5);
    setFullyChargedCellVoltage(4.2);
    setCellVoltageatEndofExponentialZone(3.53);
    setCellVoltageatEndofNominalZone(3.342);
    setDefaultNominalCellVoltage(3.342);
    setFullyChargedCellCapacity(3.2);
    setFractionofCellCapacityRemovedattheEndofExponentialZone(0.8075);
    setFractionofCellCapacityRemovedattheEndofNominalZone(0.976875);
    setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(1);
    setBatteryCellInternalElectricalResistance(0.09);
  }

  IddObjectType ElectricLoadCenterStorageLiIonNMCBattery::iddObjectType() {
    return {IddObjectType::OS_ElectricLoadCenter_Storage_LiIonNMCBattery};
  }

  std::vector<std::string> ElectricLoadCenterStorageLiIonNMCBattery::lifetimeModelValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel);
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

  // Included in parent shouldn't need that
  //boost::optional<ThermalZone> ElectricLoadCenterStorageLiIonNMCBattery::thermalZone() const {
  //  return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->thermalZone();
  //}

  double ElectricLoadCenterStorageLiIonNMCBattery::radiativeFraction() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->radiativeFraction();
  }

  std::string ElectricLoadCenterStorageLiIonNMCBattery::lifetimeModel() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->lifetimeModel();
  }

  int ElectricLoadCenterStorageLiIonNMCBattery::numberofCellsinSeries() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->numberofCellsinSeries();
  }

  int ElectricLoadCenterStorageLiIonNMCBattery::numberofStringsinParallel() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->numberofStringsinParallel();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::initialFractionalStateofCharge() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->initialFractionalStateofCharge();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::dctoDCChargingEfficiency() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->dctoDCChargingEfficiency();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::batteryMass() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->batteryMass();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::batterySurfaceArea() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->batterySurfaceArea();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::batterySpecificHeatCapacity() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->batterySpecificHeatCapacity();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::heatTransferCoefficientBetweenBatteryandAmbient() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->heatTransferCoefficientBetweenBatteryandAmbient();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::fullyChargedCellVoltage() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->fullyChargedCellVoltage();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::cellVoltageatEndofExponentialZone() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->cellVoltageatEndofExponentialZone();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::cellVoltageatEndofNominalZone() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->cellVoltageatEndofNominalZone();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::defaultNominalCellVoltage() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->defaultNominalCellVoltage();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::fullyChargedCellCapacity() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->fullyChargedCellCapacity();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::fractionofCellCapacityRemovedattheEndofExponentialZone() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->fractionofCellCapacityRemovedattheEndofExponentialZone();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::fractionofCellCapacityRemovedattheEndofNominalZone() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->fractionofCellCapacityRemovedattheEndofNominalZone();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::chargeRateatWhichVoltagevsCapacityCurveWasGenerated() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->chargeRateatWhichVoltagevsCapacityCurveWasGenerated();
  }

  double ElectricLoadCenterStorageLiIonNMCBattery::batteryCellInternalElectricalResistance() const {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->batteryCellInternalElectricalResistance();
  }

  // Setters
  bool ElectricLoadCenterStorageLiIonNMCBattery::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setAvailabilitySchedule(schedule);
  }

  // Included in parent shouldn't need that
  //bool ElectricLoadCenterStorageLiIonNMCBattery::setThermalZone(ThermalZone& zone) {
  //  return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setThermalZone(zone);
  //}
  //void ElectricLoadCenterStorageLiIonNMCBattery::resetThermalZone() {
  //  getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->resetThermalZone();
  //}

  bool ElectricLoadCenterStorageLiIonNMCBattery::setRadiativeFraction(double radiativeFraction) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setRadiativeFraction(radiativeFraction);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setLifetimeModel(const std::string& lifetimeModel) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setLifetimeModel(lifetimeModel);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setNumberofCellsinSeries(int numberofCellsinSeries) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setNumberofCellsinSeries(numberofCellsinSeries);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setNumberofStringsinParallel(int numberofStringsinParallel) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setNumberofStringsinParallel(numberofStringsinParallel);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setInitialFractionalStateofCharge(double initialFractionalStateofCharge) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setInitialFractionalStateofCharge(initialFractionalStateofCharge);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setDCtoDCChargingEfficiency(double dctoDCChargingEfficiency) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setDCtoDCChargingEfficiency(dctoDCChargingEfficiency);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setBatteryMass(double batteryMass) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setBatteryMass(batteryMass);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setBatterySurfaceArea(double batterySurfaceArea) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setBatterySurfaceArea(batterySurfaceArea);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setBatterySpecificHeatCapacity(double batterySpecificHeatCapacity) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setBatterySpecificHeatCapacity(batterySpecificHeatCapacity);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setHeatTransferCoefficientBetweenBatteryandAmbient(
    double heatTransferCoefficientBetweenBatteryandAmbient) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setHeatTransferCoefficientBetweenBatteryandAmbient(
      heatTransferCoefficientBetweenBatteryandAmbient);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setFullyChargedCellVoltage(double fullyChargedCellCapacity) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setFullyChargedCellVoltage(fullyChargedCellCapacity);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setCellVoltageatEndofExponentialZone(double cellVoltageatEndofExponentialZone) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setCellVoltageatEndofExponentialZone(cellVoltageatEndofExponentialZone);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setCellVoltageatEndofNominalZone(double cellVoltageatEndofNominalZone) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setCellVoltageatEndofNominalZone(cellVoltageatEndofNominalZone);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setDefaultNominalCellVoltage(double defaultNominalCellVoltage) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setDefaultNominalCellVoltage(defaultNominalCellVoltage);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setFullyChargedCellCapacity(double fullyChargedCellCapacity) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setFullyChargedCellCapacity(fullyChargedCellCapacity);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setFractionofCellCapacityRemovedattheEndofExponentialZone(
    double fractionofCellCapacityRemovedattheEndofExponentialZone) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setFractionofCellCapacityRemovedattheEndofExponentialZone(
      fractionofCellCapacityRemovedattheEndofExponentialZone);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setFractionofCellCapacityRemovedattheEndofNominalZone(
    double fractionofCellCapacityRemovedattheEndofNominalZone) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setFractionofCellCapacityRemovedattheEndofNominalZone(
      fractionofCellCapacityRemovedattheEndofNominalZone);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(
    double chargeRateatWhichVoltagevsCapacityCurveWasGenerated) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(
      chargeRateatWhichVoltagevsCapacityCurveWasGenerated);
  }

  bool ElectricLoadCenterStorageLiIonNMCBattery::setBatteryCellInternalElectricalResistance(double batteryCellInternalElectricalResistance) {
    return getImpl<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl>()->setBatteryCellInternalElectricalResistance(
      batteryCellInternalElectricalResistance);
  }

  /// @cond
  ElectricLoadCenterStorageLiIonNMCBattery::ElectricLoadCenterStorageLiIonNMCBattery(
    std::shared_ptr<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl> impl)
    : ElectricalStorage(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
