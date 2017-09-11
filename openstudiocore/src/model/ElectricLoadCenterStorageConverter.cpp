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

#include "ElectricLoadCenterStorageConverter.hpp"
#include "ElectricLoadCenterStorageConverter_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Storage_Converter_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricLoadCenterStorageConverter_Impl::ElectricLoadCenterStorageConverter_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterStorageConverter::iddObjectType());
  }

  ElectricLoadCenterStorageConverter_Impl::ElectricLoadCenterStorageConverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricLoadCenterStorageConverter::iddObjectType());
  }

  ElectricLoadCenterStorageConverter_Impl::ElectricLoadCenterStorageConverter_Impl(const ElectricLoadCenterStorageConverter_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricLoadCenterStorageConverter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Converter AC to DC Efficiency");
      result.push_back("Converter AC Input Electric Power");
      result.push_back("Converter AC Input Electric Energy");
      result.push_back("Converter DC Output Electric Power");
      result.push_back("Converter DC Output Electric Energy");
      result.push_back("Converter Electric Loss Power");
      result.push_back("Converter Electric Loss Energy");
      result.push_back("Converter Electric Loss Decrement Energy");
      result.push_back("Converter Thermal Loss Rate");
      result.push_back("Converter Thermal Loss Energy");
      result.push_back("Converter Ancillary AC Electric Power");
      result.push_back("Converter Ancillary AC Electric Energy");
    }
    return result;
  }

  IddObjectType ElectricLoadCenterStorageConverter_Impl::iddObjectType() const {
    return ElectricLoadCenterStorageConverter::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricLoadCenterStorageConverter_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricLoadCenterStorageConverter","Availability"));
    }
    return result;
  }

  // Private protected
  boost::optional<Schedule> ElectricLoadCenterStorageConverter_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName);
  }

  // Convenience method to find the ELCD linked to this storage device
  boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageConverter_Impl::electricLoadCenterDistribution() const {
    auto elcds = getObject<ModelObject>().getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
    if (elcds.empty()) {
      // no error
    } else if (elcds.size() == 1u) {
      return elcds[0];
    } else {
      // error
    }

    return boost::none;
  }

  Schedule ElectricLoadCenterStorageConverter_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      //LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      // I'm choosing to just return alwaysOnDiscreteSchedule
      return this->model().alwaysOnDiscreteSchedule();
    }
    return value.get();
  }

  bool ElectricLoadCenterStorageConverter_Impl::isAvailabilityScheduleDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName);
  }

  std::string ElectricLoadCenterStorageConverter_Impl::powerConversionEfficiencyMethod() const {
    boost::optional<std::string> value = getString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  // Required if powerConversionEfficiencyMethod == "SimpleFixed"
  boost::optional<double> ElectricLoadCenterStorageConverter_Impl::simpleFixedEfficiency() const {
    return getDouble(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency,false);
  }

  // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
  boost::optional<double> ElectricLoadCenterStorageConverter_Impl::designMaximumContinuousInputPower() const {
    return getDouble(OS_ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower,true);
  }

  // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
  boost::optional<Curve> ElectricLoadCenterStorageConverter_Impl::efficiencyFunctionofPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName);
  }

  double ElectricLoadCenterStorageConverter_Impl::ancillaryPowerConsumedInStandby() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterStorageConverter_Impl::isAncillaryPowerConsumedInStandbyDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby);
  }

  boost::optional<ThermalZone> ElectricLoadCenterStorageConverter_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ElectricLoadCenter_Storage_ConverterFields::ZoneName);
  }

  double ElectricLoadCenterStorageConverter_Impl::radiativeFraction() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterStorageConverter_Impl::isRadiativeFractionDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction);
  }

  // Setters

  bool ElectricLoadCenterStorageConverter_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName,
                              "ElectricLoadCenterStorageConverter",
                              "Availability",
                              schedule);
    return result;
  }

  void ElectricLoadCenterStorageConverter_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  // This method will automatically switch the PowerConversationEfficiencyMethod to "SimpleFixed" and reset the DesignMaximumContinuousInputPower and EfficiencyFunctionofPowerCurveName fields
  bool ElectricLoadCenterStorageConverter_Impl::setSimpleFixedEfficiency(double simpleFixedEfficiency) {
    // Assign the Simple Fixed Efficiency
    bool result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, simpleFixedEfficiency);
    if (result){
      // Switch the Method
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "SimpleFixed");
      OS_ASSERT(result);
      // Reset the two others
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower, "");
      OS_ASSERT(result);
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName, "");
      OS_ASSERT(result);
    }
    return result;
  }

  // This method will automatically switch the PowerConversationEfficiencyMethod to "FunctionOfPower" and reset the Simple Fixed Efficiency field.
  bool ElectricLoadCenterStorageConverter_Impl::setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower) {
    // Assign the Design Maximum Continuous Input Power
    bool result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower, designMaximumContinuousInputPower);
    if (result){
      // Switch the Method
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "FunctionOfPower");
      OS_ASSERT(result);
      // Reset the Simple Fixed Efficiency
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, "");
      OS_ASSERT(result);
    }
    return result;
  }

  // This method will automatically switch the PowerConversationEfficiencyMethod to "FunctionOfPower" and reset the Simple Fixed Efficiency field.
  bool ElectricLoadCenterStorageConverter_Impl::setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve) {
    // Set the curve
    bool result = setPointer(OS_ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName, efficiencyFunctionofPowerCurve.handle());
    if (result){
      // Switch the Method
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "FunctionOfPower");
      OS_ASSERT(result);
      // Reset the Simple Fixed Efficiency
      result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, "");
      OS_ASSERT(result);
    }
    return result;
  }

  bool ElectricLoadCenterStorageConverter_Impl::setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby, ancillaryPowerConsumedInStandby);
    return result;
  }

  void ElectricLoadCenterStorageConverter_Impl::resetAncillaryPowerConsumedInStandby() {
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageConverter_Impl::setThermalZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_ElectricLoadCenter_Storage_ConverterFields::ZoneName, thermalZone.handle());
    return result;
  }

  void ElectricLoadCenterStorageConverter_Impl::resetThermalZone() {
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterStorageConverter_Impl::setRadiativeFraction(double radiativeFraction) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction, radiativeFraction);
    return result;
  }

  void ElectricLoadCenterStorageConverter_Impl::resetRadiativeFraction() {
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction, "");
    OS_ASSERT(result);
  }

} // detail

ElectricLoadCenterStorageConverter::ElectricLoadCenterStorageConverter(const Model& model)
  : ParentObject(ElectricLoadCenterStorageConverter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricLoadCenterStorageConverter_Impl>());

  // This is actually the E+ default
  setSimpleFixedEfficiency(0.95);

  // Already defaults to alwaysOnDiscreteSchedule
  //auto availableSchedule = model.alwaysOnDiscreteSchedule();
  //setAvailabilitySchedule(availableSchedule);

}

IddObjectType ElectricLoadCenterStorageConverter::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Storage_Converter);
}

std::vector<std::string> ElectricLoadCenterStorageConverter::powerConversionEfficiencyMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod);
}

boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageConverter::electricLoadCenterDistribution() const
{
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->electricLoadCenterDistribution();
}

Schedule ElectricLoadCenterStorageConverter::availabilitySchedule() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->availabilitySchedule();
}

bool ElectricLoadCenterStorageConverter::isAvailabilityScheduleDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->isAvailabilityScheduleDefaulted();
}

std::string ElectricLoadCenterStorageConverter::powerConversionEfficiencyMethod() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->powerConversionEfficiencyMethod();
}

boost::optional<double> ElectricLoadCenterStorageConverter::simpleFixedEfficiency() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->simpleFixedEfficiency();
}

boost::optional<double> ElectricLoadCenterStorageConverter::designMaximumContinuousInputPower() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->designMaximumContinuousInputPower();
}

boost::optional<Curve> ElectricLoadCenterStorageConverter::efficiencyFunctionofPowerCurve() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->efficiencyFunctionofPowerCurve();
}

double ElectricLoadCenterStorageConverter::ancillaryPowerConsumedInStandby() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->ancillaryPowerConsumedInStandby();
}

bool ElectricLoadCenterStorageConverter::isAncillaryPowerConsumedInStandbyDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->isAncillaryPowerConsumedInStandbyDefaulted();
}

boost::optional<ThermalZone> ElectricLoadCenterStorageConverter::thermalZone() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->thermalZone();
}

double ElectricLoadCenterStorageConverter::radiativeFraction() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->radiativeFraction();
}

bool ElectricLoadCenterStorageConverter::isRadiativeFractionDefaulted() const {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->isRadiativeFractionDefaulted();
}


// Setters

bool ElectricLoadCenterStorageConverter::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setAvailabilitySchedule(schedule);
}

void ElectricLoadCenterStorageConverter::resetAvailabilitySchedule() {
  getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->resetAvailabilitySchedule();
}

bool ElectricLoadCenterStorageConverter::setSimpleFixedEfficiency(double simpleFixedEfficiency) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setSimpleFixedEfficiency(simpleFixedEfficiency);
}

bool ElectricLoadCenterStorageConverter::setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setDesignMaximumContinuousInputPower(designMaximumContinuousInputPower);
}

bool ElectricLoadCenterStorageConverter::setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setEfficiencyFunctionofPowerCurve(efficiencyFunctionofPowerCurve);
}

bool ElectricLoadCenterStorageConverter::setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setAncillaryPowerConsumedInStandby(ancillaryPowerConsumedInStandby);
}

void ElectricLoadCenterStorageConverter::resetAncillaryPowerConsumedInStandby() {
  getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->resetAncillaryPowerConsumedInStandby();
}

bool ElectricLoadCenterStorageConverter::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setThermalZone(thermalZone);
}

void ElectricLoadCenterStorageConverter::resetThermalZone() {
  getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->resetThermalZone();
}

bool ElectricLoadCenterStorageConverter::setRadiativeFraction(double radiativeFraction) {
  return getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->setRadiativeFraction(radiativeFraction);
}

void ElectricLoadCenterStorageConverter::resetRadiativeFraction() {
  getImpl<detail::ElectricLoadCenterStorageConverter_Impl>()->resetRadiativeFraction();
}

/// @cond
ElectricLoadCenterStorageConverter::ElectricLoadCenterStorageConverter(std::shared_ptr<detail::ElectricLoadCenterStorageConverter_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

