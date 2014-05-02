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

#ifndef MODEL_LIFECYCLECOSTPARAMETERS_IMPL_HPP
#define MODEL_LIFECYCLECOSTPARAMETERS_IMPL_HPP


#include <model/LifeCycleCostParameters.hpp>

#include <model/ParentObject_Impl.hpp>
#include <model/LifeCycleCostParameters.hpp>
#include <utilities/core/Optional.hpp>

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
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const {return LifeCycleCostParameters::iddObjectType();}

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
    virtual bool setParent(ParentObject& newParent);

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
