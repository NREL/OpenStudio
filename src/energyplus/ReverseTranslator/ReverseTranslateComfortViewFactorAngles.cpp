/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ComfortViewFactorAngles.hpp"
#include "../../model/ComfortViewFactorAngles_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ComfortViewFactorAngles_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  OptionalModelObject ReverseTranslator::translateComfortViewFactorAngles(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ComfortViewFactorAngles) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ComfortViewFactorAngles");
      return boost::none;
    }

    ComfortViewFactorAngles comfortViewFactorAngles(m_model);
    if (auto name = workspaceObject.name()) {
      comfortViewFactorAngles.setName(*name);
    }

    for (const auto& idfGroup : workspaceObject.extensibleGroups()) {
      auto group = idfGroup.cast<WorkspaceExtensibleGroup>();
      auto surfaceTarget = group.getTarget(ComfortViewFactorAnglesExtensibleFields::SurfaceName);
      auto angleFactor = group.getDouble(ComfortViewFactorAnglesExtensibleFields::AngleFactor);
      if (!surfaceTarget || !angleFactor) {
        continue;
      }
      if (auto modelObject = translateAndMapWorkspaceObject(*surfaceTarget)) {
        comfortViewFactorAngles.addAngleFactor(*modelObject, *angleFactor);
      }
    }

    return comfortViewFactorAngles;
  }

}  // namespace energyplus
}  // namespace openstudio
