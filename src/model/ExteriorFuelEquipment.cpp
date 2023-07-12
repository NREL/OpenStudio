/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorFuelEquipment.hpp"
#include "ExteriorFuelEquipment_Impl.hpp"

#include "Model.hpp"
#include "ExteriorFuelEquipmentDefinition.hpp"
#include "ExteriorFuelEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Exterior_FuelEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorFuelEquipment_Impl::ExteriorFuelEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorFuelEquipment::iddObjectType());
    }

    ExteriorFuelEquipment_Impl::ExteriorFuelEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorFuelEquipment::iddObjectType());
    }

    ExteriorFuelEquipment_Impl::ExteriorFuelEquipment_Impl(const ExteriorFuelEquipment_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadInstance_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> ExteriorFuelEquipment_Impl::parent() const {
      return facility();
    }

    const std::vector<std::string>& ExteriorFuelEquipment_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Exterior FuelEquipment Electric Power", "Exterior FuelEquipment Electric Energy"};
      return result;
    }

    IddObjectType ExteriorFuelEquipment_Impl::iddObjectType() const {
      return ExteriorFuelEquipment::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ExteriorFuelEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Exterior_FuelEquipmentFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ExteriorFuelEquipment", "Exterior FuelEquipment"));
      }
      return result;
    }

    int ExteriorFuelEquipment_Impl::definitionIndex() const {
      return OS_Exterior_FuelEquipmentFields::ExteriorFuelEquipmentDefinitionName;
    }

    ExteriorFuelEquipmentDefinition ExteriorFuelEquipment_Impl::exteriorFuelEquipmentDefinition() const {
      return this->definition().cast<ExteriorFuelEquipmentDefinition>();
    }

    bool ExteriorFuelEquipment_Impl::setExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& exteriorFuelEquipmentDefinition) {
      return this->setPointer(this->definitionIndex(), exteriorFuelEquipmentDefinition.handle());
    }

    bool ExteriorFuelEquipment_Impl::setDefinition(const ExteriorLoadDefinition& definition) {
      bool result = false;
      boost::optional<ExteriorFuelEquipmentDefinition> exteriorFuelEquipmentDefinition = definition.optionalCast<ExteriorFuelEquipmentDefinition>();
      if (exteriorFuelEquipmentDefinition) {
        result = setExteriorFuelEquipmentDefinition(*exteriorFuelEquipmentDefinition);
      }
      return result;
    }

    Schedule ExteriorFuelEquipment_Impl::schedule() const {
      boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Exterior_FuelEquipmentFields::ScheduleName);
      if (!value) {
        value = this->model().alwaysOnDiscreteSchedule();
      }
      return value.get();
    }

    std::string ExteriorFuelEquipment_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_Exterior_FuelEquipmentFields::FuelUseType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ExteriorFuelEquipment_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_Exterior_FuelEquipmentFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorFuelEquipment_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_Exterior_FuelEquipmentFields::Multiplier);
    }

    std::string ExteriorFuelEquipment_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Exterior_FuelEquipmentFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorFuelEquipment_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Exterior_FuelEquipmentFields::EndUseSubcategory);
    }

    bool ExteriorFuelEquipment_Impl::setSchedule(Schedule& schedule) {
      bool result =
        ModelObject_Impl::setSchedule(OS_Exterior_FuelEquipmentFields::ScheduleName, "ExteriorFuelEquipment", "Exterior FuelEquipment", schedule);
      return result;
    }

    void ExteriorFuelEquipment_Impl::resetSchedule() {
      bool result = setString(OS_Exterior_FuelEquipmentFields::ScheduleName, "");
      OS_ASSERT(result);
    }

    bool ExteriorFuelEquipment_Impl::setFuelType(const std::string& fuelType) {
      bool result = setString(OS_Exterior_FuelEquipmentFields::FuelUseType, fuelType);
      return result;
    }

    bool ExteriorFuelEquipment_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_Exterior_FuelEquipmentFields::Multiplier, multiplier);
      return result;
    }

    void ExteriorFuelEquipment_Impl::resetMultiplier() {
      bool result = setString(OS_Exterior_FuelEquipmentFields::Multiplier, "");
      OS_ASSERT(result);
    }

    bool ExteriorFuelEquipment_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Exterior_FuelEquipmentFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ExteriorFuelEquipment_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Exterior_FuelEquipmentFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    Facility ExteriorFuelEquipment_Impl::facility() const {
      return model().getUniqueModelObject<Facility>();
    }

    std::vector<std::string> ExteriorFuelEquipment_Impl::fuelTypeValues() const {
      return ExteriorFuelEquipment::fuelTypeValues();
    }

    boost::optional<ModelObject> ExteriorFuelEquipment_Impl::exteriorFuelEquipmentDefinitionAsModelObject() const {
      OptionalModelObject result = exteriorFuelEquipmentDefinition();
      return result;
    }

    boost::optional<ModelObject> ExteriorFuelEquipment_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      Schedule schedule = this->schedule();
      result = schedule.cast<ModelObject>();
      return result;
    }

    boost::optional<ModelObject> ExteriorFuelEquipment_Impl::facilityAsModelObject() const {
      OptionalModelObject result = facility();
      return result;
    }

    bool ExteriorFuelEquipment_Impl::setExteriorFuelEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalExteriorFuelEquipmentDefinition intermediate = modelObject->optionalCast<ExteriorFuelEquipmentDefinition>();
        if (intermediate) {
          return setExteriorFuelEquipmentDefinition(*intermediate);
        }
      }
      return false;
    }

    bool ExteriorFuelEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  ExteriorFuelEquipment::ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition)
    : ExteriorLoadInstance(ExteriorFuelEquipment::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::ExteriorFuelEquipment_Impl>());

    bool ok = setExteriorFuelEquipmentDefinition(definition);
    OS_ASSERT(ok);

    auto defaultSchedule = definition.model().alwaysOnDiscreteSchedule();
    ok = setSchedule(defaultSchedule);
    OS_ASSERT(ok);

    setFuelType("Electricity");

    /*
   *ok = this->setMultiplier(1.0);
   *OS_ASSERT(ok);
   */

    ok = this->setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  ExteriorFuelEquipment::ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition, Schedule& schedule)
    : ExteriorLoadInstance(ExteriorFuelEquipment::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::ExteriorFuelEquipment_Impl>());

    bool ok = setExteriorFuelEquipmentDefinition(definition);
    OS_ASSERT(ok);
    ok = setSchedule(schedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Could not set " << briefDescription() << "'s schedule to " << schedule.briefDescription() << ".");
    }

    setFuelType("NaturalGas");

    /*
   *ok = this->setMultiplier(1.0);
   *OS_ASSERT(ok);
   */
    ok = this->setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  IddObjectType ExteriorFuelEquipment::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_FuelEquipment);
    return result;
  }

  std::vector<std::string> ExteriorFuelEquipment::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Exterior_FuelEquipmentFields::FuelUseType);
  }

  std::vector<std::string> ExteriorFuelEquipment::validFuelTypeValues() {
    return ExteriorFuelEquipment::fuelTypeValues();
  }

  ExteriorFuelEquipmentDefinition ExteriorFuelEquipment::exteriorFuelEquipmentDefinition() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->exteriorFuelEquipmentDefinition();
  }

  Schedule ExteriorFuelEquipment::schedule() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->schedule();
  }

  std::string ExteriorFuelEquipment::fuelType() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->fuelType();
  }

  double ExteriorFuelEquipment::multiplier() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->multiplier();
  }

  bool ExteriorFuelEquipment::isMultiplierDefaulted() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->isMultiplierDefaulted();
  }

  std::string ExteriorFuelEquipment::endUseSubcategory() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->endUseSubcategory();
  }

  bool ExteriorFuelEquipment::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->isEndUseSubcategoryDefaulted();
  }

  bool ExteriorFuelEquipment::setExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& exteriorFuelEquipmentDefinition) {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->setExteriorFuelEquipmentDefinition(exteriorFuelEquipmentDefinition);
  }

  bool ExteriorFuelEquipment::setSchedule(Schedule& schedule) {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->setSchedule(schedule);
  }

  void ExteriorFuelEquipment::resetSchedule() {
    getImpl<detail::ExteriorFuelEquipment_Impl>()->resetSchedule();
  }

  bool ExteriorFuelEquipment::setFuelType(const std::string& fuelType) {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->setFuelType(fuelType);
  }

  bool ExteriorFuelEquipment::setMultiplier(double multiplier) {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->setMultiplier(multiplier);
  }

  void ExteriorFuelEquipment::resetMultiplier() {
    getImpl<detail::ExteriorFuelEquipment_Impl>()->resetMultiplier();
  }

  bool ExteriorFuelEquipment::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void ExteriorFuelEquipment::resetEndUseSubcategory() {
    getImpl<detail::ExteriorFuelEquipment_Impl>()->resetEndUseSubcategory();
  }

  Facility ExteriorFuelEquipment::facility() const {
    return getImpl<detail::ExteriorFuelEquipment_Impl>()->facility();
  }

  /// @cond
  ExteriorFuelEquipment::ExteriorFuelEquipment(std::shared_ptr<detail::ExteriorFuelEquipment_Impl> impl) : ExteriorLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
