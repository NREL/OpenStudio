/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"
#include "../../model/SurfacePropertyOtherSideCoefficients.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/SurfacePropertyLocalEnvironment.hpp"
#include "../../model/SurfacePropertyIncidentSolarMultiplier.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/geometry/Vector3d.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSubSurface(model::SubSurface& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::FenestrationSurface_Detailed);

    idfObject.setString(FenestrationSurface_DetailedFields::Name, modelObject.name().get());

    idfObject.clearExtensibleGroups();

    openstudio::Vector3d offset(0, 0, 0);
    boost::optional<WindowPropertyFrameAndDivider> frameAndDivider = modelObject.windowPropertyFrameAndDivider();
    if (frameAndDivider) {
      if (!frameAndDivider->isOutsideRevealDepthDefaulted()) {
        offset = -frameAndDivider->outsideRevealDepth() * modelObject.outwardNormal();
      }
      idfObject.setString(FenestrationSurface_DetailedFields::FrameandDividerName, frameAndDivider->name().get());
    }

    for (const Point3d& point : modelObject.vertices()) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      if (group.empty()) {
        LOG(Error,
            "Currently unable to translate " << modelObject.briefDescription() << ", because it has more vertices than allowed by EnergyPlus.");
        return boost::none;
      }

      const Point3d newPoint = point + offset;

      group.setDouble(0, newPoint.x());
      group.setDouble(1, newPoint.y());
      group.setDouble(2, newPoint.z());
    }

    // Register and emplace into m_map, so that any child object such as translateSurfacePropertyIncidentSolarMultiplier
    // can retrieve the subsurface namestring and we don't risk duplicating the subsurface when that child also calls
    // translateAndMapModelObject(subSurface)
    m_idfObjects.push_back(idfObject);
    m_map.insert(std::make_pair(modelObject.handle(), idfObject));

    std::string subSurfaceType = modelObject.subSurfaceType();
    if (istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType)) {
      subSurfaceType = "Window";
    } else if (istringEqual("OverheadDoor", subSurfaceType)) {
      subSurfaceType = "Door";
    } else if (istringEqual("Skylight", subSurfaceType)) {
      subSurfaceType = "Window";
    }

    boost::optional<ConstructionBase> construction = modelObject.construction();
    if (construction) {
      idfObject.setString(FenestrationSurface_DetailedFields::ConstructionName, construction->name().get());

      if (subSurfaceType == "Door" && construction->isFenestration()) {
        LOG(Warn, "SubSurface '" << modelObject.name().get() << "' uses fenestration construction, changing SubSurfaceType to GlassDoor");
        subSurfaceType = "GlassDoor";
      } else if (subSurfaceType == "GlassDoor" && !construction->isFenestration()) {
        LOG(Warn, "SubSurface '" << modelObject.name().get() << "' uses non-fenestration construction, changing SubSurfaceType to Door");
        subSurfaceType = "Door";
      }
    }

    idfObject.setString(FenestrationSurface_DetailedFields::SurfaceType, subSurfaceType);

    boost::optional<Surface> surface = modelObject.surface();
    if (surface) {
      idfObject.setString(FenestrationSurface_DetailedFields::BuildingSurfaceName, surface->name().get());
    }

    boost::optional<SubSurface> adjacentSubSurface = modelObject.adjacentSubSurface();
    if (adjacentSubSurface) {
      idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSubSurface->name().get());
    }

    boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = modelObject.surfacePropertyOtherSideCoefficients();
    if (surfacePropertyOtherSideCoefficients) {
      boost::optional<IdfObject> osc = translateAndMapModelObject(*surfacePropertyOtherSideCoefficients);
      if (osc && osc->name()) {
        idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, osc->name().get());
      } else {
        LOG(Error, "SubSurface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
      }
    }

    boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel =
      modelObject.surfacePropertyOtherSideConditionsModel();
    if (surfacePropertyOtherSideConditionsModel) {
      boost::optional<IdfObject> oscm = translateAndMapModelObject(*surfacePropertyOtherSideConditionsModel);
      if (oscm && oscm->name()) {
        idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, oscm->name().get());
      } else {
        LOG(Error, "SubSurface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
      }
    }

    // Call the translation of the SurfacePropertyConvectionCoefficients, which has two advantages:
    // * will not translate them if they are orphaned (=not referencing a surface or subsurface), and,
    // * makes the order of these objects in the IDF deterministic
    if (boost::optional<SurfacePropertyConvectionCoefficients> _sCoefs = modelObject.surfacePropertyConvectionCoefficients()) {
      translateAndMapModelObject(_sCoefs.get());
    }

    if (boost::optional<SurfacePropertyLocalEnvironment> localEnv_ = modelObject.surfacePropertyLocalEnvironment()) {
      translateAndMapModelObject(localEnv_.get());
    }

    boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
    if (viewFactortoGround) {
      idfObject.setDouble(FenestrationSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
    }

    if (!modelObject.isMultiplierDefaulted()) {
      idfObject.setDouble(FenestrationSurface_DetailedFields::Multiplier, modelObject.multiplier());
    }

    if (boost::optional<SurfacePropertyIncidentSolarMultiplier> spMult_ = modelObject.surfacePropertyIncidentSolarMultiplier()) {
      translateAndMapModelObject(spMult_.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
