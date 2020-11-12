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

    ExteriorLights_Impl::ExteriorLights_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorLights::iddObjectType());
    }

    ExteriorLights_Impl::ExteriorLights_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorLights::iddObjectType());
    }

    ExteriorLights_Impl::ExteriorLights_Impl(const ExteriorLights_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> ExteriorLights_Impl::parent() const {
      return facility();
    }

    const std::vector<std::string>& ExteriorLights_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Exterior Lights Electricity Rate", "Exterior Lights Electricity Energy"};
      return result;
    }

    IddObjectType ExteriorLights_Impl::iddObjectType() const {
      return ExteriorLights::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ExteriorLights_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Exterior_LightsFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ExteriorLights", "Exterior Lights"));
      }
      return result;
    }

    int ExteriorLights_Impl::definitionIndex() const {
      return OS_Exterior_LightsFields::ExteriorLightsDefinitionName;
    }

    ExteriorLightsDefinition ExteriorLights_Impl::exteriorLightsDefinition() const {
      return this->definition().cast<ExteriorLightsDefinition>();
    }

    bool ExteriorLights_Impl::setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition) {
      return this->setPointer(this->definitionIndex(), exteriorLightsDefinition.handle());
    }

    bool ExteriorLights_Impl::setDefinition(const ExteriorLoadDefinition& definition) {
      bool result = false;
      boost::optional<ExteriorLightsDefinition> exteriorLightsDefinition = definition.optionalCast<ExteriorLightsDefinition>();
      if (exteriorLightsDefinition) {
        result = setExteriorLightsDefinition(*exteriorLightsDefinition);
      }
      return result;
    }

    boost::optional<Schedule> ExteriorLights_Impl::schedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Exterior_LightsFields::ScheduleName);
    }

    std::string ExteriorLights_Impl::controlOption() const {
      boost::optional<std::string> value = getString(OS_Exterior_LightsFields::ControlOption, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorLights_Impl::isControlOptionDefaulted() const {
      return isEmpty(OS_Exterior_LightsFields::ControlOption);
    }

    double ExteriorLights_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_Exterior_LightsFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorLights_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_Exterior_LightsFields::Multiplier);
    }

    std::string ExteriorLights_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Exterior_LightsFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorLights_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Exterior_LightsFields::EndUseSubcategory);
    }

    bool ExteriorLights_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_Exterior_LightsFields::ScheduleName, "ExteriorLights", "Exterior Lights", schedule);
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

    bool ExteriorLights_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
      bool result = setString(OS_Exterior_LightsFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
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
      if (auto t_schedule = schedule()) {
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

    std::vector<EMSActuatorNames> ExteriorLights_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"ExteriorLights", "Electric Power"}};
      return actuators;
    }

    std::vector<std::string> ExteriorLights_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  ExteriorLights::ExteriorLights(const ExteriorLightsDefinition& definition, bool useControlOptionAstronomicalClock)
    : ExteriorLoadInstance(ExteriorLights::iddObjectType(), definition) {
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

    /*
   *ok = this->setMultiplier(1.0);
   *OS_ASSERT(ok);
   */
    ok = this->setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  ExteriorLights::ExteriorLights(const ExteriorLightsDefinition& definition, Schedule& schedule)
    : ExteriorLoadInstance(ExteriorLights::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::ExteriorLights_Impl>());

    bool ok = setExteriorLightsDefinition(definition);
    OS_ASSERT(ok);
    ok = setSchedule(schedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Could not set " << briefDescription() << "'s schedule to " << schedule.briefDescription() << ".");
    }

    /*
   *ok = this->setMultiplier(1.0);
   *OS_ASSERT(ok);
   */
    ok = this->setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  IddObjectType ExteriorLights::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_Lights);
    return result;
  }

  std::vector<std::string> ExteriorLights::controlOptionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Exterior_LightsFields::ControlOption);
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

  bool ExteriorLights::setEndUseSubcategory(std::string endUseSubcategory) {
    return getImpl<detail::ExteriorLights_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void ExteriorLights::resetEndUseSubcategory() {
    getImpl<detail::ExteriorLights_Impl>()->resetEndUseSubcategory();
  }

  Facility ExteriorLights::facility() const {
    return getImpl<detail::ExteriorLights_Impl>()->facility();
  }

  /// @cond
  ExteriorLights::ExteriorLights(std::shared_ptr<detail::ExteriorLights_Impl> impl) : ExteriorLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
