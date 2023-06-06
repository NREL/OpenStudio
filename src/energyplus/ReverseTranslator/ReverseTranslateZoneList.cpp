/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneList(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneList) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneList");
      return boost::none;
    }

    openstudio::model::SpaceType spaceType(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      spaceType.setName(*s);
    }

    // Note that this is coarse: it will create a space type for each Zonelist it finds, but if a zone is referenced by multiple zonelists,
    // its spaces will end up with a spacetype corresponding to the last Zonelist it found that references it.
    // You'll get a warning that the previous SpaceType was overwritten though
    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

      OptionalWorkspaceObject target = workspaceGroup.getTarget(0);
      if (target) {
        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
        if (modelObject) {
          if (modelObject->optionalCast<Space>()) {
            auto space = modelObject->cast<Space>();

            if (space.spaceType()) {
              LOG(Warn, "Overriding previously assigned SpaceType for Space '" << space.name().get() << "'");
            }

            space.setSpaceType(spaceType);
          }
        }
      }
    }

    return spaceType;
  }

}  // namespace energyplus

}  // namespace openstudio
