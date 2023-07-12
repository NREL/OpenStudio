/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
