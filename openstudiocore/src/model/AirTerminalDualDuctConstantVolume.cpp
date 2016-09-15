/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "AirTerminalDualDuctConstantVolume.hpp"
#include "AirTerminalDualDuctConstantVolume_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirTerminal_DualDuct_ConstantVolume_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalDualDuctConstantVolume_Impl::AirTerminalDualDuctConstantVolume_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Mixer_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalDualDuctConstantVolume::iddObjectType());
  }

  AirTerminalDualDuctConstantVolume_Impl::AirTerminalDualDuctConstantVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalDualDuctConstantVolume::iddObjectType());
  }

  AirTerminalDualDuctConstantVolume_Impl::AirTerminalDualDuctConstantVolume_Impl(const AirTerminalDualDuctConstantVolume_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalDualDuctConstantVolume_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalDualDuctConstantVolume_Impl::iddObjectType() const {
    return AirTerminalDualDuctConstantVolume::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalDualDuctConstantVolume_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_DualDuct_ConstantVolumeFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalDualDuctConstantVolume","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalDualDuctConstantVolume_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_DualDuct_ConstantVolumeFields::AvailabilitySchedule);
  }

  /*boost::optional<Connection> AirTerminalDualDuctConstantVolume_Impl::airOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNode);
  }

  boost::optional<Connection> AirTerminalDualDuctConstantVolume_Impl::hotAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNode);
  }

  boost::optional<Connection> AirTerminalDualDuctConstantVolume_Impl::coldAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNode);
  }*/

  boost::optional<double> AirTerminalDualDuctConstantVolume_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate,true);
  }

  bool AirTerminalDualDuctConstantVolume_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool AirTerminalDualDuctConstantVolume_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_DualDuct_ConstantVolumeFields::AvailabilitySchedule,
                              "AirTerminalDualDuctConstantVolume",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  /*
  bool AirTerminalDualDuctConstantVolume_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::resetAirOutletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctConstantVolume_Impl::setHotAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::resetHotAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctConstantVolume_Impl::setColdAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::resetColdAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNode, "");
    OS_ASSERT(result);
  }*/

  bool AirTerminalDualDuctConstantVolume_Impl::setMaximumAirFlowRate(double maximumAirFlowRate) {
    bool result = setDouble(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, maximumAirFlowRate);
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

} // detail

AirTerminalDualDuctConstantVolume::AirTerminalDualDuctConstantVolume(const Model& model)
  : Mixer(AirTerminalDualDuctConstantVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setMaximumAirFlowRate();
  OS_ASSERT(ok);
}

IddObjectType AirTerminalDualDuctConstantVolume::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume);
}

boost::optional<Schedule> AirTerminalDualDuctConstantVolume::availabilitySchedule() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->availabilitySchedule();
}

boost::optional<Connection> AirTerminalDualDuctConstantVolume::airOutletNode() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->airOutletNode();
}

boost::optional<Connection> AirTerminalDualDuctConstantVolume::hotAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->hotAirInletNode();
}

boost::optional<Connection> AirTerminalDualDuctConstantVolume::coldAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->coldAirInletNode();
}

boost::optional<double> AirTerminalDualDuctConstantVolume::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->maximumAirFlowRate();
}

bool AirTerminalDualDuctConstantVolume::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->isMaximumAirFlowRateAutosized();
}

bool AirTerminalDualDuctConstantVolume::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalDualDuctConstantVolume::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalDualDuctConstantVolume::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->setAirOutletNode(connection);
}

void AirTerminalDualDuctConstantVolume::resetAirOutletNode() {
  getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->resetAirOutletNode();
}

bool AirTerminalDualDuctConstantVolume::setHotAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->setHotAirInletNode(connection);
}

void AirTerminalDualDuctConstantVolume::resetHotAirInletNode() {
  getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->resetHotAirInletNode();
}

bool AirTerminalDualDuctConstantVolume::setColdAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->setColdAirInletNode(connection);
}

void AirTerminalDualDuctConstantVolume::resetColdAirInletNode() {
  getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->resetColdAirInletNode();
}

bool AirTerminalDualDuctConstantVolume::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalDualDuctConstantVolume::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->autosizeMaximumAirFlowRate();
}

/// @cond
AirTerminalDualDuctConstantVolume::AirTerminalDualDuctConstantVolume(std::shared_ptr<detail::AirTerminalDualDuctConstantVolume_Impl> impl)
  : Mixer(impl)
{}
/// @endcond

} // model
} // openstudio

