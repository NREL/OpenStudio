/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
