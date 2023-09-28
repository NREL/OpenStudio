/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SurfacePropertyLocalEnvironment.hpp"

#include "../../model/PlanarSurface.hpp"

#include "../../model/SurfacePropertySurroundingSurfaces.hpp"
#include "../../model/SurfacePropertyGroundSurfaces.hpp"

//#include "../../model/OutdoorAirNode.hpp"
//#include "../../model/OutdoorAirNode_Impl.hpp"

#include <utilities/idd/SurfaceProperty_LocalEnvironment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertyLocalEnvironment(model::SurfacePropertyLocalEnvironment& modelObject) {

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_LocalEnvironment, modelObject);

    // Exterior Surface Name: Optional Object
    if (boost::optional<PlanarSurface> _exteriorSurface = modelObject.exteriorSurface()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_exteriorSurface.get())) {
        idfObject.setString(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName, _owo->nameString());
      }
    }

    // External Shading Fraction Schedule Name: Optional Object
    if (boost::optional<Schedule> _externalShadingFractionSchedule = modelObject.externalShadingFractionSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_externalShadingFractionSchedule.get())) {
        idfObject.setString(SurfaceProperty_LocalEnvironmentFields::SunlitFractionScheduleName, _owo->nameString());
      }
    }

    // Surrounding Surfaces Object Name: Optional Object
    if (boost::optional<SurfacePropertySurroundingSurfaces> sp_ = modelObject.surfacePropertySurroundingSurfaces()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sp_.get())) {
        idfObject.setString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName, _owo->nameString());
      }
    }

    // Ground Surfaces Object Name: Optional Object
    if (boost::optional<SurfacePropertyGroundSurfaces> sp_ = modelObject.surfacePropertyGroundSurfaces()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(sp_.get())) {
        idfObject.setString(SurfaceProperty_LocalEnvironmentFields::GroundSurfacesObjectName, _owo->nameString());
      }
    }

    // Outdoor Air Node Name: Optional Object
    // if (boost::optional<OutdoorAirNode> _outdoorAirNode = modelObject.outdoorAirNode()) {
    //   if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_outdoorAirNode.get())) {
    //     idfObject.setString(SurfaceProperty_LocalEnvironmentFields::OutdoorAirNodeName, _owo->nameString());
    //   }
    // }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
