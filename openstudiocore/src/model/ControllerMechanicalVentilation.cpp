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

#include "ControllerMechanicalVentilation.hpp"
#include "ControllerMechanicalVentilation_Impl.hpp"
#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Controller_MechanicalVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ControllerMechanicalVentilation_Impl::ControllerMechanicalVentilation_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ControllerMechanicalVentilation::iddObjectType());
  }

  ControllerMechanicalVentilation_Impl::ControllerMechanicalVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ControllerMechanicalVentilation::iddObjectType());
  }

  ControllerMechanicalVentilation_Impl::ControllerMechanicalVentilation_Impl(const ControllerMechanicalVentilation_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ControllerMechanicalVentilation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ControllerMechanicalVentilation_Impl::iddObjectType() const {
    return ControllerMechanicalVentilation::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ControllerMechanicalVentilation_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Controller_MechanicalVentilationFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ControllerMechanicalVentilation","Availability Schedule"));
    }
    return result;
  }

  Schedule ControllerMechanicalVentilation_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  bool ControllerMechanicalVentilation_Impl::demandControlledVentilation() const {
    boost::optional<std::string> value = getString(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ControllerMechanicalVentilation_Impl::isDemandControlledVentilationDefaulted() const {
    return isEmpty(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation);
  }

  std::string ControllerMechanicalVentilation_Impl::systemOutdoorAirMethod() const {
    boost::optional<std::string> value = getString(OS_Controller_MechanicalVentilationFields::SystemOutdoorAirMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ControllerMechanicalVentilation_Impl::isSystemOutdoorAirMethodDefaulted() const {
    return isEmpty(OS_Controller_MechanicalVentilationFields::SystemOutdoorAirMethod);
  }

  bool ControllerMechanicalVentilation_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Controller_MechanicalVentilationFields::AvailabilitySchedule,
                              "ControllerMechanicalVentilation",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void ControllerMechanicalVentilation_Impl::setDemandControlledVentilation(bool demandControlledVentilation) {
    setBooleanFieldValue(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation, demandControlledVentilation);
    //if (demandControlledVentilation) {
    //  setBooleanFieldValue(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation, true);
    //} else {
    //  setBooleanFieldValue(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation, false);
    //}
  }

  void ControllerMechanicalVentilation_Impl::resetDemandControlledVentilation() {
    bool result = setString(OS_Controller_MechanicalVentilationFields::DemandControlledVentilation, "");
    OS_ASSERT(result);
  }

  bool ControllerMechanicalVentilation_Impl::setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod) {
    bool result = setString(OS_Controller_MechanicalVentilationFields::SystemOutdoorAirMethod, systemOutdoorAirMethod);
    return result;
  }

  void ControllerMechanicalVentilation_Impl::resetSystemOutdoorAirMethod() {
    bool result = setString(OS_Controller_MechanicalVentilationFields::SystemOutdoorAirMethod, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ControllerMechanicalVentilation_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_MechanicalVentilationFields::AvailabilitySchedule);
  }

  std::vector<std::string> ControllerMechanicalVentilation_Impl::systemOutdoorAirMethodValues() const {
    return ControllerMechanicalVentilation::systemOutdoorAirMethodValues();
  }

  boost::optional<ModelObject> ControllerMechanicalVentilation_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool ControllerMechanicalVentilation_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  ControllerOutdoorAir ControllerMechanicalVentilation_Impl::controllerOutdoorAir() const
  {
    boost::optional<ControllerOutdoorAir> result;

    std::vector<ControllerOutdoorAir>  controllers = model().getConcreteModelObjects<ControllerOutdoorAir>();

    for( std::vector<ControllerOutdoorAir>::const_iterator it = controllers.begin();
         it != controllers.end();
         ++it
       )
    {
      if( it->controllerMechanicalVentilation().handle() == handle() )
      {
        result = *it;
      }
    }

    OS_ASSERT(result);

    return result.get();
  }

} // detail

ControllerMechanicalVentilation::ControllerMechanicalVentilation(const Model& model)
  : ModelObject(ControllerMechanicalVentilation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ControllerMechanicalVentilation_Impl>());

  Schedule schedule = model.alwaysOnDiscreteSchedule();

  setAvailabilitySchedule(schedule);

  setSystemOutdoorAirMethod("ZoneSum");
}

IddObjectType ControllerMechanicalVentilation::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Controller_MechanicalVentilation);
}

std::vector<std::string> ControllerMechanicalVentilation::systemOutdoorAirMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Controller_MechanicalVentilationFields::SystemOutdoorAirMethod);
}

Schedule ControllerMechanicalVentilation::availabilitySchedule() const {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->availabilitySchedule();
}

bool ControllerMechanicalVentilation::demandControlledVentilation() const {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->demandControlledVentilation();
}

bool ControllerMechanicalVentilation::isDemandControlledVentilationDefaulted() const {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->isDemandControlledVentilationDefaulted();
}

std::string ControllerMechanicalVentilation::systemOutdoorAirMethod() const {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->systemOutdoorAirMethod();
}

bool ControllerMechanicalVentilation::isSystemOutdoorAirMethodDefaulted() const {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->isSystemOutdoorAirMethodDefaulted();
}

bool ControllerMechanicalVentilation::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->setAvailabilitySchedule(schedule);
}

void ControllerMechanicalVentilation::setDemandControlledVentilation(bool demandControlledVentilation) {
  getImpl<detail::ControllerMechanicalVentilation_Impl>()->setDemandControlledVentilation(demandControlledVentilation);
}

void ControllerMechanicalVentilation::resetDemandControlledVentilation() {
  getImpl<detail::ControllerMechanicalVentilation_Impl>()->resetDemandControlledVentilation();
}

bool ControllerMechanicalVentilation::setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod) {
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->setSystemOutdoorAirMethod(systemOutdoorAirMethod);
}

void ControllerMechanicalVentilation::resetSystemOutdoorAirMethod() {
  getImpl<detail::ControllerMechanicalVentilation_Impl>()->resetSystemOutdoorAirMethod();
}

ControllerOutdoorAir ControllerMechanicalVentilation::controllerOutdoorAir() const
{
  return getImpl<detail::ControllerMechanicalVentilation_Impl>()->controllerOutdoorAir();
}

/// @cond
ControllerMechanicalVentilation::ControllerMechanicalVentilation(std::shared_ptr<detail::ControllerMechanicalVentilation_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

