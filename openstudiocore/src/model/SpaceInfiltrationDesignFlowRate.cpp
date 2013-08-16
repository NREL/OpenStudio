/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>

#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SpaceInfiltration_DesignFlowRate_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SpaceInfiltrationDesignFlowRate_Impl::SpaceInfiltrationDesignFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoad_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SpaceInfiltrationDesignFlowRate::iddObjectType());
  }

  SpaceInfiltrationDesignFlowRate_Impl::SpaceInfiltrationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : SpaceLoad_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SpaceInfiltrationDesignFlowRate::iddObjectType());
  }

  SpaceInfiltrationDesignFlowRate_Impl::SpaceInfiltrationDesignFlowRate_Impl(const SpaceInfiltrationDesignFlowRate_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : SpaceLoad_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SpaceInfiltrationDesignFlowRate_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SpaceInfiltrationDesignFlowRate_Impl::iddObjectType() const {
    return SpaceInfiltrationDesignFlowRate::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SpaceInfiltrationDesignFlowRate_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SpaceInfiltration_DesignFlowRateFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SpaceInfiltrationDesignFlowRate","Infiltration"));
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    boost::optional<double> designFlowRate = this->designFlowRate();
    if (designFlowRate){
      return true;
    }

    boost::optional<double> flowperSpaceFloorArea = this->flowperSpaceFloorArea();
    if (flowperSpaceFloorArea){
      double floorArea = space->floorArea();

      return this->setDesignFlowRate(*flowperSpaceFloorArea * floorArea);
    }

    boost::optional<double> flowperExteriorSurfaceArea = this->flowperExteriorSurfaceArea();
    if (flowperExteriorSurfaceArea){
      double exteriorArea = space->exteriorArea();

      return this->setDesignFlowRate(*flowperExteriorSurfaceArea * exteriorArea);
    }

    boost::optional<double> flowperExteriorWallArea = this->flowperExteriorWallArea();
    if (flowperExteriorWallArea){
      double exteriorWallArea = space->exteriorWallArea();

      return this->setDesignFlowRate(*flowperExteriorWallArea * exteriorWallArea);
    }

    boost::optional<double> airChangesperHour = this->airChangesperHour();
    if (airChangesperHour){
      return this->setDesignFlowRate(*airChangesperHour * space->volume() / 3600.0);
    }

    return false;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  std::string SpaceInfiltrationDesignFlowRate_Impl::designFlowRateCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> SpaceInfiltrationDesignFlowRate_Impl::designFlowRate() const {
    boost::optional<double> result;
    if (istringEqual("Flow/Space", this->designFlowRateCalculationMethod())){
      result = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate,true);
    }
    return result;
  }

  boost::optional<double> SpaceInfiltrationDesignFlowRate_Impl::flowperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Flow/Area", this->designFlowRateCalculationMethod())){
      result = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea,true);
    }
    return result;
  }

  boost::optional<double> SpaceInfiltrationDesignFlowRate_Impl::flowperExteriorSurfaceArea() const {
    boost::optional<double> result;
    if (istringEqual("Flow/ExteriorArea", this->designFlowRateCalculationMethod())){
      result = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea,true);
    }
    return result;  
  }

  boost::optional<double> SpaceInfiltrationDesignFlowRate_Impl::flowperExteriorWallArea() const {
    boost::optional<double> result;
    if (istringEqual("Flow/ExteriorWallArea", this->designFlowRateCalculationMethod())){
      result = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea,true);
    }
    return result;  
  }

  boost::optional<double> SpaceInfiltrationDesignFlowRate_Impl::airChangesperHour() const {
    boost::optional<double> result;
    if (istringEqual("AirChanges/Hour", this->designFlowRateCalculationMethod())){
      result = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour,true);
    }
    return result;  
  }

  double SpaceInfiltrationDesignFlowRate_Impl::constantTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::isConstantTermCoefficientDefaulted() const {
    return isEmpty(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient);
  }

  double SpaceInfiltrationDesignFlowRate_Impl::temperatureTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::isTemperatureTermCoefficientDefaulted() const {
    return isEmpty(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient);
  }

  double SpaceInfiltrationDesignFlowRate_Impl::velocityTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::isVelocityTermCoefficientDefaulted() const {
    return isEmpty(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient);
  }

  double SpaceInfiltrationDesignFlowRate_Impl::velocitySquaredTermCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::isVelocitySquaredTermCoefficientDefaulted() const {
    return isEmpty(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setDesignFlowRate(boost::optional<double> designFlowRate) {
    bool result = true;
    if (designFlowRate) {
      if (*designFlowRate < 0){
        result = false;
      }else{
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Space");
        OS_ASSERT(result);
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, designFlowRate.get());
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Flow/Space", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setFlowperSpaceFloorArea(boost::optional<double> flowperSpaceFloorArea) {
    bool result = true;
    if (flowperSpaceFloorArea) {
      if (*flowperSpaceFloorArea < 0){
        result = false;
      }else{
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Area");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, flowperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Flow/Area", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setFlowperExteriorSurfaceArea(boost::optional<double> flowperExteriorSurfaceArea) {
    bool result = true;
    if (flowperExteriorSurfaceArea) {
      if (*flowperExteriorSurfaceArea < 0){
        result = false;
      }else{
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorArea");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorSurfaceArea.get());
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Flow/ExteriorArea", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setFlowperExteriorWallArea(boost::optional<double> flowperExteriorWallArea) {
    bool result = true;
    if (flowperExteriorWallArea) {
      if (*flowperExteriorWallArea < 0){
        result = false;
      }else{
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorWallArea");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorWallArea.get());
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Flow/ExteriorWallArea", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setAirChangesperHour(boost::optional<double> airChangesperHour) {
    bool result = true;
    if (airChangesperHour) {
      if (*airChangesperHour < 0){
        result = false;
      }else{
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, airChangesperHour.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("AirChanges/Hour", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setConstantTermCoefficient(double constantTermCoefficient) {
    bool result = false;
    result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient, constantTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetConstantTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
    bool result = false;
    result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, temperatureTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetTemperatureTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setVelocityTermCoefficient(double velocityTermCoefficient) {
    bool result = false;
    result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient, velocityTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetVelocityTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
    bool result = false;
    result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, velocitySquaredTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetVelocitySquaredTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, "");
    OS_ASSERT(result);
  }

  int SpaceInfiltrationDesignFlowRate_Impl::spaceIndex() const {
    return OS_SpaceInfiltration_DesignFlowRateFields::SpaceorSpaceTypeName;
  }

  boost::optional<Schedule> SpaceInfiltrationDesignFlowRate_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SpaceInfiltration_DesignFlowRateFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_SpaceInfiltration_DesignFlowRateFields::ScheduleName);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_SpaceInfiltration_DesignFlowRateFields::ScheduleName,
                                                "SpaceInfiltrationDesignFlowRate",
                                                "Infiltration",
                                                schedule);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetSchedule()
  {
    this->setString(OS_SpaceInfiltration_DesignFlowRateFields::ScheduleName, "");
  }

  boost::optional<ModelObject> SpaceInfiltrationDesignFlowRate_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSchedule();
    }
    return true;
  }

} // detail

SpaceInfiltrationDesignFlowRate::SpaceInfiltrationDesignFlowRate(const Model& model)
  : SpaceLoad(SpaceInfiltrationDesignFlowRate::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>());
  bool test = this->setDesignFlowRate(0);
  OS_ASSERT(test);
}

IddObjectType SpaceInfiltrationDesignFlowRate::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
  return result;
}

std::vector<std::string> SpaceInfiltrationDesignFlowRate::designFlowRateCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod);
}

std::vector<std::string> SpaceInfiltrationDesignFlowRate::validDesignFlowRateCalculationMethodValues() {
  return SpaceInfiltrationDesignFlowRate::designFlowRateCalculationMethodValues();
}

std::string SpaceInfiltrationDesignFlowRate::designFlowRateCalculationMethod() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->designFlowRateCalculationMethod();
}

boost::optional<double> SpaceInfiltrationDesignFlowRate::designFlowRate() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->designFlowRate();
}

boost::optional<double> SpaceInfiltrationDesignFlowRate::flowperSpaceFloorArea() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->flowperSpaceFloorArea();
}

boost::optional<double> SpaceInfiltrationDesignFlowRate::flowperExteriorSurfaceArea() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->flowperExteriorSurfaceArea();
}

boost::optional<double> SpaceInfiltrationDesignFlowRate::flowperExteriorWallArea() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->flowperExteriorWallArea();
}

boost::optional<double> SpaceInfiltrationDesignFlowRate::airChangesperHour() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->airChangesperHour();
}

double SpaceInfiltrationDesignFlowRate::constantTermCoefficient() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->constantTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::isConstantTermCoefficientDefaulted() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->isConstantTermCoefficientDefaulted();
}

double SpaceInfiltrationDesignFlowRate::temperatureTermCoefficient() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->temperatureTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::isTemperatureTermCoefficientDefaulted() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->isTemperatureTermCoefficientDefaulted();
}

double SpaceInfiltrationDesignFlowRate::velocityTermCoefficient() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->velocityTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::isVelocityTermCoefficientDefaulted() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->isVelocityTermCoefficientDefaulted();
}

double SpaceInfiltrationDesignFlowRate::velocitySquaredTermCoefficient() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->velocitySquaredTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::isVelocitySquaredTermCoefficientDefaulted() const {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->isVelocitySquaredTermCoefficientDefaulted();
}

bool SpaceInfiltrationDesignFlowRate::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setDesignFlowRate(designFlowRate);
}

bool SpaceInfiltrationDesignFlowRate::setFlowperSpaceFloorArea(double flowperSpaceFloorArea) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setFlowperSpaceFloorArea(flowperSpaceFloorArea);
}

bool SpaceInfiltrationDesignFlowRate::setFlowperExteriorSurfaceArea(double flowperExteriorSurfaceArea) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setFlowperExteriorSurfaceArea(flowperExteriorSurfaceArea);
}

bool SpaceInfiltrationDesignFlowRate::setFlowperExteriorWallArea(double flowperExteriorWallArea) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setFlowperExteriorWallArea(flowperExteriorWallArea);
}

bool SpaceInfiltrationDesignFlowRate::setAirChangesperHour(double airChangesperHour) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setAirChangesperHour(airChangesperHour);
}

bool SpaceInfiltrationDesignFlowRate::setConstantTermCoefficient(double constantTermCoefficient) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setConstantTermCoefficient(constantTermCoefficient);
}

void SpaceInfiltrationDesignFlowRate::resetConstantTermCoefficient() {
  getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->resetConstantTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setTemperatureTermCoefficient(temperatureTermCoefficient);
}

void SpaceInfiltrationDesignFlowRate::resetTemperatureTermCoefficient() {
  getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->resetTemperatureTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::setVelocityTermCoefficient(double velocityTermCoefficient) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setVelocityTermCoefficient(velocityTermCoefficient);
}

void SpaceInfiltrationDesignFlowRate::resetVelocityTermCoefficient() {
  getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->resetVelocityTermCoefficient();
}

bool SpaceInfiltrationDesignFlowRate::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setVelocitySquaredTermCoefficient(velocitySquaredTermCoefficient);
}

void SpaceInfiltrationDesignFlowRate::resetVelocitySquaredTermCoefficient() {
  getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->resetVelocitySquaredTermCoefficient();
}

boost::optional<Schedule> SpaceInfiltrationDesignFlowRate::schedule() const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->schedule();
}

bool SpaceInfiltrationDesignFlowRate::isScheduleDefaulted() const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->isScheduleDefaulted();
}

bool SpaceInfiltrationDesignFlowRate::setSchedule(Schedule& schedule)
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->setSchedule(schedule);
}

void SpaceInfiltrationDesignFlowRate::resetSchedule()
{
  getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->resetSchedule();
}

/// @cond
SpaceInfiltrationDesignFlowRate::SpaceInfiltrationDesignFlowRate(boost::shared_ptr<detail::SpaceInfiltrationDesignFlowRate_Impl> impl)
  : SpaceLoad(impl)
{}
/// @endcond


} // model
} // openstudio

