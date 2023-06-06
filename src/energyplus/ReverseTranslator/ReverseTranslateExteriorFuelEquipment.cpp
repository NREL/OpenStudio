/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
