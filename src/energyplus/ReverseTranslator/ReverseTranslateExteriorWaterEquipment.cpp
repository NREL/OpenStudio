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

#include "../../model/ExteriorWaterEquipment.hpp"
#include "../../model/ExteriorWaterEquipment_Impl.hpp"
#include "../../model/ExteriorWaterEquipmentDefinition.hpp"
#include "../../model/ExteriorWaterEquipmentDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Exterior_WaterEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateExteriorWaterEquipment(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Exterior_WaterEquipment) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not of IddObjectType::Exterior_WaterEquipment.");
      return boost::none;
    }

    // Create an EquipmentDefinition to hold the Design Level
    model::ExteriorWaterEquipmentDefinition definition(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      definition.setName(*s + " Definition");
    }

    if ((d = workspaceObject.getDouble(Exterior_WaterEquipmentFields::DesignLevel))) {
      definition.setDesignLevel(*d);
    }

    model::OptionalExteriorWaterEquipment exteriorWaterEquipment;
    model::OptionalSchedule schedule;

    if (OptionalWorkspaceObject target = workspaceObject.getTarget(Exterior_WaterEquipmentFields::ScheduleName)) {
      if (model::OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target)) {
        schedule = modelObject->optionalCast<model::Schedule>();
      }
    }

    if (schedule) {
      try {
        exteriorWaterEquipment = model::ExteriorWaterEquipment(definition, *schedule);
      } catch (std::exception& e) {
        LOG(Warn, "Could not reverse translate " << workspaceObject.briefDescription() << " in full, because " << e.what() << ".");
      }
    }
    if (!exteriorWaterEquipment) {
      exteriorWaterEquipment = model::ExteriorWaterEquipment(definition);
    }

    OS_ASSERT(exteriorWaterEquipment);

    if ((s = workspaceObject.name())) {
      exteriorWaterEquipment->setName(*s);
    }

    // Fuel Use Type is always water, so it doesn't exist in OpenStudio
    if ((s = workspaceObject.getString(Exterior_WaterEquipmentFields::FuelUseType, false, true))) {
      if (!openstudio::istringEqual("Water", *s)) {
        LOG(Error, "The Fuel Use Type is not 'Water' for "
                     << workspaceObject.briefDescription() << " but " << *s
                     << " which is highly unexpected for a water equipment. OpenStudio will use 'Water' instead.");
      }
    }

    if ((s = workspaceObject.getString(Exterior_WaterEquipmentFields::EndUseSubcategory, false, true))) {
      exteriorWaterEquipment->setEndUseSubcategory(*s);
    }

    return *exteriorWaterEquipment;
  }

}  // namespace energyplus
}  // namespace openstudio
