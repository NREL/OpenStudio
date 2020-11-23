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

#include "../../model/ExteriorFuelEquipment.hpp"
#include "../../model/ExteriorFuelEquipment_Impl.hpp"
#include "../../model/ExteriorFuelEquipmentDefinition.hpp"
#include "../../model/ExteriorFuelEquipmentDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Exterior_FuelEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateExteriorFuelEquipment(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Exterior_FuelEquipment) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not of IddObjectType::Exterior_FuelEquipment.");
      return boost::none;
    }

    // Create an EquipmentDefinition to hold the Design Level
    model::ExteriorFuelEquipmentDefinition definition(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      definition.setName(*s + " Definition");
    }

    if ((d = workspaceObject.getDouble(Exterior_FuelEquipmentFields::DesignLevel))) {
      definition.setDesignLevel(*d);
    }

    model::OptionalExteriorFuelEquipment exteriorFuelEquipment;
    model::OptionalSchedule schedule;

    if (OptionalWorkspaceObject target = workspaceObject.getTarget(Exterior_FuelEquipmentFields::ScheduleName)) {
      if (model::OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target)) {
        schedule = modelObject->optionalCast<model::Schedule>();
      }
    }

    if (schedule) {
      try {
        exteriorFuelEquipment = model::ExteriorFuelEquipment(definition, *schedule);
      } catch (std::exception& e) {
        LOG(Warn, "Could not reverse translate " << workspaceObject.briefDescription() << " in full, because " << e.what() << ".");
      }
    }
    if (!exteriorFuelEquipment) {
      exteriorFuelEquipment = model::ExteriorFuelEquipment(definition);
    }

    OS_ASSERT(exteriorFuelEquipment);

    if ((s = workspaceObject.name())) {
      exteriorFuelEquipment->setName(*s);
    }

    if ((s = workspaceObject.getString(Exterior_FuelEquipmentFields::FuelUseType, false, true))) {
      exteriorFuelEquipment->setFuelType(*s);
    } else {
      LOG(Warn, "The Fuel Use Type (required) isn't set for " << workspaceObject.briefDescription()
                                                              << " while it is a required field with no default... Will default to Electricity")
      exteriorFuelEquipment->setFuelType("Electricity");
    }

    if ((s = workspaceObject.getString(Exterior_FuelEquipmentFields::EndUseSubcategory, false, true))) {
      exteriorFuelEquipment->setEndUseSubcategory(*s);
    }

    return *exteriorFuelEquipment;
  }

}  // namespace energyplus
}  // namespace openstudio
