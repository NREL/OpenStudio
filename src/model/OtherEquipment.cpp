/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OtherEquipment.hpp"
#include "OtherEquipment_Impl.hpp"

#include "OtherEquipmentDefinition.hpp"
#include "OtherEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Model.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_OtherEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    OtherEquipment_Impl::OtherEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OtherEquipment::iddObjectType());
    }

    OtherEquipment_Impl::OtherEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OtherEquipment::iddObjectType());
    }

    OtherEquipment_Impl::OtherEquipment_Impl(const OtherEquipment_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OtherEquipment_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Other Equipment Fuel Rate",
        "Other Equipment Fuel Energy",
        "Other Equipment Radiant Heating Energy",
        "Other Equipment Radiant Heating Rate",
        "Other Equipment Convective Heating Energy",
        "Other Equipment Convective Heating Rate",
        "Other Equipment Latent Gain Energy",
        "Other Equipment Latent Gain Rate",
        "Other Equipment Lost Heat Energy",
        "Other Equipment Lost Heat Rate",
        "Other Equipment Total Heating Energy",
        "Other Equipment Total Heating Rate"

        // Reported in ThermalZone
        //"Zone Other Equipment Radiant Heating Energy",
        //"Zone Other Equipment Radiant Heating Rate",
        //"Zone Other Equipment Convective Heating Energy",
        //"Zone Other Equipment Convective Heating Rate",
        //"Zone Other Equipment Latent Gain Energy",
        //"Zone Other Equipment Latent Gain Rate",
        //"Zone Other Equipment Lost Heat Energy",
        //"Zone Other Equipment Lost Heat Rate",
        //"Zone Other Equipment Total Heating Energy",
        //"Zone Other Equipment Total Heating Rate"
      };
      return result;
    }

    IddObjectType OtherEquipment_Impl::iddObjectType() const {
      return OtherEquipment::iddObjectType();
    }

    std::vector<ScheduleTypeKey> OtherEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_OtherEquipmentFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("OtherEquipment", "Other Equipment"));
      }
      return result;
    }

    bool OtherEquipment_Impl::hardSize() {
      OptionalSpace space = this->space();
      if (!space) {
        return false;
      }

      makeUnique();

      OtherEquipmentDefinition definition = otherEquipmentDefinition();
      for (LifeCycleCost& cost : definition.lifeCycleCosts()) {
        cost.convertToCostPerEach();
      }

      if (definition.designLevel()) {
        return true;
      }

      if (OptionalDouble areaDensity = definition.wattsperSpaceFloorArea()) {
        definition.setDesignLevel(*areaDensity * space->floorArea());
        return true;
      }

      if (OptionalDouble peopleDensity = definition.wattsperPerson()) {
        definition.setDesignLevel(*peopleDensity * space->numberOfPeople());
        return true;
      }

      // should never get here.
      OS_ASSERT(false);
      return false;
    }

    bool OtherEquipment_Impl::hardApplySchedules() {
      bool result(false);
      OptionalSchedule schedule = this->schedule();
      if (schedule) {
        result = setSchedule(*schedule);
      }
      return result;
    }

    std::string OtherEquipment_Impl::endUseSubcategory() const {
      return getString(OS_OtherEquipmentFields::EndUseSubcategory, true).get();
    }

    bool OtherEquipment_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_OtherEquipmentFields::EndUseSubcategory);
    }

    std::string OtherEquipment_Impl::fuelType() const {
      return this->getString(OS_OtherEquipmentFields::FuelType, true).get();
    }

    bool OtherEquipment_Impl::isFuelTypeDefaulted() const {
      return isEmpty(OS_OtherEquipmentFields::FuelType);
    }

    OtherEquipmentDefinition OtherEquipment_Impl::otherEquipmentDefinition() const {
      return definition().cast<OtherEquipmentDefinition>();
    }

    boost::optional<Schedule> OtherEquipment_Impl::schedule() const {
      OptionalSchedule result = getObject<OtherEquipment>().getModelObjectTarget<Schedule>(OS_OtherEquipmentFields::ScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::OtherEquipmentSchedule);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::OtherEquipmentSchedule);
        }
      }
      return result;
    }

    bool OtherEquipment_Impl::isScheduleDefaulted() const {
      return isEmpty(OS_OtherEquipmentFields::ScheduleName);
    }

    double OtherEquipment_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_OtherEquipmentFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OtherEquipment_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_OtherEquipmentFields::Multiplier);
    }

    bool OtherEquipment_Impl::isAbsolute() const {
      OtherEquipmentDefinition definition = otherEquipmentDefinition();
      if (definition.designLevel()) {
        return true;
      }
      return false;
    }

    bool OtherEquipment_Impl::setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition) {
      return setPointer(definitionIndex(), definition.handle());
    }

    bool OtherEquipment_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_OtherEquipmentFields::EndUseSubcategory, endUseSubcategory);
    }

    void OtherEquipment_Impl::resetEndUseSubcategory() {
      OS_ASSERT(setString(OS_OtherEquipmentFields::EndUseSubcategory, ""));
    }

    bool OtherEquipment_Impl::setFuelType(const FuelType& fuelType) {
      return this->setString(OS_OtherEquipmentFields::FuelType, fuelType.valueDescription());
    }

    void OtherEquipment_Impl::resetFuelType() {
      this->setString(OS_OtherEquipmentFields::FuelType, "");
    }

    bool OtherEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition) {
      if (OptionalOtherEquipmentDefinition otherEquipmentDefinition = definition.optionalCast<OtherEquipmentDefinition>()) {
        return setOtherEquipmentDefinition(*otherEquipmentDefinition);
      }
      return false;
    }

    bool OtherEquipment_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_OtherEquipmentFields::ScheduleName, "OtherEquipment", "Other Equipment", schedule);
      return result;
    }

    void OtherEquipment_Impl::resetSchedule() {
      bool ok = setString(OS_OtherEquipmentFields::ScheduleName, "");
      OS_ASSERT(ok);
    }

    bool OtherEquipment_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_OtherEquipmentFields::Multiplier, multiplier);
      return result;
    }

    void OtherEquipment_Impl::resetMultiplier() {
      bool result = setString(OS_OtherEquipmentFields::Multiplier, "");
      OS_ASSERT(result);
    }

    double OtherEquipment_Impl::getDesignLevel(double floorArea, double numPeople) const {
      return otherEquipmentDefinition().getDesignLevel(floorArea, numPeople) * multiplier();
    }

    double OtherEquipment_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const {
      return otherEquipmentDefinition().getPowerPerFloorArea(floorArea, numPeople) * multiplier();
    }

    double OtherEquipment_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
      return otherEquipmentDefinition().getPowerPerPerson(floorArea, numPeople) * multiplier();
    }

    int OtherEquipment_Impl::spaceIndex() const {
      return OS_OtherEquipmentFields::SpaceorSpaceTypeName;
    }

    int OtherEquipment_Impl::definitionIndex() const {
      return OS_OtherEquipmentFields::OtherEquipmentDefinitionName;
    }

    boost::optional<ModelObject> OtherEquipment_Impl::otherEquipmentDefinitionAsModelObject() const {
      OptionalModelObject result = otherEquipmentDefinition();
      return result;
    }

    boost::optional<ModelObject> OtherEquipment_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool OtherEquipment_Impl::setOtherEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalOtherEquipmentDefinition intermediate = modelObject->optionalCast<OtherEquipmentDefinition>();
        if (intermediate) {
          return setOtherEquipmentDefinition(*intermediate);
        }
      }
      return false;
    }

    bool OtherEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

    std::vector<EMSActuatorNames> OtherEquipment_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"OtherEquipment", "Power Level"}};
      return actuators;
    }

    std::vector<std::string> OtherEquipment_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Other Equipment Design Level"};
      return types;
    }

  }  // namespace detail

  OtherEquipment::OtherEquipment(const OtherEquipmentDefinition& definition) : SpaceLoadInstance(OtherEquipment::iddObjectType(), definition) {
    OS_ASSERT(getImpl<detail::OtherEquipment_Impl>());

    /*
   *Schedule sch = this->model().alwaysOnDiscreteSchedule();
   *setSchedule(sch);
   *setMultiplier(1.0);
   *setFuelType("NaturalGas");
   */
    setEndUseSubcategory("General");
  }

  IddObjectType OtherEquipment::iddObjectType() {
    IddObjectType result(IddObjectType::OS_OtherEquipment);
    return result;
  }

  std::vector<std::string> OtherEquipment::validFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_OtherEquipmentFields::FuelType);
  }

  std::string OtherEquipment::endUseSubcategory() const {
    return getImpl<detail::OtherEquipment_Impl>()->endUseSubcategory();
  }

  bool OtherEquipment::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::OtherEquipment_Impl>()->isEndUseSubcategoryDefaulted();
  }

  bool OtherEquipment::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::OtherEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void OtherEquipment::resetEndUseSubcategory() {
    getImpl<detail::OtherEquipment_Impl>()->resetEndUseSubcategory();
  }

  std::string OtherEquipment::fuelType() const {
    return getImpl<detail::OtherEquipment_Impl>()->fuelType();
  }

  bool OtherEquipment::isFuelTypeDefaulted() const {
    return getImpl<detail::OtherEquipment_Impl>()->isFuelTypeDefaulted();
  }

  bool OtherEquipment::setFuelType(const FuelType& fuelType) {
    return getImpl<detail::OtherEquipment_Impl>()->setFuelType(fuelType);
  }

  bool OtherEquipment::setFuelType(const std::string& fuelType) {
    // Special case for this one, the only that has "None" as a possibility
    if (openstudio::istringEqual("None", fuelType)) {
      return this->setString(OS_OtherEquipmentFields::FuelType, "None");
    }
    try {
      return setFuelType(FuelType{fuelType});
    } catch (std::runtime_error& e) {
      LOG(Debug, e.what());
      return false;
    }
  }

  void OtherEquipment::resetFuelType() {
    getImpl<detail::OtherEquipment_Impl>()->resetFuelType();
  }

  OtherEquipmentDefinition OtherEquipment::otherEquipmentDefinition() const {
    return getImpl<detail::OtherEquipment_Impl>()->otherEquipmentDefinition();
  }

  boost::optional<Schedule> OtherEquipment::schedule() const {
    return getImpl<detail::OtherEquipment_Impl>()->schedule();
  }

  bool OtherEquipment::isScheduleDefaulted() const {
    return getImpl<detail::OtherEquipment_Impl>()->isScheduleDefaulted();
  }

  bool OtherEquipment::setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition) {
    return getImpl<detail::OtherEquipment_Impl>()->setOtherEquipmentDefinition(definition);
  }

  bool OtherEquipment::setSchedule(Schedule& schedule) {
    return getImpl<detail::OtherEquipment_Impl>()->setSchedule(schedule);
  }

  void OtherEquipment::resetSchedule() {
    getImpl<detail::OtherEquipment_Impl>()->resetSchedule();
  }

  bool OtherEquipment::setMultiplier(double multiplier) {
    return getImpl<detail::OtherEquipment_Impl>()->setMultiplier(multiplier);
  }

  void OtherEquipment::resetMultiplier() {
    getImpl<detail::OtherEquipment_Impl>()->resetMultiplier();
  }

  double OtherEquipment::getDesignLevel(double floorArea, double numPeople) const {
    return getImpl<detail::OtherEquipment_Impl>()->getDesignLevel(floorArea, numPeople);
  }

  double OtherEquipment::getPowerPerFloorArea(double floorArea, double numPeople) const {
    return getImpl<detail::OtherEquipment_Impl>()->getPowerPerFloorArea(floorArea, numPeople);
  }

  double OtherEquipment::getPowerPerPerson(double floorArea, double numPeople) const {
    return getImpl<detail::OtherEquipment_Impl>()->getPowerPerPerson(floorArea, numPeople);
  }

  /// @cond
  OtherEquipment::OtherEquipment(std::shared_ptr<detail::OtherEquipment_Impl> impl) : SpaceLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
