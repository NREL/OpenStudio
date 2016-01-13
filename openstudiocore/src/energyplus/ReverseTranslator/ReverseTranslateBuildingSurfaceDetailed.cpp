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

#include "../ReverseTranslator.hpp"
#include "../GeometryTranslator.hpp"

#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateBuildingSurfaceDetailed( const WorkspaceObject & workspaceObject )
{
 if( workspaceObject.iddObject().type() != IddObjectType::BuildingSurface_Detailed ){
   LOG(Error, "WorkspaceObject is not IddObjectType: BuildingSurface:Detailed");
    return boost::none;
  }

  openstudio::Point3dVector vertices = getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, workspaceObject);
 
  boost::optional<Surface> surface;
  try{
    surface = Surface(vertices, m_model);
  }catch(const std::exception&){
    LOG(Error, "Cannot create Surface for object: " << workspaceObject);
    return boost::none;
  }

  OptionalString s = workspaceObject.name();
  if(s) {
    surface->setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::BuildingSurface_DetailedFields::ConstructionName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ConstructionBase>()){
        surface->setConstruction(modelObject->cast<ConstructionBase>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::BuildingSurface_DetailedFields::ZoneName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        surface->setSpace(modelObject->cast<Space>());
      }
    }
  }

  s = workspaceObject.getString(BuildingSurface_DetailedFields::SurfaceType);
  if (s) {
    if (istringEqual("Roof", *s) || istringEqual("Ceiling", *s)){
      s = "RoofCeiling";
    }
    surface->setSurfaceType(*s);
  }
  //std::string surfaceType = surface->surfaceType();

  s = workspaceObject.getString(BuildingSurface_DetailedFields::SunExposure);
  if (s) {
    surface->setSunExposure(*s);
  }

  s = workspaceObject.getString(BuildingSurface_DetailedFields::WindExposure);
  if (s) {
    surface->setWindExposure(*s);
  }

  OptionalDouble d = workspaceObject.getDouble(BuildingSurface_DetailedFields::ViewFactortoGround);
  if (d) {
    surface->setViewFactortoGround(*d);
  }

  target = workspaceObject.getTarget(openstudio::BuildingSurface_DetailedFields::OutsideBoundaryConditionObject);
  if (target){

    if (target->iddObject().type() == IddObjectType::Zone){
      // Zone boundary condition

      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if(modelObject->optionalCast<Space>()){
        Space adjacentSpace = modelObject->cast<Space>();

        if (surface->space()){
          // insert this surface in the map so subsurface translation can find it  
          m_workspaceToModelMap.insert(std::make_pair(workspaceObject.handle(), surface.get()));

          // need to translate all sub surfaces here so they will be in adjacent space
          for (const WorkspaceObject& workspaceSubSurface : workspaceObject.getSources(IddObjectType::FenestrationSurface_Detailed)){
            translateAndMapWorkspaceObject(workspaceSubSurface);
          }

          // create adjacent surface in other space
          surface->createAdjacentSurface(adjacentSpace);
          return surface.get();
        }
      }

    }else if (target->iddObject().type() == IddObjectType::BuildingSurface_Detailed){
      // Surface boundary condition

      // see if we have already mapped other surface, don't do it here because that is circular
      if (target->handle() == workspaceObject.handle() ){
        // these objects are the same, set boundary condition to adiabatic
        surface->setOutsideBoundaryCondition("Adiabatic");
        return surface.get();
      }else{
        auto it = m_workspaceToModelMap.find(target->handle());
        if( it !=  m_workspaceToModelMap.end()){
          if (it->second.optionalCast<Surface>()){
            // this will set other side boundary object on both surfaces
            Surface adjacentSurface = it->second.cast<Surface>();
            surface->setAdjacentSurface(adjacentSurface);
            return surface.get();
          }
        }
      }

    }else{  
      LOG(Error, "OutsideBoundaryConditionObject not yet mapped for object of type " << target->iddObject().name());
    }
  }

  s = workspaceObject.getString(BuildingSurface_DetailedFields::OutsideBoundaryCondition);
  if (s) {
    surface->setOutsideBoundaryCondition(*s);
  }

  return surface.get();
}

} // energyplus

} // openstudio

