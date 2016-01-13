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

#include "ExteriorLights.hpp"
#include "ExteriorLights_Impl.hpp"

#include "Model.hpp"
#include "ExteriorLightsDefinition.hpp"
#include "ExteriorLightsDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Exterior_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExteriorLights_Impl::ExteriorLights_Impl(const IdfObject& idfObject,
                                           Model_Impl* model, 
                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExteriorLights::iddObjectType());
  }

  ExteriorLights_Impl::ExteriorLights_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExteriorLights::iddObjectType());
  }

  ExteriorLights_Impl::ExteriorLights_Impl(const ExteriorLights_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> ExteriorLights_Impl::parent() const {
    return facility();
  }

  const std::vector<std::string>& ExteriorLights_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }  

  IddObjectType ExteriorLights_Impl::iddObjectType() const {
    return ExteriorLights::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ExteriorLights_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Exterior_LightsFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ExteriorLights","Exterior Lights"));
    }
    return result;
  }

  ExteriorLightsDefinition ExteriorLights_Impl::exteriorLightsDefinition() const {
    boost::optional<ExteriorLightsDefinition> value = getObject<ModelObject>().getModelObjectTarget<ExteriorLightsDefinition>(OS_Exterior_LightsFields::ExteriorLightsDefinitionName);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ExteriorLights_Impl::schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Exterior_LightsFields::ScheduleName);
  }

  std::string ExteriorLights_Impl::controlOption() const {
    boost::optional<std::string> value = getString(OS_Exterior_LightsFields::ControlOption,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExteriorLights_Impl::isControlOptionDefaulted() const {
    return isEmpty(OS_Exterior_LightsFields::ControlOption);
  }

  double ExteriorLights_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_Exterior_LightsFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExteriorLights_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_Exterior_LightsFields::Multiplier);
  }

  std::string ExteriorLights_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Exterior_LightsFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExteriorLights_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Exterior_LightsFields::EndUseSubcategory);
  }

  bool ExteriorLights_Impl::setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition) {
    bool result = setPointer(OS_Exterior_LightsFields::ExteriorLightsDefinitionName, exteriorLightsDefinition.handle());
    return result;
  }

  bool ExteriorLights_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_Exterior_LightsFields::ScheduleName,
                                                "ExteriorLights",
                                                "Exterior Lights",
                                                schedule);
    return result;
  }

  void ExteriorLights_Impl::resetSchedule() {
    bool result = setString(OS_Exterior_LightsFields::ScheduleName, "");
    OS_ASSERT(result);
  }

  bool ExteriorLights_Impl::setControlOption(std::string controlOption) {
    bool result = setString(OS_Exterior_LightsFields::ControlOption, controlOption);
    return result;
  }

  void ExteriorLights_Impl::resetControlOption() {
    bool result = setString(OS_Exterior_LightsFields::ControlOption, "");
    OS_ASSERT(result);
  }

  bool ExteriorLights_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_Exterior_LightsFields::Multiplier, multiplier);
    return result;
  }

  void ExteriorLights_Impl::resetMultiplier() {
    bool result = setString(OS_Exterior_LightsFields::Multiplier, "");
    OS_ASSERT(result);
  }

  void ExteriorLights_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Exterior_LightsFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void ExteriorLights_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Exterior_LightsFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  Facility ExteriorLights_Impl::facility() const {
    return model().getUniqueModelObject<Facility>();
  }

  std::vector<std::string> ExteriorLights_Impl::controlOptionValues() const {
    return ExteriorLights::controlOptionValues();
  }

  boost::optional<ModelObject> ExteriorLights_Impl::exteriorLightsDefinitionAsModelObject() const {
    OptionalModelObject result = exteriorLightsDefinition();
    return result;
  }

  boost::optional<ModelObject> ExteriorLights_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    if( auto t_schedule = schedule() ) {
      result = t_schedule->cast<ModelObject>();
    }
    return result;
  }

  boost::optional<ModelObject> ExteriorLights_Impl::facilityAsModelObject() const {
    OptionalModelObject result = facility();
    return result;
  }

  bool ExteriorLights_Impl::setExteriorLightsDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalExteriorLightsDefinition intermediate = modelObject->optionalCast<ExteriorLightsDefinition>();
      if (intermediate) {
        return setExteriorLightsDefinition(*intermediate);
      }
    }
    return false;
  }

  bool ExteriorLights_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
    }
    return false;
  }

} // detail

ExteriorLights::ExteriorLights(const ExteriorLightsDefinition& definition,
                               bool useControlOptionAstronomicalClock)
  : ModelObject(ExteriorLights::iddObjectType(),definition.model())
{
  OS_ASSERT(getImpl<detail::ExteriorLights_Impl>());

  bool ok = setExteriorLightsDefinition(definition);
  OS_ASSERT(ok);

  auto defaultSchedule = definition.model().alwaysOnDiscreteSchedule();
  ok = setSchedule(defaultSchedule);
  OS_ASSERT(ok);

  if (useControlOptionAstronomicalClock) {
    ok = setControlOption("AstronomicalClock");
    OS_ASSERT(ok);
  }
}

ExteriorLights::ExteriorLights(const ExteriorLightsDefinition& definition,
                               Schedule& schedule)
  : ModelObject(ExteriorLights::iddObjectType(),definition.model())
{
  OS_ASSERT(getImpl<detail::ExteriorLights_Impl>());

  bool ok = setExteriorLightsDefinition(definition);
  OS_ASSERT(ok);
  ok = setSchedule(schedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Could not set " << briefDescription() << "'s schedule to "
                  << schedule.briefDescription() << ".");
  }
}

IddObjectType ExteriorLights::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Exterior_Lights);
  return result;
}

std::vector<std::string> ExteriorLights::controlOptionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Exterior_LightsFields::ControlOption);
}

std::vector<std::string> ExteriorLights::validControlOptionValues() {
  return ExteriorLights::controlOptionValues();
}

ExteriorLightsDefinition ExteriorLights::exteriorLightsDefinition() const {
  return getImpl<detail::ExteriorLights_Impl>()->exteriorLightsDefinition();
}

boost::optional<Schedule> ExteriorLights::schedule() const {
  return getImpl<detail::ExteriorLights_Impl>()->schedule();
}

std::string ExteriorLights::controlOption() const {
  return getImpl<detail::ExteriorLights_Impl>()->controlOption();
}

bool ExteriorLights::isControlOptionDefaulted() const {
  return getImpl<detail::ExteriorLights_Impl>()->isControlOptionDefaulted();
}

double ExteriorLights::multiplier() const {
  return getImpl<detail::ExteriorLights_Impl>()->multiplier();
}

bool ExteriorLights::isMultiplierDefaulted() const {
  return getImpl<detail::ExteriorLights_Impl>()->isMultiplierDefaulted();
}

std::string ExteriorLights::endUseSubcategory() const {
  return getImpl<detail::ExteriorLights_Impl>()->endUseSubcategory();
}

bool ExteriorLights::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::ExteriorLights_Impl>()->isEndUseSubcategoryDefaulted();
}

bool ExteriorLights::setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition) {
  return getImpl<detail::ExteriorLights_Impl>()->setExteriorLightsDefinition(exteriorLightsDefinition);
}

bool ExteriorLights::setSchedule(Schedule& schedule) {
  return getImpl<detail::ExteriorLights_Impl>()->setSchedule(schedule);
}

void ExteriorLights::resetSchedule() {
  getImpl<detail::ExteriorLights_Impl>()->resetSchedule();
}

bool ExteriorLights::setControlOption(std::string controlOption) {
  return getImpl<detail::ExteriorLights_Impl>()->setControlOption(controlOption);
}

void ExteriorLights::resetControlOption() {
  getImpl<detail::ExteriorLights_Impl>()->resetControlOption();
}

bool ExteriorLights::setMultiplier(double multiplier) {
  return getImpl<detail::ExteriorLights_Impl>()->setMultiplier(multiplier);
}

void ExteriorLights::resetMultiplier() {
  getImpl<detail::ExteriorLights_Impl>()->resetMultiplier();
}

void ExteriorLights::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::ExteriorLights_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void ExteriorLights::resetEndUseSubcategory() {
  getImpl<detail::ExteriorLights_Impl>()->resetEndUseSubcategory();
}

Facility ExteriorLights::facility() const {
  return getImpl<detail::ExteriorLights_Impl>()->facility();
}

/// @cond
ExteriorLights::ExteriorLights(std::shared_ptr<detail::ExteriorLights_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
