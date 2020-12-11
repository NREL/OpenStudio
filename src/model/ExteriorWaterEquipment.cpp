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

#include "ExteriorWaterEquipment.hpp"
#include "ExteriorWaterEquipment_Impl.hpp"

#include "Model.hpp"
#include "ExteriorWaterEquipmentDefinition.hpp"
#include "ExteriorWaterEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Exterior_WaterEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorWaterEquipment_Impl::ExteriorWaterEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorWaterEquipment::iddObjectType());
    }

    ExteriorWaterEquipment_Impl::ExteriorWaterEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorWaterEquipment::iddObjectType());
    }

    ExteriorWaterEquipment_Impl::ExteriorWaterEquipment_Impl(const ExteriorWaterEquipment_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> ExteriorWaterEquipment_Impl::parent() const {
      return facility();
    }

    const std::vector<std::string>& ExteriorWaterEquipment_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Exterior WaterEquipment Electric Power", "Exterior WaterEquipment Electric Energy"};
      return result;
    }

    IddObjectType ExteriorWaterEquipment_Impl::iddObjectType() const {
      return ExteriorWaterEquipment::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ExteriorWaterEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Exterior_WaterEquipmentFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ExteriorWaterEquipment", "Exterior WaterEquipment"));
      }
      return result;
    }

    int ExteriorWaterEquipment_Impl::definitionIndex() const {
      return OS_Exterior_WaterEquipmentFields::ExteriorWaterEquipmentDefinitionName;
    }

    ExteriorWaterEquipmentDefinition ExteriorWaterEquipment_Impl::exteriorWaterEquipmentDefinition() const {
      return this->definition().cast<ExteriorWaterEquipmentDefinition>();
    }

    bool ExteriorWaterEquipment_Impl::setExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& exteriorWaterEquipmentDefinition) {
      return this->setPointer(this->definitionIndex(), exteriorWaterEquipmentDefinition.handle());
    }

    bool ExteriorWaterEquipment_Impl::setDefinition(const ExteriorLoadDefinition& definition) {
      bool result = false;
      boost::optional<ExteriorWaterEquipmentDefinition> exteriorWaterEquipmentDefinition =
        definition.optionalCast<ExteriorWaterEquipmentDefinition>();
      if (exteriorWaterEquipmentDefinition) {
        result = setExteriorWaterEquipmentDefinition(*exteriorWaterEquipmentDefinition);
      }
      return result;
    }

    Schedule ExteriorWaterEquipment_Impl::schedule() const {
      boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Exterior_WaterEquipmentFields::ScheduleName);
      if (!value) {
        value = this->model().alwaysOnDiscreteSchedule();
      }
      return value.get();
    }

    double ExteriorWaterEquipment_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_Exterior_WaterEquipmentFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorWaterEquipment_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_Exterior_WaterEquipmentFields::Multiplier);
    }

    std::string ExteriorWaterEquipment_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Exterior_WaterEquipmentFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorWaterEquipment_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Exterior_WaterEquipmentFields::EndUseSubcategory);
    }

    bool ExteriorWaterEquipment_Impl::setSchedule(Schedule& schedule) {
      bool result =
        ModelObject_Impl::setSchedule(OS_Exterior_WaterEquipmentFields::ScheduleName, "ExteriorWaterEquipment", "Exterior WaterEquipment", schedule);
      return result;
    }

    void ExteriorWaterEquipment_Impl::resetSchedule() {
      bool result = setString(OS_Exterior_WaterEquipmentFields::ScheduleName, "");
      OS_ASSERT(result);
    }

    bool ExteriorWaterEquipment_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_Exterior_WaterEquipmentFields::Multiplier, multiplier);
      return result;
    }

    void ExteriorWaterEquipment_Impl::resetMultiplier() {
      bool result = setString(OS_Exterior_WaterEquipmentFields::Multiplier, "");
      OS_ASSERT(result);
    }

    bool ExteriorWaterEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
      bool result = setString(OS_Exterior_WaterEquipmentFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ExteriorWaterEquipment_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Exterior_WaterEquipmentFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    Facility ExteriorWaterEquipment_Impl::facility() const {
      return model().getUniqueModelObject<Facility>();
    }

    boost::optional<ModelObject> ExteriorWaterEquipment_Impl::exteriorWaterEquipmentDefinitionAsModelObject() const {
      OptionalModelObject result = exteriorWaterEquipmentDefinition();
      return result;
    }

    boost::optional<ModelObject> ExteriorWaterEquipment_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      Schedule schedule = this->schedule();
      result = schedule.cast<ModelObject>();
      return result;
    }

    boost::optional<ModelObject> ExteriorWaterEquipment_Impl::facilityAsModelObject() const {
      OptionalModelObject result = facility();
      return result;
    }

    bool ExteriorWaterEquipment_Impl::setExteriorWaterEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalExteriorWaterEquipmentDefinition intermediate = modelObject->optionalCast<ExteriorWaterEquipmentDefinition>();
        if (intermediate) {
          return setExteriorWaterEquipmentDefinition(*intermediate);
        }
      }
      return false;
    }

    bool ExteriorWaterEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        }
      }
      return false;
    }

  }  // namespace detail

  ExteriorWaterEquipment::ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition)

    : ExteriorLoadInstance(ExteriorWaterEquipment::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::ExteriorWaterEquipment_Impl>());

    bool ok = setExteriorWaterEquipmentDefinition(definition);
    OS_ASSERT(ok);

    auto defaultSchedule = definition.model().alwaysOnDiscreteSchedule();
    ok = setSchedule(defaultSchedule);
    OS_ASSERT(ok);

    /*
   *ok = this->setMultiplier(1.0);
   *OS_ASSERT(ok);
   */

    ok = this->setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  ExteriorWaterEquipment::ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition, Schedule& schedule)
    : ExteriorLoadInstance(ExteriorWaterEquipment::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::ExteriorWaterEquipment_Impl>());

    bool ok = setExteriorWaterEquipmentDefinition(definition);
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

  IddObjectType ExteriorWaterEquipment::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_WaterEquipment);
    return result;
  }

  ExteriorWaterEquipmentDefinition ExteriorWaterEquipment::exteriorWaterEquipmentDefinition() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->exteriorWaterEquipmentDefinition();
  }

  Schedule ExteriorWaterEquipment::schedule() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->schedule();
  }

  double ExteriorWaterEquipment::multiplier() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->multiplier();
  }

  bool ExteriorWaterEquipment::isMultiplierDefaulted() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->isMultiplierDefaulted();
  }

  std::string ExteriorWaterEquipment::endUseSubcategory() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->endUseSubcategory();
  }

  bool ExteriorWaterEquipment::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->isEndUseSubcategoryDefaulted();
  }

  bool ExteriorWaterEquipment::setExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& exteriorWaterEquipmentDefinition) {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->setExteriorWaterEquipmentDefinition(exteriorWaterEquipmentDefinition);
  }

  bool ExteriorWaterEquipment::setSchedule(Schedule& schedule) {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->setSchedule(schedule);
  }

  void ExteriorWaterEquipment::resetSchedule() {
    getImpl<detail::ExteriorWaterEquipment_Impl>()->resetSchedule();
  }

  bool ExteriorWaterEquipment::setMultiplier(double multiplier) {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->setMultiplier(multiplier);
  }

  void ExteriorWaterEquipment::resetMultiplier() {
    getImpl<detail::ExteriorWaterEquipment_Impl>()->resetMultiplier();
  }

  bool ExteriorWaterEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void ExteriorWaterEquipment::resetEndUseSubcategory() {
    getImpl<detail::ExteriorWaterEquipment_Impl>()->resetEndUseSubcategory();
  }

  Facility ExteriorWaterEquipment::facility() const {
    return getImpl<detail::ExteriorWaterEquipment_Impl>()->facility();
  }

  /// @cond
  ExteriorWaterEquipment::ExteriorWaterEquipment(std::shared_ptr<detail::ExteriorWaterEquipment_Impl> impl) : ExteriorLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
