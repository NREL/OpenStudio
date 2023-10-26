/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SurfacePropertyGroundSurfaces.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_GroundSurfaces_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertyGroundSurfaces(model::SurfacePropertyGroundSurfaces& modelObject) {
    boost::optional<IdfObject> result;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_GroundSurfaces, modelObject);

    for (const auto& group : modelObject.groundSurfaceGroups()) {

      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName, group.groundSurfaceName());
      eg.setDouble(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor, group.viewFactor());
      if (auto sch_ = group.temperatureSchedule()) {
        if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sch_.get())) {
          eg.setString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName, _owo->nameString());
        }  // TODO: handle failure?
      }
      if (auto sch_ = group.reflectanceSchedule()) {
        if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sch_.get())) {
          eg.setString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName, _owo->nameString());
        }  // TODO: handle failure?
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
