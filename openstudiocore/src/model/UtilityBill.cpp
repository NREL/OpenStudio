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

#include <model/UtilityBill.hpp>
#include <model/UtilityBill_Impl.hpp>
#include <model/Meter.hpp>
#include <model/Meter_Impl.hpp>
#include <model/Model.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_UtilityBill_FieldEnums.hxx>

#include <utilities/Data/DataEnums.hpp>
#include <utilities/time/Date.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  UtilityBill_Impl::UtilityBill_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == UtilityBill::iddObjectType());
  }

  UtilityBill_Impl::UtilityBill_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == UtilityBill::iddObjectType());
  }

  UtilityBill_Impl::UtilityBill_Impl(const UtilityBill_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& UtilityBill_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType UtilityBill_Impl::iddObjectType() const {
    return UtilityBill::iddObjectType();
  }

  FuelType UtilityBill_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_UtilityBillFields::FuelType,true);
    BOOST_ASSERT(value);
    return FuelType(value.get());
  }

  InstallLocationType UtilityBill_Impl::meterInstallLocation() const {
    boost::optional<std::string> value = getString(OS_UtilityBillFields::MeterInstallLocation,true);
    BOOST_ASSERT(value);
    return InstallLocationType(value.get());
  }

  bool UtilityBill_Impl::isMeterInstallLocationDefaulted() const {
    return isEmpty(OS_UtilityBillFields::MeterInstallLocation);
  }

  boost::optional<std::string> UtilityBill_Impl::meterSpecificInstallLocation() const {
    return getString(OS_UtilityBillFields::MeterSpecificInstallLocation,true,true);
  }

  boost::optional<EndUseCategoryType> UtilityBill_Impl::meterEndUse() const {
    boost::optional<EndUseCategoryType> result;
    boost::optional<std::string> tmp = getString(OS_UtilityBillFields::MeterEndUse,true,true);
    if (tmp){
      result = EndUseCategoryType(tmp.get());
    }
    return result;
  }

  boost::optional<std::string> UtilityBill_Impl::meterSpecificEndUse() const {
    return getString(OS_UtilityBillFields::MeterSpecificEndUse,true,true);
  }

  std::string UtilityBill_Impl::consumptionUnit() const {
    boost::optional<std::string> value = getString(OS_UtilityBillFields::ConsumptionUnit,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double UtilityBill_Impl::consumptionUnitConversionFactor() const {
    boost::optional<double> value = getDouble(OS_UtilityBillFields::ConsumptionUnitConversionFactor,true);
    
    // if value not set, use a smart default
    if (!value){
      FuelType fuelType = this->fuelType();
      bool isEnergy = (fuelType != FuelType::Water);

      // DLM: Energy conversion factors come from https://www.energystar.gov/ia/business/tools_resources/target_finder/help/Energy_Units_Conversion_Table.htm
      // Portfolio Manager uses these to convert to kBtu
      boost::optional<double> unitToKBtu;
      boost::optional<double> unitToM3;

      std::string consumptionUnit = this->consumptionUnit();

      if (consumptionUnit == "kBtu"){
        unitToKBtu = 1;
      }else if (consumptionUnit == "MBtu"){
        unitToKBtu = 1000;
      }else if (consumptionUnit == "kWh"){
        unitToKBtu = 3.41214163;
      }else if (consumptionUnit == "MWh"){
        unitToKBtu = 341214163;
      }else if (consumptionUnit == "Therms"){
        unitToKBtu = 100;
      }else if (consumptionUnit == "Ton Hours"){
        switch (fuelType.value()){
          case FuelType::DistrictCooling:
            unitToKBtu = 12;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Tons"){
        //switch (fuelType.value()){
          //case FuelType::Coal: 
          //  //unitToKBtu = 25090.0; // anthracite
          //  unitToKBtu = 24930.0; // bituminous
          //  break;
          //default:
          //  ;
        //}
      }else if (consumptionUnit == "Lbs"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1.194;
            break;
          //case FuelType::Coal: 
          //  //unitToKBtu = 12.545; // anthracite
          //  unitToKBtu = 12.465; // bituminous
          //  break;
          default:
            ;
        }
      }else if (consumptionUnit == "kLbs"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1194;
            break;
          //case FuelType::Coal: 
          //  //unitToKBtu = 12545; // anthracite
          //  unitToKBtu = 12465; // bituminous
          //  break;
          default:
            ;
        }
      }else if (consumptionUnit == "MLbs"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1194000;
            break;
          //case FuelType::Coal: 
          //  //unitToKBtu = 12545000; // anthracite
          //  unitToKBtu = 12465000; // bituminous
          //  break;
          default:
            ;
        }
      }else if (consumptionUnit == "Gallons"){
        switch (fuelType.value()){
          case FuelType::FuelOil_1:
            unitToKBtu = 138.6905;
            break;
          case FuelType::FuelOil_2:
            unitToKBtu = 138.6905;
            break;
          case FuelType::Propane:
            unitToKBtu = 91.6476;
            break;
          case FuelType::Water:
            unitToM3 = 0.00378541;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Liters"){
        switch (fuelType.value()){
          case FuelType::FuelOil_1:
            unitToKBtu = 36.060;
            break;
          case FuelType::FuelOil_2:
            unitToKBtu = 36.060;
            break;
          case FuelType::Propane:
            unitToKBtu = 23.828;
            break;
          case FuelType::Water:
            unitToM3 = 0.001;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Cubic Feet"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1.029;
            break;
          case FuelType::Propane:
            unitToKBtu = 2.5185;
            break;
          case FuelType::Water:
            unitToM3 = 0.0283168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Cubic Meters"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 36.339;
            break;
          case FuelType::Water:
            unitToM3 = 1;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "CCF"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 102.9;
            break;
          case FuelType::Propane:
            unitToKBtu = 251.85;
            break;
          case FuelType::Water:
            unitToM3 = 2.83168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "kCF"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1029;
            break;
          case FuelType::Propane:
            unitToKBtu = 2518.5;
            break;
          case FuelType::Water:
            unitToM3 = 28.3168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "MCF"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1029000;
            break;
          case FuelType::Water:
            unitToM3 = 28316.8;
            break;
          default:
            ;
        }
      }else{
        LOG(Error, "Unknown consumption unit '" << consumptionUnit << "' for fuel type '" << fuelType.valueName() << "'");
      }

      if (isEnergy){
        if (unitToKBtu){
          value = 1055055.85 * unitToKBtu.get();
        }
      }else{
        value = unitToM3;
      }
    }

    BOOST_ASSERT(value);
    return value.get();
  }

  bool UtilityBill_Impl::isConsumptionUnitConversionFactorDefaulted() const {
    return isEmpty(OS_UtilityBillFields::PeakDemandUnit);
  }

  boost::optional<std::string> UtilityBill_Impl::peakDemandUnit() const {
    return getString(OS_UtilityBillFields::PeakDemandUnit,true,true);
  }

  bool UtilityBill_Impl::setMeterInstallLocation(const InstallLocationType& meterInstallLocation) {
    bool result = setString(OS_UtilityBillFields::MeterInstallLocation, meterInstallLocation.valueName());
    return result;
  }

  void UtilityBill_Impl::resetMeterInstallLocation() {
    bool result = setString(OS_UtilityBillFields::MeterInstallLocation, "");
    BOOST_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation) {
    bool result = setString(OS_UtilityBillFields::MeterSpecificInstallLocation, meterSpecificInstallLocation);
    return result;
  }

  void UtilityBill_Impl::resetMeterSpecificInstallLocation() {
    bool result = setString(OS_UtilityBillFields::MeterSpecificInstallLocation, "");
    BOOST_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterEndUse(const EndUseCategoryType& meterEndUse) {
    bool result = setString(OS_UtilityBillFields::MeterEndUse, meterEndUse.valueName());
    return result;
  }

  void UtilityBill_Impl::resetMeterEndUse() {
    bool result = setString(OS_UtilityBillFields::MeterEndUse, "");
    BOOST_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterSpecificEndUse(const std::string& meterSpecificEndUse) {
    bool result = setString(OS_UtilityBillFields::MeterSpecificEndUse, meterSpecificEndUse);
    return result;
  }

  void UtilityBill_Impl::resetMeterSpecificEndUse() {
    bool result = setString(OS_UtilityBillFields::MeterSpecificEndUse, "");
    BOOST_ASSERT(result);
  }

  bool UtilityBill_Impl::setConsumptionUnit(const std::string& consumptionUnit) {
    bool result = false;
    std::vector<std::string> consumptionUnitValues = this->consumptionUnitValues();
    std::vector<std::string>::const_iterator it = std::find(consumptionUnitValues.begin(), consumptionUnitValues.end(), consumptionUnit);
    if (it != consumptionUnitValues.end()){
      std::string oldConsumptionUnit = this->consumptionUnit();
      result = setString(OS_UtilityBillFields::ConsumptionUnit, consumptionUnit);
      if (result && (oldConsumptionUnit != consumptionUnit)){
        resetConsumptionUnitConversionFactor();
      }
    }
    return result;
  }

  bool UtilityBill_Impl::setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor) {
    bool result = setDouble(OS_UtilityBillFields::ConsumptionUnitConversionFactor, consumptionUnitConversionFactor);
    return result;
  }

  void UtilityBill_Impl::resetConsumptionUnitConversionFactor() {
    bool result = setString(OS_UtilityBillFields::ConsumptionUnitConversionFactor, "");
    BOOST_ASSERT(result);
  }

  bool UtilityBill_Impl::setPeakDemandUnit(const std::string& peakDemandUnit) {
    bool result = false;
    std::vector<std::string> peakDemandUnitValues = this->peakDemandUnitValues();
    if (peakDemandUnitValues.empty() && peakDemandUnit.empty()){
      result = setString(OS_UtilityBillFields::PeakDemandUnit, peakDemandUnit);
    }else{
      std::vector<std::string>::const_iterator it = std::find(peakDemandUnitValues.begin(), peakDemandUnitValues.end(), peakDemandUnit);
      if (it != peakDemandUnitValues.end()){
        result = setString(OS_UtilityBillFields::PeakDemandUnit, peakDemandUnit);
      }
    }
    return result;
  }

  std::vector<std::string> UtilityBill_Impl::consumptionUnitValues() const {
    std::vector<std::string> result;
    FuelType fuelType = this->fuelType();
    switch (fuelType.value()){
      case FuelType::Electricity:
        result.push_back("kWh");
        result.push_back("MWh");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Gas:
        result.push_back("Therms");
        result.push_back("Cubic Feet");
        result.push_back("CCF");
        result.push_back("kCF");
        result.push_back("MCF");
        result.push_back("Cubic Meters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Gasoline:
        result.push_back("Gallons");
        result.push_back("Liters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Diesel:
        result.push_back("Gallons");
        result.push_back("Liters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      //case FuelType::Coal:
      //  result.push_back("Tons");
      //  result.push_back("Lbs");
      //  result.push_back("kLbs");
      //  result.push_back("MLbs");
      //  result.push_back("kBtu");
      //  result.push_back("MBtu");
      //  break;
      case FuelType::FuelOil_1:
        result.push_back("Gallons");
        result.push_back("Liters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::FuelOil_2:
        result.push_back("Gallons");
        result.push_back("Liters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Propane:
        result.push_back("Cubic Feet");
        result.push_back("CCF");
        result.push_back("kCF");
        result.push_back("MCF");
        result.push_back("Cubic Meters");
        result.push_back("Gallons");
        result.push_back("Liters");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Water:
        result.push_back("Gallons");
        result.push_back("Liters");
        break;
      case FuelType::Steam:
        result.push_back("Lbs");
        result.push_back("kLbs");
        result.push_back("MLbs");
        result.push_back("Therms");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::DistrictCooling:
        result.push_back("Ton Hours");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::DistrictHeating:
        result.push_back("Therms");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::EnergyTransfer:
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      default:
        LOG_AND_THROW("Unknown fuel type '" + fuelType.valueName() + "'");
    }

    return result;
  }

  std::vector<std::string> UtilityBill_Impl::peakDemandUnitValues() const {
    std::vector<std::string> result;
    FuelType fuelType = this->fuelType();
    switch (fuelType.value()){
      case FuelType::Electricity:
        result.push_back("kW");
        result.push_back("MW");
        break;
      case FuelType::Gas:
        break;
      case FuelType::Gasoline:
        break;
      case FuelType::Diesel:
        break;
      //case FuelType::Coal:
      //  break;
      case FuelType::FuelOil_1:
        break;
      case FuelType::FuelOil_2:
        break;
      case FuelType::Propane:
        break;
      case FuelType::Water:
        break;
      case FuelType::Steam:
        break;
      case FuelType::DistrictCooling:
        break;
      case FuelType::DistrictHeating:
        break;
      case FuelType::EnergyTransfer:
        break;
      default:
        LOG_AND_THROW("Unknown fuel type '" + fuelType.valueName() + "'");
    }

    return result;
  }

  Meter UtilityBill_Impl::meter() const{
    BOOST_ASSERT(false);
    return Meter(this->model());
  }

  std::vector<BillingPeriod> UtilityBill_Impl::billingPeriods() const
  {
    BOOST_ASSERT(false);
    return std::vector<BillingPeriod>();
  }

  void UtilityBill_Impl::clearBillingPeriods()
  {
    BOOST_ASSERT(false);
  }

  BillingPeriod UtilityBill_Impl::addBillingPeriod()
  {
    BOOST_ASSERT(false);
    boost::shared_ptr<openstudio::detail::IdfObject_Impl> p = shared_from_this();
    return BillingPeriod(boost::dynamic_pointer_cast<detail::UtilityBill_Impl>(p), (unsigned)0);
  }

  void UtilityBill_Impl::sortBillingPeriods()
  {
    BOOST_ASSERT(false);
  }

  boost::optional<double> UtilityBill_Impl::CVRMSE() const
  {
    BOOST_ASSERT(false);
    return boost::none;
  }

  boost::optional<double> UtilityBill_Impl::NMBE() const
  {
    BOOST_ASSERT(false);
    return boost::none;
  }

} // detail


Date BillingPeriod::startDate() const
{
  BOOST_ASSERT(false);
  return Date(1,1);
}

Date BillingPeriod::endDate() const
{
  BOOST_ASSERT(false);
  return Date(1,1);
}

unsigned BillingPeriod::numberOfDays() const
{
  BOOST_ASSERT(false);
  return 0;
}

bool BillingPeriod::setStartDate(const Date& startDate)
{
  BOOST_ASSERT(false);
  return false;
}

bool BillingPeriod::setEndDate(const Date& endDate)
{
  BOOST_ASSERT(false);
  return false;
}

bool BillingPeriod::setNumberOfDays(unsigned numberOfDays)
{
  BOOST_ASSERT(false);
  return false;
}

bool BillingPeriod::withinRunPeriod() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

bool BillingPeriod::withinPeriodicRunPeriod() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

bool BillingPeriod::overlapsRunPeriod() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::CVRMSE() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::NMBE() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::consumption() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::demand() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::totalCost() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::modelConsumption() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::modelDemand() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

boost::optional<double> BillingPeriod::modelTotalCost() const
{
  BOOST_ASSERT(false);
  return boost::none;
}

BillingPeriod::BillingPeriod(boost::shared_ptr<detail::UtilityBill_Impl> impl,unsigned index)
  : ModelExtensibleGroup(impl, index)
{}

UtilityBill::UtilityBill(const FuelType& fuelType, const Model& model)
  : ModelObject(UtilityBill::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::UtilityBill_Impl>());

  bool test;
  test = setString(OS_UtilityBillFields::FuelType, fuelType.valueName());
  BOOST_ASSERT(test);

  std::vector<std::string> consumptionUnitValues = this->consumptionUnitValues();
  BOOST_ASSERT(!consumptionUnitValues.empty());
  test = setConsumptionUnit(consumptionUnitValues[0]);
  BOOST_ASSERT(test);

  std::vector<std::string> peakDemandUnitValues = this->peakDemandUnitValues();
  if (!peakDemandUnitValues.empty()){
    test = setPeakDemandUnit(peakDemandUnitValues[0]);
    BOOST_ASSERT(test);
  }

}

IddObjectType UtilityBill::iddObjectType() {
  return IddObjectType(IddObjectType::OS_UtilityBill);
}

std::vector<std::string> UtilityBill::consumptionUnitValues() {
  return getImpl<detail::UtilityBill_Impl>()->consumptionUnitValues();
}

std::vector<std::string> UtilityBill::peakDemandUnitValues() {
  return getImpl<detail::UtilityBill_Impl>()->peakDemandUnitValues();
}

FuelType UtilityBill::fuelType() const {
  return getImpl<detail::UtilityBill_Impl>()->fuelType();
}

InstallLocationType UtilityBill::meterInstallLocation() const {
  return getImpl<detail::UtilityBill_Impl>()->meterInstallLocation();
}

bool UtilityBill::isMeterInstallLocationDefaulted() const {
  return getImpl<detail::UtilityBill_Impl>()->isMeterInstallLocationDefaulted();
}

boost::optional<std::string> UtilityBill::meterSpecificInstallLocation() const {
  return getImpl<detail::UtilityBill_Impl>()->meterSpecificInstallLocation();
}

boost::optional<EndUseCategoryType> UtilityBill::meterEndUse() const {
  return getImpl<detail::UtilityBill_Impl>()->meterEndUse();
}

boost::optional<std::string> UtilityBill::meterSpecificEndUse() const {
  return getImpl<detail::UtilityBill_Impl>()->meterSpecificEndUse();
}

std::string UtilityBill::consumptionUnit() const {
  return getImpl<detail::UtilityBill_Impl>()->consumptionUnit();
}

double UtilityBill::consumptionUnitConversionFactor() const {
  return getImpl<detail::UtilityBill_Impl>()->consumptionUnitConversionFactor();
}

bool UtilityBill::isConsumptionUnitConversionFactorDefaulted() const {
  return getImpl<detail::UtilityBill_Impl>()->isConsumptionUnitConversionFactorDefaulted();
}

boost::optional<std::string> UtilityBill::peakDemandUnit() const {
  return getImpl<detail::UtilityBill_Impl>()->peakDemandUnit();
}

bool UtilityBill::setMeterInstallLocation(const InstallLocationType& meterInstallLocation) {
  return getImpl<detail::UtilityBill_Impl>()->setMeterInstallLocation(meterInstallLocation);
}

void UtilityBill::resetMeterInstallLocation() {
  getImpl<detail::UtilityBill_Impl>()->resetMeterInstallLocation();
}

bool UtilityBill::setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation) {
  return getImpl<detail::UtilityBill_Impl>()->setMeterSpecificInstallLocation(meterSpecificInstallLocation);
}

void UtilityBill::resetMeterSpecificInstallLocation() {
  getImpl<detail::UtilityBill_Impl>()->resetMeterSpecificInstallLocation();
}

bool UtilityBill::setMeterEndUse(const EndUseCategoryType& meterEndUse) {
  return getImpl<detail::UtilityBill_Impl>()->setMeterEndUse(meterEndUse);
}

void UtilityBill::resetMeterEndUse() {
  getImpl<detail::UtilityBill_Impl>()->resetMeterEndUse();
}

bool UtilityBill::setMeterSpecificEndUse(const std::string& meterSpecificEndUse) {
  return getImpl<detail::UtilityBill_Impl>()->setMeterSpecificEndUse(meterSpecificEndUse);
}

void UtilityBill::resetMeterSpecificEndUse() {
  getImpl<detail::UtilityBill_Impl>()->resetMeterSpecificEndUse();
}

bool UtilityBill::setConsumptionUnit(const std::string& consumptionUnit) {
  return getImpl<detail::UtilityBill_Impl>()->setConsumptionUnit(consumptionUnit);
}

bool UtilityBill::setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor) {
  return getImpl<detail::UtilityBill_Impl>()->setConsumptionUnitConversionFactor(consumptionUnitConversionFactor);
}

void UtilityBill::resetConsumptionUnitConversionFactor() {
  getImpl<detail::UtilityBill_Impl>()->resetConsumptionUnitConversionFactor();
}

bool UtilityBill::setPeakDemandUnit(const std::string& peakDemandUnit) {
  return getImpl<detail::UtilityBill_Impl>()->setPeakDemandUnit(peakDemandUnit);
}

Meter UtilityBill::meter() const{
  return getImpl<detail::UtilityBill_Impl>()->meter();
}

std::vector<BillingPeriod> UtilityBill::billingPeriods() const{
  return getImpl<detail::UtilityBill_Impl>()->billingPeriods();
}

void UtilityBill::clearBillingPeriods(){
  getImpl<detail::UtilityBill_Impl>()->clearBillingPeriods();
}

BillingPeriod UtilityBill::addBillingPeriod(){
  return getImpl<detail::UtilityBill_Impl>()->addBillingPeriod();
}

void UtilityBill::sortBillingPeriods(){
  getImpl<detail::UtilityBill_Impl>()->sortBillingPeriods();
}

boost::optional<double> UtilityBill::CVRMSE() const{
  return getImpl<detail::UtilityBill_Impl>()->CVRMSE();
}

boost::optional<double> UtilityBill::NMBE() const{
  return getImpl<detail::UtilityBill_Impl>()->NMBE();
}

/// @cond
UtilityBill::UtilityBill(boost::shared_ptr<detail::UtilityBill_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

