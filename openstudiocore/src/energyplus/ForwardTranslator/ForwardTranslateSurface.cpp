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
#include "../../model/SurfacePropertyOtherSideCoefficients.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/PlanarSurface.hpp"

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

boost::optional<IdfObject> ForwardTranslator::translateSurface( model::Surface & modelObject )
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::BuildingSurface_Detailed, 
                                                       modelObject);
  
  std::string surfaceType = modelObject.surfaceType();
  if (istringEqual("RoofCeiling", surfaceType)){
    if (modelObject.isPartOfEnvelope()){
      surfaceType = "Roof";
    }else{
      surfaceType = "Ceiling";
    }
  }
  idfObject.setString(BuildingSurface_DetailedFields::SurfaceType, surfaceType);

  boost::optional<ConstructionBase> construction = modelObject.construction();
  if (construction){
    idfObject.setString(BuildingSurface_DetailedFields::ConstructionName, construction->name().get());
  }

  boost::optional<Space> space = modelObject.space();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(BuildingSurface_DetailedFields::ZoneName, thermalZone->name().get());
    }
  }

  boost::optional<Surface> adjacentSurface = modelObject.adjacentSurface();
  boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = modelObject.surfacePropertyOtherSideCoefficients();
  boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel = modelObject.surfacePropertyOtherSideConditionsModel();
  std::string outsideBoundaryCondition = modelObject.outsideBoundaryCondition();
  if (istringEqual("Surface", outsideBoundaryCondition)){
    if (!adjacentSurface){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  " 
                "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

      outsideBoundaryCondition = "Adiabatic";
    }
  }else if (istringEqual("OtherSideCoefficients", outsideBoundaryCondition)){
    if (!surfacePropertyOtherSideCoefficients){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  "
          "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

      outsideBoundaryCondition = "OtherSideCoefficients";
    }
  }else if (istringEqual("OtherSideConditionsModel", outsideBoundaryCondition)){
    if (!surfacePropertyOtherSideConditionsModel){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  "
          "Changing Outside Boundary Condition from 'OtherSideConditionsModel' to 'Adiabatic'");

      outsideBoundaryCondition = "Adiabatic";
    }
  }

  if (istringEqual("Adiabatic", outsideBoundaryCondition)){
    std::vector<SubSurface> subSurfaces = modelObject.subSurfaces();
    if (!subSurfaces.empty()){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' is adiabatic, removing all sub surfaces");
      for (auto subSurface : subSurfaces){
        subSurface.remove();
      }
    }
  }

  idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, outsideBoundaryCondition);

  if (adjacentSurface){
    // do not translate and map here, wait for adjacent surface to be translated on its own
    idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSurface->name().get());
  }

  if (surfacePropertyOtherSideCoefficients){
    boost::optional<IdfObject> osc = translateAndMapModelObject(*surfacePropertyOtherSideCoefficients);
    if (osc && osc->name()){
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, osc->name().get());
    } else{
      LOG(Error, "Surface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  if (surfacePropertyOtherSideConditionsModel){
    boost::optional<IdfObject> oscm = translateAndMapModelObject(*surfacePropertyOtherSideConditionsModel);
    if (oscm && oscm->name()){
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, oscm->name().get());
    } else{
      LOG(Error, "Surface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  if (!modelObject.isSunExposureDefaulted()){
    idfObject.setString(BuildingSurface_DetailedFields::SunExposure, modelObject.sunExposure());
  }

  if (!modelObject.isWindExposureDefaulted()){
    idfObject.setString(BuildingSurface_DetailedFields::WindExposure, modelObject.windExposure());
  }

  boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
  if (viewFactortoGround){
    idfObject.setDouble(BuildingSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
  }

  idfObject.clearExtensibleGroups();
  for (const Point3d& point : modelObject.vertices()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 3);
    group.setDouble(0, point.x());
    group.setDouble(1, point.y());
    group.setDouble(2, point.z());
  }

  // translate surfacePropertyConvectionCoefficients
  SurfacePropertyConvectionCoefficientsVector surfacePropertyConvectionCoefficients = modelObject.surfacePropertyConvectionCoefficients();
  std::sort(surfacePropertyConvectionCoefficients.begin(), surfacePropertyConvectionCoefficients.end(), WorkspaceObjectNameLess());
  for (SurfacePropertyConvectionCoefficients& surfacePropertyConvectionCoefficient : surfacePropertyConvectionCoefficients){
    translateAndMapModelObject(surfacePropertyConvectionCoefficient);
  }

  // translate subsurfaces
  SubSurfaceVector subSurfaces = modelObject.subSurfaces();
  std::sort(subSurfaces.begin(), subSurfaces.end(), WorkspaceObjectNameLess());
  for (SubSurface& subSurface : subSurfaces){
    translateAndMapModelObject(subSurface);
  }

  return idfObject;
}

} // energyplus

} // openstudio

