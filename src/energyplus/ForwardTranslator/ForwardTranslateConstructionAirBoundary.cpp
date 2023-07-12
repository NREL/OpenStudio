/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ConstructionAirBoundary.hpp"
#include "../../model/ConstructionAirBoundary_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Construction_AirBoundary_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstructionAirBoundary(ConstructionAirBoundary& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Construction_AirBoundary);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    idfObject.setName(modelObject.name().get());

    idfObject.setString(Construction_AirBoundaryFields::AirExchangeMethod, modelObject.airExchangeMethod());

    idfObject.setDouble(Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, modelObject.simpleMixingAirChangesPerHour());

    boost::optional<Schedule> schedule = modelObject.simpleMixingSchedule();
    if (schedule) {
      boost::optional<IdfObject> relatedIdfObject = translateAndMapModelObject(*schedule);
      OS_ASSERT(relatedIdfObject);
      idfObject.setString(Construction_AirBoundaryFields::SimpleMixingScheduleName, relatedIdfObject->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
