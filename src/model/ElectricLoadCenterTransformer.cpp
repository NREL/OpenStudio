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

#include "ElectricLoadCenterTransformer.hpp"
#include "ElectricLoadCenterTransformer_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Transformer_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ElectricLoadCenterTransformer_Impl::ElectricLoadCenterTransformer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterTransformer::iddObjectType());
    }

    ElectricLoadCenterTransformer_Impl::ElectricLoadCenterTransformer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ElectricLoadCenterTransformer::iddObjectType());
    }

    ElectricLoadCenterTransformer_Impl::ElectricLoadCenterTransformer_Impl(const ElectricLoadCenterTransformer_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ElectricLoadCenterTransformer_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Transformer Efficiency",
                                                   "Transformer Input Electricity Rate",
                                                   "Transformer Input Electricity Energy",
                                                   "Transformer Output Electricity Rate",
                                                   "Transformer Output Electricity Energy",
                                                   "Transformer No Load Loss Rate",
                                                   "Transformer No Load Loss Energy",
                                                   "Transformer Load Loss Rate",
                                                   "Transformer Load Loss Energy",
                                                   "Transformer Thermal Loss Rate",
                                                   "Transformer Thermal Loss Energy",
                                                   "Transformer Distribution Electricity Loss Energy",
                                                   "Transformer Cogeneration Electricity Loss Energy",
                                                   "Transformer Conversion Electricity Loss Energy"};
      return result;
    }

    IddObjectType ElectricLoadCenterTransformer_Impl::iddObjectType() const {
      return ElectricLoadCenterTransformer::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ElectricLoadCenterTransformer_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_ElectricLoadCenter_TransformerFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ElectricLoadCenterTransformer", "Availability"));
      }
      return result;
    }

    boost::optional<Schedule> ElectricLoadCenterTransformer_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_TransformerFields::AvailabilityScheduleName);
    }

    std::string ElectricLoadCenterTransformer_Impl::transformerUsage() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_TransformerFields::TransformerUsage, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isTransformerUsageDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::TransformerUsage);
    }

    boost::optional<ThermalZone> ElectricLoadCenterTransformer_Impl::zone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_TransformerFields::ZoneName);
    }

    double ElectricLoadCenterTransformer_Impl::radiativeFraction() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::RadiativeFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isRadiativeFractionDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::RadiativeFraction);
    }

    boost::optional<double> ElectricLoadCenterTransformer_Impl::ratedCapacity() const {
      return getDouble(OS_ElectricLoadCenter_TransformerFields::RatedCapacity, true);
    }

    std::string ElectricLoadCenterTransformer_Impl::phase() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_TransformerFields::Phase, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isPhaseDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::Phase);
    }

    std::string ElectricLoadCenterTransformer_Impl::conductorMaterial() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_TransformerFields::ConductorMaterial, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isConductorMaterialDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::ConductorMaterial);
    }

    double ElectricLoadCenterTransformer_Impl::fullLoadTemperatureRise() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isFullLoadTemperatureRiseDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise);
    }

    double ElectricLoadCenterTransformer_Impl::fractionofEddyCurrentLosses() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isFractionofEddyCurrentLossesDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses);
    }

    std::string ElectricLoadCenterTransformer_Impl::performanceInputMethod() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_TransformerFields::PerformanceInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isPerformanceInputMethodDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::PerformanceInputMethod);
    }

    boost::optional<double> ElectricLoadCenterTransformer_Impl::ratedNoLoadLoss() const {
      return getDouble(OS_ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, true);
    }

    boost::optional<double> ElectricLoadCenterTransformer_Impl::ratedLoadLoss() const {
      return getDouble(OS_ElectricLoadCenter_TransformerFields::RatedLoadLoss, true);
    }

    double ElectricLoadCenterTransformer_Impl::nameplateEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::NameplateEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isNameplateEfficiencyDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::NameplateEfficiency);
    }

    double ElectricLoadCenterTransformer_Impl::perUnitLoadforNameplateEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isPerUnitLoadforNameplateEfficiencyDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency);
    }

    double ElectricLoadCenterTransformer_Impl::referenceTemperatureforNameplateEfficiency() const {
      boost::optional<double> value = getDouble(OS_ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ElectricLoadCenterTransformer_Impl::isReferenceTemperatureforNameplateEfficiencyDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency);
    }

    boost::optional<double> ElectricLoadCenterTransformer_Impl::perUnitLoadforMaximumEfficiency() const {
      return getDouble(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, true);
    }

    bool ElectricLoadCenterTransformer_Impl::considerTransformerLossforUtilityCost() const {
      boost::optional<std::string> value = getString(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ElectricLoadCenterTransformer_Impl::isConsiderTransformerLossforUtilityCostDefaulted() const {
      return isEmpty(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost);
    }

    bool ElectricLoadCenterTransformer_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, "ElectricLoadCenterTransformer", "Availability", schedule);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setTransformerUsage(const std::string& transformerUsage) {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::TransformerUsage, transformerUsage);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetTransformerUsage() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::TransformerUsage, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_ElectricLoadCenter_TransformerFields::ZoneName, thermalZone.handle());
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetZone() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::ZoneName, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setRadiativeFraction(double radiativeFraction) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::RadiativeFraction, radiativeFraction);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetRadiativeFraction() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::RadiativeFraction, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setRatedCapacity(double ratedCapacity) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::RatedCapacity, ratedCapacity);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetRatedCapacity() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::RatedCapacity, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setPhase(const std::string& phase) {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::Phase, phase);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetPhase() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::Phase, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setConductorMaterial(const std::string& conductorMaterial) {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::ConductorMaterial, conductorMaterial);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetConductorMaterial() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::ConductorMaterial, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setFullLoadTemperatureRise(double fullLoadTemperatureRise) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, fullLoadTemperatureRise);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetFullLoadTemperatureRise() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setFractionofEddyCurrentLosses(double fractionofEddyCurrentLosses) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, fractionofEddyCurrentLosses);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetFractionofEddyCurrentLosses() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setPerformanceInputMethod(const std::string& performanceInputMethod) {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::PerformanceInputMethod, performanceInputMethod);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetPerformanceInputMethod() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::PerformanceInputMethod, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setRatedNoLoadLoss(double ratedNoLoadLoss) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, ratedNoLoadLoss);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetRatedNoLoadLoss() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setRatedLoadLoss(double ratedLoadLoss) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::RatedLoadLoss, ratedLoadLoss);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetRatedLoadLoss() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::RatedLoadLoss, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setNameplateEfficiency(double nameplateEfficiency) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::NameplateEfficiency, nameplateEfficiency);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetNameplateEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::NameplateEfficiency, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setPerUnitLoadforNameplateEfficiency(double perUnitLoadforNameplateEfficiency) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, perUnitLoadforNameplateEfficiency);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetPerUnitLoadforNameplateEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setReferenceTemperatureforNameplateEfficiency(double referenceTemperatureforNameplateEfficiency) {
      bool result =
        setDouble(OS_ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency, referenceTemperatureforNameplateEfficiency);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetReferenceTemperatureforNameplateEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setPerUnitLoadforMaximumEfficiency(double perUnitLoadforMaximumEfficiency) {
      bool result = setDouble(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, perUnitLoadforMaximumEfficiency);
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::resetPerUnitLoadforMaximumEfficiency() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, "");
      OS_ASSERT(result);
    }

    bool ElectricLoadCenterTransformer_Impl::setConsiderTransformerLossforUtilityCost(bool considerTransformerLossforUtilityCost) {
      return setBooleanFieldValue(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost,
                                  considerTransformerLossforUtilityCost);
      //if (considerTransformerLossforUtilityCost) {
      //  result = setBooleanFieldValue(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, "Yes");
      //} else {
      //  result = setBooleanFieldValue(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, "No");
      //}
    }

    void ElectricLoadCenterTransformer_Impl::resetConsiderTransformerLossforUtilityCost() {
      bool result = setString(OS_ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, "");
      OS_ASSERT(result);
    }

    boost::optional<std::string> ElectricLoadCenterTransformer_Impl::getMeter(unsigned index) const {
      //return meter at index
      boost::optional<std::string> result;
      auto groups = extensibleGroups();
      unsigned sizeOfGroup = numExtensibleGroups();

      if ((index < sizeOfGroup) && (!groups[index].empty())) {
        WorkspaceExtensibleGroup group = groups[index].cast<WorkspaceExtensibleGroup>();
        boost::optional<std::string> wo = group.getString(OS_ElectricLoadCenter_TransformerExtensibleFields::MeterName);
        if (wo) {
          result = wo.get();
        }
      }
      return result;
    }

    std::vector<std::string> ElectricLoadCenterTransformer_Impl::meters() const {
      // loop through extensible groups and return vector of meters
      std::vector<std::string> result;
      auto groups = extensibleGroups();

      for (const auto& elem : groups) {
        WorkspaceExtensibleGroup group = elem.cast<WorkspaceExtensibleGroup>();
        boost::optional<std::string> wo = group.getString(OS_ElectricLoadCenter_TransformerExtensibleFields::MeterName);
        if (wo) {
          std::string meter = wo.get();
          result.push_back(meter);
        }
      }
      return result;
    }

    bool ElectricLoadCenterTransformer_Impl::eraseMeter(unsigned index) {
      //erase program at index
      bool result = false;
      unsigned sizeOfGroup = numExtensibleGroups();

      if (index < sizeOfGroup) {
        eraseExtensibleGroup(index);
        result = true;
      }
      return result;
    }

    void ElectricLoadCenterTransformer_Impl::eraseMeters() {
      //erase all meters in this transformer
      clearExtensibleGroups();
    }

    bool ElectricLoadCenterTransformer_Impl::addMeter(const std::string& meterName) {
      //add meter to end of vector of meters
      bool result = false;
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      result = group.setString(OS_ElectricLoadCenter_TransformerExtensibleFields::MeterName, meterName);
      return result;
    }

    bool ElectricLoadCenterTransformer_Impl::setMeter(const std::string& meterName, unsigned index) {
      //add meter to {index} of vector of meters
      bool result = false;
      unsigned sizeOfGroup = numExtensibleGroups();

      if (index <= sizeOfGroup) {
        IdfExtensibleGroup idfGroup = insertExtensibleGroup(index, StringVector());
        OS_ASSERT(!idfGroup.empty());
        ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
        result = group.setString(0, meterName);
      }
      return result;
    }

  }  // namespace detail

  ElectricLoadCenterTransformer::ElectricLoadCenterTransformer(const Model& model)
    : ModelObject(ElectricLoadCenterTransformer::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ElectricLoadCenterTransformer_Impl>());

    ScheduleCompact alwaysOn(model, 1.0);
    alwaysOn.setName("Always On");
    setAvailabilitySchedule(alwaysOn);
    setTransformerUsage("PowerOutToGrid");
    setRatedCapacity(15000);
    //setPhase("3");
    //setConductorMaterial("Aluminum");
    //setFullLoadTemperatureRise(150);
    //setFractionofEddyCurrentLosses(0.1);
    setPerformanceInputMethod("NominalEfficiency");
    //setNameplateEfficiency(0.985);
    //setPerUnitLoadforNameplateEfficiency(0.35);
    //setReferenceTemperatureforNameplateEfficiency(75);
    //setConsiderTransformerLossforUtilityCost(true);
  }

  IddObjectType ElectricLoadCenterTransformer::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Transformer);
  }

  std::vector<std::string> ElectricLoadCenterTransformer::transformerUsageValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ElectricLoadCenter_TransformerFields::TransformerUsage);
  }

  std::vector<std::string> ElectricLoadCenterTransformer::phaseValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ElectricLoadCenter_TransformerFields::Phase);
  }

  std::vector<std::string> ElectricLoadCenterTransformer::conductorMaterialValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ElectricLoadCenter_TransformerFields::ConductorMaterial);
  }

  std::vector<std::string> ElectricLoadCenterTransformer::performanceInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ElectricLoadCenter_TransformerFields::PerformanceInputMethod);
  }

  boost::optional<Schedule> ElectricLoadCenterTransformer::availabilitySchedule() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->availabilitySchedule();
  }

  std::string ElectricLoadCenterTransformer::transformerUsage() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->transformerUsage();
  }

  bool ElectricLoadCenterTransformer::isTransformerUsageDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isTransformerUsageDefaulted();
  }

  boost::optional<ThermalZone> ElectricLoadCenterTransformer::zone() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->zone();
  }

  double ElectricLoadCenterTransformer::radiativeFraction() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->radiativeFraction();
  }

  bool ElectricLoadCenterTransformer::isRadiativeFractionDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isRadiativeFractionDefaulted();
  }

  boost::optional<double> ElectricLoadCenterTransformer::ratedCapacity() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->ratedCapacity();
  }

  std::string ElectricLoadCenterTransformer::phase() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->phase();
  }

  bool ElectricLoadCenterTransformer::isPhaseDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isPhaseDefaulted();
  }

  std::string ElectricLoadCenterTransformer::conductorMaterial() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->conductorMaterial();
  }

  bool ElectricLoadCenterTransformer::isConductorMaterialDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isConductorMaterialDefaulted();
  }

  double ElectricLoadCenterTransformer::fullLoadTemperatureRise() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->fullLoadTemperatureRise();
  }

  bool ElectricLoadCenterTransformer::isFullLoadTemperatureRiseDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isFullLoadTemperatureRiseDefaulted();
  }

  double ElectricLoadCenterTransformer::fractionofEddyCurrentLosses() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->fractionofEddyCurrentLosses();
  }

  bool ElectricLoadCenterTransformer::isFractionofEddyCurrentLossesDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isFractionofEddyCurrentLossesDefaulted();
  }

  std::string ElectricLoadCenterTransformer::performanceInputMethod() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->performanceInputMethod();
  }

  bool ElectricLoadCenterTransformer::isPerformanceInputMethodDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isPerformanceInputMethodDefaulted();
  }

  boost::optional<double> ElectricLoadCenterTransformer::ratedNoLoadLoss() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->ratedNoLoadLoss();
  }

  boost::optional<double> ElectricLoadCenterTransformer::ratedLoadLoss() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->ratedLoadLoss();
  }

  double ElectricLoadCenterTransformer::nameplateEfficiency() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->nameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::isNameplateEfficiencyDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isNameplateEfficiencyDefaulted();
  }

  double ElectricLoadCenterTransformer::perUnitLoadforNameplateEfficiency() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->perUnitLoadforNameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::isPerUnitLoadforNameplateEfficiencyDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isPerUnitLoadforNameplateEfficiencyDefaulted();
  }

  double ElectricLoadCenterTransformer::referenceTemperatureforNameplateEfficiency() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->referenceTemperatureforNameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::isReferenceTemperatureforNameplateEfficiencyDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isReferenceTemperatureforNameplateEfficiencyDefaulted();
  }

  boost::optional<double> ElectricLoadCenterTransformer::perUnitLoadforMaximumEfficiency() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->perUnitLoadforMaximumEfficiency();
  }

  bool ElectricLoadCenterTransformer::considerTransformerLossforUtilityCost() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->considerTransformerLossforUtilityCost();
  }

  bool ElectricLoadCenterTransformer::isConsiderTransformerLossforUtilityCostDefaulted() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->isConsiderTransformerLossforUtilityCostDefaulted();
  }

  bool ElectricLoadCenterTransformer::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setAvailabilitySchedule(schedule);
  }

  void ElectricLoadCenterTransformer::resetAvailabilitySchedule() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetAvailabilitySchedule();
  }

  bool ElectricLoadCenterTransformer::setTransformerUsage(const std::string& transformerUsage) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setTransformerUsage(transformerUsage);
  }

  void ElectricLoadCenterTransformer::resetTransformerUsage() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetTransformerUsage();
  }

  bool ElectricLoadCenterTransformer::setZone(const ThermalZone& thermalZone) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setZone(thermalZone);
  }

  void ElectricLoadCenterTransformer::resetZone() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetZone();
  }

  bool ElectricLoadCenterTransformer::setRadiativeFraction(double radiativeFraction) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setRadiativeFraction(radiativeFraction);
  }

  void ElectricLoadCenterTransformer::resetRadiativeFraction() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetRadiativeFraction();
  }

  bool ElectricLoadCenterTransformer::setRatedCapacity(double ratedCapacity) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setRatedCapacity(ratedCapacity);
  }

  void ElectricLoadCenterTransformer::resetRatedCapacity() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetRatedCapacity();
  }

  bool ElectricLoadCenterTransformer::setPhase(const std::string& phase) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setPhase(phase);
  }

  void ElectricLoadCenterTransformer::resetPhase() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetPhase();
  }

  bool ElectricLoadCenterTransformer::setConductorMaterial(const std::string& conductorMaterial) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setConductorMaterial(conductorMaterial);
  }

  void ElectricLoadCenterTransformer::resetConductorMaterial() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetConductorMaterial();
  }

  bool ElectricLoadCenterTransformer::setFullLoadTemperatureRise(double fullLoadTemperatureRise) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setFullLoadTemperatureRise(fullLoadTemperatureRise);
  }

  void ElectricLoadCenterTransformer::resetFullLoadTemperatureRise() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetFullLoadTemperatureRise();
  }

  bool ElectricLoadCenterTransformer::setFractionofEddyCurrentLosses(double fractionofEddyCurrentLosses) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setFractionofEddyCurrentLosses(fractionofEddyCurrentLosses);
  }

  void ElectricLoadCenterTransformer::resetFractionofEddyCurrentLosses() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetFractionofEddyCurrentLosses();
  }

  bool ElectricLoadCenterTransformer::setPerformanceInputMethod(const std::string& performanceInputMethod) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setPerformanceInputMethod(performanceInputMethod);
  }

  void ElectricLoadCenterTransformer::resetPerformanceInputMethod() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetPerformanceInputMethod();
  }

  bool ElectricLoadCenterTransformer::setRatedNoLoadLoss(double ratedNoLoadLoss) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setRatedNoLoadLoss(ratedNoLoadLoss);
  }

  void ElectricLoadCenterTransformer::resetRatedNoLoadLoss() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetRatedNoLoadLoss();
  }

  bool ElectricLoadCenterTransformer::setRatedLoadLoss(double ratedLoadLoss) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setRatedLoadLoss(ratedLoadLoss);
  }

  void ElectricLoadCenterTransformer::resetRatedLoadLoss() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetRatedLoadLoss();
  }

  bool ElectricLoadCenterTransformer::setNameplateEfficiency(double nameplateEfficiency) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setNameplateEfficiency(nameplateEfficiency);
  }

  void ElectricLoadCenterTransformer::resetNameplateEfficiency() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetNameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::setPerUnitLoadforNameplateEfficiency(double perUnitLoadforNameplateEfficiency) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setPerUnitLoadforNameplateEfficiency(perUnitLoadforNameplateEfficiency);
  }

  void ElectricLoadCenterTransformer::resetPerUnitLoadforNameplateEfficiency() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetPerUnitLoadforNameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::setReferenceTemperatureforNameplateEfficiency(double referenceTemperatureforNameplateEfficiency) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setReferenceTemperatureforNameplateEfficiency(
      referenceTemperatureforNameplateEfficiency);
  }

  void ElectricLoadCenterTransformer::resetReferenceTemperatureforNameplateEfficiency() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetReferenceTemperatureforNameplateEfficiency();
  }

  bool ElectricLoadCenterTransformer::setPerUnitLoadforMaximumEfficiency(double perUnitLoadforMaximumEfficiency) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setPerUnitLoadforMaximumEfficiency(perUnitLoadforMaximumEfficiency);
  }

  void ElectricLoadCenterTransformer::resetPerUnitLoadforMaximumEfficiency() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetPerUnitLoadforMaximumEfficiency();
  }

  bool ElectricLoadCenterTransformer::setConsiderTransformerLossforUtilityCost(bool considerTransformerLossforUtilityCost) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setConsiderTransformerLossforUtilityCost(considerTransformerLossforUtilityCost);
  }

  void ElectricLoadCenterTransformer::resetConsiderTransformerLossforUtilityCost() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->resetConsiderTransformerLossforUtilityCost();
  }

  boost::optional<std::string> ElectricLoadCenterTransformer::getMeter(unsigned index) const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->getMeter(index);
  }

  std::vector<std::string> ElectricLoadCenterTransformer::meters() const {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->meters();
  }

  bool ElectricLoadCenterTransformer::eraseMeter(unsigned index) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->eraseMeter(index);
  }

  void ElectricLoadCenterTransformer::eraseMeters() {
    getImpl<detail::ElectricLoadCenterTransformer_Impl>()->eraseMeters();
  }

  bool ElectricLoadCenterTransformer::addMeter(const std::string& meterName) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->addMeter(meterName);
  }

  bool ElectricLoadCenterTransformer::setMeter(const std::string& meterName, unsigned index) {
    return getImpl<detail::ElectricLoadCenterTransformer_Impl>()->setMeter(meterName, index);
  }

  /// @cond
  ElectricLoadCenterTransformer::ElectricLoadCenterTransformer(std::shared_ptr<detail::ElectricLoadCenterTransformer_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
