/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorPVWatts.hpp"
#include "../../model/GeneratorPVWatts_Impl.hpp"
#include "../../model/PlanarSurface.hpp"

#include <utilities/idd/Generator_PVWatts_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorPVWatts(model::GeneratorPVWatts& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Generator_PVWatts, modelObject);

    idfObject.setString(Generator_PVWattsFields::PVWattsVersion, modelObject.pvWattsVersion());

    idfObject.setDouble(Generator_PVWattsFields::DCSystemCapacity, modelObject.dcSystemCapacity());

    idfObject.setString(Generator_PVWattsFields::ModuleType, modelObject.moduleType());

    idfObject.setString(Generator_PVWattsFields::ArrayType, modelObject.arrayType());

    idfObject.setDouble(Generator_PVWattsFields::SystemLosses, modelObject.systemLosses());

    boost::optional<PlanarSurface> surface = modelObject.surface();
    if (surface) {
      boost::optional<IdfObject> surfaceIdf = translateAndMapModelObject(*surface);
      if (surfaceIdf) {
        idfObject.setString(Generator_PVWattsFields::ArrayGeometryType, "Surface");
        idfObject.setString(Generator_PVWattsFields::SurfaceName, surfaceIdf->name().get());
      }
    } else {
      idfObject.setString(Generator_PVWattsFields::ArrayGeometryType, "TiltAzimuth");
      idfObject.setDouble(Generator_PVWattsFields::TiltAngle, modelObject.tiltAngle());
      idfObject.setDouble(Generator_PVWattsFields::AzimuthAngle, modelObject.azimuthAngle());
    }

    idfObject.setDouble(Generator_PVWattsFields::GroundCoverageRatio, modelObject.groundCoverageRatio());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
