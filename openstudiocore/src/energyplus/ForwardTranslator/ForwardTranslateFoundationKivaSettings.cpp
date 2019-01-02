/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/FoundationKivaSettings.hpp"
#include "../../model/FoundationKivaSettings_Impl.hpp"

#include <utilities/idd/Foundation_Kiva_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>


using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFoundationKivaSettings( FoundationKivaSettings & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Foundation_Kiva_Settings);

  m_idfObjects.push_back(idfObject);

  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilConductivity, modelObject.soilConductivity());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilDensity, modelObject.soilDensity());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilSpecificHeat, modelObject.soilSpecificHeat());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, modelObject.groundSolarAbsorptivity());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, modelObject.groundThermalAbsorptivity());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, modelObject.groundSurfaceRoughness());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::FarFieldWidth, modelObject.farFieldWidth());
  
  idfObject.setString(Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, modelObject.deepGroundBoundaryCondition());
  
  boost::optional<double> value;

  if( modelObject.isDeepGroundDepthAutocalculated() ) {
    idfObject.setString(Foundation_Kiva_SettingsFields::DeepGroundDepth, "Autocalculate");
  }
  else if( (value = modelObject.deepGroundDepth()) ) {
    idfObject.setDouble(Foundation_Kiva_SettingsFields::DeepGroundDepth, value.get());
  }
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::MinimumCellDimension, modelObject.minimumCellDimension());
  
  idfObject.setDouble(Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, modelObject.maximumCellGrowthCoefficient());
  
  idfObject.setString(Foundation_Kiva_SettingsFields::SimulationTimestep, modelObject.simulationTimestep());

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio