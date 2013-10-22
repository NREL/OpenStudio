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

#include <model/Meter.hpp>
#include <model/Meter_Impl.hpp>

#include <model/Model.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Meter_FieldEnums.hxx>

#include <utilities/data/DataEnums.hpp>
#include <utilities/data/TimeSeries.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/sql/SqlFileEnums.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model {

namespace detail {

  Meter_Impl::Meter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Meter::iddObjectType());
  }

  Meter_Impl::Meter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Meter::iddObjectType());
  }

  Meter_Impl::Meter_Impl(const Meter_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Meter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Meter_Impl::iddObjectType() const {
    return Meter::iddObjectType();
  }

  boost::optional<ParentObject> Meter_Impl::parent() const
  {
    boost::optional<ParentObject> result;
    boost::optional<InstallLocationType> installLocationType = this->installLocationType();
    if(installLocationType){
      switch (installLocationType->value()){
        case InstallLocationType::Facility:
          result = this->model().getOptionalUniqueModelObject<Facility>();
          break;
        case InstallLocationType::Building:
          result = this->model().building();
          break;
        default:
          ;
      }

    }
    return result;
  }

  std::string Meter_Impl::name() const {
    boost::optional<std::string> value = getString(OS_MeterFields::Name,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string Meter_Impl::reportingFrequency() const {
    boost::optional<std::string> value = getString(OS_MeterFields::ReportingFrequency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Meter_Impl::isReportingFrequencyDefaulted() const {
    return isEmpty(OS_MeterFields::ReportingFrequency);
  }

  bool Meter_Impl::meterFileOnly() const {
    boost::optional<std::string> value = getString(OS_MeterFields::MeterFileOnly,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool Meter_Impl::isMeterFileOnlyDefaulted() const {
    return isEmpty(OS_MeterFields::MeterFileOnly);
  }

  bool Meter_Impl::cumulative() const {
    boost::optional<std::string> value = getString(OS_MeterFields::Cumulative,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool Meter_Impl::isCumulativeDefaulted() const {
    return isEmpty(OS_MeterFields::Cumulative);
  }

  boost::optional<std::string> Meter_Impl::specificEndUse() const
  {
    boost::optional<std::string> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, Meter::meterRegex()))
    {
      // match is always true but may be empty
      if (matches[1].first != matches[1].second){
        result = std::string(matches[1].first, matches[1].second);
      }
    }

    return result;
  }

  boost::optional<EndUseType> Meter_Impl::endUseType() const
  {
    boost::optional<EndUseType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, Meter::meterRegex()))
    {
      if (matches[2].matched){
        result = EndUseType(std::string(matches[2].first, matches[2].second));
      }
    }

    return result;
  }

  boost::optional<FuelType> Meter_Impl::fuelType() const
  {
    boost::optional<FuelType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, Meter::meterRegex()))
    {
      if (matches[3].matched){
        result = FuelType(std::string(matches[3].first, matches[3].second));
      }
    }

    return result;
  }

  boost::optional<InstallLocationType> Meter_Impl::installLocationType() const
  {
    boost::optional<InstallLocationType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, Meter::meterRegex()))
    {
      if (matches[4].matched){
        result = InstallLocationType(std::string(matches[4].first, matches[4].second));
      }
    }

    return result;
  }

  boost::optional<std::string> Meter_Impl::specificInstallLocation() const
  {
    boost::optional<std::string> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, Meter::meterRegex()))
    {
      // match is always true but may be empty
      if (matches[5].first != matches[5].second){
        result = std::string(matches[5].first, matches[5].second);
      }
    }

    return result;
  }

  bool Meter_Impl::setName(std::string name) {
    return setString(OS_MeterFields::Name, name);
  }

  bool Meter_Impl::setReportingFrequency(const std::string& reportingFrequency) {
    return setString(OS_MeterFields::ReportingFrequency, reportingFrequency);
  }

  void Meter_Impl::resetReportingFrequency() {
    bool result = setString(OS_MeterFields::ReportingFrequency, "");
    OS_ASSERT(result);
  }

  void Meter_Impl::setMeterFileOnly(bool meterFileOnly) {
    bool result = false;
    if (meterFileOnly) {
      result = setString(OS_MeterFields::MeterFileOnly, "True");
    } else {
      result = setString(OS_MeterFields::MeterFileOnly, "False");
    }
    OS_ASSERT(result);
  }

  void Meter_Impl::resetMeterFileOnly() {
    bool result = setString(OS_MeterFields::MeterFileOnly, "");
    OS_ASSERT(result);
  }

  void Meter_Impl::setCumulative(bool cumulative) {
    bool result = false;
    if (cumulative) {
      result = setString(OS_MeterFields::Cumulative, "True");
    } else {
      result = setString(OS_MeterFields::Cumulative, "False");
    }
    OS_ASSERT(result);
  }

  void Meter_Impl::resetCumulative() {
    bool result = setString(OS_MeterFields::Cumulative, "");
    OS_ASSERT(result);
  }

  bool Meter_Impl::setSpecificEndUse(const std::string& endUse)
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(endUse, endUseType(), fuelType(), installLocationType(), specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::resetSpecificEndUse()
  {
    return setSpecificEndUse("");
  }

  bool Meter_Impl::setEndUseType(EndUseType type)
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), type, fuelType(), installLocationType(), specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::resetEndUseType()
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), boost::none, fuelType(), installLocationType(), specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::setFuelType(FuelType type)
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), endUseType(), type, installLocationType(), specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::resetFuelType()
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), endUseType(), boost::none, installLocationType(), specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::setInstallLocationType(InstallLocationType type)
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), endUseType(), fuelType(), type, specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::resetInstallLocationType()
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), endUseType(), fuelType(), boost::none, specificInstallLocation());

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::setSpecificInstallLocation(const std::string& locationName)
  {
    bool result = false;
    ModelObject object = getObject<ModelObject>();

    std::string name = Meter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), locationName);

    result = object.setString(OS_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool Meter_Impl::resetSpecificInstallLocation()
  {
    return setSpecificInstallLocation("");
  }

  openstudio::OptionalTimeSeries Meter_Impl::getData(const std::string& envPeriod) const
  {
    return getData(envPeriod, specificInstallLocation());
  }

  openstudio::OptionalTimeSeries Meter_Impl::getData(const std::string& envPeriod, const OptionalString& specificInstallLocation) const
  {
    openstudio::OptionalTimeSeries result;
    OptionalSqlFile sqlFile = model().sqlFile();

    if (sqlFile){
      ModelObject object = getObject<ModelObject>();

      // get name using specific install location passed in
      std::string name;
      if (specificInstallLocation){
        name = Meter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), boost::to_upper_copy(*specificInstallLocation));
      }else{
        name = Meter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), specificInstallLocation);
      }

      std::string frequency = this->reportingFrequency();
      if (openstudio::istringEqual(frequency, "RunPeriod")){
          frequency = "Run Period";
      }else if (openstudio::istringEqual(frequency, "Timestep")){
          frequency = "Zone Timestep";
      }else if (openstudio::istringEqual(frequency, "Detailed")){
          frequency = "HVAC System Timestep"; 
	  }

      // currently the key value is not associated with the meter, it is part of the name
      result = sqlFile->timeSeries(envPeriod, frequency, name, "");

      if (!result){
        LOG(Debug, "Query for envPeriod = '" << envPeriod <<
                   "', frequency = '" << frequency <<
                   "', name = '" << name <<
                   "', keyValue = '' did not return a TimeSeries");

      }
    }

    return result;
  }

} // detail

Meter::Meter(const Model& model)
  : ModelObject(Meter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Meter_Impl>());
}

IddObjectType Meter::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Meter);
  return result;
}

boost::regex Meter::meterRegex()
{
  // DLM: Must put more specific terms, e.g. HeatingCoils, before less specific terms, e.g. Heating
  const static boost::regex result("^(.*?)?:?(InteriorLights|ExteriorLights|InteriorEquipment|ExteriorEquipment|Fans|Pumps|HeatingCoils|Heating|CoolingCoils|Cooling|HeatRejection|Humidifier|HeatRecoveryForCooling|HeatRecoveryForHeating|HeatRecovery|WaterSystems|Cogeneration|Refrigeration|Chillers|Boilers|Baseboard)?:?(Electricity|Gasoline|Gas|Diesel|Coal|FuelOil_1|FuelOil_2|Propane|Water|Steam|DistrictCooling|DistrictHeating|EnergyTransfer)?:?(Facility|Building|HVAC|Zone|System|Plant)?:?([^:]*?)?$");
  return result;
}

std::string Meter::getName(const boost::optional<std::string>& specificEndUseType,
                           const boost::optional<EndUseType>& endUseType,
                           const boost::optional<FuelType>& fuelType,
                           const boost::optional<InstallLocationType>& installLocationType,
                           const boost::optional<std::string>& specificInstallLocation)
{
  std::string result;

  if (specificEndUseType && !specificEndUseType->empty()){
    result += *specificEndUseType;
  }
  if (endUseType){
    if (!result.empty()){
      result += ":";
    }
    result += EndUseType(*endUseType).valueName();
  }
  if (fuelType){
    if (!result.empty()){
      result += ":";
    }
    result += FuelType(*fuelType).valueName();
  }
  if (installLocationType){
    // there is a weird corner case to handle 'InteriorLights:Electricity:Facility' -> 'InteriorLights:Electricity'
    // this might not catch all cases
    if (installLocationType.get() != InstallLocationType::Facility || !endUseType){
      if (!result.empty()){
        result += ":";
      }
      result += InstallLocationType(*installLocationType).valueName();
    }
  }
  if (specificInstallLocation && !specificInstallLocation->empty()){
    if (!result.empty()){
      result += ":";
    }
    result += *specificInstallLocation;
  }
  LOG(Debug, "getName result = '" << result << "'");
  return result;
}

std::string Meter::name() const {
  return getImpl<detail::Meter_Impl>()->name();
}

std::string Meter::reportingFrequency() const {
  return getImpl<detail::Meter_Impl>()->reportingFrequency();
}

bool Meter::isReportingFrequencyDefaulted() const {
  return getImpl<detail::Meter_Impl>()->isReportingFrequencyDefaulted();
}

bool Meter::meterFileOnly() const {
  return getImpl<detail::Meter_Impl>()->meterFileOnly();
}

bool Meter::isMeterFileOnlyDefaulted() const {
  return getImpl<detail::Meter_Impl>()->isMeterFileOnlyDefaulted();
}

bool Meter::cumulative() const {
  return getImpl<detail::Meter_Impl>()->cumulative();
}

bool Meter::isCumulativeDefaulted() const {
  return getImpl<detail::Meter_Impl>()->isCumulativeDefaulted();
}

boost::optional<std::string> Meter::specificEndUse() const
{
  return getImpl<detail::Meter_Impl>()->specificEndUse();
}

boost::optional<EndUseType> Meter::endUseType() const
{
  return getImpl<detail::Meter_Impl>()->endUseType();
}

boost::optional<FuelType> Meter::fuelType() const
{
  return getImpl<detail::Meter_Impl>()->fuelType();
}

boost::optional<InstallLocationType> Meter::installLocationType() const
{
  return getImpl<detail::Meter_Impl>()->installLocationType();
}

boost::optional<std::string> Meter::specificInstallLocation() const
{
  return getImpl<detail::Meter_Impl>()->specificInstallLocation();
}

bool Meter::setName(std::string name) {
  return getImpl<detail::Meter_Impl>()->setName(name);
}

bool Meter::setReportingFrequency(const std::string& reportingFrequency) {
  return getImpl<detail::Meter_Impl>()->setReportingFrequency(reportingFrequency);
}

void Meter::resetReportingFrequency() {
  getImpl<detail::Meter_Impl>()->resetReportingFrequency();
}

void Meter::setMeterFileOnly(bool meterFileOnly) {
  getImpl<detail::Meter_Impl>()->setMeterFileOnly(meterFileOnly);
}

void Meter::resetMeterFileOnly() {
  getImpl<detail::Meter_Impl>()->resetMeterFileOnly();
}

void Meter::setCumulative(bool cumulative) {
  getImpl<detail::Meter_Impl>()->setCumulative(cumulative);
}

void Meter::resetCumulative() {
  getImpl<detail::Meter_Impl>()->resetCumulative();
}

bool Meter::setSpecificEndUse(const std::string& endUse)
{
  return getImpl<detail::Meter_Impl>()->setSpecificEndUse(endUse);
}

bool Meter::resetSpecificEndUse()
{
  return getImpl<detail::Meter_Impl>()->resetSpecificEndUse();
}

bool Meter::setEndUseType(EndUseType type)
{
  return getImpl<detail::Meter_Impl>()->setEndUseType(type);
}

bool Meter::resetEndUseType()
{
  return getImpl<detail::Meter_Impl>()->resetEndUseType();
}

bool Meter::setFuelType(FuelType type)
{
  return getImpl<detail::Meter_Impl>()->setFuelType(type);
}

bool Meter::resetFuelType()
{
  return getImpl<detail::Meter_Impl>()->resetFuelType();
}

bool Meter::setInstallLocationType(InstallLocationType type)
{
  return getImpl<detail::Meter_Impl>()->setInstallLocationType(type);
}

bool Meter::resetInstallLocationType()
{
  return getImpl<detail::Meter_Impl>()->resetInstallLocationType();
}

bool Meter::setSpecificInstallLocation(const std::string& locationName)
{
  return getImpl<detail::Meter_Impl>()->setSpecificInstallLocation(locationName);
}

bool Meter::resetSpecificInstallLocation()
{
  return getImpl<detail::Meter_Impl>()->resetSpecificInstallLocation();
}

openstudio::OptionalTimeSeries Meter::getData(const std::string& envPeriod) const
{
  return getImpl<detail::Meter_Impl>()->getData(envPeriod);
}

openstudio::OptionalTimeSeries Meter::getData(const std::string& envPeriod, const OptionalString& specificInstallLocation) const
{
  return getImpl<detail::Meter_Impl>()->getData(envPeriod, specificInstallLocation);
}

bool MeterFuelTypeEquals(const Meter& meter,const FuelType& ft) {
  OptionalFuelType oft = meter.fuelType();
  if (oft && (oft.get() == ft)) { return true; }
  return false;
}

/// @cond
Meter::Meter(boost::shared_ptr<detail::Meter_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond


} // model
} // openstudio

