/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/ConstructionProperty_InternalHeatSource_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstructionWithInternalSource(model::ConstructionWithInternalSource& modelObject) {

    // New in E+ 9.5.0:  `Construction:InternalSource` used to be a standalone object listing it's layers.
    // Now it's replaced with `ConstructionProperty:InternalHeatSource` which references a `Construction` object.
    // I have made the choice to keep the OS SDK API intact and handle that in the ForwardTranslator.

    //Layers: starting in E+ 9.5.0,
    MaterialVector layers = modelObject.layers();

    if (layers.empty()) {
      LOG(Warn, modelObject.briefDescription() << " has no layers, it will not be translated.");
      return boost::none;
    }

    IdfObject constructionObject(openstudio::IddObjectType::Construction);
    m_idfObjects.push_back(constructionObject);
    //Name
    constructionObject.setString(ConstructionFields::Name, modelObject.name().get());

    unsigned fieldIndex = 1;
    for (unsigned layerIndex = 0; layerIndex < layers.size(); ++layerIndex) {
      Material material = layers[layerIndex];
      translateAndMapModelObject(material);
      constructionObject.setString(fieldIndex++, material.name().get());
    }

    // And we also do the ConstructionProperty:InternalHeatSource object
    IdfObject propertyObject(openstudio::IddObjectType::ConstructionProperty_InternalHeatSource);
    m_idfObjects.push_back(propertyObject);

    propertyObject.setString(ConstructionProperty_InternalHeatSourceFields::Name, modelObject.name().get() + " InternalHeatSource Property");

    propertyObject.setString(ConstructionProperty_InternalHeatSourceFields::ConstructionName, modelObject.name().get());

    //Source Present After Layer Number
    propertyObject.setInt(ConstructionProperty_InternalHeatSourceFields::ThermalSourcePresentAfterLayerNumber,
                          modelObject.sourcePresentAfterLayerNumber());

    //Temperature Calculation Requested After Layer Number
    propertyObject.setInt(ConstructionProperty_InternalHeatSourceFields::TemperatureCalculationRequestedAfterLayerNumber,
                          modelObject.temperatureCalculationRequestedAfterLayerNumber());

    // Dimensions for the CTF Calculation
    propertyObject.setInt(ConstructionProperty_InternalHeatSourceFields::DimensionsfortheCTFCalculation,
                          modelObject.dimensionsForTheCTFCalculation());

    // Tube Spacing
    propertyObject.setDouble(ConstructionProperty_InternalHeatSourceFields::TubeSpacing, modelObject.tubeSpacing());

    // Two Dimensional Temperature Calculation Position
    propertyObject.setDouble(ConstructionProperty_InternalHeatSourceFields::TwoDimensionalTemperatureCalculationPosition,
                             modelObject.twoDimensionalTemperatureCalculationPosition());

    // What we return is the construction object, not the property object
    return constructionObject;
  }

}  // namespace energyplus

}  // namespace openstudio
