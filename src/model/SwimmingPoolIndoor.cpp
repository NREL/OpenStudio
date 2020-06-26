/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "SwimmingPoolIndoor.hpp"
#include "SwimmingPoolIndoor_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "FloorSurface.hpp"
#include "FloorSurface_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SwimmingPool_Indoor_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SwimmingPoolIndoor_Impl::SwimmingPoolIndoor_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SwimmingPoolIndoor::iddObjectType());
  }

  SwimmingPoolIndoor_Impl::SwimmingPoolIndoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SwimmingPoolIndoor::iddObjectType());
  }

  SwimmingPoolIndoor_Impl::SwimmingPoolIndoor_Impl(const SwimmingPoolIndoor_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SwimmingPoolIndoor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SwimmingPoolIndoor_Impl::iddObjectType() const {
    return SwimmingPoolIndoor::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SwimmingPoolIndoor_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::ActivityFactorScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","Activity Factor"));
    }
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","Make-up Water Supply"));
    }
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::CoverScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","Cover"));
    }
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::SetpointTemperatureSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","Setpoint Temperature Schedule"));
    }
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::PeopleSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","People Schedule"));
    }
    if (std::find(b,e,OS_SwimmingPool_IndoorFields::PeopleHeatGainSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("SwimmingPoolIndoor","People Heat Gain Schedule"));
    }
    return result;
  }

  FloorSurface SwimmingPoolIndoor_Impl::surface() const {
    boost::optional<FloorSurface> value = optionalSurface();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
    }
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::averageDepth() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::AverageDepth,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::activityFactorSchedule() const {
    boost::optional<Schedule> value = optionalActivityFactorSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Activity Factor Schedule attached.");
    }
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::makeupWaterSupplySchedule() const {
    boost::optional<Schedule> value = optionalMakeupWaterSupplySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Makeup Water Supply Schedule attached.");
    }
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::coverSchedule() const {
    boost::optional<Schedule> value = optionalCoverSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cover Schedule attached.");
    }
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::coverEvaporationFactor() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::CoverEvaporationFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::coverConvectionFactor() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::CoverConvectionFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::coverShortWavelengthRadiationFactor() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::CoverShortWavelengthRadiationFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::coverLongWavelengthRadiationFactor() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::CoverLongWavelengthRadiationFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Connection SwimmingPoolIndoor_Impl::poolWaterInletNode() const {
    boost::optional<Connection> value = optionalPoolWaterInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Pool Water Inlet Node attached.");
    }
    return value.get();
  }

  Connection SwimmingPoolIndoor_Impl::poolWaterOutletNode() const {
    boost::optional<Connection> value = optionalPoolWaterOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Pool Water Outlet Node attached.");
    }
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::poolHeatingSystemMaximumWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::PoolHeatingSystemMaximumWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::poolMiscellaneousEquipmentPower() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::PoolMiscellaneousEquipmentPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::setpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalSetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double SwimmingPoolIndoor_Impl::maximumNumberofPeople() const {
    boost::optional<double> value = getDouble(OS_SwimmingPool_IndoorFields::MaximumNumberofPeople,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::peopleSchedule() const {
    boost::optional<Schedule> value = optionalPeopleSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an People Schedule attached.");
    }
    return value.get();
  }

  Schedule SwimmingPoolIndoor_Impl::peopleHeatGainSchedule() const {
    boost::optional<Schedule> value = optionalPeopleHeatGainSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an People Heat Gain Schedule attached.");
    }
    return value.get();
  }

  bool SwimmingPoolIndoor_Impl::setSurface(const FloorSurface& floorSurface) {
    bool result = setPointer(OS_SwimmingPool_IndoorFields::SurfaceName, floorSurface.handle());
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setAverageDepth(double averageDepth) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::AverageDepth, averageDepth);
    OS_ASSERT(result);
  }

  bool SwimmingPoolIndoor_Impl::setActivityFactorSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::ActivityFactorScheduleName,
                              "SwimmingPoolIndoor",
                              "Activity Factor",
                              schedule);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setMakeupWaterSupplySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName,
                              "SwimmingPoolIndoor",
                              "Make-up Water Supply",
                              schedule);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setCoverSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::CoverScheduleName,
                              "SwimmingPoolIndoor",
                              "Cover",
                              schedule);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setCoverEvaporationFactor(double coverEvaporationFactor) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::CoverEvaporationFactor, coverEvaporationFactor);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setCoverConvectionFactor(double coverConvectionFactor) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::CoverConvectionFactor, coverConvectionFactor);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::CoverShortWavelengthRadiationFactor, coverShortWavelengthRadiationFactor);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::CoverLongWavelengthRadiationFactor, coverLongWavelengthRadiationFactor);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPoolWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_SwimmingPool_IndoorFields::PoolWaterInletNode, connection.handle());
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPoolWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_SwimmingPool_IndoorFields::PoolWaterOutletNode, connection.handle());
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::PoolHeatingSystemMaximumWaterFlowRate, poolHeatingSystemMaximumWaterFlowRate);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::PoolMiscellaneousEquipmentPower, poolMiscellaneousEquipmentPower);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::SetpointTemperatureSchedule,
                              "SwimmingPoolIndoor",
                              "Setpoint Temperature Schedule",
                              schedule);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setMaximumNumberofPeople(double maximumNumberofPeople) {
    bool result = setDouble(OS_SwimmingPool_IndoorFields::MaximumNumberofPeople, maximumNumberofPeople);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPeopleSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::PeopleSchedule,
                              "SwimmingPoolIndoor",
                              "People Schedule",
                              schedule);
    return result;
  }

  bool SwimmingPoolIndoor_Impl::setPeopleHeatGainSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SwimmingPool_IndoorFields::PeopleHeatGainSchedule,
                              "SwimmingPoolIndoor",
                              "People Heat Gain Schedule",
                              schedule);
    return result;
  }

  boost::optional<FloorSurface> SwimmingPoolIndoor_Impl::optionalSurface() const {
    return getObject<ModelObject>().getModelObjectTarget<FloorSurface>(OS_SwimmingPool_IndoorFields::SurfaceName);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalActivityFactorSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::ActivityFactorScheduleName);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalMakeupWaterSupplySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalCoverSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::CoverScheduleName);
  }

  boost::optional<Connection> SwimmingPoolIndoor_Impl::optionalPoolWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SwimmingPool_IndoorFields::PoolWaterInletNode);
  }

  boost::optional<Connection> SwimmingPoolIndoor_Impl::optionalPoolWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SwimmingPool_IndoorFields::PoolWaterOutletNode);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::SetpointTemperatureSchedule);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalPeopleSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::PeopleSchedule);
  }

  boost::optional<Schedule> SwimmingPoolIndoor_Impl::optionalPeopleHeatGainSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SwimmingPool_IndoorFields::PeopleHeatGainSchedule);
  }

} // detail

SwimmingPoolIndoor::SwimmingPoolIndoor(const Model& model)
  : StraightComponent(SwimmingPoolIndoor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SwimmingPoolIndoor_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SwimmingPool_IndoorFields::SurfaceName
  //     OS_SwimmingPool_IndoorFields::ActivityFactorScheduleName
  //     OS_SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName
  //     OS_SwimmingPool_IndoorFields::CoverScheduleName
  //     OS_SwimmingPool_IndoorFields::PoolWaterInletNode
  //     OS_SwimmingPool_IndoorFields::PoolWaterOutletNode
  //     OS_SwimmingPool_IndoorFields::SetpointTemperatureSchedule
  //     OS_SwimmingPool_IndoorFields::PeopleSchedule
  //     OS_SwimmingPool_IndoorFields::PeopleHeatGainSchedule
  bool ok = true;
  // ok = setSurface();
  OS_ASSERT(ok);
  // setAverageDepth();
  // ok = setActivityFactorSchedule();
  OS_ASSERT(ok);
  // ok = setMakeupWaterSupplySchedule();
  OS_ASSERT(ok);
  // ok = setCoverSchedule();
  OS_ASSERT(ok);
  // ok = setCoverEvaporationFactor();
  OS_ASSERT(ok);
  // ok = setCoverConvectionFactor();
  OS_ASSERT(ok);
  // ok = setCoverShortWavelengthRadiationFactor();
  OS_ASSERT(ok);
  // ok = setCoverLongWavelengthRadiationFactor();
  OS_ASSERT(ok);
  // ok = setPoolWaterInletNode();
  OS_ASSERT(ok);
  // ok = setPoolWaterOutletNode();
  OS_ASSERT(ok);
  // ok = setPoolHeatingSystemMaximumWaterFlowRate();
  OS_ASSERT(ok);
  // ok = setPoolMiscellaneousEquipmentPower();
  OS_ASSERT(ok);
  // ok = setSetpointTemperatureSchedule();
  OS_ASSERT(ok);
  // ok = setMaximumNumberofPeople();
  OS_ASSERT(ok);
  // ok = setPeopleSchedule();
  OS_ASSERT(ok);
  // ok = setPeopleHeatGainSchedule();
  OS_ASSERT(ok);
}

IddObjectType SwimmingPoolIndoor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SwimmingPool_Indoor);
}

FloorSurface SwimmingPoolIndoor::surface() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->surface();
}

double SwimmingPoolIndoor::averageDepth() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->averageDepth();
}

Schedule SwimmingPoolIndoor::activityFactorSchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->activityFactorSchedule();
}

Schedule SwimmingPoolIndoor::makeupWaterSupplySchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->makeupWaterSupplySchedule();
}

Schedule SwimmingPoolIndoor::coverSchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->coverSchedule();
}

double SwimmingPoolIndoor::coverEvaporationFactor() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->coverEvaporationFactor();
}

double SwimmingPoolIndoor::coverConvectionFactor() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->coverConvectionFactor();
}

double SwimmingPoolIndoor::coverShortWavelengthRadiationFactor() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->coverShortWavelengthRadiationFactor();
}

double SwimmingPoolIndoor::coverLongWavelengthRadiationFactor() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->coverLongWavelengthRadiationFactor();
}

Connection SwimmingPoolIndoor::poolWaterInletNode() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->poolWaterInletNode();
}

Connection SwimmingPoolIndoor::poolWaterOutletNode() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->poolWaterOutletNode();
}

double SwimmingPoolIndoor::poolHeatingSystemMaximumWaterFlowRate() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->poolHeatingSystemMaximumWaterFlowRate();
}

double SwimmingPoolIndoor::poolMiscellaneousEquipmentPower() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->poolMiscellaneousEquipmentPower();
}

Schedule SwimmingPoolIndoor::setpointTemperatureSchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setpointTemperatureSchedule();
}

double SwimmingPoolIndoor::maximumNumberofPeople() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->maximumNumberofPeople();
}

Schedule SwimmingPoolIndoor::peopleSchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->peopleSchedule();
}

Schedule SwimmingPoolIndoor::peopleHeatGainSchedule() const {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->peopleHeatGainSchedule();
}

bool SwimmingPoolIndoor::setSurface(const FloorSurface& floorSurface) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setSurface(floorSurface);
}

void SwimmingPoolIndoor::setAverageDepth(double averageDepth) {
  getImpl<detail::SwimmingPoolIndoor_Impl>()->setAverageDepth(averageDepth);
}

bool SwimmingPoolIndoor::setActivityFactorSchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setActivityFactorSchedule(schedule);
}

bool SwimmingPoolIndoor::setMakeupWaterSupplySchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setMakeupWaterSupplySchedule(schedule);
}

bool SwimmingPoolIndoor::setCoverSchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setCoverSchedule(schedule);
}

bool SwimmingPoolIndoor::setCoverEvaporationFactor(double coverEvaporationFactor) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setCoverEvaporationFactor(coverEvaporationFactor);
}

bool SwimmingPoolIndoor::setCoverConvectionFactor(double coverConvectionFactor) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setCoverConvectionFactor(coverConvectionFactor);
}

bool SwimmingPoolIndoor::setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setCoverShortWavelengthRadiationFactor(coverShortWavelengthRadiationFactor);
}

bool SwimmingPoolIndoor::setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setCoverLongWavelengthRadiationFactor(coverLongWavelengthRadiationFactor);
}

bool SwimmingPoolIndoor::setPoolWaterInletNode(const Connection& connection) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPoolWaterInletNode(connection);
}

bool SwimmingPoolIndoor::setPoolWaterOutletNode(const Connection& connection) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPoolWaterOutletNode(connection);
}

bool SwimmingPoolIndoor::setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPoolHeatingSystemMaximumWaterFlowRate(poolHeatingSystemMaximumWaterFlowRate);
}

bool SwimmingPoolIndoor::setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPoolMiscellaneousEquipmentPower(poolMiscellaneousEquipmentPower);
}

bool SwimmingPoolIndoor::setSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setSetpointTemperatureSchedule(schedule);
}

bool SwimmingPoolIndoor::setMaximumNumberofPeople(double maximumNumberofPeople) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setMaximumNumberofPeople(maximumNumberofPeople);
}

bool SwimmingPoolIndoor::setPeopleSchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPeopleSchedule(schedule);
}

bool SwimmingPoolIndoor::setPeopleHeatGainSchedule(Schedule& schedule) {
  return getImpl<detail::SwimmingPoolIndoor_Impl>()->setPeopleHeatGainSchedule(schedule);
}

/// @cond
SwimmingPoolIndoor::SwimmingPoolIndoor(std::shared_ptr<detail::SwimmingPoolIndoor_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

