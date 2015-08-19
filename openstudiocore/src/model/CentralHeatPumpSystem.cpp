/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "CentralHeatPumpSystem.hpp"
#include "CentralHeatPumpSystem_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_CentralHeatPumpSystem_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
  }

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
  }

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CentralHeatPumpSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CentralHeatPumpSystem_Impl::iddObjectType() const {
    return CentralHeatPumpSystem::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CentralHeatPumpSystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CentralHeatPumpSystem","Ancillary Operation"));
    }
    return result;
  }

  std::string CentralHeatPumpSystem_Impl::controlMethod() const {
    boost::optional<std::string> value = getString(OS_CentralHeatPumpSystemFields::ControlMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::coolingLoopInletNode() const {
    boost::optional<Connection> value = optionalCoolingLoopInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Loop Inlet Node attached.");
    }
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::coolingLoopOutletNode() const {
    boost::optional<Connection> value = optionalCoolingLoopOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Loop Outlet Node attached.");
    }
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::sourceLoopInletNode() const {
    boost::optional<Connection> value = optionalSourceLoopInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Source Loop Inlet Node attached.");
    }
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::sourceLoopOutletNode() const {
    boost::optional<Connection> value = optionalSourceLoopOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Source Loop Outlet Node attached.");
    }
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::heatingLoopInletNode() const {
    boost::optional<Connection> value = optionalHeatingLoopInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Loop Inlet Node attached.");
    }
    return value.get();
  }

  Connection CentralHeatPumpSystem_Impl::heatingLoopOutletNode() const {
    boost::optional<Connection> value = optionalHeatingLoopOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Loop Outlet Node attached.");
    }
    return value.get();
  }

  double CentralHeatPumpSystem_Impl::ancillaryPower() const {
    boost::optional<double> value = getDouble(OS_CentralHeatPumpSystemFields::AncillaryPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> CentralHeatPumpSystem_Impl::ancillaryOperationSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName);
  }

  bool CentralHeatPumpSystem_Impl::setControlMethod(std::string controlMethod) {
    bool result = setString(OS_CentralHeatPumpSystemFields::ControlMethod, controlMethod);
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setCoolingLoopInletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setCoolingLoopOutletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setSourceLoopInletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setSourceLoopOutletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setHeatingLoopInletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setHeatingLoopOutletNode(const Connection& connection) {
    bool result = setPointer(OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName, connection.handle());
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setAncillaryPower(double ancillaryPower) {
    bool result = setDouble(OS_CentralHeatPumpSystemFields::AncillaryPower, ancillaryPower);
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setAncillaryOperationSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName,
                              "CentralHeatPumpSystem",
                              "Ancillary Operation",
                              schedule);
    return result;
  }

  void CentralHeatPumpSystem_Impl::resetAncillaryOperationSchedule() {
    bool result = setString(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalCoolingLoopInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalCoolingLoopOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalSourceLoopInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalSourceLoopOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalHeatingLoopInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName);
  }

  boost::optional<Connection> CentralHeatPumpSystem_Impl::optionalHeatingLoopOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName);
  }

} // detail

CentralHeatPumpSystem::CentralHeatPumpSystem(const Model& model)
  : WaterToWaterComponent(CentralHeatPumpSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CentralHeatPumpSystem_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName
  //     OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName
  //     OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName
  //     OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName
  //     OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName
  //     OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setControlMethod();
  OS_ASSERT(ok);
  // ok = setCoolingLoopInletNode();
  OS_ASSERT(ok);
  // ok = setCoolingLoopOutletNode();
  OS_ASSERT(ok);
  // ok = setSourceLoopInletNode();
  OS_ASSERT(ok);
  // ok = setSourceLoopOutletNode();
  OS_ASSERT(ok);
  // ok = setHeatingLoopInletNode();
  OS_ASSERT(ok);
  // ok = setHeatingLoopOutletNode();
  OS_ASSERT(ok);
  // ok = setAncillaryPower();
  OS_ASSERT(ok);
}

IddObjectType CentralHeatPumpSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CentralHeatPumpSystem);
}

std::vector<std::string> CentralHeatPumpSystem::controlMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CentralHeatPumpSystemFields::ControlMethod);
}

std::string CentralHeatPumpSystem::controlMethod() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->controlMethod();
}

Connection CentralHeatPumpSystem::coolingLoopInletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->coolingLoopInletNode();
}

Connection CentralHeatPumpSystem::coolingLoopOutletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->coolingLoopOutletNode();
}

Connection CentralHeatPumpSystem::sourceLoopInletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->sourceLoopInletNode();
}

Connection CentralHeatPumpSystem::sourceLoopOutletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->sourceLoopOutletNode();
}

Connection CentralHeatPumpSystem::heatingLoopInletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->heatingLoopInletNode();
}

Connection CentralHeatPumpSystem::heatingLoopOutletNode() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->heatingLoopOutletNode();
}

double CentralHeatPumpSystem::ancillaryPower() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryPower();
}

boost::optional<Schedule> CentralHeatPumpSystem::ancillaryOperationSchedule() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryOperationSchedule();
}

bool CentralHeatPumpSystem::setControlMethod(std::string controlMethod) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setControlMethod(controlMethod);
}

bool CentralHeatPumpSystem::setCoolingLoopInletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setCoolingLoopInletNode(connection);
}

bool CentralHeatPumpSystem::setCoolingLoopOutletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setCoolingLoopOutletNode(connection);
}

bool CentralHeatPumpSystem::setSourceLoopInletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setSourceLoopInletNode(connection);
}

bool CentralHeatPumpSystem::setSourceLoopOutletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setSourceLoopOutletNode(connection);
}

bool CentralHeatPumpSystem::setHeatingLoopInletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setHeatingLoopInletNode(connection);
}

bool CentralHeatPumpSystem::setHeatingLoopOutletNode(const Connection& connection) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setHeatingLoopOutletNode(connection);
}

bool CentralHeatPumpSystem::setAncillaryPower(double ancillaryPower) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryPower(ancillaryPower);
}

bool CentralHeatPumpSystem::setAncillaryOperationSchedule(Schedule& schedule) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryOperationSchedule(schedule);
}

void CentralHeatPumpSystem::resetAncillaryOperationSchedule() {
  getImpl<detail::CentralHeatPumpSystem_Impl>()->resetAncillaryOperationSchedule();
}

/// @cond
CentralHeatPumpSystem::CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl)
  : WaterToWaterComponent(impl)
{}
/// @endcond

} // model
} // openstudio

