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
#include "../../model/SolarCollectorIntegralCollectorStorage.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage_Impl.hpp"
#include "../../model/SolarCollectorPerformanceIntegralCollectorStorage.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/SolarCollector_IntegralCollectorStorage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorIntegralCollectorStorage(SolarCollectorIntegralCollectorStorage & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::SolarCollector_IntegralCollectorStorage);

  m_idfObjects.push_back(idfObject);

  // Name
  std::string name = modelObject.name().get();
  idfObject.setName(name);

  // SolarCollectorPerformanceIntegralCollectorStorage
  {
    auto mo = modelObject.solarCollectorPerformance();
    IdfObject performance = translateAndMapModelObject(mo).get();
    idfObject.setString(SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName, performance.name().get());
  }

  // SurfaceName
  OptionalPlanarSurface surface = modelObject.surface();
  if (surface){
    IdfObject surf = translateAndMapModelObject(*surface).get();
    idfObject.setString(SolarCollector_IntegralCollectorStorageFields::SurfaceName, surf.name().get());
  } else{
    LOG(Error, "SolarCollector:IntegralCollectorStorage '" << name << "' does not reference a surface.");
  }

  // BottomSurfaceBoundaryConditionsType
  idfObject.setString(SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType, modelObject.bottomSurfaceBoundaryConditionsType());

  // BoundaryConditionModelName
  // DLM: not implemented

  // InletNodeName
  if (boost::optional<ModelObject> mo = modelObject.inletModelObject())
  {
    if (boost::optional<Node> node = mo->optionalCast<Node>())
    {
      idfObject.setString(SolarCollector_IntegralCollectorStorageFields::InletNodeName, node->name().get());
    }
  }

  // OutletNodeName
  if (boost::optional<ModelObject> mo = modelObject.outletModelObject())
  {
    if (boost::optional<Node> node = mo->optionalCast<Node>())
    {
      idfObject.setString(SolarCollector_IntegralCollectorStorageFields::OutletNodeName, node->name().get());
    }
  }

  // MaximumFlowRate
  boost::optional<double> maximumFlowRate = modelObject.maximumFlowRate();
  if (maximumFlowRate){
    idfObject.setDouble(SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate, *maximumFlowRate);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

