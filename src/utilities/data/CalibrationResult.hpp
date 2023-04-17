/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
                         boost::optional<std::string> meterSpecificEndUse, std::string consumptionUnit, double consumptionUnitConversionFactor,
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
