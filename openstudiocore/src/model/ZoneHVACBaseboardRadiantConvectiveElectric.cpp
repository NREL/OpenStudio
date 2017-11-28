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

#include "ZoneHVACBaseboardRadiantConvectiveElectric.hpp"
#include "ZoneHVACBaseboardRadiantConvectiveElectric_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Space.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_Baseboard_RadiantConvective_Electric_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACBaseboardRadiantConvectiveElectric_Impl::ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const IdfObject& idfObject,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveElectric::iddObjectType());
  }

  ZoneHVACBaseboardRadiantConvectiveElectric_Impl::ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardRadiantConvectiveElectric::iddObjectType());
  }

  ZoneHVACBaseboardRadiantConvectiveElectric_Impl::ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const ZoneHVACBaseboardRadiantConvectiveElectric_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACBaseboardRadiantConvectiveElectric_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACBaseboardRadiantConvectiveElectric_Impl::iddObjectType() const {
    return ZoneHVACBaseboardRadiantConvectiveElectric::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACBaseboardRadiantConvectiveElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACBaseboardRadiantConvectiveElectric","Availability"));
    }
    return result;
  }

  unsigned ZoneHVACBaseboardRadiantConvectiveElectric_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  unsigned ZoneHVACBaseboardRadiantConvectiveElectric_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveElectric_Impl::thermalZone() const
  {
    ModelObject thisObject = this->getObject<ModelObject>();
    auto const thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
    for( auto const & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        return thermalZone;
      }
    }
    return boost::none;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m || thermalZone.isPlenum() )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneHVACBaseboardRadiantConvectiveElectric_Impl::removeFromThermalZone()
  {
    if ( auto thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

  std::vector<Surface> ZoneHVACBaseboardRadiantConvectiveElectric_Impl::surfaces() const {

    //vector to hold all of the surfaces that this radiant system is attached to
    std::vector<Surface> surfaces;

    //get the thermal zone this equipment belongs to
    if (auto const thermalZone = this->thermalZone()) {

      //loop through all the spaces in this zone
      for (auto const & space : thermalZone->spaces()){

        //loop through all the surfaces in this space
        for (auto const & surface : space.surfaces()){
          surfaces.push_back(surface);
        }
      }
    }

    return surfaces;
  }

  Schedule ZoneHVACBaseboardRadiantConvectiveElectric_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  std::string ZoneHVACBaseboardRadiantConvectiveElectric_Impl::heatingDesignCapacityMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacityMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACBaseboardRadiantConvectiveElectric_Impl::heatingDesignCapacity() const {
    return getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacity,true);
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::isHeatingDesignCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACBaseboardRadiantConvectiveElectric_Impl::heatingDesignCapacityPerFloorArea() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacityPerFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveElectric_Impl::fractionofAutosizedHeatingDesignCapacity() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionofAutosizedHeatingDesignCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveElectric_Impl::efficiency() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::Efficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveElectric_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACBaseboardRadiantConvectiveElectric_Impl::fractionofRadiantEnergyIncidentonPeople() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionofRadiantEnergyIncidentonPeople,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::AvailabilityScheduleName,
                              "ZoneHVACBaseboardRadiantConvectiveElectric",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
    bool result = setString(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
    bool result(false);
    if (heatingDesignCapacity) {
      result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacity, heatingDesignCapacity.get());
    }
    return result;
  }

  void ZoneHVACBaseboardRadiantConvectiveElectric_Impl::autosizeHeatingDesignCapacity() {
    bool result = setString(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setEfficiency(double efficiency) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::Efficiency, efficiency);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionRadiant, fractionRadiant);
    return result;
  }

  bool ZoneHVACBaseboardRadiantConvectiveElectric_Impl::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
    bool result = setDouble(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::FractionofRadiantEnergyIncidentonPeople, fractionofRadiantEnergyIncidentonPeople);
    return result;
  }

  boost::optional<Schedule> ZoneHVACBaseboardRadiantConvectiveElectric_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::AvailabilityScheduleName);
  }

} // detail

ZoneHVACBaseboardRadiantConvectiveElectric::ZoneHVACBaseboardRadiantConvectiveElectric(const Model& model)
  : ZoneHVACComponent(ZoneHVACBaseboardRadiantConvectiveElectric::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>());

  bool ok = true;
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setHeatingDesignCapacityMethod( "HeatingDesignCapacity" );
  OS_ASSERT(ok);
  autosizeHeatingDesignCapacity();
  ok = setHeatingDesignCapacityPerFloorArea( 0 );
  OS_ASSERT(ok);
  ok = setFractionofAutosizedHeatingDesignCapacity( 1.0 );
  OS_ASSERT(ok);
  ok = setEfficiency( 1.0 );
  OS_ASSERT(ok);
  ok = setFractionRadiant( 0.2 );
  OS_ASSERT(ok);
  ok = setFractionofRadiantEnergyIncidentonPeople( 0.3 );
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACBaseboardRadiantConvectiveElectric::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric);
}

std::vector<std::string> ZoneHVACBaseboardRadiantConvectiveElectric::heatingDesignCapacityMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_Baseboard_RadiantConvective_ElectricFields::HeatingDesignCapacityMethod);
}

Schedule ZoneHVACBaseboardRadiantConvectiveElectric::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->availabilitySchedule();
}

std::string ZoneHVACBaseboardRadiantConvectiveElectric::heatingDesignCapacityMethod() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->heatingDesignCapacityMethod();
}

boost::optional<double> ZoneHVACBaseboardRadiantConvectiveElectric::heatingDesignCapacity() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->heatingDesignCapacity();
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::isHeatingDesignCapacityAutosized() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->isHeatingDesignCapacityAutosized();
}

double ZoneHVACBaseboardRadiantConvectiveElectric::heatingDesignCapacityPerFloorArea() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->heatingDesignCapacityPerFloorArea();
}

double ZoneHVACBaseboardRadiantConvectiveElectric::fractionofAutosizedHeatingDesignCapacity() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->fractionofAutosizedHeatingDesignCapacity();
}

double ZoneHVACBaseboardRadiantConvectiveElectric::efficiency() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->efficiency();
}

double ZoneHVACBaseboardRadiantConvectiveElectric::fractionRadiant() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->fractionRadiant();
}

double ZoneHVACBaseboardRadiantConvectiveElectric::fractionofRadiantEnergyIncidentonPeople() const {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->fractionofRadiantEnergyIncidentonPeople();
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setHeatingDesignCapacity(double heatingDesignCapacity) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
}

void ZoneHVACBaseboardRadiantConvectiveElectric::autosizeHeatingDesignCapacity() {
  getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->autosizeHeatingDesignCapacity();
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setFractionofAutosizedHeatingDesignCapacity(fractionofAutosizedHeatingDesignCapacity);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setEfficiency(double efficiency) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setEfficiency(efficiency);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setFractionRadiant(fractionRadiant);
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->setFractionofRadiantEnergyIncidentonPeople(fractionofRadiantEnergyIncidentonPeople);
}

boost::optional<ThermalZone> ZoneHVACBaseboardRadiantConvectiveElectric::thermalZone() {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->thermalZone();
}

bool ZoneHVACBaseboardRadiantConvectiveElectric::addToThermalZone(ThermalZone & thermalZone) {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACBaseboardRadiantConvectiveElectric::removeFromThermalZone() {
  return getImpl<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl>()->removeFromThermalZone();
}

/// @cond
ZoneHVACBaseboardRadiantConvectiveElectric::ZoneHVACBaseboardRadiantConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

