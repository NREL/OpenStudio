/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ConstructionAirBoundary.hpp"
#include "../../model/ConstructionAirBoundary_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_AirBoundary_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateConstructionAirBoundary(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Construction_AirBoundary) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Construction:AirBoundary");
      return boost::none;
    }

    ConstructionAirBoundary constructionAirBoundary(m_model);

    OptionalString optS = workspaceObject.name();
    if (optS) {
      constructionAirBoundary.setName(*optS);
    }

    optS = workspaceObject.getString(Construction_AirBoundaryFields::AirExchangeMethod);
    if (optS) {
      constructionAirBoundary.setAirExchangeMethod(*optS);
    }

    boost::optional<double> optD = workspaceObject.getDouble(Construction_AirBoundaryFields::SimpleMixingAirChangesperHour);
    if (optD) {
      constructionAirBoundary.setSimpleMixingAirChangesPerHour(*optD);
    }

    boost::optional<WorkspaceObject> target = workspaceObject.getTarget(Construction_AirBoundaryFields::SimpleMixingAirChangesperHour);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          constructionAirBoundary.setSimpleMixingSchedule(schedule);
        }
      }
    }

    return constructionAirBoundary;
  }

}  // namespace energyplus

}  // namespace openstudio
