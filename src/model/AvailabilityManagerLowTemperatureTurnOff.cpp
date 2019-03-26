/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AvailabilityManagerLowTemperatureTurnOff.hpp"
#include "AvailabilityManagerLowTemperatureTurnOff_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_LowTemperatureTurnOff_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerLowTemperatureTurnOff::iddObjectType());
  }

  AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerLowTemperatureTurnOff::iddObjectType());
  }

  AvailabilityManagerLowTemperatureTurnOff_Impl::AvailabilityManagerLowTemperatureTurnOff_Impl(const AvailabilityManagerLowTemperatureTurnOff_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerLowTemperatureTurnOff_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      "Availability Manager Low Temperature Turn Off Control Status"
    };
    return result;
  }

  IddObjectType AvailabilityManagerLowTemperatureTurnOff_Impl::iddObjectType() const {
    return AvailabilityManagerLowTemperatureTurnOff::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AvailabilityManagerLowTemperatureTurnOff_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerLowTemperatureTurnOff","Applicability Schedule"));
    }
    return result;
  }



  boost::optional<Node> AvailabilityManagerLowTemperatureTurnOff_Impl::sensorNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName);
  }

  double AvailabilityManagerLowTemperatureTurnOff_Impl::temperature() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_LowTemperatureTurnOffFields::Temperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AvailabilityManagerLowTemperatureTurnOff_Impl::setSensorNode(const Node& node) {
    bool result = setPointer(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName, node.handle());
    return result;
  }

  void AvailabilityManagerLowTemperatureTurnOff_Impl::resetSensorNode() {
    bool result = setString(OS_AvailabilityManager_LowTemperatureTurnOffFields::SensorNodeName, "");
    OS_ASSERT(result);
  }

  bool AvailabilityManagerLowTemperatureTurnOff_Impl::setTemperature(double temperature) {
    bool result = setDouble(OS_AvailabilityManager_LowTemperatureTurnOffFields::Temperature, temperature);
    OS_ASSERT(result);
    return result;
  }

  Schedule AvailabilityManagerLowTemperatureTurnOff_Impl::applicabilitySchedule() const {
    boost::optional<Schedule> value = optionalApplicabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Applicability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<Schedule> AvailabilityManagerLowTemperatureTurnOff_Impl::optionalApplicabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName);
  }


  bool AvailabilityManagerLowTemperatureTurnOff_Impl::setApplicabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_LowTemperatureTurnOffFields::ApplicabilityScheduleName,
                              "AvailabilityManagerLowTemperatureTurnOff",
                              "Applicability Schedule",
                              schedule);
    return result;
  }


} // detail

AvailabilityManagerLowTemperatureTurnOff::AvailabilityManagerLowTemperatureTurnOff(const Model& model)
  : AvailabilityManager(AvailabilityManagerLowTemperatureTurnOff::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>());

  // Set Applicability Schedule to alwaysOnDiscreteSchedule
  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setApplicabilitySchedule(schedule);
  }
  setTemperature(30);
}

IddObjectType AvailabilityManagerLowTemperatureTurnOff::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOff);
}

boost::optional<Node> AvailabilityManagerLowTemperatureTurnOff::sensorNode() const {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->sensorNode();
}

double AvailabilityManagerLowTemperatureTurnOff::temperature() const {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->temperature();
}

bool AvailabilityManagerLowTemperatureTurnOff::setSensorNode(const Node& node) {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setSensorNode(node);
}

void AvailabilityManagerLowTemperatureTurnOff::resetSensorNode() {
  getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->resetSensorNode();
}

bool AvailabilityManagerLowTemperatureTurnOff::setTemperature(double temperature) {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setTemperature(temperature);
}

Schedule AvailabilityManagerLowTemperatureTurnOff::applicabilitySchedule() const {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->applicabilitySchedule();
}

bool AvailabilityManagerLowTemperatureTurnOff::setApplicabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerLowTemperatureTurnOff_Impl>()->setApplicabilitySchedule(schedule);
}


/// @cond
AvailabilityManagerLowTemperatureTurnOff::AvailabilityManagerLowTemperatureTurnOff(std::shared_ptr<detail::AvailabilityManagerLowTemperatureTurnOff_Impl> impl)
  : AvailabilityManager(impl)
{}
/// @endcond

} // model
} // openstudio

