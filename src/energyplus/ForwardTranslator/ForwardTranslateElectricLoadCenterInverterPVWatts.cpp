/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterInverterPVWatts.hpp"
#include "../../model/ElectricLoadCenterInverterPVWatts_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Inverter_PVWatts_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterInverterPVWatts(model::ElectricLoadCenterInverterPVWatts& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Inverter_PVWatts, modelObject);

    idfObject.setDouble(ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, modelObject.dcToACSizeRatio());

    idfObject.setDouble(ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, modelObject.inverterEfficiency());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
