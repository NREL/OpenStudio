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
    return getString(OS_UtilityBillFields::MeterSpecificInstallLocation,true);
  }

  boost::optional<EndUseCategoryType> UtilityBill_Impl::meterEndUse() const {
    boost::optional<EndUseCategoryType> result;
    boost::optional<std::string> tmp = getString(OS_UtilityBillFields::MeterEndUse,true);
    if (tmp){
      result = EndUseCategoryType(tmp.get());
    }
    return result;
  }

  boost::optional<std::string> UtilityBill_Impl::meterSpecificEndUse() const {
    return getString(OS_UtilityBillFields::MeterSpecificEndUse,true);
  }

  std::string UtilityBill_Impl::consumptionUnit() const {
    boost::optional<std::string> value = getString(OS_UtilityBillFields::ConsumptionUnit,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double UtilityBill_Impl::consumptionUnitConversionFactor() const {
    boost::optional<double> value = getDouble(OS_UtilityBillFields::ConsumptionUnitConversionFactor,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> UtilityBill_Impl::peakDemandUnit() const {
    return getString(OS_UtilityBillFields::PeakDemandUnit,true);
  }

  bool UtilityBill_Impl::setFuelType(const FuelType& fuelType) {
    bool result = setString(OS_UtilityBillFields::FuelType, fuelType.valueName());
    return result;
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
    bool result = setString(OS_UtilityBillFields::ConsumptionUnit, consumptionUnit);
    return result;
  }

  bool UtilityBill_Impl::setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor) {
    bool result = setDouble(OS_UtilityBillFields::ConsumptionUnitConversionFactor, consumptionUnitConversionFactor);
    return result;
  }

  bool UtilityBill_Impl::setPeakDemandUnit(const std::string& peakDemandUnit) {
    bool result = setString(OS_UtilityBillFields::PeakDemandUnit, peakDemandUnit);
    return result;
  }

  void UtilityBill_Impl::resetPeakDemandUnit() {
    bool result = setString(OS_UtilityBillFields::PeakDemandUnit, "");
    BOOST_ASSERT(result);
  }

  std::vector<std::string> UtilityBill_Impl::consumptionUnitValues() const {
    return UtilityBill::consumptionUnitValues();
  }

  std::vector<std::string> UtilityBill_Impl::peakDemandUnitValues() const {
    return UtilityBill::peakDemandUnitValues();
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

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
  // ok = setFuelType();
  BOOST_ASSERT(ok);
  // ok = setConsumptionUnit();
  BOOST_ASSERT(ok);
  // setConsumptionUnitConversionFactor();
}

IddObjectType UtilityBill::iddObjectType() {
  return IddObjectType(IddObjectType::OS_UtilityBill);
}

std::vector<std::string> UtilityBill::consumptionUnitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_UtilityBillFields::ConsumptionUnit);
}

std::vector<std::string> UtilityBill::peakDemandUnitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_UtilityBillFields::PeakDemandUnit);
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

bool UtilityBill::setPeakDemandUnit(const std::string& peakDemandUnit) {
  return getImpl<detail::UtilityBill_Impl>()->setPeakDemandUnit(peakDemandUnit);
}

void UtilityBill::resetPeakDemandUnit() {
  getImpl<detail::UtilityBill_Impl>()->resetPeakDemandUnit();
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

