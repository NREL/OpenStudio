/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/HeatingCoilName.hpp>
#include <model/HeatingCoilName_Impl.hpp>
#include <model/CoolingCoilName.hpp>
#include <model/CoolingCoilName_Impl.hpp>
#include <model/ZoneMixers.hpp>
#include <model/ZoneMixers_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const IdfObject& idfObject,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::iddObjectType() const {
    return AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeInduction","Availability"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumTotalAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumTotalAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inductionRatio() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isInductionRatioDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio);
  }

  Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::supplyAirInletNode() const {
    boost::optional<Connection> value = optionalSupplyAirInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Inlet Node attached.");
    }
    return value.get();
  }

  Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::inducedAirInletNode() const {
    boost::optional<Connection> value = optionalInducedAirInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Induced Air Inlet Node attached.");
    }
    return value.get();
  }

  Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::airOutletNode() const {
    boost::optional<Connection> value = optionalAirOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Node attached.");
    }
    return value.get();
  }

  HeatingCoilName AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::heatingCoil() const {
    boost::optional<HeatingCoilName> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumHotWaterFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumHotWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::minimumHotWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMinimumHotWaterFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate);
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::heatingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isHeatingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance);
  }

  boost::optional<CoolingCoilName> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::coolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<CoolingCoilName>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName);
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::maximumColdWaterFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMaximumColdWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::minimumColdWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isMinimumColdWaterFlowRateDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate);
  }

  double AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::coolingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::isCoolingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance);
  }

  boost::optional<ZoneMixers> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::zoneMixer() const {
    return getObject<ModelObject>().getModelObjectTarget<ZoneMixers>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::ZoneMixerName);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeInduction",
                              "Availability",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumTotalAirFlowRate(boost::optional<double> maximumTotalAirFlowRate) {
    bool result(false);
    if (maximumTotalAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, maximumTotalAirFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumTotalAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setInductionRatio(double inductionRatio) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio, inductionRatio);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetInductionRatio() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setSupplyAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::SupplyAirInletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setInducedAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InducedAirInletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AirOutletNodeName, connection.handle());
    return result;
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setHeatingCoil(const HeatingCoilName& heatingCoilName) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName, heatingCoilName.handle());
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate) {
    bool result(false);
    if (maximumHotWaterFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, maximumHotWaterFlowRate.get());
    }
    else {
      resetMaximumHotWaterFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMaximumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate, minimumHotWaterFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMinimumHotWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetHeatingConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setCoolingCoil(const boost::optional<CoolingCoilName>& coolingCoilName) {
    bool result(false);
    if (coolingCoilName) {
      result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName, coolingCoilName.get().handle());
    }
    else {
      resetCoolingCoil();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetCoolingCoil() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate) {
    bool result(false);
    if (maximumColdWaterFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, maximumColdWaterFlowRate.get());
    }
    else {
      resetMaximumColdWaterFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMaximumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::autosizeMaximumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate, minimumColdWaterFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetMinimumColdWaterFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance, coolingConvergenceTolerance);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetCoolingConvergenceTolerance() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::setZoneMixer(const boost::optional<ZoneMixers>& zoneMixers) {
    bool result(false);
    if (zoneMixers) {
      result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::ZoneMixerName, zoneMixers.get().handle());
    }
    else {
      resetZoneMixer();
      result = true;
    }
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::resetZoneMixer() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::ZoneMixerName, "");
    OS_ASSERT(result);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::optionalSupplyAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::SupplyAirInletNodeName);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::optionalInducedAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InducedAirInletNodeName);
  }

  boost::optional<Connection> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::optionalAirOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AirOutletNodeName);
  }

  boost::optional<HeatingCoilName> AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HeatingCoilName>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName);
  }

} // detail

AirTerminalSingleDuctConstantVolumeFourPipeInduction::AirTerminalSingleDuctConstantVolumeFourPipeInduction(const Model& model)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::SupplyAirInletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InducedAirInletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AirOutletNodeName
  //     OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setMaximumTotalAirFlowRate();
  OS_ASSERT(ok);
  // ok = setSupplyAirInletNode();
  OS_ASSERT(ok);
  // ok = setInducedAirInletNode();
  OS_ASSERT(ok);
  // ok = setAirOutletNode();
  OS_ASSERT(ok);
  // ok = setHeatingCoil();
  OS_ASSERT(ok);
}

IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeInduction::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction);
}

boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeFourPipeInduction::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumTotalAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumTotalAirFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumTotalAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumTotalAirFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::inductionRatio() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->inductionRatio();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isInductionRatioDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isInductionRatioDefaulted();
}

Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction::supplyAirInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->supplyAirInletNode();
}

Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction::inducedAirInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->inducedAirInletNode();
}

Connection AirTerminalSingleDuctConstantVolumeFourPipeInduction::airOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->airOutletNode();
}

HeatingCoilName AirTerminalSingleDuctConstantVolumeFourPipeInduction::heatingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->heatingCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumHotWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumHotWaterFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumHotWaterFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::minimumHotWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->minimumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMinimumHotWaterFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMinimumHotWaterFlowRateDefaulted();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::heatingConvergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->heatingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isHeatingConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isHeatingConvergenceToleranceDefaulted();
}

boost::optional<CoolingCoilName> AirTerminalSingleDuctConstantVolumeFourPipeInduction::coolingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->coolingCoil();
}

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeInduction::maximumColdWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->maximumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMaximumColdWaterFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMaximumColdWaterFlowRateAutosized();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::minimumColdWaterFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->minimumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isMinimumColdWaterFlowRateDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isMinimumColdWaterFlowRateDefaulted();
}

double AirTerminalSingleDuctConstantVolumeFourPipeInduction::coolingConvergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->coolingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::isCoolingConvergenceToleranceDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->isCoolingConvergenceToleranceDefaulted();
}

boost::optional<ZoneMixers> AirTerminalSingleDuctConstantVolumeFourPipeInduction::zoneMixer() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->zoneMixer();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumTotalAirFlowRate(double maximumTotalAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumTotalAirFlowRate(maximumTotalAirFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumTotalAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumTotalAirFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setInductionRatio(double inductionRatio) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setInductionRatio(inductionRatio);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetInductionRatio() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetInductionRatio();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setSupplyAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setSupplyAirInletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setInducedAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setInducedAirInletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setAirOutletNode(connection);
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setHeatingCoil(const HeatingCoilName& heatingCoilName) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setHeatingCoil(heatingCoilName);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate) {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumHotWaterFlowRate(maximumHotWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMaximumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMaximumHotWaterFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMinimumHotWaterFlowRate(minimumHotWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMinimumHotWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMinimumHotWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetHeatingConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetHeatingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setCoolingCoil(const CoolingCoilName& coolingCoilName) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setCoolingCoil(coolingCoilName);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetCoolingCoil() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetCoolingCoil();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate) {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMaximumColdWaterFlowRate(maximumColdWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMaximumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMaximumColdWaterFlowRate();
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::autosizeMaximumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->autosizeMaximumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setMinimumColdWaterFlowRate(minimumColdWaterFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetMinimumColdWaterFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetMinimumColdWaterFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setCoolingConvergenceTolerance(coolingConvergenceTolerance);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetCoolingConvergenceTolerance() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetCoolingConvergenceTolerance();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeInduction::setZoneMixer(const ZoneMixers& zoneMixers) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->setZoneMixer(zoneMixers);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInduction::resetZoneMixer() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl>()->resetZoneMixer();
}

/// @cond
AirTerminalSingleDuctConstantVolumeFourPipeInduction::AirTerminalSingleDuctConstantVolumeFourPipeInduction(boost::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

