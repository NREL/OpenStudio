/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DaylightingDeviceShelf.hpp"
#include "../../model/DaylightingDeviceShelf_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>

#include <utilities/idd/DaylightingDevice_Shelf_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDaylightingDeviceShelf(model::DaylightingDeviceShelf& modelObject) {
    SubSurface window = modelObject.subSurface();
    boost::optional<Space> space = window.space();
    boost::optional<InteriorPartitionSurface> insideShelf = modelObject.insideShelf();
    boost::optional<ShadingSurface> outsideShelf = modelObject.outsideShelf();

    if (!space) {
      return boost::none;
    }

    if (!(insideShelf || outsideShelf)) {
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::DaylightingDevice_Shelf, modelObject);

    if (boost::optional<IdfObject> w = translateAndMapModelObject(window)) {
      idfObject.setString(DaylightingDevice_ShelfFields::WindowName, w->name().get());
    }

    // inside shelf is converted to a surface
    if (insideShelf) {

      openstudio::Transformation transformation;
      boost::optional<InteriorPartitionSurfaceGroup> group = insideShelf->interiorPartitionSurfaceGroup();
      if (group) {
        transformation = group->transformation();
      }

      Point3dVector vertices = transformation * insideShelf->vertices();
      Surface newSurface(vertices, modelObject.model());
      newSurface.setName(modelObject.name().get());
      newSurface.setSpace(*space);
      newSurface.setAdjacentSurface(newSurface);

      boost::optional<ConstructionBase> construction = insideShelf->construction();
      if (!construction) {
        Model t_model = modelObject.model();
        construction = interiorPartitionSurfaceConstruction(t_model);
      }
      OS_ASSERT(construction);
      newSurface.setConstruction(*construction);

      boost::optional<IdfObject> newSurfaceObject = translateAndMapModelObject(newSurface);
      if (newSurfaceObject) {
        idfObject.setString(DaylightingDevice_ShelfFields::InsideShelfName, newSurfaceObject->name().get());
      }
    }

    if (outsideShelf) {

      idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfName, outsideShelf->name().get());

      boost::optional<ConstructionBase> construction = outsideShelf->construction();
      if (!construction) {
        Model t_model = modelObject.model();
        construction = exteriorSurfaceConstruction(t_model);
      }
      OS_ASSERT(construction);
      idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfConstructionName, construction->name().get());

      OptionalDouble d = modelObject.viewFactortoOutsideShelf();
      if (d) {
        idfObject.setDouble(DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, *d);
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
