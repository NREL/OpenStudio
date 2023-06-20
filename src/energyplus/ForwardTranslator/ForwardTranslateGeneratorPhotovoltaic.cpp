/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorPhotovoltaic.hpp"
#include "../../model/GeneratorPhotovoltaic_Impl.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/PhotovoltaicPerformance.hpp"

#include <utilities/idd/Generator_Photovoltaic_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorPhotovoltaic(model::GeneratorPhotovoltaic& modelObject) {

    // We are going to check for required properties such as Surface before we register the object
    // IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Generator_Photovoltaic, modelObject);
    IdfObject idfObject(openstudio::IddObjectType::Generator_Photovoltaic);
    // Name
    idfObject.setName(modelObject.nameString());

    PhotovoltaicPerformance performance = modelObject.photovoltaicPerformance();
    boost::optional<IdfObject> performanceIdf = translateAndMapModelObject(performance);
    if (performanceIdf) {
      idfObject.setString(Generator_PhotovoltaicFields::PhotovoltaicPerformanceObjectType, performanceIdf->iddObject().name());
      idfObject.setString(Generator_PhotovoltaicFields::ModulePerformanceName, performanceIdf->name().get());
    } else {
      LOG(Error,
          "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Module Performance Name', it will not be translated.");
      return boost::none;
    }

    boost::optional<PlanarSurface> surface = modelObject.surface();
    bool hasSurface = false;
    if (surface) {
      boost::optional<IdfObject> surfaceIdf = translateAndMapModelObject(*surface);
      if (surfaceIdf) {
        idfObject.setString(Generator_PhotovoltaicFields::SurfaceName, surfaceIdf->name().get());
        hasSurface = true;
      }
    }
    if (!hasSurface) {
      LOG(Error, "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Surface Name', it will not be translated.");
      return boost::none;
    }

    // at this point, we can register the new object
    m_idfObjects.push_back(idfObject);

    idfObject.setString(Generator_PhotovoltaicFields::HeatTransferIntegrationMode, modelObject.heatTransferIntegrationMode());

    idfObject.setDouble(Generator_PhotovoltaicFields::NumberofSeriesStringsinParallel, modelObject.numberOfModulesInParallel());

    idfObject.setDouble(Generator_PhotovoltaicFields::NumberofModulesinSeries, modelObject.numberOfModulesInSeries());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
