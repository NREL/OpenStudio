/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/GasMixture.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/WindowMaterial_GasMixture_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGasMixture(GasMixture& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_GasMixture);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowMaterial_GasMixtureFields::Name, modelObject.name().get());

    idfObject.setDouble(WindowMaterial_GasMixtureFields::Thickness, modelObject.thickness());

    idfObject.setInt(WindowMaterial_GasMixtureFields::NumberofGasesinMixture, modelObject.numGases());

    int fieldIndex = WindowMaterial_GasMixtureFields::Gas1Type;
    for (unsigned gasIndex = 0; gasIndex < modelObject.numGases(); ++gasIndex) {

      idfObject.setString(fieldIndex++, modelObject.getGasType(gasIndex));

      idfObject.setDouble(fieldIndex++, modelObject.getGasFraction(gasIndex));
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
