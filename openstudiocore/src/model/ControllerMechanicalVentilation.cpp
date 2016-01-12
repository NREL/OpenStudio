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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

