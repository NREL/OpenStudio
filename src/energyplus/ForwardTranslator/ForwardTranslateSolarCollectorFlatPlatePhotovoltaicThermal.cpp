/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/PlanarSurface.hpp"
#include "../../model/PlanarSurface_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/GeneratorPhotovoltaic.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
#include "../../model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/SolarCollector_FlatPlate_PhotovoltaicThermal_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSolarCollectorFlatPlatePhotovoltaicThermal(SolarCollectorFlatPlatePhotovoltaicThermal& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::SolarCollector_FlatPlate_PhotovoltaicThermal);

    m_idfObjects.push_back(idfObject);

    // Name
    std::string name = modelObject.name().get();
    idfObject.setName(name);

    // SolarCollectorPerformanceFlatPlate
    {
      auto mo = modelObject.solarCollectorPerformance();
      IdfObject performance = translateAndMapModelObject(mo).get();
      idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName, performance.name().get());
    }

    // SurfaceName
    OptionalPlanarSurface surface = modelObject.surface();
    if (surface) {
      IdfObject surf = translateAndMapModelObject(*surface).get();
      idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, surf.name().get());
    } else {
      LOG(Error, "SolarCollector:FlatPlate:PhotovoltaicThermal '" << name << "' does not reference a surface.");
    }

    // PhotovoltaicName
    OptionalGeneratorPhotovoltaic generatorPhotovoltaic = modelObject.generatorPhotovoltaic();
    if (generatorPhotovoltaic) {
      // generator will be translated with the other generators
      idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, generatorPhotovoltaic->name().get());
    } else {
      LOG(Warn, "SolarCollector:FlatPlate:PhotovoltaicThermal '" << name << "' does not reference a GeneratorPhotovoltaic.");
    }

    // figure out if this is on an air loop or plant loop
    boost::optional<AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC();
    boost::optional<PlantLoop> plantLoop = modelObject.plantLoop();

    // ThermalWorkingFluidType
    if (airLoopHVAC) {
      idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::ThermalWorkingFluidType, "Air");
    } else if (plantLoop) {
      idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::ThermalWorkingFluidType, "Water");
    }

    // InletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if (airLoopHVAC) {
          idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::AirInletNodeName, node->name().get());
        } else if (plantLoop) {
          idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::WaterInletNodeName, node->name().get());
        }
      }
    }

    // OutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if (airLoopHVAC) {
          idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::AirOutletNodeName, node->name().get());
        } else if (plantLoop) {
          idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::WaterOutletNodeName, node->name().get());
        }
      }
    }

    // DesignFlowRate
    boost::optional<double> designFlowRate = modelObject.designFlowRate();
    if (designFlowRate) {
      idfObject.setDouble(SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, *designFlowRate);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
