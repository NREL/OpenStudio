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
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardConvectiveElectric::iddObjectType());
  }

  ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardConvectiveElectric::iddObjectType());
  }

  ZoneHVACBaseboardConvectiveElectric_Impl::ZoneHVACBaseboardConvectiveElectric_Impl(const ZoneHVACBaseboardConvectiveElectric_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACBaseboardConvectiveElectric_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACBaseboardConvectiveElectric_Impl::iddObjectType() const {
    return ZoneHVACBaseboardConvectiveElectric::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACBaseboardConvectiveElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACBaseboardConvectiveElectric","Availability"));
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
    return getDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity,true);
  }

  OSOptionalQuantity ZoneHVACBaseboardConvectiveElectric_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    return getQuantityFromDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, value, returnIP);
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
    boost::optional<double> value = getDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACBaseboardConvectiveElectric_Impl::getEfficiency(bool returnIP) const {
    OptionalDouble value = efficiency();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACBaseboardConvectiveElectric_Impl::isEfficiencyDefaulted() const {
    return isEmpty(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency);
  }

  bool ZoneHVACBaseboardConvectiveElectric_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule,
                              "ZoneHVACBaseboardConvectiveElectric",
                              "Availability",
                              schedule);
    return result;
  }

  void ZoneHVACBaseboardConvectiveElectric_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity, nominalCapacity.get());
    }
    OS_ASSERT(result);
  }

  bool ZoneHVACBaseboardConvectiveElectric_Impl::setNominalCapacity(const OSOptionalQuantity& nominalCapacity) {
    bool result(false);
    OptionalDouble value;
    if (nominalCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::NominalCapacity,nominalCapacity.get());
      if (value) {
        setNominalCapacity(value);
        result = true;
      }
    }
    else {
      setNominalCapacity(value);
      result = true;
    }
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

  bool ZoneHVACBaseboardConvectiveElectric_Impl::setEfficiency(const Quantity& efficiency) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency,efficiency);
    if (!value) {
      return false;
    }
    return setEfficiency(value.get());
  }

  void ZoneHVACBaseboardConvectiveElectric_Impl::resetEfficiency() {
    bool result = setString(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACBaseboardConvectiveElectric_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilitySchedule);
  }

  openstudio::OSOptionalQuantity ZoneHVACBaseboardConvectiveElectric_Impl::nominalCapacity_SI() const {
    return getNominalCapacity(false);
  }

  openstudio::OSOptionalQuantity ZoneHVACBaseboardConvectiveElectric_Impl::nominalCapacity_IP() const {
    return getNominalCapacity(true);
  }

  openstudio::Quantity ZoneHVACBaseboardConvectiveElectric_Impl::efficiency_SI() const {
    return getEfficiency(false);
  }

  openstudio::Quantity ZoneHVACBaseboardConvectiveElectric_Impl::efficiency_IP() const {
    return getEfficiency(true);
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

  unsigned ZoneHVACBaseboardConvectiveElectric_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  unsigned ZoneHVACBaseboardConvectiveElectric_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveElectric_Impl::thermalZone()
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

  bool ZoneHVACBaseboardConvectiveElectric_Impl::addToThermalZone(ThermalZone & thermalZone)
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

  void ZoneHVACBaseboardConvectiveElectric_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

} // detail

ZoneHVACBaseboardConvectiveElectric::ZoneHVACBaseboardConvectiveElectric(const Model& model)
  : ZoneHVACComponent(ZoneHVACBaseboardConvectiveElectric::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>());

  autosizeNominalCapacity();

  Schedule schedule = model.alwaysOnDiscreteSchedule();

  setAvailabilitySchedule(schedule);

  setEfficiency(1.0);
}

IddObjectType ZoneHVACBaseboardConvectiveElectric::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric);
}

Schedule ZoneHVACBaseboardConvectiveElectric::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->availabilitySchedule();
}

boost::optional<double> ZoneHVACBaseboardConvectiveElectric::nominalCapacity() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->nominalCapacity();
}

OSOptionalQuantity ZoneHVACBaseboardConvectiveElectric::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->getNominalCapacity(returnIP);
}

bool ZoneHVACBaseboardConvectiveElectric::isNominalCapacityAutosized() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->isNominalCapacityAutosized();
}

double ZoneHVACBaseboardConvectiveElectric::efficiency() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->efficiency();
}

Quantity ZoneHVACBaseboardConvectiveElectric::getEfficiency(bool returnIP) const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->getEfficiency(returnIP);
}

bool ZoneHVACBaseboardConvectiveElectric::isEfficiencyDefaulted() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->isEfficiencyDefaulted();
}

bool ZoneHVACBaseboardConvectiveElectric::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setAvailabilitySchedule(schedule);
}

void ZoneHVACBaseboardConvectiveElectric::setNominalCapacity(double nominalCapacity) {
  getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setNominalCapacity(nominalCapacity);
}

bool ZoneHVACBaseboardConvectiveElectric::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setNominalCapacity(nominalCapacity);
}

void ZoneHVACBaseboardConvectiveElectric::autosizeNominalCapacity() {
  getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->autosizeNominalCapacity();
}

bool ZoneHVACBaseboardConvectiveElectric::setEfficiency(double efficiency) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setEfficiency(efficiency);
}

bool ZoneHVACBaseboardConvectiveElectric::setEfficiency(const Quantity& efficiency) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->setEfficiency(efficiency);
}

void ZoneHVACBaseboardConvectiveElectric::resetEfficiency() {
  getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->resetEfficiency();
}

boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveElectric::thermalZone()
{
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->thermalZone();
}

bool ZoneHVACBaseboardConvectiveElectric::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACBaseboardConvectiveElectric::removeFromThermalZone()
{
  getImpl<detail::ZoneHVACBaseboardConvectiveElectric_Impl>()->removeFromThermalZone();
}

/// @cond
ZoneHVACBaseboardConvectiveElectric::ZoneHVACBaseboardConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveElectric_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

