/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/AirGap.hpp"

#include <utilities/idd/Material_AirGap_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirGap(AirGap& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Material_AirGap);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Material_AirGapFields::Name, modelObject.name().get());

    idfObject.setDouble(Material_AirGapFields::ThermalResistance, modelObject.thermalResistance());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
