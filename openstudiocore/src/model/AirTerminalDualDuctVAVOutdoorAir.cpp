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

#include "AirTerminalDualDuctVAVOutdoorAir.hpp"
#include "AirTerminalDualDuctVAVOutdoorAir_Impl.hpp"
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
#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirTerminal_DualDuct_VAV_OutdoorAir_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : Mixer_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalDualDuctVAVOutdoorAir::iddObjectType());
  }

  AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalDualDuctVAVOutdoorAir::iddObjectType());
  }

  AirTerminalDualDuctVAVOutdoorAir_Impl::AirTerminalDualDuctVAVOutdoorAir_Impl(const AirTerminalDualDuctVAVOutdoorAir_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalDualDuctVAVOutdoorAir_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalDualDuctVAVOutdoorAir_Impl::iddObjectType() const {
    return AirTerminalDualDuctVAVOutdoorAir::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalDualDuctVAVOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalDualDuctVAVOutdoorAir","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalDualDuctVAVOutdoorAir_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule);
  }

  /*boost::optional<Connection> AirTerminalDualDuctVAVOutdoorAir_Impl::airOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode);
  }

  boost::optional<Connection> AirTerminalDualDuctVAVOutdoorAir_Impl::outdoorAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode);
  }

  boost::optional<Connection> AirTerminalDualDuctVAVOutdoorAir_Impl::recirculatedAirInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode);
  }*/

  unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::outletPort() const {
    return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode;
  }

  unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::inletPort(unsigned branchIndex) const {
    if( branchIndex == 0 ) {
      return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode;
    } else if( branchIndex == 1 ) {
      return OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode;
    } else {
      LOG(Warn, "Calling inletPort with branchIndex greater than 1 for " << briefDescription() << " is not valid.");
      LOG(Warn, briefDescription() << " has only two branches.");
      return std::numeric_limits<unsigned>::max();
    }
  }

  unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::nextInletPort() const {
    LOG(Warn, "nextInletPort is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
    return std::numeric_limits<unsigned>::max();
  }

  unsigned AirTerminalDualDuctVAVOutdoorAir_Impl::newInletPortAfterBranch(unsigned branchIndex) {
    LOG(Warn, "newInletPortAfterBranch is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
    return std::numeric_limits<unsigned>::max();
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::removePortForBranch(unsigned branchIndex) {
    LOG(Warn, "removePortForBranch is not supported for " << briefDescription() << " .");
    LOG(Warn, "Ports cannot be added or removed for " << briefDescription() << " .");
  }

  boost::optional<double> AirTerminalDualDuctVAVOutdoorAir_Impl::maximumTerminalAirFlowRate() const {
    return getDouble(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate,true);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::isMaximumTerminalAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  /*DesignSpecificationOutdoorAir AirTerminalDualDuctVAVOutdoorAir_Impl::designSpecificationOutdoorAirObject() const {
    boost::optional<DesignSpecificationOutdoorAir> value = optionalDesignSpecificationOutdoorAirObject();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Design Specification Outdoor Air Object attached.");
    }
    return value.get();
  }*/

  std::string AirTerminalDualDuctVAVOutdoorAir_Impl::perPersonVentilationRateMode() const {
    boost::optional<std::string> value = getString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule,
                              "AirTerminalDualDuctVAVOutdoorAir",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  /*
  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetAirOutletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setOutdoorAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetOutdoorAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode, "");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setRecirculatedAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode, connection.handle());
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::resetRecirculatedAirInletNode() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode, "");
    OS_ASSERT(result);
  }
    */

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate) {
    bool result = setDouble(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, maximumTerminalAirFlowRate);
    return result;
  }

  void AirTerminalDualDuctVAVOutdoorAir_Impl::autosizeMaximumTerminalAirFlowRate() {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) {
    bool result = setPointer(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObject, designSpecificationOutdoorAir.handle());
    return result;
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode) {
    bool result = setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode, perPersonVentilationRateMode);
    return result;
  }

  boost::optional<DesignSpecificationOutdoorAir> AirTerminalDualDuctVAVOutdoorAir_Impl::optionalDesignSpecificationOutdoorAirObject() const {
    return getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObject);
  }

  boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir_Impl::OutdoorAirInletNode() const {
    boost::optional<Node> node;
    if( auto mo = inletModelObject(0) ) {
      node = mo->optionalCast<Node>();
      OS_ASSERT(node);
    }
    return node;
  }

  boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir_Impl::RecirculatedAirInletNode() const {
    boost::optional<Node> node;
    if( auto mo = inletModelObject(1) ) {
      node = mo->optionalCast<Node>();
      OS_ASSERT(node);
    }
    return node;
  }

  std::vector<IdfObject> AirTerminalDualDuctVAVOutdoorAir_Impl::remove() {
    auto modelObject = getObject<AirTerminalDualDuctVAVOutdoorAir>();
    AirLoopHVAC_Impl::removeDualDuctTerminalFromAirLoopHVAC(modelObject,inletPort(0),inletPort(1),outletPort());
    return Mixer_Impl::remove();
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::addToNode(Node & node)
  {
    auto mo = getObject<AirTerminalDualDuctVAVOutdoorAir>();
    return AirLoopHVAC_Impl::addDualDuctTerminalToNode(mo,inletPort(0),inletPort(1),outletPort(),node);
  }

  ModelObject AirTerminalDualDuctVAVOutdoorAir_Impl::clone(Model model) const
  {
    auto t_clone = Mixer_Impl::clone(model).cast<AirTerminalDualDuctVAVOutdoorAir>();

    t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNode,"");
    t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNode,"");
    t_clone.setString(OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNode,"");

    return t_clone;
  }

  bool AirTerminalDualDuctVAVOutdoorAir_Impl::isRemovable() const
  {
    return HVACComponent_Impl::isRemovable();
  }

} // detail

AirTerminalDualDuctVAVOutdoorAir::AirTerminalDualDuctVAVOutdoorAir(const Model& model)
  : Mixer(AirTerminalDualDuctVAVOutdoorAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObject
  bool ok = true;
  // ok = setMaximumTerminalAirFlowRate();
  //OS_ASSERT(ok);
  // ok = setDesignSpecificationOutdoorAirObject();
  //OS_ASSERT(ok);
  // ok = setPerPersonVentilationRateMode();
  //OS_ASSERT(ok);
  autosizeMaximumTerminalAirFlowRate();
  setPerPersonVentilationRateMode("CurrentOccupancy");
}

IddObjectType AirTerminalDualDuctVAVOutdoorAir::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir);
}

std::vector<std::string> AirTerminalDualDuctVAVOutdoorAir::perPersonVentilationRateModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode);
}

boost::optional<Schedule> AirTerminalDualDuctVAVOutdoorAir::availabilitySchedule() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->availabilitySchedule();
}

boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir::OutdoorAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->OutdoorAirInletNode();
}

boost::optional<Node> AirTerminalDualDuctVAVOutdoorAir::RecirculatedAirInletNode() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->RecirculatedAirInletNode();
}

boost::optional<double> AirTerminalDualDuctVAVOutdoorAir::maximumTerminalAirFlowRate() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->maximumTerminalAirFlowRate();
}

bool AirTerminalDualDuctVAVOutdoorAir::isMaximumTerminalAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->isMaximumTerminalAirFlowRateAutosized();
}

/*DesignSpecificationOutdoorAir AirTerminalDualDuctVAVOutdoorAir::designSpecificationOutdoorAirObject() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->designSpecificationOutdoorAirObject();
}*/

std::string AirTerminalDualDuctVAVOutdoorAir::perPersonVentilationRateMode() const {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->perPersonVentilationRateMode();
}

bool AirTerminalDualDuctVAVOutdoorAir::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalDualDuctVAVOutdoorAir::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->resetAvailabilitySchedule();
}

/*bool AirTerminalDualDuctVAVOutdoorAir::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setAirOutletNode(connection);
}

void AirTerminalDualDuctVAVOutdoorAir::resetAirOutletNode() {
  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->resetAirOutletNode();
}

bool AirTerminalDualDuctVAVOutdoorAir::setOutdoorAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setOutdoorAirInletNode(connection);
}

void AirTerminalDualDuctVAVOutdoorAir::resetOutdoorAirInletNode() {
  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->resetOutdoorAirInletNode();
}

bool AirTerminalDualDuctVAVOutdoorAir::setRecirculatedAirInletNode(const Connection& connection) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setRecirculatedAirInletNode(connection);
}

void AirTerminalDualDuctVAVOutdoorAir::resetRecirculatedAirInletNode() {
  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->resetRecirculatedAirInletNode();
}*/

bool AirTerminalDualDuctVAVOutdoorAir::setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setMaximumTerminalAirFlowRate(maximumTerminalAirFlowRate);
}

void AirTerminalDualDuctVAVOutdoorAir::autosizeMaximumTerminalAirFlowRate() {
  getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->autosizeMaximumTerminalAirFlowRate();
}

bool AirTerminalDualDuctVAVOutdoorAir::setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setDesignSpecificationOutdoorAirObject(designSpecificationOutdoorAir);
}

bool AirTerminalDualDuctVAVOutdoorAir::setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode) {
  return getImpl<detail::AirTerminalDualDuctVAVOutdoorAir_Impl>()->setPerPersonVentilationRateMode(perPersonVentilationRateMode);
}

/// @cond
AirTerminalDualDuctVAVOutdoorAir::AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl)
  : Mixer(impl)
{}
/// @endcond

} // model
} // openstudio

