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

#include "ZoneVentilationDesignFlowRate.hpp"
#include "ZoneVentilationDesignFlowRate_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "ZoneHVACEquipmentList.hpp"
#include "ZoneHVACEquipmentList_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneVentilationDesignFlowRate_Impl::ZoneVentilationDesignFlowRate_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneVentilationDesignFlowRate::iddObjectType());
  }

  ZoneVentilationDesignFlowRate_Impl::ZoneVentilationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneVentilationDesignFlowRate::iddObjectType());
  }

  ZoneVentilationDesignFlowRate_Impl::ZoneVentilationDesignFlowRate_Impl(const ZoneVentilationDesignFlowRate_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneVentilationDesignFlowRate_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneVentilationDesignFlowRate_Impl::iddObjectType() const {
    return ZoneVentilationDesignFlowRate::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneVentilationDesignFlowRate_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Zone Ventilation Design Flow Rate"));
    }
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Minimum Indoor Temperature"));
    }
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Maximum Indoor Temperature"));
    }
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Delta Temperature"));
    }
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Minimum Outdoor Temperature"));
    }
    if (std::find(b,e,OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationDesignFlowRate","Maximum Outdoor Temperature"));
    }
    return result;
  }

  Schedule ZoneVentilationDesignFlowRate_Impl::schedule() const {
    boost::optional<Schedule> value = optionalSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
    }
    return value.get();
  }

  std::string ZoneVentilationDesignFlowRate_Impl::designFlowRateCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::designFlowRate() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::DesignFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::flowRateperZoneFloorArea() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::flowRateperPerson() const {
    auto value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::FlowRateperPerson,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::airChangesperHour() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::AirChangesperHour,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneVentilationDesignFlowRate_Impl::ventilationType() const {
    boost::optional<std::string> value = getString(OS_ZoneVentilation_DesignFlowRateFields::VentilationType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::fanPressureRise() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::FanPressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::fanTotalEfficiency() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::constantTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::temperatureTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::velocityTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::velocitySquaredTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneVentilationDesignFlowRate_Impl::minimumIndoorTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::minimumIndoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName);
  }

  double ZoneVentilationDesignFlowRate_Impl::maximumIndoorTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::maximumIndoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName);
  }

  double ZoneVentilationDesignFlowRate_Impl::deltaTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::deltaTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName);
  }

  double ZoneVentilationDesignFlowRate_Impl::minimumOutdoorTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::minimumOutdoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName);
  }

  double ZoneVentilationDesignFlowRate_Impl::maximumOutdoorTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::maximumOutdoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName);
  }

  double ZoneVentilationDesignFlowRate_Impl::maximumWindSpeed() const {
    boost::optional<double> value = getDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneVentilationDesignFlowRate_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::ScheduleName,
                                                "ZoneVentilationDesignFlowRate",
                                                "Zone Ventilation Design Flow Rate",
                                                schedule);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod) {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, designFlowRateCalculationMethod);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setDesignFlowRate(double designFlowRate) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::DesignFlowRate, designFlowRate);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea, flowRateperZoneFloorArea);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setFlowRateperPerson(double flowRateperPerson) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, flowRateperPerson);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setAirChangesperHour(double airChangesperHour) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::AirChangesperHour, airChangesperHour);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setVentilationType(std::string ventilationType) {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::VentilationType, ventilationType);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setFanPressureRise(double fanPressureRise) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::FanPressureRise, fanPressureRise);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency, fanTotalEfficiency);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::setConstantTermCoefficient(double constantTermCoefficient) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient, constantTermCoefficient);
    OS_ASSERT(result);
  }

  void ZoneVentilationDesignFlowRate_Impl::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient, temperatureTermCoefficient);
    OS_ASSERT(result);
  }

  void ZoneVentilationDesignFlowRate_Impl::setVelocityTermCoefficient(double velocityTermCoefficient) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient, velocityTermCoefficient);
    OS_ASSERT(result);
  }

  void ZoneVentilationDesignFlowRate_Impl::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, velocitySquaredTermCoefficient);
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMinimumIndoorTemperature(double minimumIndoorTemperature) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature, minimumIndoorTemperature);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMinimumIndoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName,
                              "ZoneVentilationDesignFlowRate",
                              "Minimum Indoor Temperature",
                              schedule);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::resetMinimumIndoorTemperatureSchedule() {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMaximumIndoorTemperature(double maximumIndoorTemperature) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature, maximumIndoorTemperature);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMaximumIndoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName,
                              "ZoneVentilationDesignFlowRate",
                              "Maximum Indoor Temperature",
                              schedule);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::resetMaximumIndoorTemperatureSchedule() {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setDeltaTemperature(double deltaTemperature) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperature, deltaTemperature);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setDeltaTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName,
                              "ZoneVentilationDesignFlowRate",
                              "Delta Temperature",
                              schedule);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::resetDeltaTemperatureSchedule() {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature, minimumOutdoorTemperature);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName,
                              "ZoneVentilationDesignFlowRate",
                              "Minimum Outdoor Temperature",
                              schedule);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::resetMinimumOutdoorTemperatureSchedule() {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature, maximumOutdoorTemperature);
    return result;
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName,
                              "ZoneVentilationDesignFlowRate",
                              "Maximum Outdoor Temperature",
                              schedule);
    return result;
  }

  void ZoneVentilationDesignFlowRate_Impl::resetMaximumOutdoorTemperatureSchedule() {
    bool result = setString(OS_ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneVentilationDesignFlowRate_Impl::setMaximumWindSpeed(double maximumWindSpeed) {
    bool result = setDouble(OS_ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed, maximumWindSpeed);
    return result;
  }

  boost::optional<Schedule> ZoneVentilationDesignFlowRate_Impl::optionalSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneVentilation_DesignFlowRateFields::ScheduleName);
  }

  unsigned ZoneVentilationDesignFlowRate_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  unsigned ZoneVentilationDesignFlowRate_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  boost::optional<ThermalZone> ZoneVentilationDesignFlowRate_Impl::thermalZone()
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

  bool ZoneVentilationDesignFlowRate_Impl::addToThermalZone(ThermalZone & thermalZone)
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

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneVentilationDesignFlowRate_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

} // detail

ZoneVentilationDesignFlowRate::ZoneVentilationDesignFlowRate(const Model& model)
  : ZoneHVACComponent(ZoneVentilationDesignFlowRate::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneVentilationDesignFlowRate_Impl>());

  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setSchedule(schedule);
  }

  setDesignFlowRateCalculationMethod("AirChanges/Hour");
  setDesignFlowRate(0.0);
  setFlowRateperZoneFloorArea(0.0);
  setFlowRateperPerson(0.0);
  setAirChangesperHour(5.0);
  setVentilationType("Natural");
  setFanPressureRise(0.0);
  setFanTotalEfficiency(1.0);
  setConstantTermCoefficient(0.6060000);
  setTemperatureTermCoefficient(0.03636);
  setVelocityTermCoefficient(0.1177);
  setVelocitySquaredTermCoefficient(0);
  setMinimumIndoorTemperature(18.0);
  setMaximumIndoorTemperature(100.0);
  setDeltaTemperature(1.0);
  setMinimumOutdoorTemperature(-100.0);
  setMaximumOutdoorTemperature(100.0);
  setMaximumWindSpeed(40.0);
}

IddObjectType ZoneVentilationDesignFlowRate::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneVentilation_DesignFlowRate);
}

std::vector<std::string> ZoneVentilationDesignFlowRate::designFlowRateCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod);
}

std::vector<std::string> ZoneVentilationDesignFlowRate::ventilationTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneVentilation_DesignFlowRateFields::VentilationType);
}

Schedule ZoneVentilationDesignFlowRate::schedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->schedule();
}

std::string ZoneVentilationDesignFlowRate::designFlowRateCalculationMethod() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->designFlowRateCalculationMethod();
}

double ZoneVentilationDesignFlowRate::designFlowRate() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->designFlowRate();
}

double ZoneVentilationDesignFlowRate::flowRateperZoneFloorArea() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->flowRateperZoneFloorArea();
}

double ZoneVentilationDesignFlowRate::flowRateperPerson() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->flowRateperPerson();
}

double ZoneVentilationDesignFlowRate::airChangesperHour() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->airChangesperHour();
}

std::string ZoneVentilationDesignFlowRate::ventilationType() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->ventilationType();
}

double ZoneVentilationDesignFlowRate::fanPressureRise() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->fanPressureRise();
}

double ZoneVentilationDesignFlowRate::fanTotalEfficiency() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->fanTotalEfficiency();
}

double ZoneVentilationDesignFlowRate::constantTermCoefficient() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->constantTermCoefficient();
}

double ZoneVentilationDesignFlowRate::temperatureTermCoefficient() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->temperatureTermCoefficient();
}

double ZoneVentilationDesignFlowRate::velocityTermCoefficient() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->velocityTermCoefficient();
}

double ZoneVentilationDesignFlowRate::velocitySquaredTermCoefficient() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->velocitySquaredTermCoefficient();
}

double ZoneVentilationDesignFlowRate::minimumIndoorTemperature() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->minimumIndoorTemperature();
}

boost::optional<Schedule> ZoneVentilationDesignFlowRate::minimumIndoorTemperatureSchedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->minimumIndoorTemperatureSchedule();
}

double ZoneVentilationDesignFlowRate::maximumIndoorTemperature() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->maximumIndoorTemperature();
}

boost::optional<Schedule> ZoneVentilationDesignFlowRate::maximumIndoorTemperatureSchedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->maximumIndoorTemperatureSchedule();
}

double ZoneVentilationDesignFlowRate::deltaTemperature() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->deltaTemperature();
}

boost::optional<Schedule> ZoneVentilationDesignFlowRate::deltaTemperatureSchedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->deltaTemperatureSchedule();
}

double ZoneVentilationDesignFlowRate::minimumOutdoorTemperature() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->minimumOutdoorTemperature();
}

boost::optional<Schedule> ZoneVentilationDesignFlowRate::minimumOutdoorTemperatureSchedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->minimumOutdoorTemperatureSchedule();
}

double ZoneVentilationDesignFlowRate::maximumOutdoorTemperature() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->maximumOutdoorTemperature();
}

boost::optional<Schedule> ZoneVentilationDesignFlowRate::maximumOutdoorTemperatureSchedule() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->maximumOutdoorTemperatureSchedule();
}

double ZoneVentilationDesignFlowRate::maximumWindSpeed() const {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->maximumWindSpeed();
}

bool ZoneVentilationDesignFlowRate::setSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setSchedule(schedule);
}

bool ZoneVentilationDesignFlowRate::setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setDesignFlowRateCalculationMethod(designFlowRateCalculationMethod);
}

bool ZoneVentilationDesignFlowRate::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setDesignFlowRate(designFlowRate);
}

bool ZoneVentilationDesignFlowRate::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setFlowRateperZoneFloorArea(flowRateperZoneFloorArea);
}

bool ZoneVentilationDesignFlowRate::setFlowRateperPerson(double flowRateperPerson) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setFlowRateperPerson(flowRateperPerson);
}

bool ZoneVentilationDesignFlowRate::setAirChangesperHour(double airChangesperHour) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setAirChangesperHour(airChangesperHour);
}

bool ZoneVentilationDesignFlowRate::setVentilationType(std::string ventilationType) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setVentilationType(ventilationType);
}

bool ZoneVentilationDesignFlowRate::setFanPressureRise(double fanPressureRise) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setFanPressureRise(fanPressureRise);
}

bool ZoneVentilationDesignFlowRate::setFanTotalEfficiency(double fanTotalEfficiency) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
}

void ZoneVentilationDesignFlowRate::setConstantTermCoefficient(double constantTermCoefficient) {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setConstantTermCoefficient(constantTermCoefficient);
}

void ZoneVentilationDesignFlowRate::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setTemperatureTermCoefficient(temperatureTermCoefficient);
}

void ZoneVentilationDesignFlowRate::setVelocityTermCoefficient(double velocityTermCoefficient) {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setVelocityTermCoefficient(velocityTermCoefficient);
}

void ZoneVentilationDesignFlowRate::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setVelocitySquaredTermCoefficient(velocitySquaredTermCoefficient);
}

bool ZoneVentilationDesignFlowRate::setMinimumIndoorTemperature(double minimumIndoorTemperature) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMinimumIndoorTemperature(minimumIndoorTemperature);
}

bool ZoneVentilationDesignFlowRate::setMinimumIndoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMinimumIndoorTemperatureSchedule(schedule);
}

void ZoneVentilationDesignFlowRate::resetMinimumIndoorTemperatureSchedule() {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->resetMinimumIndoorTemperatureSchedule();
}

bool ZoneVentilationDesignFlowRate::setMaximumIndoorTemperature(double maximumIndoorTemperature) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMaximumIndoorTemperature(maximumIndoorTemperature);
}

bool ZoneVentilationDesignFlowRate::setMaximumIndoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMaximumIndoorTemperatureSchedule(schedule);
}

void ZoneVentilationDesignFlowRate::resetMaximumIndoorTemperatureSchedule() {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->resetMaximumIndoorTemperatureSchedule();
}

bool ZoneVentilationDesignFlowRate::setDeltaTemperature(double deltaTemperature) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setDeltaTemperature(deltaTemperature);
}

bool ZoneVentilationDesignFlowRate::setDeltaTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setDeltaTemperatureSchedule(schedule);
}

void ZoneVentilationDesignFlowRate::resetDeltaTemperatureSchedule() {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->resetDeltaTemperatureSchedule();
}

bool ZoneVentilationDesignFlowRate::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMinimumOutdoorTemperature(minimumOutdoorTemperature);
}

bool ZoneVentilationDesignFlowRate::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMinimumOutdoorTemperatureSchedule(schedule);
}

void ZoneVentilationDesignFlowRate::resetMinimumOutdoorTemperatureSchedule() {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->resetMinimumOutdoorTemperatureSchedule();
}

bool ZoneVentilationDesignFlowRate::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMaximumOutdoorTemperature(maximumOutdoorTemperature);
}

bool ZoneVentilationDesignFlowRate::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMaximumOutdoorTemperatureSchedule(schedule);
}

void ZoneVentilationDesignFlowRate::resetMaximumOutdoorTemperatureSchedule() {
  getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->resetMaximumOutdoorTemperatureSchedule();
}

bool ZoneVentilationDesignFlowRate::setMaximumWindSpeed(double maximumWindSpeed) {
  return getImpl<detail::ZoneVentilationDesignFlowRate_Impl>()->setMaximumWindSpeed(maximumWindSpeed);
}

/// @cond
ZoneVentilationDesignFlowRate::ZoneVentilationDesignFlowRate(std::shared_ptr<detail::ZoneVentilationDesignFlowRate_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

