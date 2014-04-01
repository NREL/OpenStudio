/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>

#include <model/Model.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/SimulationControl.hpp>
#include <model/SimulationControl_Impl.hpp>
#include <model/Meter.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/URLHelpers.hpp>

namespace openstudio {
namespace model {

namespace detail {

  TimeDependentValuation_Impl::TimeDependentValuation_Impl(const IdfObject& idfObject, 
                                                           Model_Impl* model, 
                                                           bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == TimeDependentValuation::iddObjectType());
  }

  TimeDependentValuation_Impl::TimeDependentValuation_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == TimeDependentValuation::iddObjectType());
  }

  TimeDependentValuation_Impl::TimeDependentValuation_Impl(
      const TimeDependentValuation_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> TimeDependentValuation_Impl::parent() const {
    boost::optional<ParentObject> result(this->site());
    return result;
  }

  const std::vector<std::string>& TimeDependentValuation_Impl::outputVariableNames() const {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType TimeDependentValuation_Impl::iddObjectType() const {
    return TimeDependentValuation::iddObjectType();
  }

  boost::optional<TimeDependentValuationFile> TimeDependentValuation_Impl::file(const openstudio::path& dir) const {
    if (!m_file) {

      // get current path
      boost::optional<openstudio::path> currentPath = this->path();
      if (currentPath){
  
        // try to load absolute path
        if (currentPath->is_complete() && boost::filesystem::exists(*currentPath)) {
          m_file = TimeDependentValuationFile::load(*currentPath);

          // loading absolute path failed, try as relative path
          if (!m_file){
            currentPath = currentPath->filename();
          }
        }

        // try relative path
        if (!m_file){
          if (!dir.empty()){
            openstudio::path newPath = boost::filesystem::complete(*currentPath, dir);
            if (boost::filesystem::exists(newPath)) { 
              m_file = TimeDependentValuationFile::load(*currentPath);
            }
          }
        }
      }
    }
    return m_file;
  }

  boost::optional<TimeDependentValuationFile> TimeDependentValuation_Impl::fileInSIUnits() const {
    if (!m_fileInSIUnits) {
      OptionalTimeDependentValuationFile oTdvFile = file();
      if (oTdvFile) {
        LOG(Debug,"Converting TimeDependentValuationFile to SI Units.");
        m_fileInSIUnits = oTdvFile->convertUnits();
      }
    }
    return m_fileInSIUnits;
  }

  boost::optional<Site> TimeDependentValuation_Impl::site() const {
    return this->model().getOptionalUniqueModelObject<Site>();
  }

  boost::optional<std::string> TimeDependentValuation_Impl::url() const {
    return getString(OS_TimeDependentValuationFields::Url, true, true);
  }

  boost::optional<openstudio::path> TimeDependentValuation_Impl::path() const {
    boost::optional<openstudio::path> result;
    OptionalString oUrlStr = this->url();
    if (oUrlStr) { 
      result = toPath(QUrl(toQString(*oUrlStr)).toLocalFile()); 
    }
    return result;
  }

  BuildingSector TimeDependentValuation_Impl::activeBuildingSector() const {
    OptionalString os = getString(OS_TimeDependentValuationFields::ActiveBuildingSector,true);
    OS_ASSERT(os);
    BuildingSector result(*os); // can throw if keys and enum get out of sync.
    return result;
  }

  std::vector<FuelType> TimeDependentValuation_Impl::availableFuelTypes() const {
    FuelTypeVector result;
    IdfExtensibleGroupVector egs = extensibleGroups();
    BOOST_FOREACH(const IdfExtensibleGroup& eg,egs) {
      OptionalString os = eg.getString(OS_TimeDependentValuationExtensibleFields::AvailableFuelType,true);
      OS_ASSERT(os);
      FuelType candidate(*os); // can throw if keys and enum get out of sync.
      result.push_back(candidate);
    }
    return result;
  }

  bool TimeDependentValuation_Impl::setParent(ParentObject& newParent) {
    if (newParent.optionalCast<Site>() && (newParent.model() == model())){
      return true;
    }
    return false;
  }

  bool TimeDependentValuation_Impl::makeUrlRelative(const openstudio::path& basePath) {
    boost::optional<openstudio::path> currentPath = this->path();
    if (currentPath){
      openstudio::path newPath;
      if (basePath.empty()) { 
        newPath = currentPath->filename();
      } else {
        newPath = relativePath(*currentPath,basePath);
      }
      return setString(OS_TimeDependentValuationFields::Url,toString(toURL(newPath)));
    }
    return false;
  }

  bool TimeDependentValuation_Impl::makeUrlAbsolute(const openstudio::path& searchDirectory) {
    boost::optional<openstudio::path> currentPath = this->path();
    if (currentPath){
      if (currentPath->is_complete() && boost::filesystem::exists(*currentPath)) {
        return true;
      }
      openstudio::path workingPath(*currentPath);
      // if currentPath is complete but does not exist, go to extreme measures
      if (currentPath->is_complete()) {
        workingPath = currentPath->filename();
      }
      openstudio::path newPath = boost::filesystem::complete(workingPath,searchDirectory);
      if (newPath.empty() || !boost::filesystem::exists(newPath)) { 
        return false; 
      }
      return setString(OS_TimeDependentValuationFields::Url,toString(toURL(newPath)));
    }
    return false;
  }

  void TimeDependentValuation_Impl::setActiveBuildingSector(const BuildingSector& sector) {
    bool ok = setString(OS_TimeDependentValuationFields::ActiveBuildingSector,sector.valueName());
    OS_ASSERT(ok);
  }

  void TimeDependentValuation_Impl::clearCache() {
    m_file = boost::none;
    m_fileInSIUnits = boost::none;
  }

  boost::optional<double> TimeDependentValuation_Impl::energyTimeDependentValuation(
      const std::string& environmentPeriod) const 
  {
    OptionalDouble result;
    LOG(Debug,"Calculating energyTimeDependentValuation for environmentPeriod " << environmentPeriod 
        << ".");

    // need TDV file in SI Units
    OptionalTimeDependentValuationFile oTDVFile = fileInSIUnits();
    if (!oTDVFile) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"Unable to compute energyTimeDependentValuation because the time dependent valuation "
           << "file '" << toString(urlString) << "' associated with " << briefDescription() 
           << " could not be loaded and/or converted to SI units.");
      return result;
    }
    TimeDependentValuationFile tdvFile = *oTDVFile;

    // need non-ambiguous identification of annual run
    std::string envPer(environmentPeriod);
    if (envPer.empty()) {
      StringVector annualSimulationEnvironmentNames = model().getUniqueModelObject<SimulationControl>().annualSimulationEnvironmentPeriods();
      if (annualSimulationEnvironmentNames.size() == 1u) {
        envPer = annualSimulationEnvironmentNames[0];
      }
      else {
        LOG(Info,"Unable to compute energyTimeDependentValuation for " << briefDescription() 
            << ", because an appropriate environment period could not be identified.");
        return result;
      }
    }

    FuelTypeVector fts = availableFuelTypes();
    MeterVector meters = model().getUniqueModelObject<Facility>().meters();
    for (unsigned i = 0, n = fts.size(); i < n; ++i) {
      MeterVector::const_iterator meterIt = std::find_if(meters.begin(),meters.end(),
          boost::bind(MeterFuelTypeEquals,_1,fts[i]));
      if (meterIt == meters.end()) {
        std::string urlString;
        OptionalString oUrlString = this->url();
        if (oUrlString){
          urlString = *oUrlString;
        }
        LOG(Info,"No meter associated with fuel type '" << fts[i].valueName() << "' called out "
            << "in time dependent valuation file '" << urlString << "'.");
        continue;
      }
      OptionalTimeSeries ots = meterIt->getData(envPer);
      if (!ots) {
        std::string urlString;
        OptionalString oUrlString = this->url();
        if (oUrlString){
          urlString = *oUrlString;
        }
        LOG(Info,"No time series data retrieved for " << meterIt->briefDescription() << ". Unable "
            << "to included fuel type '" << fts[i].valueName() << "' in time dependent valuation "
            << "based on file '" << urlString << "'.");
        continue;
      }
      TimeSeries meterTimeSeries = *ots;
      Vector timeSeriesValues = meterTimeSeries.values();
      // check 8760
      if (timeSeriesValues.size() != 8760u) {
        std::string urlString;
        OptionalString oUrlString = this->url();
        if (oUrlString){
          urlString = *oUrlString;
        }
        LOG(Info,"Time series data retrieved for " << meterIt->briefDescription() << " is of "
            << "length " << timeSeriesValues.size() << " rather than 8760. Unable "
            << "to included fuel type '" << fts[i].valueName() << "' in time dependent valuation "
            << "based on file '" << urlString << "'.");
        continue;
      }
      // get TDV column
      DoubleVector temp = mf_tdvValues(tdvFile,fts[i]);
      Vector tdvValues = createVector(temp);
      OS_ASSERT(tdvValues.size() == 8760u);

      // dot product 
      double thisTdvContribution = dot(timeSeriesValues,tdvValues);
      if (result) { result = (*result) + thisTdvContribution; }
      else { result = thisTdvContribution; }

    }

    return result;
  }

  boost::optional<double> TimeDependentValuation_Impl::costTimeDependentValuation(
      const std::string& environmentPeriod) const 
  {
    OptionalDouble result;
    LOG(Debug,"Calculating costTimeDependentValuation for environmentPeriod " << environmentPeriod 
        << ".");    
    
    // need TDV file
    OptionalTimeDependentValuationFile oTDVFile = fileInSIUnits();
    if (!oTDVFile) {
        std::string urlString;
        OptionalString oUrlString = this->url();
        if (oUrlString){
          urlString = *oUrlString;
        }
      LOG(Info,"Unable to compute costTimeDependentValuation because the time dependent valuation "
          << "file '" << urlString << "' associated with " << briefDescription() 
          << " could not be loaded and/or converted to SI units.");
      return result;
    }
    TimeDependentValuationFile tdvFile = *oTDVFile;

    // need cost of energy
    OptionalDouble oCostPerJ;
    BuildingSector bs = activeBuildingSector();
    if (bs == BuildingSector::Commercial) {
      oCostPerJ = tdvFile.nominalCommercialCostOfEnergy();
    }
    else if (bs == BuildingSector::Residential) {
      oCostPerJ = tdvFile.nominalResidentialCostOfEnergy();
    }
    else { OS_ASSERT(false); }
    if (!oCostPerJ) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"Unable to compute costTimeDependentValuation because the nominal cost of energy "
          << "could not be extracted from the time dependent valuation file '" << urlString 
          << "' associated with " << briefDescription() << ".");
      return result;
    }

    // apply to energy TDV
    OptionalDouble energyTDV = energyTimeDependentValuation(environmentPeriod);
    if (energyTDV) {
      result = (*oCostPerJ) * (*energyTDV);
    }

    return result;
  }

  boost::optional<double> TimeDependentValuation_Impl::getEnergyTimeDependentValuation(
      const FuelType& fuelType,const std::string& environmentPeriod) const
  {
    OptionalDouble result;
    LOG(Debug,"Calculating energyTimeDependentValuation for fuelType " << fuelType.valueName() 
        << " and environmentPeriod " << environmentPeriod << ".");
    
    // need TDV file
    OptionalTimeDependentValuationFile oTDVFile = fileInSIUnits();
    if (!oTDVFile) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
       LOG(Info,"Unable to compute energyTimeDependentValuation for FuelType '" 
           << fuelType.valueName() << "', because the time dependent valuation "
           << "file '" << urlString << "' associated with " << briefDescription() 
           << " could not be loaded and/or converted to SI units.");
       return result;
    }
    TimeDependentValuationFile tdvFile = *oTDVFile;

    // need non-ambiguous identification of annual run
    std::string envPer(environmentPeriod);
    if (envPer.empty()) {
      StringVector annualSimulationEnvironmentNames = model().getUniqueModelObject<SimulationControl>().annualSimulationEnvironmentPeriods();
      if (annualSimulationEnvironmentNames.size() == 1u) {
        envPer = annualSimulationEnvironmentNames[0];
      }
      else {
        LOG(Info,"Unable to compute energyTimeDependentValuation for " << briefDescription() 
            << ", because an appropriate environment period could not be identified.");
        return result;
      }
    }

    // check that FuelType is available
    FuelTypeVector fts = availableFuelTypes();
    if (std::find(fts.begin(),fts.end(),fuelType) == fts.end()) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"No time dependent valuation information available for '" << fuelType.valueName() 
          << "' in file '" << urlString << "'.");
      return result;
    }

    // find meter
    MeterVector meters = model().getUniqueModelObject<Facility>().meters();
    MeterVector::const_iterator meterIt = std::find_if(meters.begin(),meters.end(),
        boost::bind(MeterFuelTypeEquals,_1,fuelType));
    if (meterIt == meters.end()) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"No meter associated with fuel type '" << fuelType.valueName() << "' called out "
          << "in time dependent valuation file '" << urlString << "'.");
      return result;
    }
    Meter meter = *meterIt;

    OptionalTimeSeries ots = meter.getData(envPer);
    if (!ots) {
      LOG(Info,"No time series data retrieved for " << meter.briefDescription() << ". Unable "
          << "to calculate time dependent valuation for fuel type '" << fuelType.valueName() << "'.");
      return result;
    }
    TimeSeries meterTimeSeries = *ots;
    Vector timeSeriesValues = meterTimeSeries.values();
    // check 8760
    if (timeSeriesValues.size() != 8760u) {
      LOG(Info,"Time series data retrieved for " << meter.briefDescription() << " is of "
          << "length " << timeSeriesValues.size() << " rather than 8760. Unable "
          << "to calculate time dependent valuation for fuel type '" << fuelType.valueName() << "'.");
      return result;
    }
    // get TDV column
    DoubleVector temp = mf_tdvValues(tdvFile,fuelType);
    Vector tdvValues = createVector(temp);
    OS_ASSERT(tdvValues.size() == 8760u);

    // dot product 
    result = dot(timeSeriesValues,tdvValues);

    return result;
  }

  boost::optional<double> TimeDependentValuation_Impl::getCostTimeDependentValuation(
      const FuelType& fuelType,const std::string& environmentPeriod) const
  {
    OptionalDouble result;
    LOG(Debug,"Calculating costTimeDependentValuation for fuelType " << fuelType.valueName() 
        << " and environmentPeriod " << environmentPeriod << ".");    
    
    // need TDV file
    OptionalTimeDependentValuationFile oTDVFile = fileInSIUnits();
    if (!oTDVFile) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"Unable to compute costTimeDependentValuation for FuelType '" 
         << fuelType.valueName() << "', because the time dependent valuation "
         << "file '" << urlString << "' associated with " << briefDescription() 
         << " could not be loaded and/or converted to SI units.");
      return result;
    }
    TimeDependentValuationFile tdvFile = *oTDVFile;

    // need cost of energy
    OptionalDouble oCostPerJ;
    BuildingSector bs = activeBuildingSector();
    if (bs == BuildingSector::Commercial) {
      oCostPerJ = tdvFile.nominalCommercialCostOfEnergy();
    }
    else if (bs == BuildingSector::Residential) {
      oCostPerJ = tdvFile.nominalResidentialCostOfEnergy();
    }
    else { OS_ASSERT(false); }
    if (!oCostPerJ) {
      std::string urlString;
      OptionalString oUrlString = this->url();
      if (oUrlString){
        urlString = *oUrlString;
      }
      LOG(Info,"Unable to compute costTimeDependentValuation for '" << fuelType.valueName() 
          << "' because the nominal cost of energy could not be extracted from the time "
          << "dependent valuation file '" << urlString << "' associated with " 
          << briefDescription() << ".");
      return result;
    }

    // apply to energy TDV
    OptionalDouble energyTDV = getEnergyTimeDependentValuation(fuelType,environmentPeriod);
    if (energyTDV) {
      result = (*oCostPerJ) * (*energyTDV);
    }

    return result;
  }

  std::vector<double> TimeDependentValuation_Impl::mf_tdvValues(
      const TimeDependentValuationFile& tdvFile,const FuelType& ft) const
  {
    DoubleVector result;
    OS_ASSERT(tdvFile.system() == UnitSystem::SI);
    IdfExtensibleGroupVector egs = extensibleGroups();
    BOOST_FOREACH(const IdfExtensibleGroup& eg,egs) {
      OptionalString os = eg.getString(OS_TimeDependentValuationExtensibleFields::AvailableFuelType,true);
      OS_ASSERT(os);
      FuelType candidate(*os); // can throw if keys and enum get out of sync.
      if (candidate == ft) {
        BuildingSector bs = activeBuildingSector();
        if (bs == BuildingSector::Commercial) {
          OptionalUnsigned ou = eg.getUnsigned(OS_TimeDependentValuationExtensibleFields::CommercialColumnIndexForFuelType);
          OS_ASSERT(ou);
          result = tdvFile.values(*ou);
        }
        else if (bs == BuildingSector::Residential) {
          OptionalUnsigned ou = eg.getUnsigned(OS_TimeDependentValuationExtensibleFields::ResidentialColumnIndexForFuelType);
          OS_ASSERT(ou);
          result = tdvFile.values(*ou);
        }
        else {
          OS_ASSERT(false);
        }
      }
    }
    return result;
  }

} // detail

TimeDependentValuation TimeDependentValuation::setTimeDependentValuation(
    Model& model,const TimeDependentValuationFile& tdvFile) 
{
  LOG(Debug,"Setting the TDV file.");
  TimeDependentValuation result = model.getUniqueModelObject<TimeDependentValuation>();
  result.getImpl<detail::TimeDependentValuation_Impl>()->clearCache(); // clear the cache
  result.setName(tdvFile.name());
  result.setString(OS_TimeDependentValuationFields::Url,toString(toURL(tdvFile.path())));
  result.setString(OS_TimeDependentValuationFields::Checksum,tdvFile.checksum());  

  // get available fuel types
  LOG(Debug,"Extracting fuel types.");
  boost::optional<IdfExtensibleGroup> oeg;
  FuelTypeVector fuelTypes;
  StringVector newFuelTypeGroup(3);
  result.clearExtensibleGroups();
  Facility facility = model.getUniqueModelObject<Facility>();
  for (unsigned i = 0, n = tdvFile.numColumns(); i < n; ++i) {
    FuelType thisFuelType = tdvFile.fuelType(i);
    FuelTypeVector::const_iterator it = std::find(fuelTypes.begin(),fuelTypes.end(),thisFuelType);
    if (it == fuelTypes.end()) {
      newFuelTypeGroup[0] = thisFuelType.valueDescription();
      oeg = result.pushExtensibleGroup(newFuelTypeGroup);
      fuelTypes.push_back(thisFuelType);
      
      // make sure there is a meter
      OptionalMeter meter = facility.getMeterByFuelType(thisFuelType);
      if (!meter) {
        Meter newMeter(model);
        newMeter.setFuelType(thisFuelType);
        newMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));
      }
    }
    else {
      oeg = result.getExtensibleGroup(int(it - fuelTypes.begin()));
    }
    OS_ASSERT(oeg);
    OS_ASSERT(!oeg->empty());
    BuildingSector thisBuildingSector = tdvFile.buildingSector(i);
    unsigned bsi(0);
    if (thisBuildingSector == BuildingSector::Commercial) {
      bsi = OS_TimeDependentValuationExtensibleFields::CommercialColumnIndexForFuelType;
    }
    else if (thisBuildingSector == BuildingSector::Residential) {
      bsi = OS_TimeDependentValuationExtensibleFields::ResidentialColumnIndexForFuelType;
    }
    else { OS_ASSERT(false); }
    OptionalUnsigned oVal = oeg->getUnsigned(bsi,true);
    if (oVal) {
      LOG(Warn,"Time dependent valuation file at '" << toString(tdvFile.path()) << "' has two "
          << "columns for fuel type '" << thisFuelType.valueName() << "' and building sector '" 
          << thisBuildingSector.valueName() << "'.");
      continue;
    }
    oeg->setUnsigned(bsi,i);
  }
  OS_ASSERT(result.numExtensibleGroups() == fuelTypes.size());
  return result;
}

boost::optional<TimeDependentValuationFile> TimeDependentValuation::file(const openstudio::path& dir) const {
  return getImpl<detail::TimeDependentValuation_Impl>()->file(dir);
}

boost::optional<TimeDependentValuationFile> TimeDependentValuation::fileInSIUnits() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->fileInSIUnits();
}

boost::optional<Site> TimeDependentValuation::site() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->site();
}

boost::optional<std::string> TimeDependentValuation::url() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->url();
}

boost::optional<openstudio::path> TimeDependentValuation::path() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->path();
}

BuildingSector TimeDependentValuation::activeBuildingSector() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->activeBuildingSector();
}

std::vector<FuelType> TimeDependentValuation::availableFuelTypes() const {
  return getImpl<detail::TimeDependentValuation_Impl>()->availableFuelTypes();
}

bool TimeDependentValuation::makeUrlRelative(const openstudio::path& basePath) {
  return getImpl<detail::TimeDependentValuation_Impl>()->makeUrlRelative(basePath);
}

bool TimeDependentValuation::makeUrlAbsolute(const openstudio::path& searchDirectory) {
  return getImpl<detail::TimeDependentValuation_Impl>()->makeUrlAbsolute(searchDirectory);
}

void TimeDependentValuation::setActiveBuildingSector(const BuildingSector& sector) {
  return getImpl<detail::TimeDependentValuation_Impl>()->setActiveBuildingSector(sector);
}

boost::optional<double> TimeDependentValuation::energyTimeDependentValuation(
    const std::string& environmentPeriod) const
{
  return getImpl<detail::TimeDependentValuation_Impl>()->energyTimeDependentValuation(environmentPeriod);
}

boost::optional<double> TimeDependentValuation::costTimeDependentValuation(
    const std::string& environmentPeriod) const
{
  return getImpl<detail::TimeDependentValuation_Impl>()->costTimeDependentValuation(environmentPeriod);
}

boost::optional<double> TimeDependentValuation::getEnergyTimeDependentValuation(
    const FuelType& fuelType,const std::string& environmentPeriod) const
{
  return getImpl<detail::TimeDependentValuation_Impl>()->getEnergyTimeDependentValuation(fuelType,environmentPeriod);
}

boost::optional<double> TimeDependentValuation::getCostTimeDependentValuation(
    const FuelType& fuelType,const std::string& environmentPeriod) const
{
  return getImpl<detail::TimeDependentValuation_Impl>()->getCostTimeDependentValuation(fuelType,environmentPeriod);
}

IddObjectType TimeDependentValuation::iddObjectType() {
  IddObjectType result(IddObjectType::OS_TimeDependentValuation);
  return result;
}

/// @cond
TimeDependentValuation::TimeDependentValuation(const Model& model)
  : ModelObject(TimeDependentValuation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::TimeDependentValuation_Impl>());
}

TimeDependentValuation::TimeDependentValuation(
    boost::shared_ptr<detail::TimeDependentValuation_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
