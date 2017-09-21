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

#include "RefrigerationAirChiller.hpp"
#include "RefrigerationAirChiller_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
// #include "CurveLinear.hpp"
// #include "CurveLinear_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_AirChiller_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationAirChiller_Impl::RefrigerationAirChiller_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationAirChiller::iddObjectType());
  }

  RefrigerationAirChiller_Impl::RefrigerationAirChiller_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationAirChiller::iddObjectType());
  }

  RefrigerationAirChiller_Impl::RefrigerationAirChiller_Impl(const RefrigerationAirChiller_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationAirChiller_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationAirChiller_Impl::iddObjectType() const {
    return RefrigerationAirChiller::iddObjectType();
  }

  std::vector<ScheduleTypeKey> RefrigerationAirChiller_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_AirChillerFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationAirChiller","Availability"));
    }
    if (std::find(b,e,OS_Refrigeration_AirChillerFields::HeatingPowerScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationAirChiller","Heating Power"));
    }
    if (std::find(b,e,OS_Refrigeration_AirChillerFields::DefrostScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationAirChiller","Defrost"));
    }
    if (std::find(b,e,OS_Refrigeration_AirChillerFields::DefrostDripDownScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationAirChiller","Defrost Drip-Down"));
    }
    return result;
  }

  ModelObject RefrigerationAirChiller_Impl::clone(Model model) const
  {
    RefrigerationAirChiller airChillerClone = ZoneHVACComponent_Impl::clone(model).cast<RefrigerationAirChiller>();
    return airChillerClone;
  }

  std::vector<IdfObject> RefrigerationAirChiller_Impl::remove()
  {
    return ZoneHVACComponent_Impl::remove();
  }

  // std::vector<ModelObject> RefrigerationAirChiller_Impl::children() const
  // {
  //   std::vector<ModelObject> result;
  //   if (boost::optional<CurveLinear> intermediate = capacityCorrectionCurve()) {
  //     result.push_back(*intermediate);
  //   }
  //   return result;
  // }

  unsigned RefrigerationAirChiller_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  unsigned RefrigerationAirChiller_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

  boost::optional<ThermalZone> RefrigerationAirChiller_Impl::thermalZone()
  {
    Model m = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = m.getConcreteModelObjects<ThermalZone>();
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

  bool RefrigerationAirChiller_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    removeFromThermalZone();
    thermalZone.setUseIdealAirLoads(false);
    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void RefrigerationAirChiller_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

  boost::optional<Schedule> RefrigerationAirChiller_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_AirChillerFields::AvailabilityScheduleName);
  }

  std::string RefrigerationAirChiller_Impl::capacityRatingType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::CapacityRatingType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> RefrigerationAirChiller_Impl::ratedUnitLoadFactor() const {
    return getDouble(OS_Refrigeration_AirChillerFields::RatedUnitLoadFactor,true);
  }

  boost::optional<double> RefrigerationAirChiller_Impl::ratedCapacity() const {
    return getDouble(OS_Refrigeration_AirChillerFields::RatedCapacity,true);
  }

  double RefrigerationAirChiller_Impl::ratedRelativeHumidity() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedRelativeHumidity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isRatedRelativeHumidityDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::RatedRelativeHumidity);
  }

  double RefrigerationAirChiller_Impl::ratedCoolingSourceTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedCoolingSourceTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationAirChiller_Impl::ratedTemperatureDifferenceDT1() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedTemperatureDifferenceDT1,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> RefrigerationAirChiller_Impl::maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const {
    return getDouble(OS_Refrigeration_AirChillerFields::MaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature,true);
  }

  double RefrigerationAirChiller_Impl::coilMaterialCorrectionFactor() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::CoilMaterialCorrectionFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isCoilMaterialCorrectionFactorDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::CoilMaterialCorrectionFactor);
  }

  double RefrigerationAirChiller_Impl::refrigerantCorrectionFactor() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RefrigerantCorrectionFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isRefrigerantCorrectionFactorDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::RefrigerantCorrectionFactor);
  }

  // std::string RefrigerationAirChiller_Impl::capacityCorrectionCurveType() const {
  //   boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveType,true);
  //   OS_ASSERT(value);
  //   return value.get();
  // }

  // bool RefrigerationAirChiller_Impl::isCapacityCorrectionCurveTypeDefaulted() const {
  //   return isEmpty(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveType);
  // }

  // boost::optional<CurveLinear> RefrigerationAirChiller_Impl::capacityCorrectionCurve() const {
  //   return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveName);
  // }

  double RefrigerationAirChiller_Impl::sHR60CorrectionFactor() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::SHR60CorrectionFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isSHR60CorrectionFactorDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::SHR60CorrectionFactor);
  }

  double RefrigerationAirChiller_Impl::ratedTotalHeatingPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedTotalHeatingPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> RefrigerationAirChiller_Impl::heatingPowerSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_AirChillerFields::HeatingPowerScheduleName);
  }

  std::string RefrigerationAirChiller_Impl::fanSpeedControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::FanSpeedControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isFanSpeedControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::FanSpeedControlType);
  }

  double RefrigerationAirChiller_Impl::ratedFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isRatedFanPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::RatedFanPower);
  }

  double RefrigerationAirChiller_Impl::ratedAirFlow() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::RatedAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationAirChiller_Impl::minimumFanAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::MinimumFanAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isMinimumFanAirFlowRatioDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::MinimumFanAirFlowRatio);
  }

  std::string RefrigerationAirChiller_Impl::defrostType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::DefrostType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isDefrostTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::DefrostType);
  }

  std::string RefrigerationAirChiller_Impl::defrostControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::DefrostControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isDefrostControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::DefrostControlType);
  }

  Schedule RefrigerationAirChiller_Impl::defrostSchedule() const {
    boost::optional<Schedule> value = optionalDefrostSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Defrost Schedule attached.");
    }
    return value.get();
  }

  boost::optional<Schedule> RefrigerationAirChiller_Impl::defrostDripDownSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_AirChillerFields::DefrostDripDownScheduleName);
  }

  boost::optional<double> RefrigerationAirChiller_Impl::defrostPower() const {
    return getDouble(OS_Refrigeration_AirChillerFields::DefrostPower,true);
  }

  boost::optional<double> RefrigerationAirChiller_Impl::temperatureTerminationDefrostFractiontoIce() const {
    return getDouble(OS_Refrigeration_AirChillerFields::TemperatureTerminationDefrostFractiontoIce,true);
  }

  std::string RefrigerationAirChiller_Impl::verticalLocation() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_AirChillerFields::VerticalLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isVerticalLocationDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::VerticalLocation);
  }

  double RefrigerationAirChiller_Impl::averageRefrigerantChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_AirChillerFields::AverageRefrigerantChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationAirChiller_Impl::isAverageRefrigerantChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_AirChillerFields::AverageRefrigerantChargeInventory);
  }

  bool RefrigerationAirChiller_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_AirChillerFields::AvailabilityScheduleName,
                              "RefrigerationAirChiller",
                              "Availability",
                              schedule);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Refrigeration_AirChillerFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setCapacityRatingType(std::string capacityRatingType) {
    bool result = setString(OS_Refrigeration_AirChillerFields::CapacityRatingType, capacityRatingType);
    return result;
  }

  void RefrigerationAirChiller_Impl::setRatedUnitLoadFactor(boost::optional<double> ratedUnitLoadFactor) {
    bool result(false);
    if (ratedUnitLoadFactor) {
      result = setDouble(OS_Refrigeration_AirChillerFields::RatedUnitLoadFactor, ratedUnitLoadFactor.get());
    }
    else {
      resetRatedUnitLoadFactor();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::resetRatedUnitLoadFactor() {
    bool result = setString(OS_Refrigeration_AirChillerFields::RatedUnitLoadFactor, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setRatedCapacity(boost::optional<double> ratedCapacity) {
    bool result(false);
    if (ratedCapacity) {
      result = setDouble(OS_Refrigeration_AirChillerFields::RatedCapacity, ratedCapacity.get());
    }
    else {
      resetRatedCapacity();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::resetRatedCapacity() {
    bool result = setString(OS_Refrigeration_AirChillerFields::RatedCapacity, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setRatedRelativeHumidity(double ratedRelativeHumidity) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedRelativeHumidity, ratedRelativeHumidity);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetRatedRelativeHumidity() {
    bool result = setString(OS_Refrigeration_AirChillerFields::RatedRelativeHumidity, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedCoolingSourceTemperature, ratedCoolingSourceTemperature);
    return result;
  }

  bool RefrigerationAirChiller_Impl::setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedTemperatureDifferenceDT1, ratedTemperatureDifferenceDT1);
    return result;
  }

  bool RefrigerationAirChiller_Impl::setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature) {
    bool result(false);
    if (maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature) {
      result = setDouble(OS_Refrigeration_AirChillerFields::MaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature, maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature.get());
    }
    else {
      resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();
      result = true;
    }
    return result;
  }

  void RefrigerationAirChiller_Impl::resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() {
    bool result = setString(OS_Refrigeration_AirChillerFields::MaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::CoilMaterialCorrectionFactor, coilMaterialCorrectionFactor);
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::resetCoilMaterialCorrectionFactor() {
    bool result = setString(OS_Refrigeration_AirChillerFields::CoilMaterialCorrectionFactor, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RefrigerantCorrectionFactor, refrigerantCorrectionFactor);
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::resetRefrigerantCorrectionFactor() {
    bool result = setString(OS_Refrigeration_AirChillerFields::RefrigerantCorrectionFactor, "");
    OS_ASSERT(result);
  }

  // bool RefrigerationAirChiller_Impl::setCapacityCorrectionCurveType(std::string capacityCorrectionCurveType) {
  //   bool result = setString(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveType, capacityCorrectionCurveType);
  //   return result;
  // }

  // void RefrigerationAirChiller_Impl::resetCapacityCorrectionCurveType() {
  //   bool result = setString(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveType, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationAirChiller_Impl::setCapacityCorrectionCurve(const boost::optional<CurveLinear>& curveLinear) {
  //   bool result(false);
  //   if (curveLinear) {
  //     result = setPointer(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveName, curveLinear.get().handle());
  //   }
  //   else {
  //     resetCapacityCorrectionCurve();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationAirChiller_Impl::resetCapacityCorrectionCurve() {
  //   bool result = setString(OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveName, "");
  //   OS_ASSERT(result);
  // }

  bool RefrigerationAirChiller_Impl::setSHR60CorrectionFactor(double sHR60CorrectionFactor) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::SHR60CorrectionFactor, sHR60CorrectionFactor);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetSHR60CorrectionFactor() {
    bool result = setString(OS_Refrigeration_AirChillerFields::SHR60CorrectionFactor, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setRatedTotalHeatingPower(double ratedTotalHeatingPower) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedTotalHeatingPower, ratedTotalHeatingPower);
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setHeatingPowerSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_AirChillerFields::HeatingPowerScheduleName,
                              "RefrigerationAirChiller",
                              "Heating Power",
                              schedule);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetHeatingPowerSchedule() {
    bool result = setString(OS_Refrigeration_AirChillerFields::HeatingPowerScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setFanSpeedControlType(std::string fanSpeedControlType) {
    bool result = setString(OS_Refrigeration_AirChillerFields::FanSpeedControlType, fanSpeedControlType);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetFanSpeedControlType() {
    bool result = setString(OS_Refrigeration_AirChillerFields::FanSpeedControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setRatedFanPower(double ratedFanPower) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedFanPower, ratedFanPower);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetRatedFanPower() {
    bool result = setString(OS_Refrigeration_AirChillerFields::RatedFanPower, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setRatedAirFlow(double ratedAirFlow) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::RatedAirFlow, ratedAirFlow);
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::MinimumFanAirFlowRatio, minimumFanAirFlowRatio);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetMinimumFanAirFlowRatio() {
    bool result = setString(OS_Refrigeration_AirChillerFields::MinimumFanAirFlowRatio, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setDefrostType(std::string defrostType) {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostType, defrostType);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetDefrostType() {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setDefrostControlType(std::string defrostControlType) {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostControlType, defrostControlType);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetDefrostControlType() {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setDefrostSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_AirChillerFields::DefrostScheduleName,
                              "RefrigerationAirChiller",
                              "Defrost",
                              schedule);
    return result;
  }

  bool RefrigerationAirChiller_Impl::setDefrostDripDownSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_AirChillerFields::DefrostDripDownScheduleName,
                              "RefrigerationAirChiller",
                              "Defrost Drip-Down",
                              schedule);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetDefrostDripDownSchedule() {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostDripDownScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setDefrostPower(boost::optional<double> defrostPower) {
    bool result(false);
    if (defrostPower) {
      result = setDouble(OS_Refrigeration_AirChillerFields::DefrostPower, defrostPower.get());
    }
    else {
      resetDefrostPower();
      result = true;
    }
    return result;
  }

  void RefrigerationAirChiller_Impl::resetDefrostPower() {
    bool result = setString(OS_Refrigeration_AirChillerFields::DefrostPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce) {
    bool result(false);
    if (temperatureTerminationDefrostFractiontoIce) {
      result = setDouble(OS_Refrigeration_AirChillerFields::TemperatureTerminationDefrostFractiontoIce, temperatureTerminationDefrostFractiontoIce.get());
    }
    else {
      resetTemperatureTerminationDefrostFractiontoIce();
      result = true;
    }
    return result;
  }

  void RefrigerationAirChiller_Impl::resetTemperatureTerminationDefrostFractiontoIce() {
    bool result = setString(OS_Refrigeration_AirChillerFields::TemperatureTerminationDefrostFractiontoIce, "");
    OS_ASSERT(result);
  }

  bool RefrigerationAirChiller_Impl::setVerticalLocation(std::string verticalLocation) {
    bool result = setString(OS_Refrigeration_AirChillerFields::VerticalLocation, verticalLocation);
    return result;
  }

  void RefrigerationAirChiller_Impl::resetVerticalLocation() {
    bool result = setString(OS_Refrigeration_AirChillerFields::VerticalLocation, "");
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
    bool result = setDouble(OS_Refrigeration_AirChillerFields::AverageRefrigerantChargeInventory, averageRefrigerantChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationAirChiller_Impl::resetAverageRefrigerantChargeInventory() {
    bool result = setString(OS_Refrigeration_AirChillerFields::AverageRefrigerantChargeInventory, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> RefrigerationAirChiller_Impl::optionalDefrostSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_AirChillerFields::DefrostScheduleName);
  }

} // detail

RefrigerationAirChiller::RefrigerationAirChiller(const Model& model, Schedule& defrostSchedule)
  : ZoneHVACComponent(RefrigerationAirChiller::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationAirChiller_Impl>());

  bool ok = setCapacityRatingType("UnitLoadFactorSensibleOnly");
  OS_ASSERT(ok);
  setRatedUnitLoadFactor(15000.0);
  ok = setRatedCoolingSourceTemperature(-30.0);
  OS_ASSERT(ok);
  ok = setRatedTemperatureDifferenceDT1(5.6);
  OS_ASSERT(ok);
  ok = setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(7.3);
  OS_ASSERT(ok);
  setCoilMaterialCorrectionFactor(1.0);
  setRefrigerantCorrectionFactor(1.0);
  ok = setSHR60CorrectionFactor(1.5);
  OS_ASSERT(ok);
  setRatedTotalHeatingPower(275.0);
  ok = setFanSpeedControlType("Fixed");
  OS_ASSERT(ok);
  ok = setRatedFanPower(1500.0);
  OS_ASSERT(ok);
  setRatedAirFlow(20.0);
  ok = setMinimumFanAirFlowRatio(0.2);
  OS_ASSERT(ok);
  ok = setDefrostType("Electric");
  OS_ASSERT(ok);
  ok = setDefrostControlType("TimeSchedule");
  OS_ASSERT(ok);
  ok = setDefrostSchedule(defrostSchedule);
  OS_ASSERT(ok);
  ok = setDefrostPower(75000.0);
  OS_ASSERT(ok);
  ok = setTemperatureTerminationDefrostFractiontoIce(0.7);
  OS_ASSERT(ok);
  ok = getImpl<detail::RefrigerationAirChiller_Impl>()->setVerticalLocation("Middle");
  OS_ASSERT(ok);
}

IddObjectType RefrigerationAirChiller::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_AirChiller);
}

std::vector<std::string> RefrigerationAirChiller::capacityRatingTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_AirChillerFields::CapacityRatingType);
}

// std::vector<std::string> RefrigerationAirChiller::capacityCorrectionCurveTypeValues() {
//   return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
//                         OS_Refrigeration_AirChillerFields::CapacityCorrectionCurveType);
// }

std::vector<std::string> RefrigerationAirChiller::fanSpeedControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_AirChillerFields::FanSpeedControlType);
}

std::vector<std::string> RefrigerationAirChiller::defrostTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_AirChillerFields::DefrostType);
}

std::vector<std::string> RefrigerationAirChiller::defrostControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_AirChillerFields::DefrostControlType);
}

std::vector<std::string> RefrigerationAirChiller::verticalLocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_AirChillerFields::VerticalLocation);
}

boost::optional<Schedule> RefrigerationAirChiller::availabilitySchedule() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->availabilitySchedule();
}

std::string RefrigerationAirChiller::capacityRatingType() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->capacityRatingType();
}

boost::optional<double> RefrigerationAirChiller::ratedUnitLoadFactor() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedUnitLoadFactor();
}

boost::optional<double> RefrigerationAirChiller::ratedCapacity() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedCapacity();
}

double RefrigerationAirChiller::ratedRelativeHumidity() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedRelativeHumidity();
}

bool RefrigerationAirChiller::isRatedRelativeHumidityDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isRatedRelativeHumidityDefaulted();
}

double RefrigerationAirChiller::ratedCoolingSourceTemperature() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedCoolingSourceTemperature();
}

double RefrigerationAirChiller::ratedTemperatureDifferenceDT1() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedTemperatureDifferenceDT1();
}

boost::optional<double> RefrigerationAirChiller::maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();
}

double RefrigerationAirChiller::coilMaterialCorrectionFactor() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->coilMaterialCorrectionFactor();
}

bool RefrigerationAirChiller::isCoilMaterialCorrectionFactorDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isCoilMaterialCorrectionFactorDefaulted();
}

double RefrigerationAirChiller::refrigerantCorrectionFactor() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->refrigerantCorrectionFactor();
}

bool RefrigerationAirChiller::isRefrigerantCorrectionFactorDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isRefrigerantCorrectionFactorDefaulted();
}

// std::string RefrigerationAirChiller::capacityCorrectionCurveType() const {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->capacityCorrectionCurveType();
// }

// bool RefrigerationAirChiller::isCapacityCorrectionCurveTypeDefaulted() const {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->isCapacityCorrectionCurveTypeDefaulted();
// }

// boost::optional<CurveLinear> RefrigerationAirChiller::capacityCorrectionCurve() const {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->capacityCorrectionCurve();
// }

double RefrigerationAirChiller::sHR60CorrectionFactor() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->sHR60CorrectionFactor();
}

bool RefrigerationAirChiller::isSHR60CorrectionFactorDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isSHR60CorrectionFactorDefaulted();
}

double RefrigerationAirChiller::ratedTotalHeatingPower() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedTotalHeatingPower();
}

boost::optional<Schedule> RefrigerationAirChiller::heatingPowerSchedule() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->heatingPowerSchedule();
}

std::string RefrigerationAirChiller::fanSpeedControlType() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->fanSpeedControlType();
}

bool RefrigerationAirChiller::isFanSpeedControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isFanSpeedControlTypeDefaulted();
}

double RefrigerationAirChiller::ratedFanPower() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedFanPower();
}

bool RefrigerationAirChiller::isRatedFanPowerDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isRatedFanPowerDefaulted();
}

double RefrigerationAirChiller::ratedAirFlow() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->ratedAirFlow();
}

double RefrigerationAirChiller::minimumFanAirFlowRatio() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->minimumFanAirFlowRatio();
}

bool RefrigerationAirChiller::isMinimumFanAirFlowRatioDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isMinimumFanAirFlowRatioDefaulted();
}

std::string RefrigerationAirChiller::defrostType() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->defrostType();
}

bool RefrigerationAirChiller::isDefrostTypeDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isDefrostTypeDefaulted();
}

std::string RefrigerationAirChiller::defrostControlType() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->defrostControlType();
}

bool RefrigerationAirChiller::isDefrostControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isDefrostControlTypeDefaulted();
}

Schedule RefrigerationAirChiller::defrostSchedule() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->defrostSchedule();
}

boost::optional<Schedule> RefrigerationAirChiller::defrostDripDownSchedule() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->defrostDripDownSchedule();
}

boost::optional<double> RefrigerationAirChiller::defrostPower() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->defrostPower();
}

boost::optional<double> RefrigerationAirChiller::temperatureTerminationDefrostFractiontoIce() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->temperatureTerminationDefrostFractiontoIce();
}

std::string RefrigerationAirChiller::verticalLocation() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->verticalLocation();
}

bool RefrigerationAirChiller::isVerticalLocationDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isVerticalLocationDefaulted();
}

double RefrigerationAirChiller::averageRefrigerantChargeInventory() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->averageRefrigerantChargeInventory();
}

bool RefrigerationAirChiller::isAverageRefrigerantChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->isAverageRefrigerantChargeInventoryDefaulted();
}

bool RefrigerationAirChiller::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setAvailabilitySchedule(schedule);
}

void RefrigerationAirChiller::resetAvailabilitySchedule() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetAvailabilitySchedule();
}

bool RefrigerationAirChiller::setCapacityRatingType(std::string capacityRatingType) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setCapacityRatingType(capacityRatingType);
}

void RefrigerationAirChiller::setRatedUnitLoadFactor(double ratedUnitLoadFactor) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedUnitLoadFactor(ratedUnitLoadFactor);
}

void RefrigerationAirChiller::resetRatedUnitLoadFactor() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetRatedUnitLoadFactor();
}

void RefrigerationAirChiller::setRatedCapacity(double ratedCapacity) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedCapacity(ratedCapacity);
}

void RefrigerationAirChiller::resetRatedCapacity() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetRatedCapacity();
}

bool RefrigerationAirChiller::setRatedRelativeHumidity(double ratedRelativeHumidity) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedRelativeHumidity(ratedRelativeHumidity);
}

void RefrigerationAirChiller::resetRatedRelativeHumidity() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetRatedRelativeHumidity();
}

bool RefrigerationAirChiller::setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedCoolingSourceTemperature(ratedCoolingSourceTemperature);
}

bool RefrigerationAirChiller::setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedTemperatureDifferenceDT1(ratedTemperatureDifferenceDT1);
}

bool RefrigerationAirChiller::setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(double maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature);
}

void RefrigerationAirChiller::resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();
}

void RefrigerationAirChiller::setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setCoilMaterialCorrectionFactor(coilMaterialCorrectionFactor);
}

void RefrigerationAirChiller::resetCoilMaterialCorrectionFactor() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetCoilMaterialCorrectionFactor();
}

void RefrigerationAirChiller::setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setRefrigerantCorrectionFactor(refrigerantCorrectionFactor);
}

void RefrigerationAirChiller::resetRefrigerantCorrectionFactor() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetRefrigerantCorrectionFactor();
}

// bool RefrigerationAirChiller::setCapacityCorrectionCurveType(std::string capacityCorrectionCurveType) {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->setCapacityCorrectionCurveType(capacityCorrectionCurveType);
// }

// void RefrigerationAirChiller::resetCapacityCorrectionCurveType() {
//   getImpl<detail::RefrigerationAirChiller_Impl>()->resetCapacityCorrectionCurveType();
// }

// bool RefrigerationAirChiller::setCapacityCorrectionCurve(const CurveLinear& curveLinear) {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->setCapacityCorrectionCurve(curveLinear);
// }

// void RefrigerationAirChiller::resetCapacityCorrectionCurve() {
//   getImpl<detail::RefrigerationAirChiller_Impl>()->resetCapacityCorrectionCurve();
// }

bool RefrigerationAirChiller::setSHR60CorrectionFactor(double sHR60CorrectionFactor) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setSHR60CorrectionFactor(sHR60CorrectionFactor);
}

void RefrigerationAirChiller::resetSHR60CorrectionFactor() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetSHR60CorrectionFactor();
}

void RefrigerationAirChiller::setRatedTotalHeatingPower(double ratedTotalHeatingPower) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedTotalHeatingPower(ratedTotalHeatingPower);
}

bool RefrigerationAirChiller::setHeatingPowerSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setHeatingPowerSchedule(schedule);
}

void RefrigerationAirChiller::resetHeatingPowerSchedule() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetHeatingPowerSchedule();
}

bool RefrigerationAirChiller::setFanSpeedControlType(std::string fanSpeedControlType) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setFanSpeedControlType(fanSpeedControlType);
}

void RefrigerationAirChiller::resetFanSpeedControlType() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetFanSpeedControlType();
}

bool RefrigerationAirChiller::setRatedFanPower(double ratedFanPower) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedFanPower(ratedFanPower);
}

void RefrigerationAirChiller::resetRatedFanPower() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetRatedFanPower();
}

void RefrigerationAirChiller::setRatedAirFlow(double ratedAirFlow) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setRatedAirFlow(ratedAirFlow);
}

bool RefrigerationAirChiller::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setMinimumFanAirFlowRatio(minimumFanAirFlowRatio);
}

void RefrigerationAirChiller::resetMinimumFanAirFlowRatio() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetMinimumFanAirFlowRatio();
}

bool RefrigerationAirChiller::setDefrostType(std::string defrostType) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setDefrostType(defrostType);
}

void RefrigerationAirChiller::resetDefrostType() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetDefrostType();
}

bool RefrigerationAirChiller::setDefrostControlType(std::string defrostControlType) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setDefrostControlType(defrostControlType);
}

void RefrigerationAirChiller::resetDefrostControlType() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetDefrostControlType();
}

bool RefrigerationAirChiller::setDefrostSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setDefrostSchedule(schedule);
}

bool RefrigerationAirChiller::setDefrostDripDownSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setDefrostDripDownSchedule(schedule);
}

void RefrigerationAirChiller::resetDefrostDripDownSchedule() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetDefrostDripDownSchedule();
}

bool RefrigerationAirChiller::setDefrostPower(double defrostPower) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setDefrostPower(defrostPower);
}

void RefrigerationAirChiller::resetDefrostPower() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetDefrostPower();
}

bool RefrigerationAirChiller::setTemperatureTerminationDefrostFractiontoIce(double temperatureTerminationDefrostFractiontoIce) {
  return getImpl<detail::RefrigerationAirChiller_Impl>()->setTemperatureTerminationDefrostFractiontoIce(temperatureTerminationDefrostFractiontoIce);
}

void RefrigerationAirChiller::resetTemperatureTerminationDefrostFractiontoIce() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetTemperatureTerminationDefrostFractiontoIce();
}

// bool RefrigerationAirChiller::setVerticalLocation(std::string verticalLocation) {
//   return getImpl<detail::RefrigerationAirChiller_Impl>()->setVerticalLocation(verticalLocation);
// }

// void RefrigerationAirChiller::resetVerticalLocation() {
//   getImpl<detail::RefrigerationAirChiller_Impl>()->resetVerticalLocation();
// }

void RefrigerationAirChiller::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
  getImpl<detail::RefrigerationAirChiller_Impl>()->setAverageRefrigerantChargeInventory(averageRefrigerantChargeInventory);
}

void RefrigerationAirChiller::resetAverageRefrigerantChargeInventory() {
  getImpl<detail::RefrigerationAirChiller_Impl>()->resetAverageRefrigerantChargeInventory();
}

/// @cond
RefrigerationAirChiller::RefrigerationAirChiller(std::shared_ptr<detail::RefrigerationAirChiller_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

