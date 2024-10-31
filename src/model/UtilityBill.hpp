/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_UTILITYBILL_HPP
#define MODEL_UTILITYBILL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/data/Vector.hpp"

namespace openstudio {

class FuelType;
class InstallLocationType;
class EndUseCategoryType;
class Date;

namespace model {

  class OutputMeter;

  namespace detail {

    class UtilityBill_Impl;

  }  // namespace detail

  /** BillingPeriod is a ModelExtensibleGroup that represents a single billing period in the
    UtilityBill ModelObject. */
  class MODEL_API BillingPeriod : public ModelExtensibleGroup
  {
   public:
    /** @name Getters */
    //@{

    /** The date that the billing period starts. */
    Date startDate() const;

    /** The date that the billing period ends. */
    Date endDate() const;

    /** The duration of the billing period in days. */
    unsigned numberOfDays() const;

    /** Returns the consumption in billing units. */
    boost::optional<double> consumption() const;

    /** Returns the peak demand in billing units. */
    boost::optional<double> peakDemand() const;

    /** Returns the total cost of the bill in dollars. */
    boost::optional<double> totalCost() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the start date.  If startDate is before endDate then endDate is retained.
      If startDate is after endDate then numberOfDays is retained. */
    bool setStartDate(const Date& startDate);

    /** Sets the end date.  If endDate is after startDate then startDate is retained.
      If endDate is before startDate then numberOfDays is retained. */
    bool setEndDate(const Date& endDate);

    /** Sets the number of days in billing period, startDate is always retained. */
    bool setNumberOfDays(unsigned numberOfDays);

    /** Sets the consumption in billing units. */
    bool setConsumption(double consumption);

    void resetConsumption();

    /** Sets the peak demand in billing units. */
    bool setPeakDemand(double peakDemand);

    void resetPeakDemand();

    /** Sets the total cost of the bill in dollars. */
    bool setTotalCost(double totalCost);

    void resetTotalCost();

    //@}
    /** @name Other */
    //@{

    /** Returns true if this billing period is strictly within the model's run period.*/
    bool withinRunPeriod() const;

    /** Returns true if this billing period is within the model's run period assuming periodic boundary conditions.*/
    bool withinPeriodicRunPeriod() const;

    /** Returns true if this billing period is partially within the model's run period.*/
    bool overlapsRunPeriod() const;

    /** Returns the consumption for each day in billing period from simulation in model units. */
    Vector modelConsumptionValues() const;

    /** Returns the sum of modelConsumptionValues if it is not empty. */
    boost::optional<double> modelConsumption() const;

    /** Returns the maximum peak over the billing period if available. */
    boost::optional<double> modelPeakDemand() const;

    /** Returns total energy cost over the billing period if available. */
    boost::optional<double> modelTotalCost() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::UtilityBill_Impl;

    friend class IdfExtensibleGroup;
    friend class detail::UtilityBill_Impl;

    BillingPeriod(std::shared_ptr<detail::UtilityBill_Impl> impl, unsigned index);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.BillingPeriod");
  };

  /** \relates BillingPeriod */
  using BillingPeriodVector = std::vector<BillingPeriod>;

  /** UtilityBill is a ModelObject that wraps the OpenStudio IDD object 'OS:UtilityBill'.
    This object allows the user to enter in actual fuel usage for model calibration.
*/
  class MODEL_API UtilityBill : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit UtilityBill(const FuelType& fuelType, const Model& model);

    virtual ~UtilityBill() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    UtilityBill(const UtilityBill& other) = default;
    UtilityBill(UtilityBill&& other) = default;
    UtilityBill& operator=(const UtilityBill&) = default;
    UtilityBill& operator=(UtilityBill&&) = default;

    //@}

    static IddObjectType iddObjectType();

    std::vector<std::string> consumptionUnitValues();

    std::vector<std::string> peakDemandUnitValues();

    static std::vector<std::string> calibrationGuidelines();

    static boost::optional<double> maxNMBE(const std::string& calibrationGuideline);

    static boost::optional<double> maxCVRMSE(const std::string& calibrationGuideline);

    /** @name Getters */
    //@{

    FuelType fuelType() const;

    InstallLocationType meterInstallLocation() const;

    bool isMeterInstallLocationDefaulted() const;

    boost::optional<std::string> meterSpecificInstallLocation() const;

    boost::optional<EndUseCategoryType> meterEndUseCategory() const;

    boost::optional<std::string> meterSpecificEndUse() const;

    std::string consumptionUnit() const;

    boost::optional<std::string> consumptionUnitDescription() const;

    double consumptionUnitConversionFactor() const;

    bool isConsumptionUnitConversionFactorDefaulted() const;

    boost::optional<std::string> peakDemandUnit() const;

    boost::optional<std::string> peakDemandUnitDescription() const;

    boost::optional<double> peakDemandUnitConversionFactor() const;

    boost::optional<unsigned> timestepsInPeakDemandWindow() const;

    bool isTimestepsInPeakDemandWindowDefaulted() const;

    boost::optional<double> minutesInPeakDemandWindow() const;

    std::vector<BillingPeriod> billingPeriods() const;

    //@}
    /** @name Setters */
    //@{

    bool setMeterInstallLocation(const InstallLocationType& meterInstallLocation);

    void resetMeterInstallLocation();

    bool setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation);

    void resetMeterSpecificInstallLocation();

    bool setMeterEndUseCategory(const EndUseCategoryType& meterEndUseCategory);

    void resetMeterEndUseCategory();

    bool setMeterSpecificEndUse(const std::string& meterSpecificEndUse);

    void resetMeterSpecificEndUse();

    bool setConsumptionUnit(const std::string& consumptionUnit);

    bool setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor);

    void resetConsumptionUnitConversionFactor();

    bool setPeakDemandUnit(const std::string& peakDemandUnit);

    bool setTimestepsInPeakDemandWindow(unsigned timestepsInPeakDemandWindow);

    void resetTimestepsInPeakDemandWindow();

    void clearBillingPeriods();

    BillingPeriod addBillingPeriod();

    //@}
    /** @name Other */
    //@{

    /** Gets the meter associated with consumption for this UtilityBill, creates it if it does not exist.*/
    OutputMeter consumptionMeter() const;

    /** Gets the meter associated with peak demand for this UtilityBill, creates it if it does not exist.*/
    boost::optional<OutputMeter> peakDemandMeter() const;

    /** Number of billing periods used to compute CVRMSE or NMBE.*/
    unsigned numberBillingPeriodsInCalculations() const;

    /** Coefficient of variation of the root mean square error, see ASHRAE 14-2002 5.2.11.3.*/
    boost::optional<double> CVRMSE() const;

    /** Normalized mean bias error, see ASHRAE 14-2002 5.2.11.3.*/
    boost::optional<double> NMBE() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::UtilityBill_Impl;

    explicit UtilityBill(std::shared_ptr<detail::UtilityBill_Impl> impl);

    friend class detail::UtilityBill_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.UtilityBill");
  };

  /** \relates UtilityBill*/
  using OptionalUtilityBill = boost::optional<UtilityBill>;

  /** \relates UtilityBill*/
  using UtilityBillVector = std::vector<UtilityBill>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_UTILITYBILL_HPP
