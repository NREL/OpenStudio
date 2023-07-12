/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../GeometryTranslator.hpp"

#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Shading_Site_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateShadingSiteDetailed(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Shading_Site_Detailed) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Shading:Site:Detailed");
      return boost::none;
    }

    openstudio::Point3dVector vertices = getVertices(Shading_Site_DetailedFields::NumberofVertices + 1, workspaceObject);

    boost::optional<ShadingSurface> shadingSurface;
    try {
      shadingSurface = ShadingSurface(vertices, m_model);
    } catch (const std::exception&) {
      LOG(Error, "Cannot create ShadingSurface for object: " << workspaceObject);
      return boost::none;
    }

    OptionalString s = workspaceObject.name();
    if (s) {
      shadingSurface->setName(*s);
    }

    // look for first site level shading surface group
    OptionalShadingSurfaceGroup shadingSurfaceGroup;
    for (const ShadingSurfaceGroup& group : m_model.getConcreteModelObjects<ShadingSurfaceGroup>()) {
      if (istringEqual("Site", group.shadingSurfaceType())) {
        shadingSurfaceGroup = group;
        break;
      }
    }

    // if not found make one
    if (!shadingSurfaceGroup) {
      shadingSurfaceGroup = ShadingSurfaceGroup(m_model);
      shadingSurfaceGroup->setShadingSurfaceType("Site");
    }

    shadingSurface->setShadingSurfaceGroup(*shadingSurfaceGroup);

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::Shading_Site_DetailedFields::TransmittanceScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          shadingSurface->setTransmittanceSchedule(schedule);
        }
      }
    }

    return shadingSurface.get();
  }

}  // namespace energyplus

}  // namespace openstudio
