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
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"

#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"

#include <utilities/idd/Construction_InternalSource_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstructionWithInternalSource( model::ConstructionWithInternalSource & modelObject )
{

  IdfObject idfObject( openstudio::IddObjectType::Construction_InternalSource );

  m_idfObjects.push_back(idfObject);

  //Name
  idfObject.setString(Construction_InternalSourceFields::Name, modelObject.name().get());

  //Source Present After Layer Number
  idfObject.setInt(Construction_InternalSourceFields::SourcePresentAfterLayerNumber, modelObject.sourcePresentAfterLayerNumber());

  //Temperature Calculation Requested After Layer Number
  idfObject.setInt(Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber, modelObject.temperatureCalculationRequestedAfterLayerNumber());

  // Dimensions for the CTF Calculation
  idfObject.setInt(Construction_InternalSourceFields::DimensionsfortheCTFCalculation, modelObject.dimensionsForTheCTFCalculation());

  // Tube Spacing
  idfObject.setDouble(Construction_InternalSourceFields::TubeSpacing, modelObject.tubeSpacing());

  //Layers
  MaterialVector layers = modelObject.layers();

  unsigned fieldIndex = Construction_InternalSourceFields::TubeSpacing + 1;
  for(unsigned layerIndex = 0; layerIndex < layers.size(); ++layerIndex ) {
    Material material = layers[layerIndex];
    translateAndMapModelObject(material);
    idfObject.setString(fieldIndex, material.name().get());
    fieldIndex++;
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

