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

      virtual ~UtilityBill_Impl() = default;

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
