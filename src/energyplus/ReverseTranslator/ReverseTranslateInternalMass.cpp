/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateInternalMass(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::InternalMass) {
      LOG(Error, "WorkspaceObject is not IddObjectType: InternalMass");
      return boost::none;
    }

    // create the definition
    openstudio::model::InternalMassDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::InternalMassFields::ConstructionName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ConstructionBase>()) {
          definition.setConstruction(modelObject->cast<ConstructionBase>());
        }
      }
    }

    OptionalDouble d = workspaceObject.getDouble(openstudio::InternalMassFields::SurfaceArea);
    if (d) {
      definition.setSurfaceArea(*d);
    }

    // create the instance
    InternalMass internalMass(definition);

    s = workspaceObject.name();
    if (s) {
      internalMass.setName(*s);
    }

    target = workspaceObject.getTarget(openstudio::InternalMassFields::ZoneorZoneListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          internalMass.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          internalMass.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    return internalMass;
  }

}  // namespace energyplus

}  // namespace openstudio
