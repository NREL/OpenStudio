/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

boost::optional<IdfObject> ForwardTranslator::translateGeneratorPhotovoltaic(model::GeneratorPhotovoltaic & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Generator_Photovoltaic, modelObject);

  PhotovoltaicPerformance performance = modelObject.photovoltaicPerformance();
  boost::optional<IdfObject> performanceIdf = translateAndMapModelObject(performance);
  if (performanceIdf){
    idfObject.setString(Generator_PhotovoltaicFields::PhotovoltaicPerformanceObjectType, performanceIdf->iddObject().name());
    idfObject.setString(Generator_PhotovoltaicFields::ModulePerformanceName, performanceIdf->name().get());
  } else{
    LOG(Warn, "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Module Performance Name'")
  }

  boost::optional<PlanarSurface> surface = modelObject.surface();
  bool hasSurface = false;
  if (surface){
    boost::optional<IdfObject> surfaceIdf = translateAndMapModelObject(*surface);
    if (surfaceIdf){
      idfObject.setString(Generator_PhotovoltaicFields::SurfaceName, surfaceIdf->name().get());
      hasSurface = true;
    }
  }
  if (!hasSurface){
    LOG(Warn, "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Surface Name'")
  }

  idfObject.setString(Generator_PhotovoltaicFields::HeatTransferIntegrationMode, modelObject.heatTransferIntegrationMode());

  idfObject.setDouble(Generator_PhotovoltaicFields::NumberofModulesinParallel, modelObject.numberOfModulesInParallel());
  
  idfObject.setDouble(Generator_PhotovoltaicFields::NumberofModulesinSeries, modelObject.numberOfModulesInSeries());

  return idfObject;
}

} // energyplus

} // openstudio

