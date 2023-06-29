/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SurfacePropertyIncidentSolarMultiplier.hpp"

#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/SurfaceProperty_IncidentSolarMultiplier_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSurfacePropertyIncidentSolarMultiplier(model::SurfacePropertyIncidentSolarMultiplier& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::SurfaceProperty_IncidentSolarMultiplier, modelObject);

    // Surface Name: Required Object
    SubSurface subSurface = modelObject.subSurface();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(subSurface)) {
      idfObject.setString(SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName, _owo->nameString());
    }

    // Incident Solar Multiplier: Optional Double
    idfObject.setDouble(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier, modelObject.incidentSolarMultiplier());

    // Incident Solar Multiplier Schedule Name: Optional Object
    if (boost::optional<Schedule> _incidentSolarMultiplierSchedule = modelObject.incidentSolarMultiplierSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_incidentSolarMultiplierSchedule.get())) {
        idfObject.setString(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName, _owo->nameString());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
