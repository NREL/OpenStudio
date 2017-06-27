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

#include "ThermalStorageChilledWaterStratified.hpp"
#include "ThermalStorageChilledWaterStratified_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ThermalStorage_ChilledWater_Stratified_FieldEnums.hxx>

#include "../utilities/time/Time.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermalStorageChilledWaterStratified_Impl::ThermalStorageChilledWaterStratified_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermalStorageChilledWaterStratified::iddObjectType());
  }

  ThermalStorageChilledWaterStratified_Impl::ThermalStorageChilledWaterStratified_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermalStorageChilledWaterStratified::iddObjectType());
  }

  ThermalStorageChilledWaterStratified_Impl::ThermalStorageChilledWaterStratified_Impl(const ThermalStorageChilledWaterStratified_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ThermalStorageChilledWaterStratified_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ThermalStorageChilledWaterStratified_Impl::iddObjectType() const {
    return ThermalStorageChilledWaterStratified::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ThermalStorageChilledWaterStratified_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ThermalStorage_ChilledWater_StratifiedFields::SetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ThermalStorageChilledWaterStratified","Setpoint Temperature"));
    }
    if (std::find(b,e,OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ThermalStorageChilledWaterStratified","Ambient Temperature"));
    }
    if (std::find(b,e,OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ThermalStorageChilledWaterStratified","Use Side Availability"));
    }
    if (std::find(b,e,OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ThermalStorageChilledWaterStratified","Source Side Availability"));
    }
    return result;
  }

  unsigned ThermalStorageChilledWaterStratified_Impl::supplyInletPort()
  {
    return OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideInletNodeName;
  }

  unsigned ThermalStorageChilledWaterStratified_Impl::supplyOutletPort()
  {
    return OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideOutletNodeName;
  }

  unsigned ThermalStorageChilledWaterStratified_Impl::demandInletPort()
  {
    return OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideInletNodeName;
  }

  unsigned ThermalStorageChilledWaterStratified_Impl::demandOutletPort()
  {
    return OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletNodeName;
  }

  double ThermalStorageChilledWaterStratified_Impl::tankVolume() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankVolume,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::tankHeight() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ThermalStorageChilledWaterStratified_Impl::tankShape() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::TankShape,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::tankPerimeter() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankPerimeter,true);
  }

  boost::optional<Schedule> ThermalStorageChilledWaterStratified_Impl::setpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ThermalStorage_ChilledWater_StratifiedFields::SetpointTemperatureScheduleName);
  }

  double ThermalStorageChilledWaterStratified_Impl::deadbandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::DeadbandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::temperatureSensorHeight() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TemperatureSensorHeight,true);
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::minimumTemperatureLimit() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::MinimumTemperatureLimit,true);
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::nominalCoolingCapacity() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::NominalCoolingCapacity,true);
  }

  std::string ThermalStorageChilledWaterStratified_Impl::ambientTemperatureIndicator() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureIndicator,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ThermalStorageChilledWaterStratified_Impl::ambientTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureScheduleName);
  }

  boost::optional<ThermalZone> ThermalStorageChilledWaterStratified_Impl::ambientTemperatureThermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureThermalZoneName);
  }

  boost::optional<std::string> ThermalStorageChilledWaterStratified_Impl::ambientTemperatureOutdoorAirNodeName() const {
    return getString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName,true);
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature,true);
  }

  double ThermalStorageChilledWaterStratified_Impl::useSideHeatTransferEffectiveness() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideHeatTransferEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ThermalStorageChilledWaterStratified_Impl::useSideAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideAvailabilityScheduleName);
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::useSideInletHeight() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight,true);
  }

  bool ThermalStorageChilledWaterStratified_Impl::isUseSideInletHeightAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  double ThermalStorageChilledWaterStratified_Impl::useSideOutletHeight() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideOutletHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::useSideDesignFlowRate() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate,true);
  }

  bool ThermalStorageChilledWaterStratified_Impl::isUseSideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ThermalStorageChilledWaterStratified_Impl::sourceSideHeatTransferEffectiveness() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideHeatTransferEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ThermalStorageChilledWaterStratified_Impl::sourceSideAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideAvailabilityScheduleName);
  }

  double ThermalStorageChilledWaterStratified_Impl::sourceSideInletHeight() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideInletHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::sourceSideOutletHeight() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight,true);
  }

  bool ThermalStorageChilledWaterStratified_Impl::isSourceSideOutletHeightAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  boost::optional<double> ThermalStorageChilledWaterStratified_Impl::sourceSideDesignFlowRate() const {
    return getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate,true);
  }

  bool ThermalStorageChilledWaterStratified_Impl::isSourceSideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ThermalStorageChilledWaterStratified_Impl::tankRecoveryTime() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankRecoveryTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ThermalStorageChilledWaterStratified_Impl::inletMode() const {
    boost::optional<std::string> value = getString(OS_ThermalStorage_ChilledWater_StratifiedFields::InletMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  int ThermalStorageChilledWaterStratified_Impl::numberofNodes() const {
    boost::optional<int> value = getInt(OS_ThermalStorage_ChilledWater_StratifiedFields::NumberofNodes,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::additionalDestratificationConductivity() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::AdditionalDestratificationConductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node1AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node1AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node2AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node2AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node3AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node3AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node4AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node4AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node5AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node5AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node6AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node6AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node7AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node7AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node8AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node8AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node9AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node9AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ThermalStorageChilledWaterStratified_Impl::node10AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node10AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTankVolume(double tankVolume) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankVolume, tankVolume);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTankHeight(double tankHeight) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankHeight, tankHeight);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTankShape(std::string tankShape) {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::TankShape, tankShape);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTankPerimeter(boost::optional<double> tankPerimeter) {
    bool result(false);
    if (tankPerimeter) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankPerimeter, tankPerimeter.get());
    }
    else {
      resetTankPerimeter();
      result = true;
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetTankPerimeter() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::TankPerimeter, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ThermalStorage_ChilledWater_StratifiedFields::SetpointTemperatureScheduleName,
                              "ThermalStorageChilledWaterStratified",
                              "Setpoint Temperature",
                              schedule);
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetSetpointTemperatureSchedule() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::SetpointTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setDeadbandTemperatureDifference(double deadbandTemperatureDifference) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::DeadbandTemperatureDifference, deadbandTemperatureDifference);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTemperatureSensorHeight(boost::optional<double> temperatureSensorHeight) {
    bool result(false);
    if (temperatureSensorHeight) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TemperatureSensorHeight, temperatureSensorHeight.get());
    }
    else {
      resetTemperatureSensorHeight();
      result = true;
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetTemperatureSensorHeight() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::TemperatureSensorHeight, "");
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setMinimumTemperatureLimit(boost::optional<double> minimumTemperatureLimit) {
    bool result(false);
    if (minimumTemperatureLimit) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::MinimumTemperatureLimit, minimumTemperatureLimit.get());
    }
    else {
      resetMinimumTemperatureLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::resetMinimumTemperatureLimit() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::MinimumTemperatureLimit, "");
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNominalCoolingCapacity(boost::optional<double> nominalCoolingCapacity) {
    bool result(false);
    if (nominalCoolingCapacity) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::NominalCoolingCapacity, nominalCoolingCapacity.get());
    }
    else {
      resetNominalCoolingCapacity();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::resetNominalCoolingCapacity() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::NominalCoolingCapacity, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureIndicator, ambientTemperatureIndicator);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setAmbientTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureScheduleName,
                              "ThermalStorageChilledWaterStratified",
                              "Ambient Temperature",
                              schedule);
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetAmbientTemperatureSchedule() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureThermalZoneName, thermalZone.get().handle());
    }
    else {
      resetAmbientTemperatureThermalZone();
      result = true;
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetAmbientTemperatureThermalZone() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureThermalZoneName, "");
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName) {
    bool result(false);
    if (ambientTemperatureOutdoorAirNodeName) {
      result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, ambientTemperatureOutdoorAirNodeName.get());
    }
    else {
      resetAmbientTemperatureOutdoorAirNodeName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::resetAmbientTemperatureOutdoorAirNodeName() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
    bool result(false);
    if (uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, uniformSkinLossCoefficientperUnitAreatoAmbientTemperature.get());
    }
    else {
      resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
      result = true;
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideHeatTransferEffectiveness, useSideHeatTransferEffectiveness);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUseSideAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideAvailabilityScheduleName,
                              "ThermalStorageChilledWaterStratified",
                              "Use Side Availability",
                              schedule);
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetUseSideAvailabilitySchedule() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUseSideInletHeight(boost::optional<double> useSideInletHeight) {
    bool result(false);
    if (useSideInletHeight) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight, useSideInletHeight.get());
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::autocalculateUseSideInletHeight() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUseSideOutletHeight(double useSideOutletHeight) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideOutletHeight, useSideOutletHeight);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate) {
    bool result(false);
    if (useSideDesignFlowRate) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate, useSideDesignFlowRate.get());
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::autosizeUseSideDesignFlowRate() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideHeatTransferEffectiveness, sourceSideHeatTransferEffectiveness);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSourceSideAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideAvailabilityScheduleName,
                              "ThermalStorageChilledWaterStratified",
                              "Source Side Availability",
                              schedule);
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::resetSourceSideAvailabilitySchedule() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSourceSideInletHeight(double sourceSideInletHeight) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideInletHeight, sourceSideInletHeight);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSourceSideOutletHeight(boost::optional<double> sourceSideOutletHeight) {
    bool result(false);
    if (sourceSideOutletHeight) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight, sourceSideOutletHeight.get());
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::autocalculateSourceSideOutletHeight() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate) {
    bool result(false);
    if (sourceSideDesignFlowRate) {
      result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate, sourceSideDesignFlowRate.get());
    }
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::autosizeSourceSideDesignFlowRate() {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ThermalStorageChilledWaterStratified_Impl::setTankRecoveryTime(double tankRecoveryTime) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::TankRecoveryTime, tankRecoveryTime);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setInletMode(std::string inletMode) {
    bool result = setString(OS_ThermalStorage_ChilledWater_StratifiedFields::InletMode, inletMode);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setNumberofNodes(int numberofNodes) {
    bool result = setInt(OS_ThermalStorage_ChilledWater_StratifiedFields::NumberofNodes, numberofNodes);
    return result;
  }

  bool ThermalStorageChilledWaterStratified_Impl::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::AdditionalDestratificationConductivity, additionalDestratificationConductivity);
    return result;
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node1AdditionalLossCoefficient, node1AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node2AdditionalLossCoefficient, node2AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node3AdditionalLossCoefficient, node3AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node4AdditionalLossCoefficient, node4AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node5AdditionalLossCoefficient, node5AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node6AdditionalLossCoefficient, node6AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node7AdditionalLossCoefficient, node7AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node8AdditionalLossCoefficient, node8AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node9AdditionalLossCoefficient, node9AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void ThermalStorageChilledWaterStratified_Impl::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
    bool result = setDouble(OS_ThermalStorage_ChilledWater_StratifiedFields::Node10AdditionalLossCoefficient, node10AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

} // detail

ThermalStorageChilledWaterStratified::ThermalStorageChilledWaterStratified(const Model& model)
  : WaterToWaterComponent(ThermalStorageChilledWaterStratified::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermalStorageChilledWaterStratified_Impl>());

  bool ok = true;
  ok = setTankVolume( 50.0 );
  OS_ASSERT(ok);
  ok = setTankHeight( 8.0 );
  OS_ASSERT(ok);
  ok = setTankShape( "VerticalCylinder" );
  OS_ASSERT(ok);
  ok = setDeadbandTemperatureDifference( 2.5 );
  OS_ASSERT(ok);
  ok = setAmbientTemperatureIndicator( "Outdoors" );
  OS_ASSERT(ok);

  ScheduleRuleset setpoint_schedule_1(model);
  setpoint_schedule_1.defaultDaySchedule().addValue(Time(0,24,0,0),48.89);
  setAmbientTemperatureSchedule(setpoint_schedule_1);

  ok = setUseSideHeatTransferEffectiveness( 1.0 );
  OS_ASSERT(ok);
  autocalculateUseSideInletHeight();
  ok = setUseSideOutletHeight( 0.0 );
  OS_ASSERT(ok);
  autosizeUseSideDesignFlowRate();
  ok = setSourceSideHeatTransferEffectiveness( 1.0 );
  OS_ASSERT(ok);
  ok = setSourceSideInletHeight( 0.0 );
  OS_ASSERT(ok);
  autocalculateSourceSideOutletHeight();
  autosizeSourceSideDesignFlowRate();
  ok = setTankRecoveryTime( 4.0 );
  OS_ASSERT(ok);
  ok = setInletMode( "Fixed" );
  OS_ASSERT(ok);
  ok = setNumberofNodes( 6 );
  OS_ASSERT(ok);
  ok = setAdditionalDestratificationConductivity( 0.0 );
  OS_ASSERT(ok);
  setNode1AdditionalLossCoefficient( 0.0 );
  setNode2AdditionalLossCoefficient( 0.0 );
  setNode3AdditionalLossCoefficient( 0.0 );
  setNode4AdditionalLossCoefficient( 0.0 );
  setNode5AdditionalLossCoefficient( 0.0 );
  setNode6AdditionalLossCoefficient( 0.0 );
  setNode7AdditionalLossCoefficient( 0.0 );
  setNode8AdditionalLossCoefficient( 0.0 );
  setNode9AdditionalLossCoefficient( 0.0 );
  setNode10AdditionalLossCoefficient( 0.0 );
}

IddObjectType ThermalStorageChilledWaterStratified::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ThermalStorage_ChilledWater_Stratified);
}

std::vector<std::string> ThermalStorageChilledWaterStratified::tankShapeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalStorage_ChilledWater_StratifiedFields::TankShape);
}

std::vector<std::string> ThermalStorageChilledWaterStratified::ambientTemperatureIndicatorValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureIndicator);
}

std::vector<std::string> ThermalStorageChilledWaterStratified::inletModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ThermalStorage_ChilledWater_StratifiedFields::InletMode);
}

double ThermalStorageChilledWaterStratified::tankVolume() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->tankVolume();
}

double ThermalStorageChilledWaterStratified::tankHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->tankHeight();
}

std::string ThermalStorageChilledWaterStratified::tankShape() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->tankShape();
}

boost::optional<double> ThermalStorageChilledWaterStratified::tankPerimeter() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->tankPerimeter();
}

boost::optional<Schedule> ThermalStorageChilledWaterStratified::setpointTemperatureSchedule() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setpointTemperatureSchedule();
}

double ThermalStorageChilledWaterStratified::deadbandTemperatureDifference() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->deadbandTemperatureDifference();
}

boost::optional<double> ThermalStorageChilledWaterStratified::temperatureSensorHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->temperatureSensorHeight();
}

boost::optional<double> ThermalStorageChilledWaterStratified::minimumTemperatureLimit() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->minimumTemperatureLimit();
}

boost::optional<double> ThermalStorageChilledWaterStratified::nominalCoolingCapacity() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->nominalCoolingCapacity();
}

std::string ThermalStorageChilledWaterStratified::ambientTemperatureIndicator() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->ambientTemperatureIndicator();
}

boost::optional<Schedule> ThermalStorageChilledWaterStratified::ambientTemperatureSchedule() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->ambientTemperatureSchedule();
}

boost::optional<ThermalZone> ThermalStorageChilledWaterStratified::ambientTemperatureThermalZone() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->ambientTemperatureThermalZone();
}

boost::optional<std::string> ThermalStorageChilledWaterStratified::ambientTemperatureOutdoorAirNodeName() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->ambientTemperatureOutdoorAirNodeName();
}

boost::optional<double> ThermalStorageChilledWaterStratified::uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->uniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
}

double ThermalStorageChilledWaterStratified::useSideHeatTransferEffectiveness() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->useSideHeatTransferEffectiveness();
}

boost::optional<Schedule> ThermalStorageChilledWaterStratified::useSideAvailabilitySchedule() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->useSideAvailabilitySchedule();
}

boost::optional<double> ThermalStorageChilledWaterStratified::useSideInletHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->useSideInletHeight();
}

bool ThermalStorageChilledWaterStratified::isUseSideInletHeightAutocalculated() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->isUseSideInletHeightAutocalculated();
}

double ThermalStorageChilledWaterStratified::useSideOutletHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->useSideOutletHeight();
}

boost::optional<double> ThermalStorageChilledWaterStratified::useSideDesignFlowRate() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->useSideDesignFlowRate();
}

bool ThermalStorageChilledWaterStratified::isUseSideDesignFlowRateAutosized() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->isUseSideDesignFlowRateAutosized();
}

double ThermalStorageChilledWaterStratified::sourceSideHeatTransferEffectiveness() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->sourceSideHeatTransferEffectiveness();
}

boost::optional<Schedule> ThermalStorageChilledWaterStratified::sourceSideAvailabilitySchedule() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->sourceSideAvailabilitySchedule();
}

double ThermalStorageChilledWaterStratified::sourceSideInletHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->sourceSideInletHeight();
}

boost::optional<double> ThermalStorageChilledWaterStratified::sourceSideOutletHeight() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->sourceSideOutletHeight();
}

bool ThermalStorageChilledWaterStratified::isSourceSideOutletHeightAutocalculated() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->isSourceSideOutletHeightAutocalculated();
}

boost::optional<double> ThermalStorageChilledWaterStratified::sourceSideDesignFlowRate() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->sourceSideDesignFlowRate();
}

bool ThermalStorageChilledWaterStratified::isSourceSideDesignFlowRateAutosized() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->isSourceSideDesignFlowRateAutosized();
}

double ThermalStorageChilledWaterStratified::tankRecoveryTime() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->tankRecoveryTime();
}

std::string ThermalStorageChilledWaterStratified::inletMode() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->inletMode();
}

int ThermalStorageChilledWaterStratified::numberofNodes() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->numberofNodes();
}

double ThermalStorageChilledWaterStratified::additionalDestratificationConductivity() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->additionalDestratificationConductivity();
}

double ThermalStorageChilledWaterStratified::node1AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node1AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node2AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node2AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node3AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node3AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node4AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node4AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node5AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node5AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node6AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node6AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node7AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node7AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node8AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node8AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node9AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node9AdditionalLossCoefficient();
}

double ThermalStorageChilledWaterStratified::node10AdditionalLossCoefficient() const {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->node10AdditionalLossCoefficient();
}

bool ThermalStorageChilledWaterStratified::setTankVolume(double tankVolume) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTankVolume(tankVolume);
}

bool ThermalStorageChilledWaterStratified::setTankHeight(double tankHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTankHeight(tankHeight);
}

bool ThermalStorageChilledWaterStratified::setTankShape(std::string tankShape) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTankShape(tankShape);
}

bool ThermalStorageChilledWaterStratified::setTankPerimeter(double tankPerimeter) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTankPerimeter(tankPerimeter);
}

void ThermalStorageChilledWaterStratified::resetTankPerimeter() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetTankPerimeter();
}

bool ThermalStorageChilledWaterStratified::setSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSetpointTemperatureSchedule(schedule);
}

void ThermalStorageChilledWaterStratified::resetSetpointTemperatureSchedule() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetSetpointTemperatureSchedule();
}

bool ThermalStorageChilledWaterStratified::setDeadbandTemperatureDifference(double deadbandTemperatureDifference) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setDeadbandTemperatureDifference(deadbandTemperatureDifference);
}

bool ThermalStorageChilledWaterStratified::setTemperatureSensorHeight(double temperatureSensorHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTemperatureSensorHeight(temperatureSensorHeight);
}

void ThermalStorageChilledWaterStratified::resetTemperatureSensorHeight() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetTemperatureSensorHeight();
}

void ThermalStorageChilledWaterStratified::setMinimumTemperatureLimit(double minimumTemperatureLimit) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setMinimumTemperatureLimit(minimumTemperatureLimit);
}

void ThermalStorageChilledWaterStratified::resetMinimumTemperatureLimit() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetMinimumTemperatureLimit();
}

void ThermalStorageChilledWaterStratified::setNominalCoolingCapacity(double nominalCoolingCapacity) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNominalCoolingCapacity(nominalCoolingCapacity);
}

void ThermalStorageChilledWaterStratified::resetNominalCoolingCapacity() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetNominalCoolingCapacity();
}

bool ThermalStorageChilledWaterStratified::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setAmbientTemperatureIndicator(ambientTemperatureIndicator);
}

bool ThermalStorageChilledWaterStratified::setAmbientTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setAmbientTemperatureSchedule(schedule);
}

void ThermalStorageChilledWaterStratified::resetAmbientTemperatureSchedule() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetAmbientTemperatureSchedule();
}

bool ThermalStorageChilledWaterStratified::setAmbientTemperatureThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setAmbientTemperatureThermalZone(thermalZone);
}

void ThermalStorageChilledWaterStratified::resetAmbientTemperatureThermalZone() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetAmbientTemperatureThermalZone();
}

void ThermalStorageChilledWaterStratified::setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setAmbientTemperatureOutdoorAirNodeName(ambientTemperatureOutdoorAirNodeName);
}

void ThermalStorageChilledWaterStratified::resetAmbientTemperatureOutdoorAirNodeName() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetAmbientTemperatureOutdoorAirNodeName();
}

bool ThermalStorageChilledWaterStratified::setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);
}

void ThermalStorageChilledWaterStratified::resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
}

bool ThermalStorageChilledWaterStratified::setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUseSideHeatTransferEffectiveness(useSideHeatTransferEffectiveness);
}

bool ThermalStorageChilledWaterStratified::setUseSideAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUseSideAvailabilitySchedule(schedule);
}

void ThermalStorageChilledWaterStratified::resetUseSideAvailabilitySchedule() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetUseSideAvailabilitySchedule();
}

bool ThermalStorageChilledWaterStratified::setUseSideInletHeight(double useSideInletHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUseSideInletHeight(useSideInletHeight);
}

void ThermalStorageChilledWaterStratified::autocalculateUseSideInletHeight() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->autocalculateUseSideInletHeight();
}

bool ThermalStorageChilledWaterStratified::setUseSideOutletHeight(double useSideOutletHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUseSideOutletHeight(useSideOutletHeight);
}

bool ThermalStorageChilledWaterStratified::setUseSideDesignFlowRate(double useSideDesignFlowRate) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setUseSideDesignFlowRate(useSideDesignFlowRate);
}

void ThermalStorageChilledWaterStratified::autosizeUseSideDesignFlowRate() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->autosizeUseSideDesignFlowRate();
}

bool ThermalStorageChilledWaterStratified::setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSourceSideHeatTransferEffectiveness(sourceSideHeatTransferEffectiveness);
}

bool ThermalStorageChilledWaterStratified::setSourceSideAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSourceSideAvailabilitySchedule(schedule);
}

void ThermalStorageChilledWaterStratified::resetSourceSideAvailabilitySchedule() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->resetSourceSideAvailabilitySchedule();
}

bool ThermalStorageChilledWaterStratified::setSourceSideInletHeight(double sourceSideInletHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSourceSideInletHeight(sourceSideInletHeight);
}

bool ThermalStorageChilledWaterStratified::setSourceSideOutletHeight(double sourceSideOutletHeight) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSourceSideOutletHeight(sourceSideOutletHeight);
}

void ThermalStorageChilledWaterStratified::autocalculateSourceSideOutletHeight() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->autocalculateSourceSideOutletHeight();
}

bool ThermalStorageChilledWaterStratified::setSourceSideDesignFlowRate(double sourceSideDesignFlowRate) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setSourceSideDesignFlowRate(sourceSideDesignFlowRate);
}

void ThermalStorageChilledWaterStratified::autosizeSourceSideDesignFlowRate() {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->autosizeSourceSideDesignFlowRate();
}

bool ThermalStorageChilledWaterStratified::setTankRecoveryTime(double tankRecoveryTime) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setTankRecoveryTime(tankRecoveryTime);
}

bool ThermalStorageChilledWaterStratified::setInletMode(std::string inletMode) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setInletMode(inletMode);
}

bool ThermalStorageChilledWaterStratified::setNumberofNodes(int numberofNodes) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNumberofNodes(numberofNodes);
}

bool ThermalStorageChilledWaterStratified::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
  return getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setAdditionalDestratificationConductivity(additionalDestratificationConductivity);
}

void ThermalStorageChilledWaterStratified::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode1AdditionalLossCoefficient(node1AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode2AdditionalLossCoefficient(node2AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode3AdditionalLossCoefficient(node3AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode4AdditionalLossCoefficient(node4AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode5AdditionalLossCoefficient(node5AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode6AdditionalLossCoefficient(node6AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode7AdditionalLossCoefficient(node7AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode8AdditionalLossCoefficient(node8AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode9AdditionalLossCoefficient(node9AdditionalLossCoefficient);
}

void ThermalStorageChilledWaterStratified::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
  getImpl<detail::ThermalStorageChilledWaterStratified_Impl>()->setNode10AdditionalLossCoefficient(node10AdditionalLossCoefficient);
}

/// @cond
ThermalStorageChilledWaterStratified::ThermalStorageChilledWaterStratified(std::shared_ptr<detail::ThermalStorageChilledWaterStratified_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

