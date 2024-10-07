/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_UTILITYBILL_IMPL_HPP
#define MODEL_UTILITYBILL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

class FuelType;
class InstallLocationType;
class EndUseCategoryType;

namespace model {

  class BillingPeriod;

  namespace detail {

    class MODEL_API UtilityBill_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      UtilityBill_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      UtilityBill_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      UtilityBill_Impl(const UtilityBill_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~UtilityBill_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<std::string> consumptionUnitValues() const;

      std::vector<std::string> peakDemandUnitValues() const;

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

      OutputMeter consumptionMeter() const;

      boost::optional<OutputMeter> peakDemandMeter() const;

      unsigned numberBillingPeriodsInCalculations() const;

      boost::optional<double> CVRMSE() const;

      boost::optional<double> NMBE() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.UtilityBill");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_UTILITYBILL_IMPL_HPP
