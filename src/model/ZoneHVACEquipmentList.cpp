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

#include "ZoneHVACEquipmentList.hpp"
#include "ZoneHVACEquipmentList_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "ScheduleConstant_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACEquipmentList::iddObjectType());
    }

    ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACEquipmentList::iddObjectType());
    }

    ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const ZoneHVACEquipmentList_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneHVACEquipmentList_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ZoneHVACEquipmentList_Impl::iddObjectType() const {
      return ZoneHVACEquipmentList::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACEquipmentList_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;

      // This is unusual, but the schedules here are in the Extensible Fields (and only there)
      // UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector fieldIndices;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        UnsignedVector thisFieldIndices = group.getSourceFieldIndices(schedule.handle());
        fieldIndices.insert(fieldIndices.end(), thisFieldIndices.begin(), thisFieldIndices.end());
      }

      // Make unique, have to sort before calling unique, unique only works on consecutive elements
      std::sort(fieldIndices.begin(), fieldIndices.end());
      fieldIndices.erase(std::unique(fieldIndices.begin(), fieldIndices.end()), fieldIndices.end());

      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACEquipmentList", "Sequential Cooling Fraction"));
      }
      if (std::find(b, e, OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ZoneHVACEquipmentList", "Sequential Heating Fraction"));
      }

      return result;
    }

    std::string ZoneHVACEquipmentList_Impl::loadDistributionScheme() const {
      auto value = getString(OS_ZoneHVAC_EquipmentListFields::LoadDistributionScheme, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACEquipmentList_Impl::setLoadDistributionScheme(std::string scheme) {
      // Backward compat
      if (istringEqual(scheme, "Sequential")) {
        scheme = "SequentialLoad";
      } else if (istringEqual(scheme, "Uniform")) {
        scheme = "UniformLoad";
      }

      // Reset the Sequential Cooling/Heating fractions if not 'SequentialLoad'
      if (!istringEqual(scheme, "SequentialLoad")) {
        for (IdfExtensibleGroup& eg : extensibleGroups()) {
          eg.setString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFractionScheduleName, "");
          eg.setString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFractionScheduleName, "");
          // TODO: delete existing schedule(s) if not tied to anything else
        }
      }

      return setString(OS_ZoneHVAC_EquipmentListFields::LoadDistributionScheme, scheme);
    }

    bool ZoneHVACEquipmentList_Impl::addEquipment(const ModelObject& equipment) {

      // Get max priorities before pushing an extensible group
      unsigned heatingCount = this->equipmentInHeatingOrder().size();
      unsigned coolingCount = this->equipmentInCoolingOrder().size();

      ModelExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<ModelExtensibleGroup>();

      bool ok = eg.setPointer(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment, equipment.handle());

      if (ok) {
        eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence, coolingCount + 1);
        eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence, heatingCount + 1);
      } else {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }
      return ok;
    }

    bool ZoneHVACEquipmentList_Impl::setCoolingPriority(const ModelObject& equipment, unsigned priority) {
      std::vector<ModelObject> equipmentVector = equipmentInCoolingOrder();

      if (priority > equipmentVector.size()) {
        priority = equipmentVector.size();
      } else if (priority < 1) {
        // TODO: Temp, until E+ actually allows zero priority (=unavailable) like the IDD seems to indicate
        priority = 1;
      }

      boost::optional<ModelExtensibleGroup> _eg = getGroupForModelObject(equipment);
      if (!_eg) {
        LOG(Warn, "Cannot set cooling priority of an equipment that isn't in the ZoneHVACEquipmentList for " << briefDescription());
        return false;
      } else {
        // Set the priority of this one, in case we actually put it to zero
        bool success = _eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence, priority);
        OS_ASSERT(success);
      }

      equipmentVector.erase(std::find(equipmentVector.begin(), equipmentVector.end(), equipment));

      // If supplied priority isn't zero, we have to insert it where it belongs
      if (priority > 0) {
        equipmentVector.insert(equipmentVector.begin() + (priority - 1), equipment);
      } else {
        // If zero, we just remove it from the list of available equipment (no insertion needed now)
        // and we reset the Sequential Cooling Fraction
        _eg->setString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFractionScheduleName, "");
        // TODO: delete existing schedule(s) if not tied to anything else
      }

      unsigned newPriority = 1;

      for (const auto& equipment : equipmentVector) {
        boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(equipment);
        OS_ASSERT(eg);
        eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence, newPriority);

        newPriority++;
      }
      return true;
    }

    bool ZoneHVACEquipmentList_Impl::setHeatingPriority(const ModelObject& equipment, unsigned priority) {
      std::vector<ModelObject> equipmentVector = equipmentInHeatingOrder();

      if (priority > equipmentVector.size()) {
        priority = equipmentVector.size();
      } else if (priority < 1) {
        // TODO: Temp, until E+ actually allows zero priority (=unavailable) like the IDD seems to indicate
        priority = 1;
      }
      boost::optional<ModelExtensibleGroup> _eg = getGroupForModelObject(equipment);
      if (!_eg) {
        LOG(Warn, "Cannot set Heating priority of an equipment that isn't in the ZoneHVACEquipmentList for " << briefDescription());
        return false;
      } else {
        // Set the priority of this one, in case we actually put it to zero
        bool success = _eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence, priority);
        OS_ASSERT(success);
      }

      equipmentVector.erase(std::find(equipmentVector.begin(), equipmentVector.end(), equipment));

      // If supplied priority isn't zero, we have to insert it where it belongs
      if (priority > 0) {
        equipmentVector.insert(equipmentVector.begin() + (priority - 1), equipment);
      } else {
        // If zero, we just remove it from the list of available equipment (no insertion needed now)
        // and we reset the Sequential Heating Fraction
        _eg->setString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFractionScheduleName, "");
        // TODO: delete existing schedule(s) if not tied to anything else
      }

      unsigned newPriority = 1;

      for (const auto& equipment : equipmentVector) {
        boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(equipment);
        OS_ASSERT(eg);
        eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence, newPriority);

        newPriority++;
      }
      return true;
    }

    boost::optional<ModelExtensibleGroup> ZoneHVACEquipmentList_Impl::getGroupForModelObject(const ModelObject& modelObject) const {
      boost::optional<ModelExtensibleGroup> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

        OS_ASSERT(wo);

        if (wo->handle() == modelObject.handle()) {
          result = group.cast<ModelExtensibleGroup>();

          break;
        }
      }

      return result;
    }

    std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipment() const {
      std::vector<ModelObject> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

        if (wo) {
          result.push_back(wo->cast<ModelObject>());
        }
      }

      return result;
    }

    std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipmentInHeatingOrder() const {
      std::map<unsigned, ModelObject> equipmentMap;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        unsigned heatingPriority = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence).get();

        // Filter out equipment having a priority of 0 (=unavailable for heating)
        if (heatingPriority > 0) {
          boost::optional<WorkspaceObject> wo =
            group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

          OS_ASSERT(wo);

          ModelObject mo = wo->cast<ModelObject>();

          equipmentMap.insert(std::make_pair(heatingPriority, mo));
        }
      }

      std::vector<ModelObject> equipmentVector;

      for (unsigned i = 1; i <= equipmentMap.size(); i++) {
        ModelObject mo = equipmentMap.find(i)->second;

        equipmentVector.push_back(mo);
      }

      return equipmentVector;
    }

    std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipmentInCoolingOrder() const {
      std::map<unsigned, ModelObject> equipmentMap;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        unsigned coolingPriority = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence).get();

        // Filter out equipment having a priority of 0 (=unavailable for cooling)
        if (coolingPriority > 0) {

          boost::optional<WorkspaceObject> wo =
            group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

          OS_ASSERT(wo);

          ModelObject mo = wo->cast<ModelObject>();

          equipmentMap.insert(std::make_pair(coolingPriority, mo));
        }
      }

      std::vector<ModelObject> equipmentVector;

      for (unsigned i = 1; i <= equipmentMap.size(); i++) {
        ModelObject mo = equipmentMap.find(i)->second;

        equipmentVector.push_back(mo);
      }

      return equipmentVector;
    }

    ThermalZone ZoneHVACEquipmentList_Impl::thermalZone() const {
      boost::optional<WorkspaceObject> wo = getTarget(OS_ZoneHVAC_EquipmentListFields::ThermalZone);

      OS_ASSERT(wo);

      return wo->cast<ThermalZone>();
    }

    bool ZoneHVACEquipmentList_Impl::removeEquipment(const ModelObject& equipment) {
      bool result = false;

      std::vector<ModelObject> coolingVector = equipmentInCoolingOrder();
      std::vector<ModelObject> heatingVector = equipmentInHeatingOrder();

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo = group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

        OS_ASSERT(wo);

        if (wo->handle() == equipment.handle()) {
          getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
          // Found it
          result = true;
          break;
        }
      }

      coolingVector.erase(std::find(coolingVector.begin(), coolingVector.end(), equipment));
      heatingVector.erase(std::find(heatingVector.begin(), heatingVector.end(), equipment));

      unsigned priority = 1;

      for (const auto& elem : coolingVector) {
        boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(elem);
        OS_ASSERT(eg);
        eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence, priority);

        priority++;
      }

      priority = 1;

      for (const auto& elem : heatingVector) {
        boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(elem);
        OS_ASSERT(eg);
        eg->setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence, priority);

        priority++;
      }

      return result;
    }

    unsigned ZoneHVACEquipmentList_Impl::heatingPriority(const ModelObject& equipment) const {
      boost::optional<unsigned> result;

      boost::optional<ModelExtensibleGroup> _eg = getGroupForModelObject(equipment);
      if (!_eg) {
        LOG(Warn, "Cannot get Heating Priority of an equipment that isn't in the ZoneHVACEquipmentList for " << briefDescription());
        return 0;
      } else {
        result = _eg->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence);
        OS_ASSERT(result);

        return result.get();
      }
    }

    unsigned ZoneHVACEquipmentList_Impl::coolingPriority(const ModelObject& equipment) const {
      boost::optional<unsigned> result;

      boost::optional<ModelExtensibleGroup> _eg = getGroupForModelObject(equipment);
      if (!_eg) {
        LOG(Warn, "Cannot get Cooling Priority of an equipment that isn't in the ZoneHVACEquipmentList for " << briefDescription());
        return 0;
      } else {
        result = _eg->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence);
        OS_ASSERT(result);

        return result.get();
      }
    }

    boost::optional<Schedule> ZoneHVACEquipmentList_Impl::sequentialCoolingFractionSchedule(const ModelObject& equipment) const {

      boost::optional<Schedule> result;

      if ((openstudio::istringEqual(loadDistributionScheme(), "SequentialLoad")) && (coolingPriority(equipment) > 0)) {

        std::vector<IdfExtensibleGroup> groups = extensibleGroups();

        for (const auto& group : groups) {
          boost::optional<WorkspaceObject> wo =
            group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

          OS_ASSERT(wo);

          if (wo->handle() == equipment.handle()) {
            boost::optional<WorkspaceObject> wo2 = group.cast<ModelExtensibleGroup>().getTarget(
              OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFractionScheduleName);
            if (wo2) {
              result = wo2->optionalCast<Schedule>();
              break;
            }
          }
        }
      }

      return result;
    }

    boost::optional<Schedule> ZoneHVACEquipmentList_Impl::sequentialHeatingFractionSchedule(const ModelObject& equipment) const {

      boost::optional<Schedule> result;

      if ((openstudio::istringEqual(loadDistributionScheme(), "SequentialLoad")) && (heatingPriority(equipment) > 0)) {

        std::vector<IdfExtensibleGroup> groups = extensibleGroups();

        for (const auto& group : groups) {
          boost::optional<WorkspaceObject> wo =
            group.cast<ModelExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

          OS_ASSERT(wo);

          if (wo->handle() == equipment.handle()) {
            boost::optional<WorkspaceObject> wo2 = group.cast<ModelExtensibleGroup>().getTarget(
              OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFractionScheduleName);
            if (wo2) {
              result = wo2->optionalCast<Schedule>();
              break;
            }
          }
        }
      }

      return result;
    }

    bool ZoneHVACEquipmentList_Impl::setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule) {
      if (schedule.model() != model()) {
        LOG(Info, "Cannot set Sequential Cooling Fraction for equipment " << equipment.nameString()
                                                                          << " with a schedule that isn't part of the same model.");
        return false;
      }

      boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(equipment);

      if (!eg) {
        LOG(Info,
            "Cannot set Sequential Cooling Fraction for equipment " << equipment.nameString() << " that isn't part of the ZoneHVACEquipmentList.");
        return false;
      }
      if (!openstudio::istringEqual(loadDistributionScheme(), "SequentialLoad")) {
        LOG(Info, "Cannot set Sequential Cooling Fraction for a Load Distribution Scheme other than 'SequentialLoad'");
        return false;
      }

      if (coolingPriority(equipment) == 0) {
        LOG(Info, "Cannot set Sequential Cooling Fraction for an equipment that doesn't have a cooling priority strictly greater than zero.");
        return false;
      }

      bool result = eg->setSchedule(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFractionScheduleName,
                                    "ZoneHVACEquipmentList", "Sequential Cooling Fraction", schedule);
      return result;
    }

    bool ZoneHVACEquipmentList_Impl::setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule) {
      if (schedule.model() != model()) {
        LOG(Info, "Cannot set Sequential Cooling Fraction for equipment " << equipment.nameString()
                                                                          << " with a schedule that isn't part of the same model.");
        return false;
      }

      boost::optional<ModelExtensibleGroup> eg = getGroupForModelObject(equipment);
      if (!eg) {
        LOG(Info,
            "Cannot set Sequential Heating Fraction for equipment " << equipment.nameString() << " that isn't part of the ZoneHVACEquipmentList.");
        return false;
      }
      if (!openstudio::istringEqual(loadDistributionScheme(), "SequentialLoad")) {
        LOG(Info, "Cannot set Sequential Heating Fraction for a Load Distribution Scheme other than 'SequentialLoad'");
        return false;
      }

      if (heatingPriority(equipment) == 0) {
        LOG(Info, "Cannot set Sequential Heating Fraction for an equipment that doesn't have a heating priority strictly greater than zero.");
        return false;
      }

      bool result = eg->setSchedule(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFractionScheduleName,
                                    "ZoneHVACEquipmentList", "Sequential Heating Fraction", schedule);
      return result;
    }

    // Deprecated

    boost::optional<double> ZoneHVACEquipmentList_Impl::sequentialCoolingFraction(const ModelObject& equipment) const {

      boost::optional<double> result;

      if (boost::optional<Schedule> _schedule = sequentialCoolingFractionSchedule(equipment)) {
        if (boost::optional<ScheduleConstant> _schConstant = _schedule->optionalCast<ScheduleConstant>()) {
          result = boost::optional<double>(_schConstant->value());
        } else {
          LOG(Warn, "This deprecated method cannot return a double when the "
                    "'Zone Equipment Sequential Cooling Fraction Schedule' isn't a Schedule:Constant, "
                    "here the schedule is a '"
                      << _schedule->iddObject().name() << ". Occurred for " << briefDescription());
        }
      }

      return result;
    }

    boost::optional<double> ZoneHVACEquipmentList_Impl::sequentialHeatingFraction(const ModelObject& equipment) const {

      boost::optional<double> result;

      if (boost::optional<Schedule> _schedule = sequentialHeatingFractionSchedule(equipment)) {
        if (boost::optional<ScheduleConstant> _schConstant = _schedule->optionalCast<ScheduleConstant>()) {
          result = boost::optional<double>(_schConstant->value());
        } else {
          LOG(Warn, "This deprecated method cannot return a double when the "
                    "'Zone Equipment Sequential Heating Fraction Schedule' isn't a Schedule:Constant, "
                    "here the schedule is a '"
                      << _schedule->iddObject().name() << ". Occurred for " << briefDescription());
        }
      }

      return result;
    }

    bool ZoneHVACEquipmentList_Impl::setSequentialCoolingFraction(const ModelObject& equipment, double fraction) {
      Model model = this->model();
      ScheduleConstant schedule(model);
      schedule.setValue(fraction);

      return setSequentialCoolingFractionSchedule(equipment, schedule);
    }

    bool ZoneHVACEquipmentList_Impl::setSequentialHeatingFraction(const ModelObject& equipment, double fraction) {
      Model model = this->model();
      ScheduleConstant schedule(model);
      schedule.setValue(fraction);

      return setSequentialHeatingFractionSchedule(equipment, schedule);
    }

  }  // namespace detail

  ZoneHVACEquipmentList::ZoneHVACEquipmentList(const ThermalZone& thermalZone)
    : ModelObject(ZoneHVACEquipmentList::iddObjectType(), thermalZone.model()) {
    OS_ASSERT(getImpl<detail::ZoneHVACEquipmentList_Impl>());

    setPointer(OS_ZoneHVAC_EquipmentListFields::ThermalZone, thermalZone.handle());
  }

  IddObjectType ZoneHVACEquipmentList::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ZoneHVAC_EquipmentList);
  }

  std::string ZoneHVACEquipmentList::loadDistributionScheme() const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->loadDistributionScheme();
  }

  std::vector<std::string> ZoneHVACEquipmentList::loadDistributionSchemeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneHVAC_EquipmentListFields::LoadDistributionScheme);
  }

  std::vector<std::string> ZoneHVACEquipmentList::validLoadDistributionSchemeValues() {
    return loadDistributionSchemeValues();
  }

  bool ZoneHVACEquipmentList::setLoadDistributionScheme(std::string scheme) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setLoadDistributionScheme(scheme);
  }

  bool ZoneHVACEquipmentList::addEquipment(const ModelObject& equipment) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->addEquipment(equipment);
  }

  bool ZoneHVACEquipmentList::setCoolingPriority(const ModelObject& equipment, unsigned priority) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setCoolingPriority(equipment, priority);
  }

  bool ZoneHVACEquipmentList::setHeatingPriority(const ModelObject& equipment, unsigned priority) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setHeatingPriority(equipment, priority);
  }

  std::vector<ModelObject> ZoneHVACEquipmentList::equipment() const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipment();
  }

  std::vector<ModelObject> ZoneHVACEquipmentList::equipmentInHeatingOrder() const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipmentInHeatingOrder();
  }

  std::vector<ModelObject> ZoneHVACEquipmentList::equipmentInCoolingOrder() const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipmentInCoolingOrder();
  }

  ThermalZone ZoneHVACEquipmentList::thermalZone() const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->thermalZone();
  }

  bool ZoneHVACEquipmentList::removeEquipment(const ModelObject& equipment) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->removeEquipment(equipment);
  }

  unsigned ZoneHVACEquipmentList::heatingPriority(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->heatingPriority(equipment);
  }

  unsigned ZoneHVACEquipmentList::coolingPriority(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->coolingPriority(equipment);
  }

  boost::optional<Schedule> ZoneHVACEquipmentList::sequentialCoolingFractionSchedule(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->sequentialCoolingFractionSchedule(equipment);
  }

  bool ZoneHVACEquipmentList::setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setSequentialCoolingFractionSchedule(equipment, schedule);
  }

  boost::optional<Schedule> ZoneHVACEquipmentList::sequentialHeatingFractionSchedule(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->sequentialHeatingFractionSchedule(equipment);
  }

  bool ZoneHVACEquipmentList::setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setSequentialHeatingFractionSchedule(equipment, schedule);
  }

  // DEPRECATED
  boost::optional<double> ZoneHVACEquipmentList::sequentialCoolingFraction(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->sequentialCoolingFraction(equipment);
  }

  bool ZoneHVACEquipmentList::setSequentialCoolingFraction(const ModelObject& equipment, double fraction) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setSequentialCoolingFraction(equipment, fraction);
  }

  boost::optional<double> ZoneHVACEquipmentList::sequentialHeatingFraction(const ModelObject& equipment) const {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->sequentialHeatingFraction(equipment);
  }

  bool ZoneHVACEquipmentList::setSequentialHeatingFraction(const ModelObject& equipment, double fraction) {
    return getImpl<detail::ZoneHVACEquipmentList_Impl>()->setSequentialHeatingFraction(equipment, fraction);
  }

  /// @cond
  ZoneHVACEquipmentList::ZoneHVACEquipmentList(std::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
