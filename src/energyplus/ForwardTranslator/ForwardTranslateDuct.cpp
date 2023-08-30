/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Duct.hpp"
#include "../../model/Duct_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Duct_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDuct(Duct& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Duct);

    m_idfObjects.push_back(idfObject);

    if (auto value = modelObject.name()) {
      idfObject.setName(value.get());
    }

    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(openstudio::DuctFields::InletNodeName, node->name().get());
    }

    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(openstudio::DuctFields::OutletNodeName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
