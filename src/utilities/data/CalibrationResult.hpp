/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_CALIBRATIONRESULT_HPP
#define UTILITIES_DATA_CALIBRATIONRESULT_HPP

#include "../UtilitiesAPI.hpp"
#include "Attribute.hpp"
#include "DataEnums.hpp"

namespace openstudio {

class Date;

/** CalibrationBillingPeriod is wrapper around Attribute for storing information about model calibration.
*/
class UTILITIES_API CalibrationBillingPeriod
{
 public:
  /// Returns the expected name for attributes of this type, 'CalibrationBillingPeriod'.
  static std::string attributeName();

  /// Creates a CalibrationBillingPeriod wrapper around attribute if it is correctly formatted.
  static boost::optional<CalibrationBillingPeriod> fromAttribute(const Attribute& attribute);

  /// Create attribute wrapper with all required information.
  CalibrationBillingPeriod(const Date& startDate, unsigned numberOfDays, const std::string& consumptionUnit,
                           boost::optional<std::string> peakDemandUnit, boost::optional<double> consumption, boost::optional<double> peakDemand,
                           boost::optional<double> totalCost, boost::optional<double> modelConsumption, boost::optional<double> modelPeakDemand,
                           boost::optional<double> modelTotalCost);

  /// Returns the attribute
  Attribute attribute() const;

  /** The date that the billing period starts. */
  Date startDate() const;

  /** The date that the billing period ends. */
  Date endDate() const;

  /** The duration of the billing period in days. */
  unsigned numberOfDays() const;

  std::string consumptionUnit() const;

  boost::optional<std::string> peakDemandUnit() const;

  /** Returns the consumption in billing units. */
  boost::optional<double> consumption() const;

  /** Returns the peak demand in billing units. */
  boost::optional<double> peakDemand() const;

  /** Returns the total cost of the bill in dollars. */
  boost::optional<double> totalCost() const;

  /** Returns the sum of modelConsumptionValues if it is not empty. */
  boost::optional<double> modelConsumption() const;

  /** Returns the maximum peak over the billing period if available. */
  boost::optional<double> modelPeakDemand() const;

  /** Returns total energy cost over the billing period if available. */
  boost::optional<double> modelTotalCost() const;

 private:
  CalibrationBillingPeriod(const Attribute& attribute);

  Attribute m_attribute;

  REGISTER_LOGGER("openstudio.CalibrationBillingPeriod");
};

/** CalibrationUtilityBill is wrapper around Attribute for storing information about model calibration.
*/
class UTILITIES_API CalibrationUtilityBill
{
 public:
  /// Returns the expected name for attributes of this type, 'CalibrationUtilityBill'.
  static std::string attributeName();

  /// Creates a CalibrationUtilityBill wrapper around attribute if it is correctly formatted.
  static boost::optional<CalibrationUtilityBill> fromAttribute(const Attribute& attribute);

  /// Create attribute wrapper with all required information.
  CalibrationUtilityBill(const std::string& name, const FuelType& fuelType, const InstallLocationType& meterInstallLocation,
                         boost::optional<std::string> meterSpecificInstallLocation, boost::optional<EndUseCategoryType> meterEndUseCategory,
                         boost::optional<std::string> meterSpecificEndUse, const std::string& consumptionUnit, double consumptionUnitConversionFactor,
                         boost::optional<std::string> peakDemandUnit, boost::optional<double> peakDemandUnitConversionFactor,
                         boost::optional<unsigned> timestepsInPeakDemandWindow, boost::optional<double> minutesInPeakDemandWindow,
                         boost::optional<unsigned> numberBillingPeriodsInCalculations, boost::optional<double> CVRMSE, boost::optional<double> NMBE);

  /// Returns the attribute
  Attribute attribute() const;

  std::string name() const;

  FuelType fuelType() const;

  InstallLocationType meterInstallLocation() const;

  boost::optional<std::string> meterSpecificInstallLocation() const;

  boost::optional<EndUseCategoryType> meterEndUseCategory() const;

  boost::optional<std::string> meterSpecificEndUse() const;

  std::string consumptionUnit() const;

  double consumptionUnitConversionFactor() const;

  boost::optional<std::string> peakDemandUnit() const;

  boost::optional<double> peakDemandUnitConversionFactor() const;

  boost::optional<unsigned> timestepsInPeakDemandWindow() const;

  boost::optional<double> minutesInPeakDemandWindow() const;

  boost::optional<unsigned> numberBillingPeriodsInCalculations() const;

  boost::optional<double> CVRMSE() const;

  boost::optional<double> NMBE() const;

  /// Returns all CalibrationBillingPeriod associated with this CalibrationUtilityBill.
  std::vector<CalibrationBillingPeriod> billingPeriods() const;

  /// Add a CalibrationBillingPeriod to this CalibrationUtilityBill.
  // cppcheck-suppress functionConst
  bool addBillingPeriod(const CalibrationBillingPeriod& billingPeriod);

 private:
  CalibrationUtilityBill(const Attribute& attribute);

  Attribute m_attribute;

  REGISTER_LOGGER("openstudio.CalibrationUtilityBill");
};

/** CalibrationResult is wrapper around Attribute for storing information about model calibration.
*/
class UTILITIES_API CalibrationResult
{
 public:
  /// Returns the expected name for attributes of this type, 'CalibrationResult'.
  static std::string attributeName();

  /// Creates a CalibrationResult wrapper around attribute if it is correctly formatted.
  static boost::optional<CalibrationResult> fromAttribute(const Attribute& attribute);

  /// Constructs a new calibration result attribute
  CalibrationResult();

  /// Returns the attribute
  Attribute attribute() const;

  /// get all utility bills
  std::vector<CalibrationUtilityBill> utilityBills() const;

  /// add a utility bill
  // cppcheck-suppress functionConst
  bool addUtilityBill(const CalibrationUtilityBill& utilityBill);

 private:
  CalibrationResult(const Attribute& attribute);

  Attribute m_attribute;

  REGISTER_LOGGER("openstudio.CalibrationResult");
};

}  // namespace openstudio

#endif  // UTILITIES_DATA_CALIBRATIONRESULT_HPP
