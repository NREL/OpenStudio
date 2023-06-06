/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_ExposedFoundationPerimeter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSurfacePropertyExposedFoundationPerimeter(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SurfaceProperty_ExposedFoundationPerimeter) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SurfacePropertyExposedFoundationPerimeter");
      return boost::none;
    }

    OptionalString s = workspaceObject.getString(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterCalculationMethod);

    OptionalDouble d = workspaceObject.getDouble(SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter);

    boost::optional<SurfacePropertyExposedFoundationPerimeter> surfacePropertyExposedFoundationPerimeter;

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Surface>()) {
          boost::optional<Surface> surface = modelObject->optionalCast<Surface>();
          surfacePropertyExposedFoundationPerimeter = surface.get().createSurfacePropertyExposedFoundationPerimeter(*s, *d);
        }
      }
    }

    return surfacePropertyExposedFoundationPerimeter.get();
  }

}  // namespace energyplus

}  // namespace openstudio
