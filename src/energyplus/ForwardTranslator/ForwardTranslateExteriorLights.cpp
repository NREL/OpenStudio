/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Exterior_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExteriorLights(model::ExteriorLights& modelObject) {
    IdfObject idfObject(IddObjectType::Exterior_Lights);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Exterior_LightsFields::Name, modelObject.name().get());

    OptionalIdfObject relatedIdfObject;

    boost::optional<model::Schedule> schedule;
    if (auto t_schedule = modelObject.schedule()) {
      schedule = t_schedule.get();
    } else {
      schedule = modelObject.model().alwaysOnDiscreteSchedule();
    }
    OS_ASSERT(schedule);
    relatedIdfObject = translateAndMapModelObject(schedule.get());
    OS_ASSERT(relatedIdfObject);
    idfObject.setString(Exterior_LightsFields::ScheduleName, relatedIdfObject->name().get());

    model::ExteriorLightsDefinition definition = modelObject.exteriorLightsDefinition();

    double designLevel = definition.designLevel() * modelObject.multiplier();
    idfObject.setDouble(Exterior_LightsFields::DesignLevel, designLevel);

    if (!modelObject.isControlOptionDefaulted()) {
      idfObject.setString(Exterior_LightsFields::ControlOption, modelObject.controlOption());
    }

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(Exterior_LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
