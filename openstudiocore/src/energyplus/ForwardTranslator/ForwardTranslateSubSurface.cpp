/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/ShadingControl.hpp"
#include "../../model/ShadingControl_Impl.hpp"
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

boost::optional<IdfObject> ForwardTranslator::translateSubSurface( model::SubSurface & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::FenestrationSurface_Detailed);
  
  idfObject.setString(FenestrationSurface_DetailedFields::Name, modelObject.name().get());

  std::string subSurfaceType = modelObject.subSurfaceType();
  if (istringEqual("FixedWindow", subSurfaceType)){
    subSurfaceType = "Window";
  }else if (istringEqual("OperableWindow", subSurfaceType)){
    subSurfaceType = "Window";
  }else if (istringEqual("OverheadDoor", subSurfaceType)){
    subSurfaceType = "Door";
  }else if (istringEqual("Skylight", subSurfaceType)){
    subSurfaceType = "Window";
  }

  boost::optional<ConstructionBase> construction = modelObject.construction();
  if (construction){
    idfObject.setString(FenestrationSurface_DetailedFields::ConstructionName, construction->name().get());

    if (subSurfaceType == "Door" && construction->isFenestration()){
      LOG(Warn, "SubSurface '" << modelObject.name().get() << "' uses fenestration construction, changing SubSurfaceType to Door");
      subSurfaceType = "GlassDoor";
    } else if (subSurfaceType == "GlassDoor" && !construction->isFenestration()){
      LOG(Warn, "SubSurface '" << modelObject.name().get() << "' uses non-fenestration construction, changing SubSurfaceType to GlassDoor");
      subSurfaceType = "Door";
    }
  }

  idfObject.setString(FenestrationSurface_DetailedFields::SurfaceType, subSurfaceType);

  boost::optional<Surface> surface = modelObject.surface();
  if (surface){
    idfObject.setString(FenestrationSurface_DetailedFields::BuildingSurfaceName, surface->name().get());
  }

  boost::optional<SubSurface> adjacentSubSurface = modelObject.adjacentSubSurface();
  if (adjacentSubSurface){
    idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSubSurface->name().get());
  }

  boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = modelObject.surfacePropertyOtherSideCoefficients();
  if (surfacePropertyOtherSideCoefficients){
    boost::optional<IdfObject> osc = translateAndMapModelObject(*surfacePropertyOtherSideCoefficients);
    if (osc && osc->name()){
      idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, osc->name().get());
    } else{
      LOG(Error, "SubSurface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel = modelObject.surfacePropertyOtherSideConditionsModel();
  if (surfacePropertyOtherSideConditionsModel){
    boost::optional<IdfObject> oscm = translateAndMapModelObject(*surfacePropertyOtherSideConditionsModel);
    if (oscm && oscm->name()){
      idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, oscm->name().get());
    } else{
      LOG(Error, "SubSurface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
  if (viewFactortoGround){
    idfObject.setDouble(FenestrationSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
  }

  boost::optional<ShadingControl> shadingControl = modelObject.shadingControl();
  if (shadingControl){
    idfObject.setString(FenestrationSurface_DetailedFields::ShadingControlName, shadingControl->name().get());
  }

  boost::optional<WindowPropertyFrameAndDivider> frameAndDivider = modelObject.windowPropertyFrameAndDivider();
  openstudio::Vector3d offset(0, 0, 0);
  if (frameAndDivider){
    if (!frameAndDivider->isOutsideRevealDepthDefaulted()){
      offset = -frameAndDivider->outsideRevealDepth() * modelObject.outwardNormal();
    }
    idfObject.setString(FenestrationSurface_DetailedFields::FrameandDividerName, frameAndDivider->name().get());
  }

  if(!modelObject.isMultiplierDefaulted()){
    idfObject.setDouble(FenestrationSurface_DetailedFields::Multiplier, modelObject.multiplier());
  }

  idfObject.clearExtensibleGroups();
  for (const Point3d& point : modelObject.vertices()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    if (group.empty()) {
      LOG(Error,"Currently unable to translate " << modelObject.briefDescription() 
          << ", because it has more vertices than allowed by EnergyPlus.");
      return boost::none;
    }

    Point3d newPoint = point + offset;

    group.setDouble(0, newPoint.x());
    group.setDouble(1, newPoint.y());
    group.setDouble(2, newPoint.z());
  }
  
  m_idfObjects.push_back(idfObject);

  // translate surfacePropertyConvectionCoefficients
  SurfacePropertyConvectionCoefficientsVector surfacePropertyConvectionCoefficients = modelObject.surfacePropertyConvectionCoefficients();
  std::sort(surfacePropertyConvectionCoefficients.begin(), surfacePropertyConvectionCoefficients.end(), WorkspaceObjectNameLess());
  for (SurfacePropertyConvectionCoefficients& surfacePropertyConvectionCoefficient : surfacePropertyConvectionCoefficients){
    translateAndMapModelObject(surfacePropertyConvectionCoefficient);
  }

  return idfObject;

}

} // energyplus

} // openstudio

