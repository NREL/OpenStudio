/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/DaylightingDeviceLightWell.hpp"
#include "../../model/SubSurface.hpp"

#include <utilities/idd/DaylightingDevice_LightWell_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDaylightingDeviceLightWell(model::DaylightingDeviceLightWell& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::DaylightingDevice_LightWell, modelObject);

    SubSurface subSurface = modelObject.subSurface();
    if (boost::optional<IdfObject> subSurf = translateAndMapModelObject(subSurface)) {
      idfObject.setString(DaylightingDevice_LightWellFields::ExteriorWindowName, subSurf->nameString());
    }

    idfObject.setDouble(DaylightingDevice_LightWellFields::HeightofWell, modelObject.heightofWell());

    idfObject.setDouble(DaylightingDevice_LightWellFields::PerimeterofBottomofWell, modelObject.perimeterofBottomofWell());

    idfObject.setDouble(DaylightingDevice_LightWellFields::AreaofBottomofWell, modelObject.areaofBottomofWell());

    idfObject.setDouble(DaylightingDevice_LightWellFields::VisibleReflectanceofWellWalls, modelObject.visibleReflectanceofWellWalls());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
