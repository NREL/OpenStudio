/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/PlanarSurface_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/SolarCollectorFlatPlateWater.hpp"
#include "../../model/SolarCollectorFlatPlateWater_Impl.hpp"
#include "../../model/SolarCollectorPerformanceFlatPlate.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/SolarCollector_FlatPlate_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorFlatPlateWater(SolarCollectorFlatPlateWater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::SolarCollector_FlatPlate_Water);

    m_idfObjects.push_back(idfObject);

    // Name
    std::string name = modelObject.name().get();
    idfObject.setName(name);

    // SolarCollectorPerformanceFlatPlate
    {
      auto mo = modelObject.solarCollectorPerformance();
      IdfObject performance = translateAndMapModelObject(mo).get();
      idfObject.setString(SolarCollector_FlatPlate_WaterFields::SolarCollectorPerformanceName, performance.name().get());
    }

    OptionalPlanarSurface surface = modelObject.surface();
    if (surface) {
      IdfObject surf = translateAndMapModelObject(*surface).get();
      idfObject.setString(SolarCollector_FlatPlate_WaterFields::SurfaceName, surf.name().get());
    } else {
      LOG(Error, "SolarCollector:FlatPlate:Water '" << name << "' does not reference a surface.");
    }

    // InletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(SolarCollector_FlatPlate_WaterFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(SolarCollector_FlatPlate_WaterFields::OutletNodeName, node->name().get());
      }
    }

    // MaximumFlowRate
    boost::optional<double> maximumFlowRate = modelObject.maximumFlowRate();
    if (maximumFlowRate) {
      idfObject.setDouble(SolarCollector_FlatPlate_WaterFields::MaximumFlowRate, *maximumFlowRate);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
