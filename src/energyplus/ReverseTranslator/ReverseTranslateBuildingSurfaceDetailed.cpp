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

#include "../ReverseTranslator.hpp"
#include "../GeometryTranslator.hpp"

#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateBuildingSurfaceDetailed(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::BuildingSurface_Detailed) {
      LOG(Error, "WorkspaceObject is not IddObjectType: BuildingSurface:Detailed");
      return boost::none;
    }

    openstudio::Point3dVector vertices = getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, workspaceObject);

    boost::optional<Surface> surface;
    try {
      surface = Surface(vertices, m_model);
    } catch (const std::exception&) {
      LOG(Error, "Cannot create Surface for object: " << workspaceObject);
      return boost::none;
    }

    OptionalString s = workspaceObject.name();
    if (s) {
      surface->setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::BuildingSurface_DetailedFields::ConstructionName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ConstructionBase>()) {
          surface->setConstruction(modelObject->cast<ConstructionBase>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::BuildingSurface_DetailedFields::ZoneName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          surface->setSpace(modelObject->cast<Space>());
        }
      }
    }

    s = workspaceObject.getString(BuildingSurface_DetailedFields::SurfaceType);
    if (s) {
      if (istringEqual("Roof", *s) || istringEqual("Ceiling", *s)) {
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
    if (target) {

      if (target->iddObject().type() == IddObjectType::Zone) {
        // Zone boundary condition

        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
        if (modelObject->optionalCast<Space>()) {
          Space adjacentSpace = modelObject->cast<Space>();

          if (surface->space()) {
            // insert this surface in the map so subsurface translation can find it
            m_workspaceToModelMap.insert(std::make_pair(workspaceObject.handle(), surface.get()));

            // need to translate all sub surfaces here so they will be in adjacent space
            for (const WorkspaceObject& workspaceSubSurface : workspaceObject.getSources(IddObjectType::FenestrationSurface_Detailed)) {
              translateAndMapWorkspaceObject(workspaceSubSurface);
            }

            // create adjacent surface in other space
            surface->createAdjacentSurface(adjacentSpace);
            return surface.get();
          }
        }

      } else if (target->iddObject().type() == IddObjectType::BuildingSurface_Detailed) {
        // Surface boundary condition

        // see if we have already mapped other surface, don't do it here because that is circular
        if (target->handle() == workspaceObject.handle()) {
          // these objects are the same, set boundary condition to adiabatic
          surface->setOutsideBoundaryCondition("Adiabatic");
          return surface.get();
        } else {
          auto it = m_workspaceToModelMap.find(target->handle());
          if (it != m_workspaceToModelMap.end()) {
            if (it->second.optionalCast<Surface>()) {
              // this will set other side boundary object on both surfaces
              Surface adjacentSurface = it->second.cast<Surface>();
              surface->setAdjacentSurface(adjacentSurface);
              return surface.get();
            }
          }
        }

      } else if (target->iddObject().type() == IddObjectType::Foundation_Kiva) {
        // Foundation boundary condition

        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
        if (modelObject->optionalCast<FoundationKiva>()) {
          FoundationKiva foundationKiva = modelObject->cast<FoundationKiva>();
          surface->setAdjacentFoundation(foundationKiva);
          return surface.get();
        }

      } else {
        LOG(Error, "OutsideBoundaryConditionObject not yet mapped for object of type " << target->iddObject().name());
      }
    }

    s = workspaceObject.getString(BuildingSurface_DetailedFields::OutsideBoundaryCondition);
    if (s) {
      surface->setOutsideBoundaryCondition(*s);
    }

    return surface.get();
  }

}  // namespace energyplus

}  // namespace openstudio
