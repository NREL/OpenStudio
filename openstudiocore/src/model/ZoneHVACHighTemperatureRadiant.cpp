/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ZoneHVACHighTemperatureRadiant.hpp"
#include "ZoneHVACHighTemperatureRadiant_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_HighTemperatureRadiant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACHighTemperatureRadiant_Impl::ZoneHVACHighTemperatureRadiant_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACHighTemperatureRadiant::iddObjectType());
  }

  ZoneHVACHighTemperatureRadiant_Impl::ZoneHVACHighTemperatureRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACHighTemperatureRadiant::iddObjectType());
  }

  ZoneHVACHighTemperatureRadiant_Impl::ZoneHVACHighTemperatureRadiant_Impl(const ZoneHVACHighTemperatureRadiant_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACHighTemperatureRadiant_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACHighTemperatureRadiant_Impl::iddObjectType() const {
    return ZoneHVACHighTemperatureRadiant::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACHighTemperatureRadiant_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_HighTemperatureRadiantFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACHighTemperatureRadiant","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingSetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACHighTemperatureRadiant","Heating Setpoint Temperature"));
    }
    return result;
  }

  boost::optional<Schedule> ZoneHVACHighTemperatureRadiant_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_HighTemperatureRadiantFields::AvailabilityScheduleName);
  }

  boost::optional<double> ZoneHVACHighTemperatureRadiant_Impl::maximumPowerInput() const {
    return getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::MaximumPowerInput,true);
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::isMaximumPowerInputAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_HighTemperatureRadiantFields::MaximumPowerInput, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ZoneHVACHighTemperatureRadiant_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_HighTemperatureRadiantFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACHighTemperatureRadiant_Impl::combustionEfficiency() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::CombustionEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACHighTemperatureRadiant_Impl::fractionofInputConvertedtoRadiantEnergy() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoRadiantEnergy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACHighTemperatureRadiant_Impl::fractionofInputConvertedtoLatentEnergy() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoLatentEnergy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACHighTemperatureRadiant_Impl::fractionofInputthatIsLost() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputthatIsLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACHighTemperatureRadiant_Impl::temperatureControlType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_HighTemperatureRadiantFields::TemperatureControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACHighTemperatureRadiant_Impl::heatingThrottlingRange() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingThrottlingRange,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneHVACHighTemperatureRadiant_Impl::heatingSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingSetpointTemperatureScheduleName);
  }

  double ZoneHVACHighTemperatureRadiant_Impl::fractionofRadiantEnergyIncidentonPeople() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofRadiantEnergyIncidentonPeople,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_HighTemperatureRadiantFields::AvailabilityScheduleName,
                              "ZoneHVACHighTemperatureRadiant",
                              "Availability",
                              schedule);
    return result;
  }

  void ZoneHVACHighTemperatureRadiant_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ZoneHVAC_HighTemperatureRadiantFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setMaximumPowerInput(boost::optional<double> maximumPowerInput) {
    bool result(false);
    if (maximumPowerInput) {
      result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::MaximumPowerInput, maximumPowerInput.get());
    }
    return result;
  }

  void ZoneHVACHighTemperatureRadiant_Impl::autosizeMaximumPowerInput() {
    bool result = setString(OS_ZoneHVAC_HighTemperatureRadiantFields::MaximumPowerInput, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_ZoneHVAC_HighTemperatureRadiantFields::FuelType, fuelType);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setCombustionEfficiency(double combustionEfficiency) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::CombustionEfficiency, combustionEfficiency);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoRadiantEnergy, fractionofInputConvertedtoRadiantEnergy);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoLatentEnergy, fractionofInputConvertedtoLatentEnergy);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setFractionofInputthatIsLost(double fractionofInputthatIsLost) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofInputthatIsLost, fractionofInputthatIsLost);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setTemperatureControlType(std::string temperatureControlType) {
    bool result = setString(OS_ZoneHVAC_HighTemperatureRadiantFields::TemperatureControlType, temperatureControlType);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setHeatingThrottlingRange(double heatingThrottlingRange) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingThrottlingRange, heatingThrottlingRange);
    return result;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setHeatingSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingSetpointTemperatureScheduleName,
                              "ZoneHVACHighTemperatureRadiant",
                              "Heating Setpoint Temperature",
                              schedule);
    return result;
  }

  void ZoneHVACHighTemperatureRadiant_Impl::resetHeatingSetpointTemperatureSchedule() {
    bool result = setString(OS_ZoneHVAC_HighTemperatureRadiantFields::HeatingSetpointTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    bool result = setDouble(OS_ZoneHVAC_HighTemperatureRadiantFields::FractionofRadiantEnergyIncidentonPeople, fractionofRadiantEnergyIncidentonPeople);
    return result;
  }

  unsigned ZoneHVACHighTemperatureRadiant_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  unsigned ZoneHVACHighTemperatureRadiant_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  boost::optional<ThermalZone> ZoneHVACHighTemperatureRadiant_Impl::thermalZone() const
  {
    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
    for( const auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        return thermalZone;
      }
    }
    return boost::none;
  }

  bool ZoneHVACHighTemperatureRadiant_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    if( thermalZone.isPlenum() )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneHVACHighTemperatureRadiant_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

  std::vector<Surface> ZoneHVACHighTemperatureRadiant_Impl::surfaces() const {

    //vector to hold all of the surfaces that this radiant system is attached to
    std::vector<Surface> surfaces;

    //get the thermal zone this equipment belongs to
    if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {

      //loop through all the spaces in this zone
      for (const Space& space : thermalZone->spaces()){

        //loop through all the surfaces in this space
        for (const Surface& surface : space.surfaces()){
          surfaces.push_back(surface);
        }
      }
    }

    return surfaces;
  }

} // detail

ZoneHVACHighTemperatureRadiant::ZoneHVACHighTemperatureRadiant(const Model& model)
  : ZoneHVACComponent(ZoneHVACHighTemperatureRadiant::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>());

  autosizeMaximumPowerInput();
  setFuelType("NaturalGas");
  setCombustionEfficiency(0.90);
  setFractionofInputConvertedtoRadiantEnergy(0.70);
  setFractionofInputConvertedtoLatentEnergy(0.00);
  setFractionofInputthatIsLost(0.00);
  setTemperatureControlType("OperativeTemperature");
  setHeatingThrottlingRange(2.0);
  setFractionofRadiantEnergyIncidentonPeople(0.04);
}

IddObjectType ZoneHVACHighTemperatureRadiant::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant);
}

std::vector<std::string> ZoneHVACHighTemperatureRadiant::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_HighTemperatureRadiantFields::FuelType);
}

std::vector<std::string> ZoneHVACHighTemperatureRadiant::temperatureControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_HighTemperatureRadiantFields::TemperatureControlType);
}

boost::optional<Schedule> ZoneHVACHighTemperatureRadiant::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->availabilitySchedule();
}

boost::optional<double> ZoneHVACHighTemperatureRadiant::maximumPowerInput() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->maximumPowerInput();
}

bool ZoneHVACHighTemperatureRadiant::isMaximumPowerInputAutosized() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->isMaximumPowerInputAutosized();
}

std::string ZoneHVACHighTemperatureRadiant::fuelType() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->fuelType();
}

double ZoneHVACHighTemperatureRadiant::combustionEfficiency() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->combustionEfficiency();
}

double ZoneHVACHighTemperatureRadiant::fractionofInputConvertedtoRadiantEnergy() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->fractionofInputConvertedtoRadiantEnergy();
}

double ZoneHVACHighTemperatureRadiant::fractionofInputConvertedtoLatentEnergy() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->fractionofInputConvertedtoLatentEnergy();
}

double ZoneHVACHighTemperatureRadiant::fractionofInputthatIsLost() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->fractionofInputthatIsLost();
}

std::string ZoneHVACHighTemperatureRadiant::temperatureControlType() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->temperatureControlType();
}

double ZoneHVACHighTemperatureRadiant::heatingThrottlingRange() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->heatingThrottlingRange();
}

boost::optional<Schedule> ZoneHVACHighTemperatureRadiant::heatingSetpointTemperatureSchedule() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->heatingSetpointTemperatureSchedule();
}

double ZoneHVACHighTemperatureRadiant::fractionofRadiantEnergyIncidentonPeople() const {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->fractionofRadiantEnergyIncidentonPeople();
}

bool ZoneHVACHighTemperatureRadiant::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setAvailabilitySchedule(schedule);
}

void ZoneHVACHighTemperatureRadiant::resetAvailabilitySchedule() {
  getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->resetAvailabilitySchedule();
}

bool ZoneHVACHighTemperatureRadiant::setMaximumPowerInput(double maximumPowerInput) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setMaximumPowerInput(maximumPowerInput);
}

void ZoneHVACHighTemperatureRadiant::autosizeMaximumPowerInput() {
  getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->autosizeMaximumPowerInput();
}

bool ZoneHVACHighTemperatureRadiant::setFuelType(std::string fuelType) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setFuelType(fuelType);
}

bool ZoneHVACHighTemperatureRadiant::setCombustionEfficiency(double combustionEfficiency) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setCombustionEfficiency(combustionEfficiency);
}

bool ZoneHVACHighTemperatureRadiant::setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setFractionofInputConvertedtoRadiantEnergy(fractionofInputConvertedtoRadiantEnergy);
}

bool ZoneHVACHighTemperatureRadiant::setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setFractionofInputConvertedtoLatentEnergy(fractionofInputConvertedtoLatentEnergy);
}

bool ZoneHVACHighTemperatureRadiant::setFractionofInputthatIsLost(double fractionofInputthatIsLost) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setFractionofInputthatIsLost(fractionofInputthatIsLost);
}

bool ZoneHVACHighTemperatureRadiant::setTemperatureControlType(std::string temperatureControlType) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setTemperatureControlType(temperatureControlType);
}

bool ZoneHVACHighTemperatureRadiant::setHeatingThrottlingRange(double heatingThrottlingRange) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setHeatingThrottlingRange(heatingThrottlingRange);
}

bool ZoneHVACHighTemperatureRadiant::setHeatingSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setHeatingSetpointTemperatureSchedule(schedule);
}

void ZoneHVACHighTemperatureRadiant::resetHeatingSetpointTemperatureSchedule() {
  getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->resetHeatingSetpointTemperatureSchedule();
}

bool ZoneHVACHighTemperatureRadiant::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->setFractionofRadiantEnergyIncidentonPeople(fractionofRadiantEnergyIncidentonPeople);
}

boost::optional<ThermalZone> ZoneHVACHighTemperatureRadiant::thermalZone() const
{
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->thermalZone();
}

bool ZoneHVACHighTemperatureRadiant::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACHighTemperatureRadiant::removeFromThermalZone()
{
  getImpl<detail::ZoneHVACHighTemperatureRadiant_Impl>()->removeFromThermalZone();
}

/// @cond
ZoneHVACHighTemperatureRadiant::ZoneHVACHighTemperatureRadiant(std::shared_ptr<detail::ZoneHVACHighTemperatureRadiant_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

