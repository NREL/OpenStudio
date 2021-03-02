/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    std::string subSurfaceType = modelObject.subSurfaceType();
    if (istringEqual("FixedWindow", subSurfaceType)) {
      subSurfaceType = "Window";
    } else if (istringEqual("OperableWindow", subSurfaceType)) {
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

    boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
    if (viewFactortoGround) {
      idfObject.setDouble(FenestrationSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
    }

    boost::optional<WindowPropertyFrameAndDivider> frameAndDivider = modelObject.windowPropertyFrameAndDivider();
    openstudio::Vector3d offset(0, 0, 0);
    if (frameAndDivider) {
      if (!frameAndDivider->isOutsideRevealDepthDefaulted()) {
        offset = -frameAndDivider->outsideRevealDepth() * modelObject.outwardNormal();
      }
      idfObject.setString(FenestrationSurface_DetailedFields::FrameandDividerName, frameAndDivider->name().get());
    }

    if (!modelObject.isMultiplierDefaulted()) {
      idfObject.setDouble(FenestrationSurface_DetailedFields::Multiplier, modelObject.multiplier());
    }

    idfObject.clearExtensibleGroups();
    for (const Point3d& point : modelObject.vertices()) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      if (group.empty()) {
        LOG(Error,
            "Currently unable to translate " << modelObject.briefDescription() << ", because it has more vertices than allowed by EnergyPlus.");
        return boost::none;
      }

      Point3d newPoint = point + offset;

      group.setDouble(0, newPoint.x());
      group.setDouble(1, newPoint.y());
      group.setDouble(2, newPoint.z());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
