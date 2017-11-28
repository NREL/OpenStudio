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

#include "FanZoneExhaust.hpp"
#include "FanZoneExhaust_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Fan_ZoneExhaust_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanZoneExhaust::iddObjectType());
  }

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanZoneExhaust::iddObjectType());
  }

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const FanZoneExhaust_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanZoneExhaust_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanZoneExhaust_Impl::iddObjectType() const {
    return FanZoneExhaust::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanZoneExhaust_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Availability"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::FlowFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Flow Fraction"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Minimum Zone Temperature Limit"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Balanced Exhaust Fraction"));
    }
    return result;
  }

  boost::optional<ThermalZone> FanZoneExhaust_Impl::thermalZone()
  {
    boost::optional<ThermalZone> result;

    if( boost::optional<ModelObject> mo1 = connectedObject(inletPort()) )
    {
      if( boost::optional<Node> node = mo1->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> mo2 = node->inletModelObject() )
        {
          if( boost::optional<PortList> pl = mo2->optionalCast<PortList>() )
          {
            if( boost::optional<ThermalZone> tz = pl->thermalZone() )
            {
              result = tz;
            }
          }
        }
      }
    }

    return result;
  }

  bool FanZoneExhaust_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    // Zone Exhaust Node (Exhaust Fan Inlet node)

    Node exhaustNode(m);

    PortList exhaustPortList = thermalZone.exhaustPortList();

    unsigned nextPort = exhaustPortList.nextPort();

    m.connect(exhaustPortList,nextPort,exhaustNode,exhaustNode.inletPort());

    ModelObject mo = this->getObject<ModelObject>();

    m.connect(exhaustNode,exhaustNode.outletPort(),mo,this->inletPort());

    // Node (Exhaust Fan Outlet Node)

    Node exhaustFanOutletNode(m);

    m.connect(mo,this->outletPort(),exhaustFanOutletNode,exhaustFanOutletNode.inletPort());

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName);
  }

  double FanZoneExhaust_Impl::fanEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_ZoneExhaustFields::FanEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanZoneExhaust_Impl::pressureRise() const {
    boost::optional<double> value = getDouble(OS_Fan_ZoneExhaustFields::PressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanZoneExhaust_Impl::maximumFlowRate() const {
    return getDouble(OS_Fan_ZoneExhaustFields::MaximumFlowRate,true);
  }

  unsigned FanZoneExhaust_Impl::inletPort() const
  {
    return OS_Fan_ZoneExhaustFields::AirInletNodeName;
  }

  unsigned FanZoneExhaust_Impl::outletPort() const
  {
    return OS_Fan_ZoneExhaustFields::AirOutletNodeName;
  }

  std::string FanZoneExhaust_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_ZoneExhaustFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::flowFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName);
  }

  std::string FanZoneExhaust_Impl::systemAvailabilityManagerCouplingMode() const {
    boost::optional<std::string> value = getString(OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::minimumZoneTemperatureLimitSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName);
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::balancedExhaustFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName);
  }

  bool FanZoneExhaust_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName,
                              "FanZoneExhaust",
                              "Availability",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setFanEfficiency(double fanEfficiency) {
    bool result = setDouble(OS_Fan_ZoneExhaustFields::FanEfficiency, fanEfficiency);
    return result;
  }

  void FanZoneExhaust_Impl::setPressureRise(double pressureRise) {
    bool result = setDouble(OS_Fan_ZoneExhaustFields::PressureRise, pressureRise);
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result(false);
    if (maximumFlowRate) {
      result = setDouble(OS_Fan_ZoneExhaustFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

  void FanZoneExhaust_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_Fan_ZoneExhaustFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  void FanZoneExhaust_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Fan_ZoneExhaustFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setFlowFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName,
                              "FanZoneExhaust",
                              "Flow Fraction",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetFlowFractionSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode) {
    bool result = setString(OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode, systemAvailabilityManagerCouplingMode);
    return result;
  }

  bool FanZoneExhaust_Impl::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName,
                              "FanZoneExhaust",
                              "Minimum Zone Temperature Limit",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetMinimumZoneTemperatureLimitSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setBalancedExhaustFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName,
                              "FanZoneExhaust",
                              "Balanced Exhaust Fraction",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetBalancedExhaustFractionSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

FanZoneExhaust::FanZoneExhaust(const Model& model)
  : ZoneHVACComponent(FanZoneExhaust::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanZoneExhaust_Impl>());

  setFanEfficiency(0.60);
  setPressureRise(0);
  setEndUseSubcategory("General");
  setSystemAvailabilityManagerCouplingMode("Decoupled");
}

IddObjectType FanZoneExhaust::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Fan_ZoneExhaust);
}

std::vector<std::string> FanZoneExhaust::systemAvailabilityManagerCouplingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode);
}

boost::optional<Schedule> FanZoneExhaust::availabilitySchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->availabilitySchedule();
}

double FanZoneExhaust::fanEfficiency() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->fanEfficiency();
}

double FanZoneExhaust::pressureRise() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->pressureRise();
}

boost::optional<double> FanZoneExhaust::maximumFlowRate() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->maximumFlowRate();
}

std::string FanZoneExhaust::endUseSubcategory() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->endUseSubcategory();
}

boost::optional<Schedule> FanZoneExhaust::flowFractionSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->flowFractionSchedule();
}

std::string FanZoneExhaust::systemAvailabilityManagerCouplingMode() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->systemAvailabilityManagerCouplingMode();
}

boost::optional<Schedule> FanZoneExhaust::minimumZoneTemperatureLimitSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->minimumZoneTemperatureLimitSchedule();
}

boost::optional<Schedule> FanZoneExhaust::balancedExhaustFractionSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->balancedExhaustFractionSchedule();
}

bool FanZoneExhaust::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setAvailabilitySchedule(schedule);
}

void FanZoneExhaust::resetAvailabilitySchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetAvailabilitySchedule();
}

bool FanZoneExhaust::setFanEfficiency(double fanEfficiency) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setFanEfficiency(fanEfficiency);
}

void FanZoneExhaust::setPressureRise(double pressureRise) {
  getImpl<detail::FanZoneExhaust_Impl>()->setPressureRise(pressureRise);
}

bool FanZoneExhaust::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanZoneExhaust::resetMaximumFlowRate() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetMaximumFlowRate();
}

void FanZoneExhaust::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::FanZoneExhaust_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

bool FanZoneExhaust::setFlowFractionSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setFlowFractionSchedule(schedule);
}

void FanZoneExhaust::resetFlowFractionSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetFlowFractionSchedule();
}

bool FanZoneExhaust::setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setSystemAvailabilityManagerCouplingMode(systemAvailabilityManagerCouplingMode);
}

bool FanZoneExhaust::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setMinimumZoneTemperatureLimitSchedule(schedule);
}

void FanZoneExhaust::resetMinimumZoneTemperatureLimitSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetMinimumZoneTemperatureLimitSchedule();
}

bool FanZoneExhaust::setBalancedExhaustFractionSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setBalancedExhaustFractionSchedule(schedule);
}

void FanZoneExhaust::resetBalancedExhaustFractionSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetBalancedExhaustFractionSchedule();
}

/// @cond
FanZoneExhaust::FanZoneExhaust(std::shared_ptr<detail::FanZoneExhaust_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

