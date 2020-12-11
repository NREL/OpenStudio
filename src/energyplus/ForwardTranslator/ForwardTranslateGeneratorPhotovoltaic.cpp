/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Generator_Photovoltaic, modelObject);

    PhotovoltaicPerformance performance = modelObject.photovoltaicPerformance();
    boost::optional<IdfObject> performanceIdf = translateAndMapModelObject(performance);
    if (performanceIdf) {
      idfObject.setString(Generator_PhotovoltaicFields::PhotovoltaicPerformanceObjectType, performanceIdf->iddObject().name());
      idfObject.setString(Generator_PhotovoltaicFields::ModulePerformanceName, performanceIdf->name().get());
    } else {
      LOG(Warn, "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Module Performance Name'")
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
      LOG(Warn, "Generator:Photovoltaic '" << idfObject.name().get() << "' missing required field 'Surface Name'")
    }

    idfObject.setString(Generator_PhotovoltaicFields::HeatTransferIntegrationMode, modelObject.heatTransferIntegrationMode());

    idfObject.setDouble(Generator_PhotovoltaicFields::NumberofSeriesStringsinParallel, modelObject.numberOfModulesInParallel());

    idfObject.setDouble(Generator_PhotovoltaicFields::NumberofModulesinSeries, modelObject.numberOfModulesInSeries());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
