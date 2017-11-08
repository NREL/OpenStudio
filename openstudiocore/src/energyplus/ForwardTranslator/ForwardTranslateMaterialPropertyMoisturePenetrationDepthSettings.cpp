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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"

#include <utilities/idd/MaterialProperty_MoisturePenetrationDepth_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>


using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateMaterialPropertyMoisturePenetrationDepthSettings( MaterialPropertyMoisturePenetrationDepthSettings & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::MaterialProperty_MoisturePenetrationDepth_Settings);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::Name, modelObject.materialName());

  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::WaterVaporDiffusionResistanceFactor, modelObject.waterVaporDiffusionResistanceFactor());
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta, modelObject.moistureEquationCoefficientA());
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb, modelObject.moistureEquationCoefficientB());
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc, modelObject.moistureEquationCoefficientC());
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd, modelObject.moistureEquationCoefficientD());
  
  boost::optional<double> value;

  if( modelObject.isSurfaceLayerPenetrationDepthAutocalculated() )
  {
    idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth,"Autocalculate");
  }
  else if( (value = modelObject.surfaceLayerPenetrationDepth()) )
  {
    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth,value.get());
  }
  
  if( modelObject.isDeepLayerPenetrationDepthAutocalculated() )
  {
    idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth,"Autocalculate");
  }
  else if( (value = modelObject.deepLayerPenetrationDepth()) )
  {
    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth,value.get());
  }
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerThickness, modelObject.coatingLayerThickness());
  
  idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerWaterVaporDiffusionResistanceFactor, modelObject.coatingLayerWaterVaporDiffusionResistanceFactor());  

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio