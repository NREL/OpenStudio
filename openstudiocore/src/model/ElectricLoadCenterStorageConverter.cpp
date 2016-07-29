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
  boost::optional<ElectricLoadCenterDistribution> ElectricLoadCenterStorageConverter_Impl::electricLoadCenterDistribution() const
  {
    boost::optional<ElectricLoadCenterDistribution> thiselcd;
    for (const ElectricLoadCenterDistribution& elcd : this->model().getConcreteModelObjects<ElectricLoadCenterDistribution>()) {
      if (boost::optional<ElectricLoadCenterStorageConverter> elcConv = elcd.storageConverter()) {
        if (elcConv->handle() == this->handle()) {
          thiselcd = elcd;
        }
      }
    }
    OS_ASSERT(thiselcd);
    return thiselcd.get();
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
    // Switch the Method
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "SimpleFixed");
    OS_ASSERT(result);
    // Assign the Simple Fixed Efficiency
    result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, simpleFixedEfficiency);
    OS_ASSERT(result);
    // Reset the two others
    result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower, "");
    OS_ASSERT(result);
    result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName, "");
    OS_ASSERT(result);
    return result;
  }

  // This method will automatically switch the PowerConversationEfficiencyMethod to "FunctionOfPower" and reset the Simple Fixed Efficiency field.
  bool ElectricLoadCenterStorageConverter_Impl::setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower) {
    // Switch the Method
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "FunctionOfPower");
    OS_ASSERT(result);
    // Assign the Design Maximum Continuous Input Power
    result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower, designMaximumContinuousInputPower);
    OS_ASSERT(result);
    // Reset the Simple Fixed Efficiency
    result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, "");
    OS_ASSERT(result);
    return result;
  }

  // This method will automatically switch the PowerConversationEfficiencyMethod to "FunctionOfPower" and reset the Simple Fixed Efficiency field.
  bool ElectricLoadCenterStorageConverter_Impl::setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve) {
    // Switch the Method
    bool result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, "FunctionOfPower");
    OS_ASSERT(result);
    // Set the curve
    result = setPointer(OS_ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName, efficiencyFunctionofPowerCurve.handle());
    OS_ASSERT(result);
    // Reset the Simple Fixed Efficiency
    result = setString(OS_ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, "");
    OS_ASSERT(result);
    return result;
  }

  bool ElectricLoadCenterStorageConverter_Impl::setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby) {
    bool result = setDouble(OS_ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby, ancillaryPowerConsumedInStandby);
    OS_ASSERT(result);
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
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

