/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/SurfacePropertyOtherSideCoefficients.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/SurfacePropertyLocalEnvironment.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurface(model::Surface& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::BuildingSurface_Detailed, modelObject);

    std::string surfaceType = modelObject.surfaceType();
    if (istringEqual("RoofCeiling", surfaceType)) {
      if (modelObject.isPartOfEnvelope()) {
        surfaceType = "Roof";
      } else {
        surfaceType = "Ceiling";
      }
    }
    idfObject.setString(BuildingSurface_DetailedFields::SurfaceType, surfaceType);

    boost::optional<ConstructionBase> construction = modelObject.construction();
    if (construction) {
      idfObject.setString(BuildingSurface_DetailedFields::ConstructionName, construction->nameString());
    }

    boost::optional<Space> space = modelObject.space();
    if (space) {
      boost::optional<ThermalZone> thermalZone = space->thermalZone();
      if (thermalZone) {
        idfObject.setString(BuildingSurface_DetailedFields::ZoneName, thermalZone->nameString());
      }
      // TODO: do we want that?
      if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        idfObject.setString(BuildingSurface_DetailedFields::SpaceName, space->nameString());
      }
    }

    boost::optional<Surface> adjacentSurface = modelObject.adjacentSurface();
    boost::optional<FoundationKiva> adjacentFoundation = modelObject.adjacentFoundation();
    boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = modelObject.surfacePropertyOtherSideCoefficients();
    boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel =
      modelObject.surfacePropertyOtherSideConditionsModel();
    std::string outsideBoundaryCondition = modelObject.outsideBoundaryCondition();
    if (istringEqual("Surface", outsideBoundaryCondition)) {
      if (!adjacentSurface) {
        LOG(Warn, "Surface '" << modelObject.nameString()
                              << "' has blank Outside Boundary Condition Object.  "
                                 "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

        outsideBoundaryCondition = "Adiabatic";
      }
    } else if (istringEqual("OtherSideCoefficients", outsideBoundaryCondition)) {
      if (!surfacePropertyOtherSideCoefficients) {
        LOG(Warn, "Surface '" << modelObject.nameString()
                              << "' has blank Outside Boundary Condition Object.  "
                                 "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

        outsideBoundaryCondition = "OtherSideCoefficients";
      }
    } else if (istringEqual("OtherSideConditionsModel", outsideBoundaryCondition)) {
      if (!surfacePropertyOtherSideConditionsModel) {
        LOG(Warn, "Surface '" << modelObject.nameString()
                              << "' has blank Outside Boundary Condition Object.  "
                                 "Changing Outside Boundary Condition from 'OtherSideConditionsModel' to 'Adiabatic'");

        outsideBoundaryCondition = "Adiabatic";
      }
    }

    if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
      std::vector<SubSurface> subSurfaces = modelObject.subSurfaces();
      if (!subSurfaces.empty()) {
        LOG(Warn, "Surface '" << modelObject.nameString() << "' is adiabatic, removing all sub surfaces");
        for (auto& subSurface : subSurfaces) {
          subSurface.remove();
        }
      }
    }

    idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, outsideBoundaryCondition);

    if (adjacentSurface) {
      // do not translate and map here, wait for adjacent surface to be translated on its own
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSurface->nameString());
    }

    if (adjacentFoundation) {
      // do not translate and map here, wait for adjacent foundation to be translated on its own
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentFoundation->nameString());
    }

    if (surfacePropertyOtherSideCoefficients) {
      boost::optional<IdfObject> osc = translateAndMapModelObject(*surfacePropertyOtherSideCoefficients);
      if (osc && osc->name()) {
        idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, osc->nameString());
      } else {
        LOG(Error, "Surface '" << modelObject.nameString() << "', could not translate OutsideBoundaryConditionObject");
      }
    }

    if (surfacePropertyOtherSideConditionsModel) {
      boost::optional<IdfObject> oscm = translateAndMapModelObject(*surfacePropertyOtherSideConditionsModel);
      if (oscm && oscm->name()) {
        idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, oscm->nameString());
      } else {
        LOG(Error, "Surface '" << modelObject.nameString() << "', could not translate OutsideBoundaryConditionObject");
      }
    }

    if (!modelObject.isSunExposureDefaulted()) {
      idfObject.setString(BuildingSurface_DetailedFields::SunExposure, modelObject.sunExposure());
    }

    if (!modelObject.isWindExposureDefaulted()) {
      idfObject.setString(BuildingSurface_DetailedFields::WindExposure, modelObject.windExposure());
    }

    boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
    if (viewFactortoGround) {
      idfObject.setDouble(BuildingSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
    }

    idfObject.clearExtensibleGroups();
    for (const Point3d& point : modelObject.vertices()) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      OS_ASSERT(group.numFields() == 3);
      group.setDouble(0, point.x());
      group.setDouble(1, point.y());
      group.setDouble(2, point.z());
    }

    // translate subsurfaces
    SubSurfaceVector subSurfaces = modelObject.subSurfaces();
    std::sort(subSurfaces.begin(), subSurfaces.end(), WorkspaceObjectNameLess());
    for (SubSurface& subSurface : subSurfaces) {
      translateAndMapModelObject(subSurface);
    }

    // Call the translation of these objects, which has two advantages:
    // * will not translate them if they are orphaned (=not referencing a surface), and,
    // * makes the order of these objects in the IDF deterministic
    if (boost::optional<SurfaceControlMovableInsulation> _sIns = modelObject.surfaceControlMovableInsulation()) {
      translateAndMapModelObject(_sIns.get());
    }

    if (boost::optional<SurfacePropertyConvectionCoefficients> _sCoefs = modelObject.surfacePropertyConvectionCoefficients()) {
      translateAndMapModelObject(_sCoefs.get());
    }

    if (boost::optional<SurfacePropertyLocalEnvironment> localEnv_ = modelObject.surfacePropertyLocalEnvironment()) {
      translateAndMapModelObject(localEnv_.get());
    }

    if (boost::optional<SurfacePropertyExposedFoundationPerimeter> _sProps = modelObject.surfacePropertyExposedFoundationPerimeter()) {
      translateAndMapModelObject(_sProps.get());
    } else {
      // create a defaulted SurfacePropertyExposedFoundationPerimeter object if it doesn't exist
      if ((adjacentFoundation) && (surfaceType == "Floor")) {
        boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
          modelObject.createSurfacePropertyExposedFoundationPerimeter("ExposedPerimeterFraction", 1);
        translateAndMapModelObject(optprop.get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
