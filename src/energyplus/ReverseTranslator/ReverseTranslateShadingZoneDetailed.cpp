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
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateShadingZoneDetailed(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Shading_Zone_Detailed) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Shading:Zone:Detailed");
      return boost::none;
    }

    openstudio::Point3dVector vertices = getVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, workspaceObject);

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

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::Shading_Zone_DetailedFields::BaseSurfaceName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Surface>()) {
          OptionalSpace space = modelObject->cast<Surface>().space();
          if (space) {
            ShadingSurfaceGroupVector groups = space->shadingSurfaceGroups();
            if (groups.empty()) {
              ShadingSurfaceGroup shadingSurfaceGroup(m_model);
              shadingSurfaceGroup.setSpace(*space);
              groups.push_back(shadingSurfaceGroup);
            }
            shadingSurface->setShadingSurfaceGroup(groups[0]);
          }
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::Shading_Zone_DetailedFields::TransmittanceScheduleName);
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
