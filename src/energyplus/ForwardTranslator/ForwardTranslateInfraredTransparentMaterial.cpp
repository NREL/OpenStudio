/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/InfraredTransparentMaterial.hpp"

#include <utilities/idd/Material_InfraredTransparent_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateInfraredTransparentMaterial(InfraredTransparentMaterial& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Material_InfraredTransparent);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Material_InfraredTransparentFields::Name, modelObject.name().get());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
