/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CalibrationResult.hpp"
#include "../core/Assert.hpp"
#include "../time/Date.hpp"

namespace openstudio {

std::string CalibrationBillingPeriod::attributeName() {
  return "CalibrationBillingPeriod";
}

boost::optional<CalibrationBillingPeriod> CalibrationBillingPeriod::fromAttribute(const Attribute& attribute) {
  if (attribute.name() != CalibrationBillingPeriod::attributeName()) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute because attribute name is '" << attribute.name() << "'.");
    return boost::none;
  }

  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute because attribute is a " << attribute.valueType().valueDescription()
                                                                                              << ", not an AttributeVector.");
    return boost::none;
  }

  if (attribute.units()) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute because attribute has units '" << attribute.units().get() << "'.");
    return boost::none;
  }

  // at minimum need startDate, numberOfDays, consumptionUnit
  boost::optional<Attribute> test = attribute.findChildByName("startDate");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute without 'startDate'.");
    return boost::none;
  }
  try {
    // cppcheck-suppress unusedScopedObject
    Date(test->valueAsString());
  } catch (const std::exception&) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute with improper 'startDate'.");
    return boost::none;
  }

  test = attribute.findChildByName("numberOfDays");
  if (!test || !(test->valueType() == AttributeValueType::Unsigned)) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute without 'numberOfDays'.");
    return boost::none;
  }

  test = attribute.findChildByName("consumptionUnit");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationBillingPeriod from Attribute without 'consumptionUnit'.");
    return boost::none;
  }

  return CalibrationBillingPeriod(attribute);
}

CalibrationBillingPeriod::CalibrationBillingPeriod(const Date& startDate, unsigned numberOfDays, const std::string& consumptionUnit,
                                                   boost::optional<std::string> peakDemandUnit, boost::optional<double> consumption,
                                                   boost::optional<double> peakDemand, boost::optional<double> totalCost,
                                                   boost::optional<double> modelConsumption, boost::optional<double> modelPeakDemand,
                                                   boost::optional<double> modelTotalCost)
  : m_attribute(CalibrationBillingPeriod::attributeName(), std::vector<openstudio::Attribute>(), boost::none) {
  std::vector<Attribute> values;

  std::stringstream ss;
  ss << startDate;
  values.push_back(Attribute("startDate", ss.str()));
  values.push_back(Attribute("numberOfDays", numberOfDays));
  values.push_back(Attribute("consumptionUnit", consumptionUnit));
  if (peakDemandUnit) {
    values.push_back(Attribute("peakDemandUnit", *peakDemandUnit));
  }

  if (consumption) {
    values.push_back(Attribute("consumption", *consumption, consumptionUnit));
  }
  if (peakDemand) {
    if (peakDemandUnit) {
      values.push_back(Attribute("peakDemand", *peakDemand, *peakDemandUnit));
    }
  }
  if (totalCost) {
    values.push_back(Attribute("totalCost", *totalCost));
  }

  if (modelConsumption) {
    values.push_back(Attribute("modelConsumption", *modelConsumption, consumptionUnit));
  }
  if (modelPeakDemand) {
    if (peakDemandUnit) {
      values.push_back(Attribute("modelPeakDemand", *modelPeakDemand, *peakDemandUnit));
    }
  }
  if (modelTotalCost) {
    values.push_back(Attribute("modelTotalCost", *modelTotalCost));
  }

  m_attribute.setValue(values);
}

CalibrationBillingPeriod::CalibrationBillingPeriod(const Attribute& attribute) : m_attribute(attribute) {}

Attribute CalibrationBillingPeriod::attribute() const {
  return m_attribute;
}

Date CalibrationBillingPeriod::startDate() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("startDate");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return Date(attribute->valueAsString());
}

Date CalibrationBillingPeriod::endDate() const {
  Date result = this->startDate() + Time(this->numberOfDays() - 1);
  return result;
}

unsigned CalibrationBillingPeriod::numberOfDays() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("numberOfDays");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::Unsigned);
  return attribute->valueAsUnsigned();
}

std::string CalibrationBillingPeriod::consumptionUnit() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("consumptionUnit");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return attribute->valueAsString();
}

boost::optional<std::string> CalibrationBillingPeriod::peakDemandUnit() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("peakDemandUnit");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::String);
    return attribute->valueAsString();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::consumption() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("consumption");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::peakDemand() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("peakDemand");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::totalCost() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("totalCost");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::modelConsumption() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("modelConsumption");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::modelPeakDemand() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("modelPeakDemand");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationBillingPeriod::modelTotalCost() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("modelTotalCost");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

std::string CalibrationUtilityBill::attributeName() {
  return "CalibrationUtilityBill";
}

boost::optional<CalibrationUtilityBill> CalibrationUtilityBill::fromAttribute(const Attribute& attribute) {
  if (attribute.name() != CalibrationUtilityBill::attributeName()) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute because attribute name is '" << attribute.name() << "'.");
    return boost::none;
  }

  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute because attribute is a " << attribute.valueType().valueDescription()
                                                                                            << ", not an AttributeVector.");
    return boost::none;
  }

  if (attribute.units()) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute because attribute has units '" << attribute.units().get() << "'.");
    return boost::none;
  }

  // minimum required attributes are name, fuelType, meterInstallLocation, consumptionUnit, consumptionUnitConversionFactor
  boost::optional<Attribute> test = attribute.findChildByName("name");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute without 'name'.");
    return boost::none;
  }

  test = attribute.findChildByName("fuelType");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute without 'fuelType'.");
    return boost::none;
  }
  try {
    // cppcheck-suppress unusedScopedObject
    FuelType(test->valueAsString());
  } catch (const std::exception&) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute with improper 'fuelType'.");
    return boost::none;
  }

  test = attribute.findChildByName("meterInstallLocation");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute without 'meterInstallLocation'.");
    return boost::none;
  }
  try {
    // cppcheck-suppress unusedScopedObject
    InstallLocationType(test->valueAsString());
  } catch (const std::exception&) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute with improper 'meterInstallLocation'.");
    return boost::none;
  }

  test = attribute.findChildByName("consumptionUnit");
  if (!test || !(test->valueType() == AttributeValueType::String)) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute without 'consumptionUnit'.");
    return boost::none;
  }

  test = attribute.findChildByName("consumptionUnitConversionFactor");
  if (!test || !(test->valueType() == AttributeValueType::Double)) {
    LOG(Warn, "Cannot create CalibrationUtilityBill from Attribute without 'consumptionUnitConversionFactor'.");
    return boost::none;
  }

  return CalibrationUtilityBill(attribute);
}

CalibrationUtilityBill::CalibrationUtilityBill(
  const std::string& name, const FuelType& fuelType, const InstallLocationType& meterInstallLocation,
  boost::optional<std::string> meterSpecificInstallLocation, boost::optional<EndUseCategoryType> meterEndUseCategory,
  boost::optional<std::string> meterSpecificEndUse, const std::string& consumptionUnit, double consumptionUnitConversionFactor,
  boost::optional<std::string> peakDemandUnit, boost::optional<double> peakDemandUnitConversionFactor,
  boost::optional<unsigned> timestepsInPeakDemandWindow, boost::optional<double> minutesInPeakDemandWindow,
  boost::optional<unsigned> numberBillingPeriodsInCalculations, boost::optional<double> CVRMSE, boost::optional<double> NMBE)
  : m_attribute(CalibrationUtilityBill::attributeName(), std::vector<openstudio::Attribute>(), boost::none) {
  std::vector<Attribute> values;

  values.push_back(Attribute("name", name));
  values.push_back(Attribute("fuelType", fuelType.valueName()));
  values.push_back(Attribute("meterInstallLocation", meterInstallLocation.valueName()));
  if (meterSpecificInstallLocation) {
    values.push_back(Attribute("meterSpecificInstallLocation", *meterSpecificInstallLocation));
  }
  if (meterEndUseCategory) {
    values.push_back(Attribute("meterEndUseCategory", meterEndUseCategory->valueName()));
  }
  if (meterSpecificEndUse) {
    values.push_back(Attribute("meterSpecificEndUse", *meterSpecificEndUse));
  }
  values.push_back(Attribute("consumptionUnit", consumptionUnit));
  values.push_back(Attribute("consumptionUnitConversionFactor", consumptionUnitConversionFactor));
  if (peakDemandUnit) {
    values.push_back(Attribute("peakDemandUnit", *peakDemandUnit));
  }
  if (peakDemandUnitConversionFactor) {
    values.push_back(Attribute("peakDemandUnitConversionFactor", *peakDemandUnitConversionFactor));
  }
  if (timestepsInPeakDemandWindow) {
    values.push_back(Attribute("timestepsInPeakDemandWindow", *timestepsInPeakDemandWindow));
  }
  if (minutesInPeakDemandWindow) {
    values.push_back(Attribute("minutesInPeakDemandWindow", *minutesInPeakDemandWindow));
  }
  if (numberBillingPeriodsInCalculations) {
    values.push_back(Attribute("numberBillingPeriodsInCalculations", *numberBillingPeriodsInCalculations));
  }
  if (CVRMSE) {
    values.push_back(Attribute("cvrmse", *CVRMSE));
  }
  if (NMBE) {
    values.push_back(Attribute("nmbe", *NMBE));
  }

  values.push_back(Attribute("billingPeriods", std::vector<Attribute>()));

  m_attribute.setValue(values);
}

CalibrationUtilityBill::CalibrationUtilityBill(const Attribute& attribute) : m_attribute(attribute) {}

Attribute CalibrationUtilityBill::attribute() const {
  return m_attribute;
}

std::string CalibrationUtilityBill::name() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("name");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return attribute->valueAsString();
}

FuelType CalibrationUtilityBill::fuelType() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("fuelType");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return FuelType(attribute->valueAsString());
}

InstallLocationType CalibrationUtilityBill::meterInstallLocation() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("meterInstallLocation");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return InstallLocationType(attribute->valueAsString());
}

boost::optional<std::string> CalibrationUtilityBill::meterSpecificInstallLocation() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("meterSpecificInstallLocation");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::String);
    return attribute->valueAsString();
  }
  return boost::none;
}

boost::optional<EndUseCategoryType> CalibrationUtilityBill::meterEndUseCategory() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("meterEndUseCategory");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::String);
    return EndUseCategoryType(attribute->valueAsString());
  }
  return boost::none;
}

boost::optional<std::string> CalibrationUtilityBill::meterSpecificEndUse() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("meterSpecificEndUse");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::String);
    return attribute->valueAsString();
  }
  return boost::none;
}

std::string CalibrationUtilityBill::consumptionUnit() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("consumptionUnit");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::String);
  return attribute->valueAsString();
}

double CalibrationUtilityBill::consumptionUnitConversionFactor() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("consumptionUnitConversionFactor");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
  return attribute->valueAsDouble();
}

boost::optional<std::string> CalibrationUtilityBill::peakDemandUnit() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("peakDemandUnit");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::String);
    return attribute->valueAsString();
  }
  return boost::none;
}

boost::optional<double> CalibrationUtilityBill::peakDemandUnitConversionFactor() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("peakDemandUnitConversionFactor");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<unsigned> CalibrationUtilityBill::timestepsInPeakDemandWindow() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("timestepsInPeakDemandWindow");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Unsigned);
    return attribute->valueAsUnsigned();
  }
  return boost::none;
}

boost::optional<double> CalibrationUtilityBill::minutesInPeakDemandWindow() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("minutesInPeakDemandWindow");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<unsigned> CalibrationUtilityBill::numberBillingPeriodsInCalculations() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("numberBillingPeriodsInCalculations");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Unsigned);
    return attribute->valueAsUnsigned();
  }
  return boost::none;
}

boost::optional<double> CalibrationUtilityBill::CVRMSE() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("cvrmse");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> CalibrationUtilityBill::NMBE() const {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("nmbe");
  if (attribute) {
    OS_ASSERT(attribute->valueType() == AttributeValueType::Double);
    return attribute->valueAsDouble();
  }
  return boost::none;
}

std::vector<CalibrationBillingPeriod> CalibrationUtilityBill::billingPeriods() const {
  std::vector<CalibrationBillingPeriod> result;

  boost::optional<Attribute> attribute = m_attribute.findChildByName("billingPeriods");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::AttributeVector);
  for (const Attribute& value : attribute->valueAsAttributeVector()) {
    boost::optional<CalibrationBillingPeriod> tmp = CalibrationBillingPeriod::fromAttribute(value);
    if (tmp) {
      result.push_back(*tmp);
    }
  }

  return result;
}

bool CalibrationUtilityBill::addBillingPeriod(const CalibrationBillingPeriod& billingPeriod) {
  if (billingPeriod.consumptionUnit() != this->consumptionUnit()) {
    return false;
  }
  if (billingPeriod.peakDemandUnit() != this->peakDemandUnit()) {
    return false;
  }

  boost::optional<Attribute> attribute = m_attribute.findChildByName("billingPeriods");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::AttributeVector);
  std::vector<Attribute> values = attribute->valueAsAttributeVector();
  values.push_back(billingPeriod.attribute());
  attribute->setValue(values);

  return true;
}

std::string CalibrationResult::attributeName() {
  return "CalibrationResult";
}

boost::optional<CalibrationResult> CalibrationResult::fromAttribute(const Attribute& attribute) {
  if (attribute.name() != CalibrationResult::attributeName()) {
    LOG(Warn, "Cannot create CalibrationResult from Attribute because attribute name is '" << attribute.name() << "'.");
    return boost::none;
  }

  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG(Warn, "Cannot create CalibrationResult from Attribute because attribute is a " << attribute.valueType().valueDescription()
                                                                                       << ", not an AttributeVector.");
    return boost::none;
  }

  if (attribute.units()) {
    LOG(Warn, "Cannot create CalibrationResult from Attribute because attribute has units '" << attribute.units().get() << "'.");
    return boost::none;
  }

  // minimum required attributes are utilityBills
  boost::optional<Attribute> test = attribute.findChildByName("utilityBills");
  if (!test || !(test->valueType() == AttributeValueType::AttributeVector)) {
    LOG(Warn, "Cannot create CalibrationResult from Attribute without 'utilityBills'.");
    return boost::none;
  }

  return CalibrationResult(attribute);
}

CalibrationResult::CalibrationResult() : m_attribute(CalibrationResult::attributeName(), std::vector<Attribute>(), boost::none) {
  std::vector<Attribute> values;

  values.push_back(Attribute("utilityBills", std::vector<Attribute>()));

  m_attribute.setValue(values);
}

CalibrationResult::CalibrationResult(const Attribute& attribute) : m_attribute(attribute) {}

Attribute CalibrationResult::attribute() const {
  return m_attribute;
}

std::vector<CalibrationUtilityBill> CalibrationResult::utilityBills() const {
  std::vector<CalibrationUtilityBill> result;

  boost::optional<Attribute> attribute = m_attribute.findChildByName("utilityBills");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::AttributeVector);
  for (const Attribute& value : attribute->valueAsAttributeVector()) {
    boost::optional<CalibrationUtilityBill> tmp = CalibrationUtilityBill::fromAttribute(value);
    if (tmp) {
      result.push_back(*tmp);
    }
  }

  return result;
}

bool CalibrationResult::addUtilityBill(const CalibrationUtilityBill& utilityBill) {
  boost::optional<Attribute> attribute = m_attribute.findChildByName("utilityBills");
  OS_ASSERT(attribute);
  OS_ASSERT(attribute->valueType() == AttributeValueType::AttributeVector);
  std::vector<Attribute> values = attribute->valueAsAttributeVector();
  values.push_back(utilityBill.attribute());
  attribute->setValue(values);

  return true;
}

}  // namespace openstudio
