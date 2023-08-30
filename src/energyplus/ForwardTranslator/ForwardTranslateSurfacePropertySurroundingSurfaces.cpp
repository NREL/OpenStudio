/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SurfacePropertySurroundingSurfaces.hpp"

#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_SurroundingSurfaces_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertySurroundingSurfaces(model::SurfacePropertySurroundingSurfaces& modelObject) {

    if (modelObject.numberofSurroundingSurfaceGroups() == 0) {
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_SurroundingSurfaces, modelObject);

    // Sky View Factor: Optional Double
    if (boost::optional<double> skyViewFactor_ = modelObject.skyViewFactor()) {
      idfObject.setDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, skyViewFactor_.get());
    }

    // Sky Temperature Schedule Name: Optional Object
    if (boost::optional<Schedule> _skyTemperatureSchedule = modelObject.skyTemperatureSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_skyTemperatureSchedule.get())) {
        idfObject.setString(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName, _owo->nameString());
      }
    }

    // Ground View Factor: Optional Double
    if (boost::optional<double> groundViewFactor_ = modelObject.groundViewFactor()) {
      idfObject.setDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, groundViewFactor_.get());
    }

    // Ground Temperature Schedule Name: Optional Object
    if (boost::optional<Schedule> _groundTemperatureSchedule = modelObject.groundTemperatureSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_groundTemperatureSchedule.get())) {
        idfObject.setString(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName, _owo->nameString());
      }
    }

    for (const auto& group : modelObject.surroundingSurfaceGroups()) {

      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName, group.surroundingSurfaceName());
      eg.setDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor, group.viewFactor());
      auto sch = group.temperatureSchedule();
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sch)) {
        eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, _owo->nameString());
      }  // TODO: handle failure?
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
