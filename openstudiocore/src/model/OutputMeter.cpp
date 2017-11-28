/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"

#include "Model.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/data/DataEnums.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/sql/SqlFileEnums.hpp"
#include "../utilities/core/Assert.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model {

namespace detail {

  OutputMeter_Impl::OutputMeter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutputMeter::iddObjectType());
  }

  OutputMeter_Impl::OutputMeter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutputMeter::iddObjectType());
  }

  OutputMeter_Impl::OutputMeter_Impl(const OutputMeter_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& OutputMeter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutputMeter_Impl::iddObjectType() const {
    return OutputMeter::iddObjectType();
  }

  boost::optional<ParentObject> OutputMeter_Impl::parent() const
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

  std::vector<openstudio::IdfObject> OutputMeter_Impl::remove() {
    //Note: Cant do /object-list implementation for EMS Sensor since Auto Naming of Objects causes issues.
    //      Instead, doing an /alpha getString implementation so we need to manually remove any referring Sensors
    const Model m = this->model();

    std::vector<EnergyManagementSystemSensor> objects = m.getConcreteModelObjects<EnergyManagementSystemSensor>();
    for (auto & sensor : objects) {
      if (sensor.outputMeter()) {
        if (sensor.outputMeter().get().name() == this->name()) {
          sensor.remove();
        }
      }
    }
    return ModelObject_Impl::remove();
  }

  std::string OutputMeter_Impl::name() const {
    boost::optional<std::string> value = getString(OS_Output_MeterFields::Name,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string OutputMeter_Impl::reportingFrequency() const {
    boost::optional<std::string> value = getString(OS_Output_MeterFields::ReportingFrequency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool OutputMeter_Impl::isReportingFrequencyDefaulted() const {
    return isEmpty(OS_Output_MeterFields::ReportingFrequency);
  }

  bool OutputMeter_Impl::meterFileOnly() const {
    boost::optional<std::string> value = getString(OS_Output_MeterFields::MeterFileOnly,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool OutputMeter_Impl::isMeterFileOnlyDefaulted() const {
    return isEmpty(OS_Output_MeterFields::MeterFileOnly);
  }

  bool OutputMeter_Impl::cumulative() const {
    boost::optional<std::string> value = getString(OS_Output_MeterFields::Cumulative,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "True");
  }

  bool OutputMeter_Impl::isCumulativeDefaulted() const {
    return isEmpty(OS_Output_MeterFields::Cumulative);
  }

  boost::optional<std::string> OutputMeter_Impl::specificEndUse() const
  {
    boost::optional<std::string> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, OutputMeter::meterRegex()))
    {
      // match is always true but may be empty
      if (matches[1].first != matches[1].second){
        result = std::string(matches[1].first, matches[1].second);
      }
    }

    return result;
  }

  boost::optional<EndUseType> OutputMeter_Impl::endUseType() const
  {
    boost::optional<EndUseType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, OutputMeter::meterRegex()))
    {
      if (matches[2].matched){
        result = EndUseType(std::string(matches[2].first, matches[2].second));
      }
    }

    return result;
  }

  boost::optional<FuelType> OutputMeter_Impl::fuelType() const
  {
    boost::optional<FuelType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, OutputMeter::meterRegex()))
    {
      if (matches[3].matched){
        result = FuelType(std::string(matches[3].first, matches[3].second));
      }
    }

    return result;
  }

  boost::optional<InstallLocationType> OutputMeter_Impl::installLocationType() const
  {
    boost::optional<InstallLocationType> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, OutputMeter::meterRegex()))
    {
      if (matches[4].matched){
        result = InstallLocationType(std::string(matches[4].first, matches[4].second));
      }
    }

    return result;
  }

  boost::optional<std::string> OutputMeter_Impl::specificInstallLocation() const
  {
    boost::optional<std::string> result;
    std::string myName = name();
    boost::smatch matches;
    if (boost::regex_search(myName, matches, OutputMeter::meterRegex()))
    {
      // match is always true but may be empty
      if (matches[5].first != matches[5].second){
        result = std::string(matches[5].first, matches[5].second);
      }
    }

    return result;
  }

  bool OutputMeter_Impl::setName(std::string name) {
    return setString(OS_Output_MeterFields::Name, name);
  }

  bool OutputMeter_Impl::setReportingFrequency(const std::string& reportingFrequency) {
    return setString(OS_Output_MeterFields::ReportingFrequency, reportingFrequency);
  }

  void OutputMeter_Impl::resetReportingFrequency() {
    bool result = setString(OS_Output_MeterFields::ReportingFrequency, "");
    OS_ASSERT(result);
  }

  void OutputMeter_Impl::setMeterFileOnly(bool meterFileOnly) {
    bool result = false;
    if (meterFileOnly) {
      result = setString(OS_Output_MeterFields::MeterFileOnly, "True");
    } else {
      result = setString(OS_Output_MeterFields::MeterFileOnly, "False");
    }
    OS_ASSERT(result);
  }

  void OutputMeter_Impl::resetMeterFileOnly() {
    bool result = setString(OS_Output_MeterFields::MeterFileOnly, "");
    OS_ASSERT(result);
  }

  void OutputMeter_Impl::setCumulative(bool cumulative) {
    bool result = false;
    if (cumulative) {
      result = setString(OS_Output_MeterFields::Cumulative, "True");
    } else {
      result = setString(OS_Output_MeterFields::Cumulative, "False");
    }
    OS_ASSERT(result);
  }

  void OutputMeter_Impl::resetCumulative() {
    bool result = setString(OS_Output_MeterFields::Cumulative, "");
    OS_ASSERT(result);
  }

  bool OutputMeter_Impl::setSpecificEndUse(const std::string& endUse)
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(endUse, endUseType(), fuelType(), installLocationType(), specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::resetSpecificEndUse()
  {
    return setSpecificEndUse("");
  }

  bool OutputMeter_Impl::setEndUseType(EndUseType type)
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), type, fuelType(), installLocationType(), specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::resetEndUseType()
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), boost::none, fuelType(), installLocationType(), specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::setFuelType(FuelType type)
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), endUseType(), type, installLocationType(), specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::resetFuelType()
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), endUseType(), boost::none, installLocationType(), specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::setInstallLocationType(InstallLocationType type)
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), endUseType(), fuelType(), type, specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::resetInstallLocationType()
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), endUseType(), fuelType(), boost::none, specificInstallLocation());

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::setSpecificInstallLocation(const std::string& locationName)
  {
    ModelObject object = getObject<ModelObject>();

    std::string name = OutputMeter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), locationName);

    bool result = object.setString(OS_Output_MeterFields::Name, name);
    if (!result){
      LOG(Error, "Could not set name to '" << name << "'");
    }

    return result;
  }

  bool OutputMeter_Impl::resetSpecificInstallLocation()
  {
    return setSpecificInstallLocation("");
  }

  openstudio::OptionalTimeSeries OutputMeter_Impl::getData(const std::string& envPeriod) const
  {
    return getData(envPeriod, specificInstallLocation());
  }

  openstudio::OptionalTimeSeries OutputMeter_Impl::getData(const std::string& envPeriod, const OptionalString& specificInstallLocation) const
  {
    openstudio::OptionalTimeSeries result;
    OptionalSqlFile sqlFile = model().sqlFile();

    if (sqlFile){
      ModelObject object = getObject<ModelObject>();

      // get name using specific install location passed in
      std::string name;
      if (specificInstallLocation){
        name = OutputMeter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), boost::to_upper_copy(*specificInstallLocation));
      }else{
        name = OutputMeter::getName(specificEndUse(), endUseType(), fuelType(), installLocationType(), specificInstallLocation);
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

OutputMeter::OutputMeter(const Model& model)
  : ModelObject(OutputMeter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::OutputMeter_Impl>());
}

IddObjectType OutputMeter::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Output_Meter);
  return result;
}

boost::regex OutputMeter::meterRegex()
{
  // DLM: Must put more specific terms, e.g. HeatingCoils, before less specific terms, e.g. Heating
  const static boost::regex result("^(.*?)?:?(InteriorLights|ExteriorLights|InteriorEquipment|ExteriorEquipment|Fans|Pumps|HeatingCoils|Heating|CoolingCoils|Cooling|HeatRejection|Humidifier|HeatRecoveryForCooling|HeatRecoveryForHeating|HeatRecovery|WaterSystems|Cogeneration|Refrigeration|Chillers|Boilers|Baseboard)?:?(Electricity|Gasoline|Gas|Diesel|Coal|FuelOil_1|FuelOil_2|Propane|Water|Steam|DistrictCooling|DistrictHeating|EnergyTransfer)?:?(Facility|Building|HVAC|Zone|System|Plant)?:?([^:]*?)?$");
  return result;
}

std::string OutputMeter::getName(const boost::optional<std::string>& specificEndUseType,
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

std::string OutputMeter::name() const {
  return getImpl<detail::OutputMeter_Impl>()->name();
}

std::string OutputMeter::reportingFrequency() const {
  return getImpl<detail::OutputMeter_Impl>()->reportingFrequency();
}

bool OutputMeter::isReportingFrequencyDefaulted() const {
  return getImpl<detail::OutputMeter_Impl>()->isReportingFrequencyDefaulted();
}

bool OutputMeter::meterFileOnly() const {
  return getImpl<detail::OutputMeter_Impl>()->meterFileOnly();
}

bool OutputMeter::isMeterFileOnlyDefaulted() const {
  return getImpl<detail::OutputMeter_Impl>()->isMeterFileOnlyDefaulted();
}

bool OutputMeter::cumulative() const {
  return getImpl<detail::OutputMeter_Impl>()->cumulative();
}

bool OutputMeter::isCumulativeDefaulted() const {
  return getImpl<detail::OutputMeter_Impl>()->isCumulativeDefaulted();
}

boost::optional<std::string> OutputMeter::specificEndUse() const
{
  return getImpl<detail::OutputMeter_Impl>()->specificEndUse();
}

boost::optional<EndUseType> OutputMeter::endUseType() const
{
  return getImpl<detail::OutputMeter_Impl>()->endUseType();
}

boost::optional<FuelType> OutputMeter::fuelType() const
{
  return getImpl<detail::OutputMeter_Impl>()->fuelType();
}

boost::optional<InstallLocationType> OutputMeter::installLocationType() const
{
  return getImpl<detail::OutputMeter_Impl>()->installLocationType();
}

boost::optional<std::string> OutputMeter::specificInstallLocation() const
{
  return getImpl<detail::OutputMeter_Impl>()->specificInstallLocation();
}

bool OutputMeter::setName(std::string name) {
  return getImpl<detail::OutputMeter_Impl>()->setName(name);
}

bool OutputMeter::setReportingFrequency(const std::string& reportingFrequency) {
  return getImpl<detail::OutputMeter_Impl>()->setReportingFrequency(reportingFrequency);
}

void OutputMeter::resetReportingFrequency() {
  getImpl<detail::OutputMeter_Impl>()->resetReportingFrequency();
}

void OutputMeter::setMeterFileOnly(bool meterFileOnly) {
  getImpl<detail::OutputMeter_Impl>()->setMeterFileOnly(meterFileOnly);
}

void OutputMeter::resetMeterFileOnly() {
  getImpl<detail::OutputMeter_Impl>()->resetMeterFileOnly();
}

void OutputMeter::setCumulative(bool cumulative) {
  getImpl<detail::OutputMeter_Impl>()->setCumulative(cumulative);
}

void OutputMeter::resetCumulative() {
  getImpl<detail::OutputMeter_Impl>()->resetCumulative();
}

bool OutputMeter::setSpecificEndUse(const std::string& endUse)
{
  return getImpl<detail::OutputMeter_Impl>()->setSpecificEndUse(endUse);
}

bool OutputMeter::resetSpecificEndUse()
{
  return getImpl<detail::OutputMeter_Impl>()->resetSpecificEndUse();
}

bool OutputMeter::setEndUseType(EndUseType type)
{
  return getImpl<detail::OutputMeter_Impl>()->setEndUseType(type);
}

bool OutputMeter::resetEndUseType()
{
  return getImpl<detail::OutputMeter_Impl>()->resetEndUseType();
}

bool OutputMeter::setFuelType(FuelType type)
{
  return getImpl<detail::OutputMeter_Impl>()->setFuelType(type);
}

bool OutputMeter::resetFuelType()
{
  return getImpl<detail::OutputMeter_Impl>()->resetFuelType();
}

bool OutputMeter::setInstallLocationType(InstallLocationType type)
{
  return getImpl<detail::OutputMeter_Impl>()->setInstallLocationType(type);
}

bool OutputMeter::resetInstallLocationType()
{
  return getImpl<detail::OutputMeter_Impl>()->resetInstallLocationType();
}

bool OutputMeter::setSpecificInstallLocation(const std::string& locationName)
{
  return getImpl<detail::OutputMeter_Impl>()->setSpecificInstallLocation(locationName);
}

bool OutputMeter::resetSpecificInstallLocation()
{
  return getImpl<detail::OutputMeter_Impl>()->resetSpecificInstallLocation();
}

openstudio::OptionalTimeSeries OutputMeter::getData(const std::string& envPeriod) const
{
  return getImpl<detail::OutputMeter_Impl>()->getData(envPeriod);
}

openstudio::OptionalTimeSeries OutputMeter::getData(const std::string& envPeriod, const OptionalString& specificInstallLocation) const
{
  return getImpl<detail::OutputMeter_Impl>()->getData(envPeriod, specificInstallLocation);
}

std::vector<IdfObject> OutputMeter::remove() {
  return getImpl<detail::OutputMeter_Impl>()->remove();
}

bool MeterFuelTypeEquals(const OutputMeter& meter,const FuelType& ft) {
  OptionalFuelType oft = meter.fuelType();
  if (oft && (oft.get() == ft)) { return true; }
  return false;
}

/// @cond
OutputMeter::OutputMeter(std::shared_ptr<detail::OutputMeter_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

