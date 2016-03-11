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
#include "../../model/PlanarSurface.hpp"
#include "../../model/PlanarSurface_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/GeneratorPhotovoltaic.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
#include "../../model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
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

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorFlatPlatePhotovoltaicThermal(SolarCollectorFlatPlatePhotovoltaicThermal & modelObject)
{
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
  if (surface){
    IdfObject surf = translateAndMapModelObject(*surface).get();
    idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, surf.name().get());
  } else{
    LOG(Error, "SolarCollector:FlatPlate:PhotovoltaicThermal '" << name << "' does not reference a surface.");
  }

  // PhotovoltaicName
  OptionalGeneratorPhotovoltaic generatorPhotovoltaic = modelObject.generatorPhotovoltaic();
  if (generatorPhotovoltaic){
    // generator will be translated with the other generators
    idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, generatorPhotovoltaic->name().get());
  } else{
    LOG(Warn, "SolarCollector:FlatPlate:PhotovoltaicThermal '" << name << "' does not reference a GeneratorPhotovoltaic.");
  }

  // figure out if this is on an air loop or plant loop
  boost::optional<AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC();
  boost::optional<PlantLoop> plantLoop = modelObject.plantLoop();

  // ThermalWorkingFluidType
  if (airLoopHVAC){
    idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::ThermalWorkingFluidType, "Air");
  } else if (plantLoop){
    idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::ThermalWorkingFluidType, "Water");
  }

  // InletNodeName
  if (boost::optional<ModelObject> mo = modelObject.inletModelObject())
  {
    if (boost::optional<Node> node = mo->optionalCast<Node>())
    {
      if (airLoopHVAC){
        idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::AirInletNodeName, node->name().get());
      } else if (plantLoop){
        idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::WaterInletNodeName, node->name().get());
      }
    }
  }

  // OutletNodeName
  if (boost::optional<ModelObject> mo = modelObject.outletModelObject())
  {
    if (boost::optional<Node> node = mo->optionalCast<Node>())
    {
      if (airLoopHVAC){
        idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::AirOutletNodeName, node->name().get());
      } else if (plantLoop){
        idfObject.setString(SolarCollector_FlatPlate_PhotovoltaicThermalFields::WaterOutletNodeName, node->name().get());
      }
    }
  }

  // DesignFlowRate
  boost::optional<double> designFlowRate = modelObject.designFlowRate();
  if (designFlowRate){
    idfObject.setDouble(SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, *designFlowRate);
  }

  return idfObject;
}

} // energyplus

} // openstudio

