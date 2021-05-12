/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/EnvironmentalImpactFactors.hpp"

#include <utilities/idd/EnvironmentalImpactFactors_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEnvironmentalImpactFactors( model::EnvironmentalImpactFactors& modelObject )
{
  boost::optional<IdfObject> result;

  // Instantiate an IdfObject of the class to store the values,
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EnvironmentalImpactFactors, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::EnvironmentalImpactFactors );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // District Heating Efficiency: Optional Double
  double districtHeatingEfficiency = modelObject.districtHeatingEfficiency();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, districtHeatingEfficiency);


  // District Cooling COP: Optional Double
  double districtCoolingCOP = modelObject.districtCoolingCOP();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP, districtCoolingCOP);


  // Steam Conversion Efficiency: Optional Double
  double steamConversionEfficiency = modelObject.steamConversionEfficiency();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::SteamConversionEfficiency, steamConversionEfficiency);


  // Total Carbon Equivalent Emission Factor From N2O: Optional Double
  double totalCarbonEquivalentEmissionFactorFromN2O = modelObject.totalCarbonEquivalentEmissionFactorFromN2O();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, totalCarbonEquivalentEmissionFactorFromN2O);


  // Total Carbon Equivalent Emission Factor From CH4: Optional Double
  double totalCarbonEquivalentEmissionFactorFromCH4 = modelObject.totalCarbonEquivalentEmissionFactorFromCH4();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, totalCarbonEquivalentEmissionFactorFromCH4);


  // Total Carbon Equivalent Emission Factor From CO2: Optional Double
  double totalCarbonEquivalentEmissionFactorFromCO2 = modelObject.totalCarbonEquivalentEmissionFactorFromCO2();
  idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, totalCarbonEquivalentEmissionFactorFromCO2);


  result = IdfObject;
  return result;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio