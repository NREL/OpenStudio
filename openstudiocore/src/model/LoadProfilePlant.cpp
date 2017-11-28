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

#include "LoadProfilePlant.hpp"
#include "LoadProfilePlant_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleDay.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "../utilities/time/Time.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

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

  unsigned LoadProfilePlant_Impl::inletPort()
  {
    return OS_LoadProfile_PlantFields::InletNodeName;
  }

  unsigned LoadProfilePlant_Impl::outletPort()
  {
    return OS_LoadProfile_PlantFields::OutletNodeName;
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

  boost::optional<Schedule> LoadProfilePlant_Impl::optionalLoadSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::LoadScheduleName);
  }

  boost::optional<Schedule> LoadProfilePlant_Impl::optionalFlowRateFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::FlowRateFractionScheduleName);
  }

  bool LoadProfilePlant_Impl::addToNode(Node & node)
  {
    if( auto plant = node.plantLoop() ) {
      if( plant->demandComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode(node);
      }
    }
    return false;
  }

} // detail

LoadProfilePlant::LoadProfilePlant(const Model& model)
  : StraightComponent(LoadProfilePlant::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LoadProfilePlant_Impl>());

  auto alwaysOn = model.alwaysOnDiscreteSchedule();

  auto scheduleRuleset = ScheduleRuleset(model);
  auto scheduleDay = scheduleRuleset.defaultDaySchedule();
  scheduleDay.addValue(Time(0,4),8000);
  scheduleDay.addValue(Time(0,8),6000);
  scheduleDay.addValue(Time(0,9),0);
  scheduleDay.addValue(Time(0,12),6000);
  scheduleDay.addValue(Time(0,24),10000);

  bool ok = true;
  ok = setLoadSchedule( scheduleRuleset );
  OS_ASSERT(ok);
  setPeakFlowRate( 0.002 );
  ok = setFlowRateFractionSchedule( alwaysOn );
  OS_ASSERT(ok);
}

LoadProfilePlant::LoadProfilePlant(const Model& model, Schedule& loadSchedule, Schedule& flowRateFractionSchedule)
  : StraightComponent(LoadProfilePlant::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LoadProfilePlant_Impl>());

  setLoadSchedule( loadSchedule );
  setPeakFlowRate( 0.0 );
  setFlowRateFractionSchedule( flowRateFractionSchedule );
}

IddObjectType LoadProfilePlant::iddObjectType() {
  return IddObjectType(IddObjectType::OS_LoadProfile_Plant);
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
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

