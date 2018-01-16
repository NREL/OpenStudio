/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

