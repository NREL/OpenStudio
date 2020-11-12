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

#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/OtherEquipmentDefinition.hpp"
#include "../../model/OtherEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/OtherEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOtherEquipment(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::OtherEquipment) {
      LOG(Error, "WorkspaceObject is not IddObjectType::OtherEquipment");
      return boost::none;
    }

    LOG(Info, "OtherEquipment is an internal load separate from any other system, and not "
                << "associated with an end use subcategory. Use with caution.");

    // create the definition
    openstudio::model::OtherEquipmentDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    s = workspaceObject.getString(openstudio::OtherEquipmentFields::DesignLevelCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("EquipmentLevel", *s)) {
      d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::DesignLevel);
      if (d) {
        if (d.get() < 0.0) {
          LOG(Warn, "OtherEquipment has negative energy use. Advanced feature--use with caution.");
        }
        definition.setDesignLevel(*d);
      } else {
        LOG(Error, "EquipmentLevel value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::PowerperZoneFloorArea);
      if (d) {
        if (d.get() < 0.0) {
          LOG(Warn, "OtherEquipment has negative energy use. Advanced feature--use with caution.");
        }
        definition.setWattsperSpaceFloorArea(*d);
      } else {
        LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::PowerperPerson);
      if (d) {
        if (d.get() < 0.0) {
          LOG(Warn, "OtherEquipment has negative energy use. Advanced feature--use with caution.");
        }
        definition.setWattsperPerson(*d);
      } else {
        LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::FractionLatent);
    if (d) {
      definition.setFractionLatent(*d);
    }

    d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::FractionRadiant);
    if (d) {
      definition.setFractionRadiant(*d);
    }

    d = workspaceObject.getDouble(openstudio::OtherEquipmentFields::FractionLost);
    if (d) {
      definition.setFractionLost(*d);
    }

    // create the instance
    OtherEquipment otherEquipment(definition);

    s = workspaceObject.name();
    if (s) {
      otherEquipment.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::OtherEquipmentFields::ZoneorZoneListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          otherEquipment.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          otherEquipment.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::OtherEquipmentFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          otherEquipment.setSchedule(schedule);
        }
      }
    }

    s = workspaceObject.getString(openstudio::OtherEquipmentFields::EndUseSubcategory);
    if (s) {
      otherEquipment.setEndUseSubcategory(*s);
    }

    return otherEquipment;
  }

}  // namespace energyplus

}  // namespace openstudio
