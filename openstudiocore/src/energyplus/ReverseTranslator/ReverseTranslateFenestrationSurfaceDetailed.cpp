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

#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/ShadingControl.hpp"
#include "../../model/ShadingControl_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateFenestrationSurfaceDetailed( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::FenestrationSurface_Detailed ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Site:FenestrationSurface_Detailed");
    return boost::none;
  }

  openstudio::Point3dVector vertices = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, workspaceObject);
 
  boost::optional<SubSurface> subSurface;
  try{
    subSurface = SubSurface(vertices, m_model);
  }catch(const std::exception&){
    LOG(Error, "Cannot create SubSurface for object: " << workspaceObject);
    return boost::none;
  }

  OptionalString s = workspaceObject.name();
  if(s) {
    subSurface->setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::FenestrationSurface_DetailedFields::ConstructionName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ConstructionBase>()){
        subSurface->setConstruction(modelObject->cast<ConstructionBase>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::FenestrationSurface_DetailedFields::BuildingSurfaceName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Surface>()){
        subSurface->setSurface(modelObject->cast<Surface>());
      }
    }
  }

  // needs to be after .setSurface.
  s = workspaceObject.getString(FenestrationSurface_DetailedFields::SurfaceType);
  if (s) {
    if (istringEqual("Window", *s)){
      s = "FixedWindow";

      boost::optional<Surface> surface = subSurface->surface();
      if (surface){
        if ((surface->surfaceType() == "RoofCeiling") &&
            (surface->outsideBoundaryCondition() == "Outdoors")){
              s = "Skylight";
        }
      }
    }
    subSurface->setSubSurfaceType(*s);
  }

  target = workspaceObject.getTarget(openstudio::FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject);
  if (target){
    if (target->iddObject().type() == IddObjectType::Zone){
      // Zone boundary condition

      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if(modelObject->optionalCast<Space>()){
        Space adjacentSpace = modelObject->cast<Space>();

        OptionalSurface surface = subSurface->surface();
        if (surface && surface->space()){
          Space space = surface->space().get();

          if (surface->adjacentSurface()){
            Surface adjacentSurface = surface->adjacentSurface().get();

            if (adjacentSurface.space() && adjacentSpace.handle() == adjacentSurface.space()->handle()){
              Transformation transformation = adjacentSpace.transformation().inverse()*surface->space()->transformation();

              // duplicate subsurface in other space
              SubSurface adjacentSubSurface = subSurface->clone(m_model).cast<SubSurface>();
              adjacentSubSurface.setName(subSurface->name().get() + " Reversed");
              std::reverse(vertices.begin(), vertices.end());
              adjacentSubSurface.setVertices(transformation*vertices);
              adjacentSubSurface.setSurface(adjacentSurface);
              subSurface->setAdjacentSubSurface(adjacentSubSurface);

              return subSurface.get();
            }
          }
        }
      }

    }else if (target->iddObject().type() == IddObjectType::FenestrationSurface_Detailed){
      // SubSurface boundary condition

      // see if we have already mapped other sub surface, don't do it here because that is circular
      auto it = m_workspaceToModelMap.find(target->handle());
      if( it !=  m_workspaceToModelMap.end()){
        if (it->second.optionalCast<SubSurface>()){
          // this will set other side boundary object on both surfaces
          SubSurface adjacentSubSurface = it->second.cast<SubSurface>();
          subSurface->setAdjacentSubSurface(adjacentSubSurface);
          return subSurface.get();
        }
      }
    }else{  
      LOG(Error, "OutsideBoundaryConditionObject not yet mapped for object of type " << target->iddObject().name());
    }
  }

  // DLM: should these be before control paths that return above?
  OptionalDouble d = workspaceObject.getDouble(FenestrationSurface_DetailedFields::ViewFactortoGround);
  if (d) {
    subSurface->setViewFactortoGround(*d);
  }

  target = workspaceObject.getTarget(openstudio::FenestrationSurface_DetailedFields::ShadingControlName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject && modelObject->optionalCast<model::ShadingControl>()){
      subSurface->setShadingControl(modelObject->cast<model::ShadingControl>());
    }
  }

  target = workspaceObject.getTarget(openstudio::FenestrationSurface_DetailedFields::FrameandDividerName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject && modelObject->optionalCast<model::WindowPropertyFrameAndDivider>()){
      subSurface->setWindowPropertyFrameAndDivider(modelObject->cast<model::WindowPropertyFrameAndDivider>());
    }
  }

  // DLM: what about multipliers on interior sub surfaces?
  OptionalInt i = workspaceObject.getInt(FenestrationSurface_DetailedFields::Multiplier);
  if (i) {
    subSurface->setMultiplier(*i);
  }

  return subSurface.get();
}

} // energyplus

} // openstudio

