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
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
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

  unsigned AirTerminalDualDuctConstantVolume_Impl::outletPort() const {
    return OS_AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNode;
  }

  unsigned AirTerminalDualDuctConstantVolume_Impl::inletPort(unsigned branchIndex) const {
    if( branchIndex == 0 ) {
      return OS_AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNode;
    } else if( branchIndex == 1 ) {
      return OS_AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNode;
    } else {
      LOG(Warn, "Calling inletPort with branchIndex greater than 1 for " << briefDescription() << " is not valid.");
      LOG(Warn, briefDescription() << " has only two branches.");
      return std::numeric_limits<unsigned>::max();
    }
  }

  unsigned AirTerminalDualDuctConstantVolume_Impl::nextInletPort() const {
    LOG(Warn, "nextInletPort is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
    return std::numeric_limits<unsigned>::max();
  }

  unsigned AirTerminalDualDuctConstantVolume_Impl::newInletPortAfterBranch(unsigned branchIndex) {
    LOG(Warn, "newInletPortAfterBranch is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
    return std::numeric_limits<unsigned>::max();
  }

  void AirTerminalDualDuctConstantVolume_Impl::removePortForBranch(unsigned branchIndex) {
    LOG(Warn, "removePortForBranch is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
  }

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

  bool AirTerminalDualDuctConstantVolume_Impl::setMaximumAirFlowRate(double maximumAirFlowRate) {
    bool result = setDouble(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, maximumAirFlowRate);
    return result;
  }

  void AirTerminalDualDuctConstantVolume_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<Node> AirTerminalDualDuctConstantVolume_Impl::HotAirInletNode() const {
    boost::optional<Node> node;
    if( auto mo = inletModelObject(0) ) {
      node = mo->optionalCast<Node>();
      OS_ASSERT(node);
    }
    return node;
  }

  boost::optional<Node> AirTerminalDualDuctConstantVolume_Impl::ColdAirInletNode() const {
    boost::optional<Node> node;
    if( auto mo = inletModelObject(1) ) {
      node = mo->optionalCast<Node>();
      OS_ASSERT(node);
    }
    return node;
  }

  std::vector<IdfObject> AirTerminalDualDuctConstantVolume_Impl::remove() {
    auto modelObject = getObject<AirTerminalDualDuctConstantVolume>();
    AirLoopHVAC_Impl::removeDualDuctTerminalFromAirLoopHVAC(modelObject,inletPort(0),inletPort(1),outletPort());
    return Mixer_Impl::remove();
  }

  bool AirTerminalDualDuctConstantVolume_Impl::addToNode(Node & node)
  {
    auto mo = getObject<AirTerminalDualDuctConstantVolume>();
    return AirLoopHVAC_Impl::addDualDuctTerminalToNode(mo,inletPort(0),inletPort(1),outletPort(),node);
  }

  ModelObject AirTerminalDualDuctConstantVolume_Impl::clone(Model model) const
  {
    auto t_clone = Mixer_Impl::clone(model).cast<AirTerminalDualDuctConstantVolume>();

    t_clone.setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNode,"");
    t_clone.setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNode,"");
    t_clone.setString(OS_AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNode,"");

    return t_clone;
  }

  bool AirTerminalDualDuctConstantVolume_Impl::isRemovable() const
  {
    return HVACComponent_Impl::isRemovable();
  }

} // detail

AirTerminalDualDuctConstantVolume::AirTerminalDualDuctConstantVolume(const Model& model)
  : Mixer(AirTerminalDualDuctConstantVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  // bool ok = true;
  // ok = setMaximumAirFlowRate();
  // OS_ASSERT(ok);
  autosizeMaximumAirFlowRate();
}

IddObjectType AirTerminalDualDuctConstantVolume::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume);
}

boost::optional<Schedule> AirTerminalDualDuctConstantVolume::availabilitySchedule() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->availabilitySchedule();
}

boost::optional<Node> AirTerminalDualDuctConstantVolume::HotAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->HotAirInletNode();
}

boost::optional<Node> AirTerminalDualDuctConstantVolume::ColdAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctConstantVolume_Impl>()->ColdAirInletNode();
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

