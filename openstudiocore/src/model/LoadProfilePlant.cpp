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

#include "LoadProfilePlant.hpp"
#include "LoadProfilePlant_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_LoadProfile_Plant_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  LoadProfilePlant_Impl::LoadProfilePlant_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LoadProfilePlant::iddObjectType());
  }

  LoadProfilePlant_Impl::LoadProfilePlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LoadProfilePlant::iddObjectType());
  }

  LoadProfilePlant_Impl::LoadProfilePlant_Impl(const LoadProfilePlant_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& LoadProfilePlant_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType LoadProfilePlant_Impl::iddObjectType() const {
    return LoadProfilePlant::iddObjectType();
  }

  std::vector<ScheduleTypeKey> LoadProfilePlant_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_LoadProfile_PlantFields::LoadScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("LoadProfilePlant","Load"));
    }
    if (std::find(b,e,OS_LoadProfile_PlantFields::FlowRateFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("LoadProfilePlant","Flow Rate Fraction"));
    }
    return result;
  }

  Connection LoadProfilePlant_Impl::inletNode() const {
    boost::optional<Connection> value = optionalInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Inlet Node attached.");
    }
    return value.get();
  }

  Connection LoadProfilePlant_Impl::outletNode() const {
    boost::optional<Connection> value = optionalOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Outlet Node attached.");
    }
    return value.get();
  }

  Schedule LoadProfilePlant_Impl::loadSchedule() const {
    boost::optional<Schedule> value = optionalLoadSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Load Schedule attached.");
    }
    return value.get();
  }

  double LoadProfilePlant_Impl::peakFlowRate() const {
    boost::optional<double> value = getDouble(OS_LoadProfile_PlantFields::PeakFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule LoadProfilePlant_Impl::flowRateFractionSchedule() const {
    boost::optional<Schedule> value = optionalFlowRateFractionSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Flow Rate Fraction Schedule attached.");
    }
    return value.get();
  }

  bool LoadProfilePlant_Impl::setInletNode(const Connection& connection) {
    bool result = setPointer(OS_LoadProfile_PlantFields::InletNodeName, connection.handle());
    return result;
  }

  bool LoadProfilePlant_Impl::setOutletNode(const Connection& connection) {
    bool result = setPointer(OS_LoadProfile_PlantFields::OutletNodeName, connection.handle());
    return result;
  }

  bool LoadProfilePlant_Impl::setLoadSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_LoadProfile_PlantFields::LoadScheduleName,
                              "LoadProfilePlant",
                              "Load",
                              schedule);
    return result;
  }

  void LoadProfilePlant_Impl::setPeakFlowRate(double peakFlowRate) {
    bool result = setDouble(OS_LoadProfile_PlantFields::PeakFlowRate, peakFlowRate);
    OS_ASSERT(result);
  }

  bool LoadProfilePlant_Impl::setFlowRateFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_LoadProfile_PlantFields::FlowRateFractionScheduleName,
                              "LoadProfilePlant",
                              "Flow Rate Fraction",
                              schedule);
    return result;
  }

  boost::optional<Connection> LoadProfilePlant_Impl::optionalInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_LoadProfile_PlantFields::InletNodeName);
  }

  boost::optional<Connection> LoadProfilePlant_Impl::optionalOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_LoadProfile_PlantFields::OutletNodeName);
  }

  boost::optional<Schedule> LoadProfilePlant_Impl::optionalLoadSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::LoadScheduleName);
  }

  boost::optional<Schedule> LoadProfilePlant_Impl::optionalFlowRateFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::FlowRateFractionScheduleName);
  }

} // detail

LoadProfilePlant::LoadProfilePlant(const Model& model)
  : StraightComponent(LoadProfilePlant::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LoadProfilePlant_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_LoadProfile_PlantFields::InletNodeName
  //     OS_LoadProfile_PlantFields::OutletNodeName
  //     OS_LoadProfile_PlantFields::LoadScheduleName
  //     OS_LoadProfile_PlantFields::FlowRateFractionScheduleName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setInletNode();
  OS_ASSERT(ok);
  // ok = setOutletNode();
  OS_ASSERT(ok);
  // ok = setLoadSchedule();
  OS_ASSERT(ok);
  // setPeakFlowRate();
  // ok = setFlowRateFractionSchedule();
  OS_ASSERT(ok);
}

IddObjectType LoadProfilePlant::iddObjectType() {
  return IddObjectType(IddObjectType::OS_LoadProfile_Plant);
}

Connection LoadProfilePlant::inletNode() const {
  return getImpl<detail::LoadProfilePlant_Impl>()->inletNode();
}

Connection LoadProfilePlant::outletNode() const {
  return getImpl<detail::LoadProfilePlant_Impl>()->outletNode();
}

Schedule LoadProfilePlant::loadSchedule() const {
  return getImpl<detail::LoadProfilePlant_Impl>()->loadSchedule();
}

double LoadProfilePlant::peakFlowRate() const {
  return getImpl<detail::LoadProfilePlant_Impl>()->peakFlowRate();
}

Schedule LoadProfilePlant::flowRateFractionSchedule() const {
  return getImpl<detail::LoadProfilePlant_Impl>()->flowRateFractionSchedule();
}

bool LoadProfilePlant::setInletNode(const Connection& connection) {
  return getImpl<detail::LoadProfilePlant_Impl>()->setInletNode(connection);
}

bool LoadProfilePlant::setOutletNode(const Connection& connection) {
  return getImpl<detail::LoadProfilePlant_Impl>()->setOutletNode(connection);
}

bool LoadProfilePlant::setLoadSchedule(Schedule& schedule) {
  return getImpl<detail::LoadProfilePlant_Impl>()->setLoadSchedule(schedule);
}

void LoadProfilePlant::setPeakFlowRate(double peakFlowRate) {
  getImpl<detail::LoadProfilePlant_Impl>()->setPeakFlowRate(peakFlowRate);
}

bool LoadProfilePlant::setFlowRateFractionSchedule(Schedule& schedule) {
  return getImpl<detail::LoadProfilePlant_Impl>()->setFlowRateFractionSchedule(schedule);
}

/// @cond
LoadProfilePlant::LoadProfilePlant(std::shared_ptr<detail::LoadProfilePlant_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

