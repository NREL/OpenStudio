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

#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateInternalMass(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::InternalMass) {
      LOG(Error, "WorkspaceObject is not IddObjectType: InternalMass");
      return boost::none;
    }

    // create the definition
    openstudio::model::InternalMassDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::InternalMassFields::ConstructionName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ConstructionBase>()) {
          definition.setConstruction(modelObject->cast<ConstructionBase>());
        }
      }
    }

    OptionalDouble d = workspaceObject.getDouble(openstudio::InternalMassFields::SurfaceArea);
    if (d) {
      definition.setSurfaceArea(*d);
    }

    // create the instance
    InternalMass internalMass(definition);

    s = workspaceObject.name();
    if (s) {
      internalMass.setName(*s);
    }

    target = workspaceObject.getTarget(openstudio::InternalMassFields::ZoneorZoneListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          internalMass.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          internalMass.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    return internalMass;
  }

}  // namespace energyplus

}  // namespace openstudio
