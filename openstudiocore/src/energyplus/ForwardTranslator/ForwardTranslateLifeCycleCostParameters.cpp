/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/LifeCycleCostParameters.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/time/Date.hpp"

#include <utilities/idd/LifeCycleCost_Parameters_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_UsePriceEscalation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

void makeUsePriceEscalation(boost::optional<double> rate, const std::string& fuelName, const std::string& baseDateMonth, int baseDateYear, int lengthOfStudyPeriodInYears, std::vector<IdfObject>& idfObjects)
{
  if (!rate || rate.get() == 0){
    return;
  }

  IdfObject idfObject(IddObjectType::LifeCycleCost_UsePriceEscalation);
  idfObject.setName(fuelName + " Use Price Escalation");
  idfObject.setString(LifeCycleCost_UsePriceEscalationFields::Resource, fuelName);
  idfObject.setInt(LifeCycleCost_UsePriceEscalationFields::EscalationStartYear, baseDateYear);
  idfObject.setString(LifeCycleCost_UsePriceEscalationFields::EscalationStartMonth, baseDateMonth);

  for (int i = 0; i < lengthOfStudyPeriodInYears; ++i){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    double value = pow(1.0+rate.get(), i);
    group.setDouble(LifeCycleCost_UsePriceEscalationExtensibleFields::YearEscalation, value);
  }

  idfObjects.push_back(idfObject);
}

boost::optional<IdfObject> ForwardTranslator::translateLifeCycleCostParameters( model::LifeCycleCostParameters & modelObject )
{
  // create, register, and name object
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::LifeCycleCost_Parameters,
                                                       modelObject);

  idfObject.setString(LifeCycleCost_ParametersFields::DiscountingConvention, modelObject.discountingConvention());
  
  idfObject.setString(LifeCycleCost_ParametersFields::InflationApproach, modelObject.inflationApproach());

  boost::optional<double> d = modelObject.realDiscountRate();
  if (d){
    idfObject.setDouble(LifeCycleCost_ParametersFields::RealDiscountRate, *d);
  }

  d = modelObject.nominalDiscountRate();
  if (d){
    idfObject.setDouble(LifeCycleCost_ParametersFields::NominalDiscountRate, *d);
  }

  d = modelObject.inflation();
  if (d){
    idfObject.setDouble(LifeCycleCost_ParametersFields::Inflation, *d);
  }

  if (!modelObject.isBaseDateMonthDefaulted()){
    idfObject.setString(LifeCycleCost_ParametersFields::BaseDateMonth, modelObject.baseDateMonth().valueDescription());
  }

  idfObject.setInt(LifeCycleCost_ParametersFields::BaseDateYear, modelObject.baseDateYear());

  if (!modelObject.isServiceDateMonthDefaulted()){
    idfObject.setString(LifeCycleCost_ParametersFields::ServiceDateMonth, modelObject.serviceDateMonth().valueDescription());
  }

  idfObject.setInt(LifeCycleCost_ParametersFields::ServiceDateYear, modelObject.serviceDateYear());

  idfObject.setInt(LifeCycleCost_ParametersFields::LengthofStudyPeriodinYears, modelObject.lengthOfStudyPeriodInYears());

  d = modelObject.taxRate();
  if (d){
    idfObject.setDouble(LifeCycleCost_ParametersFields::Taxrate, *d);
  }

  if (modelObject.isDepreciationMethodDefaulted()){
    idfObject.setString(LifeCycleCost_ParametersFields::DepreciationMethod, "None");
  }else{
    idfObject.setString(LifeCycleCost_ParametersFields::DepreciationMethod, modelObject.depreciationMethod());
  }
  
  // if useNISTFuelEscalationRates then load idf obbjects from file in resources
  if (modelObject.useNISTFuelEscalationRates()){

    boost::optional<std::string> region = modelObject.nistRegion();
    if (!region){
      region = "U.S. Avg";
    }
    boost::optional<std::string> sector = modelObject.nistSector();
    if (!sector){
      sector = "Commercial";
    }

    static boost::optional<IdfFile> usePriceEscalationFile;
    if (!usePriceEscalationFile){
     usePriceEscalationFile = findIdfFile(":/Resources/LCCusePriceEscalationDataSet2011.idf");
    }
    OS_ASSERT(usePriceEscalationFile);

    for (IdfObject object : usePriceEscalationFile->objects()){
      std::string name = object.name().get();
      if ((name.find(*region) == 0) &&
          (name.find(*sector) != string::npos)){
        m_idfObjects.push_back(object);
      }
    }
  }else{

    std::string baseDateMonth;
    if (!modelObject.isBaseDateMonthDefaulted()){
      baseDateMonth = modelObject.baseDateMonth().valueDescription();
    }
    int baseDateYear = modelObject.baseDateYear();
    int lengthOfStudyPeriodInYears = modelObject.lengthOfStudyPeriodInYears();

    // make a use price escalation for each fuel type we have inflation for
    makeUsePriceEscalation(modelObject.electricityInflation(), "Electricity", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.naturalGasInflation(),  "NaturalGas", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.steamInflation(),       "Steam", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.gasolineInflation(),    "Gasoline", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.dieselInflation(),      "Diesel", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.coalInflation(),        "Coal", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.fuelOil1Inflation(),    "FuelOil#1", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.fuelOil2Inflation(),    "FuelOil#2", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.propaneInflation(),     "Propane", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
    makeUsePriceEscalation(modelObject.waterInflation(),       "Water", baseDateMonth, baseDateYear, lengthOfStudyPeriodInYears, m_idfObjects);
  }

  return idfObject;
}

} // energyplus

} // openstudio

