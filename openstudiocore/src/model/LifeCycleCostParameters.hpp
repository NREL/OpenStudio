/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_LIFECYCLECOSTPARAMETERS_HPP
#define MODEL_LIFECYCLECOSTPARAMETERS_HPP

#include <model/ModelAPI.hpp>
#include <model/ParentObject.hpp>
#include <utilities/core/Optional.hpp>

namespace openstudio{

  class MonthOfYear;

namespace model{

namespace detail{
  class LifeCycleCostParameters_Impl;
}

/** LifeCycleCostParameters derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost:Parameters".
 *
 *  LifeCycleCostParameters is a unique object which defines important parameters for life cycle analysis such as the length of the study,
 *  inflation rates, discounting convention, etc.
 *   
 */
class MODEL_API LifeCycleCostParameters : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{
  virtual ~LifeCycleCostParameters() {}

  //@}
  /** @name Getters */
  //@{

  static int nistYear();
  static double fempRealDiscountRate();
  static double fempNominalDiscountRate();
  static double fempInflation();

  std::string analysisType() const;

  bool isAnalysisTypeDefaulted() const;

  /// returns true if analysis type is FEMP
  bool isFEMPAnalysis() const;

  std::string discountingConvention() const;

  bool isDiscountingConventionDefaulted() const;

  std::string inflationApproach() const;

  bool isInflationApproachDefaulted() const;

  bool isConstantDollarAnalysis() const;

  boost::optional<double> realDiscountRate() const;
  
  boost::optional<double> nominalDiscountRate() const;

  boost::optional<double> inflation() const;

  MonthOfYear baseDateMonth() const;

  bool isBaseDateMonthDefaulted() const;

  int baseDateYear() const;

  bool isBaseDateYearDefaulted() const;

  MonthOfYear serviceDateMonth() const;

  bool isServiceDateMonthDefaulted() const;

  int serviceDateYear() const;

  bool isServiceDateYearDefaulted() const;

  int lengthOfStudyPeriodInYears() const;

  bool isLengthOfStudyPeriodInYearsDefaulted() const;

  boost::optional<double> taxRate() const;

  std::string depreciationMethod() const;

  bool isDepreciationMethodDefaulted() const;

  bool useNISTFuelEscalationRates() const;

  bool isUseNISTFuelEscalationRatesDefaulted() const;

  boost::optional<std::string> nistRegion() const;

  std::vector<std::string> validNistRegionValues() const;

  bool isNISTRegionDefaulted() const;

  boost::optional<std::string> nistSector() const;

  std::vector<std::string> validNistSectorValues() const;

  bool isNISTSectorDefaulted() const;

  boost::optional<double> electricityInflation() const;

  boost::optional<double> naturalGasInflation() const;

  boost::optional<double> steamInflation() const;

  boost::optional<double> gasolineInflation() const;

  boost::optional<double> dieselInflation() const;

  boost::optional<double> coalInflation() const;

  boost::optional<double> fuelOil1Inflation() const;

  boost::optional<double> fuelOil2Inflation() const;

  boost::optional<double> propaneInflation() const;

  boost::optional<double> waterInflation() const;

  //@}
  /** @name Setters */

  bool setAnalysisType(const std::string& analysisType);

  void resetAnalysisType();

  bool setDiscountingConvention(const std::string& discountingConvention);

  void resetDiscountingConvention();

  bool setInflationApproach(const std::string& inflationApproach);

  void resetInflationApproach();

  /// This cannot be set when analysis type is FEMP or when inflation approach is "CurrentDollar"
  bool setRealDiscountRate(double realDiscountRate);

  /// This cannot be set when analysis type is FEMP or when inflation approach is "ConstantDollar"
  bool setNominalDiscountRate(double nominalDiscountRate);

  /// This cannot be set when analysis type is FEMP or when inflation approach is "ConstantDollar"
  bool setInflation(double inflation);
  
  bool setBaseDateMonth(const MonthOfYear& baseDateMonth);

  void resetBaseDateMonth();

  bool setBaseDateYear(int baseDateYear);

  void resetBaseDateYear();

  bool setServiceDateMonth(const MonthOfYear& serviceDateMonth);

  void resetServiceDateMonth();

  bool setServiceDateYear(int serviceDateYear);

  void resetServiceDateYear();

  /// This cannot be set greater than 25 years when analysis type is FEMP.
  /// This cannot be set greater than 30 years when using NIST fuel escalation rates.
  bool setLengthOfStudyPeriodInYears(int lengthOfStudyPeriodInYears);

  void resetLengthOfStudyPeriodInYears();

  bool setTaxRate(double taxRate);

  void resetTaxRate();

  bool setDepreciationMethod(const std::string& depreciationMethod);

  void resetDepreciationMethod();

  /// This cannot be set when analysis type is FEMP
  bool setUseNISTFuelEscalationRates(bool useNISTFuelEscalationRates);

  void resetUseNISTFuelEscalationRates();

  /// This cannot be set when use NIST fuel escalation rates is false
  bool setNISTRegion(const std::string& nistRegion);

  void resetNISTRegion();

  /// This cannot be set when use NIST fuel escalation rates is false
  bool setNISTSector(const std::string& nistSector);

  void resetNISTSector();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setElectricityInflation(double electricityInflation);

  void resetElectricityInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setNaturalGasInflation(double naturalGasInflation);

  void resetNaturalGasInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setSteamInflation(double steamInflation);

  void resetSteamInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setGasolineInflation(double gasolineInflation);

  void resetGasolineInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setDieselInflation(double dieselInflation);

  void resetDieselInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setCoalInflation(double coalInflation);

  void resetCoalInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setFuelOil1Inflation(double fuelOil1Inflation);

  void resetFuelOil1Inflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setFuelOil2Inflation(double fuelOil2Inflation);

  void resetFuelOil2Inflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setPropaneInflation(double propaneInflation);

  void resetPropaneInflation();

  /// This cannot be set when use NIST fuel escalation rates is true
  bool setWaterInflation(double waterInflation);

  void resetWaterInflation();

  //@}  

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

  /// Returns allowable values for analysisType.
  static std::vector<std::string> validAnalysisTypeValues();

  /// Returns allowable values for discountingConvention.
  static std::vector<std::string> validDiscountingConventionValues();

  /// Returns allowable values for inflationApproach.
  static std::vector<std::string> validInflationApproachValues();

  /// Returns allowable values for depreciationMethod.
  static std::vector<std::string> validDepreciationMethodValues();

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new LifeCycleCostParameters object in the model.
  explicit LifeCycleCostParameters(const Model& model);

  //@}

  /// @cond

  typedef detail::LifeCycleCostParameters_Impl ImplType;

  friend class Model;
  friend class IdfObject;

  // constructor
  explicit LifeCycleCostParameters(boost::shared_ptr<detail::LifeCycleCostParameters_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.LifeCycleCostParameters");

  /// @endcond
};

/** \relates LifeCycleCostParameters */
typedef boost::optional<LifeCycleCostParameters> OptionalLifeCycleCostParameters;

/** \relates LifeCycleCostParameters */
typedef std::vector<LifeCycleCostParameters> LifeCycleCostParametersVector;

} // model
} // openstudio

#endif // MODEL_LIFECYCLECOSTPARAMETERS_HPP
