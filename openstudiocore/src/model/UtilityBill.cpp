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
#include <model/OutputVariable.hpp>
#include <model/OutputVariable_Impl.hpp>
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>

#include <model/Model.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_UtilityBill_FieldEnums.hxx>

#include <utilities/Data/DataEnums.hpp>
#include <utilities/Data/Timeseries.hpp>

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

  boost::optional<EndUseCategoryType> UtilityBill_Impl::meterEndUseCategory() const {
    boost::optional<EndUseCategoryType> result;
    boost::optional<std::string> tmp = getString(OS_UtilityBillFields::MeterEndUseCategory,true,true);
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
      //
      // Original source: 
      // TABLE C–1 TO SUBPART C OF PART 98—DEFAULT CO2 EMISSION FACTORS AND HIGH HEAT VALUES FOR VARIOUS TYPES OF FUEL
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
          case FuelType::Steam:
            unitToKBtu = 1.194;
            break;
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
          case FuelType::Steam:
            unitToKBtu = 1194;
            break;
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
          case FuelType::Steam:
            unitToKBtu = 1194000;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Gallons"){
        switch (fuelType.value()){
          case FuelType::Gasoline:
            unitToKBtu = 125;
            break;
          case FuelType::Diesel:
            unitToKBtu = 138;
            break;
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
          case FuelType::Gasoline:
            unitToKBtu = 33.0215;
            break;
          case FuelType::Diesel:
            unitToKBtu = 36.456;
            break;
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
          case FuelType::Propane:
            unitToKBtu = 2.5185;
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
          case FuelType::Propane:
            unitToKBtu = 2518500;
            break;
          case FuelType::Water:
            unitToM3 = 28316.8;
            break;
          default:
            ;
        }
      }

      if (isEnergy){
        if (unitToKBtu){
          value = 1055055.85 * unitToKBtu.get();
        }
      }else{
        value = unitToM3;
      }

      if (!value){
        LOG(Error, "Unknown consumption unit '" << consumptionUnit << "' for fuel type '" << fuelType.valueName() << "'");
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

  bool UtilityBill_Impl::setMeterEndUseCategory(const EndUseCategoryType& meterEndUseCategory) {
    bool result = setString(OS_UtilityBillFields::MeterEndUseCategory, meterEndUseCategory.valueName());
    return result;
  }

  void UtilityBill_Impl::resetMeterEndUseCategory() {
    bool result = setString(OS_UtilityBillFields::MeterEndUseCategory, "");
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

  Meter UtilityBill_Impl::consumptionMeter() const{
    FuelType fuelType = this->fuelType();
    InstallLocationType meterInstallLocation = this->meterInstallLocation();
    boost::optional<std::string> meterSpecificInstallLocation = this->meterSpecificInstallLocation();
    boost::optional<EndUseCategoryType> meterEndUseCategory = this->meterEndUseCategory();
    boost::optional<EndUseType> meterEndUse;
    if (meterEndUseCategory){
      meterEndUse = EndUseType(meterEndUseCategory->valueName());
    }
    boost::optional<std::string> meterSpecificEndUse = this->meterSpecificEndUse();

    std::string meterName = Meter::getName(meterSpecificEndUse,
                                           meterEndUse,
                                           fuelType,
                                           meterInstallLocation,
                                           meterSpecificInstallLocation);

    boost::optional<Meter> result;
    BOOST_FOREACH(const Meter& meter, this->model().getModelObjects<Meter>()){
      if ((istringEqual(meter.name(), meterName)) &&
          (istringEqual("Daily", meter.reportingFrequency()))){
        return meter;
      }
    }

    result = Meter(this->model());
    result->setReportingFrequency("Daily");
    result->setFuelType(fuelType);
    result->setInstallLocationType(meterInstallLocation);
    if (meterSpecificInstallLocation){
      result->setSpecificInstallLocation(*meterSpecificInstallLocation);
    }
    if (meterEndUse){
      result->setEndUseType(*meterEndUse);
    }
    if (meterSpecificEndUse){
      result->setSpecificEndUse(*meterSpecificEndUse);
    }

    return result.get();
  }

  boost::optional<OutputVariable> UtilityBill_Impl::peakDemandVariable() const{
    FuelType fuelType = this->fuelType();
    InstallLocationType meterInstallLocation = this->meterInstallLocation();
    boost::optional<std::string> meterSpecificInstallLocation = this->meterSpecificInstallLocation();
    boost::optional<EndUseCategoryType> meterEndUseCategory = this->meterEndUseCategory();
    boost::optional<EndUseType> meterEndUse;

    std::string variableName;
    if (fuelType == FuelType::Electricity){
      if (meterInstallLocation == InstallLocationType::Facility){
        variableName = "Facility Total Electric Demand Power";
      }else if (meterInstallLocation == InstallLocationType::Building){
        variableName = "Facility Total Building Electric Demand Power";
      }
    }

    if (variableName.empty()){
      return boost::none;
    }

    BOOST_FOREACH(const OutputVariable& variable, this->model().getModelObjects<OutputVariable>()){
      if (istringEqual(variableName, variable.name().get()) &&
          istringEqual("Daily", variable.reportingFrequency())){
        return variable;
      }
    }

    OutputVariable variable(variableName, this->model());
    variable.setReportingFrequency("Daily");

    return variable;
  }

  std::vector<BillingPeriod> UtilityBill_Impl::billingPeriods() const
  {
    IdfExtensibleGroupVector egs = extensibleGroups();
    std::vector<BillingPeriod> result;
    BOOST_FOREACH(const IdfExtensibleGroup& eg,egs) {
      result.push_back(eg.cast<BillingPeriod>());
    }
    return result;
  }

  void UtilityBill_Impl::clearBillingPeriods()
  {
    clearExtensibleGroups();
    BOOST_ASSERT(numExtensibleGroups() == 0u);
  }

  BillingPeriod UtilityBill_Impl::addBillingPeriod()
  {
    std::vector<BillingPeriod> billingPeriods = this->billingPeriods();

    IdfExtensibleGroup eg = pushExtensibleGroup(StringVector(), false);
    BillingPeriod result = eg.cast<BillingPeriod>();
    
    if (billingPeriods.empty()){
      boost::optional<model::YearDescription> yd = this->model().yearDescription();
      //DLM: we could assert yd and also yd->calendarYear here
      if (!yd){
        yd = this->model().getUniqueModelObject<model::YearDescription>();
      }

      bool test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth, 1);
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, 1);
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, yd->assumedYear());
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod, 30);
      BOOST_ASSERT(test);
    }else{
      Date startDate = billingPeriods.back().endDate() + Time(1);

      bool test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth, startDate.monthOfYear().value());
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, startDate.dayOfMonth());
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, startDate.year());
      BOOST_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod, billingPeriods.back().numberOfDays());
      BOOST_ASSERT(test);
    }
      
    return result;
  }

  boost::optional<double> UtilityBill_Impl::CVRMSE() const
  {
    boost::optional<double> result;
    double ysum = 0;
    double squaredError = 0;
    unsigned n = 0;
    double conv = this->consumptionUnitConversionFactor();
    BOOST_FOREACH(const BillingPeriod& p, this->billingPeriods()){
      boost::optional<double> consumption = p.consumption();
      if (consumption){
        double convertedConsumption = conv * consumption.get();
        boost::optional<double> modelConsumption = p.modelConsumption();
        if (modelConsumption){
          ysum += convertedConsumption;
          squaredError += std::pow(convertedConsumption - *modelConsumption, 2);
          n += 1;
        }
      }
    }

    if (n > 1){
      double ybar = ysum/n;
      result = 100.0 * std::pow(squaredError/(n-1), 0.5) / ybar;
    }

    return result;
  }

  boost::optional<double> UtilityBill_Impl::NMBE() const
  {
    boost::optional<double> result;
    double ysum = 0;
    double sumError = 0;
    unsigned n = 0;
    double conv = this->consumptionUnitConversionFactor();
    BOOST_FOREACH(const BillingPeriod& p, this->billingPeriods()){
      boost::optional<double> consumption = p.consumption();
      if (consumption){
        double convertedConsumption = conv * consumption.get();
        boost::optional<double> modelConsumption = p.modelConsumption();
        if (modelConsumption){
          ysum += convertedConsumption;
          sumError += (convertedConsumption - *modelConsumption);
          n += 1;
        }
      }
    }

    if (n > 1){
      double ybar = ysum/n;
      result = 100.0 * (sumError/(n-1)) / ybar;
    }

    return result;
  }

} // detail


Date BillingPeriod::startDate() const
{
  boost::optional<unsigned> beginMonth = getUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth);
  BOOST_ASSERT(beginMonth);
  boost::optional<unsigned> beginDay = getUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth);
  BOOST_ASSERT(beginDay);
  boost::optional<unsigned> beginYear = getUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear);
  BOOST_ASSERT(beginYear);

  return Date(beginMonth.get(), beginDay.get(), beginYear.get());
}

Date BillingPeriod::endDate() const
{
  Date result = this->startDate() + Time(this->numberOfDays() - 1);
  return result;
}

unsigned BillingPeriod::numberOfDays() const
{
  boost::optional<unsigned> numberOfDays = getUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod);
  BOOST_ASSERT(numberOfDays);
  return numberOfDays.get();
}

boost::optional<double> BillingPeriod::consumption() const
{
  return getDouble(OS_UtilityBillExtensibleFields::BillingPeriodConsumption);
}

boost::optional<double> BillingPeriod::peakDemand() const
{
  return getDouble(OS_UtilityBillExtensibleFields::BillingPeriodPeakDemand);
}

boost::optional<double> BillingPeriod::totalCost() const
{
  return getDouble(OS_UtilityBillExtensibleFields::BillingPeriodTotalCost);
}

bool BillingPeriod::setStartDate(const Date& startDate)
{
  Date currentStartDate = this->startDate();
  unsigned currentNumberOfDays = this->numberOfDays();
  Date currentEndDate = this->endDate();

  /* If startDate is before endDate then endDate is retained.
     If startDate is after endDate then numberOfDays is retained. */

  bool test = setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth, startDate.monthOfYear().value());
  BOOST_ASSERT(test);
  test = setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, startDate.dayOfMonth());
  BOOST_ASSERT(test);
  test = setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, startDate.assumedBaseYear());
  BOOST_ASSERT(test);

  if (startDate < currentEndDate){
    Time newNumberOfDays = currentEndDate - startDate;
    test = this->setNumberOfDays(newNumberOfDays.days() + 1);
    BOOST_ASSERT(test);
  }

  return test;
}

bool BillingPeriod::setEndDate(const Date& endDate)
{
  Date currentStartDate = this->startDate();
  unsigned currentNumberOfDays = this->numberOfDays();
  Date currentEndDate = this->endDate();

  /* If endDate is after startDate then startDate is retained.
     If endDate is before startDate then numberOfDays is retained. */

  bool test = false;

  if (endDate > currentStartDate){
    Time newNumberOfDays = endDate - currentStartDate;
    test = this->setNumberOfDays(newNumberOfDays.days() + 1);
    BOOST_ASSERT(test);
  }else{
    Date newStartDate = endDate - Time(currentNumberOfDays - 1);
    test = this->setStartDate(newStartDate);
    BOOST_ASSERT(test);
    test = this->setNumberOfDays(currentNumberOfDays);
    BOOST_ASSERT(test);
  }

  return test;
}

bool BillingPeriod::setNumberOfDays(unsigned numberOfDays)
{
  bool test = setUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod, numberOfDays);
  return test;
}

bool BillingPeriod::setConsumption(double consumption)
{
  return setDouble(OS_UtilityBillExtensibleFields::BillingPeriodConsumption, consumption);
}

void BillingPeriod::resetConsumption()
{
  bool test = setString(OS_UtilityBillExtensibleFields::BillingPeriodConsumption, "");
  BOOST_ASSERT(test);
}

bool BillingPeriod::setPeakDemand(double peakDemand)
{
  return setDouble(OS_UtilityBillExtensibleFields::BillingPeriodPeakDemand, peakDemand);
}

void BillingPeriod::resetPeakDemand()
{
  bool test = setString(OS_UtilityBillExtensibleFields::BillingPeriodPeakDemand, "");
  BOOST_ASSERT(test);
}

bool BillingPeriod::setTotalCost(double totalCost)
{
  return setDouble(OS_UtilityBillExtensibleFields::BillingPeriodTotalCost, totalCost);
}

void BillingPeriod::resetTotalCost()
{
  bool test = setString(OS_UtilityBillExtensibleFields::BillingPeriodTotalCost, "");
  BOOST_ASSERT(test);
}

bool BillingPeriod::withinRunPeriod() const
{
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return false;
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return false;
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return false;
  }

  Date runPeriodStartDate = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth(), *calendarYear);
  Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth(), *calendarYear);

  Date startDate = this->startDate();
  Date endDate = this->endDate();

  bool startDateInRunPeriod = (startDate >= runPeriodStartDate) && (startDate <= runPeriodEndDate);
  bool endDateInRunPeriod = (endDate >= runPeriodStartDate) && (endDate <= runPeriodEndDate);

  bool result = (startDateInRunPeriod && endDateInRunPeriod);

  return result;
}

bool BillingPeriod::withinPeriodicRunPeriod() const
{
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  bool withinRunPeriod = this->withinRunPeriod();
  if (withinRunPeriod){
    return true;
  }

  bool overlapsRunPeriod = this->overlapsRunPeriod();
  if (!withinRunPeriod){
    return false;
  }

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return false;
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return false;
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return false;
  }

  Date runPeriodStartDatePeriodic = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth(), *calendarYear + 1);
  Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth(), *calendarYear);
  Time time = runPeriodStartDatePeriodic - runPeriodEndDate;

  bool result = (time.days() == 1);

  return result;
}

bool BillingPeriod::overlapsRunPeriod() const
{
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return false;
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return false;
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return false;
  }

  Date runPeriodStartDate = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth(), *calendarYear);
  Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth(), *calendarYear);

  Date startDate = this->startDate();
  Date endDate = this->endDate();

  bool startDateInRunPeriod = (startDate >= runPeriodStartDate) && (startDate <= runPeriodEndDate);
  bool endDateInRunPeriod = (endDate >= runPeriodStartDate) && (endDate <= runPeriodEndDate);

  bool result = (startDateInRunPeriod || endDateInRunPeriod);

  return result;
}

Vector BillingPeriod::modelConsumptionValues() const
{
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return Vector();
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return Vector();
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return Vector();
  }

  Vector result;

  Meter meter = getImpl<detail::UtilityBill_Impl>()->consumptionMeter();

  Date runPeriodStartDate = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth(), *calendarYear);
  Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth(), *calendarYear);

  boost::optional<openstudio::TimeSeries> timeseries = meter.getData(runPeriod->name().get());
  if (timeseries){
    result = Vector(this->numberOfDays());

    double outOfRangeValue = std::numeric_limits<double>::min();
    timeseries->setOutOfRangeValue(outOfRangeValue);

    unsigned i = 0;
    Date date = this->startDate();
    Date endDate = this->endDate();
    while (date <= endDate){

      // Do not include year in date for query
      Date tmp(date.monthOfYear(), date.dayOfMonth());
      DateTime dateTime(tmp, Time(1));

      double value = timeseries->value(dateTime);
      if (value == outOfRangeValue){
        return Vector();
      }else{
        result[i] = value;
      }

      ++i;
      date += Time(1);
    }
  }

  return result;
}

Vector BillingPeriod::modelPeakDemandValues() const
{
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return Vector();
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return Vector();
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return Vector();
  }

  boost::optional<OutputVariable> variable = getImpl<detail::UtilityBill_Impl>()->peakDemandVariable();
  if (!variable){
    return Vector();
  }

  boost::optional<ModelObject> modelObject;
  InstallLocationType meterInstallLocation = getImpl<detail::UtilityBill_Impl>()->meterInstallLocation();
  if (meterInstallLocation == InstallLocationType::Facility){
    modelObject = model.getOptionalUniqueModelObject<Facility>();
  }else if(meterInstallLocation == InstallLocationType::Building){
    modelObject = model.building();
  }

  if (!modelObject){
    return Vector();
  }

  Vector result;

  Date runPeriodStartDate = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth(), *calendarYear);
  Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth(), *calendarYear);

  boost::optional<openstudio::TimeSeries> timeseries = modelObject->getData(*variable, runPeriod->name().get());
  if (timeseries){
    result = Vector(this->numberOfDays());

    double outOfRangeValue = std::numeric_limits<double>::min();
    timeseries->setOutOfRangeValue(outOfRangeValue);

    unsigned i = 0;
    Date date = this->startDate();
    Date endDate = this->endDate();
    while (date <= endDate){

      // Do not include year in date for query
      Date tmp(date.monthOfYear(), date.dayOfMonth());
      DateTime dateTime(tmp, Time(1));

      double value = timeseries->value(dateTime);
      if (value == outOfRangeValue){
        return Vector();
      }else{
        result[i] = value;
      }

      ++i;
      date += Time(1);
    }
  }

  return result;
}

Vector BillingPeriod::modelTotalCostValues() const
{
  return Vector();
}

boost::optional<double> BillingPeriod::modelConsumption() const
{
  boost::optional<double> result;
  Vector modelConsumptionValues = this->modelConsumptionValues();
  if (!modelConsumptionValues.empty()){
    result = sum(modelConsumptionValues);
  }
  return result;
}

boost::optional<double> BillingPeriod::modelPeakDemand() const
{
  boost::optional<double> result;
  Vector modelPeakDemandValues = this->modelPeakDemandValues();
  if (!modelPeakDemandValues.empty()){
    result = sum(modelPeakDemandValues);
  }
  return result;
}

boost::optional<double> BillingPeriod::modelTotalCost() const
{
  boost::optional<double> result;
  Vector modelTotalCostValues = this->modelTotalCostValues();
  if (!modelTotalCostValues.empty()){
    result = sum(modelTotalCostValues);
  }
  return result;
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
  if (!test){
    LOG(Error, fuelType.valueName());
  }
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

boost::optional<EndUseCategoryType> UtilityBill::meterEndUseCategory() const {
  return getImpl<detail::UtilityBill_Impl>()->meterEndUseCategory();
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

bool UtilityBill::setMeterEndUseCategory(const EndUseCategoryType& meterEndUseCategory) {
  return getImpl<detail::UtilityBill_Impl>()->setMeterEndUseCategory(meterEndUseCategory);
}

void UtilityBill::resetMeterEndUseCategory() {
  getImpl<detail::UtilityBill_Impl>()->resetMeterEndUseCategory();
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

Meter UtilityBill::consumptionMeter() const{
  return getImpl<detail::UtilityBill_Impl>()->consumptionMeter();
}

boost::optional<OutputVariable> UtilityBill::peakDemandVariable() const{
  return getImpl<detail::UtilityBill_Impl>()->peakDemandVariable();
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

