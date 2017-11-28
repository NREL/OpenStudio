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

#include "LifeCycleCostParameters.hpp"
#include "LifeCycleCostParameters_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_LifeCycleCost_Parameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/time/Date.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model  {

namespace detail {

  LifeCycleCostParameters_Impl::LifeCycleCostParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LifeCycleCostParameters::iddObjectType());
  }

  LifeCycleCostParameters_Impl::LifeCycleCostParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LifeCycleCostParameters::iddObjectType());
  }

  LifeCycleCostParameters_Impl::LifeCycleCostParameters_Impl(const LifeCycleCostParameters_Impl& other,Model_Impl* model,bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> LifeCycleCostParameters_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> LifeCycleCostParameters_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  std::vector<IddObjectType> LifeCycleCostParameters_Impl::allowableChildTypes() const {
    IddObjectTypeVector result;
    return result;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& LifeCycleCostParameters_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::string LifeCycleCostParameters_Impl::analysisType() const{
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::AnalysisType,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return *os;
  }

  bool LifeCycleCostParameters_Impl::isAnalysisTypeDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::AnalysisType);
  }

  bool LifeCycleCostParameters_Impl::isFEMPAnalysis() const{
    return ("FEMP" == this->analysisType());
  }

  std::string LifeCycleCostParameters_Impl::discountingConvention() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::DiscountingConvention,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return *os;
  }

  bool LifeCycleCostParameters_Impl::isDiscountingConventionDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::DiscountingConvention);
  }

  std::string LifeCycleCostParameters_Impl::inflationApproach() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::InflationApproach,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return *os;
  }

  bool LifeCycleCostParameters_Impl::isInflationApproachDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::InflationApproach);
  }

  bool LifeCycleCostParameters_Impl::isConstantDollarAnalysis() const{
    return ("ConstantDollar" == this->inflationApproach());
  }

  boost::optional<double> LifeCycleCostParameters_Impl::realDiscountRate() const {
    boost::optional<double> result;
    if (this->isConstantDollarAnalysis()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::RealDiscountRate,true);
      if (!result){
        // default is not in the IDD, act like it is here
        result = LifeCycleCostParameters::fempRealDiscountRate();
      }
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::nominalDiscountRate() const {
    boost::optional<double> result;
    if (!this->isConstantDollarAnalysis()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::NominalDiscountRate,true);
      if (!result){
        // default is not in the IDD, act like it is here
        result = LifeCycleCostParameters::fempNominalDiscountRate();
      }
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::inflation() const {
    boost::optional<double> result;
    if (!this->isConstantDollarAnalysis()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::Inflation,true);
      if (!result){
        // default is not in the IDD, act like it is here
        result = LifeCycleCostParameters::fempInflation();
      }
    }
    return result;
  }

  MonthOfYear LifeCycleCostParameters_Impl::baseDateMonth() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::BaseDateMonth,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return MonthOfYear(*os);
  }

  bool LifeCycleCostParameters_Impl::isBaseDateMonthDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::BaseDateMonth);
  }

  int LifeCycleCostParameters_Impl::baseDateYear() const {
    boost::optional<int> result = getInt(OS_LifeCycleCost_ParametersFields::BaseDateYear,true);
    if (!result){
      result = LifeCycleCostParameters::nistYear();
    }
    return *result;
  }

  bool LifeCycleCostParameters_Impl::isBaseDateYearDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::BaseDateYear);
  }

  MonthOfYear LifeCycleCostParameters_Impl::serviceDateMonth() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::ServiceDateMonth,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return MonthOfYear(*os);
  }

  bool LifeCycleCostParameters_Impl::isServiceDateMonthDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::BaseDateMonth);
  }

  int LifeCycleCostParameters_Impl::serviceDateYear() const {
    boost::optional<int> result = getInt(OS_LifeCycleCost_ParametersFields::ServiceDateYear,true);
    if (!result){
      result = LifeCycleCostParameters::nistYear();
    }
    return *result;
  }

  bool LifeCycleCostParameters_Impl::isServiceDateYearDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::ServiceDateYear);
  }

  int LifeCycleCostParameters_Impl::lengthOfStudyPeriodInYears() const {
    OptionalInt result = getInt(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears,true);
    OS_ASSERT(result);
    return *result;
  }

  bool LifeCycleCostParameters_Impl::isLengthOfStudyPeriodInYearsDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears);
  }

  boost::optional<double> LifeCycleCostParameters_Impl::taxRate() const {
    return getDouble(OS_LifeCycleCost_ParametersFields::TaxRate,true);
  }

  std::string LifeCycleCostParameters_Impl::depreciationMethod() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::DepreciationMethod,true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return *os;
  }

  bool LifeCycleCostParameters_Impl::isDepreciationMethodDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::DepreciationMethod);
  }

  bool LifeCycleCostParameters_Impl::useNISTFuelEscalationRates() const {
    OptionalString os = getString(OS_LifeCycleCost_ParametersFields::UseNISTFuelEscalationRates, true);
    OS_ASSERT(os); OS_ASSERT(!os->empty());
    return ("Yes" == *os);
  }

  bool LifeCycleCostParameters_Impl::isUseNISTFuelEscalationRatesDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::UseNISTFuelEscalationRates);
  }

  boost::optional<std::string> LifeCycleCostParameters_Impl::nistRegion() const
  {
    boost::optional<std::string> result;
    if (this->useNISTFuelEscalationRates()){
      result = getString(OS_LifeCycleCost_ParametersFields::NISTRegion, true);
    }
    return result;
  }

  std::vector<std::string> LifeCycleCostParameters_Impl::validNistRegionValues() const
  {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_LifeCycleCost_ParametersFields::NISTRegion);
  }

  bool LifeCycleCostParameters_Impl::isNISTRegionDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::NISTRegion);
  }

  boost::optional<std::string> LifeCycleCostParameters_Impl::nistSector() const
  {
    boost::optional<std::string> result;
    if (this->useNISTFuelEscalationRates()){
      result = getString(OS_LifeCycleCost_ParametersFields::NISTSector, true);
    }
    return result;
  }

  std::vector<std::string> LifeCycleCostParameters_Impl::validNistSectorValues() const
  {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_LifeCycleCost_ParametersFields::NISTSector);
  }

  bool LifeCycleCostParameters_Impl::isNISTSectorDefaulted() const{
    return isEmpty(OS_LifeCycleCost_ParametersFields::NISTSector);
  }

  boost::optional<double> LifeCycleCostParameters_Impl::electricityInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::ElectricityInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::naturalGasInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::NaturalGasInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::steamInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::SteamInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::gasolineInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::GasolineInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::dieselInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::DieselInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::coalInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::CoalInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::fuelOil1Inflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::FuelOil1Inflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::fuelOil2Inflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::FuelOil2Inflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::propaneInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::PropaneInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  boost::optional<double> LifeCycleCostParameters_Impl::waterInflation() const
  {
    boost::optional<double> result;
    if (!this->useNISTFuelEscalationRates()){
      result = getDouble(OS_LifeCycleCost_ParametersFields::WaterInflation, true);

      // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length if no result?
    }
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool LifeCycleCostParameters_Impl::setParent(ParentObject& newParent) {
    return false;
  }

  bool LifeCycleCostParameters_Impl::setAnalysisType(const std::string& analysisType)
  {
    bool changed = (this->analysisType() != analysisType);

    bool result = setString(OS_LifeCycleCost_ParametersFields::AnalysisType, analysisType, false);
    if (result && changed){
      if (isFEMPAnalysis()){
        if (isConstantDollarAnalysis()){
          // DLM: this call has logic that prevents it from working if isFEMPAnalysis
          // DLM: don't emit change signals until end
          //setRealDiscountRate(LifeCycleCostParameters::fempRealDiscountRate());
          setDouble(OS_LifeCycleCost_ParametersFields::RealDiscountRate,LifeCycleCostParameters::fempRealDiscountRate(), false);
          setString(OS_LifeCycleCost_ParametersFields::NominalDiscountRate, "", false);
          setString(OS_LifeCycleCost_ParametersFields::Inflation, "", false);
        }else{
          // DLM: this call has logic that prevents it from working if isFEMPAnalysis
          // DLM: don't emit change signals until end
          //setNominalDiscountRate(LifeCycleCostParameters::fempNominalDiscountRate());
          //setInflation(LifeCycleCostParameters::fempInflation());
          setString(OS_LifeCycleCost_ParametersFields::RealDiscountRate, "", false);
          setDouble(OS_LifeCycleCost_ParametersFields::NominalDiscountRate, LifeCycleCostParameters::fempNominalDiscountRate(), false);
          setDouble(OS_LifeCycleCost_ParametersFields::Inflation, LifeCycleCostParameters::fempInflation(), false);
        }

        if (lengthOfStudyPeriodInYears() > 25){
          setLengthOfStudyPeriodInYears(25);
        }
      }
    }

    emitChangeSignals();

    return result;
  }

  void LifeCycleCostParameters_Impl::resetAnalysisType()
  {
    bool test = setAnalysisType("FEMP");
    OS_ASSERT(test);
    test = setString(OS_LifeCycleCost_ParametersFields::AnalysisType, "");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setDiscountingConvention(const std::string& discountingConvention)
  {
    return setString(OS_LifeCycleCost_ParametersFields::DiscountingConvention,discountingConvention);
  }

  void LifeCycleCostParameters_Impl::resetDiscountingConvention()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::DiscountingConvention,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setInflationApproach(const std::string& inflationApproach)
  {
    bool changed = (this->inflationApproach() != inflationApproach);

    bool result = setString(OS_LifeCycleCost_ParametersFields::InflationApproach, inflationApproach);
    if (result && changed){
      if (isConstantDollarAnalysis()){
        setRealDiscountRate(LifeCycleCostParameters::fempRealDiscountRate());
        setString(OS_LifeCycleCost_ParametersFields::NominalDiscountRate, "");
        setString(OS_LifeCycleCost_ParametersFields::Inflation, "");
      }else{
        setString(OS_LifeCycleCost_ParametersFields::RealDiscountRate, "");
        setNominalDiscountRate(LifeCycleCostParameters::fempNominalDiscountRate());
        setInflation(LifeCycleCostParameters::fempInflation());
      }
    }

    return result;
  }

  void LifeCycleCostParameters_Impl::resetInflationApproach()
  {
    bool test = setAnalysisType("ConstantDollar");
    OS_ASSERT(test);
    test = setString(OS_LifeCycleCost_ParametersFields::AnalysisType, "");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setRealDiscountRate(double realDiscountRate)
  {
    bool result = false;
    if (isConstantDollarAnalysis()){
      if (!isFEMPAnalysis()){
        result = setDouble(OS_LifeCycleCost_ParametersFields::RealDiscountRate,realDiscountRate);
      }
    }
    return result;
  }

  bool LifeCycleCostParameters_Impl::setNominalDiscountRate(double nominalDiscountRate)
  {
    bool result = false;
    if (!isConstantDollarAnalysis()){
      if (!isFEMPAnalysis()){
        result = setDouble(OS_LifeCycleCost_ParametersFields::NominalDiscountRate,nominalDiscountRate);
      }
    }
    return result;
  }

  bool LifeCycleCostParameters_Impl::setInflation(double inflation)
  {
    bool result = false;
    if (!isConstantDollarAnalysis()){
      if (!isFEMPAnalysis()){
        result = setDouble(OS_LifeCycleCost_ParametersFields::Inflation,inflation);
      }
    }
    return result;
  }

  bool LifeCycleCostParameters_Impl::setBaseDateMonth(const MonthOfYear& baseDateMonth)
  {
    return setString(OS_LifeCycleCost_ParametersFields::BaseDateMonth,baseDateMonth.valueDescription());
  }

  void LifeCycleCostParameters_Impl::resetBaseDateMonth()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::BaseDateMonth,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setBaseDateYear(int baseDateYear)
  {
    return setInt(OS_LifeCycleCost_ParametersFields::BaseDateYear,baseDateYear);
  }

  void LifeCycleCostParameters_Impl::resetBaseDateYear()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::BaseDateYear,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setServiceDateMonth(const MonthOfYear& serviceDateMonth)
  {
    return setString(OS_LifeCycleCost_ParametersFields::ServiceDateMonth,serviceDateMonth.valueDescription());
  }

  void LifeCycleCostParameters_Impl::resetServiceDateMonth()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::ServiceDateMonth,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setServiceDateYear(int serviceDateYear)
  {
    return setInt(OS_LifeCycleCost_ParametersFields::ServiceDateYear,serviceDateYear);
  }

  void LifeCycleCostParameters_Impl::resetServiceDateYear()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::ServiceDateYear,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setLengthOfStudyPeriodInYears(int lengthOfStudyPeriodInYears)
  {
    bool result = false;
    if (isFEMPAnalysis()){
      if (lengthOfStudyPeriodInYears <= 25){
        result = setInt(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears,lengthOfStudyPeriodInYears);
      }
    }else if (useNISTFuelEscalationRates()){
      if (lengthOfStudyPeriodInYears <= 30){
        result = setInt(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears,lengthOfStudyPeriodInYears);
      }
    }else{
      result = setInt(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears,lengthOfStudyPeriodInYears);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetLengthOfStudyPeriodInYears()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setTaxRate(double TaxRate)
  {
    return setDouble(OS_LifeCycleCost_ParametersFields::TaxRate,TaxRate);
  }

  void LifeCycleCostParameters_Impl::resetTaxRate()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::TaxRate,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setDepreciationMethod(const std::string& depreciationMethod)
  {
    return setString(OS_LifeCycleCost_ParametersFields::DepreciationMethod,depreciationMethod);
  }

  void LifeCycleCostParameters_Impl::resetDepreciationMethod()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::DepreciationMethod,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setUseNISTFuelEscalationRates(bool useNISTFuelEscalationRates)
  {
    bool changed = (useNISTFuelEscalationRates != this->useNISTFuelEscalationRates());

    bool result = false;
    if (useNISTFuelEscalationRates){
      result = setString(OS_LifeCycleCost_ParametersFields::UseNISTFuelEscalationRates,"Yes");
      OS_ASSERT(result);

      if (lengthOfStudyPeriodInYears() > 30){
        setLengthOfStudyPeriodInYears(30);
      }
    }else{
      result = setString(OS_LifeCycleCost_ParametersFields::UseNISTFuelEscalationRates,"No");
      OS_ASSERT(result);
    }

    // DLM: provide "equivalent" FEMP fuel escalation rates based on nist data and analysis length?

    if (changed){
      result = setString(OS_LifeCycleCost_ParametersFields::NISTRegion,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::NISTSector,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::ElectricityInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::NaturalGasInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::SteamInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::GasolineInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::DieselInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::CoalInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::FuelOil1Inflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::FuelOil2Inflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::PropaneInflation,"");
      OS_ASSERT(result);
      result = setString(OS_LifeCycleCost_ParametersFields::WaterInflation,"");
      OS_ASSERT(result);
    }

    return result;
  }

  void LifeCycleCostParameters_Impl::resetUseNISTFuelEscalationRates()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::UseNISTFuelEscalationRates,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setNISTRegion(const std::string& nistRegion)
  {
    bool result = false;
    if (useNISTFuelEscalationRates()){
      result = setString(OS_LifeCycleCost_ParametersFields::NISTRegion,nistRegion);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetNISTRegion()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::NISTRegion,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setNISTSector(const std::string& nistSector)
  {
    bool result = false;
    if (useNISTFuelEscalationRates()){
      result = setString(OS_LifeCycleCost_ParametersFields::NISTSector,nistSector);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetNISTSector()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::NISTSector,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setElectricityInflation(double electricityInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::ElectricityInflation,electricityInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetElectricityInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::ElectricityInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setNaturalGasInflation(double naturalGasInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::NaturalGasInflation,naturalGasInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetNaturalGasInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::NaturalGasInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setSteamInflation(double steamInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::SteamInflation,steamInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetSteamInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::SteamInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setGasolineInflation(double gasolineInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::GasolineInflation,gasolineInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetGasolineInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::GasolineInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setDieselInflation(double dieselInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::DieselInflation,dieselInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetDieselInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::DieselInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setCoalInflation(double coalInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::CoalInflation,coalInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetCoalInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::CoalInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setFuelOil1Inflation(double fuelOil1Inflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::FuelOil1Inflation,fuelOil1Inflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetFuelOil1Inflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::FuelOil1Inflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setFuelOil2Inflation(double fuelOil2Inflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::FuelOil2Inflation,fuelOil2Inflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetFuelOil2Inflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::FuelOil2Inflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setPropaneInflation(double propaneInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::PropaneInflation,propaneInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetPropaneInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::PropaneInflation,"");
    OS_ASSERT(test);
  }

  bool LifeCycleCostParameters_Impl::setWaterInflation(double waterInflation)
  {
    bool result = false;
    if (!useNISTFuelEscalationRates()){
      result = setDouble(OS_LifeCycleCost_ParametersFields::WaterInflation,waterInflation);
    }
    return result;
  }

  void LifeCycleCostParameters_Impl::resetWaterInflation()
  {
    bool test = setString(OS_LifeCycleCost_ParametersFields::WaterInflation,"");
    OS_ASSERT(test);
  }

} // detail

/// constructor
LifeCycleCostParameters::LifeCycleCostParameters(const Model& model)
  : ParentObject(LifeCycleCostParameters::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LifeCycleCostParameters_Impl>());
}

// constructor
LifeCycleCostParameters::LifeCycleCostParameters(std::shared_ptr<detail::LifeCycleCostParameters_Impl> impl)
  : ParentObject(std::move(impl))
{}

int LifeCycleCostParameters::nistYear()
{
  return 2011;
}

double LifeCycleCostParameters::fempRealDiscountRate()
{
  return 0.03;
}

double LifeCycleCostParameters::fempNominalDiscountRate()
{
  return 0.035;
}

double LifeCycleCostParameters::fempInflation()
{
  return 0.005;
}

std::string LifeCycleCostParameters::analysisType() const{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->analysisType();
}

bool LifeCycleCostParameters::isAnalysisTypeDefaulted() const{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isAnalysisTypeDefaulted();
}

bool LifeCycleCostParameters::isFEMPAnalysis() const{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isFEMPAnalysis();
}

std::string LifeCycleCostParameters::discountingConvention() const{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->discountingConvention();
}

bool LifeCycleCostParameters::isDiscountingConventionDefaulted() const{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isDiscountingConventionDefaulted();
}

std::string LifeCycleCostParameters::inflationApproach() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->inflationApproach();
}

bool LifeCycleCostParameters::isInflationApproachDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isInflationApproachDefaulted();
}

bool LifeCycleCostParameters::isConstantDollarAnalysis() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isConstantDollarAnalysis();
}

boost::optional<double> LifeCycleCostParameters::realDiscountRate() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->realDiscountRate();
}

boost::optional<double> LifeCycleCostParameters::nominalDiscountRate() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->nominalDiscountRate();
}

boost::optional<double> LifeCycleCostParameters::inflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->inflation();
}

MonthOfYear LifeCycleCostParameters::baseDateMonth() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->baseDateMonth();
}

bool LifeCycleCostParameters::isBaseDateMonthDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isBaseDateMonthDefaulted();
}

int LifeCycleCostParameters::baseDateYear() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->baseDateYear();
}

bool LifeCycleCostParameters::isBaseDateYearDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isBaseDateYearDefaulted();
}

MonthOfYear LifeCycleCostParameters::serviceDateMonth() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->serviceDateMonth();
}

bool LifeCycleCostParameters::isServiceDateMonthDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isServiceDateMonthDefaulted();
}

int LifeCycleCostParameters::serviceDateYear() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->serviceDateYear();
}

bool LifeCycleCostParameters::isServiceDateYearDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isServiceDateYearDefaulted();
}

int LifeCycleCostParameters::lengthOfStudyPeriodInYears() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->lengthOfStudyPeriodInYears();
}

bool LifeCycleCostParameters::isLengthOfStudyPeriodInYearsDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isLengthOfStudyPeriodInYearsDefaulted();
}

boost::optional<double> LifeCycleCostParameters::taxRate() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->taxRate();
}

std::string LifeCycleCostParameters::depreciationMethod() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->depreciationMethod();
}

bool LifeCycleCostParameters::isDepreciationMethodDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isDepreciationMethodDefaulted();
}

bool LifeCycleCostParameters::useNISTFuelEscalationRates() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->useNISTFuelEscalationRates();
}

bool LifeCycleCostParameters::isUseNISTFuelEscalationRatesDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isUseNISTFuelEscalationRatesDefaulted();
}

boost::optional<std::string> LifeCycleCostParameters::nistRegion() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->nistRegion();
}

std::vector<std::string> LifeCycleCostParameters::validNistRegionValues() const
{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->validNistRegionValues();
}

bool LifeCycleCostParameters::isNISTRegionDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isNISTRegionDefaulted();
}

boost::optional<std::string> LifeCycleCostParameters::nistSector() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->nistSector();
}

std::vector<std::string> LifeCycleCostParameters::validNistSectorValues() const
{
  return getImpl<detail::LifeCycleCostParameters_Impl>()->validNistSectorValues();
}

bool LifeCycleCostParameters::isNISTSectorDefaulted() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->isNISTSectorDefaulted();
}

boost::optional<double> LifeCycleCostParameters::electricityInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->electricityInflation();
}

boost::optional<double> LifeCycleCostParameters::naturalGasInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->naturalGasInflation();
}

boost::optional<double> LifeCycleCostParameters::steamInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->steamInflation();
}

boost::optional<double> LifeCycleCostParameters::gasolineInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->gasolineInflation();
}

boost::optional<double> LifeCycleCostParameters::dieselInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->dieselInflation();
}

boost::optional<double> LifeCycleCostParameters::coalInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->coalInflation();
}

boost::optional<double> LifeCycleCostParameters::fuelOil1Inflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->fuelOil1Inflation();
}

boost::optional<double> LifeCycleCostParameters::fuelOil2Inflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->fuelOil2Inflation();
}

boost::optional<double> LifeCycleCostParameters::propaneInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->propaneInflation();
}

boost::optional<double> LifeCycleCostParameters::waterInflation() const {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->waterInflation();
}

bool LifeCycleCostParameters::setAnalysisType(const std::string& analysisType){
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setAnalysisType(analysisType);
}

void LifeCycleCostParameters::resetAnalysisType(){
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetAnalysisType();
}

bool LifeCycleCostParameters::setDiscountingConvention(const std::string& discountingConvention){
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setDiscountingConvention(discountingConvention);
}

void LifeCycleCostParameters::resetDiscountingConvention(){
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetDiscountingConvention();
}

bool LifeCycleCostParameters::setInflationApproach(const std::string& inflationApproach) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setInflationApproach(inflationApproach);
}

void LifeCycleCostParameters::resetInflationApproach() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetInflationApproach();
}

bool LifeCycleCostParameters::setRealDiscountRate(double realDiscountRate) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setRealDiscountRate(realDiscountRate);
}

bool LifeCycleCostParameters::setNominalDiscountRate(double nominalDiscountRate) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setNominalDiscountRate(nominalDiscountRate);
}

bool LifeCycleCostParameters::setInflation(double inflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setInflation(inflation);
}

bool LifeCycleCostParameters::setBaseDateMonth(const MonthOfYear& baseDateMonth) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setBaseDateMonth(baseDateMonth);
}

void LifeCycleCostParameters::resetBaseDateMonth() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetBaseDateMonth();
}

bool LifeCycleCostParameters::setBaseDateYear(int baseDateYear) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setBaseDateYear(baseDateYear);
}

void LifeCycleCostParameters::resetBaseDateYear() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetBaseDateYear();
}

bool LifeCycleCostParameters::setServiceDateMonth(const MonthOfYear& serviceDateMonth) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setServiceDateMonth(serviceDateMonth);
}

void LifeCycleCostParameters::resetServiceDateMonth() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetServiceDateMonth();
}

bool LifeCycleCostParameters::setServiceDateYear(int serviceDateYear) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setServiceDateYear(serviceDateYear);
}

void LifeCycleCostParameters::resetServiceDateYear() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetServiceDateYear();
}

bool LifeCycleCostParameters::setLengthOfStudyPeriodInYears(int lengthOfStudyPeriodInYears) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setLengthOfStudyPeriodInYears(lengthOfStudyPeriodInYears);
}

void LifeCycleCostParameters::resetLengthOfStudyPeriodInYears() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetLengthOfStudyPeriodInYears();
}

bool LifeCycleCostParameters::setTaxRate(double taxRate) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setTaxRate(taxRate);
}

void LifeCycleCostParameters::resetTaxRate() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetTaxRate();
}

bool LifeCycleCostParameters::setDepreciationMethod(const std::string& depreciationMethod) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setDepreciationMethod(depreciationMethod);
}

void LifeCycleCostParameters::resetDepreciationMethod() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetDepreciationMethod();
}

bool LifeCycleCostParameters::setUseNISTFuelEscalationRates(bool useNISTFuelEscalationRates) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setUseNISTFuelEscalationRates(useNISTFuelEscalationRates);
}

void LifeCycleCostParameters::resetUseNISTFuelEscalationRates() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetUseNISTFuelEscalationRates();
}

bool LifeCycleCostParameters::setNISTRegion(const std::string& nistRegion) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setNISTRegion(nistRegion);
}

void LifeCycleCostParameters::resetNISTRegion() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetNISTRegion();
}

bool LifeCycleCostParameters::setNISTSector(const std::string& nistSector) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setNISTSector(nistSector);
}

void LifeCycleCostParameters::resetNISTSector() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetNISTSector();
}

bool LifeCycleCostParameters::setElectricityInflation(double electricityInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setElectricityInflation(electricityInflation);
}

void LifeCycleCostParameters::resetElectricityInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetElectricityInflation();
}

bool LifeCycleCostParameters::setNaturalGasInflation(double naturalGasInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setNaturalGasInflation(naturalGasInflation);
}

void LifeCycleCostParameters::resetNaturalGasInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetNaturalGasInflation();
}

bool LifeCycleCostParameters::setSteamInflation(double steamInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setSteamInflation(steamInflation);
}

void LifeCycleCostParameters::resetSteamInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetSteamInflation();
}

bool LifeCycleCostParameters::setGasolineInflation(double gasolineInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setGasolineInflation(gasolineInflation);
}

void LifeCycleCostParameters::resetGasolineInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetGasolineInflation();
}

bool LifeCycleCostParameters::setDieselInflation(double dieselInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setDieselInflation(dieselInflation);
}

void LifeCycleCostParameters::resetDieselInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetDieselInflation();
}

bool LifeCycleCostParameters::setCoalInflation(double coalInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setCoalInflation(coalInflation);
}

void LifeCycleCostParameters::resetCoalInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetCoalInflation();
}

bool LifeCycleCostParameters::setFuelOil1Inflation(double fuelOil1Inflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setFuelOil1Inflation(fuelOil1Inflation);
}

void LifeCycleCostParameters::resetFuelOil1Inflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetFuelOil1Inflation();
}

bool LifeCycleCostParameters::setFuelOil2Inflation(double fuelOil2Inflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setFuelOil2Inflation(fuelOil2Inflation);
}

void LifeCycleCostParameters::resetFuelOil2Inflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetFuelOil2Inflation();
}

bool LifeCycleCostParameters::setPropaneInflation(double propaneInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setPropaneInflation(propaneInflation);
}

void LifeCycleCostParameters::resetPropaneInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetPropaneInflation();
}

bool LifeCycleCostParameters::setWaterInflation(double waterInflation) {
  return getImpl<detail::LifeCycleCostParameters_Impl>()->setWaterInflation(waterInflation);
}

void LifeCycleCostParameters::resetWaterInflation() {
  getImpl<detail::LifeCycleCostParameters_Impl>()->resetWaterInflation();
}

std::vector<std::string> LifeCycleCostParameters::validAnalysisTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_LifeCycleCost_ParametersFields::AnalysisType);
}

std::vector<std::string> LifeCycleCostParameters::validDiscountingConventionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_LifeCycleCost_ParametersFields::DiscountingConvention);
}

std::vector<std::string> LifeCycleCostParameters::validInflationApproachValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_LifeCycleCost_ParametersFields::InflationApproach);
}

std::vector<std::string> LifeCycleCostParameters::validDepreciationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_LifeCycleCost_ParametersFields::DepreciationMethod);
}

IddObjectType LifeCycleCostParameters::iddObjectType()
{
  IddObjectType result(IddObjectType::OS_LifeCycleCost_Parameters);
  return result;
}

} // model
} // openstudio
