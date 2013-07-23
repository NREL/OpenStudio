/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_UTILITYBILL_IMPL_HPP
#define MODEL_UTILITYBILL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
  
class FuelType;
class InstallLocationType;
class EndUseCategoryType;

namespace model {

class BillingPeriod;

namespace detail {

  class MODEL_API UtilityBill_Impl : public ModelObject_Impl {
    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    UtilityBill_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle);

    UtilityBill_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    UtilityBill_Impl(const UtilityBill_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~UtilityBill_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    FuelType fuelType() const;

    InstallLocationType meterInstallLocation() const;

    bool isMeterInstallLocationDefaulted() const;

    boost::optional<std::string> meterSpecificInstallLocation() const;

    boost::optional<EndUseCategoryType> meterEndUse() const;

    boost::optional<std::string> meterSpecificEndUse() const;

    std::string consumptionUnit() const;

    double consumptionUnitConversionFactor() const;

    Quantity getConsumptionUnitConversionFactor(bool returnIP=false) const;

    boost::optional<std::string> peakDemandUnit() const;

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(const FuelType& fuelType);

    bool setMeterInstallLocation(const InstallLocationType& meterInstallLocation);

    void resetMeterInstallLocation();

    bool setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation);

    void resetMeterSpecificInstallLocation();

    bool setMeterEndUse(const EndUseCategoryType& meterEndUse);

    void resetMeterEndUse();

    bool setMeterSpecificEndUse(const std::string& meterSpecificEndUse);

    void resetMeterSpecificEndUse();

    bool setConsumptionUnit(const std::string& consumptionUnit);

    bool setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor);

    bool setPeakDemandUnit(const std::string& peakDemandUnit);

    void resetPeakDemandUnit();

    //@}
    /** @name Other */
    //@{

    Meter meter() const;

    std::vector<BillingPeriod> billingPeriods() const;

    void clearBillingPeriods();

    BillingPeriod addBillingPeriod();

    void sortBillingPeriods();

    boost::optional<double> CVRMSE() const;

    boost::optional<double> NMBE() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.UtilityBill");

    std::vector<std::string> consumptionUnitValues() const;
    std::vector<std::string> peakDemandUnitValues() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_UTILITYBILL_IMPL_HPP

