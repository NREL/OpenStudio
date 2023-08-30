/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CFactorUndergroundWallConstruction.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/OS_Construction_CfactorUndergroundWall_FieldEnums.hxx>

#include <utilities/idd/Construction_CfactorUndergroundWall_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCFactorUndergroundWallConstruction(model::CFactorUndergroundWallConstruction& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Construction_CfactorUndergroundWall);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    idfObject.setString(Construction_CfactorUndergroundWallFields::Name, modelObject.name().get());

    OptionalDouble d = modelObject.getDouble(OS_Construction_CfactorUndergroundWallFields::CFactor, false);
    if (d) {
      idfObject.setDouble(Construction_CfactorUndergroundWallFields::CFactor, *d);
    } else {
      LOG(Error, "Missing required input 'C-Factor' for Construction:CfactorUndergroundWall named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_Construction_CfactorUndergroundWallFields::Height, false);
    if (d) {
      idfObject.setDouble(Construction_CfactorUndergroundWallFields::Height, *d);
    } else {
      LOG(Error, "Missing required input 'Height' for Construction:CfactorUndergroundWall named '" << modelObject.name().get() << "'");
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
