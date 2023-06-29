/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/FFactorGroundFloorConstruction.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/OS_Construction_FfactorGroundFloor_FieldEnums.hxx>

#include <utilities/idd/Construction_FfactorGroundFloor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFFactorGroundFloorConstruction(FFactorGroundFloorConstruction& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Construction_FfactorGroundFloor);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    idfObject.setString(Construction_FfactorGroundFloorFields::Name, modelObject.name().get());

    OptionalDouble d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::FFactor, false);
    if (d) {
      idfObject.setDouble(Construction_FfactorGroundFloorFields::FFactor, *d);
    } else {
      LOG(Error, "Missing required input 'F-Factor' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::Area, false);
    if (d) {
      idfObject.setDouble(Construction_FfactorGroundFloorFields::Area, *d);
    } else {
      LOG(Error, "Missing required input 'Area' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::PerimeterExposed, false);
    if (d) {
      idfObject.setDouble(Construction_FfactorGroundFloorFields::PerimeterExposed, *d);
    } else {
      LOG(Error, "Missing required input 'PerimeterExposed' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
