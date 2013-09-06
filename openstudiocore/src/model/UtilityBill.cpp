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
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Timestep.hpp>
#include <model/Timestep_Impl.hpp>


#include <model/Model.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_UtilityBill_FieldEnums.hxx>

#include <utilities/data/DataEnums.hpp>
#include <utilities/data/TimeSeries.hpp>

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
    OS_ASSERT(idfObject.iddObject().type() == UtilityBill::iddObjectType());
  }

  UtilityBill_Impl::UtilityBill_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == UtilityBill::iddObjectType());
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
    OS_ASSERT(value);
    return FuelType(value.get());
  }

  InstallLocationType UtilityBill_Impl::meterInstallLocation() const {
    boost::optional<std::string> value = getString(OS_UtilityBillFields::MeterInstallLocation,true);
    OS_ASSERT(value);
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
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> UtilityBill_Impl::consumptionUnitDescription() const
  {
    boost::optional<std::string> result;

    FuelType fuelType = this->fuelType();

    std::string consumptionUnit = this->consumptionUnit();

    if (consumptionUnit == "kBtu"){
      result = "thousand Btu";
    }else if (consumptionUnit == "MBtu"){
      result = "million Btu";
    }else if (consumptionUnit == "kWh"){
      result = "thousand Watt hours";
    }else if (consumptionUnit == "MWh"){
      result = "million Watt hours";
    }else if (consumptionUnit == "therms"){
      result = "hundred thousand Btu";
    }else if (consumptionUnit == "RTh"){
      result = "refrigeration ton hours";
    }else if (consumptionUnit == "tons"){
      result = "two thousand pounds";
    }else if (consumptionUnit == "lbs"){
      result = "pounds";
    }else if (consumptionUnit == "kLbs"){
      result = "thousand pounds";
    }else if (consumptionUnit == "MLbs"){
      result = "million pounds";
    }else if (consumptionUnit == "gal"){
      result = "gallons";
    }else if (consumptionUnit == "L"){
      result = "liters";
    }else if (consumptionUnit == "cf"){
      result = "cubic feet";
    }else if (consumptionUnit == "m3"){
      result = "cubic meters";
    }else if (consumptionUnit == "Ccf"){
      result = "hundred cubic feet";
    }else if (consumptionUnit == "Mcf"){
      result = "thousand cubic feet";
    }else if (consumptionUnit == "MMcf"){
      result = "million cubic feet";
    }

    if (!result){
      LOG(Error, "Unknown consumption unit '" << consumptionUnit << "' for fuel type '" << fuelType.valueName() << "'");
    }

    return result;
  }

  double UtilityBill_Impl::consumptionUnitConversionFactor() const {
    boost::optional<double> value = getDouble(OS_UtilityBillFields::ConsumptionUnitConversionFactor,true);
    
    // if value not set, use a smart default
    if (!value){
      FuelType fuelType = this->fuelType();
      bool isEnergy = (fuelType != FuelType::Water);

      // DLM: Energy conversion factors come from https://www.energystar.gov/ia/business/tools_resources/target_finder/help/Energy_Units_Conversion_Table.htm
      // Thermal Energy Conversions, Technical Reference, July 2013
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
        unitToKBtu = 3.4121416331;
      }else if (consumptionUnit == "MWh"){
        unitToKBtu = 3412.1416331;
      }else if (consumptionUnit == "therms"){
        unitToKBtu = 100;
      }else if (consumptionUnit == "RTh"){
        switch (fuelType.value()){
          case FuelType::DistrictCooling:
            unitToKBtu = 12;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "tons"){
        //switch (fuelType.value()){
          //case FuelType::Coal: 
          //  //unitToKBtu = 25090.0; // anthracite
          //  unitToKBtu = 24930.0; // bituminous
          //  break;
          //default:
          //  ;
        //}
      }else if (consumptionUnit == "lbs"){
        switch (fuelType.value()){
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
      }else if (consumptionUnit == "gal"){
        switch (fuelType.value()){
          case FuelType::Gasoline:
            unitToKBtu = 125;
            break;
          case FuelType::Diesel:
            unitToKBtu = 138;
            break;
          case FuelType::FuelOil_1:
            unitToKBtu = 139;
            break;
          case FuelType::FuelOil_2:
            unitToKBtu = 138;
            break;
          case FuelType::Propane:
            unitToKBtu = 91;
            break;
          case FuelType::Water:
            unitToM3 = 0.00378541;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "L"){
        switch (fuelType.value()){
          case FuelType::Gasoline:
            unitToKBtu = 33.0217;
            break;
          case FuelType::Diesel:
            unitToKBtu = 36.456;
            break;
          case FuelType::FuelOil_1:
            unitToKBtu = 36.720;
            break;
          case FuelType::FuelOil_2:
            unitToKBtu = 36.456;
            break;
          case FuelType::Propane:
            unitToKBtu = 24.040;
            break;
          case FuelType::Water:
            unitToM3 = 0.001;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "cf"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1.028;
            break;
          case FuelType::Propane:
            unitToKBtu = 0.680;
            break;
          case FuelType::Water:
            unitToM3 = 0.0283168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "m3"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 36.303;
            break;
          case FuelType::Propane:
            unitToKBtu = 24.0137;
            break;
          case FuelType::Water:
            unitToM3 = 1;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Ccf"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 102.8;
            break;
          case FuelType::Propane:
            unitToKBtu = 68;
            break;
          case FuelType::Water:
            unitToM3 = 2.83168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "Mcf"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1028;
            break;
          case FuelType::Propane:
            unitToKBtu = 680;
            break;
          case FuelType::Water:
            unitToM3 = 28.3168;
            break;
          default:
            ;
        }
      }else if (consumptionUnit == "MMcf"){
        switch (fuelType.value()){
          case FuelType::Gas:
            unitToKBtu = 1028000;
            break;
          case FuelType::Propane:
            unitToKBtu = 680000;
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


    OS_ASSERT(value);
    return value.get();
  }

  bool UtilityBill_Impl::isConsumptionUnitConversionFactorDefaulted() const {
    return isEmpty(OS_UtilityBillFields::PeakDemandUnit);
  }

  boost::optional<std::string> UtilityBill_Impl::peakDemandUnit() const {
    if (this->fuelType() == FuelType::Electricity){
      return getString(OS_UtilityBillFields::PeakDemandUnit,true,true);
    }
    return boost::none;
  }

  boost::optional<std::string> UtilityBill_Impl::peakDemandUnitDescription() const
  {
    boost::optional<std::string> result;
    boost::optional<std::string> peakDemandUnit = this->peakDemandUnit();
    if (peakDemandUnit){
      if (peakDemandUnit.get() == "kW"){
        result = "thousand Watts";
      }else if (peakDemandUnit.get() == "MW"){
        result = "million Watts";
      }else if (peakDemandUnit.get() == "W"){
        result = "Watts";
      }else{
        LOG(Error, "Unknown peak demand unit '" << peakDemandUnit.get() << "' for fuel type '" << this->fuelType().valueName() << "'");
      }
    }
    return result;
  }

  boost::optional<double> UtilityBill_Impl::peakDemandUnitConversionFactor() const
  {
    boost::optional<double> result;
    boost::optional<std::string> peakDemandUnit = this->peakDemandUnit();
    if (peakDemandUnit){
      FuelType fuelType = this->fuelType();

      if (peakDemandUnit.get() == "kW"){
        result = 1000;
      }else if (peakDemandUnit.get() == "MW"){
        result = 1000000;
      }else if (peakDemandUnit.get() == "W"){
        result = 1;
      }else{
        LOG(Error, "Unknown peak demand unit '" << peakDemandUnit.get() << "' for fuel type '" << fuelType.valueName() << "'");
      }
    }
    return result;
  }

  boost::optional<unsigned> UtilityBill_Impl::timestepsInPeakDemandWindow() const {
    if (this->fuelType() == FuelType::Electricity){
      return getUnsigned(OS_UtilityBillFields::TimestepsinPeakDemandWindow,true);
    }
    return boost::none;
  }

  bool UtilityBill_Impl::isTimestepsInPeakDemandWindowDefaulted() const {
    return isEmpty(OS_UtilityBillFields::TimestepsinPeakDemandWindow);
  }

  boost::optional<double> UtilityBill_Impl::minutesInPeakDemandWindow() const {
    boost::optional<double> result;
    boost::optional<unsigned> timestepsInPeakDemandWindow = this->timestepsInPeakDemandWindow();
    if (timestepsInPeakDemandWindow){
      Timestep timestep = this->model().getUniqueModelObject<Timestep>();
      result = (60.0 * timestepsInPeakDemandWindow.get()) / timestep.numberOfTimestepsPerHour();

    }
    return result;
  }

  bool UtilityBill_Impl::setMeterInstallLocation(const InstallLocationType& meterInstallLocation) {
    bool result = setString(OS_UtilityBillFields::MeterInstallLocation, meterInstallLocation.valueName());
    return result;
  }

  void UtilityBill_Impl::resetMeterInstallLocation() {
    bool result = setString(OS_UtilityBillFields::MeterInstallLocation, "");
    OS_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation) {
    bool result = setString(OS_UtilityBillFields::MeterSpecificInstallLocation, meterSpecificInstallLocation);
    return result;
  }

  void UtilityBill_Impl::resetMeterSpecificInstallLocation() {
    bool result = setString(OS_UtilityBillFields::MeterSpecificInstallLocation, "");
    OS_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterEndUseCategory(const EndUseCategoryType& meterEndUseCategory) {
    bool result = setString(OS_UtilityBillFields::MeterEndUseCategory, meterEndUseCategory.valueName());
    return result;
  }

  void UtilityBill_Impl::resetMeterEndUseCategory() {
    bool result = setString(OS_UtilityBillFields::MeterEndUseCategory, "");
    OS_ASSERT(result);
  }

  bool UtilityBill_Impl::setMeterSpecificEndUse(const std::string& meterSpecificEndUse) {
    bool result = setString(OS_UtilityBillFields::MeterSpecificEndUse, meterSpecificEndUse);
    return result;
  }

  void UtilityBill_Impl::resetMeterSpecificEndUse() {
    bool result = setString(OS_UtilityBillFields::MeterSpecificEndUse, "");
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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

  bool UtilityBill_Impl::setTimestepsInPeakDemandWindow(unsigned timestepsInPeakDemandWindow){
    if (this->fuelType() == FuelType::Electricity){
      return setUnsigned(OS_UtilityBillFields::TimestepsinPeakDemandWindow, timestepsInPeakDemandWindow);
    }
    return false;
  }

  void UtilityBill_Impl::resetTimestepsInPeakDemandWindow(){
    bool test = setString(OS_UtilityBillFields::TimestepsinPeakDemandWindow, "");
    OS_ASSERT(test);
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
        result.push_back("therms");
        result.push_back("cf");
        result.push_back("Ccf");
        result.push_back("Mcf");
        result.push_back("MMcf");
        result.push_back("m3");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Gasoline:
        result.push_back("gal");
        result.push_back("L");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Diesel:
        result.push_back("gal");
        result.push_back("L");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      //case FuelType::Coal:
      //  result.push_back("tons");
      //  result.push_back("lbs");
      //  result.push_back("kLbs");
      //  result.push_back("MLbs");
      //  result.push_back("kBtu");
      //  result.push_back("MBtu");
      //  break;
      case FuelType::FuelOil_1:
        result.push_back("gal");
        result.push_back("L");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::FuelOil_2:
        result.push_back("gal");
        result.push_back("L");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Propane:
        result.push_back("cf");
        result.push_back("Ccf");
        result.push_back("Mcf");
        result.push_back("MMcf");
        result.push_back("m3");
        result.push_back("gal");
        result.push_back("L");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::Water:
        result.push_back("gal");
        result.push_back("L");
        break;
      case FuelType::Steam:
        result.push_back("lbs");
        result.push_back("kLbs");
        result.push_back("MLbs");
        result.push_back("therms");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::DistrictCooling:
        result.push_back("RTh");
        result.push_back("kBtu");
        result.push_back("MBtu");
        break;
      case FuelType::DistrictHeating:
        result.push_back("therms");
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
        result.push_back("W");
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

  boost::optional<Meter> UtilityBill_Impl::peakDemandMeter() const{
    FuelType fuelType = this->fuelType();
    InstallLocationType meterInstallLocation = this->meterInstallLocation();
    boost::optional<std::string> meterSpecificInstallLocation = this->meterSpecificInstallLocation();
    boost::optional<EndUseCategoryType> meterEndUseCategory = this->meterEndUseCategory();
    boost::optional<EndUseType> meterEndUse;
    if (meterEndUseCategory){
      meterEndUse = EndUseType(meterEndUseCategory->valueName());
    }
    boost::optional<std::string> meterSpecificEndUse = this->meterSpecificEndUse();

    std::string variableName;
    if (fuelType != FuelType::Electricity){
      return boost::none;
    }

    std::string meterName = Meter::getName(meterSpecificEndUse,
                                           meterEndUse,
                                           fuelType,
                                           meterInstallLocation,
                                           meterSpecificInstallLocation);

    boost::optional<Meter> result;
    BOOST_FOREACH(const Meter& meter, this->model().getModelObjects<Meter>()){
      if ((istringEqual(meter.name(), meterName)) &&
          (istringEqual("Timestep", meter.reportingFrequency()))){
        return meter;
      }
    }

    result = Meter(this->model());
    result->setReportingFrequency("Timestep");
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

    return result;
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
    OS_ASSERT(numExtensibleGroups() == 0u);
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

      bool wasBlocked = this->blockSignals(true);
      bool test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth, 1);
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, 1);
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, yd->assumedYear());
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod, 30);
      OS_ASSERT(test);
      this->blockSignals(wasBlocked);
      this->emitChangeSignals();
    }else{
      Date startDate = billingPeriods.back().endDate() + Time(1);

      bool wasBlocked = this->blockSignals(true);
      bool test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginMonth, startDate.monthOfYear().value());
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, startDate.dayOfMonth());
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, startDate.year());
      OS_ASSERT(test);
      test = result.setUnsigned(OS_UtilityBillExtensibleFields::NumberofDaysinBillingPeriod, billingPeriods.back().numberOfDays());
      OS_ASSERT(test);
      this->blockSignals(wasBlocked);
      this->emitChangeSignals();
    }
      
    return result;
  }

  unsigned UtilityBill_Impl::numberBillingPeriodsInCalculations() const
  {
    unsigned n = 0;
    BOOST_FOREACH(const BillingPeriod& p, this->billingPeriods()){
      boost::optional<double> consumption = p.consumption();
      if (consumption){
        boost::optional<double> modelConsumption = p.modelConsumption();
        if (modelConsumption){
          n += 1;
        }
      }
    }

    return n;
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
  OS_ASSERT(beginMonth);
  boost::optional<unsigned> beginDay = getUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth);
  OS_ASSERT(beginDay);
  boost::optional<unsigned> beginYear = getUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear);
  OS_ASSERT(beginYear);

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
  OS_ASSERT(numberOfDays);
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
  OS_ASSERT(test);
  test = setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginDayofMonth, startDate.dayOfMonth());
  OS_ASSERT(test);
  test = setUnsigned(OS_UtilityBillExtensibleFields::BillingPeriodBeginYear, startDate.assumedBaseYear());
  OS_ASSERT(test);

  if (startDate < currentEndDate){
    Time newNumberOfDays = currentEndDate - startDate;
    test = this->setNumberOfDays(newNumberOfDays.days() + 1);
    OS_ASSERT(test);
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
    OS_ASSERT(test);
  }else{
    Date newStartDate = endDate - Time(currentNumberOfDays - 1);
    test = this->setStartDate(newStartDate);
    OS_ASSERT(test);
    test = this->setNumberOfDays(currentNumberOfDays);
    OS_ASSERT(test);
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
  OS_ASSERT(test);
}

bool BillingPeriod::setPeakDemand(double peakDemand)
{
  return setDouble(OS_UtilityBillExtensibleFields::BillingPeriodPeakDemand, peakDemand);
}

void BillingPeriod::resetPeakDemand()
{
  bool test = setString(OS_UtilityBillExtensibleFields::BillingPeriodPeakDemand, "");
  OS_ASSERT(test);
}

bool BillingPeriod::setTotalCost(double totalCost)
{
  return setDouble(OS_UtilityBillExtensibleFields::BillingPeriodTotalCost, totalCost);
}

void BillingPeriod::resetTotalCost()
{
  bool test = setString(OS_UtilityBillExtensibleFields::BillingPeriodTotalCost, "");
  OS_ASSERT(test);
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

      boost::optional<DateTime> dateTime;

      // Do not include year in date for query, trap for case of leap year
      if ((date.monthOfYear() == MonthOfYear::Feb) && (date.dayOfMonth()==29)){
        Date tmp(MonthOfYear::Feb, 28);
        dateTime = DateTime(tmp, Time(1));
      }else{
        Date tmp(date.monthOfYear(), date.dayOfMonth());
        dateTime = DateTime(tmp, Time(1));
      }
      OS_ASSERT(dateTime);

      double value = timeseries->value(*dateTime);
      if (value == outOfRangeValue){
        LOG(Debug, "Could not find value of timeseries at dateTime " << dateTime);
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
  model::Model model = getImpl<detail::UtilityBill_Impl>()->model();

  boost::optional<RunPeriod> runPeriod = model.runPeriod();
  if (!runPeriod){
    return boost::none;
  }

  boost::optional<model::YearDescription> yd = model.yearDescription();
  if (!yd){
    return boost::none;
  }

  boost::optional<int> calendarYear = yd->calendarYear();
  if (!calendarYear){
    return boost::none;
  }

  boost::optional<Meter> meter = getImpl<detail::UtilityBill_Impl>()->peakDemandMeter();
  if (!meter){
    return boost::none;
  }

  boost::optional<unsigned> timestepsInPeakDemandWindow = getImpl<detail::UtilityBill_Impl>()->timestepsInPeakDemandWindow();
  if (!timestepsInPeakDemandWindow){
    return boost::none;
  }

  boost::optional<double> minutesInPeakDemandWindow = getImpl<detail::UtilityBill_Impl>()->minutesInPeakDemandWindow();
  if (!minutesInPeakDemandWindow){
    return boost::none;
  }

  boost::optional<double> result;
  Vector window(*timestepsInPeakDemandWindow, 0);

  boost::optional<openstudio::TimeSeries> timeseries = meter->getData(runPeriod->name().get());
  if (timeseries){
    result = std::numeric_limits<double>::min();

    double outOfRangeValue = std::numeric_limits<double>::min();
    timeseries->setOutOfRangeValue(outOfRangeValue);

    // intentionally leave out calendar year
    Date runPeriodStartDate = Date(runPeriod->getBeginMonth(), runPeriod->getBeginDayOfMonth());
    Date runPeriodEndDate = Date(runPeriod->getEndMonth(), runPeriod->getEndDayOfMonth());

    DateTime runPeriodStartDateTime = DateTime(runPeriodStartDate, Time(0,1,0,0));
    DateTime runPeriodEndDateTime = DateTime(runPeriodEndDate, Time(0,24,0,0));

    Vector values = timeseries->values(runPeriodStartDateTime, runPeriodEndDateTime);
    unsigned numValues = values.size();

    if (numValues < *timestepsInPeakDemandWindow){
      return boost::none;
    }

    for (unsigned i = 0; i < numValues; ++i){
      //  shift window
      for (unsigned j = *timestepsInPeakDemandWindow - 1; j > 0; --j){
        window[j] = window[j-1];
      }
      window[0] = values[i];

      // compute energy over window in J
      double energy = sum(window);

      // compute average power in W
      double power = energy / (60.0*minutesInPeakDemandWindow.get());

      result = std::max(power, *result);
    }

    
  }

  return result;
}

boost::optional<double> BillingPeriod::modelTotalCost() const
{
  return boost::none;
}

BillingPeriod::BillingPeriod(boost::shared_ptr<detail::UtilityBill_Impl> impl,unsigned index)
  : ModelExtensibleGroup(impl, index)
{}

UtilityBill::UtilityBill(const FuelType& fuelType, const Model& model)
  : ModelObject(UtilityBill::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::UtilityBill_Impl>());

  bool test;
  test = setString(OS_UtilityBillFields::FuelType, fuelType.valueName());
  if (!test){
    LOG(Error, fuelType.valueName());
  }
  OS_ASSERT(test);

  std::vector<std::string> consumptionUnitValues = this->consumptionUnitValues();
  OS_ASSERT(!consumptionUnitValues.empty());
  test = setConsumptionUnit(consumptionUnitValues[0]);
  OS_ASSERT(test);

  std::vector<std::string> peakDemandUnitValues = this->peakDemandUnitValues();
  if (!peakDemandUnitValues.empty()){
    test = setPeakDemandUnit(peakDemandUnitValues[0]);
    OS_ASSERT(test);
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

std::vector<std::string> UtilityBill::calibrationGuidelines()
{
  std::vector<std::string> result;
  result.push_back("ASHRAE 14-2002");
  result.push_back("FEMP"); // TODO, name, year?
  return result;
}

boost::optional<double> UtilityBill::maxNMBE(const std::string& calibrationGuideline)
{
  boost::optional<double> result;
  if (istringEqual("ASHRAE 14-2002", calibrationGuideline)){
    result = 5;
  }else if (istringEqual("FEMP", calibrationGuideline)){
    result = 15;
  }
  return result;
}

boost::optional<double> UtilityBill::maxCVRMSE(const std::string& calibrationGuideline)
{
  boost::optional<double> result;
  if (istringEqual("ASHRAE 14-2002", calibrationGuideline)){
    result = 15;
  }else if (istringEqual("FEMP", calibrationGuideline)){
    result = 10;
  }
  return result;
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

boost::optional<std::string> UtilityBill::consumptionUnitDescription() const
{
  return getImpl<detail::UtilityBill_Impl>()->consumptionUnitDescription();
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

boost::optional<std::string> UtilityBill::peakDemandUnitDescription() const
{
  return getImpl<detail::UtilityBill_Impl>()->peakDemandUnitDescription();
}

boost::optional<double> UtilityBill::peakDemandUnitConversionFactor() const {
  return getImpl<detail::UtilityBill_Impl>()->peakDemandUnitConversionFactor();
}

boost::optional<unsigned> UtilityBill::timestepsInPeakDemandWindow() const {
  return getImpl<detail::UtilityBill_Impl>()->timestepsInPeakDemandWindow();
}

bool UtilityBill::isTimestepsInPeakDemandWindowDefaulted() const {
  return getImpl<detail::UtilityBill_Impl>()->isTimestepsInPeakDemandWindowDefaulted();
}

boost::optional<double> UtilityBill::minutesInPeakDemandWindow() const {
  return getImpl<detail::UtilityBill_Impl>()->minutesInPeakDemandWindow();
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

bool UtilityBill::setTimestepsInPeakDemandWindow(unsigned timestepsInPeakDemandWindow){
  return getImpl<detail::UtilityBill_Impl>()->setTimestepsInPeakDemandWindow(timestepsInPeakDemandWindow);
}

void UtilityBill::resetTimestepsInPeakDemandWindow(){
  getImpl<detail::UtilityBill_Impl>()->resetTimestepsInPeakDemandWindow();
}

Meter UtilityBill::consumptionMeter() const{
  return getImpl<detail::UtilityBill_Impl>()->consumptionMeter();
}

boost::optional<Meter> UtilityBill::peakDemandMeter() const{
  return getImpl<detail::UtilityBill_Impl>()->peakDemandMeter();
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

unsigned UtilityBill::numberBillingPeriodsInCalculations() const
{
  return getImpl<detail::UtilityBill_Impl>()->numberBillingPeriodsInCalculations();
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

