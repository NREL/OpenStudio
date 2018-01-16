/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_LIFECYCLECOSTPARAMETERS_IMPL_HPP
#define MODEL_LIFECYCLECOSTPARAMETERS_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "LifeCycleCostParameters.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {

  class MonthOfYear;

namespace model {

namespace detail {

  class MODEL_API LifeCycleCostParameters_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    LifeCycleCostParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    LifeCycleCostParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

    // clone copy constructor
    LifeCycleCostParameters_Impl(const LifeCycleCostParameters_Impl& other,Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~LifeCycleCostParameters_Impl(){}

    //@}
    /** @name Getters */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const override;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const override;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override {return LifeCycleCostParameters::iddObjectType();}

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
    //@{

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent) override;

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

    /// This cannot be set greater than 25 years when analysis type is FEMP
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
   private:
    REGISTER_LOGGER("openstudio.model.LifeCycleCostParameters");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LIFECYCLECOSTPARAMETERS_IMPL_HPP
