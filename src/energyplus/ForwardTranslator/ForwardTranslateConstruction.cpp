/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ShadingControl.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstruction(Construction& modelObject) {
    IdfObject construction(openstudio::IddObjectType::Construction);
    m_idfObjects.push_back(construction);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    construction.setName(modelObject.name().get());

    MaterialVector layers = modelObject.layers();

    unsigned fieldIndex = 1;
    for (unsigned layerIndex = 0; layerIndex < layers.size(); ++layerIndex) {
      Material material = layers[layerIndex];
      translateAndMapModelObject(material);
      construction.setString(fieldIndex++, material.name().get());
    }

    return boost::optional<IdfObject>(construction);
  }

}  // namespace energyplus

}  // namespace openstudio
