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

#include "SpaceInfiltrationDesignFlowRate.hpp"
#include "SpaceInfiltrationDesignFlowRate_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SpaceInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/QuantityConverter.hpp"

#include "../utilities/core/Assert.hpp"

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

  bool SpaceInfiltrationDesignFlowRate_Impl::isAbsolute() const
  {
    if (designFlowRate()){
      return true;
    }
    return false;
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
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, designFlowRate.get());
        if (result) {
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Space");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
          OS_ASSERT(result);
        }
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
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, flowperSpaceFloorArea.get());
        if (result) {
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Area");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
          OS_ASSERT(result);
        }
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
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorSurfaceArea.get());
        if (result) {
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorArea");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
          OS_ASSERT(result);
        }
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
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, flowperExteriorWallArea.get());
        if (result) {
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/ExteriorWallArea");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, "");
          OS_ASSERT(result);
        }
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
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, airChangesperHour.get());
        if (result) {
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::DesignFlowRate, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperSpaceFloorArea, "");
          OS_ASSERT(result);
          result = setString(OS_SpaceInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, "");
          OS_ASSERT(result);
        }
      }
    } else {
      if (istringEqual("AirChanges/Hour", this->designFlowRateCalculationMethod())){
        result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::AirChangesperHour, 0.0);
      }
    }
    return result;
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setConstantTermCoefficient(double constantTermCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient, constantTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetConstantTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::ConstantTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setTemperatureTermCoefficient(double temperatureTermCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, temperatureTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetTemperatureTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setVelocityTermCoefficient(double velocityTermCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient, velocityTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetVelocityTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::VelocityTermCoefficient, "");
    OS_ASSERT(result);
  }

  bool SpaceInfiltrationDesignFlowRate_Impl::setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, velocitySquaredTermCoefficient);
    return result;
  }

  void SpaceInfiltrationDesignFlowRate_Impl::resetVelocitySquaredTermCoefficient() {
    bool result = setString(OS_SpaceInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, "");
    OS_ASSERT(result);
  }

  double SpaceInfiltrationDesignFlowRate_Impl::getDesignFlowRate(double floorArea,
                                                                 double exteriorSurfaceArea,
                                                                 double exteriorWallArea,
                                                                 double airVolume) const
  {
    std::string method = designFlowRateCalculationMethod();

    if (method == "Flow/Space") {
      return designFlowRate().get();
    }
    else if (method == "Flow/Area") {
      return flowperSpaceFloorArea().get() * floorArea;
    }
    else if (method == "Flow/ExteriorArea") {
      return flowperExteriorSurfaceArea().get() * exteriorSurfaceArea;
    }
    else if (method == "Flow/ExteriorWallArea") {
      return flowperExteriorWallArea().get() * exteriorWallArea;
    }
    else if (method == "AirChanges/Hour") {
      return convert(airChangesperHour().get() * airVolume,"m^3/h","m^3/s").get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SpaceInfiltrationDesignFlowRate_Impl::getFlowPerSpaceFloorArea(double floorArea,
                                                                        double exteriorSurfaceArea,
                                                                        double exteriorWallArea,
                                                                        double airVolume) const
  {
    std::string method = designFlowRateCalculationMethod();

    if (method == "Flow/Space") {
      return designFlowRate().get() / floorArea;
    }
    else if (method == "Flow/Area") {
      return flowperSpaceFloorArea().get();
    }
    else if (method == "Flow/ExteriorArea") {
      return flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / floorArea);
    }
    else if (method == "Flow/ExteriorWallArea") {
      return flowperExteriorWallArea().get() * (exteriorWallArea / floorArea);
    }
    else if (method == "AirChanges/Hour") {
      return convert(airChangesperHour().get() * (airVolume / floorArea),"m/h","m/s").get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SpaceInfiltrationDesignFlowRate_Impl::getFlowPerExteriorSurfaceArea(double floorArea,
                                                                             double exteriorSurfaceArea,
                                                                             double exteriorWallArea,
                                                                             double airVolume) const
  {
    std::string method = designFlowRateCalculationMethod();

    if (method == "Flow/Space") {
      return designFlowRate().get() / exteriorSurfaceArea;
    }
    else if (method == "Flow/Area") {
      return flowperSpaceFloorArea().get() * (floorArea / exteriorSurfaceArea);
    }
    else if (method == "Flow/ExteriorArea") {
      return flowperExteriorSurfaceArea().get();
    }
    else if (method == "Flow/ExteriorWallArea") {
      return flowperExteriorWallArea().get() * (exteriorWallArea / exteriorSurfaceArea);
    }
    else if (method == "AirChanges/Hour") {
      return convert(airChangesperHour().get() * (airVolume / exteriorSurfaceArea),"m/h","m/s").get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SpaceInfiltrationDesignFlowRate_Impl::getFlowPerExteriorWallArea(double floorArea,
                                                                          double exteriorSurfaceArea,
                                                                          double exteriorWallArea,
                                                                          double airVolume) const
  {
    std::string method = designFlowRateCalculationMethod();

    if (method == "Flow/Space") {
      return designFlowRate().get() / exteriorWallArea;
    }
    else if (method == "Flow/Area") {
      return flowperSpaceFloorArea().get() * (floorArea / exteriorWallArea);
    }
    else if (method == "Flow/ExteriorArea") {
      return flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / exteriorWallArea);
    }
    else if (method == "Flow/ExteriorWallArea") {
      return flowperExteriorWallArea().get();
    }
    else if (method == "AirChanges/Hour") {
      return convert(airChangesperHour().get() * (airVolume / exteriorWallArea),"m/h","m/s").get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double SpaceInfiltrationDesignFlowRate_Impl::getAirChangesPerHour(double floorArea,
                                                                    double exteriorSurfaceArea,
                                                                    double exteriorWallArea,
                                                                    double airVolume) const
  {
    std::string method = designFlowRateCalculationMethod();

    if (method == "Flow/Space") {
      return convert(designFlowRate().get() / airVolume, "1/s", "1/h").get();
    }
    else if (method == "Flow/Area") {
      return convert(flowperSpaceFloorArea().get() * (floorArea / airVolume), "1/s", "1/h").get();
    }
    else if (method == "Flow/ExteriorArea") {
      return convert(flowperExteriorSurfaceArea().get() * (exteriorSurfaceArea / airVolume), "1/s", "1/h").get();
    }
    else if (method == "Flow/ExteriorWallArea") {
      return convert(flowperExteriorWallArea().get() * (exteriorWallArea / airVolume), "1/s", "1/h").get();
    }
    else if (method == "AirChanges/Hour") {
      return airChangesperHour().get();
    }

    OS_ASSERT(false);
    return 0.0;
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

double SpaceInfiltrationDesignFlowRate::getDesignFlowRate(double floorArea,
                                                          double exteriorSurfaceArea,
                                                          double exteriorWallArea,
                                                          double airVolume) const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->getDesignFlowRate(floorArea,
                                                                                    exteriorSurfaceArea,
                                                                                    exteriorWallArea,
                                                                                    airVolume);
}

double SpaceInfiltrationDesignFlowRate::getFlowPerSpaceFloorArea(double floorArea,
                                                                 double exteriorSurfaceArea,
                                                                 double exteriorWallArea,
                                                                 double airVolume) const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->getFlowPerSpaceFloorArea(floorArea,
                                                                                           exteriorSurfaceArea,
                                                                                           exteriorWallArea,
                                                                                           airVolume);
}

double SpaceInfiltrationDesignFlowRate::getFlowPerExteriorSurfaceArea(double floorArea,
                                                                      double exteriorSurfaceArea,
                                                                      double exteriorWallArea,
                                                                      double airVolume) const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->getFlowPerExteriorSurfaceArea(floorArea,
                                                                                                exteriorSurfaceArea,
                                                                                                exteriorWallArea,
                                                                                                airVolume);
}

double SpaceInfiltrationDesignFlowRate::getFlowPerExteriorWallArea(double floorArea,
                                                                   double exteriorSurfaceArea,
                                                                   double exteriorWallArea,
                                                                   double airVolume) const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->getFlowPerExteriorWallArea(floorArea,
                                                                                             exteriorSurfaceArea,
                                                                                             exteriorWallArea,
                                                                                             airVolume);
}

double SpaceInfiltrationDesignFlowRate::getAirChangesPerHour(double floorArea,
                                                             double exteriorSurfaceArea,
                                                             double exteriorWallArea,
                                                             double airVolume) const
{
  return getImpl<detail::SpaceInfiltrationDesignFlowRate_Impl>()->getAirChangesPerHour(floorArea,
                                                                                       exteriorSurfaceArea,
                                                                                       exteriorWallArea,
                                                                                       airVolume);
}

/// @cond
SpaceInfiltrationDesignFlowRate::SpaceInfiltrationDesignFlowRate(std::shared_ptr<detail::SpaceInfiltrationDesignFlowRate_Impl> impl)
  : SpaceLoad(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

