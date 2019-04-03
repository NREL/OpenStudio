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
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include <utilities/idd/Foundation_Kiva_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>


using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFoundationKiva( FoundationKiva & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Foundation_Kiva);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(Foundation_KivaFields::Name, modelObject.name().get());

  boost::optional<Material> interiorHorizontalInsulationMaterial = modelObject.interiorHorizontalInsulationMaterial();
  if( interiorHorizontalInsulationMaterial ) {
    idfObject.setString(Foundation_KivaFields::InteriorHorizontalInsulationMaterialName, interiorHorizontalInsulationMaterial->name().get());
  }

  boost::optional<double> value;

  idfObject.setDouble(Foundation_KivaFields::InteriorHorizontalInsulationDepth, modelObject.interiorHorizontalInsulationDepth());

  if( (value = modelObject.interiorHorizontalInsulationWidth()) ) {
    idfObject.setDouble(Foundation_KivaFields::InteriorHorizontalInsulationWidth, value.get());
  }
  
  boost::optional<Material> interiorVerticalInsulationMaterial = modelObject.interiorVerticalInsulationMaterial();
  if( interiorVerticalInsulationMaterial ) {
    idfObject.setString(Foundation_KivaFields::InteriorVerticalInsulationMaterialName, interiorVerticalInsulationMaterial->name().get());
  }

  if( (value = modelObject.interiorVerticalInsulationDepth()) ) {
    idfObject.setDouble(Foundation_KivaFields::InteriorVerticalInsulationDepth, value.get());
  }

  boost::optional<Material> exteriorHorizontalInsulationMaterial = modelObject.exteriorHorizontalInsulationMaterial();
  if( exteriorHorizontalInsulationMaterial ) {
    idfObject.setString(Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName, exteriorHorizontalInsulationMaterial->name().get());
  }

  if( (value = modelObject.exteriorHorizontalInsulationDepth()) ) {
    idfObject.setDouble(Foundation_KivaFields::ExteriorHorizontalInsulationDepth, value.get());
  }

  idfObject.setDouble(Foundation_KivaFields::ExteriorHorizontalInsulationWidth, modelObject.exteriorHorizontalInsulationWidth());

  boost::optional<Material> exteriorVerticalInsulationMaterial = modelObject.exteriorVerticalInsulationMaterial();
  if( exteriorVerticalInsulationMaterial ) {
    idfObject.setString(Foundation_KivaFields::ExteriorVerticalInsulationMaterialName, exteriorVerticalInsulationMaterial->name().get());
  }

  if( (value = modelObject.exteriorVerticalInsulationDepth()) ) {
    idfObject.setDouble(Foundation_KivaFields::ExteriorVerticalInsulationDepth, value.get());
  }

  idfObject.setDouble(Foundation_KivaFields::WallHeightAboveGrade, modelObject.wallHeightAboveGrade());

  idfObject.setDouble(Foundation_KivaFields::WallDepthBelowSlab, modelObject.wallDepthBelowSlab());

  boost::optional<ConstructionBase> footingWallConstruction = modelObject.footingWallConstruction();
  if( footingWallConstruction ) {
    idfObject.setString(Foundation_KivaFields::FootingWallConstructionName, footingWallConstruction->name().get());
  }

  boost::optional<Material> footingMaterial = modelObject.footingMaterial();
  if( footingMaterial ) {
    idfObject.setString(Foundation_KivaFields::FootingMaterialName, footingMaterial->name().get());
  }

  idfObject.setDouble(Foundation_KivaFields::FootingDepth, modelObject.footingDepth());

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio