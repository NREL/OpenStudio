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

#include "RefrigerationCondenserEvaporativeCooled.hpp"
#include "RefrigerationCondenserEvaporativeCooled_Impl.hpp"

// WaterStorageTank object not used
//#include "WaterStorageTank.hpp"
//#include "WaterStorageTank_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Condenser_EvaporativeCooled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCondenserEvaporativeCooled_Impl::RefrigerationCondenserEvaporativeCooled_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationCondenserEvaporativeCooled::iddObjectType());
  }

  RefrigerationCondenserEvaporativeCooled_Impl::RefrigerationCondenserEvaporativeCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationCondenserEvaporativeCooled::iddObjectType());
  }

  RefrigerationCondenserEvaporativeCooled_Impl::RefrigerationCondenserEvaporativeCooled_Impl(const RefrigerationCondenserEvaporativeCooled_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationCondenserEvaporativeCooled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationCondenserEvaporativeCooled_Impl::iddObjectType() const {
    return RefrigerationCondenserEvaporativeCooled::iddObjectType();
  }

  ModelObject RefrigerationCondenserEvaporativeCooled_Impl::clone(Model model) const
  {
    RefrigerationCondenserEvaporativeCooled modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationCondenserEvaporativeCooled>();

    //modelObjectClone.resetAirInletNode();

    return modelObjectClone;
  }

  std::vector<ScheduleTypeKey> RefrigerationCondenserEvaporativeCooled_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeCondenserAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCondenserEvaporativeCooled","Evaporative Condenser Availability"));
    }
    return result;
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::ratedEffectiveTotalHeatRejectionRate() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedEffectiveTotalHeatRejectionRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::ratedSubcoolingTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedSubcoolingTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedSubcoolingTemperatureDifference);
  }

  std::string RefrigerationCondenserEvaporativeCooled_Impl::fanSpeedControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isFanSpeedControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::ratedFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::minimumFanAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumFanAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isMinimumFanAirFlowRatioDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumFanAirFlowRatio);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::approachTemperatureConstantTerm() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureConstantTerm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isApproachTemperatureConstantTermDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureConstantTerm);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::approachTemperatureCoefficient2() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isApproachTemperatureCoefficient2Defaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient2);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::approachTemperatureCoefficient3() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isApproachTemperatureCoefficient3Defaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient3);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::approachTemperatureCoefficient4() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isApproachTemperatureCoefficient4Defaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient4);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::minimumCapacityFactor() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumCapacityFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isMinimumCapacityFactorDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumCapacityFactor);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::maximumCapacityFactor() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MaximumCapacityFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isMaximumCapacityFactorDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::MaximumCapacityFactor);
  }

  // std::string RefrigerationCondenserEvaporativeCooled_Impl::airInletNode() const {
  //   boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_EvaporativeCooledFields::AirInletNodeName,true);
  //   OS_ASSERT(value);
  //   return value.get();
  // }

  boost::optional<double> RefrigerationCondenserEvaporativeCooled_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate,true);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isRatedAirFlowRateDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isRatedAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isBasinHeaterCapacityDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterCapacity);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isBasinHeaterSetpointTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterSetpointTemperature);
  }

  boost::optional<double> RefrigerationCondenserEvaporativeCooled_Impl::ratedWaterPumpPower() const {
    return getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower,true);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isRatedWaterPumpPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isRatedWaterPumpPowerAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  /*boost::optional<WaterStorageTank> RefrigerationCondenserEvaporativeCooled_Impl::evaporativeWaterSupplyTank() const {
    return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeWaterSupplyTankName);
  }*/

  boost::optional<Schedule> RefrigerationCondenserEvaporativeCooled_Impl::evaporativeCondenserAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeCondenserAvailabilityScheduleName);
  }

  std::string RefrigerationCondenserEvaporativeCooled_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_EvaporativeCooledFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::EndUseSubcategory);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::condenserRefrigerantOperatingChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondenserRefrigerantOperatingChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondenserRefrigerantOperatingChargeInventory);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::condensateReceiverRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensateReceiverRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isCondensateReceiverRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensateReceiverRefrigerantInventory);
  }

  double RefrigerationCondenserEvaporativeCooled_Impl::condensatePipingRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensatePipingRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::isCondensatePipingRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensatePipingRefrigerantInventory);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedEffectiveTotalHeatRejectionRate, ratedEffectiveTotalHeatRejectionRate);
    return result;
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedSubcoolingTemperatureDifference, ratedSubcoolingTemperatureDifference);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetRatedSubcoolingTemperatureDifference() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedSubcoolingTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setFanSpeedControlType(std::string fanSpeedControlType) {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType, fanSpeedControlType);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetFanSpeedControlType() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setRatedFanPower(double ratedFanPower) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedFanPower, ratedFanPower);
    return result;
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumFanAirFlowRatio, minimumFanAirFlowRatio);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetMinimumFanAirFlowRatio() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumFanAirFlowRatio, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setApproachTemperatureConstantTerm(double approachTemperatureConstantTerm) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureConstantTerm, approachTemperatureConstantTerm);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetApproachTemperatureConstantTerm() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureConstantTerm, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setApproachTemperatureCoefficient2(double approachTemperatureCoefficient2) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient2, approachTemperatureCoefficient2);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetApproachTemperatureCoefficient2() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient2, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setApproachTemperatureCoefficient3(double approachTemperatureCoefficient3) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient3, approachTemperatureCoefficient3);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetApproachTemperatureCoefficient3() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient3, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setApproachTemperatureCoefficient4(double approachTemperatureCoefficient4) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient4, approachTemperatureCoefficient4);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetApproachTemperatureCoefficient4() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient4, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setMinimumCapacityFactor(double minimumCapacityFactor) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumCapacityFactor, minimumCapacityFactor);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetMinimumCapacityFactor() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::MinimumCapacityFactor, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setMaximumCapacityFactor(double maximumCapacityFactor) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::MaximumCapacityFactor, maximumCapacityFactor);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetMaximumCapacityFactor() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::MaximumCapacityFactor, "");
    OS_ASSERT(result);
  }

  // bool RefrigerationCondenserEvaporativeCooled_Impl::setAirInletNode(std::string airInletNode) {
  //   bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::AirInletNodeName, airInletNode);
  //   return result;
  // }

  // void RefrigerationCondenserEvaporativeCooled_Impl::resetAirInletNode() {
  //   bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::AirInletNodeName, "");
  //   OS_ASSERT(result);
  // }

  void RefrigerationCondenserEvaporativeCooled_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    else {
      resetRatedAirFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetRatedAirFlowRate() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::autocalculateRatedAirFlowRate() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetBasinHeaterCapacity() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterCapacity, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserEvaporativeCooled_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetBasinHeaterSetpointTemperature() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterSetpointTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setRatedWaterPumpPower(boost::optional<double> ratedWaterPumpPower) {
    bool result(false);
    if (ratedWaterPumpPower) {
      result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, ratedWaterPumpPower.get());
    }
    else {
      resetRatedWaterPumpPower();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetRatedWaterPumpPower() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::autocalculateRatedWaterPumpPower() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, "autocalculate");
    OS_ASSERT(result);
  }

  /*bool RefrigerationCondenserEvaporativeCooled_Impl::setEvaporativeWaterSupplyTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
    bool result(false);
    if (waterStorageTank) {
      result = setPointer(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeWaterSupplyTankName, waterStorageTank.get().handle());
    }
    else {
      resetEvaporativeWaterSupplyTank();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetEvaporativeWaterSupplyTank() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeWaterSupplyTankName, "");
    OS_ASSERT(result);
  }*/

  bool RefrigerationCondenserEvaporativeCooled_Impl::setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeCondenserAvailabilityScheduleName,
                              "RefrigerationCondenserEvaporativeCooled",
                              "Evaporative Condenser Availability",
                              schedule);
    return result;
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetEvaporativeCondenserAvailabilitySchedule() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeCondenserAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondenserRefrigerantOperatingChargeInventory, condenserRefrigerantOperatingChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetCondenserRefrigerantOperatingChargeInventory() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondenserRefrigerantOperatingChargeInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensateReceiverRefrigerantInventory, condensateReceiverRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetCondensateReceiverRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensateReceiverRefrigerantInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensatePipingRefrigerantInventory, condensatePipingRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserEvaporativeCooled_Impl::resetCondensatePipingRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_EvaporativeCooledFields::CondensatePipingRefrigerantInventory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationCondenserEvaporativeCooled::RefrigerationCondenserEvaporativeCooled(const Model& model)
  : ParentObject(RefrigerationCondenserEvaporativeCooled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>());

  bool ok = setRatedEffectiveTotalHeatRejectionRate(43100.0);
  OS_ASSERT(ok);
  ok = setRatedSubcoolingTemperatureDifference(0.0);
  OS_ASSERT(ok);
  ok = setFanSpeedControlType("VariableSpeed");
  OS_ASSERT(ok);
  ok = setRatedFanPower(373.0);
  OS_ASSERT(ok);
  ok = setMinimumFanAirFlowRatio(0.2);
  OS_ASSERT(ok);
  ok = setApproachTemperatureConstantTerm(6.63);
  OS_ASSERT(ok);
  ok = setApproachTemperatureCoefficient2(0.468);
  OS_ASSERT(ok);
  ok = setApproachTemperatureCoefficient3(17.93);
  OS_ASSERT(ok);
  ok = setApproachTemperatureCoefficient4(-0.322);
  OS_ASSERT(ok);
  setMinimumCapacityFactor(0.6);
  setMaximumCapacityFactor(4.8);
  setRatedAirFlowRate(1.37);
  ok = setBasinHeaterCapacity(66.7);
  OS_ASSERT(ok);
  ok = setBasinHeaterSetpointTemperature(2.0);
  OS_ASSERT(ok);
  setRatedWaterPumpPower(250.0);
}

IddObjectType RefrigerationCondenserEvaporativeCooled::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled);
}

std::vector<std::string> RefrigerationCondenserEvaporativeCooled::fanSpeedControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType);
}

double RefrigerationCondenserEvaporativeCooled::ratedEffectiveTotalHeatRejectionRate() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->ratedEffectiveTotalHeatRejectionRate();
}

double RefrigerationCondenserEvaporativeCooled::ratedSubcoolingTemperatureDifference() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->ratedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserEvaporativeCooled::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isRatedSubcoolingTemperatureDifferenceDefaulted();
}

std::string RefrigerationCondenserEvaporativeCooled::fanSpeedControlType() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->fanSpeedControlType();
}

bool RefrigerationCondenserEvaporativeCooled::isFanSpeedControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isFanSpeedControlTypeDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::ratedFanPower() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->ratedFanPower();
}

double RefrigerationCondenserEvaporativeCooled::minimumFanAirFlowRatio() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->minimumFanAirFlowRatio();
}

bool RefrigerationCondenserEvaporativeCooled::isMinimumFanAirFlowRatioDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isMinimumFanAirFlowRatioDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::approachTemperatureConstantTerm() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->approachTemperatureConstantTerm();
}

bool RefrigerationCondenserEvaporativeCooled::isApproachTemperatureConstantTermDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isApproachTemperatureConstantTermDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::approachTemperatureCoefficient2() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->approachTemperatureCoefficient2();
}

bool RefrigerationCondenserEvaporativeCooled::isApproachTemperatureCoefficient2Defaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isApproachTemperatureCoefficient2Defaulted();
}

double RefrigerationCondenserEvaporativeCooled::approachTemperatureCoefficient3() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->approachTemperatureCoefficient3();
}

bool RefrigerationCondenserEvaporativeCooled::isApproachTemperatureCoefficient3Defaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isApproachTemperatureCoefficient3Defaulted();
}

double RefrigerationCondenserEvaporativeCooled::approachTemperatureCoefficient4() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->approachTemperatureCoefficient4();
}

bool RefrigerationCondenserEvaporativeCooled::isApproachTemperatureCoefficient4Defaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isApproachTemperatureCoefficient4Defaulted();
}

double RefrigerationCondenserEvaporativeCooled::minimumCapacityFactor() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->minimumCapacityFactor();
}

bool RefrigerationCondenserEvaporativeCooled::isMinimumCapacityFactorDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isMinimumCapacityFactorDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::maximumCapacityFactor() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->maximumCapacityFactor();
}

bool RefrigerationCondenserEvaporativeCooled::isMaximumCapacityFactorDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isMaximumCapacityFactorDefaulted();
}

// std::string RefrigerationCondenserEvaporativeCooled::airInletNode() const {
//   return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->airInletNode();
// }

boost::optional<double> RefrigerationCondenserEvaporativeCooled::ratedAirFlowRate() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->ratedAirFlowRate();
}

bool RefrigerationCondenserEvaporativeCooled::isRatedAirFlowRateDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isRatedAirFlowRateDefaulted();
}

bool RefrigerationCondenserEvaporativeCooled::isRatedAirFlowRateAutocalculated() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isRatedAirFlowRateAutocalculated();
}

double RefrigerationCondenserEvaporativeCooled::basinHeaterCapacity() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->basinHeaterCapacity();
}

bool RefrigerationCondenserEvaporativeCooled::isBasinHeaterCapacityDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isBasinHeaterCapacityDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::basinHeaterSetpointTemperature() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->basinHeaterSetpointTemperature();
}

bool RefrigerationCondenserEvaporativeCooled::isBasinHeaterSetpointTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isBasinHeaterSetpointTemperatureDefaulted();
}

boost::optional<double> RefrigerationCondenserEvaporativeCooled::ratedWaterPumpPower() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->ratedWaterPumpPower();
}

bool RefrigerationCondenserEvaporativeCooled::isRatedWaterPumpPowerDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isRatedWaterPumpPowerDefaulted();
}

bool RefrigerationCondenserEvaporativeCooled::isRatedWaterPumpPowerAutocalculated() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isRatedWaterPumpPowerAutocalculated();
}

/*boost::optional<WaterStorageTank> RefrigerationCondenserEvaporativeCooled::evaporativeWaterSupplyTank() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->evaporativeWaterSupplyTank();
}*/

boost::optional<Schedule> RefrigerationCondenserEvaporativeCooled::evaporativeCondenserAvailabilitySchedule() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->evaporativeCondenserAvailabilitySchedule();
}

std::string RefrigerationCondenserEvaporativeCooled::endUseSubcategory() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->endUseSubcategory();
}

bool RefrigerationCondenserEvaporativeCooled::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isEndUseSubcategoryDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::condenserRefrigerantOperatingChargeInventory() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->condenserRefrigerantOperatingChargeInventory();
}

bool RefrigerationCondenserEvaporativeCooled::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isCondenserRefrigerantOperatingChargeInventoryDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::condensateReceiverRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->condensateReceiverRefrigerantInventory();
}

bool RefrigerationCondenserEvaporativeCooled::isCondensateReceiverRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isCondensateReceiverRefrigerantInventoryDefaulted();
}

double RefrigerationCondenserEvaporativeCooled::condensatePipingRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->condensatePipingRefrigerantInventory();
}

bool RefrigerationCondenserEvaporativeCooled::isCondensatePipingRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->isCondensatePipingRefrigerantInventoryDefaulted();
}

bool RefrigerationCondenserEvaporativeCooled::setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setRatedEffectiveTotalHeatRejectionRate(ratedEffectiveTotalHeatRejectionRate);
}

bool RefrigerationCondenserEvaporativeCooled::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setRatedSubcoolingTemperatureDifference(ratedSubcoolingTemperatureDifference);
}

void RefrigerationCondenserEvaporativeCooled::resetRatedSubcoolingTemperatureDifference() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetRatedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserEvaporativeCooled::setFanSpeedControlType(std::string fanSpeedControlType) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setFanSpeedControlType(fanSpeedControlType);
}

void RefrigerationCondenserEvaporativeCooled::resetFanSpeedControlType() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetFanSpeedControlType();
}

bool RefrigerationCondenserEvaporativeCooled::setRatedFanPower(double ratedFanPower) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setRatedFanPower(ratedFanPower);
}

bool RefrigerationCondenserEvaporativeCooled::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setMinimumFanAirFlowRatio(minimumFanAirFlowRatio);
}

void RefrigerationCondenserEvaporativeCooled::resetMinimumFanAirFlowRatio() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetMinimumFanAirFlowRatio();
}

bool RefrigerationCondenserEvaporativeCooled::setApproachTemperatureConstantTerm(double approachTemperatureConstantTerm) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setApproachTemperatureConstantTerm(approachTemperatureConstantTerm);
}

void RefrigerationCondenserEvaporativeCooled::resetApproachTemperatureConstantTerm() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetApproachTemperatureConstantTerm();
}

bool RefrigerationCondenserEvaporativeCooled::setApproachTemperatureCoefficient2(double approachTemperatureCoefficient2) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setApproachTemperatureCoefficient2(approachTemperatureCoefficient2);
}

void RefrigerationCondenserEvaporativeCooled::resetApproachTemperatureCoefficient2() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetApproachTemperatureCoefficient2();
}

bool RefrigerationCondenserEvaporativeCooled::setApproachTemperatureCoefficient3(double approachTemperatureCoefficient3) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setApproachTemperatureCoefficient3(approachTemperatureCoefficient3);
}

void RefrigerationCondenserEvaporativeCooled::resetApproachTemperatureCoefficient3() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetApproachTemperatureCoefficient3();
}

bool RefrigerationCondenserEvaporativeCooled::setApproachTemperatureCoefficient4(double approachTemperatureCoefficient4) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setApproachTemperatureCoefficient4(approachTemperatureCoefficient4);
}

void RefrigerationCondenserEvaporativeCooled::resetApproachTemperatureCoefficient4() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetApproachTemperatureCoefficient4();
}

void RefrigerationCondenserEvaporativeCooled::setMinimumCapacityFactor(double minimumCapacityFactor) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setMinimumCapacityFactor(minimumCapacityFactor);
}

void RefrigerationCondenserEvaporativeCooled::resetMinimumCapacityFactor() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetMinimumCapacityFactor();
}

void RefrigerationCondenserEvaporativeCooled::setMaximumCapacityFactor(double maximumCapacityFactor) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setMaximumCapacityFactor(maximumCapacityFactor);
}

void RefrigerationCondenserEvaporativeCooled::resetMaximumCapacityFactor() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetMaximumCapacityFactor();
}

// bool RefrigerationCondenserEvaporativeCooled::setAirInletNode(std::string airInletNode) {
//   return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setAirInletNode(airInletNode);
// }

// void RefrigerationCondenserEvaporativeCooled::resetAirInletNode() {
//   getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetAirInletNode();
// }

void RefrigerationCondenserEvaporativeCooled::setRatedAirFlowRate(double ratedAirFlowRate) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void RefrigerationCondenserEvaporativeCooled::resetRatedAirFlowRate() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetRatedAirFlowRate();
}

void RefrigerationCondenserEvaporativeCooled::autocalculateRatedAirFlowRate() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->autocalculateRatedAirFlowRate();
}

bool RefrigerationCondenserEvaporativeCooled::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

void RefrigerationCondenserEvaporativeCooled::resetBasinHeaterCapacity() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetBasinHeaterCapacity();
}

bool RefrigerationCondenserEvaporativeCooled::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

void RefrigerationCondenserEvaporativeCooled::resetBasinHeaterSetpointTemperature() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetBasinHeaterSetpointTemperature();
}

void RefrigerationCondenserEvaporativeCooled::setRatedWaterPumpPower(double ratedWaterPumpPower) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setRatedWaterPumpPower(ratedWaterPumpPower);
}

void RefrigerationCondenserEvaporativeCooled::resetRatedWaterPumpPower() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetRatedWaterPumpPower();
}

void RefrigerationCondenserEvaporativeCooled::autocalculateRatedWaterPumpPower() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->autocalculateRatedWaterPumpPower();
}

/*bool RefrigerationCondenserEvaporativeCooled::setEvaporativeWaterSupplyTank(const WaterStorageTank& waterStorageTank) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setEvaporativeWaterSupplyTank(waterStorageTank);
}

void RefrigerationCondenserEvaporativeCooled::resetEvaporativeWaterSupplyTank() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetEvaporativeWaterSupplyTank();
}*/

bool RefrigerationCondenserEvaporativeCooled::setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setEvaporativeCondenserAvailabilitySchedule(schedule);
}

void RefrigerationCondenserEvaporativeCooled::resetEvaporativeCondenserAvailabilitySchedule() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetEvaporativeCondenserAvailabilitySchedule();
}

void RefrigerationCondenserEvaporativeCooled::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationCondenserEvaporativeCooled::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetEndUseSubcategory();
}

void RefrigerationCondenserEvaporativeCooled::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setCondenserRefrigerantOperatingChargeInventory(condenserRefrigerantOperatingChargeInventory);
}

void RefrigerationCondenserEvaporativeCooled::resetCondenserRefrigerantOperatingChargeInventory() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetCondenserRefrigerantOperatingChargeInventory();
}

void RefrigerationCondenserEvaporativeCooled::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setCondensateReceiverRefrigerantInventory(condensateReceiverRefrigerantInventory);
}

void RefrigerationCondenserEvaporativeCooled::resetCondensateReceiverRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetCondensateReceiverRefrigerantInventory();
}

void RefrigerationCondenserEvaporativeCooled::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->setCondensatePipingRefrigerantInventory(condensatePipingRefrigerantInventory);
}

void RefrigerationCondenserEvaporativeCooled::resetCondensatePipingRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserEvaporativeCooled_Impl>()->resetCondensatePipingRefrigerantInventory();
}

/// @cond
RefrigerationCondenserEvaporativeCooled::RefrigerationCondenserEvaporativeCooled(std::shared_ptr<detail::RefrigerationCondenserEvaporativeCooled_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

